//
//  RwdLevelUpLayer.h
//  AgainstWar
//
//  Created by user on 13-8-8.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__RwdLevelUpLayer__
#define __AgainstWar__RwdLevelUpLayer__

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "CListLayer.h"
#include "CMenuBtn.h"
#include "CommDlg.h"
#include "Dialog1.h"
#include "MainScene.h"
#include "NoFriendLayer.h"

USING_NS_CC;

class RwdLevelUpRet : public CCObject
{
public:
    int    m_idx;
};

class RwdLevelUpLayer : public CCLayer
{
    CCObject*               m_pListener;
    SEL_CallFuncO           m_pfnSelector;
    RwdLevelUpRet           m_ret;
    
    Dialog1*                m_dlgAlert;
    CommDlg*                m_commDlg;
    int                     m_iListSel;
    
    string                  m_awardDsp;
    
    CListLayer*             m_LevelUpList;
    
    NoFriendLayer*          m_dlgNoTip;
    
    void update(ccTime dt);
    void cbCommDlg(CCObject* pObj);
    void showAlertGetSuccessDlg();
    void cbAlertDlg();
    
    void showLevelUpList();
    void cbList(CCObject* pObj);
    void insertItemByIdx(CCObject* pObj);
    void cbGetRwdItem(CCObject* pObj);
    void GetRwdItem();
    
    void showAlertNoCombineDlg();
    void cbNoCombineDlg();
public:
    
    
    RwdLevelUpLayer(CCObject* target, SEL_CallFuncO selector);
    ~RwdLevelUpLayer();
    
};

#endif /* defined(__AgainstWar__RwdLevelUpLayer__) */
