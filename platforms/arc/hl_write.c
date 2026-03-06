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
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

#include "hl_toolchain.h"
#include "hl_api.h"

static ssize_t
_hl_write(int fd, const char *buf, size_t nbyte)
{
    ssize_t                   ret;
    int32_t                   n;
    uint32_t                  host_errno;
    volatile __uncached char *p;

    p = _hl_message(HL_SYSCALL_WRITE, "ipi:ii", (uint32_t)fd, buf, (uint32_t)nbyte, (uint32_t)nbyte,
                    (uint32_t *)&n, (uint32_t *)&host_errno);

    if (p == NULL) {
        errno = ETIMEDOUT;
        ret = -1;
    } else if (n < 0) {
        errno = host_errno;
        ret = -1;
    } else {
        ret = n;
    }

    _hl_delete();

    return ret;
}

ssize_t
write(int fd, const void *buf, size_t count)
{
    const uint32_t hl_iochunk = _hl_iochunk_size();
    size_t         to_write = count;
    size_t         offset = 0;
    ssize_t        ret = 0;

    while (to_write > hl_iochunk) {
        ret = _hl_write(fd, (char *)buf + offset, hl_iochunk);

        if (ret < 0)
            return ret;

        offset += ret;

        if (ret != (ssize_t)hl_iochunk)
            return offset;

        to_write -= hl_iochunk;
    }

    if (to_write) {
        ret = _hl_write(fd, (char *)buf + offset, to_write);

        if (ret < 0)
            return ret;

        ret += offset;
    }

    return ret;
}
