cmake_minimum_required(VERSION 3.7)

set(TIMER_SOURCES
    "main.cpp"
)

add_executable(Timer ${TIMER_SOURCES})

target_link_libraries(Timer PRIVATE lua_s_lib)
target_link_libraries(Timer PRIVATE SDL3::SDL3)

#Set C++20 per target
#target_compile_features(project PUBLIC cxx_std_20)

# Set working directory to the build directory
#set_target_properties(project PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(TARGET Timer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different #or command: copy 
        ${CMAKE_CURRENT_SOURCE_DIR}/Script.lua
        #${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        $<TARGET_FILE_DIR:Timer>
)


#CMAKE_SOURCE_DIR → top-level source folder (root of the project)
#CMAKE_BINARY_DIR → top-level build folder
#CMAKE_CURRENT_SOURCE_DIR → folder of the current CMakeLists.txt
#CMAKE_CURRENT_BINARY_DIR → build folder corresponding to current CMakeLists.txt