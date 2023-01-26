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

add_compile_definitions(_FORTIFY_SOURCE=2)

target_compile_options(tftpd PRIVATE
    -fasynchronous-unwind-tables
    -fexceptions
    -fpie $<$<C_COMPILER_ID:GNU>:-Wl,-pie>
    -g $<$<C_COMPILER_ID:GNU>:-grecord-gcc-switches>
    -O2 -pipe
    -Wall -Wextra -Wpedantic
    -Werror=format-security
    -Werror=implicit-function-declaration
    $<$<C_COMPILER_ID:GNU>:-Wl,-z,defs>
)

target_link_libraries(tftpd
    PUBLIC
        Task::task
)

install(PROGRAMS tftpd
        TYPE SBIN
)
