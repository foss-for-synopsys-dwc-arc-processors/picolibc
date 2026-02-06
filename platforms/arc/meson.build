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

src_uart_8250 = [
                 'uart_8250_exit.c',
                 'uart_8250_kill.c',
                 'uart_8250_stub.c',
                 'uart_8250.c',
                ]

src_hsdk = files(['hsdk.c'] + src_uart_8250)
src_emsdp = files(['emsdp.c'] + src_uart_8250)

foreach params : targets
  target = params['name']
  target_dir = params['dir']
  target_c_args = params['c_args']
  src_platform = []

  is_arcem = '''
  #ifndef __ARCEM__
  #error __ARCEM__ is not defined
  #endif
  '''

  if cc.compiles(is_arcem, args: target_c_args + c_args)
    platform_name = 'emsdp'
    src_platform = src_emsdp
  endif

  is_archs = '''
  #ifndef __ARCHS__
  #error __ARCHS__ is not defined
  #endif
  '''

  if cc.compiles(is_archs, args: target_c_args + c_args)
    platform_name = 'hsdk'
    src_platform = src_hsdk
  endif

  if src_platform == []
    continue
  endif

  instdir = join_paths(lib_dir, target_dir)

  if meson.version().version_compare('>=1.10')
    _lib = static_library(platform_name,
                          src_platform,
                          build_subdir : target_dir,
                          install : really_install,
                          install_dir : instdir,
                          include_directories : inc,
                          c_args : target_c_args + c_args)
  else
    target_lib_prefix = params['lib_prefix']
    _lib = static_library(join_paths(target_dir, target_lib_prefix + platform_name),
                          src_platform,
                          install : really_install,
                          install_dir : instdir,
                          include_directories : inc,
                          c_args : target_c_args + c_args)
  endif
endforeach

foreach specs_file : ['hsdk.specs', 'emsdp1.1.specs', 'emsdp1.2.specs']
  if specs_install and really_install
    install_data(specs_file, install_dir: specs_dir)
  endif
endforeach
