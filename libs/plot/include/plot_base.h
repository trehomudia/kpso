#ifndef PLOT_BASE_H
#define PLOT_BASE_H

#include "plot_api.h"
#include "qwt/qwt_plot.h"

class QwtPlotCurve;
class QwtPlotMarker;
class QwtPlotItem;
class QProgressBar;
class QwtPlotGrid;
class CViewingPlotPicker;
class CSelectingPlotPicker;

typedef enum
{
	SELECT,
	CUT
} SELECTION_MODE;

class PLOT_API PlotBase : public QwtPlot
{
Q_OBJECT

private slots:

	void onCorrectSelectorSelection(QPolygon& pa);
	void onCorrectCutterSelection(QPolygon& pa);

signals:

	void pointSelected(const QPointF&, bool ctrlPressed);
	void pointDoubleClicked(const QPointF&);

	void correctSelection(QVector<QPointF>& pv, SELECTION_MODE mode);
	void smallAmountOfPoints();
	void badSelectionEnd();

	void resizedHorizontally(const QwtPlot* w, const QResizeEvent* e);
	void resizedVertically(const QwtPlot* w, const QResizeEvent* e);

public:

	typedef enum
	{
		NONE_FORMAT,
		SIMPLE_FORMAT,
		INTEGER_FORMAT,
		TIME_FORMAT
	} SCALE_FORMAT;

	typedef enum
	{
		SIMPLE_TYPE,
		LOGARITHMIC_TYPE
	} SCALE_TYPE;

	typedef enum
	{
		NONE_SELECTED_OBJECT_TYPE = 0,
		CURVE_SELECTED_OBJECT_TYPE = 1,
		MARKER_SELECTED_OBJECT_TYPE = 2
	} SELECTED_OBJECT_TYPE;

	struct CSelectedObjectDescription
	{
		SELECTED_OBJECT_TYPE type;
		QString name;
		QwtPlotItem* item;
		int pointIndex;

		CSelectedObjectDescription();
		inline bool operator==(const CSelectedObjectDescription &other);
	};

	PlotBase(QWidget* parent = 0);
	~PlotBase();

	void setSelectionEnabled(const bool enabled);
	
	virtual void setYLeftAxis(const QString& title, double minValue, double maxValue, const SCALE_FORMAT format = SIMPLE_FORMAT, const SCALE_TYPE type = SIMPLE_TYPE);
	virtual void setXBottomAxis(const QString& title, double minValue, double maxValue, const SCALE_FORMAT format = SIMPLE_FORMAT, const SCALE_TYPE type = SIMPLE_TYPE);
	virtual void setYLeftAxisScale(double minValue, double maxValue);
	virtual void setXBottomAxisScale(double minValue, double maxValue);

	void addCurve(QwtPlotCurve* curve, const QString& name = "Noname");
	void removeCurve(QwtPlotCurve* curve, const QString& name);
	void setCurveSelectionTypes(const QSet<QString>& value);

	void addMarker(QwtPlotMarker* marker, const QString& name = "Noname");
	void removeMarker(QwtPlotMarker* marker, const QString& name);
	void setMarkerSelectionTypes(const QSet<QString>& value);

	void removeAllCurves();
	void removeAllMarkers();
	
	int getXPixelValue(double xValue) const;
	int getYPixelValue(double yValue) const;
	
	void replotWithProgress();
	QProgressBar* getProgressBar();

	QRectF getRectScale() const;

	virtual bool event(QEvent* e);

	const CSelectedObjectDescription& getSelectedObject(const QPointF& p);

protected:

	virtual void mouseMoveEvent(QMouseEvent* pe);
	virtual void mouseReleaseEvent(QMouseEvent* pe);
	virtual void mouseDoubleClickEvent(QMouseEvent* pe);
	virtual void keyReleaseEvent(QKeyEvent*);
	virtual void resizeEvent(QResizeEvent* e);

	QwtPlotGrid* m_plotterGrid;
	CViewingPlotPicker* m_plotterPicker;
	CSelectingPlotPicker* m_plotterSelector;
	CSelectingPlotPicker* m_plotterCutter;
	QProgressBar* m_pProgress;

	QList<QwtPlotCurve*> m_curveList;
	QMap<QString, QList<QwtPlotCurve*> > m_curveMap;
	QSet<QString> m_curveSet;

	QList<QwtPlotMarker*> m_markerList;
	QMap<QString, QList<QwtPlotMarker*> > m_markerMap;
	QSet<QString> m_markerSet;
	
	double m_yLeftMin;
	double m_yLeftMax;
	double m_xBottomMin;
	double m_xBottomMax;
	
	bool m_selectionEnabled;

	int getRoundedStep(const int step);

	void createProgressBar();
	void createGrid();
	void createPicker();
	void createSelector();
	void createCutter();

private:

	QPointF m_lastSelectedPoint;
	CSelectedObjectDescription m_lastSelectedObject;
	QMap<double, QList<CSelectedObjectDescription> > m_selectedObjects;

	void findSelectedObjects(const QPointF& p);
	bool isCurveSelected(const QwtPlotCurve* curve, const QPointF& p, double& d, int& index);
	bool isMarkerSelected(const QwtPlotMarker* marker, const QPointF& p, double& d);
	inline double getDistanceToPoint(const QPointF& p1, const QPointF& p2) const;
	inline double getDistanceToSegment(const QPointF& p, const QPointF& p1, const QPointF& p2) const;
};

#endif
