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
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "hl_toolchain.h"
#include "hl_api.h"

/* Hostlink IO version of struct stat. */
struct hl_stat {
    uint32_t hl_dev;    /* ID of device containing file. */
    uint16_t hl_ino;    /* inode number. */
    uint16_t hl_mode;   /* File type and access mode. */
    int16_t  hl_nlink;  /* Number of hard links. */
    int16_t  hl_uid;    /* Owner's UID. */
    int16_t  hl_gid;    /* Owner's GID. */
    uint8_t  hl_pad[2]; /* Padding to match simulator struct. */
    uint32_t hl_rdev;   /* Device ID (if special file). */
    int32_t  hl_size;   /* Size in bytes. */
    int32_t  hl_atime;  /* Access time. */
    int32_t  hl_mtime;  /* Modification time. */
    int32_t  hl_ctime;  /* Creation time. */
} __packed;

int
fstat(int fd, struct stat *statbuf)
{
    struct hl_stat hl_statbuf;
    int32_t        ret;
    uint32_t       host_errno;

    /* Special version of hostlink - returned values are passed
     * through inargs.
     */
    host_errno = _user_hostlink(HL_GNUIO_EXT_VENDOR_ID, HL_GNUIO_EXT_FSTAT, "iii", (uint32_t)fd,
                                (uint32_t)&hl_statbuf, (uint32_t)&ret);

    _hl_delete();

    if (ret < 0) {
        errno = host_errno;
        return ret;
    }

    statbuf->st_dev = hl_statbuf.hl_dev;
    statbuf->st_ino = hl_statbuf.hl_ino;
    statbuf->st_mode = hl_statbuf.hl_mode;
    statbuf->st_nlink = hl_statbuf.hl_nlink;
    statbuf->st_uid = hl_statbuf.hl_uid;
    statbuf->st_gid = hl_statbuf.hl_gid;
    statbuf->st_rdev = hl_statbuf.hl_rdev;
    statbuf->st_size = hl_statbuf.hl_size;
    statbuf->st_atime = hl_statbuf.hl_atime;
    statbuf->st_mtime = hl_statbuf.hl_mtime;
    statbuf->st_ctime = hl_statbuf.hl_ctime;

    return ret;
}
