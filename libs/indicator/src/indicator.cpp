#include "indicator.h"
#include <algorithm>
#include "regression/regression.h"
#include <math.h>

CIndicator::CIndicator(std::vector< std::pair< double, double > > timeSamples) :
m_timeSamples(timeSamples)
{
	//std::sort(m_timeSamples.begin(), m_timeSamples.end());
}

CIndicator::~CIndicator()
{

}

CIndicator & CIndicator::operator = (const CIndicator & other)
{
	if(this == &other)
		return *this;

	m_timeSamples = other.m_timeSamples;

	return *this;
}

std::vector< std::pair< double, double > > CIndicator::GetMovingRegression(int winWidth, int polyDegree)
{
	std::vector< std::pair< double, double > > movingRegression;

	for(int i = 0; i < m_timeSamples.size(); i++)
	{
		movingRegression.push_back(std::pair<double, double>( m_timeSamples[i].first, GetSingleMovingRegression(winWidth, polyDegree, i) ));
	}

	return movingRegression;
}

std::vector< std::pair< double, double > > CIndicator::GetMovingAverage(int winWidth, int step)
{
	std::vector< std::pair< double, double > > movingAverage;

	//for(int i = step/2; i < m_timeSamples.size(); i += step)
	for(int i = 0; i < m_timeSamples.size(); i += step)
	{
		movingAverage.push_back( std::pair<double, double>(m_timeSamples[i].first, GetSingleMovingAverage(winWidth, i) ));
	}

	return movingAverage;
}


std::vector< MovingAveragePoint > CIndicator::GetRealTimeMovingRegression(int winWidth, int step)
{
	std::vector< MovingAveragePoint > movingRegression;

	std::pair< double, double > currentMr;
	MovingAveragePoint currentMrPoint;
	for(int i = 0; i < m_timeSamples.size(); i += step)
	{
		currentMr = GetSingleRealTimeMovingRegression(winWidth, 1, i);

		currentMrPoint.x				= m_timeSamples[i].first;
		currentMrPoint.movingAverage	= currentMr.first;
		currentMrPoint.movingDispersion	= currentMr.second;

		//movingAverage.push_back( std::pair<double, double>(m_timeSamples[i].first, GetSingleRealTimeMovingAverage(winWidth, i).first ));
		movingRegression.push_back( currentMrPoint );
	}

	return movingRegression;
}

//std::vector< std::pair< double, double > > CIndicator::GetRealTimeMovingAverage(int winWidth, int step)
std::vector< MovingAveragePoint > CIndicator::GetRealTimeMovingAverage(int winWidth, int step)
{
	//std::vector< std::pair< double, double > > movingAverage;
	std::vector< MovingAveragePoint > movingAverage;

	std::pair< double, double > currentMa;
	MovingAveragePoint currentMaPoint;
	for(int i = 0; i < m_timeSamples.size(); i += step)
	{
		currentMa = GetSingleRealTimeMovingAverage(winWidth, i);

		currentMaPoint.x				= m_timeSamples[i].first;
		currentMaPoint.movingAverage	= currentMa.first;
		currentMaPoint.movingDispersion	= currentMa.second;

		//movingAverage.push_back( std::pair<double, double>(m_timeSamples[i].first, GetSingleRealTimeMovingAverage(winWidth, i).first ));
		movingAverage.push_back( currentMaPoint );
	}

	return movingAverage;
}

double CIndicator::GetSingleMovingRegression(int winWidth, int polyDegree, int index)
{
	if(m_timeSamples.empty())
		return 0.0;

	if(index < polyDegree)
		return m_timeSamples[index].second;

	int startIndex = (index+1 < winWidth) ? 0 : index + 1 - winWidth;
	int endIndex = index + 1;

	std::vector< std::pair< double, double > > regressionIn;

	double firstTime = m_timeSamples[startIndex].first;

	for(int i = startIndex; i < endIndex; i++)
	{
		regressionIn.push_back( std::pair< double, double >( m_timeSamples[i].first - firstTime, m_timeSamples[i].second ) );
	}

//	std::vector< std::pair< double, double > >::iterator startIt	= m_timeSamples.begin() + startIndex;

//	std::vector< std::pair< double, double > >::iterator endIt		= m_timeSamples.begin() + endIndex;

//	std::vector< std::pair< double, double > > regressionIn(startIt, endIt);

	CRegression regression(regressionIn);

	std::vector< double > coefficients = regression.GetOLS(polyDegree).coefficients;

	double value = 0.0;
	for(int i = 0; i < coefficients.size(); i++)
	{
		value += coefficients[i] * pow(m_timeSamples[index].first - firstTime, i);
	}

	return value;
}


double CIndicator::GetSingleMovingAverage(int winWidth, int index)
{
	if(m_timeSamples.empty())
		return 0.0;

	int size = m_timeSamples.size();

	int startIndex	= (index - winWidth/2 < 0) ? 0 : index - winWidth/2;
	int endIndex	= 0;
	
	if(winWidth%2)	// нечетная ширина
		endIndex = (index + winWidth/2 >= size) ? size : index + winWidth/2 + 1;
	else			// четная ширина
		endIndex = (index + winWidth/2 >= size) ? size : index + winWidth/2;

	double sum = 0.0;

	for(int i = startIndex; i < endIndex; i++)
	{
		sum += m_timeSamples[i].second;
	}
	sum /= static_cast< double >(endIndex - startIndex);

	return sum;
}

std::pair< double, double > CIndicator::GetSingleRealTimeMovingAverage(int winWidth, int index)
{
	if(m_timeSamples.empty())
		return std::pair< double, double >(0.0, 0.0);

	int size = m_timeSamples.size();

	int startIndex	= (index - winWidth + 1 < 0) ? 0 : index - winWidth + 1;
	int endIndex	= index + 1;

	// среднее:
	double mean = 0.0;
	for(int i = startIndex; i < endIndex; i++)
	{
		mean += m_timeSamples[i].second;
	}
	mean /= static_cast< double >(endIndex - startIndex);

	// дисперсия:
	double dispersion = 0.0;
	double delta = 0.0;
	for(int i = startIndex; i < endIndex; i++)
	{
		delta = m_timeSamples[i].second - mean;
		dispersion += delta * delta;
	}
	dispersion /= static_cast< double >(endIndex - startIndex);

	return std::pair< double, double >(mean, dispersion);
}


std::pair< double, double > CIndicator::GetSingleRealTimeMovingRegression(int winWidth, int polyDegree, int index)
{
	if(m_timeSamples.empty())
		return std::pair< double, double >(0.0, 0.0);

	if(index < polyDegree)
		return std::pair< double, double >(m_timeSamples[index].second, 0.0);

	int startIndex = (index+1 < winWidth) ? 0 : index + 1 - winWidth;
	int endIndex = index + 1;

	std::vector< std::pair< double, double > > regressionIn;

	double firstTime = m_timeSamples[startIndex].first;

	for(int i = startIndex; i < endIndex; i++)
	{
		regressionIn.push_back( std::pair< double, double >( m_timeSamples[i].first - firstTime, m_timeSamples[i].second ) );
	}

	CRegression regression(regressionIn);

	COlsResult olsResult = regression.GetOLS(polyDegree);
	std::vector< double > coefficients = olsResult.coefficients;

	double value = 0.0;
	for(int i = 0; i < coefficients.size(); i++)
	{
		value += coefficients[i] * pow(m_timeSamples[index].first - firstTime, i);
	}

	return std::pair< double, double >(value, olsResult.sigma2);
}