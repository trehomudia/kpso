#ifndef TIME_PLOT_H
#define TIME_PLOT_H

#include "utils.h"
#include "plot/plot.h"


class CTimePlot : public Plot
{
	Q_OBJECT

public:

	CTimePlot(QWidget *parent = 0);

	~CTimePlot();

	void SetData(QVector< QPair< double, double > > samples);

	void SetMovingRegressionIndicator(int win, int polyDeg);

	void SetMovingAverageIndicator(int win, int step);

	void SetRealTimeMovingAverageIndicator(int win, int step);
	void SetRealTimeMovingRegressionIndicator(int win, int step);
	void SetRealTimeRepeatMovingRegressionIndicator(int win, int step);
	void SetRealTimeRepeat3MovingRegressionIndicator(int win, int step);
	void SetRealTimeRepeat4MovingRegressionIndicator(int win, int step);

private:

	void FormPriceLine();

	void FormIncreaseLines(std::vector< std::pair< std::pair< double, double >, std::pair< double, double > > > &increaseIntervals);

	void FormText(std::vector< FuncInterval > &increaseIntervalVec);
	

	QwtPlotCurve * GetIndicatorLine(std::vector< std::pair< double, double > > samples, int index, QColor color = Qt::red);

	

	//QVector< QPair< double, double > >	m_samples;
	//QwtPlotCurve						*m_priceCurve;
	//QMap< int, QwtPlotCurve* >			m_indicatorMap;
	//QList< QwtPlotCurve *>				m_increaseIntervalLines;

	class PrivateData;
	PrivateData* m_pData;

};

#endif // TIME_PLOT_H
