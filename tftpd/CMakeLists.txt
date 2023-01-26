cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

configure_file(config.h.in config.h)

add_executable(tftpd tftpd.c)

target_include_directories(tftpd
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(tftpd PRIVATE
    $<$<OR:$<C_COMPILER_ID:Clang>,$<C_COMPILER_ID:AppleClang>,$<C_COMPILER_ID:GNU>>:
    -Wall -Wextra -Wpedantic>)

target_link_libraries(tftpd
    PUBLIC
        Task::task
)
