#https://github.com/tpoechtrager/osxcross/issues/80
set(CMAKE_SYSTEM_NAME Darwin)
set(TOOLCHAIN_PREFIX x86_64-apple-darwin15)

# cross compilers to use for C and C++
set(CMAKE_C_COMPILER /qemu/osxcross/target/bin/${TOOLCHAIN_PREFIX}-clang)
set(CMAKE_CXX_COMPILER /qemu/osxcross/target/bin/${TOOLCHAIN_PREFIX}-clang++-libc++)

# target environment on the build host system
#   set 1st to dir with the cross compiler's C/C++ headers/libs
set(CMAKE_FIND_ROOT_PATH "/qemu/osxcross/target/")

# modify default behavior of FIND_XXX() commands to
# search for headers/libs in the target environment and
# search for programs in the build host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

set(CMAKE_OSX_SYSROOT "/qemu/osxcross/target/SDK/MacOSX10.11.sdk/")

set(CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG "-Wl,-rpath,")
set(CMAKE_SHARED_LIBRARY_RUNTIME_C_FLAG_SEP ":")

set(CMAKE_INSTALL_NAME_DIR "@rpath")
set(CMAKE_INSTALL_RPATH ...)
cmake_policy(SET CMP0025 NEW)

SET(CMAKE_CXX_FLAGS "-std=c++14")