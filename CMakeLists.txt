cmake_minimum_required(VERSION 3.20)
project(Communicado VERSION 0.0.1 LANGUAGES C ASM)

add_subdirectory(tftpd)

# Project's libraries
add_subdirectory(lib/tftpd)
add_subdirectory(lib/tftpack)

# Third party libraries
add_subdirectory(vendor/libtask)
