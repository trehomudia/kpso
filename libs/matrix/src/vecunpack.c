/////////////////////////////////////////////
//
//   функции запаковки и распаковки вектора
//
/////////////////////////////////////////////

// возвращаемые значения:
// 0 - распаковка прошла успешно
// 1 - буффер пуст
// 2 - память под вектор не выделена


//////////////////////////////////

#include        <stdio.h>
#include        <math.h>
#include        "..//include//matrix.h"


// распаковка вектора
int v_unpack(VEC **vec, MatrixReal *buffer, int size)
{
	if (*vec == NULL)
		return 2;
	if (buffer == NULL)
		return 1;

    *vec = (VEC*)buffer;
	// память для чисел вектора
	(*vec)->ve = (MatrixReal*)((char*)(buffer)+sizeof(VEC));
	
	// заполнене вспомогательных полей
	(*vec)->dim = (*vec)->max_dim = size;
	return 0;
}
