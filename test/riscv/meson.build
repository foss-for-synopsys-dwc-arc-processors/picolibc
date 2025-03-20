riscv_tests = [
  'riscv-fpu-mstatus',
  'riscv-vector-mstatus'
]

foreach params : targets
  target = params['name']
  target_dir = params['dir']
  target_c_args = params['c_args']
  target_lib_prefix = params['lib_prefix']

  _libs = [get_variable('lib_c' + target)]
  _libs += [get_variable('lib_semihost' + target)]
  if is_variable(crt0_test + target)
    _objs = [get_variable(crt0_test + target)]
  else
    _objs = []
  endif

  foreach riscv_test : riscv_tests
    riscv_test_src = riscv_test + '.c'

    # Add extra options for some types of tests
    new_target_c_args = []
    foreach target_c_arg : target_c_args

      # Usually -march prebuilt configurations with V extension are
      # not shipped with GCC distribution. But we need to enable
      # vector tests, thus add this extension for them.
      if target_c_arg.startswith('-march') and riscv_test.startswith('riscv-vector')
        new_march_parts = []
        foreach march_part : target_c_arg.split('_')
          if march_part.contains('rv') and not march_part.endswith('v')
            new_march_parts += march_part + 'v'
          else
            new_march_parts += march_part
          endif
        endforeach
        new_target_c_args += '_'.join(new_march_parts)
      else
        new_target_c_args += target_c_arg
      endif

    endforeach
    target_c_args = new_target_c_args

    _c_args = target_c_args + get_variable('test_c_args' + target, test_c_args)
    _link_args = target_c_args + get_variable('test_link_args' + target, test_link_args)
    _link_depends = get_variable('test_link_depends' + target, test_link_depends)

    test(riscv_test + target,
        executable(riscv_test + target, [riscv_test_src],
            c_args: _c_args,
            link_args: _link_args,
            objects: _objs,
            link_with: _libs,
            link_depends: _link_depends,
            include_directories: inc),
            depends: bios_bin,
            suite: 'riscv',
            env: test_env)
  endforeach

endforeach
