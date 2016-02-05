#ifndef QWT_PLOT_WD_VIEWING_PLOT_PICKER_H
#define QWT_PLOT_WD_VIEWING_PLOT_PICKER_H

#include "qwt/qwt_plot_picker.h"

class CViewingPlotPicker : public QwtPlotPicker
{
Q_OBJECT

public:
	
	CViewingPlotPicker(int xAxis, int yAxis, RubberBand rubberBand, DisplayMode trackerMode, QWidget* canvas);
	~CViewingPlotPicker();

	QPointF transformFromPixelToCoordinates(const QPoint& point) const;
	QPoint transformFromCoordinatesToPixel(const QPointF& point) const;
	void setTimeModeX(bool mode);
	void setTimeModeY(bool mode);
	bool getTimeModeX();
	bool getTimeModeY();

private:

	bool m_timeModeX;
	bool m_timeModeY;

	QwtText trackerText(const QPoint&) const;
    QwtText trackerTextF(const QPointF&) const;
};

#endif

