#include "time_scale_draw.h"
#include <math.h>
#include <QDateTime>

CTimeScaleDraw::CTimeScaleDraw()
{
	setLabelRotation(-15.0);
}

QwtText CTimeScaleDraw::label(double v) const
{
	double val = v;
	const QDateTime DT2006(QDate(2006, 1, 1), QTime(0, 0, 0, 0));
	qint64 seconds = qint64(v);
	v -= seconds;
	int days = seconds/86400;
	QDateTime dt = DT2006.addDays(days);
	seconds %= 86400;
	dt = dt.addSecs(seconds);
	int milliseconds = int(ceil(v * 10000.0)/10.0);
	dt = dt.addMSecs(milliseconds);
	
	if (fmod(val, 3600.0 * 24.0) == 0)
		return dt.date().toString("yyyy-MM-dd");

	if (fmod(val, 3600.0) == 0)
		return dt.time().toString("hh");

	if (fmod(val, 60.0) == 0)
		return dt.time().toString("hh:mm");

	if (milliseconds == 0)
		return dt.time().toString("hh:mm:ss");

	return dt.time().toString("hh:mm:ss:zzz");
}
