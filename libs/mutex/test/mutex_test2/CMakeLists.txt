cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME mutex_test2)
set(PROJECT_LANGUAGES CXX C)
set(ROOT_PATH "../../../..")

include(${ROOT_PATH}/project/General.cmake)
project(${PROJECT_NAME} ${PROJECT_LANGUAGES})

find_package(Qt4 REQUIRED QtTest QtCore)
include(${QT_USE_FILE})

set(SOURCES
	src/mutex_test2.cpp
)

set(MOC_HEADERS
  include/mutex_test2.h
)

qt4_wrap_cpp (MOC_SOURCES ${MOC_HEADERS})

add_executable(${PROJECT_NAME} ${SOURCES} ${MOC_HEADERS} ${MOC_SOURCES})

add_test(${PROJECT_NAME} ${EXECUTABLE_DIR}/${PROJECT_NAME})

set(DEPENDS_MODULE
	mutex
)

make_dep_tree()

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})


