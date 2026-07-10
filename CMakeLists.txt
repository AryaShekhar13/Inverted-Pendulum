cmake_minimum_required(VERSION 3.20)

project(Pendulum LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(Pendulum
    test.cpp
)

target_link_libraries(Pendulum PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)