#include "regression.h"

CRegression::CRegression(const std::vector< std::pair< double, double > > &dependence) :
m_dependence(dependence)
{
}

CRegression::~CRegression()
{}

COlsResult CRegression::GetOLS(const std::vector<ElemFuncType> funcVec)
{
	COlsResult res;

	if(m_dependence.empty() || funcVec.empty())
		return res;

	MAT *Y = FillYMatrix();

	MAT *F = FillFuncMatrix(funcVec);

	res = GetOLS(Y, F);

	res.funcVec = funcVec;

	matrix_free(Y);

	matrix_free(F);

	return res;
}

COlsResult CRegression::GetOLS(const unsigned int polyDegree)
{
	COlsResult res;

	if(m_dependence.empty())
		return res;

	MAT *Y = FillYMatrix();
	MAT *F = FillPolyMatrix(polyDegree);

	res = GetOLS(Y, F);

	res.funcVec.clear();
	
	matrix_free(Y);

	matrix_free(F);

	return res;
}

double CRegression::GetValue(const COlsResult &ols, double x)
{
	double value = 0.0;

	if(ols.coefficients.empty())
		return value;

	if(ols.funcVec.empty()) // полином:
	{
		// для каждой степени:
		for(int i = 0; i < ols.coefficients.size(); i++)
		{
			value += ols.coefficients[i] * pow(x, static_cast<double>(i));
		}

	}	
	else // набор элементарных функций:
	{
		if(ols.funcVec.size() != ols.coefficients.size())
			return value;

		// для каждой функции:
		for(int i = 0; i < ols.funcVec.size(); i++)
		{
			value += ols.coefficients[i] * GetFuncValue(ols.funcVec[i], x);
		}
	}
	return value;
}


// Получение значения регрессии в произвольной точке x
double CRegression::GetFuncValue(const ElemFuncType func, double x)
{
	switch(func)
	{
		case EFT_ONE :
		{
			return 1.0;
		}
		case EFT_LN :
		{
			return log(x);
		}
		case EFT_EXP:
		{
			return exp(x);
		}
		case EFT_EXP_MINUS:
		{
			return exp(x);
		}
		case EFT_HYP :
		{
			return 1.0 / x;
		}
		case EFT_SQRT_HYP:
		{
			return 1.0 / sqrt(x);
		}
		default:
		{
			return -1.0;
		}
	};
}


COlsResult CRegression::GetOLS(const MAT *Y, const MAT *F)
{
	COlsResult res;
	if(Y->m != F->m)
		return res;

	std::vector< double > coefficients;
	std::vector< std::pair< double, double > > outDependence;


	MAT *FT = 0, *FTF = 0, *invFTF = 0, *FTY = 0, *C = 0, *out = 0, *U = 0, *UT = 0, *S = 0;
	FT		= m_transp(F,FT);
	FTF		= m_mlt(FT, F, FTF);
	invFTF	= m_inverse(FTF, invFTF);
	FTY		= m_mlt(FT, Y, FTY);
	C		= m_mlt(invFTF, FTY, C);

	int coefCount = C->m;
	for(int i = 0; i < coefCount; i++)
	{
		coefficients.push_back(C->me[i][0]);
	}
	res.coefficients = coefficients;


	out = m_mlt(F, C, out);

	double x = 0.0;
	double y = 0.0;

	int m = out->m;	// число строк
	for(int i = 0; i < m; i++)
	{
		x = m_dependence[i].first;
		y = out->me[i][0];
		outDependence.push_back(std::pair< double, double>(x,y));
	}
	res.outDependence = outDependence;

	U	= m_sub(Y, out, U);
	UT	= m_transp(U, UT);
	S	= m_mlt(UT, U, S);

	res.sigma2 = S->me[0][0] / (m - coefCount);

	matrix_free(FT);
	matrix_free(FTF);
	matrix_free(invFTF);
	matrix_free(FTY);
	matrix_free(C);
	matrix_free(out);
	matrix_free(U);
	matrix_free(UT);
	matrix_free(S);


	return res;
}

MAT * CRegression::FillYMatrix()
{
	int size = m_dependence.size();
	MAT	*pY	= matrix_get(size, 1);
	m_zero(pY);

	double currentY = 0.0;
	for(int i = 0; i < size; i++)
	{
		currentY = m_dependence[i].second;
		pY->me[i][0] = currentY;
	}

	return pY;
}

MAT * CRegression::FillFuncMatrix(const std::vector< ElemFuncType > funcVec)
{
	MAT *pF;

	if(m_dependence.empty() || funcVec.empty())
		return pF;

	int depedenseSize = m_dependence.size();
	int funcCount = funcVec.size();

	pF = matrix_get(depedenseSize, funcCount);
	m_zero(pF);

	ElemFuncType currentFunc;
	for(int i = 0; i < funcCount; i++)
	{
		currentFunc = funcVec[i];
		if(-1 == FillFuncMatrixColumn(pF, i, currentFunc))
		{
			m_zero(pF);
			return pF;
		}
	}

	return pF;
}

int CRegression::FillFuncMatrixColumn(MAT * pF, int columnNum, ElemFuncType func)
{

	for(int i = 0; i < m_dependence.size(); i++)
	{
		pF->me[i][columnNum] = GetFuncValue(func, m_dependence[i].first);
	}

	return 0;
}

MAT * CRegression::FillPolyMatrix(const unsigned int polyDegree)
{
	MAT *pF;

	if(m_dependence.empty())
		return pF;

	int size = m_dependence.size();

	pF = matrix_get(size, polyDegree+1);
	m_zero(pF);

	for(int i = 0; i < size; i++)
	{
		for(int deg = 0; deg <= polyDegree; deg++)
		{
			pF->me[i][deg] = pow(m_dependence[i].first, deg);
		}
	}

	return pF;
}
