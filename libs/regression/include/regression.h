#ifndef REGRESSION_H
#define REGRESSION_H

#include "common_def.h"

#ifdef REGRESSION_STATICLIB
#define REGRESSION_API
#else
#ifdef REGRESSION_EXPORTS
#define REGRESSION_API SHARED_DECL_EXPORT
#else
#define REGRESSION_API SHARED_DECL_IMPORT
#endif
#endif

#include <vector>
#include "matrix/matrix.h"
#include "matrix/matrix2.h"
#include <math.h>

/**	@brief Обозначения элементарных функций */
enum ElemFuncType
{
	EFT_ONE = 1,	//!< тождественная единица
	EFT_LN,			//!< натуральный логарифм log(x) 
	EFT_EXP,		//!< экспонента exp(x)
	EFT_EXP_MINUS,	//!< экспонента exp(-x)
	EFT_HYP,		//!< 1/x
	EFT_SQRT_HYP	//!< 1 / sqrt(x)
};

/**	@brief Результат оценки параметров регрессии [по методу наименьших квадратов] */
struct COlsResult
{
	std::vector< double >						coefficients;	//!< коэффициенты регрессии
	std::vector< std::pair< double, double > >	outDependence;	//!< регрессия y(x)
	double										sigma2;			//!< оценка остаточной дисперсии
	std::vector< ElemFuncType >					funcVec;		//!< набор элементарных функций
};

class REGRESSION_API CRegression
{

public:

/**	@brief Конструктор
	@param dependence - точки {x,y} исходной зависимости y(x)
*/
	CRegression(const std::vector< std::pair< double, double > > &dependence);


/**	@brief Деструктор */
	~CRegression();


/**	@brief Получение регрессии в виде линейной комбинации элементарных функций по методу наименьших квадратов (Ordinary Less Squares)
	@param funcVec - массив элементарных функций
	@return регрессия
*/
	COlsResult GetOLS(const std::vector< ElemFuncType > funcVec);


/**	@brief Получение регрессии в виде многочлена заданной степени по методу наименьших квадратов (Ordinary Less Squares)
	@param polyDegree - степень многочлена
	@return регрессия
*/
	COlsResult GetOLS(const unsigned int polyDegree);


/**	@brief Получение значения регрессии в произвольной точке х
	@param ols - регрессия
	@param x - аргумент
	@return значение регрессии в точке x
*/
	static double GetValue(const COlsResult &ols, double x);

private:


/**	@brief Получение значения элементарной функции в произвольной точке х
	@param func - вид элементарной функции
	@param x аргумент
	@return значение функции в точке x	
*/
	static double GetFuncValue(const ElemFuncType func, double x);


/**	@brief Получение регрессии по методу наименьших квадратов (Ordinary Less Squares)
	@param pY - вектор значений
	@param pF - матрица плана
	@return регрессия
*/
	COlsResult GetOLS(const MAT *pY, const MAT *pF);


/**	@brief Заполнение вектора значений
	@return вектор значений
*/
	MAT * FillYMatrix();


/**	@brief Заполнение матрицы плана для получения регрессии в виде линейной комбинации элементарных функций
	@param funcVec - набор элементарных функций
	@return матрица плана
*/
	MAT * FillFuncMatrix(const std::vector<ElemFuncType> funcVec);


/**	@brief Заполнение матрицы плана для получения регрессии в виде многочлена заданной степени
	@param polyDegree - степень многочлена
	@return матрица плана
*/
	MAT * FillPolyMatrix(const unsigned int polyDegree);


/**	@brief Заполнение столбца матрицы плана значениями заданной элементарной функции
	@param pF - матрицы плана
	@param columnNum - номер столбца (нумерация с 0)
	@param func - вид элементарной функции
	@return 0
*/
	int FillFuncMatrixColumn(MAT * pF, int columnNum, ElemFuncType func);

	const std::vector< std::pair< double, double > > m_dependence;	//!< входная зависимость

};


#endif // REGRESSION_H
