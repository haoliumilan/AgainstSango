//
//  RwdBuchangLayer.h
//  AgainstWar
//
//  Created by caojiye on 13-8-13.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__RwdBuchangLayer__
#define __AgainstWar__RwdBuchangLayer__

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "CListLayer.h"
#include "CMenuBtn.h"
#include "CommDlg.h"
#include "MainScene.h"

USING_NS_CC;

class RwdBuchangRet : public CCObject
{
public:
    int    m_idx;
};

class RwdBuchangLayer : public CCLayer
{
    CCObject*               m_pListener;
    SEL_CallFuncO           m_pfnSelector;
    RwdBuchangRet           m_ret;
    
    CListLayer*             m_BuchangList;
    
    Dialog1*                m_dlgAlert;
    CommDlg*                m_commDlg;
    int                     m_iListSel;
    
    void showBuchangList();
    void cbList(CCObject* pObj);
    void insertItemByIdx(CCObject* pObj);
    void cbGetRwdItem(CCObject* pObj);
    
    void update(ccTime dt);
    void cbCommDlg(CCObject* pObj);
    void showAlertGetSuccessDlg();
    void cbAlertDlg();
    void GetRwdItem();
    
public:
    
    RwdBuchangLayer(CCObject* target, SEL_CallFuncO selector);
};

#endif /* defined(__AgainstWar__RwdBuchangLayer__) */
