//
//  Header.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-10-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CompGachaWeb_h
#define AgainstWar_CompGachaWeb_h

#include "cocos2d.h"
#include "CGamedata.h"

using namespace cocos2d;

class CompGachaWeb :public CUiLayerBase{
    
private:
    CCSprite *sureBtn ;
    static void compGachaWebviewLoadSuccess();
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
public:
    CompGachaWeb();
    virtual void enter();
    virtual void exit();
    
    virtual void onExit();
    
};


#endif
