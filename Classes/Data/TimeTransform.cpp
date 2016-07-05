/*
 * TimeTransform.cpp
 *
 *  Created on: 2012-8-17
 *      Author: wangbin
 */

#include "TimeTransform.h"

TimeTransform::TimeTransform() {
	// TODO Auto-generated constructor stub

}

TimeTransform::~TimeTransform() {
	// TODO Auto-generated destructor stub
}


static char timeStr[10];
string TimeTransform::dateInFormat(time_t dateTime ,string stringFormat)
{

    char buffer[80];

    const char *format = stringFormat.c_str();

    struct tm * timeinfo;

    timeinfo = localtime(&dateTime);

    strftime(buffer, 80, format, timeinfo);

    string retStr(buffer);

    return retStr;
}

struct tm * TimeTransform::timeToStruct(time_t dateTime)
{
    static struct tm timeinfo;
    
    struct tm * curtime = localtime(&dateTime);
    
    memcpy(&timeinfo, curtime, sizeof(struct tm));
    
    return &timeinfo;
}

time_t TimeTransform::currentTime()
{
	time_t unixTime = time(NULL);
    return unixTime;
}

char * TimeTransform::transFormTime(long time)
{
//    struct tm *timeInfo;
//    timeInfo =  timeToStruct(time);
    sprintf(timeStr, "%ld:%02ld ", (time / 60) %60, time % 60);
    return timeStr;
}

char* TimeTransform::transHourFormTime(long time)
{
//    struct tm *timeInfo;
//    timeInfo = timeToStruct(1359513911);
    sprintf(timeStr, "%d:%02d:%02ld", int(time / 3600) % 24, int(time / 60) % 60 , time % 60);//timeInfo->tm_hour, timeInfo->tm_min, timeInfo->tm_sec);
    return timeStr;
}






