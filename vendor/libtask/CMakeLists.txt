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

target_compile_options(task PRIVATE
    -Wall
    $<IF:$<C_COMPILER_ID:GNU>,-ggdb,-g>)
