/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright © 2024-2026, Synopsys Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials provided
 *    with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include "uart_8250.h"

#define read_aux_reg(r)     __builtin_arc_lr(r)
#define write_aux_reg(v, r) __builtin_arc_sr((unsigned int)(v), r)

/*
 * List of UART 8250 registers with offsets:
 *
 *     0x00 - Transmit Holding Register, WO, LCR_DLAB == 0
 *     0x00 - Receive Buffer Register, RO, LCR_DLAB == 0
 *     0x00 - Divisor Latch Low, RW, LCR_DLAB == 1
 *     0x04 - Divisor Latch High, RW, LCR_DLAB == 1
 *     0x04 - Interrupt Enable Register, RW, LCR_DLAB == 0
 *     0x08 - FIFO Control Register, WO
 *     0x08 - Interrupt Identification Register, RO
 *     0x0C - Line Control Register, RW
 *     0x10 - Modem Control Register, RW
 *     0x14 - Line Status Register, RO
 *     0x18 - Modem Status Register, RO
 */

#define THR_OFFSET 0x00
#define RBR_OFFSET 0x00
#define DLL_OFFSET 0x00
#define DLH_OFFSET 0x04
#define IER_OFFSET 0x04
#define FCR_OFFSET 0x08
#define IIR_OFFSET 0x08
#define LCR_OFFSET 0x0C
#define MCR_OFFSET 0x10
#define LSR_OFFSET 0x14
#define MSR_OFFSET 0x18

/*
 * LCR (Line Control Register) fields:
 *
 *       [7] - Divisor Latch Access Bit
 *       [6] - Set Break
 *       [5] - Stick Parity
 *       [4] - Even Parity Select
 *       [3] - Parity Enable
 *       [2] - Number of Stop Bits
 *     [1:0] - Data Length Select. Values:
 *                 0b11 - 8 data bits per character (default)
 *                 0b10 - 7 data bits per character
 *                 0b01 - 6 data bits per character
 *                 0b00 - 5 data bits per character
 */

#define LCR_DLAB_SHIFT 7
#define LCR_SB_SHIFT   6
#define LCR_SP_SHIFT   5
#define LCR_EPS_SHIFT  4
#define LCR_PEN_SHIFT  3
#define LCR_STB_SHIFT  2
#define LCR_DLS_SHIFT  0
/*
 * MCR (Modem Control Register) fields:
 *
 *     [4] - LoopBack Bit
 *     [3] - Auxiliary Output 2
 *     [2] - Auxiliary Output 1
 *     [1] - Request To Send (ON by default)
 *     [0] - Data Terminal Ready (ON by default)
 */

#define MCR_LB_SHIFT   4
#define MCR_OUT2_SHIFT 3
#define MCR_OUT1_SHIFT 2
#define MCR_RTS_SHIFT  1
#define MCR_DTR_SHIFT  0

/*
 * LSR (Line Status Register) fields:
 *
 *     [7] - Receiver FIFO Error
 *     [6] - Transmitter Empty
 *     [5] - Transmit Holding Register Empty
 *     [4] - Break Interrupt
 *     [3] - Framing Error
 *     [2] - Parity Error
 *     [1] - Overrun Error
 *     [0] - Data Ready
 */

#define LSR_RFE_SHIFT  7
#define LSR_TEMT_SHIFT 6
#define LSR_THRE_SHIFT 5
#define LSR_BI_SHIFT   4
#define LSR_FE_SHIFT   3
#define LSR_PE_SHIFT   2
#define LSR_OE_SHIFT   1
#define LSR_DR_SHIFT   0

/*
 * Default initial values for configuration registers.
 */

#define FCR_DEFAULT     0x0
#define IER_DEFAULT     0x0
#define LCR_DLS_DEFAULT 0x3
#define LCR_DEFAULT     (LCR_DLS_DEFAULT << LCR_DLS_SHIFT)
#define MCR_RTS_DEFAULT 0x1
#define MCR_DTR_DEFAULT 0x1
#define MCR_DEFAULT     ((MCR_RTS_DEFAULT << MCR_RTS_SHIFT) | (MCR_DTR_DEFAULT << MCR_DTR_SHIFT))

#define LCR_MODE_SETUP  (0x1 << LCR_DLAB_SHIFT)
#define LSR_MODE_GETC   (0x1 << LSR_DR_SHIFT)
#define LSR_MODE_PUTC   ((0x1 << LSR_TEMT_SHIFT) | (0x1 << LSR_THRE_SHIFT))

/* Main UART control structure */
struct _uart_8250 {
    volatile char *base;       /* Start of UART registers */
    uint32_t       clk;        /* UART clock */
    uint32_t       baud;       /* Baud rate */
    int            aux_mapped; /* If UART registers are mapped to AUX or memory */
    int            ready;      /* If UART is ready to use or not */
};

static struct _uart_8250 _uart_8250
    = { .base = "", .clk = 0, .baud = 0, .aux_mapped = 0, .ready = 0 };

/* Write 32-bit value to the UART register */
static inline void
_uart_8250_write_reg(const struct _uart_8250 *uart, uint32_t reg, uint32_t value)
{
    if (uart->aux_mapped)
        write_aux_reg(value, (uint32_t)uart->base + reg);
    else
        *(volatile uint32_t *)(uart->base + reg) = value;
}

/* Read 32-bit value from the UART register */
static inline uint32_t
_uart_8250_read_reg(const struct _uart_8250 *uart, uint32_t reg)
{
    if (uart->aux_mapped)
        return read_aux_reg((uint32_t)uart->base + reg);
    else
        return *(volatile uint32_t *)(uart->base + reg);
}

/* Wait until all flags are set.  */
static inline void
_uart_8250_wait(const struct _uart_8250 *uart, uint32_t reg, uint32_t flags)
{
    while (1) {
        if ((_uart_8250_read_reg(uart, reg) & flags) == flags)
            break;
    }
}

/* Get one character from UART.  CR is converted to NL */
static int
_uart_8250_getc(const struct _uart_8250 *uart)
{
    char c;

    _uart_8250_wait(uart, LSR_OFFSET, LSR_MODE_GETC);

    c = _uart_8250_read_reg(uart, RBR_OFFSET);

    if (c == '\r')
        c = '\n';

    return c;
}

/* Put one character to UART. CR is placed before NL */
static void
_uart_8250_putc(const struct _uart_8250 *uart, char c)
{
    if (c == '\n')
        _uart_8250_putc(uart, '\r');

    _uart_8250_wait(uart, LSR_OFFSET, LSR_MODE_PUTC);

    _uart_8250_write_reg(uart, THR_OFFSET, c);
}

/*
 * Setup UART control structure and following parameters:
 *
 *     - baudrate if clock and baudrate are passed to function
 *     - 8n1 (8 data bits, no parity bit, one stop bit)
 *     - disable interrupts
 *     - disable FIFO
 *     - set Request To Send and Data Terminal Ready
 *
 * Arguments:
 *
 *     - base - start address of UART registers
 *     - aux_mapped - set if UART registers are mapped to ARC AUX
 *     - clk - UART clock frequency
 *     - baud - UART baudrate to setup
 *
 * The function returns 0 on success.
 */
int
_uart_8250_setup(void *base, int aux_mapped, uint32_t clk, uint32_t baud)
{
    struct _uart_8250 *uart = &_uart_8250;

    uart->base = base;
    uart->aux_mapped = aux_mapped;
    uart->clk = clk;
    uart->baud = baud;

    if (clk && baud) {
        uint32_t div;

        div = ((clk + 8 * baud) / baud) / 16;
        _uart_8250_write_reg(uart, LCR_OFFSET, LCR_MODE_SETUP);
        _uart_8250_write_reg(uart, DLL_OFFSET, div & 0xFF);
        _uart_8250_write_reg(uart, DLH_OFFSET, div >> 8);
    }

    _uart_8250_write_reg(uart, FCR_OFFSET, FCR_DEFAULT);
    _uart_8250_write_reg(uart, IER_OFFSET, IER_DEFAULT);
    _uart_8250_write_reg(uart, LCR_OFFSET, LCR_DEFAULT);
    _uart_8250_write_reg(uart, MCR_OFFSET, MCR_DEFAULT);

    uart->ready = 1;

    return 0;
}

/* read() is implemented only for stdin. Each read character is echoed */
ssize_t
read(int fd, void *buf, size_t count)
{
    struct _uart_8250 *uart = &_uart_8250;
    size_t             bytes_read;
    char              *buf_char = buf;
    int                c;

    if (uart->ready == 0)
        _setup_arc_platform();

    if (fd != STDIN_FILENO) {
        errno = ENOSYS;
        return -1;
    }

    if (!uart->ready) {
        errno = EIO;
        return -1;
    }

    bytes_read = 0;
    c = EOF;
    /* Break on '\n' to simulate readline behavior */
    while (bytes_read != count && c != '\n') {
        c = _uart_8250_getc(uart);
        if (c == EOF)
            break;

        /* Echo character to the console */
        _uart_8250_putc(uart, c);

        buf_char[bytes_read] = c;
        bytes_read++;
    }

    return bytes_read;
}

/* _write() is implemented only for stdout and stderr */
ssize_t
write(int fd, const void *buf, size_t count)
{
    struct _uart_8250 *uart = &_uart_8250;
    size_t             bytes_written;

    if (uart->ready == 0)
        _setup_arc_platform();

    if (fd != STDOUT_FILENO && fd != STDERR_FILENO) {
        errno = ENOSYS;
        return -1;
    }

    if (!uart->ready) {
        errno = EIO;
        return -1;
    }

    bytes_written = 0;
    while (bytes_written != count) {
        _uart_8250_putc(uart, ((char *)buf)[bytes_written]);
        bytes_written++;
    }

    return (ssize_t)bytes_written;
}
