set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main main.c)

add_library(CSVPretPrin CSVPretPrin.c)

target_link_libraries(main PRIVATE CSVPretPrin)