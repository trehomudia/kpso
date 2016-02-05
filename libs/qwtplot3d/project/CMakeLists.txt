cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME qwtplot3d)
set(PROJECT_LANGUAGES CXX C)
set(ROOT_PATH "../../..")
set(CLIENT kpso)

include(${ROOT_PATH}/project/General.cmake)
project(${PROJECT_NAME} ${PROJECT_LANGUAGES})

find_package(Qt4 REQUIRED QtCore QtGui QtXml QtSql QtOpenGL)
find_package(GLUT)
find_package(OpenGL)
include(${QT_USE_FILE})

set(SOURCES
	../src/qwt3d_axis.cpp 
	../src/qwt3d_color.cpp 
	../src/qwt3d_coordsys.cpp 
	../src/qwt3d_drawable.cpp 
	../src/qwt3d_mousekeyboard.cpp 
	../src/qwt3d_movements.cpp 
	../src/qwt3d_lighting.cpp 
	../src/qwt3d_colorlegend.cpp 
	../src/qwt3d_plot.cpp 
	../src/qwt3d_label.cpp 
	../src/qwt3d_types.cpp 
	../src/qwt3d_enrichment_std.cpp 
	../src/qwt3d_autoscaler.cpp 
	../src/qwt3d_io_reader.cpp 
	../src/qwt3d_io.cpp 
	../src/qwt3d_scale.cpp
	../src/qwt3d_gridmapping.cpp 
	../src/qwt3d_parametricsurface.cpp 
	../src/qwt3d_function.cpp
	../src/qwt3d_surfaceplot.cpp 
	../src/qwt3d_gridplot.cpp 
	../src/qwt3d_meshplot.cpp
	../src/qwt3d_io_gl2ps.cpp 
	../3rdparty/gl2ps/gl2ps.c
)
	
set(MOC_HEADERS
	../include/qwt3d_color.h 
	../include/qwt3d_global.h 
	../include/qwt3d_types.h 
	../include/qwt3d_axis.h 
	../include/qwt3d_coordsys.h 
	../include/qwt3d_drawable.h 
	../include/qwt3d_helper.h 
	../include/qwt3d_label.h 
	../include/qwt3d_openglhelper.h 
	../include/qwt3d_colorlegend.h 
	../include/qwt3d_plot.h 
	../include/qwt3d_enrichment.h 
	../include/qwt3d_enrichment_std.h 
	../include/qwt3d_autoscaler.h 
	../include/qwt3d_autoptr.h 
	../include/qwt3d_io.h 
	../include/qwt3d_io_reader.h 
	../include/qwt3d_scale.h 
	../include/qwt3d_portability.h
	../include/qwt3d_mapping.h 
	../include/qwt3d_gridmapping.h 
	../include/qwt3d_parametricsurface.h 
	../include/qwt3d_function.h
	../include/qwt3d_surfaceplot.h 
	../include/qwt3d_volumeplot.h 
	../include/qwt3d_graphplot.h 
	../include/qwt3d_multiplot.h
	../3rdparty/gl2ps/gl2ps.h 
	../include/qwt3d_io_gl2ps.h
)

qt4_wrap_cpp(MOC_SOURCES ${MOC_HEADERS})

include_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	../include
)

add_library(${PROJECT_NAME} SHARED ${SOURCES} ${MOC_HEADERS} ${MOC_SOURCES})

target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES} ${QT_QTMAIN_LIBRARY} ${GLUT_LIBRARY} ${OPENGL_LIBRARY})

add_definitions(-DQT_DLL -DQWT3D_DLL -DQWT3D_MAKEDLL -DQT_OPENGL_LIB)

set(DEPENDS_MODULE
)

make_dep_tree()
