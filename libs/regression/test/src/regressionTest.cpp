#include <mex.h>
#include "mextype/mextype.h"
#include <vector>

#include "regression/regression.h"

/**	@brief ѕреобразование двумерного MATLAB-массива (массива n-мерных точек) в "двумерный" с++ вектор
	@param x - указатель на двумерный MATLAB-массив
	@param numPoints - количество строк (количество n-мерных точек)
	@param dimension - количество столбцов (размерность)
	@return "двумерный" с++ вектор
*/
std::vector< std::vector< double > > ConvertMxArray2ToStdVector2(double *x, size_t numPoints, size_t dimension)
{
	std::vector< std::vector< double > > points;
	double currentValue = 0;
	std::vector< double > currentVector;

	for(int row = 0; row < numPoints; row++)
	{
		currentVector.clear();
		for(int column = 0; column < dimension; column++)
		{
			currentValue = x[row + column*numPoints];
			currentVector.push_back(currentValue);
		}
		points.push_back(currentVector);
	}
	return points;
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	if(nrhs < 1)
	{
		mexErrMsgTxt( "Ќеверные входные параметры!" );
		return;
	}

	size_t numPoints = 0, dimension = 0;
//	int k = 0;

	double *x = 0;
//
	dimension = mxGetN( prhs[0] );
	if(dimension != 2)
	{
		mexErrMsgTxt( "Ќеверные входные параметры!" );
		return;
	}

	numPoints = mxGetM( prhs[0] );

	x = mxGetPr(prhs[0]);

	std::vector< std::vector< double > > tempVec = ConvertMxArray2ToStdVector2(x, numPoints, dimension);

	std::vector< std::pair< double, double > > depedence;

	for(int i = 0; i < tempVec.size(); i++)
	{
		double x = tempVec[i][0];
		double y = tempVec[i][1];

		depedence.push_back(std::pair<double, double>(x,y));
	}

	CRegression reg(depedence);
	std::vector< ElemFuncType > funcVec;
	funcVec.push_back(EFT_LN);
	funcVec.push_back(EFT_ONE);

	std::vector< std::pair< double, double > > outDepedence;

	std::vector< double > coef = reg.GetOLS(funcVec).coefficients;

	std::vector< double > polyCoef = reg.GetOLS(5).coefficients;

	return;
}