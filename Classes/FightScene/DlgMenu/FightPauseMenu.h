//
//  FightPauseMenu.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-12.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_FightPauseMenu_h
#define AgainstWar_FightPauseMenu_h

#include "cocos2d.h"
#include "SettingLayer.h"

using namespace cocos2d;

class Dialog2;
class TextNode;

class FightPauseMenuRet :public CCObject{
        
    
public:
    int iBtnSel;
}; 

class FightPauseMenu:public CCLayer {
    CCObject *m_listener;
    SEL_CallFuncO m_selctor;
    FightPauseMenuRet iRet;

private:
    CCLayerColor *coverLayer;
    SettingLayer *settingLayer;
    TextNode *  titleLabel;
    Dialog2 *exitBackground;
    CCMenu *menu2;
    
public:
    CCMenu *menu1;
    FightPauseMenu(CCObject *target,SEL_CallFuncO selector);
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);  
    void callBackMenu(CCObject *pSender);
    void exitCallBack(CCObject *pSender);
    void setCallBack(CCObject *pSender);
    void setTitleString(const char* titleStr);
    
    virtual void onExit();
};

#endif
