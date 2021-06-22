
cmake_minimum_required(VERSION 3.15.0)
project(Communication)

find_package(Qt5 COMPONENTS
    Core
    Network
    SerialPort
)

aux_source_directory(. NETWORK_SRC)

add_library(${PROJECT_NAME} SHARED ${NETWORK_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt5::Core
    Qt5::Network
    Qt5::SerialPort
)