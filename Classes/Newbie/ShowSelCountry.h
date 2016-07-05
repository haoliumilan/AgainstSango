//
//  ShowSelCountry.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-12.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_ShowSelCountry_h
#define AgainstWar_ShowSelCountry_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class ShowSelCtryRet : public CCObject
{
public:
    bool bOk;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class ShowSelCountry : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    ShowSelCtryRet  m_ret;
    
public:
    ShowSelCountry(CCObject* target, SEL_CallFuncO selector);
    
    void show(int iCountry);

    virtual void onExit();
    
private:
    void btnCallback(CCObject* sender);
};

#endif
