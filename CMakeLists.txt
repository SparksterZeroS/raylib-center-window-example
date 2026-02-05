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

set_target_properties(
        app
        PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
)

target_compile_options(
        app
        PRIVATE
        $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)