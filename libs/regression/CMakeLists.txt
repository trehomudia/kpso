cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME regression)
set(PROJECT_LANGUAGES CXX C)
set(ROOT_PATH "../..")
	
include(${ROOT_PATH}/project/General.cmake)
project(${PROJECT_NAME} ${PROJECT_LANGUAGES})

find_package(Qt4 REQUIRED QtCore)
include(${QT_USE_FILE})

set(HEADERS
	include/regression.h
)

set(SOURCES
	src/regression.cpp
)

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})

target_link_libraries(${PROJECT_NAME})

set(DEPENDS_MODULE
	matrix
)
	
make_dep_tree()