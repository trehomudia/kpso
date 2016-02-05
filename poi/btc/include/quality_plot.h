#ifndef QUALITY_PLOT_H
#define QUALITY_PLOT_H

#include "plot/plot.h"

class CQualityPlot : public Plot
{
	Q_OBJECT

public:

	CQualityPlot(QWidget *parent = 0);

	~CQualityPlot();

	void SetData(QVector< QPair< double, double > > samples);

	void SetAddData(QVector< QPair< double, double > > samples);

private:

//	void FormLine();

	QwtPlotCurve	*m_curve;
	QwtPlotCurve	*m_addCurve;

};

#endif // QUALITY_PLOT_H
