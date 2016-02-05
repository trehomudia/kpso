
#include "utils.h"
#include <QFileDialog>
#include <QTextStream>
#include <QApplication>
#include <QProgressBar>

#include "indicator/indicator.h"

QVector< QPair< double, double > > GetPriceSamplesFromCsvFile(const QString separator)
{
	QVector< QPair< double, double > > doubleData;

	QStringList pathList = QFileDialog::getOpenFileNames(0, QObject::trUtf8("Выбор файла"), "", "*.*", 0, 0);

	foreach(QString path, pathList)
	{
		QFile file(path);

		if(!file.open(QIODevice::ReadOnly))
			return doubleData;

		QTextStream in(&file);

		while(!in.atEnd())
		{
			QString line = in.readLine();
			QStringList stringList = line.split(separator);

			if(stringList.size() < 2)
				continue;

			doubleData << QPair< double, double >(stringList[0].toDouble(), stringList[1].toDouble());

		}
	}

	return doubleData;
}

std::vector< FuncPoint > ConvertQVectorQPairToStd(QVector< QPair< double, double > > &qVectorQPair)
{
	std::vector< FuncPoint > res;

	if(qVectorQPair.empty())
		return res;

	for(int i = 0; i < qVectorQPair.size(); i++)
	{
		res.push_back(std::pair< double, double >( qVectorQPair[i].first, qVectorQPair[i].second ));
	}

	return res;
}


QVector< QPair< double, double > > ConvertStdVectorStdPairToQt(std::vector< FuncPoint > &stdVectorStdPair)
{
	QVector< QPair< double, double > > res;
	
	if(stdVectorStdPair.empty())
		return res;

	for(int i = 0; i < stdVectorStdPair.size(); i++)
	{
		res.push_back(QPair< double, double >( stdVectorStdPair[i].first, stdVectorStdPair[i].second ));
	}

	return res;

}


std::vector< FuncInterval > GetIncreaseIntervals(std::multimap< FuncPoint, bool > &localExtremums)
{
	std::vector< FuncInterval > increaseIntervals;

	if(localExtremums.empty())
		return increaseIntervals;

	if(localExtremums.begin()->second == true && localExtremums.size() < 3)
		return increaseIntervals;

	std::multimap< FuncPoint, bool >::iterator extremumIt;
	std::multimap< FuncPoint, bool >::iterator extremumItStart = localExtremums.begin();
	int startI = 0;

	int size = localExtremums.size();

	if( localExtremums.begin()->second == true )
	{
		extremumItStart++;
		startI++;
	}

	extremumIt = extremumItStart;
	//for(extremumIt = extremumItStart; extremumIt != localExtremums.end(); extremumIt++)
	for(int i = startI; i < size-1; i+=2)
	{
		double x1 = extremumIt->first.first;
		double y1 = extremumIt->first.second;
		extremumIt++;

		double x2 = extremumIt->first.first;
		double y2 = extremumIt->first.second;

		increaseIntervals.push_back( FuncInterval( FuncPoint(x1,y1), FuncPoint(x2,y2) ) );

		extremumIt++;
	}



	return increaseIntervals;
}

std::multimap< FuncPoint, bool > GetSourceLocalExtremums(std::vector< FuncPoint > &sourceDependence,
																		std::multimap< FuncPoint, bool > &localExtremums)
{
	//typedef std::pair< double, double > FuncPoint;
	typedef std::pair< FuncPoint, bool > Extremum;

	std::multimap< FuncPoint, bool > extremums;

	if(sourceDependence.empty() || localExtremums.empty())
		return extremums;

	std::multimap< FuncPoint, bool >::iterator extremumIt;

	int sourceDependenceIndex = 0;

	for(extremumIt = localExtremums.begin(); extremumIt != localExtremums.end(); extremumIt++)
	{
		double	currentX			= extremumIt->first.first;
		bool	currentIsMaximum	= extremumIt->second;

		while(sourceDependence[sourceDependenceIndex].first != currentX)
			sourceDependenceIndex++;

		extremums.insert( Extremum( FuncPoint(currentX, sourceDependence[sourceDependenceIndex].second), currentIsMaximum ) );
	}

	return extremums;
}

std::multimap< FuncPoint, bool > GetLocalExtremums(std::vector< FuncPoint > &dependence)
{
	//typedef std::pair< double, double > FuncPoint;
	typedef std::pair< FuncPoint, bool > Extremum;

	std::multimap< FuncPoint, bool > extremums;

	if(dependence.empty())
		return extremums;

	std::vector< double > values;
	for(int i = 0; i < dependence.size(); i++)
		values.push_back(dependence[i].second);

	std::multimap< std::pair< int, int >, bool > extremumIndexes = GetLocalExtremumIndexes(values);
	
	std::multimap< std::pair< int, int >, bool >::iterator it;
	for(it = extremumIndexes.begin(); it != extremumIndexes.end(); it++)
		extremums.insert( Extremum( FuncPoint( dependence[ it->first.first ].first, dependence[ it->first.first ].second ), it->second ) );
	
	return extremums;
}

std::multimap< IndexInterval, bool > GetLocalExtremumIndexes(std::vector< double > &samples)
{
//	typedef std::pair< int, int > IndexInterval;
	typedef std::pair< IndexInterval, bool > Extremum;

	std::multimap< IndexInterval, bool > extremums;

	if(samples.empty())
		return extremums;

	int trend			= 0;
	int lastUpIndex		= -1;
	int lastDownIndex	= -1;
	int n				= samples.size();

	for(int i = 1; i < n; i++)
	{
		if(samples[i] > samples[i-1]) trend = 1;
		if(samples[i] < samples[i-1]) trend = -1;
		if(samples[i] == samples[i-1]) trend = 0;

		if(i == 1 && trend == -1)
			extremums.insert( Extremum(IndexInterval(0, 0), true) );	// максимум в точке с индексом 0
		if(i == 1 && trend == 1)
			extremums.insert( Extremum(IndexInterval(0, 0), false) );	// миниимум в точке с индексом 0

		if(i == 1 && trend == 0)
		{
			lastUpIndex		= 0;
			lastDownIndex	= 0;
		}

		if(trend == 1) lastUpIndex = i;
		if(trend == -1) lastDownIndex = i;

		if(trend == -1 && lastUpIndex != -1 && i != 1)
		{
			extremums.insert( Extremum(IndexInterval(lastUpIndex, (i-1)), true) ); // максимум в интервале индексов от lastUpIndex до i-1
			lastUpIndex = -1;
		}
		if(trend == 1 && lastDownIndex != -1 && i != 1)
		{
			extremums.insert( Extremum(IndexInterval(lastDownIndex, (i-1)), false) ); // миниимум в интервале индексов от lastDownIndex до i-1
			lastDownIndex = -1;
		}

		if(i == (n-1) && lastUpIndex != -1 && trend != -1)
			extremums.insert( Extremum(IndexInterval(lastUpIndex, (i)), true) ); // максимум в интервале индексов от lastUpIndex до i

		if(i == (n-1) && lastDownIndex != -1 && trend != 1)
			extremums.insert( Extremum(IndexInterval(lastDownIndex, (i)), false) ); // миниимум в интервале индексов от lastDownIndex до i

	}

	return extremums;
}

IncreasePercentResult GetIncreasePercent( std::vector< FuncInterval > &increaseIntervalVec, double comissionPercent )
{
	IncreasePercentResult res;

	if(increaseIntervalVec.empty())
		return res;

	int size = increaseIntervalVec.size();
	
	double startT	= increaseIntervalVec.begin()->first.first;
	double endT		= increaseIntervalVec.rbegin()->second.first;

	double deltaSum				= 0.0;
	double deltaWithComissionSum= 0.0;
	double startPrice			= 0.0;
	double endPrice				= 0.0;
	double startPriceWithComission= 0.0;
	double endPriceWithComission= 0.0;



	for(int i = 0; i < size; i++)
	{
		startPrice	= increaseIntervalVec[i].first.second;
		endPrice	= increaseIntervalVec[i].second.second;

		startPriceWithComission = startPrice * (1.0 + comissionPercent);
		endPriceWithComission	= endPrice * (1.0 - comissionPercent);
		
		deltaSum				+= (endPrice - startPrice) / startPrice;
		deltaWithComissionSum	+= (endPriceWithComission - startPriceWithComission) / startPriceWithComission;
	}

	res.n							= size;
	res.dt							= static_cast<int>(endT - startT);
	res.increasePercent				= deltaSum;
	res.increasePercentWithComission= deltaWithComissionSum;

	return res;
}

IncreasePercentResult GetIncreasePercent( std::vector< FuncPoint > &dependence, int winWidth, int step, double comissionPercent)
{
	IncreasePercentResult res;

	if(dependence.empty())
		return res;

	CIndicator ind(dependence);

	std::vector< FuncPoint > movingAverage = ind.GetMovingAverage(winWidth, step);

	if(movingAverage.empty())
	{
		res.n = 0;
		return res;
	}

	//std::multimap< FuncPoint, bool > extremums = GetLocalExtremums(movingAverage);
	// поиск индексов экстремумов индикатора:
	std::vector< double > values;
	for(int i = 0; i < movingAverage.size(); i++)
		values.push_back(movingAverage[i].second);

	std::multimap< std::pair< int, int >, bool > extremumIndexes = GetLocalExtremumIndexes(values);
	std::multimap< IndexInterval, bool >::iterator extremumIt;
	std::multimap< IndexInterval, bool >::iterator extremumItStart = extremumIndexes.begin();
	int startI = 0;
	int size = extremumIndexes.size();

	if( extremumIndexes.begin()->second == true )
	{
		extremumItStart++;
		startI++;
	}

	extremumIt = extremumItStart;

	double	deltaSum				= 0.0;
	double	deltaWithComissionSum	= 0.0;
	double	currentX				= 0.0;
	double	y1						= 0.0;
	double	y2						= 0.0;
	double	y1Com					= 0.0;
	double	y2Com					= 0.0;

	int		sourceDependenceIndex	= 0;
	int		n = 0;

	for(int i = startI; i < size-1; i+=2)
	{
		currentX = movingAverage[extremumIt->first.first].first;
		
		while(dependence[sourceDependenceIndex].first != currentX)
			sourceDependenceIndex++;

		y1 = dependence[sourceDependenceIndex].second;
		y1Com = (y1 * (1.0 + comissionPercent));

		extremumIt++;

		currentX = movingAverage[extremumIt->first.first].first;
		while(dependence[sourceDependenceIndex].first != currentX)
			sourceDependenceIndex++;

		y2 = dependence[sourceDependenceIndex].second;
		y2Com = (y2 * (1.0 - comissionPercent));

		deltaSum				+= (y2 - y1) / y1;
		deltaWithComissionSum	+= (y2Com - y1Com) / y1Com;

		//increaseIntervals.push_back( FuncInterval( FuncPoint(x1,y1), FuncPoint(x2,y2) ) );

		extremumIt++;
		n++;
	}


	res.dt	= static_cast<int>(dependence.rbegin()->first - dependence.begin()->first);
	res.n	= n;
	res.increasePercent				= deltaSum;
	res.increasePercentWithComission= deltaWithComissionSum;

	return res;
}


QVector< QPair< double, double > > GetStepQualityDependence(QVector< QPair< double, double > > &dep, int winWidth, int stepStart, int stepEnd, int stepStep)
{
	QVector< QPair< double, double > > qualityDep;

	if(dep.empty())
		return qualityDep;

	std::vector< FuncPoint > stdSamples = ConvertQVectorQPairToStd(dep);

	IncreasePercentResult res;

	QProgressBar *progress = new QProgressBar();
	progress->setMinimum(stepStart);
    progress->setMaximum(stepEnd);
    progress->setValue(0);
    progress->setTextVisible(true);
    progress->show();

	for(int i = stepStart; i <= stepEnd; i += stepStep)
	{
		res = GetIncreasePercent(stdSamples, winWidth, i, 0.002 );
		if(res.n == 0)
			break;

		qualityDep.push_back( QPair< double, double >( i, res.increasePercentWithComission*100.0  ) );
		progress->setValue(i);
		QApplication::processEvents();
	}

	progress->hide();
	delete progress;

	return qualityDep;
}

QVector< QPair< double, double > > GetWinQualityDependence(QVector< QPair< double, double > > &dep, int step, int winStart, int winEnd, int winStep)
{
	QVector< QPair< double, double > > qualityDep;

	if(dep.empty())
		return qualityDep;

	std::vector< FuncPoint > stdSamples = ConvertQVectorQPairToStd(dep);

	IncreasePercentResult res;

	QProgressBar *progress = new QProgressBar();
	progress->setMinimum(winStart);
    progress->setMaximum(winEnd);
    progress->setValue(0);
    progress->setTextVisible(true);
    progress->show();

	for(int i = winStart; i <= winEnd; i += winStep)
	{
		res = GetIncreasePercent(stdSamples, i, step, 0.002 );

		if(res.n == 0)
			break;

		qualityDep.push_back( QPair< double, double >( i, res.increasePercentWithComission*100.0  ) );
		progress->setValue(i);
		QApplication::processEvents();
	}

	progress->hide();
	delete progress;

	return qualityDep;
}

QMap< QPair< int, int >, double > GetQualityDependence(QVector< QPair< double, double > > &dep, int winStart, int winEnd, int winStep, int stepStart, int stepEnd, int stepStep)
{
	QMap< QPair< int, int >, double > qualityDependence;

	if(dep.empty())
		return qualityDependence;

	std::vector< FuncPoint > stdSamples = ConvertQVectorQPairToStd(dep);

	IncreasePercentResult res;

	QProgressBar *progress = new QProgressBar();
	progress->setMinimum(winStart);
    progress->setMaximum(winEnd);
    progress->setValue(0);
    progress->setTextVisible(true);
    progress->show();

	for(int i = winStart; i <= winEnd; i += winStep)
	{
		for(int j = stepStart; j <= stepEnd; j += stepStep)
		{
			res = GetIncreasePercent(stdSamples, i, j, 0.002 );

			if(res.n == 0)
				break;

			qualityDependence.insert( QPair< int, int >( i, j ), res.increasePercentWithComission*100.0 );

			progress->setValue(i);
			QApplication::processEvents();
		}
	}

	progress->hide();
	delete progress;

	return qualityDependence;
}


CDoubleSpinBox::CDoubleSpinBox(QWidget * parent)
: QDoubleSpinBox(parent)
, val(value())
{
  connect(this, SIGNAL(editingFinished()), this, SLOT(Edit()));
}

void CDoubleSpinBox::Edit()
{
	if (val != value())
	{
		val = value();
		emit EditingFinished(value());
	}
}

CSpinBox::CSpinBox(QWidget * parent)
: QSpinBox(parent)
, val(value())
{
  connect(this, SIGNAL(editingFinished()), this, SLOT(Edit()));
}

void CSpinBox::Edit()
{
	if (val != value())
	{
		val = value();
		emit EditingFinished(value());
	}
}
