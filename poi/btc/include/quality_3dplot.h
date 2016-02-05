#ifndef QUALITY_3D_PLOT_H
#define QUALITY_3D_PLOT_H

#include <math.h>
#include "qwtplot3d/qwt3d_surfaceplot.h"
#include "qwtplot3d/qwt3d_function.h"


class Plot3D : public Qwt3D::SurfacePlot
{

public:

  Plot3D(QWidget *parent = 0);

 void SetData(QMap< QPair< int, int >, double > &points, int nx, int ny, int dx, int dy);

};





#endif // QUALITY_3D_PLOT_H
