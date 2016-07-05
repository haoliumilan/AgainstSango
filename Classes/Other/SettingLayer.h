//
//  SettingLayer.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-7.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_SettingLayer_h
#define AgainstWar_SettingLayer_h
#include "cocos2d.h"

using namespace cocos2d;


class CSettingLayerRet : public CCObject
{
public:
    int     iBtnSel; 
};

class SettingLayer : public CCLayer{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    CSettingLayerRet m_ret;
private: 
    CCSprite *m_backGround;
    CCMenuItem *musicOpenItem,*musicCloseItem;
    CCMenuItem *soundOpenItem,*soundCloseItem;
    CCMenuItem *skillOpenItem, *skillClostItem;
    CCMenuItem *backItem;
    bool  isFighting; 
public:
    SettingLayer(CCObject* target, SEL_CallFuncO selector);
    void enter();
    void exit();
    void setIsFighting(bool b);
    void menuCallback(CCObject* pSender);
    void musicSoundTurn(CCObject *pSender);
    void refreshSetting();
};


#endif
