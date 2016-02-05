cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME mutex_test1)
set(PROJECT_LANGUAGES CXX C)
set(ROOT_PATH "../../../..")

include(${ROOT_PATH}/project/General.cmake)
project(${PROJECT_NAME} ${PROJECT_LANGUAGES})

set(SOURCES
	src/mutex_test1.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${${PROJECT_NAME}_MOC_SOURCES})

add_test(${PROJECT_NAME} ${EXECUTABLE_DIR}/${PROJECT_NAME})

set(DEPENDS_MODULE
	mutex
	thread
)

make_dep_tree()
