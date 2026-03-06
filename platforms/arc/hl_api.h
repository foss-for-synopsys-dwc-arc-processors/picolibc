/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright © 2026, Synopsys Inc.
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

#include <stdint.h>
#include "hl_toolchain.h"

#ifndef _HL_API_H
#define _HL_API_H

#define HL_SYSCALL_OPEN        0
#define HL_SYSCALL_CLOSE       1
#define HL_SYSCALL_READ        2
#define HL_SYSCALL_WRITE       3
#define HL_SYSCALL_LSEEK       4
#define HL_SYSCALL_UNLINK      5
#define HL_SYSCALL_ISATTY      6
#define HL_SYSCALL_TMPNAM      7
#define HL_SYSCALL_GETENV      8
#define HL_SYSCALL_CLOCK       9
#define HL_SYSCALL_TIME        10
#define HL_SYSCALL_RENAME      11
#define HL_SYSCALL_ARGC        12
#define HL_SYSCALL_ARGV        13
#define HL_SYSCALL_RETCODE     14
#define HL_SYSCALL_ACCESS      15
#define HL_SYSCALL_GETPID      16
#define HL_SYSCALL_GETCWD      17
#define HL_SYSCALL_USER        18

#define HL_GNUIO_EXT_VENDOR_ID 1025

#define HL_GNUIO_EXT_FSTAT     1

/*
 * Main functions to work with regular syscalls and user-defined hostlink
 * messages.
 */
volatile __uncached char *_hl_message(uint32_t syscall, const char *format, ...);
uint32_t                  _user_hostlink(uint32_t vendor, uint32_t opcode, const char *format, ...);

/* Fuctions for direct work with the Hostlink buffer. */
volatile __uncached char *_hl_pack_int(volatile __uncached char *p, uint32_t x);
volatile __uncached char *_hl_pack_ptr(volatile __uncached char *p, const void *s, uint16_t len);
volatile __uncached char *_hl_pack_str(volatile __uncached char *p, const char *s);
volatile __uncached char *_hl_unpack_int(volatile __uncached char *p, uint32_t *x);
volatile __uncached char *_hl_unpack_ptr(volatile __uncached char *p, void *s, uint32_t *plen);
volatile __uncached char *_hl_unpack_str(volatile __uncached char *p, char *s);
uint32_t                  _hl_get_ptr_len(volatile __uncached char *p);

/* Low-level functions from hl_gw. */
extern uint32_t           _hl_iochunk_size(void);
extern void               _hl_delete(void);
int                       get_cmdline(char *buffer, int count);

#endif
