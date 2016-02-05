#ifndef QWT_PLOT_WD_SELECTING_PLOT_PICKER_H
#define QWT_PLOT_WD_SELECTING_PLOT_PICKER_H

#include "qwt/qwt_plot_picker.h"

class CSelectingPlotPicker : public QwtPlotPicker
{
Q_OBJECT

public:

	CSelectingPlotPicker(QWidget* canvas, bool doReplot = true );
	CSelectingPlotPicker(int xAxis, int yAxis, QWidget* canvas, bool doReplot = true);
	virtual ~CSelectingPlotPicker();

	virtual void setAxis(int xAxis, int yAxis);

	void stop();

signals:
    
	void correctSelection(QPolygon& pa);
	void smallAmountOfPoints();
	void badSelectionEnd();

protected:
    
	virtual void begin();
	virtual bool end(bool ok = true);
	virtual void append(const QPoint& point);
	
private:

	void init(bool doReplot);
	bool areSegmentsIntersected(const QPoint& p11, const QPoint& p12, const QPoint& p21, const QPoint& p22);
	int getSign(const int value);
};

#endif
