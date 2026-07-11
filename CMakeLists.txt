cmake_minimum_required(VERSION 3.20)

project(Pendulum LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(Pendulum
    test.cpp
)

add_custom_command(TARGET Pendulum POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:Pendulum>
        $<TARGET_FILE_DIR:Pendulum>
    COMMAND_EXPAND_LISTS
)

target_link_libraries(Pendulum PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)