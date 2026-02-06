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

#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

off_t
lseek(int fd, off_t offset, int whence)
{
    (void)fd;
    (void)offset;
    (void)whence;
    errno = ENOSYS;
    return -1;
}

_off64_t
lseek64(int fd, _off64_t offset, int whence)
{
    return (_off64_t)lseek(fd, (off_t)offset, whence);
}

/* Always return character device with 1024 bytes block */
int
fstat(int fd __attribute__((unused)), struct stat * restrict statbuf)
{
    memset(statbuf, 0, sizeof(*statbuf));
    statbuf->st_mode = S_IFCHR;
    statbuf->st_blksize = 1024;
    return 0;
}

int
fstat64(int fd __attribute__((unused)), struct stat64 *statbuf)
{
    memset(statbuf, 0, sizeof(*statbuf));
    statbuf->st_mode = S_IFCHR;
    statbuf->st_blksize = 1024;
    return 0;
}

int
close(int fd)
{
    (void)fd;
    errno = ENOSYS;
    return -1;
}

int
open(const char *pathname, int flags, ...)
{
    (void)pathname;
    (void)flags;
    errno = ENOSYS;
    return -1;
}

int
unlink(const char *name)
{
    (void)name;
    errno = ENOSYS;
    return -1;
}

int
stat(const char *pathname, struct stat * restrict statbuf)
{
    (void)pathname;
    (void)statbuf;
    errno = ENOSYS;
    return -1;
}

int
isatty(int fd)
{
    (void)fd;
    return 1;
}

int
getentropy(void *buffer, size_t length)
{
    (void)buffer;
    (void)length;
    errno = ENOSYS;
    return -1;
}
