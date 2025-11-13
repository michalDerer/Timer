cmake_minimum_required(VERSION 3.16)
project(projektTimer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)
add_subdirectory(vendored/SDL_image EXCLUDE_FROM_ALL)

add_executable(Timer source/main.cpp)

target_link_libraries(Timer PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)
