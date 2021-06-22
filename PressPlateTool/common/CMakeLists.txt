set(SUBPRI_NAME common)

find_package(Qt5 COMPONENTS
    Core
    Widgets
    Network
    SerialPort
    Concurrent
REQUIRED
)
set(HEADERS
threadpool.h
workerthread.h
ybsensordata.h
gatherdata.h
)

set(SOURCES
threadpool.cpp
workerthread.cpp
ybsensordata.cpp
gatherdata.cpp
)

add_library(${SUBPRI_NAME}
STATIC
${HEADERS}
${SOURCES}
)

target_link_libraries(${SUBPRI_NAME} PUBLIC
    Qt5::Core
    Qt5::Widgets
    Qt5::Network
    Qt5::SerialPort
    Qt5::Concurrent
)