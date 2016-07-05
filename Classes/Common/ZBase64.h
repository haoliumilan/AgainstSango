//
//  ZBase64.h
//  AgainstWar
//
//  Created by 王 海桅 on 12-12-11.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__ZBase64__
#define __AgainstWar__ZBase64__

#include "cocos2d.h"

class ZBase64
{
private:
    
    //Base64编码解码表
    const char* m_Base64_Table;
public:
    //构造
    ZBase64();
    //编码
    string EncodeBase64(const string strSource);
    //解码
    string DecodeBase64(const string strSource);
};

#endif /* defined(__AgainstWar__ZBase64__) */
