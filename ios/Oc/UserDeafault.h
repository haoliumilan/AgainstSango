//
//  UserDeafault.h
//  AgainstWar
//
//  Created by Liu Hao on 12-12-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_UserDeafault_h
#define AgainstWar_UserDeafault_h

#include "cocos2d.h"

using namespace cocos2d;

class UserDefault
{
public:
    UserDefault();
    ~UserDefault();
    
    static UserDefault* Inst();
    
    void setIsLock(const char *ucid, bool isLock);
    bool getIsLock(const char *ucid);
    
    void setBoolValueForKey(bool bValue, const char* key);
    bool getBoolValueForKey(const char* key);
    
    void setIntValueForKey(int bValue, const char* key);
    int getIntValueForKey(const char* key);
    
    void setFloatValueForKey(float fValue, const char* key);
    float getFloatValueForKey(const char* key);
    
    void setValueForKey(const char* value, const char* key);
    const char* getValueForKey(const char* key);
    
    void removeValueForKey(const char* key);

    void synchronizeDefaultData();
private:
    static UserDefault* s_pcIf;
    
};

#endif
