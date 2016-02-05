#include "integer_scale_draw.h"
#include "time_scale_draw.h"
#include "viewing_plot_picker.h"
#include "selecting_plot_picker.h"
#include "plot_base.h"
#include "qwt/qwt_plot_layout.h"
#include "qwt/qwt_scale_widget.h"
#include "qwt/qwt_plot_glcanvas.h"
#include "qwt/qwt_scale_engine.h"
#include "qwt/qwt_plot_curve.h"
#include "qwt/qwt_plot_marker.h"
#include "qwt/qwt_plot_grid.h"
#include "qwt/qwt_plot_canvas.h"
#include "qwt/qwt_picker_machine.h"
#include "qwt/qwt_plot_item.h"
#include <QProgressBar>
#include <QMouseEvent>
#include <QApplication>
#include <float.h>

const double SELECTION_DISTANCE = 15.0;

PlotBase::PlotBase(QWidget* parent)
: QwtPlot(parent)
, m_pProgress(0)
, m_plotterGrid(0)
, m_plotterPicker(0)
, m_plotterSelector(0)
, m_plotterCutter(0)
, m_yLeftMin(0)
, m_yLeftMax(1000)
, m_xBottomMin(0)
, m_xBottomMax(1000)
, m_selectionEnabled(false)
{
	// Настройки плоттера
	//setCanvas(new QwtPlotGLCanvas(this));
	canvas()->setStyleSheet("QwtPlotCanvas{margin: 0px; padding: 0px; border: 2px solid transparent; border-right: 0px solid transparent; border-top: 0px solid transparent; border-radius: 0px; background-color: white;}");

	plotLayout()->setAlignCanvasToScales(false);
	plotLayout()->setCanvasMargin(0);
	plotLayout()->setSpacing(0);

	QwtScaleWidget* yAxis = axisWidget(QwtPlot::yLeft);
	yAxis->setMargin(0);
	yAxis->setSpacing(0);
	yAxis->setMouseTracking(true);

	QwtScaleWidget* xAxis = axisWidget(QwtPlot::xBottom);
	xAxis->setMargin(0);
	xAxis->setSpacing(0);
	xAxis->setMouseTracking(true);

	setMinimumSize(250, 150);

	// Создание элементов плоттера
	createProgressBar();
	createGrid();
	createPicker();

	setFocusPolicy(Qt::StrongFocus);

	m_lastSelectedPoint.setX(FLT_MAX);
	m_lastSelectedPoint.setY(FLT_MAX);
}

PlotBase::~PlotBase()
{
	if (m_pProgress)
		delete m_pProgress;

	if (m_plotterGrid)
		delete m_plotterGrid;

	if (m_plotterPicker)
		delete m_plotterPicker;

	if (m_plotterSelector)
		delete m_plotterSelector;
}

void PlotBase::setSelectionEnabled(const bool enabled)
{
	m_selectionEnabled = enabled;

	if (!enabled)
		return;

	createSelector();
	createCutter();

	connect(m_plotterSelector, SIGNAL(badSelectionEnd()), this, SIGNAL(badSelectionEnd()));
	connect(m_plotterSelector, SIGNAL(smallAmountOfPoints()), this, SIGNAL(smallAmountOfPoints()));
	connect(m_plotterSelector, SIGNAL(correctSelection(QPolygon&)), this, SLOT(onCorrectSelectorSelection(QPolygon&)));

	connect(m_plotterCutter, SIGNAL(badSelectionEnd()), this, SIGNAL(badSelectionEnd()));
	connect(m_plotterCutter, SIGNAL(smallAmountOfPoints()), this, SIGNAL(smallAmountOfPoints()));
	connect(m_plotterCutter, SIGNAL(correctSelection(QPolygon&)), this, SLOT(onCorrectCutterSelection(QPolygon&)));
}

void PlotBase::setYLeftAxis(const QString& title, double minValue, double maxValue, const SCALE_FORMAT format, const SCALE_TYPE type)
{
	setProperty("yLogarithmic", false);

	if (minValue == maxValue)
	{
		minValue -= fabs(minValue)/10.0;
		maxValue += fabs(minValue)/10.0;
	}

	switch (type)
	{
	case SIMPLE_TYPE:

		setAxisScaleEngine(QwtPlot::yLeft, new QwtLinearScaleEngine());
		
		break;

	case LOGARITHMIC_TYPE:

		setProperty("yLogarithmic", true);
		setAxisScaleEngine(QwtPlot::yLeft, new QwtLogScaleEngine());
		
		break;
	}

	setAxisTitle(QwtPlot::yLeft, title);
	setAxisScale(QwtPlot::yLeft, minValue, maxValue);

	switch (format)
	{
	case NONE_FORMAT:

		setAxisScaleDraw(QwtPlot::yLeft, new QwtScaleDraw());
		m_plotterPicker->setTimeModeY(false);
		
		break;

	case SIMPLE_FORMAT:

		setAxisMaxMajor(QwtPlot::yLeft, 10);
		setAxisMaxMinor(QwtPlot::yLeft, 10);

		setAxisScaleDraw(QwtPlot::yLeft, new QwtScaleDraw());
		m_plotterPicker->setTimeModeY(false);
		
		break;

	case INTEGER_FORMAT:

		setAxisMaxMajor(QwtPlot::yLeft, 10);
		setAxisMaxMinor(QwtPlot::yLeft, 10);

		setAxisScaleDraw(QwtPlot::yLeft, new CIntegerScaleDraw());
		m_plotterPicker->setTimeModeY(false);
		
		break;

	case TIME_FORMAT:

		setAxisMaxMajor(QwtPlot::yLeft, 5);
		setAxisMaxMinor(QwtPlot::yLeft, 10);

		setAxisScaleDraw(QwtPlot::yLeft, new CTimeScaleDraw());
		m_plotterPicker->setTimeModeY(true);

		break;
	}

	m_yLeftMin = minValue;
	m_yLeftMax = maxValue;
}

void PlotBase::setXBottomAxis(const QString& title, double minValue, double maxValue, const SCALE_FORMAT format, const SCALE_TYPE type)
{
	setProperty("xLogarithmic", false);

	if (minValue == maxValue)
	{
		minValue -= fabs(minValue)/10.0;
		maxValue += fabs(minValue)/10.0;
	}

	switch (type)
	{
	case SIMPLE_TYPE:

		setAxisScaleEngine(QwtPlot::xBottom, new QwtLinearScaleEngine());
		
		break;

	case LOGARITHMIC_TYPE:

		setProperty("xLogarithmic", true);
		setAxisScaleEngine(QwtPlot::xBottom, new QwtLogScaleEngine());
		
		break;
	}

	setAxisTitle(QwtPlot::xBottom, title);
	setAxisScale(QwtPlot::xBottom, minValue, maxValue);

	switch (format)
	{
	case NONE_FORMAT:

		setAxisScaleDraw(QwtPlot::xBottom, new QwtScaleDraw());
		m_plotterPicker->setTimeModeX(false);
		
		break;

	case SIMPLE_FORMAT:

		setAxisMaxMajor(QwtPlot::xBottom, 10);
		setAxisMaxMinor(QwtPlot::xBottom, 10);

		setAxisScaleDraw(QwtPlot::xBottom, new QwtScaleDraw());
		m_plotterPicker->setTimeModeX(false);
		
		break;

	case INTEGER_FORMAT:

		setAxisMaxMajor(QwtPlot::xBottom, 10);
		setAxisMaxMinor(QwtPlot::xBottom, 10);

		setAxisScaleDraw(QwtPlot::xBottom, new CIntegerScaleDraw());
		m_plotterPicker->setTimeModeX(false);
		
		break;

	case TIME_FORMAT:

		setAxisMaxMajor(QwtPlot::xBottom, 5);
		setAxisMaxMinor(QwtPlot::xBottom, 10);

		setAxisScaleDraw(QwtPlot::xBottom, new CTimeScaleDraw());
		m_plotterPicker->setTimeModeX(true);
		
		break;
	}

	m_xBottomMin = minValue;
	m_xBottomMax = maxValue;
}

void PlotBase::setYLeftAxisScale(double minValue, double maxValue)
{
	if (minValue == maxValue)
	{
		minValue -= fabs(minValue)/10.0;
		maxValue += fabs(minValue)/10.0;
	}

	setAxisScale(QwtPlot::yLeft, minValue, maxValue);

	m_yLeftMin = minValue;
	m_yLeftMax = maxValue;
}

void PlotBase::setXBottomAxisScale(double minValue, double maxValue)
{
	if (minValue == maxValue)
	{
		minValue -= fabs(minValue)/10.0;
		maxValue += fabs(minValue)/10.0;
	}

	setAxisScale(QwtPlot::xBottom, minValue, maxValue);

	m_xBottomMin = minValue;
	m_xBottomMax = maxValue;
}

void PlotBase::onCorrectSelectorSelection(QPolygon& pa)
{
	QVector<QPointF> pv;

	for (int i = 0; i < pa.size(); i++)
		pv.push_back(m_plotterPicker->transformFromPixelToCoordinates(pa.point(i)));

	emit(correctSelection(pv, SELECT));
}

void PlotBase::onCorrectCutterSelection(QPolygon& pa)
{
	QVector<QPointF> pv;

	for (int i = 0; i < pa.size(); i++)
		pv.push_back(m_plotterPicker->transformFromPixelToCoordinates(pa.point(i)));

	emit(correctSelection(pv, CUT));
}

void PlotBase::addCurve(QwtPlotCurve* curve, const QString& name)
{
	curve->attach(this);
	m_curveList.push_back(curve);
	m_curveMap[name].push_back(curve);
}

void PlotBase::removeCurve(QwtPlotCurve* curve, const QString& name)
{
	curve->detach();

	if (m_curveMap.contains(name))
		m_curveMap[name].removeOne(curve);
}

void PlotBase::setCurveSelectionTypes(const QSet<QString>& value)
{
	m_curveSet = value;
}

void PlotBase::addMarker(QwtPlotMarker* marker, const QString& name)
{
	marker->attach(this);
	m_markerList.push_back(marker);
	m_markerMap[name].push_back(marker);
}

void PlotBase::removeMarker(QwtPlotMarker* marker, const QString& name)
{
	marker->detach();

	if (m_markerMap.contains(name))
		m_markerMap[name].removeOne(marker);
}

void PlotBase::setMarkerSelectionTypes(const QSet<QString>& value)
{
	m_markerSet.clear();
}

void PlotBase::removeAllCurves()
{
	while (!m_curveList.isEmpty())
	{
		m_curveList[0]->detach();
		QwtPlotCurve* pCurve = m_curveList[0];
		m_curveList.removeAt(0);
		delete pCurve;
	}

	m_curveMap.clear();
}

void PlotBase::removeAllMarkers()
{
	while (!m_markerList.isEmpty())
	{
		m_markerList[0]->detach();
		QwtPlotMarker* pMarker = m_markerList[0];
		m_markerList.removeAt(0);
		delete pMarker;
	}

	m_markerMap.clear();
}

void PlotBase::createProgressBar()
{
	m_pProgress = new QProgressBar(canvas());
	m_pProgress->setVisible(false);
	m_pProgress->setGeometry(0, 0, m_pProgress->width(), 20);
	m_pProgress->setStyleSheet(QObject::tr("QProgressBar{border: 0px; text-align: center;} QDialog{background-color: rgb(245, 245, 245);}") +
		QObject::tr("QProgressBar::chunk{background-color: rgba(75, 162, 255, 255);}"));
}

void PlotBase::createGrid()
{
	m_plotterGrid = new QwtPlotGrid();
	m_plotterGrid->setMajorPen(QPen(Qt::black, 0, Qt::DotLine));
	m_plotterGrid->attach(this);
}

void PlotBase::createPicker()
{
	m_plotterPicker = new CViewingPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPicker::CrossRubberBand, QwtPicker::AlwaysOn, canvas());
	m_plotterPicker->setStateMachine(new QwtPickerClickPointMachine());
	
	m_plotterPicker->setMousePattern(QwtEventPattern::MouseSelect1, Qt::NoButton);
	m_plotterPicker->setMousePattern(QwtEventPattern::MouseSelect2, Qt::NoButton);
	m_plotterPicker->setMousePattern(QwtEventPattern::MouseSelect3, Qt::NoButton);
	m_plotterPicker->setMousePattern(QwtEventPattern::MouseSelect4, Qt::NoButton);
	m_plotterPicker->setMousePattern(QwtEventPattern::MouseSelect5, Qt::NoButton);
	m_plotterPicker->setMousePattern(QwtEventPattern::MouseSelect6, Qt::NoButton);

	m_plotterPicker->setKeyPattern(QwtEventPattern::KeySelect1, Qt::NoButton);
	m_plotterPicker->setKeyPattern(QwtEventPattern::KeySelect2, Qt::NoButton);
	m_plotterPicker->setKeyPattern(QwtEventPattern::KeyAbort, Qt::NoButton);
	m_plotterPicker->setKeyPattern(QwtEventPattern::KeyLeft, Qt::NoButton);
	m_plotterPicker->setKeyPattern(QwtEventPattern::KeyRight, Qt::NoButton);
	m_plotterPicker->setKeyPattern(QwtEventPattern::KeyUp, Qt::NoButton);
	m_plotterPicker->setKeyPattern(QwtEventPattern::KeyDown, Qt::NoButton);
	m_plotterPicker->setKeyPattern(QwtEventPattern::KeyRedo, Qt::NoButton);
	m_plotterPicker->setKeyPattern(QwtEventPattern::KeyUndo, Qt::NoButton);
	m_plotterPicker->setKeyPattern(QwtEventPattern::KeyHome, Qt::NoButton);
}

void PlotBase::createSelector()
{
	m_plotterSelector = new CSelectingPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, canvas());
	m_plotterSelector->setRubberBand(QwtPicker::PolygonRubberBand);
	m_plotterSelector->setRubberBandPen(QPen(QBrush(Qt::black), 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));

	m_plotterSelector->setMousePattern(QwtEventPattern::MouseSelect1, Qt::LeftButton, Qt::ControlModifier);
	m_plotterSelector->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::ControlModifier);
	m_plotterSelector->setMousePattern(QwtEventPattern::MouseSelect3, Qt::NoButton);
	m_plotterSelector->setMousePattern(QwtEventPattern::MouseSelect4, Qt::NoButton);
	m_plotterSelector->setMousePattern(QwtEventPattern::MouseSelect5, Qt::NoButton);
	m_plotterSelector->setMousePattern(QwtEventPattern::MouseSelect6, Qt::NoButton);

	m_plotterSelector->setKeyPattern(QwtEventPattern::KeySelect1, Qt::NoButton);
	m_plotterSelector->setKeyPattern(QwtEventPattern::KeySelect2, Qt::NoButton);
	m_plotterSelector->setKeyPattern(QwtEventPattern::KeyAbort, Qt::NoButton);
	m_plotterSelector->setKeyPattern(QwtEventPattern::KeyLeft, Qt::NoButton);
	m_plotterSelector->setKeyPattern(QwtEventPattern::KeyRight, Qt::NoButton);
	m_plotterSelector->setKeyPattern(QwtEventPattern::KeyUp, Qt::NoButton);
	m_plotterSelector->setKeyPattern(QwtEventPattern::KeyDown, Qt::NoButton);
	m_plotterSelector->setKeyPattern(QwtEventPattern::KeyRedo, Qt::NoButton);
	m_plotterSelector->setKeyPattern(QwtEventPattern::KeyUndo, Qt::NoButton);
	m_plotterSelector->setKeyPattern(QwtEventPattern::KeyHome, Qt::NoButton);
	
	m_plotterSelector->setResizeMode(QwtPicker::Stretch);
	m_plotterSelector->setTrackerMode(QwtPicker::AlwaysOff);
}

void PlotBase::createCutter()
{
	m_plotterCutter = new CSelectingPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, canvas());
	m_plotterCutter->setRubberBand(QwtPicker::PolygonRubberBand);
	m_plotterCutter->setRubberBandPen(QPen(QBrush(Qt::black), 2, Qt::DashLine, Qt::RoundCap, Qt::RoundJoin));

	m_plotterCutter->setMousePattern(QwtEventPattern::MouseSelect1, Qt::LeftButton, Qt::AltModifier);
	m_plotterCutter->setMousePattern(QwtEventPattern::MouseSelect2, Qt::RightButton, Qt::AltModifier);
	m_plotterCutter->setMousePattern(QwtEventPattern::MouseSelect3, Qt::NoButton);
	m_plotterCutter->setMousePattern(QwtEventPattern::MouseSelect4, Qt::NoButton);
	m_plotterCutter->setMousePattern(QwtEventPattern::MouseSelect5, Qt::NoButton);
	m_plotterCutter->setMousePattern(QwtEventPattern::MouseSelect6, Qt::NoButton);

	m_plotterCutter->setKeyPattern(QwtEventPattern::KeySelect1, Qt::NoButton);
	m_plotterCutter->setKeyPattern(QwtEventPattern::KeySelect2, Qt::NoButton);
	m_plotterCutter->setKeyPattern(QwtEventPattern::KeyAbort, Qt::NoButton);
	m_plotterCutter->setKeyPattern(QwtEventPattern::KeyLeft, Qt::NoButton);
	m_plotterCutter->setKeyPattern(QwtEventPattern::KeyRight, Qt::NoButton);
	m_plotterCutter->setKeyPattern(QwtEventPattern::KeyUp, Qt::NoButton);
	m_plotterCutter->setKeyPattern(QwtEventPattern::KeyDown, Qt::NoButton);
	m_plotterCutter->setKeyPattern(QwtEventPattern::KeyRedo, Qt::NoButton);
	m_plotterCutter->setKeyPattern(QwtEventPattern::KeyUndo, Qt::NoButton);
	m_plotterCutter->setKeyPattern(QwtEventPattern::KeyHome, Qt::NoButton);
	
	m_plotterCutter->setResizeMode(QwtPicker::Stretch);
	m_plotterCutter->setTrackerMode(QwtPicker::AlwaysOff);
}

int PlotBase::getXPixelValue(double xValue) const
{
	QPointF point(xValue, 0);
	QPoint p(m_plotterPicker->transformFromCoordinatesToPixel(point));

	return p.x();
}

int PlotBase::getYPixelValue(double yValue) const
{
	QPointF point(0, yValue);
	QPoint p(m_plotterPicker->transformFromCoordinatesToPixel(point));

	return p.y();
}

void PlotBase::replotWithProgress()
{	
	m_pProgress->setValue(10);
	m_pProgress->setVisible(true);
	QApplication::processEvents();

	replot();

	m_pProgress->setValue(100);
	QApplication::processEvents();
	m_pProgress->setVisible(false);
	QApplication::processEvents();
}

QProgressBar* PlotBase::getProgressBar()
{
	return m_pProgress;
}

QRectF PlotBase::getRectScale() const
{
	return QRectF(m_xBottomMin, m_yLeftMin, m_xBottomMax - m_xBottomMin, m_yLeftMax - m_yLeftMin);
}

bool PlotBase::event(QEvent* e)
{
	bool ok = QwtPlot::event(e);

	if (e->type() == QEvent::EnabledChange)
	{
		if (m_plotterPicker)
			m_plotterPicker->setEnabled(isEnabled());

		if (m_plotterSelector)
			m_plotterSelector->setEnabled(isEnabled());

		if (m_plotterCutter)
			m_plotterCutter->setEnabled(isEnabled());
	}

	return ok;
}

void PlotBase::mouseMoveEvent(QMouseEvent* pe)
{
	if ((pe->buttons() == Qt::NoButton) && (QApplication::keyboardModifiers() == Qt::NoModifier))
		QwtPlot::mouseMoveEvent(pe);
}

void PlotBase::mouseReleaseEvent(QMouseEvent* pe)
{
	QPoint point(m_plotterPicker->trackerPosition());
	QPointF p(m_plotterPicker->transformFromPixelToCoordinates(point));

	emit(pointSelected(p, QApplication::keyboardModifiers() == Qt::ControlModifier));
}

void PlotBase::mouseDoubleClickEvent(QMouseEvent* pe)
{
	if (QApplication::keyboardModifiers() != Qt::NoModifier)
		return;

	if (pe->button() != Qt::LeftButton)
		return;

	QPoint point(m_plotterPicker->trackerPosition());

	if ((point.x() == -1) && (point.y() == -1))
		return;

	QPointF p(m_plotterPicker->transformFromPixelToCoordinates(point));
	m_pProgress->setGeometry(point.x(), point.y(), m_pProgress->width(), m_pProgress->height());
	
	emit(pointDoubleClicked(p));
}

void PlotBase::keyReleaseEvent(QKeyEvent* event)
{
	switch (event->key())
	{
	case Qt::Key_Control:
		{
			if (m_selectionEnabled)
				m_plotterSelector->stop();

			break;
		}
	case Qt::Key_Alt:
		{
			if (m_selectionEnabled)
				m_plotterCutter->stop();

			break;
		}
	}
}

void PlotBase::resizeEvent(QResizeEvent* e)
{
	QwtPlot::resizeEvent(e);

	if (e->oldSize().width() != e->size().width())
		emit(resizedHorizontally(this, e));

	if (e->oldSize().height() != e->size().height())
		emit(resizedVertically(this, e));
}

int PlotBase::getRoundedStep(const int step)
{
	if (step < 1)
		return 1;

	QVector<int> steps;

	steps.push_back(1);
	steps.push_back(2);
	steps.push_back(5);
	steps.push_back(10);
	steps.push_back(15);
	steps.push_back(20);
	steps.push_back(30);
	steps.push_back(60);
	steps.push_back(60 * 6);

	for (int i = 1; i < steps.size(); i++)
	{
		if (step < steps[i])
			return (steps[i - 1]);
	}

	return 1;
}

const PlotBase::CSelectedObjectDescription& PlotBase::getSelectedObject(const QPointF& p)
{
	// Если попали в ту же точку, что и в прошлый раз (рассматреть случай с зумом перед выделением той же точки)
	if (p == m_lastSelectedPoint)
	{
		bool finded = false;

		for (QMap<double, QList<CSelectedObjectDescription> >::iterator i = m_selectedObjects.begin(); i != m_selectedObjects.end(); i++)
		{
			for (int j = 0; j < i->size(); j++)
			{
				if (finded)
				{
					m_lastSelectedObject = i.value()[j];
					finded = false;

					break;
				}

				if (i.value()[j] == m_lastSelectedObject)
					finded = true;
			}
		}

		if (finded)
			if (!m_selectedObjects.isEmpty())
				if (!m_selectedObjects.begin().value().isEmpty())
					m_lastSelectedObject = m_selectedObjects.begin().value().first();
	}
	else
	{
		findSelectedObjects(p);

		if (m_selectedObjects.isEmpty())
		{
			m_lastSelectedObject.type = NONE_SELECTED_OBJECT_TYPE;
			m_lastSelectedObject.item = 0;
		}
		else
		{
			if (!m_selectedObjects.begin().value().isEmpty())
				m_lastSelectedObject = m_selectedObjects.begin().value().first();
		}
	}

	m_lastSelectedPoint = p;

	return m_lastSelectedObject;
}

void PlotBase::findSelectedObjects(const QPointF& p)
{
	double stepCount = static_cast<double>(m_curveSet.size() + m_markerSet.size());

	if (stepCount == 0.0)
		return;

	// Очистка массива выделенных объектов
	m_selectedObjects.clear();

	const double PROGRESS_STEP = 100.0 / stepCount;

	m_pProgress->setValue(0);
	m_pProgress->setVisible(true);
	QApplication::processEvents();

	int progress = 0;
	double progressTemp = 0.0;

	// Поиск ближайшей кривой
	double distance = 0.0;
	int index = -1;
	int curveCount = 0;

	for (QSet<QString>::iterator i = m_curveSet.begin(); i != m_curveSet.end(); i++)
		curveCount += m_curveMap[*i].count();

	double progressStepTemp = PROGRESS_STEP / static_cast<double>(curveCount);

	for (QSet<QString>::iterator i = m_curveSet.begin(); i != m_curveSet.end(); i++)
	{
		for (int j = 0; j < m_curveMap[*i].size(); j++)
		{
			if (isCurveSelected(m_curveMap[*i][j], p, distance, index))
			{
				CSelectedObjectDescription object;

				object.type = CURVE_SELECTED_OBJECT_TYPE;
				object.name = *i;
				object.item = m_curveMap[*i][j];
				object.pointIndex = index;

				m_selectedObjects[distance].push_back(object);
			}

			progressTemp += progressStepTemp;

			if (static_cast<int>(progressTemp) > progress)
			{
				progress = static_cast<int>(progressTemp);
				m_pProgress->setValue(progress);
				QApplication::processEvents();
			}
		}
	}

	// Поиск ближайшего маркера
	int markerCount = 0;

	for (QSet<QString>::iterator i = m_markerSet.begin(); i != m_markerSet.end(); i++)
		markerCount += m_markerMap[*i].count();

	progressStepTemp = PROGRESS_STEP / static_cast<double>(markerCount);

	for (QSet<QString>::iterator i = m_markerSet.begin(); i != m_markerSet.end(); i++)
	{
		for (int j = 0; j < m_markerMap[*i].size(); j++)
		{
			if (isMarkerSelected(m_markerMap[*i][j], p, distance))
			{
				CSelectedObjectDescription object;

				object.type = MARKER_SELECTED_OBJECT_TYPE;
				object.name = *i;
				object.item = m_markerMap[*i][j];

				m_selectedObjects[distance].push_back(object);
			}

			progressTemp += progressStepTemp;

			if (static_cast<int>(progressTemp) > progress)
			{
				progress = static_cast<int>(progressTemp);
				m_pProgress->setValue(progress);
				QApplication::processEvents();
			}
		}
	}

	m_pProgress->setValue(100);
	QApplication::processEvents();

	m_pProgress->setVisible(false);
	QApplication::processEvents();
}

PlotBase::CSelectedObjectDescription::CSelectedObjectDescription()
{
	type = NONE_SELECTED_OBJECT_TYPE;
	name = "";
	item = 0;
	pointIndex = -1;
}

bool PlotBase::CSelectedObjectDescription::operator==(const CSelectedObjectDescription &other)
{
	if ((other.type == type) && (other.name == name) && (other.item == item) && (other.pointIndex == pointIndex))
		return true;

	return false;
}

bool PlotBase::isCurveSelected(const QwtPlotCurve* curve, const QPointF& p, double& d, int& index)
{
	const QwtSeriesData<QPointF>* data = curve->data();

	if (data == 0)
		return false;
	
	// Поиск ближайшей точки
	double distanceToClosestPoint = 1.0e10;
	double distance = -1.0;
	int closestPointIndex = -1;

	for (int i = 0; i < data->size(); i++)
	{
		distance = getDistanceToPoint(curve->sample(i), p);

		if (distance < distanceToClosestPoint)
		{
			closestPointIndex = i;
			distanceToClosestPoint = distance;
		}
	}

	// Поиск ближайшего расстояния до кривой в случае, когда отрезки между точками отображаются
	double distanceToClosestSegment = 1.0e10;
	int closestSegmentPointIndex = -1;

	if ((curve->style() != QwtPlotCurve::NoCurve) && (curve->isVisible()))
	{
		for (int i = 0; i < data->size() - 1; i++)
		{
			distance = getDistanceToSegment(p, curve->sample(i), curve->sample(i + 1));

			if (distance < 0.0)
				continue;

			if (distance < distanceToClosestSegment)
			{
				closestSegmentPointIndex = i;
				distanceToClosestSegment = distance;
			}
		}
	}

	// Выбор ближайшего элемента
	if (distanceToClosestPoint <= distanceToClosestSegment)
	{
		if (distanceToClosestPoint <= SELECTION_DISTANCE)
		{
			d = distanceToClosestPoint;
			index = closestPointIndex;

			return true;
		}
	}
	else if (distanceToClosestSegment <= SELECTION_DISTANCE)
	{
		distance = getDistanceToPoint(curve->sample(closestSegmentPointIndex), p);

		if (getDistanceToPoint(curve->sample(closestSegmentPointIndex + 1), p) < distance)
			closestSegmentPointIndex++;

		d = distanceToClosestSegment;
		index = closestSegmentPointIndex;

		return true;
	}

	return false;
}

bool PlotBase::isMarkerSelected(const QwtPlotMarker* marker, const QPointF& p, double& d)
{
	if (!marker->isVisible())
		return false;

	d = getDistanceToPoint(marker->value(), p);

	if (d <= SELECTION_DISTANCE)
		return true;

	return false;
}

double PlotBase::getDistanceToPoint(const QPointF& p1, const QPointF& p2) const
{
	return pow(pow(static_cast<double>(getXPixelValue(p1.x()) - getXPixelValue(p2.x())), 2.0) + pow(static_cast<double>(getYPixelValue(p1.y()) - getYPixelValue(p2.y())), 2.0), 0.5);
}

double PlotBase::getDistanceToSegment(const QPointF& p, const QPointF& p1, const QPointF& p2) const
{
	double x = static_cast<double>(getXPixelValue(p.x()));
	double y = static_cast<double>(getYPixelValue(p.y()));
	double x1 = static_cast<double>(getXPixelValue(p1.x()));
	double y1 = static_cast<double>(getYPixelValue(p1.y()));
	double x2 = static_cast<double>(getXPixelValue(p2.x()));
	double y2 = static_cast<double>(getYPixelValue(p2.y()));

	// Проверка на принадлежность точки к области вокруг отрезка
	if (x1 < x2)
	{
		if (y1 < y2)
		{
			if ((x < x1 - SELECTION_DISTANCE) || (x > x2 + SELECTION_DISTANCE) || (y < y1 - SELECTION_DISTANCE) || (y > y2 + SELECTION_DISTANCE))
				return -1.0;
		}
		else
		{
			if ((x < x1 - SELECTION_DISTANCE) || (x > x2 + SELECTION_DISTANCE) || (y < y2 - SELECTION_DISTANCE) || (y > y1 + SELECTION_DISTANCE))
				return -1.0;
		}
	}
	else
	{
		if (y1 < y2)
		{
			if ((x < x2 - SELECTION_DISTANCE) || (x > x1 + SELECTION_DISTANCE) || (y < y1 - SELECTION_DISTANCE) || (y > y2 + SELECTION_DISTANCE))
				return -1.0;
		}
		else
		{
			if ((x < x2 - SELECTION_DISTANCE) || (x > x1 + SELECTION_DISTANCE) || (y < y2 - SELECTION_DISTANCE) || (y > y1 + SELECTION_DISTANCE))
				return -1.0;
		}
	}

	double a = pow(pow(x - x1, 2.0) + pow(y - y1, 2.0), 0.5);
	double b = pow(pow(x - x2, 2.0) + pow(y - y2, 2.0), 0.5);
	double c = pow(pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0), 0.5);

	// Проверка на тупой угол (перпендикуляр лежит за пределами отрезка)
	if ((a * a + c * c - b * b < 0) || (b * b + c * c - a * a < 0))
		return -1.0;

	// Полупериметр
	double hp = (a + b + c) / 2.0;
	
	// Высота в треугольнике
	return 2.0 / c * pow(hp * (hp - a) * (hp - b) * (hp - c), 0.5);
}
