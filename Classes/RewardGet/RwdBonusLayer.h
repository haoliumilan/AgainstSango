//
//  RwdBonusLayer.h
//  AgainstWar
//
//  Created by user on 13-8-8.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__RwdBonusLayer__
#define __AgainstWar__RwdBonusLayer__

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "CListLayer.h"
#include "CMenuBtn.h"
#include "CommDlg.h"
#include "MainScene.h"
#include "NoFriendLayer.h"

USING_NS_CC;

class RwdBonusRet : public CCObject
{
public:
    int    m_idx;
};

class RwdBonusLayer : public CCLayer
{
    CCObject*               m_pListener;
    SEL_CallFuncO           m_pfnSelector;
    RwdBonusRet             m_ret;
    
    Dialog1*                m_dlgAlert;
    CommDlg*                m_commDlg;
    int                     m_iListSel;
    
    string                  m_awardDsp;
    
    CListLayer*             m_BonusList;
    
    NoFriendLayer*          m_dlgNoTip;
    
    void update(ccTime dt);
    void cbCommDlg(CCObject* pObj);
    void showAlertGetSuccessDlg();
    void cbAlertDlg();
    
    void showBonusList();
    void cbList(CCObject* pObj);
    void insertItemByIdx(CCObject* pObj);
    void cbGetRwdItem(CCObject* pObj);
    void GetRwdItem();
    
    void showAlertNoCombineDlg();
    void cbNoCombineDlg();
    
public:
    
    
    RwdBonusLayer(CCObject* target, SEL_CallFuncO selector);
    ~RwdBonusLayer();
    
};

#endif /* defined(__AgainstWar__RwdBonusLayer__) */
