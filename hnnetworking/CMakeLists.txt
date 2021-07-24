
include_directories(include ${PROJECT_SOURCE_DIR}/Log/include)

set(SRC
    src/hnnetworking.cpp
    )

set(HEADER
    include/hnnetworking.h
    )

add_library(hnnetworking ${HEADER} ${SRC})

target_link_libraries(hnnetworking log)
