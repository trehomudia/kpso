#ifndef QWT_PLOT_WD_TIME_SCALE_DRAW_H
#define QWT_PLOT_WD_TIME_SCALE_DRAW_H

#include "qwt/qwt_scale_draw.h"

class CTimeScaleDraw : public QwtScaleDraw
{
public:

	CTimeScaleDraw();

	QwtText label(double v) const;

private:

};

#endif
