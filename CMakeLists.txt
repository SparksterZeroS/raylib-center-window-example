cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(raylib_playground)

include(CPM.cmake)
CPMAddPackage(
        URI "gh:raysan5/raylib#master"
)


add_executable(app src/main.c)
target_link_libraries(
    app
    PRIVATE
    raylib
)