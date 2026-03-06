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

#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "hl_toolchain.h"
#include "hl_api.h"

int
open(const char *pathname, int flags, ...)
{
    va_list                   ap;
    mode_t                    mode = 0;
    int32_t                   fd;
    uint32_t                  host_errno;
    uint32_t                  hl_flags = 0;
    volatile __uncached char *p;

    va_start(ap, flags);
    if (flags & O_CREAT)
        mode = va_arg(ap, mode_t);
    va_end(ap);

    hl_flags |= (flags & O_RDONLY) ? 0x0000 : 0;
    hl_flags |= (flags & O_WRONLY) ? 0x0001 : 0;
    hl_flags |= (flags & O_RDWR) ? 0x0002 : 0;
    hl_flags |= (flags & O_APPEND) ? 0x0008 : 0;
    hl_flags |= (flags & O_CREAT) ? 0x0100 : 0;
    hl_flags |= (flags & O_TRUNC) ? 0x0200 : 0;
    hl_flags |= (flags & O_EXCL) ? 0x0400 : 0;

    p = _hl_message(HL_SYSCALL_OPEN, "sii:ii", pathname, (uint32_t)hl_flags, (uint32_t)mode,
                    (uint32_t *)&fd, (uint32_t *)&host_errno);
    _hl_delete();

    if (p == NULL) {
        errno = ETIMEDOUT;
        fd = -1;
    } else if (fd < 0) {
        errno = host_errno;
        fd = -1;
    }

    return fd;
}
