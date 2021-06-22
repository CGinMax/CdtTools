cmake_minimum_required(VERSION 3.10.0 FATAL_ERROR)

project(ProtocolTools-Solution LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(asyncfuture)
add_subdirectory(QtAwesome)
add_subdirectory(Communication)
add_subdirectory(Protocols)
add_subdirectory(PressPlateTool)
# add_subdirectory(ProtocolTools)

# enable_testing()
# add_subdirectory(test)
