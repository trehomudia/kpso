#include "qwt/qwt_color_map.h"
#include "qwt/qwt_plot_spectrogram.h"
#include "qwt/qwt_scale_widget.h"
#include "qwt/qwt_scale_draw.h"
#include "qwt/qwt_plot_zoomer.h"
#include "qwt/qwt_plot_panner.h"
#include "qwt/qwt_plot_layout.h"
#include "qwt/qwt_plot_renderer.h"
#include "spectrogram_plot.h"


CSpectrogramPlot::CSpectrogramPlot(QWidget *parent) : QwtPlotWd(parent)
{
	m_spectrogram = new QwtPlotSpectrogram();
}

CSpectrogramPlot::~CSpectrogramPlot()
{

}

void CSpectrogramPlot::SetData(QMap< QPair< int, int >, double > points)
{
	m_points = points;
}

void CSpectrogramPlot::OnShowContour( bool on )
{

}

void CSpectrogramPlot::OnShowSpectrogram( bool on )
{

}

