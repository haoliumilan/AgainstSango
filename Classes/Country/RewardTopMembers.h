//
//  RewardTopMembers.h
//  AgainstWar
//
//  Created by user on 13-8-5.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__RewardTopMembers__
#define __AgainstWar__RewardTopMembers__

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"
#include "CScrollBar.h"
#include "ItemCard.h"
#include "MainScene.h"
#include "TextNode.h"
#include "CountryDialog.h"
#include "RareGacha10Show.h"
#include "TitleBar.h"
#include "CListLayer.h"
#include "CMenuBtn.h"
#include "NoFriendLayer.h"

USING_NS_CC;

class RwdTopMemRet : public CCObject
{
public:
    int     iBtnSel;
};

class RewardTopMembers : public CCLayer
{
    CCObject*               m_pListener;
    SEL_CallFuncO           m_pfnSelector;
    RwdTopMemRet            m_ret;
    
    CListLayer*             m_topMemberList;    
    
    
    void                    cbList(CCObject* pObj);
    void                    insertBaoKuItemByIdx(CCObject* pObj);
    
    void                    showTopMemberList();
    
    NoFriendLayer*           m_dlgNoTip;
    void showAlertNoTopDlg();
    void cbAlertDlg();
    
public:
    
    RewardTopMembers(CCObject* target, SEL_CallFuncO selector);
     ~RewardTopMembers();
};


#endif /* defined(__AgainstWar__RewardTopMembers__) */






