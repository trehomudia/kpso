#include "selecting_plot_picker.h"
#include "qwt/qwt_plot.h"
#include "qwt/qwt_picker_machine.h"
#include <QApplication>

CSelectingPlotPicker::CSelectingPlotPicker(QWidget* canvas, bool doReplot) : QwtPlotPicker(canvas)
{
	if (canvas)
		init(doReplot);
}

CSelectingPlotPicker::CSelectingPlotPicker(int xAxis, int yAxis, QWidget* canvas, bool doReplot) : QwtPlotPicker(xAxis, yAxis, canvas)
{
	if (canvas)
		init(doReplot);
}

void CSelectingPlotPicker::init(bool doReplot)
{
	setTrackerMode(ActiveOnly);
	setRubberBand(PolygonRubberBand);
	setStateMachine(new QwtPickerPolygonMachine());

	if (doReplot && plot())
		plot()->replot();
}

CSelectingPlotPicker::~CSelectingPlotPicker()
{

}

void CSelectingPlotPicker::setAxis(int xAxis, int yAxis)
{
	if (xAxis != QwtPlotPicker::xAxis() || yAxis != QwtPlotPicker::yAxis())
	{
		QwtPlotPicker::setAxis(xAxis, yAxis);
	}
}

void CSelectingPlotPicker::stop()
{
	reset();
}

void CSelectingPlotPicker::begin()
{
	QwtPlotPicker::begin();
}

void CSelectingPlotPicker::append(const QPoint& point)
{
	this->setEnabled(false);
	this->blockSignals(true);
	bool append = true;

	QwtPickerMachine::Command command = QwtPickerMachine::Command(stateMachine()->state());
	QPolygon pa = selection();
	
	if (pa.size() > 0)
		pa.pop_back();
	
	const int SELECTION_SIZE = pa.size();
	
	if (SELECTION_SIZE > 2)
	{
		for (int i = 0; i < SELECTION_SIZE - 2; i++)
		{
			if (areSegmentsIntersected(pa.point(i), pa.point(i + 1), pa.point(SELECTION_SIZE - 1), point))
			{
				if (command == QwtPickerMachine::Begin)
					stateMachine()->setState(QwtPickerMachine::Append);

				append = false;
				break;
			}
		}

		if (append)
		{
			QwtPlotPicker::append(point);
		}
	}
	else
	{
		QwtPlotPicker::append(point);
	}

	this->setEnabled(true);
	this->blockSignals(true);
}

bool CSelectingPlotPicker::end(bool ok)
{
	if (!ok)
	{
		QwtPlotPicker::end(ok);
		return false;
	}

	QPolygon pa = selection();
	pa.pop_back();

	if (pa.size() > 2)
	{
		this->move(pa.point(0));
		QApplication::processEvents();
		append(pa.point(0));
	}
	else
	{
		bool block = this->signalsBlocked();
		blockSignals(false);
		emit(smallAmountOfPoints());
		blockSignals(block);
		reset();		

		return false;
	}

	QwtPickerMachine::Command command = QwtPickerMachine::Command(stateMachine()->state());

	if (command == QwtPickerMachine::Append)
	{
		bool block = this->signalsBlocked();
		blockSignals(false);
		emit(badSelectionEnd());
		blockSignals(block);
		reset();		
		
		return false;
	}

	pa.push_back(pa.point(0));
	bool block = this->signalsBlocked();
	blockSignals(false);
	emit(correctSelection(pa));
	blockSignals(block);
	ok = QwtPlotPicker::end(true);

	if (!ok)
		return false;	

    return true;
}

bool CSelectingPlotPicker::areSegmentsIntersected(const QPoint& p11, const QPoint& p12, const QPoint& p21, const QPoint& p22)
{
	double k1 = double(p12.x() - p11.x())/double(p12.y() - p11.y());
	double k2 = double(p22.x() - p21.x())/double(p22.y() - p21.y());

	// Если прямые параллельны
	if (k1 == k2)
		return false;

	int dir1x = p12.x() - p11.x();
	int dir1y = p12.y() - p11.y();
	int dir2x = p22.x() - p21.x();
	int dir2y = p22.y() - p21.y();

	// Считаем уравнения прямых проходящих через отрезки
	int a1 = -dir1y;
	int b1 = dir1x;
	int d1 = -(a1 * p11.x() + b1 * p11.y());

	int a2 = -dir2y;
	int b2 = dir2x;
	int d2 = -(a2 * p21.x() + b2 * p21.y());

	// Подставляем концы отрезков, для выяснения в каких полуплоскотях они
	int seg1_line2_start = a2 * p11.x() + b2 * p11.y() + d2;
	int seg1_line2_end = a2 * p12.x() + b2 * p12.y() + d2;

	int seg2_line1_start = a1 * p21.x() + b1 * p21.y() + d1;
	int seg2_line1_end = a1 * p22.x() + b1 * p22.y() + d1;

	// Если концы одного отрезка имеют один знак, значит он в одной полуплоскости и пересечения нет
	if ((getSign(seg1_line2_start) * getSign(seg1_line2_end) >= 0) || (getSign(seg2_line1_start) * getSign(seg2_line1_end) >= 0))
		return false;

	return true;
}

int CSelectingPlotPicker::getSign(const int value)
{
	if (value > 0)
		return 1;
	else if (value == 0)
		return 0;
	else if (value < 0)
		return -1;

	return 0;
}
