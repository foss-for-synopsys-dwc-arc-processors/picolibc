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

#include <string.h>
#include <stddef.h>
#include <sys/cdefs.h>

#include "hl_toolchain.h"
#include "hl_api.h"

static __always_inline int
_hl_argc(void)
{
    uint32_t                  ret;
    volatile __uncached char *p;

    p = _hl_message(HL_SYSCALL_ARGC, ":i", (uint32_t *)&ret);
    _hl_delete();

    if (p == NULL)
        ret = 0;

    return ret;
}

static __always_inline int
_hl_argv(int a, char *arg)
{
    int                       ret = 0;
    volatile __uncached char *p;

    p = _hl_message(HL_SYSCALL_ARGV, "i:s", (uint32_t)a, (char *)arg);
    _hl_delete();

    if (p == NULL)
        ret = -1;

    return ret;
}

/* Get buffer length for argv[a] using HL_SYSCALL_ARGV */
static __always_inline uint32_t
_hl_argv_len(int a)
{
    uint32_t                  ret = 0;
    volatile __uncached char *p;

    p = _hl_message(HL_SYSCALL_ARGV, "i", (uint32_t)a);

    if (p != NULL)
        ret = _hl_get_ptr_len(p);

    _hl_delete();

    return ret;
}

int
get_cmdline(char *buffer, int count)
{
    int      argc;
    uint32_t argv_len;
    int      i;

    argc = _hl_argc();

    if (argc == 0)
        return -1;

    for (i = 0; i < argc; i++) {
        /* This variables store's argument's length including \0 */
        argv_len = _hl_argv_len(i);

        /* Stop processing arguments when the limit is exceeded */
        count -= argv_len;
        if (count < 0)
            break;

        if (i != 0) {
            buffer[0] = ' ';
            buffer += 1;
        }

        if (_hl_argv(i, buffer) < 0)
            return -1;

        buffer += argv_len - 1;
    }

    return 0;
}
