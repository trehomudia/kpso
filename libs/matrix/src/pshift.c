#include  <stdio.h>
#include  <math.h>
#include  "..//include//matrix.h"
#include  "..//include//matrix3.h"
#include	<stdlib.h>

void *pshift(void *pMem, // текущее положение указателя
			 int n,		 // кол-во строк (если 0 - определяется как вектор, размер вектора будет равен кол. столбцов)
			 int m)		 // кол-во столбцов (если 0 - определяется как вектор, размер вектора будет равен кол. строк)
{
	void *p_local;

	if (n==0 && m==0){
		printf ("Error in function pshift!!");
		//exit(1);
    return 0;
	}

	if (n == 0){
		p_local = ((char*)pMem+SET_VEC_SIZE(m));
		return p_local;
	}
	else if (m == 0){
		p_local = ((char*)pMem+SET_VEC_SIZE(n));
		return p_local;
	}

	p_local = ((char*)pMem+SET_MAT_SIZE(m,n));
	

	return p_local;
}
