#ifndef SPECTROGRAM_PLOT_H
#define SPECTROGRAM_PLOT_H

#include "qwt_plot_wd/qwt_plot_wd.h"
#include "qwt/qwt_plot_spectrogram.h"
//#include <QVector3D>

class CSpectrogramPlot : public QwtPlotWd
{
	Q_OBJECT

public:

	CSpectrogramPlot(QWidget *parent = 0);

	~CSpectrogramPlot();

	//void SetData(QVector3D< double > points);
	void SetData(QMap< QPair< int, int >, double > points);

public Q_SLOTS:

	void OnShowContour( bool on );

	void OnShowSpectrogram( bool on );

private:

    QwtPlotSpectrogram				*m_spectrogram;

	QMap< QPair< int, int >, double > m_points;

};

#endif // SPECTROGRAM_PLOT_H