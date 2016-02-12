cmake_minimum_required( VERSION 2.8 )

set(PROJECT_NAME btc)
set(PROJECT_LANGUAGES CXX C)
set(ROOT_PATH "../..")

include(${ROOT_PATH}/project/General.cmake)
project(${PROJECT_NAME} ${PROJECT_LANGUAGES})

find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL)
include(${QT_USE_FILE})

set(HEADERS
	include/football_manager.h
	include/storage.h
	include/table_view.h
	include/team.h
)

set(MOC_HEADERS
	include/football_manager.h
	include/storage.h
	include/table_view.h
	include/team.h
)

set(SOURCES
	src/football_manager.cpp
	src/main.cpp
	src/storage.cpp
	src/table_view.cpp
	src/team.cpp
)

set(UI_SOURCES
	form/championat_widget.ui
	form/football_manager.ui
)

include_directories(
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_SOURCES})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${MOC_HEADERS} ${MOC_SOURCES} ${UI_HEADERS} ${UI_SOURCES})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})

set(DEPENDS_MODULE
)

make_dep_tree()
