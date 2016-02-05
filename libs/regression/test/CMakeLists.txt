cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME regressionTest)
set(PROJECT_LANGUAGES CXX C)
set(ROOT_PATH "../../../../..")

include(${ROOT_PATH}/project/General.cmake)
project(${PROJECT_NAME} ${PROJECT_LANGUAGES})

find_package(Matlab REQUIRED libmx)
include_directories(${MATLAB_INCLUDE_DIR})


set(HEADERS
#	src/k_means_test.cpp
)

set(SOURCES
	src/regressionTest.cpp
)

add_library(${PROJECT_NAME} MODULE ${HEADERS} ${SOURCES})
 
set(DEPENDS_MODULE
	mextype
	regression
)

message("MATLAB_MX_LIBRARY ################## ${MATLAB_MX_LIBRARY}")
message("MATLAB_MX_LIBRARY ################## ${MATLAB_MEX_LIBRARY}")
message("MATLAB_MX_LIBRARY ################## ${MATLAB_INCLUDE_DIR}")
make_dep_tree()

add_definitions(-DUNICODE -D_UNICODE)

rename_matlab_suffix( ${PROJECT_NAME} )

target_link_libraries(${PROJECT_NAME} "${MATLAB_MX_LIBRARY}")
target_link_libraries(${PROJECT_NAME} "${MATLAB_MEX_LIBRARY}")