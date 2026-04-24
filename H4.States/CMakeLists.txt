set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(tests tests.c states.c)

add_executable(main main.c states.c)

add_library(states states.c)

target_link_libraries(tests PRIVATE states)