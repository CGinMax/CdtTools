cmake_minimum_required(VERSION 3.15.0)

project(PressPlateTool)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS
    Core
    Gui
    Widgets
    SerialPort
    Network
    Concurrent
)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(../QtAwesome/QtAwesome)
include_directories(../Communication)
link_directories(${CMAKE_BINARY_DIR}/../QtAwesome/QtAwesome)
add_subdirectory(common)
add_subdirectory(protocol)
add_subdirectory(ui)

add_executable(${PROJECT_NAME}
main.cpp
mainwindow.cpp
mainwindow.h
mainwindow.ui
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::SerialPort
    Qt5::Network
    Qt5::Concurrent
    Communication
    QtAwesome
)