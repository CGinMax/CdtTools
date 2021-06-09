cmake_minimum_required(VERSION 3.10.0)

project(FileTransServer)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
add_definitions(-std=c++11)

find_package(Qt5 COMPONENTS Core SerialPort Network REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::SerialPort Qt5::Network)
