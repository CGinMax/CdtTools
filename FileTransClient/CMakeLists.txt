cmake_minimum_required(VERSION 3.10.0)
project(FileTransClient)

find_package(Qt5 COMPONENTS Core Quick Qml REQUIRED)
add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Qml Qt5::Quick)

