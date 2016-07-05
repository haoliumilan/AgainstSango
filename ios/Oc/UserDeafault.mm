//
//  UserDeafault.cpp
//  AgainstWar
//
//  Created by Liu Hao on 12-12-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "UserDeafault.h"

UserDefault* UserDefault::s_pcIf = NULL;

UserDefault::UserDefault()
{

}


UserDefault::~UserDefault()
{

}

UserDefault* UserDefault::Inst()
{
    if(s_pcIf == NULL){
        s_pcIf = new UserDefault();
    }
    
    return s_pcIf;
}

void UserDefault::setIsLock(const char *ucid, bool isLock)
{
    NSString *strUcid = [NSString stringWithFormat:@"%s", ucid];
    [[NSUserDefaults standardUserDefaults] setBool:isLock forKey:strUcid];
    [[NSUserDefaults standardUserDefaults] synchronize];
}

bool UserDefault::getIsLock(const char *ucid)
{
    NSString *strUcid = [NSString stringWithFormat:@"%s", ucid];
    NSString *value = [[NSUserDefaults standardUserDefaults] valueForKey:strUcid];
    if (value) {
        return [value boolValue];
    }
    else {
        return false;
    }
    return false;
}

void UserDefault::setBoolValueForKey(bool bValue, const char *key)
{
    NSString *strKey = [NSString stringWithFormat:@"%s", key];
    [[NSUserDefaults standardUserDefaults] setBool:bValue forKey:strKey];
}

bool UserDefault::getBoolValueForKey(const char *key)
{
    NSString * strKey = [NSString stringWithFormat:@"%s", key];
    bool bValue = [[NSUserDefaults standardUserDefaults] boolForKey:strKey];
    return bValue;
}

void UserDefault::setIntValueForKey(int bValue, const char *key)
{
    NSString *strKey = [NSString stringWithFormat:@"%s", key];
    [[NSUserDefaults standardUserDefaults] setInteger:bValue forKey:strKey];
}

int UserDefault::getIntValueForKey(const char *key)
{
    NSString * strKey = [NSString stringWithFormat:@"%s", key];
    int bValue = [[NSUserDefaults standardUserDefaults] integerForKey:strKey];
    return bValue;
}

void UserDefault::setFloatValueForKey(float fValue, const char* key)
{
    NSString *strKey = [NSString stringWithFormat:@"%s", key];
    [[NSUserDefaults standardUserDefaults] setFloat:fValue forKey:strKey];
}

float UserDefault::getFloatValueForKey(const char* key)
{
    NSString *strKey = [NSString stringWithFormat:@"%s", key];
    float fValue = [[NSUserDefaults standardUserDefaults] floatForKey:strKey];
    return fValue;
}

void UserDefault::setValueForKey(const char *value, const char *key)
{
    NSString *strValue = [NSString stringWithFormat:@"%s", value];
    NSString *strKey = [NSString stringWithFormat:@"%s", key];
    [[NSUserDefaults standardUserDefaults] setObject:strValue forKey:strKey];
}

const char* UserDefault::getValueForKey(const char *key)
{
    NSString *strKey = [NSString stringWithFormat:@"%s", key];
    NSString *strValue = [[NSUserDefaults standardUserDefaults] objectForKey:strKey];
    
    if(strValue)
        return [strValue UTF8String];
    else
        return "";
}

void UserDefault::removeValueForKey(const char *key)
{
    NSString *strKey = [NSString stringWithFormat:@"%s", key];
    [[NSUserDefaults standardUserDefaults] removeObjectForKey:strKey];
}

void UserDefault::synchronizeDefaultData()
{
    [[NSUserDefaults standardUserDefaults] synchronize];
}




