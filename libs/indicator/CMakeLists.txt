cmake_minimum_required( VERSION 2.8 )

set(PROJECT_NAME indicator)
set(PROJECT_LANGUAGES CXX C)
set(ROOT_PATH "../..")

include(${ROOT_PATH}/project/General.cmake)
project(${PROJECT_NAME} ${PROJECT_LANGUAGES})

set(HEADERS
	include/indicator.h
)

set(SOURCES
	src/indicator.cpp
)


add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES} )


set(DEPENDS_MODULE
	regression
)

make_dep_tree()
