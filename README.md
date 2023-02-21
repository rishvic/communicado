# Communicado: A TFTP Server in C

![GitHub](https://img.shields.io/github/license/rishvic/communicado)

A simple server, implementing the Trivial File Transfer Protocol (TFTP), as
desribed in RFC [1350](https://datatracker.ietf.org/doc/html/rfc1350).
Implemented a concurrent server by using [Libtask](https://swtch.com/libtask/).

## Building ⚒️

This project uses [CMake](https://www.cmake.org/) as its build system. To build
the package, follow the below instructions:

```shell
# 0. Obtain the source and `cd` into it
git clone https://github.com/rishvic/communicado.git
cd communicado

# 1. Run `cmake` to generate the build files
cmake .

# 2. Build the project
cmake --build .
```

This will build the binary at `tftpd/tftpd`.

To change the default TFTP root directory (set as `"/var/www/tftp"` by default),
while running `cmake`, set the option `TFTPD_DEFAULT_ROOT_DIR` as follows:

```shell
# ...
# 1. Run `cmake`, but with a custom TFTPD_DEFAULT_ROOT_DIR
cmake -DTFTPD_DEFAULT_ROOT_DIR=/var/lib/communicado/tftpd/data .
# ...
```

**Note:** If you want to see development-related info, then see the
`README-hacking.md` file.

### Out-of-source build

If you want out-of-source build, you can create a new directory, and pass the
source directory while running `cmake` in step 1 like so:

```shell
# This example will build tftp inside `build/` directory inside the source

# 1. Create the `build/` directory and cd into it, then run `cmake`
mkdir build && cd build
cmake ..

# 2. Build the project (Note: The build instruction points at `build/`
#    directory, not the source directory)
cmake --build .
```

This will build the binary at `build/tftpd/tftpd`. Note, that it seems
`install` command seems to not work with out-of-source builds.

## Running 🏃

To run the server, execute the `tftpd` binary. For configuration, pass along
options via command line arguments.

CLI Options:

- **`-d ROOT`** specifies the directory root, relative from which to read/write
  files. Default is _/var/www/tftp_.
- **`-p PORT`** specifies the server port. Default is _69_.
- **`-h`** displays a help message, and exits.
- **`-v`** outputs version information and exits.
