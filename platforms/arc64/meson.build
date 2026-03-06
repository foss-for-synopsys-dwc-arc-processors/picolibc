#
# SPDX-License-Identifier: BSD-3-Clause
#
# Copyright © 2026, Synopsys Inc.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above
#    copyright notice, this list of conditions and the following
#    disclaimer in the documentation and/or other materials provided
#    with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
# OF THE POSSIBILITY OF SUCH DAMAGE.
#

src_hl = files([
                '../arc/hl_api.c',
                '../arc/hl_close.c',
                '../arc/hl_exit.c',
                '../arc/hl_get_cmdline.c',
                '../arc/hl_gettimeofday.c',
                '../arc/hl_gw.c',
                '../arc/hl_isatty.c',
                '../arc/hl_kill.c',
                '../arc/hl_lseek.c',
                '../arc/hl_open.c',
                '../arc/hl_read.c',
                '../arc/hl_times.c',
                '../arc/hl_unlink.c',
                '../arc/hl_write.c',
               ])

foreach params : targets
  target = params['name']
  target_dir = params['dir']
  target_c_args = params['c_args']

  instdir = join_paths(lib_dir, target_dir)

  if meson.version().version_compare('>=1.10')
    _lib = static_library('hl',
                          src_hl,
                          build_subdir : target_dir,
                          install : really_install,
                          install_dir : instdir,
                          include_directories : inc,
                          c_args : target_c_args + c_args)
  else
    target_lib_prefix = params['lib_prefix']
    _lib = static_library(join_paths(target_dir, target_lib_prefix + 'hl'),
                          src_hl,
                          install : really_install,
                          install_dir : instdir,
                          include_directories : inc,
                          c_args : target_c_args + c_args)
  endif
endforeach
