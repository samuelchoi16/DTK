/*
 *
 *  Copyright (C) 2015, IRM Inc.
 *  All rights reserved.  See LICENSE file for details.
 *
 *  This software and supporting documentation were developed by
 *		IRM Inc., Korea.
 *  through the contract with
 *		Seoul National University Bundang Hospital
 *  under the support of
 *		Ministry of Trade, Industry and Energy, Republic of Korea.
 *
 *  Author:  Samuel Choi (samuelchoi@irm.kr)
 *
 */

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

DateTime DateTime::currentDateTime()
{
	DateTime dt;
	time_t t = time(NULL);
#ifdef	HAVE_LOCALTIME_R
	localtime_r(&t, &dt);
#else
	localtime_s(&dt, &t);
#endif
	return dt;
}
