//
//  RaidersLayer.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-8.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_RaidersLayer_h
#define AgainstWar_RaidersLayer_h

#include "cocos2d.h"

enum enUrlType {
    enUrlTypeHelp = 1,
    enUrlTypeAbout,
    enUrlTypeCntryHelp,
};


using namespace cocos2d;
class RaidersLayer:public CCLayer{
    
private:
public:
    void init();
    void enterWithType(enUrlType type);
    void exit();
    static void webviewLoadSuccess();
    RaidersLayer();
    
    
};

#endif
