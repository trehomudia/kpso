#include "quality_3dplot.h"


class QualityFunc : public Qwt3D::Function
{

public:

QualityFunc(Qwt3D::SurfacePlot* pw, QMap< QPair< int, int >, double > &points, int dx, int dy) : Qwt3D::Function(pw)
, m_points(points)
, m_dx(dx)
, m_dy(dy)
{
	QList< QPair< int, int > > arguments = m_points.keys();

	qSort(arguments);
	if(arguments.size() > 1)
		int testDx = arguments[1].first - arguments[0].first;

	m_x0 = arguments[0].first;
	m_y0 = arguments[0].second;
}

double operator()(double x, double y)
{
	int n = floor( ( x - m_x0 ) / m_dx + 0.5);

	int xVal = m_x0 + n*m_dx;

	int m = floor( ( y - m_y0) / m_dy + 0.5);

	int yVal = m_y0 + m*m_dy;

	return m_points[ QPair< int, int >(xVal, yVal) ];
}

private:

	QMap< QPair< int, int >, double > m_points;
	
	int m_dx;
	int m_dy;
	int m_x0;
	int m_y0;

};



Plot3D::Plot3D(QWidget *parent) : Qwt3D::SurfacePlot(parent)
{
//    setTitle("A Simple SurfacePlot Demonstration");
}

void Plot3D::SetData(QMap< QPair< int, int >, double > &points, int nx, int ny, int dx, int dy)
{

	QualityFunc func(this, points, dx, dy);
    
    func.setMesh(nx, ny);

	QList< QPair< int, int> > arguments = points.keys();

	qSort(arguments);

	int xStart	= arguments[0].first;
	int xEnd	= xStart + (nx-1)*dx;
	int yStart	= arguments[0].second;
	int yEnd	= yStart + (ny-1)*dy;

	func.setDomain(xStart, xEnd, yStart, yEnd);

    func.create();

    setRotation(30,0,15);
    setScale( 1.0 / (double)(xEnd-xStart)*100.0 , 1.0 / (double)(yEnd - yStart)*100.0 , 100);
    setShift(0.15,0,0);
    setZoom(0.9);

    for (unsigned i=0; i!=coordinates()->axes.size(); ++i)
    {
      coordinates()->axes[i].setMajors(7);
      coordinates()->axes[i].setMinors(4);
    }


	coordinates()->axes[Qwt3D::X1].setLabelString(QObject::trUtf8("Окно"));
    coordinates()->axes[Qwt3D::Y1].setLabelString(QObject::trUtf8("Шаг"));
    coordinates()->axes[Qwt3D::Z1].setLabelString(QString(QChar(0x03A3)) + QString(QChar(0x0394)) + QString("y"));


    setCoordinateStyle(Qwt3D::BOX);

    updateData();
    updateGL();

}
