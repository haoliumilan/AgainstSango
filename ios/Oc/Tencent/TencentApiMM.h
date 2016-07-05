//
//  TencentApiMM.h
//  AgainstWar
//
//  Created by 王 海桅 on 12-12-6.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "cocos2d.h"

using namespace cocos2d;

class TencentApiMM :public  CCObject {
    
public:
    TencentApiMM();
    static TencentApiMM* Inst();
    void login();
    int getLoginStatus();
    const char* getOpenId();
    const char* getAccessToken();
    void sendWeibo(const char *str ,bool isImageExist);
    void setIsGachaWeibo(bool isOk);
    void clearOauthData();
};
