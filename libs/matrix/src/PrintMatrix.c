#include <stdio.h>
#include "matrix//include//matrix.h"

int PrintMat(MAT *pMat, FILE *pFile)
{
	//Переменные
	int m = 0;
	int	n = 0;

	if(NULL == pMat)
		return -1;
	
	printf("\n ================ \n");
	for(m = 0; m < (int)pMat->m; m++){
		for(n = 0; n < (int)pMat->n; n++){
			printf(/*pFile, */"%8.8f\t", pMat->me[m][n]);
		}
		printf(/*pFile, */"\n");
	}
	printf("\n ================ \n");

	return 0;
}

int PrintVec(VEC *pVec, FILE *pFile)
{
	//Переменные
	int m = 0;

	if(NULL == pVec)
		return -1;
	
	printf("\n ================ \n");
	for(m = 0; m < (int)pVec->dim; m++){
		printf(/*pFile, */"%8.8f\n", pVec->ve[m]);
	}
	printf("\n ================ \n");

	return 0;
}
