/*
 * TimeTransform.h
 *
 *  Created on: 2012-8-17
 *      Author: wangbin
 */
#ifndef TIMETRANSFORM_H_
#define TIMETRANSFORM_H_
#include "cocos2d.h"
#include "time.h"
#include <stdlib.h>
#include <stdio.h>

class TimeTransform {
public:
	TimeTransform();
	virtual ~TimeTransform();
	static string dateInFormat(time_t dateTime, string stringFormat);
	static time_t currentTime();
	static struct tm* timeToStruct(time_t dateTime);
	static char* transFormTime(long time);
    static char* transHourFormTime(long time);
};

#endif /* TIMETRANSFORM_H_ */
