cmake_minimum_required(VERSION 3.20)

# tftpd defaults
set(TFTPD_DEFAULT_ROOT_DIR /srv/tftp CACHE PATH "Default root directory for tftpd")

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h
)

add_executable(tftpd-bin
        tftpd.c
        args.c
        cli.c
)

set_target_properties(tftpd-bin
    PROPERTIES
        OUTPUT_NAME tftpd
)

target_include_directories(tftpd-bin
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(tftpd-bin PRIVATE
    _FORTIFY_SOURCE=2
    _GLIBCXX_ASSERTIONS
)

target_compile_options(tftpd-bin PRIVATE
    -fasynchronous-unwind-tables
    -fexceptions
    -fpie
    -fstack-clash-protection
    -fstack-protector-strong
    -g -grecord-gcc-switches
    -fcf-protection
    -O2 -pipe
    -Wall -Wextra -Wpedantic
    -Werror=format-security
    -Werror=implicit-function-declaration
)

target_link_options(tftpd-bin PRIVATE
    "LINKER:-pie"
    "LINKER:-z,defs"
    "LINKER:-z,now"
    "LINKER:-z,relro"
)

target_link_libraries(tftpd-bin
    PUBLIC
        Task::task
    PRIVATE
        Communicado::TFTPd
)

install(PROGRAMS tftpd-bin
        TYPE SBIN
)
