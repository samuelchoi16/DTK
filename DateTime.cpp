#include "DTK.h"
#include "DTKinternal.h"

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
