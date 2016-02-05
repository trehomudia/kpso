/////////////////////////////////////////////
//
//   функции запаковки и распаковки матрицы
//
/////////////////////////////////////////////

// возвращаемые значения:
// 0 - распаковка прошла успешно
// 1 - буффер пуст
// 2 - память под матрицу не выделена


//////////////////////////////////

#include        <stdio.h>
#include        <math.h>
#include        "matrix//include//matrix.h"



// распаковка матрицы
int m_unpack(MAT **mat, MatrixReal *buffer, int m, int n)
{
	int i;

	if (*mat == NULL)
		return 2;
	if (buffer == NULL)
		return 1;

	// память для структуры
	*mat = (MAT*)buffer;
	// память для чисел
	(*mat)->base = (MatrixReal*)((char*)buffer + sizeof(MAT));
	// память для указателей
	(*mat)->me = (MatrixReal**)((char*)buffer + sizeof(MAT) + m*n*sizeof(MatrixReal));
	
	// установка указателей
	for ( i=0; i<m; i++ )
     (*mat)->me[i] = &((*mat)->base[i*n]);

   // заполнение вспомогательных полей матрицы
   (*mat)->m = (*mat)->max_m = m;		
   (*mat)->n = (*mat)->max_n = n;
   (*mat)->max_size = m*n;

   return 0;
}