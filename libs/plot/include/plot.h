#ifndef PLOT_H
#define PLOT_H

#include "plot_base.h"

class QwtPlotZoomer;

class PLOT_API Plot : public PlotBase
{
Q_OBJECT

private slots:

	void onZoomed(const QRectF& rect);

public:

	Plot(QWidget* parent = 0);
	~Plot();

	void setYLeftAxis(const QString& title, double minValue, double maxValue, const SCALE_FORMAT format = SIMPLE_FORMAT, const SCALE_TYPE type = SIMPLE_TYPE);
	void setXBottomAxis(const QString& title, double minValue, double maxValue, const SCALE_FORMAT format = SIMPLE_FORMAT, const SCALE_TYPE type = SIMPLE_TYPE);
	void setYLeftAxisScale(double minValue, double maxValue);
	void setXBottomAxisScale(double minValue, double maxValue);
	
	void setZoomBorders(const double xMin, const double xMax, const double yMin, const double yMax);

	void zoom(const double xMin, const double xMax, const double yMin, const double yMax);
	
	void setHardZoomBorders(bool value);
	bool getHardZoomBorders() const;

	virtual bool event(QEvent* e);

protected:
	void removeZoomer();
	void createZoomer();
	virtual void mouseReleaseEvent(QMouseEvent* pe);
	virtual void keyPressEvent(QKeyEvent*);

private:

	QwtPlotZoomer* m_plotterZoomer;
	
	bool m_zoomed;
	bool m_hardZoomBorders;

	void initialZoom();
};

#endif
