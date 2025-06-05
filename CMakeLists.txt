cmake_minimum_required(VERSION 3.10)

project(Crealiz)

file(GLOB_RECURSE SOURCES "src/*.c")

add_executable(Crealiz ${SOURCES})
target_include_directories(Crealiz PRIVATE "include")

set_target_properties(Crealiz PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)
