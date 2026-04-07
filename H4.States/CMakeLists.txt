set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(main main.c states.c)

add_library(states states.c)

target_link_libraries(main PRIVATE states)