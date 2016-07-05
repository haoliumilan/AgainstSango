//
//  RwdSighLayer.h
//  AgainstWar
//
//  Created by user on 13-8-7.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__RwdSighLayer__
#define __AgainstWar__RwdSighLayer__

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "CListLayer.h"
#include "CMenuBtn.h"
#include "CommDlg.h"
#include "Dialog1.h"
#include "MainScene.h"

USING_NS_CC;

class RwdSigh : public CCObject
{
public:
    int    m_idx;
};

class RwdSighLayer : public CCLayer
{
    CCObject*               m_pListener;
    SEL_CallFuncO           m_pfnSelector;
    RwdSigh                 m_ret;
    
    CListLayer*             m_SighList;
    
    Dialog1*                m_dlgAlert;
    CommDlg*                m_commDlg;
    
    int                     m_iListSel;
    
    string                  m_awardDsp;
    
    void update(ccTime dt);
    void cbCommDlg(CCObject* pObj);
    void showAlertGetSuccessDlg();
    void cbAlertDlg();
    
    void showSighList();
    void cbList(CCObject* pObj);
    void insertItemByIdx(CCObject* pObj);
    void cbGetRwdItem(CCObject* pObj);
    void GetRwdItem();
    
public:
    
    
    RwdSighLayer(CCObject* target, SEL_CallFuncO selector);
    ~RwdSighLayer();
    
};


#endif /* defined(__AgainstWar__RwdSighLayer__) */


