set(TIMER_SOURCES
    "main.cpp"
    "core.cpp"
    "core.hpp"
    "luaCode.cpp"
    "Script.lua")

add_executable(Timer ${TIMER_SOURCES})


#dat find package here ??, a odobrat add subdir ako tvrdi chatgpt, findovat, len ked je package nainstalovane
#find_package(SDL3 REQUIRED)


target_link_libraries(Timer PRIVATE lua SDL3::SDL3 SDL3_image::SDL3_image)
#target_link_libraries(Timer PRIVATE SDL3::SDL3)
#target_link_libraries(Timer PRIVATE SDL3_image::SDL3_image)
#target_link_libraries(hello PRIVATE SDL3_image::SDL3_image SDL3::SDL3)


#Set C++20 per target
#target_compile_features(project PUBLIC cxx_std_20)


# Set working directory to the build directory target specific
#set_target_properties(project PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")


add_custom_command(TARGET Timer POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E copy_if_different #or command: copy 
        ${CMAKE_CURRENT_SOURCE_DIR}/pika.png
        $<TARGET_FILE_DIR:Timer>

    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:Timer>/scripts

    COMMAND ${CMAKE_COMMAND} -E copy #or command: copy_if_different 
        ${CMAKE_CURRENT_SOURCE_DIR}/Script.lua
        $<TARGET_FILE_DIR:Timer>/scripts
)


#add_custom_command(TARGET Timer POST_BUILD

#    COMMAND ${CMAKE_COMMAND} -E copy_if_different #or command: copy 
#        ${CMAKE_CURRENT_SOURCE_DIR}/pika.png
#        #${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
#        $<TARGET_FILE_DIR:Timer>
#)


#CMAKE_SOURCE_DIR → top-level source folder (root of the project)
#CMAKE_BINARY_DIR → top-level build folder
#CMAKE_CURRENT_SOURCE_DIR → folder of the current CMakeLists.txt
#CMAKE_CURRENT_BINARY_DIR → build folder corresponding to current CMakeLists.txt