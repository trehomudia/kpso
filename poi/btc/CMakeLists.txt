cmake_minimum_required( VERSION 2.8 )

set(PROJECT_NAME btc)
set(PROJECT_LANGUAGES CXX C)
set(ROOT_PATH "../..")

include(${ROOT_PATH}/project/General.cmake)
project(${PROJECT_NAME} ${PROJECT_LANGUAGES})

find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL)
include(${QT_USE_FILE})

set(HEADERS
#	include/btc.h
	include/main_window.h
	include/time_plot.h
	include/utils.h
	include/quality_plot.h
#	include/spectrogram_plot.h
	include/quality_3dplot.h
)

set(MOC_HEADERS
#	include/btc.h
	include/main_window.h
	include/time_plot.h
	include/utils.h
	include/quality_plot.h
#	include/spectrogram_plot.h
	include/quality_3dplot.h
)

set(SOURCES
	src/btc.cpp
	src/main_window.cpp
	src/time_plot.cpp
	src/utils.cpp
	src/quality_plot.cpp
#	src/spectrogram_plot.cpp
	src/quality_3dplot.cpp
)

set(UI_SOURCES
	form/btc.ui
)

include_directories(
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})
qt4_wrap_ui(UI_HEADERS ${UI_SOURCES})

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${MOC_HEADERS} ${MOC_SOURCES} ${UI_HEADERS} ${UI_SOURCES})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY})

set(DEPENDS_MODULE
	plot
	indicator
	qwtplot3d
)

make_dep_tree()
