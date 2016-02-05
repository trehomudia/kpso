#include "integer_scale_draw.h"

CIntegerScaleDraw::CIntegerScaleDraw()
{

}

QwtText CIntegerScaleDraw::label(double v) const
{
	return QString::number(static_cast<int>(v));
}
