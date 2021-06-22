cmake_minimum_required(VERSION 3.15.0)

project(Protocols)

set(HEADERS
convert.h
protocols_global.h
iprotocols.h
)

set(SOURCES
convert.cpp
iprotocols.cpp
)
add_subdirectory(IEC104)
add_subdirectory(YBProtocol)
add_library(${PROJECT_NAME}
${HEADERS}
${SOURCES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    IEC104
    YBProtocol
)