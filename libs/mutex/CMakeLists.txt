cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME mutex)
set(PROJECT_LANGUAGES C)
set(ROOT_PATH "../..")

include(${ROOT_PATH}/project/General.cmake)
project(${PROJECT_NAME} ${PROJECT_LANGUAGES})

set(SOURCES
	src/mutex.c
)

set(HEADERS
	include/mutex.h
)

add_library(${PROJECT_NAME} ${SOURCES} ${HEADERS})

if (CMAKE_SYSTEM_NAME MATCHES Linux)
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(${PROJECT_NAME} rt)
endif (CMAKE_SYSTEM_NAME MATCHES Linux)

set(DEPENDS_MODULE

)
make_dep_tree()