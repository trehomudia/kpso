#include "viewing_plot_picker.h"
#include "qwt/qwt_picker_machine.h"
#include <math.h>
#include <QDateTime>

CViewingPlotPicker::CViewingPlotPicker(int xAxis, int yAxis, RubberBand rubberBand, DisplayMode trackerMode, QWidget* canvas) : QwtPlotPicker(xAxis, yAxis, rubberBand, trackerMode, canvas)
{
	m_timeModeX = false;
	m_timeModeY = false;
}

CViewingPlotPicker::~CViewingPlotPicker()
{

}

QPointF CViewingPlotPicker::transformFromPixelToCoordinates(const QPoint& point) const
{
	return invTransform(point);
}

QPoint CViewingPlotPicker::transformFromCoordinatesToPixel(const QPointF& point) const
{
	return transform(point);
}

void CViewingPlotPicker::setTimeModeX(bool mode)
{
	m_timeModeX = mode;
}

void CViewingPlotPicker::setTimeModeY(bool mode)
{
	m_timeModeY = mode;
}

bool CViewingPlotPicker::getTimeModeX()
{
	return m_timeModeX;
}

bool CViewingPlotPicker::getTimeModeY()
{
	return m_timeModeY;
}

QwtText CViewingPlotPicker::trackerText(const QPoint& pos) const
{
    return trackerTextF(invTransform(pos));
}

QwtText CViewingPlotPicker::trackerTextF(const QPointF& pos) const
{
	const QDateTime DT2006(QDate(2006, 1, 1), QTime(0, 0, 0, 0));
    QString text;	

    switch (rubberBand())
    {
        case HLineRubberBand:
		{
			if (!m_timeModeY)
			{
				text.sprintf("%.4f", pos.y());
			}
			else
			{
				double v = pos.y();
				qint64 seconds = qint64(v);
				v -= seconds;
				int days = seconds/86400;
				QDateTime dt = DT2006.addDays(days);
				seconds %= 86400;
				dt = dt.addSecs(seconds);
				int milliseconds = int(ceil(v * 10000.0) / 10.0);
				dt = dt.addMSecs(milliseconds);
				text = dt.toString("yyyy-MM-dd hh:mm:ss.zzz");
			}

			break;
		}
        case VLineRubberBand:
		{
			if (!m_timeModeX)
			{
				text.sprintf("%.4f", pos.x());
			}
			else
			{
				double v = pos.x();
				qint64 seconds = qint64(v);
				v -= seconds;
				int days = seconds/86400;
				QDateTime dt = DT2006.addDays(days);
				seconds %= 86400;
				dt = dt.addSecs(seconds);
				int milliseconds = int(ceil(v * 10000)/10);
				dt = dt.addMSecs(milliseconds);
				text = dt.toString("yyyy-MM-dd hh:mm:ss.zzz");
			}
        
			break;
		}
        default:
		{
			if ((!m_timeModeX) && (!m_timeModeY))
			{
				text.sprintf("%.4f, %.4f", pos.x(), pos.y());
			}
			else
			{
				if (!m_timeModeX)
				{
					text.sprintf("%.4f", pos.x());
				}
				else
				{
					double v = pos.x();
					qint64 seconds = qint64(v);
					v -= seconds;
					int days = seconds/86400;
					QDateTime dt = DT2006.addDays(days);
					seconds %= 86400;
					dt = dt.addSecs(seconds);
					int milliseconds = int(ceil(v * 10000)/10);
					dt = dt.addMSecs(milliseconds);
					text = dt.toString("yyyy-MM-dd hh:mm:ss.zzz");
				}

				text += QObject::tr(", ");

				if (!m_timeModeY)
				{
					text += QString::number(pos.y(), 'f', 4);
				}
				else
				{
					double v = pos.y();
					qint64 seconds = qint64(v);
					v -= seconds;
					int days = seconds/86400;
					QDateTime dt = DT2006.addDays(days);
					seconds %= 86400;
					dt = dt.addSecs(seconds);
					int milliseconds = int(ceil(v * 10000)/10);
					dt = dt.addMSecs(milliseconds);
					text += dt.toString("yyyy-MM-dd hh:mm:ss.zzz");
				}
			}
		}
    }

    return QwtText(text);
}
