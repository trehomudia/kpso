#ifndef QWT_PLOT_WD_INTEGER_SCALE_DRAW_H
#define QWT_PLOT_WD_INTEGER_SCALE_DRAW_H

#include "qwt/qwt_scale_draw.h"

class CIntegerScaleDraw: public QwtScaleDraw
{
public:

	CIntegerScaleDraw();

	QwtText label(double v) const;

private:

};

#endif
