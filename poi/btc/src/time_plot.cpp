#include "time_plot.h"
#include "indicator/indicator.h"
#include "qwt/qwt_plot_curve.h"
#include "qwt/qwt_plot_marker.h"
#include "qwt/qwt_plot_textlabel.h"
#include <math.h>

class CTimePlot::PrivateData
{
public:
	PrivateData() : 
m_priceCurve(0)
,label(0)
	{
		title.setRenderFlags(Qt::AlignLeft | Qt::AlignTop);
		QFont font("Times New Roman");
		font.setPointSize(15);
		title.setFont(font);

	}

~PrivateData()
{
	if(label)
		delete label;

	if(m_priceCurve)
		delete m_priceCurve;

	QList< int > indicatirKeys = m_indicatorMap.keys();
	for(int i = 0; i < indicatirKeys.size(); i++)
	{
		if(m_indicatorMap[indicatirKeys[i]])
			delete m_indicatorMap[indicatirKeys[i]];
	}

	foreach(QwtPlotCurve *curve, m_increaseIntervalLines)
	{
		if(curve)
			delete curve;
	}
}

	QVector< QPair< double, double > >	m_samples;
	QwtPlotCurve						*m_priceCurve;
	QMap< int, QwtPlotCurve* >			m_indicatorMap;
	QList< QwtPlotCurve *>				m_increaseIntervalLines;


	std::vector< std::pair< double, double > > firstMovingRegressionSamples;
	std::vector< std::pair< double, double > > secondMovingRegressionSamples;
	std::vector< std::pair< double, double > > thirdMovingRegressionSamples;
	std::vector< std::pair< double, double > > fourthMovingRegressionSamples;
	//std::vector< std::vector< std::pair< double, double > > > mrsVec;	//!< набор последовательных скользящиих регрессий


	QwtPlotTextLabel*					label;					//!< надпись на графике
	QwtText								title;					//!< текст надписи
};


CTimePlot::CTimePlot(QWidget *parent) : Plot(parent)
{
	m_pData = new PrivateData();
}

CTimePlot::~CTimePlot()
{
	delete m_pData;
}

void CTimePlot::SetData(QVector< QPair< double, double > > samples)
{
	m_pData->m_samples = samples;

	FormPriceLine();

	replot();
}

void CTimePlot::SetMovingRegressionIndicator(int win, int polyDeg)
{
	if(m_pData->m_samples.empty())
		return;

	CIndicator ind(ConvertQVectorQPairToStd(m_pData->m_samples));

	QwtPlotCurve *indicatorCurve = GetIndicatorLine(ind.GetMovingRegression(win, 2), 0);

	indicatorCurve->attach(this);

	replot();

}

void CTimePlot::SetMovingAverageIndicator(int win, int step)
{
	if(m_pData->m_samples.empty())
		return;

	CIndicator ind(ConvertQVectorQPairToStd(m_pData->m_samples));

	std::vector< std::pair< double, double > > samples = ind.GetMovingAverage(win, step);

	std::multimap< std::pair< double, double >, bool > extremums = GetLocalExtremums(samples);

	std::vector< FuncPoint > sourceDependence = ConvertQVectorQPairToStd(m_pData->m_samples);

	std::multimap< std::pair< double, double >, bool > srcExtr = GetSourceLocalExtremums(sourceDependence, extremums);

	std::vector< FuncInterval > increaseIntervals = GetIncreaseIntervals(srcExtr);

	// test:
	IncreasePercentResult res = GetIncreasePercent( increaseIntervals, 0.002 );
	//IncreasePercentResult res2 = GetIncreasePercent(ConvertQVectorQPairToStd(m_pData->m_samples), win, step, 0.002 );

	QwtPlotCurve *indicatorCurve	= GetIndicatorLine(samples, 0, QColor(0,128,192));
	//QwtPlotCurve *extremumCurve		= GetIndicatorLine(samples, 0, QColor(0,128,192));
	indicatorCurve->attach(this);

	FormIncreaseLines(increaseIntervals);

	FormText(increaseIntervals);

	replot();

}

void CTimePlot::SetRealTimeMovingAverageIndicator(int win, int step)
{
	if(m_pData->m_samples.empty())
		return;

	CIndicator ind(ConvertQVectorQPairToStd(m_pData->m_samples));

//	std::vector< std::pair< double, double > > samples = ind.GetRealTimeMovingAverage(win, step);
	std::vector< MovingAveragePoint > samples = ind.GetRealTimeMovingAverage(win, step);

	// вектор средних:
	std::vector< std::pair< double, double > > avrSamples;
	for(int i = 0; i < samples.size(); i++)
		avrSamples.push_back( std::pair< double, double >( samples[i].x, samples[i].movingAverage ) );

	// вектор дисперсий:
	std::vector< std::pair< double, double > > dispersionSamples;
	for(int i = 0; i < samples.size(); i++)
		dispersionSamples.push_back( std::pair< double, double >( samples[i].x, samples[i].movingDispersion ) );

	// вектор углов наклона изменения средних:
	std::vector< std::pair< double, double > > angleSamples;
	double k = 0.0;
	for(int i = 1; i < samples.size(); i++)
	{
		k = (samples[i].movingAverage - samples[i-1].movingAverage) / (samples[i].x - samples[i-1].x) ;

		angleSamples.push_back( std::pair< double, double >( samples[i].x, atan(k) ) );
	}


	QwtPlotCurve *avrCurve		= GetIndicatorLine(avrSamples, 0, QColor(0,128,192));
	QwtPlotCurve *dispCurve		= GetIndicatorLine(dispersionSamples, 1, QColor(Qt::darkGreen));
	QwtPlotCurve *angleCurve	= GetIndicatorLine(angleSamples, 2, QColor(Qt::darkBlue));

	
	avrCurve->attach(this);

	enableAxis( QwtPlot::yRight );

//	dispCurve->setYAxis(QwtPlot::yRight);
//	dispCurve->attach(this);

	angleCurve->setYAxis(QwtPlot::yRight);
	angleCurve->attach(this);

	replot();
}


void CTimePlot::SetRealTimeMovingRegressionIndicator(int win, int step)
{
	if(m_pData->m_samples.empty())
		return;

	CIndicator ind(ConvertQVectorQPairToStd(m_pData->m_samples));

//	std::vector< std::pair< double, double > > samples = ind.GetRealTimeMovingAverage(win, step);
	std::vector< MovingAveragePoint > samples = ind.GetRealTimeMovingRegression(win, step);

	// вектор средних:
	//std::vector< std::pair< double, double > > avrSamples;
	m_pData->firstMovingRegressionSamples.clear();
	for(int i = 0; i < samples.size(); i++)
		m_pData->firstMovingRegressionSamples.push_back( std::pair< double, double >( samples[i].x, samples[i].movingAverage ) );
		//avrSamples.push_back( std::pair< double, double >( samples[i].x, samples[i].movingAverage ) );

	//// вектор дисперсий:
	//std::vector< std::pair< double, double > > dispersionSamples;
	//for(int i = 0; i < samples.size(); i++)
	//	dispersionSamples.push_back( std::pair< double, double >( samples[i].x, samples[i].movingDispersion ) );

	// вектор углов наклона изменения средних:
	std::vector< std::pair< double, double > > angleSamples;
	double k = 0.0;
	for(int i = 1; i < samples.size(); i++)
	{
		k = (samples[i].movingAverage - samples[i-1].movingAverage) / (samples[i].x - samples[i-1].x) ;

		angleSamples.push_back( std::pair< double, double >( samples[i].x, atan(k) ) );
	}




	QwtPlotCurve *avrCurve		= GetIndicatorLine(m_pData->firstMovingRegressionSamples, 0, QColor(0,128,192));
	//QwtPlotCurve *dispCurve		= GetIndicatorLine(dispersionSamples, 1, QColor(Qt::darkGreen));
	QwtPlotCurve *angleCurve	= GetIndicatorLine(angleSamples, 2, QColor(Qt::darkBlue));

	// пунктирная линия уровня 0 второй оси X:
	QVector< QPointF > nullPoints;
	nullPoints << QPointF(angleSamples[0].first, 0.0) << QPointF(angleSamples[angleSamples.size()-1].first, 0.0);
	QwtPlotCurve *nullAngleCurve = new QwtPlotCurve();
	nullAngleCurve->setSamples(nullPoints);
	nullAngleCurve->setPen(* new QPen(Qt::darkBlue, 1, Qt::DashLine)); 
	//nullAngleCurve->setRenderHint(QwtPlotItem::RenderAntialiased);


	
	avrCurve->attach(this);

	enableAxis( QwtPlot::yRight );

//	dispCurve->setYAxis(QwtPlot::yRight);
//	dispCurve->attach(this);

	angleCurve->setYAxis(QwtPlot::yRight);
	angleCurve->attach(this);
	nullAngleCurve->setYAxis(QwtPlot::yRight);
	nullAngleCurve->attach(this);

	replot();
}



void CTimePlot::SetRealTimeRepeatMovingRegressionIndicator(int win, int step)
{
	if(m_pData->firstMovingRegressionSamples.empty())
		return;

	CIndicator ind(m_pData->firstMovingRegressionSamples);

	std::vector< MovingAveragePoint > samples = ind.GetRealTimeMovingRegression(win, step);

	// вектор средних:
	m_pData->secondMovingRegressionSamples.clear();
	for(int i = 0; i < samples.size(); i++)
		m_pData->secondMovingRegressionSamples.push_back( std::pair< double, double >( samples[i].x, samples[i].movingAverage ) );

	// вектор углов наклона изменения средних:
	std::vector< std::pair< double, double > > angleSamples;
	double k = 0.0;
	for(int i = 1; i < samples.size(); i++)
	{
		k = (samples[i].movingAverage - samples[i-1].movingAverage) / (samples[i].x - samples[i-1].x) ;

		angleSamples.push_back( std::pair< double, double >( samples[i].x, atan(k) ) );
	}

	QwtPlotCurve *avrCurve		= GetIndicatorLine(m_pData->secondMovingRegressionSamples, 3, QColor(Qt::red));
	//QwtPlotCurve *dispCurve		= GetIndicatorLine(dispersionSamples, 1, QColor(Qt::darkGreen));
	QwtPlotCurve *angleCurve	= GetIndicatorLine(angleSamples, 2, QColor(Qt::darkBlue));

	avrCurve->attach(this);

	enableAxis( QwtPlot::yRight );

	angleCurve->setYAxis(QwtPlot::yRight);
	angleCurve->attach(this);

	replot();
}

void CTimePlot::SetRealTimeRepeat3MovingRegressionIndicator(int win, int step)
{
	if(m_pData->secondMovingRegressionSamples.empty())
		return;

	CIndicator ind(m_pData->secondMovingRegressionSamples);

	std::vector< MovingAveragePoint > samples = ind.GetRealTimeMovingRegression(win, step);

	// вектор средних:
	m_pData->thirdMovingRegressionSamples.clear();
	for(int i = 0; i < samples.size(); i++)
		m_pData->thirdMovingRegressionSamples.push_back( std::pair< double, double >( samples[i].x, samples[i].movingAverage ) );

	// вектор углов наклона изменения средних:
	std::vector< std::pair< double, double > > angleSamples;
	double k = 0.0;
	for(int i = 1; i < samples.size(); i++)
	{
		k = (samples[i].movingAverage - samples[i-1].movingAverage) / (samples[i].x - samples[i-1].x) ;

		angleSamples.push_back( std::pair< double, double >( samples[i].x, atan(k) ) );
	}

	QwtPlotCurve *avrCurve		= GetIndicatorLine(m_pData->thirdMovingRegressionSamples, 4, QColor(128,0,128));
	//QwtPlotCurve *dispCurve		= GetIndicatorLine(dispersionSamples, 1, QColor(Qt::darkGreen));
	QwtPlotCurve *angleCurve	= GetIndicatorLine(angleSamples, 2, QColor(Qt::darkBlue));

	avrCurve->attach(this);

	enableAxis( QwtPlot::yRight );

	angleCurve->setYAxis(QwtPlot::yRight);
	angleCurve->attach(this);

	replot();
}

void CTimePlot::SetRealTimeRepeat4MovingRegressionIndicator(int win, int step)
{
	if(m_pData->thirdMovingRegressionSamples.empty())
		return;

	CIndicator ind(m_pData->thirdMovingRegressionSamples);

	std::vector< MovingAveragePoint > samples = ind.GetRealTimeMovingRegression(win, step);

	// вектор средних:
	m_pData->thirdMovingRegressionSamples.clear();
	for(int i = 0; i < samples.size(); i++)
		m_pData->fourthMovingRegressionSamples.push_back( std::pair< double, double >( samples[i].x, samples[i].movingAverage ) );

	// вектор углов наклона изменения средних:
	std::vector< std::pair< double, double > > angleSamples;
	double k = 0.0;
	for(int i = 1; i < samples.size(); i++)
	{
		k = (samples[i].movingAverage - samples[i-1].movingAverage) / (samples[i].x - samples[i-1].x) ;

		angleSamples.push_back( std::pair< double, double >( samples[i].x, atan(k) ) );
	}

    QwtPlotCurve *avrCurve		= GetIndicatorLine(m_pData->fourthMovingRegressionSamples, 5, QColor(255,128,64));
	//QwtPlotCurve *dispCurve		= GetIndicatorLine(dispersionSamples, 1, QColor(Qt::darkGreen));
	QwtPlotCurve *angleCurve	= GetIndicatorLine(angleSamples, 2, QColor(Qt::darkBlue));

	avrCurve->attach(this);

	enableAxis( QwtPlot::yRight );

	angleCurve->setYAxis(QwtPlot::yRight);
	angleCurve->attach(this);

	replot();
}



void CTimePlot::FormPriceLine()
{
	if(m_pData->m_samples.empty())
		return;

	if(m_pData->m_priceCurve)
	{
		m_pData->m_priceCurve->detach();
		delete m_pData->m_priceCurve;
	}

	m_pData->m_priceCurve = new QwtPlotCurve();

	QVector< QPointF > points;

	double yMin = 10e9;
	double yMax = 0.0;

	for(int i = 0; i < m_pData->m_samples.size(); i++)
	{
		yMin = (m_pData->m_samples[i].second < yMin) ? m_pData->m_samples[i].second : yMin;
		yMax = (m_pData->m_samples[i].second > yMax) ? m_pData->m_samples[i].second : yMax;

		points << QPointF(m_pData->m_samples[i].first, m_pData->m_samples[i].second);
	}

	QColor color(231, 150, 14);
	color.setAlpha(190);

	QPen pen;
	pen.setColor(color);
	pen.setWidth(1);

	m_pData->m_priceCurve->setSamples(points);
	m_pData->m_priceCurve->setPen(pen);
	m_pData->m_priceCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
	m_pData->m_priceCurve->attach(this);

	// Настройки поведения графика при масштабировании:
	double xMin = m_pData->m_samples.first().first;
	double xMax = m_pData->m_samples.last().first;

	double deltaX = (xMax - xMin) / 20.0;
	double deltaY = (yMax - yMin) / 20.0;
	
	setXBottomAxis(QObject::trUtf8(""), xMin - deltaX, xMax + deltaX);
	setYLeftAxis(tr(""), yMin - deltaY, yMax + deltaY);
	setZoomBorders(xMin - deltaX, xMax + deltaX, yMin - deltaY, yMax + deltaY);
	setHardZoomBorders(true);
}

void CTimePlot::FormIncreaseLines(std::vector< std::pair< std::pair< double, double >, std::pair< double, double > > > &increaseIntervals)
{
	if(increaseIntervals.empty())
		return;

	foreach(QwtPlotCurve *curve, m_pData->m_increaseIntervalLines)
	{
		curve->detach();
		delete curve;
	}
	m_pData->m_increaseIntervalLines.clear();

	//QColor color(34, 177, 76);
	QColor color(72, 121, 28);
	QPen pen;
	pen.setColor(color);
	pen.setWidth(3);

	QVector< QPointF > points;
 
	for(int i = 0; i < increaseIntervals.size(); i++)
	{
		points.clear();
		QwtPlotCurve *curve = new QwtPlotCurve();

		points << QPointF( increaseIntervals[i].first.first, increaseIntervals[i].first.second ) << QPointF(increaseIntervals[i].second.first, increaseIntervals[i].second.second);
		curve->setSamples(points);
		curve->setPen(pen);
		curve->setRenderHint(QwtPlotItem::RenderAntialiased);

		m_pData->m_increaseIntervalLines << curve;

		curve->attach(this);
	}
}

QwtPlotCurve * CTimePlot::GetIndicatorLine(std::vector< std::pair< double, double > > samples, int index, QColor color)
{
	if(samples.empty())
		return 0;

	if(m_pData->m_indicatorMap.contains(index))
	{
		m_pData->m_indicatorMap[index]->detach();
		delete m_pData->m_indicatorMap[index];
	}

	QwtPlotCurve *indicatorCurve = new QwtPlotCurve();

	QVector< QPointF > points;

	for(int i = 0; i < samples.size(); i++)
		points << QPointF(samples[i].first, samples[i].second);

	QPen pen;
	pen.setColor(color);
	pen.setWidth(2);

	indicatorCurve->setSamples(points);
	indicatorCurve->setPen(pen);
	indicatorCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
	//m_priceCurve->attach(this);

	m_pData->m_indicatorMap.insert(index, indicatorCurve);

	return indicatorCurve;

}


void CTimePlot::FormText(std::vector< FuncInterval > &increaseIntervalVec)
{
	if(increaseIntervalVec.empty())
		return;

	if(m_pData->label)
	{
		m_pData->label->detach();
		delete m_pData->label;
	}

	IncreasePercentResult res = GetIncreasePercent( increaseIntervalVec, 0.002 );

	double v = res.increasePercentWithComission * 100.0 * 86400.0  / static_cast<double>(res.dt);

	QString string;
	string += QString("n = ") + QString::number( res.n ) + QObject::trUtf8("<br>");
	string += QString(QChar(0x0394)) + QString("t = ") + QString::number( res.dt ) + QObject::trUtf8("<br>");
	string += QString(QChar(0x03A3)) + QString("<sub>") + QString::number(1) + QString("</sub> ") + QString(QChar(0x0394)) + QString("y = ") + QString::number((res.increasePercent*100.0), 'f', 3) + QObject::trUtf8(" %<br>");
	string += QString(QChar(0x03A3)) + QString("<sub>") + QString::number(2) + QString("</sub> ") + QString(QChar(0x0394)) + QString("y = ") + QString::number((res.increasePercentWithComission*100.0), 'f', 3) + QObject::trUtf8(" %<br>");
	string += QString("v<sub>") + QString::number(2) + QString("</sub> ") + QString(" = ") + QString::number(v, 'f', 3) + QObject::trUtf8(" % / день <br>");

	m_pData->title.setText(string);

	m_pData->label = new QwtPlotTextLabel();
	m_pData->label->attach(this);
	m_pData->label->setText(m_pData->title);
}
