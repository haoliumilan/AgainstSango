//
//  RwdHuodongLayer.h
//  AgainstWar
//
//  Created by user on 13-8-8.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__RwdHuodongLayer__
#define __AgainstWar__RwdHuodongLayer__

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "CListLayer.h"
#include "CMenuBtn.h"
#include "CommDlg.h"
#include "MainScene.h"

USING_NS_CC;

class RwdHuodongRet : public CCObject
{
public:
    int    m_idx;
};

class RwdHuodongLayer : public CCLayer
{
    CCObject*               m_pListener;
    SEL_CallFuncO           m_pfnSelector;
    RwdHuodongRet           m_ret;
    
    CListLayer*             m_BonusList;
    
    string                  m_awardDsp;
    
    Dialog1*                m_dlgAlert;
    CommDlg*                m_commDlg;
    int                     m_iListSel;
    
    CCSprite* m_background;
    
    void showBonusList();
    void cbList(CCObject* pObj);
    void insertItemByIdx(CCObject* pObj);
    void cbGetRwdItem(CCObject* pObj);
    
    void update(ccTime dt);
    void cbCommDlg(CCObject* pObj);
    void showAlertGetSuccessDlg();
    void cbAlertDlg();
    void GetRwdItem();
    
    void showAlertNoTaskDlg();
    
public:
    
    
    RwdHuodongLayer(CCObject* target, SEL_CallFuncO selector);
    ~RwdHuodongLayer();
    
};

#endif /* defined(__AgainstWar__RwdHuodongLayer__) */
