#include "DTK.h"
#include "DTKinternal.h"
#include <QDateTime>

////////////////////////////////////////////////////////////////////////////////////////////////////////////

DateTime::DateTime(void)
{
	memset(this, 0, sizeof(tm));
}

DateTime::DateTime(int year, int month, int day)
{
	memset(this, 0, sizeof(tm));

	tm_year = year;
	tm_mon = month;
	tm_mday = day;
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second)
{
	memset(this, 0, sizeof(tm));

	tm_year = year;
	tm_mon = month;
	tm_mday = day;
	tm_hour = hour;
	tm_min = minute;
	tm_sec = second;
}

String DateTime::format(const String& format) const
{
	char buffer[1000];
	if (strftime(buffer, sizeof(buffer)-1, format.c_str(), this) > 0)
		return buffer;
	else
		return "";
}

QString DateTime::formatQ(const QString& format) const
{
	return QDateTime(QDate(tm_year + 1900, tm_mon + 1, tm_mday), QTime(tm_hour, tm_min, tm_sec)).toString(format);
}

DateTime DateTime::currentDateTime()
{
	DateTime dt;
	time_t t = time(NULL);
	localtime_s(&dt, &t);
	return dt;
}
