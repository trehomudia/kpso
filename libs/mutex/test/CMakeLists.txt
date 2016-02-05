cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME mutex_test)
set(PROJECT_LANGUAGES CXX C)
set(ROOT_PATH "../../..")

include(${ROOT_PATH}/project/General.cmake)
project(${PROJECT_NAME} ${PROJECT_LANGUAGES})

add_custom_target(${PROJECT_NAME})

set(GROUP_TESTS
	mutex_test1
	#mutex_test2
)

add_collection_test()
