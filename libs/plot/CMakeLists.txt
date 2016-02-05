cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME plot)
set(PROJECT_LANGUAGES CXX C)
set(ROOT_PATH "../..")
	
include(${ROOT_PATH}/project/General.cmake)
project(${PROJECT_NAME} ${PROJECT_LANGUAGES})

find_package(Qt4 REQUIRED QtCore QtGui QtOpenGL)
include(${QT_USE_FILE})

set(HEADERS
	./include/plot_api.h
	./include/plot_base.h
	./include/plot.h
	./include/viewing_plot_picker.h
	./include/selecting_plot_picker.h
	./include/integer_scale_draw.h
	./include/time_scale_draw.h
)

set(MOC_HEADERS
	./include/plot_base.h
	./include/plot.h
	./include/viewing_plot_picker.h
	./include/selecting_plot_picker.h
)

set(SOURCES
	./src/plot_base.cpp
	./src/plot.cpp
	./src/viewing_plot_picker.cpp
	./src/selecting_plot_picker.cpp
	./src/integer_scale_draw.cpp
	./src/time_scale_draw.cpp
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES} ${MOC_HEADERS} ${MOC_SOURCES})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES})

set(DEPENDS_MODULE
  qwt
)

make_dep_tree()
