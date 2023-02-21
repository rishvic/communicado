cmake_minimum_required(VERSION 3.20)

add_library(tftpd STATIC
    src/server.c
)

add_library(Communicado::TFTPd ALIAS tftpd)

target_include_directories(tftpd
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(tftpd PRIVATE
    _FORTIFY_SOURCE=2
    _GLIBCXX_ASSERTIONS
)

target_compile_options(tftpd PRIVATE
    -fasynchronous-unwind-tables
    -fexceptions
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
    "LINKER:-z,defs"
    "LINKER:-z,now"
    "LINKER:-z,relro"
)

target_link_libraries(tftpd
    PUBLIC
        Task::task
        Communicado::TFTPack
)
