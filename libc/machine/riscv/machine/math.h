/*
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright © 2020 Keith Packard
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

#ifndef _MACHINE_MATH_H_
#define _MACHINE_MATH_H_

#if defined(__riscv_flen) || defined(__riscv_zfinx)

#if (__riscv_flen >= 64) || defined(__riscv_zdinx)
#define __RISCV_HARD_FLOAT 64
#else
#define __RISCV_HARD_FLOAT 32
#endif

#define FCLASS_NEG_INF       (1 << 0)
#define FCLASS_NEG_NORMAL    (1 << 1)
#define FCLASS_NEG_SUBNORMAL (1 << 2)
#define FCLASS_NEG_ZERO      (1 << 3)
#define FCLASS_POS_ZERO      (1 << 4)
#define FCLASS_POS_SUBNORMAL (1 << 5)
#define FCLASS_POS_NORMAL    (1 << 6)
#define FCLASS_POS_INF       (1 << 7)
#define FCLASS_SNAN          (1 << 8)
#define FCLASS_QNAN          (1 << 9)

#define FCLASS_INF           (FCLASS_NEG_INF | FCLASS_POS_INF)
#define FCLASS_ZERO          (FCLASS_NEG_ZERO | FCLASS_POS_ZERO)
#define FCLASS_NORMAL        (FCLASS_NEG_NORMAL | FCLASS_POS_NORMAL)
#define FCLASS_SUBNORMAL     (FCLASS_NEG_SUBNORMAL | FCLASS_POS_SUBNORMAL)
#define FCLASS_NAN           (FCLASS_SNAN | FCLASS_QNAN)

#if __RISCV_HARD_FLOAT >= 64

/* anything with a 64-bit FPU has FMA */
#define __HAVE_FAST_FMA 1

#define _fclass_d(_x)                                          \
    (__extension__({                                           \
        long __fclass;                                         \
        __asm__("fclass.d %0, %1" : "=r"(__fclass) : "f"(_x)); \
        __fclass;                                              \
    }))

#endif

#if __RISCV_HARD_FLOAT >= 32

/* anything with a 32-bit FPU has FMAF */
#define __HAVE_FAST_FMAF 1

#define _fclass_f(_x)                                          \
    (__extension__({                                           \
        long __fclass;                                         \
        __asm__("fclass.s %0, %1" : "=r"(__fclass) : "f"(_x)); \
        __fclass;                                              \
    }))

#endif

#endif /* __RISCV_HARD_FLOAT */

#endif /* _MACHINE_MATH_H_ */
