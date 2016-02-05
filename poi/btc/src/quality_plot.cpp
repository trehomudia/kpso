#include "quality_plot.h"
#include "qwt/qwt_plot_curve.h"

CQualityPlot::CQualityPlot(QWidget *parent) : Plot(parent)
, m_curve(0)
, m_addCurve(0)
{

}

CQualityPlot::~CQualityPlot()
{
	if(m_curve)
		delete m_curve;

	if(m_addCurve)
		delete m_addCurve;
}

void CQualityPlot::SetData(QVector< QPair< double, double > > samples)
{
	if(samples.empty())
		return;

	if(m_curve)
	{
		m_curve->detach();
		delete m_curve;
	}

	m_curve = new QwtPlotCurve();

	QVector< QPointF > points;

	double yMin = 10e9;
	double yMax = 0.0;

	for(int i = 0; i < samples.size(); i++)
	{
		yMin = (samples[i].second < yMin) ? samples[i].second : yMin;
		yMax = (samples[i].second > yMax) ? samples[i].second : yMax;

		points << QPointF(samples[i].first, samples[i].second);
	}

	QColor color(231, 150, 14);
	//color.setAlpha(190);

	QPen pen;
	pen.setColor(color);
	pen.setWidth(1);

	m_curve->setSamples(points);
	m_curve->setPen(pen);
	m_curve->setRenderHint(QwtPlotItem::RenderAntialiased);
	m_curve->attach(this);

	// Настройки поведения графика при масштабировании:
	double xMin = samples.first().first;
	double xMax = samples.last().first;

	double deltaX = (xMax - xMin) / 20.0;
	double deltaY = (yMax - yMin) / 20.0;
	
	setXBottomAxis(QObject::trUtf8(""), xMin - deltaX, xMax + deltaX);
	setYLeftAxis(tr(""), yMin - deltaY, yMax + deltaY);
	setZoomBorders(xMin - deltaX, xMax + deltaX, yMin - deltaY, yMax + deltaY);
	setHardZoomBorders(true);

	replot();
	/**/

}

void CQualityPlot::SetAddData(QVector< QPair< double, double > > samples)
{
	if(samples.empty())
		return;

	if(m_addCurve)
	{
		m_addCurve->detach();
		delete m_addCurve;
	}

	m_addCurve = new QwtPlotCurve();

	QVector< QPointF > points;

	for(int i = 0; i < samples.size(); i++)
	{
		points << QPointF(samples[i].first, samples[i].second);
	}

	QColor color(Qt::red);
	//color.setAlpha(190);

	QPen pen;
	pen.setColor(color);
	pen.setWidth(1);

	m_addCurve->setSamples(points);
	m_addCurve->setPen(pen);
	m_addCurve->setRenderHint(QwtPlotItem::RenderAntialiased);
	m_addCurve->attach(this);

	replot();
}

//void CQualityPlot::FormLine()
//{
//
//}
