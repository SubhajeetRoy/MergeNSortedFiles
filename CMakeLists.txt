cmake_minimum_required(VERSION 2.8)


project( MAV )

set(SOURCES 
src/coreworker.cpp
src/utils.cpp
src/sortfiles.cpp
)
include_directories(./src)




set(MARGE_FLAGS_CM " -Wall -fpermissive -Wformat -Wparentheses -Wno-variadic-macros -Wno-sign-compare ")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} ${MARGE_FLAGS_CM}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MARGE_FLAGS_CM} -Wno-reorder")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--gc-sections,--stats,--Map,map.info")
set(CMAKE_C_FLAGS_DEBUG   "-DDEBUG -g -ggdb -O0")
set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG -g -ggdb -O0")

set(CMAKE_C_FLAGS_RELEASE   "-DRELEASE -Os")
set(CMAKE_CXX_FLAGS_RELEASE " -DRELEASE -Os")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-Wl,--strip-all")
add_executable(${PROJECT_NAME} ${SOURCES} )

#TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)

install(TARGETS ${PROJECT_NAME}
        DESTINATION sbin)


