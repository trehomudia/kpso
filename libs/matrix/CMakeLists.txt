cmake_minimum_required(VERSION 2.6)

set(PROJECT_NAME matrix)
set(PROJECT_LANGUAGES C)
set(ROOT_PATH "../../")

include(${ROOT_PATH}/project/General.cmake)
project(${PROJECT_NAME} ${PROJECT_LANGUAGES})

set(HEADERS
    include/err.h
    include/machine.h
    include/matrix.h
    include/matrix2.h
    include/matrix3.h
    include/meminfo.h
    )

set(SOURCES
	src/copy.c
	src/err.c
	src/fact.c
	src/givens.c
	src/init.c
	src/inits.c
	src/ivecop.c
	src/lufactor.c
	src/machine.c
	src/matop.c
	src/meminfo.c
	src/Memory.c
	src/memstat.c
	src/norm.c
	src/pshift.c
	src/pxop.c
	src/solve.c
	src/submat.c
	src/symm_inv.c
	src/vecop.c
	src/symmeig.c
	src/hessen.c
	src/hsehldr.c
	)
add_library(${PROJECT_NAME} ${HEADERS} ${SOURCES})

# ���������� ������ ��������� �������
set(DEPENDS_MODULE
	mutex 
	)

make_dep_tree()
