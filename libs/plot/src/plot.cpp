#include "integer_scale_draw.h"
#include "time_scale_draw.h"
#include "viewing_plot_picker.h"
#include "selecting_plot_picker.h"
#include "plot.h"
#include "qwt/qwt_plot_layout.h"
#include "qwt/qwt_scale_widget.h"
#include "qwt/qwt_plot_glcanvas.h"
#include "qwt/qwt_scale_engine.h"
#include "qwt/qwt_plot_zoomer.h"
#include "qwt/qwt_plot_curve.h"
#include "qwt/qwt_plot_marker.h"
#include "qwt/qwt_plot_grid.h"
#include "qwt/qwt_legend.h"
#include "qwt/qwt_plot_canvas.h"
#include "qwt/qwt_picker_machine.h"
#include <QProgressBar>
#include <QMouseEvent>
#include <QApplication>


Plot::Plot(QWidget* parent)
: PlotBase(parent)
, m_plotterZoomer(0)
, m_hardZoomBorders(false)
{
	m_zoomed = false;

	createZoomer();
}

Plot::~Plot()
{
	if (m_plotterZoomer)
		delete m_plotterZoomer;
}

void Plot::setYLeftAxis(const QString& title, double minValue, double maxValue, const SCALE_FORMAT format, const SCALE_TYPE type)
{
	PlotBase::setYLeftAxis(title, minValue, maxValue, format, type);

	initialZoom();
}

void Plot::setXBottomAxis(const QString& title, double minValue, double maxValue, const SCALE_FORMAT format, const SCALE_TYPE type)
{
	PlotBase::setXBottomAxis(title, minValue, maxValue, format, type);

	initialZoom();
}

void Plot::setYLeftAxisScale(double minValue, double maxValue)
{
	PlotBase::setYLeftAxisScale(minValue, maxValue);

	initialZoom();
}

void Plot::setXBottomAxisScale(double minValue, double maxValue)
{
	PlotBase::setXBottomAxisScale(minValue, maxValue);

	initialZoom();
}

void Plot::onZoomed(const QRectF& rect)
{
	if (!m_plotterZoomer)
		return;

	QRectF zoomRect = rect;
	bool needReplot = false;

	if (m_hardZoomBorders)
	{
		QStack<QRectF> stack = m_plotterZoomer->zoomStack();
		QRectF last = stack.last();

		if (last.left() < m_xBottomMin)
			last.setLeft(m_xBottomMin);

		if (last.right() > m_xBottomMax)
			last.setRight(m_xBottomMax);

		double bottom = qMin(last.bottom(), last.top());
		double top = qMax(last.bottom(), last.top());

		if (bottom < m_yLeftMin)
			last = QRectF(last.left(), m_yLeftMin, last.right() - last.left(), top - m_yLeftMin);

		if (top > m_yLeftMax)
			last = QRectF(last.left(), bottom, last.right() - last.left(), m_yLeftMax - bottom);

		if (last != stack.last())
		{
			stack.pop_back();
			stack.push_back(last);
			m_plotterZoomer->setZoomStack(stack);
		}
	}

	if (m_plotterPicker->getTimeModeX())
	{
		double timeMin = zoomRect.left();
		double timeMax = zoomRect.right();

		if ((timeMax - timeMin)/60 < 10)
		{
			this->setAxisScale(QwtPlot::xBottom, timeMin, timeMax);
		}
		else
		{
			int step = int((timeMax - timeMin)/60/10);

			if (step > 60 * 6)
				step = 60 * 6;
			else
				step = getRoundedStep(step);

			double minValue = double(int(timeMin/60/(double)step) * step) * 60;
			double maxValue = double(int(timeMax/60/(double)step + 1) * step) * 60;

			this->setAxisScale(QwtPlot::xBottom, minValue, maxValue, step * 60);
		}

		needReplot = true;
	}

	if (m_plotterPicker->getTimeModeY())
	{
		double timeMin = qMin(zoomRect.bottom(), zoomRect.top());
		double timeMax = qMax(zoomRect.bottom(), zoomRect.top());

		if ((timeMax - timeMin)/60 < 10)
		{
			setAxisScale(QwtPlot::yLeft, timeMin, timeMax);
		}
		else
		{
			int step = int((timeMax - timeMin)/60/10);

			if (step > 60)
				step = 60;
			else
				step = getRoundedStep(step);

			double minValue = double(int(timeMin/60/(double)step) * step) * 60;
			double maxValue = double(int(timeMax/60/(double)step + 1) * step) * 60;

			this->setAxisScale(QwtPlot::yLeft, minValue, maxValue, step * 60);
		}

		needReplot = true;
	}

	if (needReplot)
		replot();

	m_zoomed = true;
}

void Plot::createZoomer()
{
	if (m_plotterZoomer)
	{
		disconnect(m_plotterZoomer, SIGNAL(zoomed(const QRectF&)), this, SLOT(onZoomed(const QRectF&)));
		delete m_plotterZoomer;
	}

	m_plotterZoomer = new QwtPlotZoomer(QwtPlot::xBottom, QwtPlot::yLeft, canvas());
	m_plotterZoomer->setRubberBand(QwtPicker::RectRubberBand);
	m_plotterZoomer->setRubberBandPen(QColor(Qt::black));

	m_plotterZoomer->setMousePattern(QwtEventPattern::MouseSelect1, Qt::LeftButton);
	m_plotterZoomer->setMousePattern(QwtEventPattern::MouseSelect2, Qt::NoButton);
	m_plotterZoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);
	m_plotterZoomer->setMousePattern(QwtEventPattern::MouseSelect4, Qt::NoButton);
	m_plotterZoomer->setMousePattern(QwtEventPattern::MouseSelect5, Qt::NoButton);
	m_plotterZoomer->setMousePattern(QwtEventPattern::MouseSelect6, Qt::NoButton);

	m_plotterZoomer->setKeyPattern(QwtEventPattern::KeySelect1, Qt::NoButton);
	m_plotterZoomer->setKeyPattern(QwtEventPattern::KeySelect2, Qt::NoButton);
	m_plotterZoomer->setKeyPattern(QwtEventPattern::KeyAbort, Qt::NoButton);
	m_plotterZoomer->setKeyPattern(QwtEventPattern::KeyLeft, Qt::NoButton);
	m_plotterZoomer->setKeyPattern(QwtEventPattern::KeyRight, Qt::NoButton);
	m_plotterZoomer->setKeyPattern(QwtEventPattern::KeyUp, Qt::NoButton);
	m_plotterZoomer->setKeyPattern(QwtEventPattern::KeyDown, Qt::NoButton);

	m_plotterZoomer->setResizeMode(QwtPicker::Stretch);	
	m_plotterZoomer->setTrackerMode(QwtPicker::AlwaysOff);
	m_plotterZoomer->setMaxStackDepth(100);

	connect(m_plotterZoomer, SIGNAL(zoomed(const QRectF&)), this, SLOT(onZoomed(const QRectF&)));
	initialZoom();
}

void Plot::removeZoomer()
{
	if (m_plotterZoomer)
	{
		delete m_plotterZoomer;
		m_plotterZoomer = 0;
	}
}

void Plot::initialZoom()
{
	if (!m_plotterZoomer)
		return;

	m_plotterZoomer->blockSignals(true);
	QRectF rect(1, 1, 2, 2);
	m_plotterZoomer->zoom(rect);
	m_plotterZoomer->blockSignals(false);

	QStack<QRectF> stack;
	stack.push_back(QRectF(m_xBottomMin, m_yLeftMin, m_xBottomMax - m_xBottomMin, m_yLeftMax - m_yLeftMin));
	m_plotterZoomer->setZoomStack(stack);

	if (!property("xLogarithmic").toBool() && !property("yLogarithmic").toBool())
	{
		rect = QRectF(m_xBottomMin - (m_xBottomMax - m_xBottomMin), m_yLeftMin - (m_yLeftMax - m_yLeftMin), 3 * (m_xBottomMax - m_xBottomMin), 3 * (m_yLeftMax - m_yLeftMin));
	}
	else
	{
		if (property("xLogarithmic").toBool())
		{
			int xMin = getXPixelValue(m_xBottomMin);
			int xMax = getXPixelValue(m_xBottomMax);

			QPointF pMin(m_plotterPicker->transformFromPixelToCoordinates(QPoint(xMin - (xMax - xMin), getYPixelValue(m_yLeftMin))));
			QPointF pMax(m_plotterPicker->transformFromPixelToCoordinates(QPoint(xMax + (xMax - xMin), getYPixelValue(m_yLeftMin))));

			rect = QRectF(pMin.x(), m_yLeftMin - (m_yLeftMax - m_yLeftMin), pMax.x() - pMin.x(), 3 * (m_yLeftMax - m_yLeftMin));
		}

		if (property("yLogarithmic").toBool())
		{
			int yMin = getYPixelValue(m_yLeftMin);
			int yMax = getYPixelValue(m_yLeftMax);

			QPointF pMin(m_plotterPicker->transformFromPixelToCoordinates(QPoint(getXPixelValue(m_xBottomMin), yMin - (yMax - yMin))));
			QPointF pMax(m_plotterPicker->transformFromPixelToCoordinates(QPoint(getXPixelValue(m_xBottomMin), yMax + (yMax - yMin))));

			rect = QRectF(m_xBottomMin - (m_xBottomMax - m_xBottomMin), pMin.y(), 3 * (m_xBottomMax - m_xBottomMin), pMax.y() - pMin.y());
		}
	}

	m_plotterZoomer->setZoomBase(rect);

	m_zoomed = false;
}

void Plot::setZoomBorders(const double xMin, const double xMax, const double yMin, const double yMax)
{
	double yMinTemp = yMin;
	double yMaxTemp = yMax;

	if (yMin > yMax)
	{
		yMinTemp = yMax;
		yMaxTemp = yMin;
	}

	QStack<QRectF> stack;
	stack.push_back(QRectF(m_xBottomMin, m_yLeftMin, m_xBottomMax - m_xBottomMin, m_yLeftMax - m_yLeftMin));
	m_plotterZoomer->setZoomStack(stack);

	QRectF rect;

	if (xMin < m_xBottomMin)
		rect.setLeft(xMin);
	else
		rect.setLeft(m_xBottomMin);

	if (xMax > m_xBottomMax)
		rect.setRight(xMax);
	else
		rect.setRight(m_xBottomMax);

	if (yMinTemp < m_yLeftMin)
		rect.setBottom(yMinTemp);
	else
		rect.setBottom(m_yLeftMin);

	if (yMaxTemp > m_yLeftMax)
		rect.setTop(yMaxTemp);
	else
		rect.setTop(m_yLeftMax);

	m_plotterZoomer->setZoomBase(rect);

	m_zoomed = false;
}

void Plot::zoom(const double xMin, const double xMax, const double yMin, const double yMax)
{
	QRectF rect;
	rect.setBottom(qMin(yMin, yMax));
	rect.setTop(qMax(yMin, yMax));
	rect.setLeft(qMin(xMin, xMax));
	rect.setRight(qMax(xMin, xMax));

	m_plotterZoomer->zoom(rect);
}

void Plot::setHardZoomBorders(bool value)
{
	m_hardZoomBorders = value;
}

bool Plot::getHardZoomBorders() const
{
	return m_hardZoomBorders;
}

bool Plot::event(QEvent* e)
{
	bool ok = PlotBase::event(e);

	if (e->type() == QEvent::EnabledChange)
	{
		if (m_plotterZoomer)
			m_plotterZoomer->setEnabled(isEnabled());
	}

	return ok;
}

void Plot::mouseReleaseEvent(QMouseEvent* pe)
{
	if (pe->button() == Qt::RightButton)
	{
		m_zoomed = false;
		return;
	}

	if (pe->button() != Qt::LeftButton)
		return;

	if (m_zoomed)
	{
		m_zoomed = false;
		return;
	}

	PlotBase::mouseReleaseEvent(pe);
}

void Plot::keyPressEvent(QKeyEvent* event)
{
	if (QApplication::keyboardModifiers() != Qt::NoModifier)
		return;

	QRectF rect(m_plotterZoomer->zoomRect());

	switch (event->key())
	{
	case Qt::Key_Up:
		{
			double step = rect.height()/10;
			m_plotterZoomer->moveBy(0, step);

			break;
		}
	case Qt::Key_Down:
		{
			double step = rect.height()/10;
			m_plotterZoomer->moveBy(0, -step);

			break;
		}
	case Qt::Key_Left:
		{
			double step = rect.width()/10;
			m_plotterZoomer->moveBy(-step, 0);

			break;
		}
	case Qt::Key_Right:
		{
			double step = rect.width()/10;
			m_plotterZoomer->moveBy(step, 0);

			break;
		}
	case Qt::Key_Space:
		{
			setZoomBorders(m_plotterZoomer->zoomBase().left(), m_plotterZoomer->zoomBase().right(), m_plotterZoomer->zoomBase().bottom(), m_plotterZoomer->zoomBase().top());

			break;
		}
	}
}
