
include_directories(include ${PROJECT_SOURCE_DIR}/Log/include)

set(SRC
    src/hnvalues.cpp
    src/init.cpp
    src/connect.cpp
    )

set(HEADER
    include/hnvalues.h
    )

add_library(hnvalues ${HEADER} ${SRC})

target_link_libraries(hnvalues log)
