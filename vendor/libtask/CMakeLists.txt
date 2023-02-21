cmake_minimum_required(VERSION 3.20)

add_library(task STATIC
        src/asm.S
        src/channel.c
        src/context.c
        src/fd.c
        src/net.c
        src/print.c
        src/qlock.c
        src/rendez.c
        src/task.c
)

add_library(Task::task ALIAS task)

target_include_directories(task
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(task PRIVATE
    _FORTIFY_SOURCE=2
    _GLIBCXX_ASSERTIONS
)

target_compile_options(task PRIVATE
    -fasynchronous-unwind-tables
    -fexceptions
    -fstack-clash-protection
    -fstack-protector-strong
    -g -grecord-gcc-switches -ggdb
    -fcf-protection
    -O2 -pipe
    -Wall
    -Werror=format-security
    -Werror=implicit-function-declaration
)

target_link_options(task PRIVATE
    "LINKER:-z,defs"
    "LINKER:-z,now"
    "LINKER:-z,relro"
)
