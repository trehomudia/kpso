#include "..//include//matrix.h"
#include "..//include//matrix2.h"
#include <math.h>


int symm_inv3(const double *matA1, const double *matA2, const double *matA3, double* matInvA1, double* matInvA2, double* matInvA3) 
{

	double d = 0.;

	if( 0 == matA1 || 0 == matA2 || 0 == matA3 || 0 == matInvA1 || 0 == matInvA2 || 0 == matInvA3)
		return -1;
	
	d = matA1[0] * matA2[1] * matA3[2]
		- matA1[0] * matA2[2] * matA2[2]
		- matA1[1] * matA1[1] * matA3[2]
		+ 2.*matA1[1] * matA1[2] * matA2[2]
		- matA1[2] * matA1[2] * matA2[1];
	
	if(fabs(d) > DBL_EPSILON) {
		d = 1. / d;		
		
		matInvA1[0] = (matA2[1] * matA3[2] - matA2[2] * matA2[2]) * d;
		matInvA1[1] = (matA1[2] * matA2[2] - matA1[1] * matA3[2]) * d;
		matInvA2[0] =  matInvA1[1];
		matInvA1[2] = (matA1[1] * matA2[2] - matA1[2] * matA2[1]) * d;
		matInvA3[0] =  matInvA1[2];
		matInvA2[1] = (matA1[0] * matA3[2] - matA1[2] * matA1[2]) * d;
		matInvA2[2] = (matA1[1] * matA1[2] - matA1[0] * matA2[2]) * d;
		matInvA3[1] =  matInvA2[2];
		matInvA3[2] = (matA1[0] * matA2[1] - matA1[1] * matA1[1]) * d;

		return 0;
	} else {
		if(fabs(matA1[0]) > DBL_EPSILON)
			matInvA1[0] = 1./matA1[0];
		else
			matInvA1[0] = 1.;
		matInvA1[1] = 0.;
		matInvA2[0] = 0.;
		matInvA1[2] = 0.;
		matInvA3[0] = 0.;
		if(fabs(matA2[1]) > DBL_EPSILON)
			matInvA2[1] = 1./matA2[1];
		else
			matInvA2[1] = 1.;
		matInvA2[2] = 0.;
		matInvA3[1] = 0.;
		if(fabs(matA3[2]) > DBL_EPSILON)
			matInvA3[2] = 1./matA3[2];
		else
			matInvA3[2] = 1.;
		return -2;
	}
}

double norm_inf6(double *A) {
	
	unsigned int i, j;
	double sum, maxval;

	maxval = 0.;
	for(i=0; i<6; i++) {
		sum = 0.;
		for(j=0; j<6; j++) {
			sum += fabs(A[i*6 + j]);
		}
		maxval = mat_max(maxval, sum);
	}
	return maxval;
}

int symm_inv6(const double *matA, double* matInvA) 
{
	double invA[9];
	double invAB[9];
	double invF[9];

	/* считаем по блочной формуле адаптированной для симметричных матриц
	 пусть matA = [A B; B' D], тогда matInvA = [(invA+E*invF*E') E; E' F]
	 здесь F = inv(D-B'*invA*B), E = -invA*B*F*/

	
	symm_inv3(&matA[0], &matA[6], &matA[12], &invA[0], &invA[3], &invA[6]);

	invAB[0] = -invA[0]*matA[3] - invA[1]*matA[9]  - invA[2]*matA[15];
	invAB[1] = -invA[0]*matA[4] - invA[1]*matA[10] - invA[2]*matA[16];
	invAB[2] = -invA[0]*matA[5] - invA[1]*matA[11] - invA[2]*matA[17];
	invAB[3] = -invA[1]*matA[3] - invA[4]*matA[9]  - invA[5]*matA[15];
	invAB[4] = -invA[1]*matA[4] - invA[4]*matA[10] - invA[5]*matA[16];
	invAB[5] = -invA[1]*matA[5] - invA[4]*matA[11] - invA[5]*matA[17];
	invAB[6] = -invA[2]*matA[3] - invA[5]*matA[9]  - invA[8]*matA[15];
	invAB[7] = -invA[2]*matA[4] - invA[5]*matA[10] - invA[8]*matA[16];
	invAB[8] = -invA[2]*matA[5] - invA[5]*matA[11] - invA[8]*matA[17];

	invF[0] = matA[21] + invAB[0]*matA[3] + invAB[3]*matA[9]  + invAB[6]*matA[15]; 
	invF[1] = matA[22] + invAB[1]*matA[3] + invAB[4]*matA[9]  + invAB[7]*matA[15];
	invF[2] = matA[23] + invAB[2]*matA[3] + invAB[5]*matA[9]  + invAB[8]*matA[15];
	invF[3] = invF[1];//matA[27] + invAB[0]*matA[4] + invAB[3]*matA[10] + invAB[6]*matA[16];
	invF[4] = matA[28] + invAB[1]*matA[4] + invAB[4]*matA[10] + invAB[7]*matA[16];
	invF[5] = matA[29] + invAB[2]*matA[4] + invAB[5]*matA[10] + invAB[8]*matA[16];
	invF[6] = invF[2];//matA[33] + invAB[0]*matA[5] + invAB[3]*matA[11] + invAB[6]*matA[17];
	invF[7] = matA[29];//matA[34] + invAB[1]*matA[5] + invAB[4]*matA[11] + invAB[7]*matA[17];
	invF[8] = matA[35] + invAB[2]*matA[5] + invAB[5]*matA[11] + invAB[8]*matA[17];
																														  
	symm_inv3(&invF[0], &invF[3], &invF[6], &matInvA[21], &matInvA[27], &matInvA[33]);																													  
	
	matInvA[3]  = invAB[0]*matInvA[21] + invAB[1]*matInvA[27] + invAB[2]*matInvA[33]; 
	matInvA[4]  = invAB[0]*matInvA[22] + invAB[1]*matInvA[28] + invAB[2]*matInvA[34]; 
	matInvA[5]  = invAB[0]*matInvA[23] + invAB[1]*matInvA[29] + invAB[2]*matInvA[35]; 
	matInvA[9]  = invAB[3]*matInvA[21] + invAB[4]*matInvA[27] + invAB[5]*matInvA[33]; 
	matInvA[10] = invAB[3]*matInvA[22] + invAB[4]*matInvA[28] + invAB[5]*matInvA[34]; 
	matInvA[11] = invAB[3]*matInvA[23] + invAB[4]*matInvA[29] + invAB[5]*matInvA[35]; 
	matInvA[15] = invAB[6]*matInvA[21] + invAB[7]*matInvA[27] + invAB[8]*matInvA[33]; 
	matInvA[16] = invAB[6]*matInvA[22] + invAB[7]*matInvA[28] + invAB[8]*matInvA[34]; 
	matInvA[17] = invAB[6]*matInvA[23] + invAB[7]*matInvA[29] + invAB[8]*matInvA[35]; 

	matInvA[0]  = invA[0] + matInvA[3]*invAB[0]  + matInvA[4]*invAB[1]  + matInvA[5]*invAB[2]; 
	matInvA[1]  = invA[1] + matInvA[3]*invAB[3]  + matInvA[4]*invAB[4]  + matInvA[5]*invAB[5];
	matInvA[2]  = invA[2] + matInvA[3]*invAB[6]  + matInvA[4]*invAB[7]  + matInvA[5]*invAB[8];
	matInvA[6]  = matInvA[1];//invA[3] + matInvA[9]*invAB[0]  + matInvA[10]*invAB[1] + matInvA[11]*invAB[2];
	matInvA[7]  = invA[4] + matInvA[9]*invAB[3]  + matInvA[10]*invAB[4] + matInvA[11]*invAB[5];
	matInvA[8]  = invA[5] + matInvA[9]*invAB[6]  + matInvA[10]*invAB[7] + matInvA[11]*invAB[8];
	matInvA[12] = matInvA[2];//invA[6] + matInvA[15]*invAB[0] + matInvA[16]*invAB[1] + matInvA[17]*invAB[2];
	matInvA[13] = matInvA[8];//invA[7] + matInvA[15]*invAB[3] + matInvA[16]*invAB[4] + matInvA[17]*invAB[5];
	matInvA[14] = invA[8] + matInvA[15]*invAB[6] + matInvA[16]*invAB[7] + matInvA[17]*invAB[8];

	matInvA[18] = matInvA[3] ; 
	matInvA[24] = matInvA[4] ; 
	matInvA[30] = matInvA[5] ; 
	matInvA[19] = matInvA[9] ; 
	matInvA[25] = matInvA[10]; 
	matInvA[31] = matInvA[11]; 
	matInvA[20] = matInvA[15]; 
	matInvA[26] = matInvA[16]; 
	matInvA[32] = matInvA[17]; 

	return 0;

}