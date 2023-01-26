cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(Communicado VERSION 0.0.1 LANGUAGES C ASM)

add_subdirectory(tftpd)

# Third party libraries
add_subdirectory(vendor/libtask)
