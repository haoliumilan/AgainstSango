//
//  Dialog1.h
//  AgainstWar
//
//  Created by XIII on 12-6-12.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_DialogForGacha_h
#define AgainstWar_DialogForGacha_h

#include "cocos2d.h"
#include "CommonDef.h"

#define kDlg1ForGacha 0x119

using namespace cocos2d;

enum Dlg_GachaType{
    Dlg_GachaType_Null,
    Dlg_GachaType_Frd1,
    Dlg_GachaType_Frd10,
    Dlg_GachaType_Rare1,
    Dlg_GachaType_Rare10,
    Dlg_GachaType_SuiTang,
    
    Dlg_GachaType_Other,
    
    Dlg_GachaType_QianJiang,
    Dlg_GachaType_QianJiangST,
    Dlg_GachaType_QianJiangOT,
    
    Dlg_GachaType_NoMoney,

    
    
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class DialogForGachaRet : public CCObject
{
public:
    bool bOk;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class DialogForGacha : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    DialogForGachaRet      m_ret;
    
public:
    DialogForGacha(CCObject* target, SEL_CallFuncO selector, int myValue, Dlg_GachaType type);
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    virtual void onExit();
    
private:
    
};

#endif
