
set(SUBDIR_NAME protocol)
find_package(Qt5 COMPONENTS
    Core
    SerialPort
    Network
REQUIRED
)
set(HEADERS
ybprotocolchannel.h
protocolchannelbase.h
)

set(SOURCES
ybprotocolchannel.cpp
protocolchannelbase.cpp
)
add_library(${SUBDIR_NAME}
STATIC
${HEADERS}
${SOURCES}
)
target_link_libraries(${SUBDIR_NAME} PUBLIC
    Qt5::Core
    Qt5::SerialPort
    Qt5::Network
)