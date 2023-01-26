# Communicado: A TFTP Server in C

![GitHub](https://img.shields.io/github/license/rishvic/communicado)

A simple server, implementing the Trivial File Transfer Protocol (TFTP),
as desribed in RFC [1350](https://datatracker.ietf.org/doc/html/rfc1350).
Implemented a concurrent server by using [Libtask](https://swtch.com/libtask/).

## Building ⚒️

This project uses [CMake](https://www.cmake.org/) as its build system. To build
the package, follow the below instructions:

```shell
# 0. Obtain the source and `cd` into it
git clone https://github.com/rishvic/communicado.git
cd communicado

# 1. Create a build directory, and run `cmake` to generate the build files
mkdir build && cd build
cmake ..

# 2. Build the project
cmake --build .
```

This will build the binaries inside `build` directory.

**Note:** If you want to see development-related info, then see the
          `README-hacking.md` file.
