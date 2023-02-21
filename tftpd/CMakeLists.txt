cmake_minimum_required(VERSION 3.20)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/config.h
)

add_executable(tftpd
        tftpd.c
        server.c
        args.c
        cli.c
)

target_include_directories(tftpd
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(tftpd PRIVATE
    _FORTIFY_SOURCE=2
    _GLIBCXX_ASSERTIONS
)

target_compile_options(tftpd PRIVATE
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

target_link_options(tftpd PRIVATE
    "LINKER:-pie"
    "LINKER:-z,defs"
    "LINKER:-z,now"
    "LINKER:-z,relro"
)

target_link_libraries(tftpd
    PUBLIC
        Task::task
)

install(PROGRAMS tftpd
        TYPE SBIN
)
