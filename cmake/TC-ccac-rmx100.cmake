# the name of the target operating system
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_SYSTEM_PROCESSOR riscv)

# which compilers to use for C
if (DEFINED TARGET_COMPILE_OPTIONS)
separate_arguments(TARGET_COMPILE_OPTIONS UNIX_COMMAND "${TARGET_COMPILE_OPTIONS}")
else()
set(TARGET_COMPILE_OPTIONS -tcf rmx100.tcf -Zf -g -O0 -Dwint_t=unsigned -Hnocopyr -Xno_unaligned)
endif()

set(CMAKE_C_COMPILER ccac)

# where is the target environment located
set(CMAKE_FIND_ROOT_PATH /usr/bin)

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(_HAVE_PICOLIBC_TLS_API FALSE)

set(FORMAT_DEFAULT_DOUBLE 1)
set(FORMAT_DEFAULT_FLOAT 0)
set(FORMAT_DEFAULT_INTEGER 0)

set(PICOLIBC_LINK_FLAGS -Hnocrt -Hnolib ${METAWARE_ROOT}/arc/lib/av5rmx/le/libmw.a)
