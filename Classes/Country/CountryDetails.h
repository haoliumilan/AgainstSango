//
//  CountryDetails.h
//  AgainstWar
//
//  Created by Liu Hao on 13-1-6.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__CountryDetails__
#define __AgainstWar__CountryDetails__

#include <iostream>
#include "cocos2d.h"
#include "CGameData.h"

#define enCDBtn_Back        1001
#define enCDBtn_LvUp        1002
#define enCDBtn_BuffUp      1003
#define enCDBtn_Apply       1004
#define enCDBtn_Quit        1005
#define enCDBtn_Manage      1006

using namespace cocos2d;

class CountryDetails : public CCLayer
{
public:
    CountryDetails(CCObject* target, SEL_MenuHandler selector);
    ~CountryDetails();
    
    void showCountryDetails();  // user is one of the country
    void showOneCntryDetails(CCntryBaseInfo* cntryInfo); // user is not one of the country

private:
    virtual void onEnter();
    virtual void onExit();
    
    void newBtn(CCMenuItem *&menuItem, const char *label,
                const CCPoint& position, int tag);
    
    void btnCallback(CCObject* pObj);
    
    void showLeaderBtn();
    void showMemberBtn(bool bCntrySelf);// bCntrySelf 是否是自己的国家详情
    void showNormalBtn(bool bApply);    // bApply 这个国家是否已经被申请过了
    
    void showCntryProperty(const char* title, const char* value, float posY);
    void showBuffEffect(const char* iconName, const char* effect, const CCPoint& position);

private:
    CCObject*           m_pListener;
    SEL_MenuHandler     m_pfnSelector;

};

#endif /* defined(__AgainstWar__CountryDetails__) */
