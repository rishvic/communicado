cmake_minimum_required(VERSION 3.20)

add_library(tftpack STATIC
    src/send.c
)

add_library(Communicado::TFTPack ALIAS tftpack)

target_include_directories(tftpack
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(tftpack PRIVATE
    _FORTIFY_SOURCE=2
    _GLIBCXX_ASSERTIONS
)

target_compile_options(tftpack PRIVATE
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

target_link_options(tftpack PRIVATE
    "LINKER:-z,defs"
    "LINKER:-z,now"
    "LINKER:-z,relro"
)

target_link_libraries(tftpack
    PUBLIC
        Task::task
)
