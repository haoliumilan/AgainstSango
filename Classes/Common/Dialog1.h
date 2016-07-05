//
//  Dialog1.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-12.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_Dialog1_h
#define AgainstWar_Dialog1_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class Dialog1Ret : public CCObject
{
public:
    bool bOk;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class Dialog1 : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    Dialog1Ret      m_ret;
    
public:
    Dialog1(CCObject* target, SEL_CallFuncO selector, const char* dsp, bool isWelcomeDlg );
    
    Dialog1(CCObject* target, SEL_CallFuncO selector, const char* dsp);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    virtual void onExit();
    
    void setBtnName(string s_bName);
private:
    
};

#endif
