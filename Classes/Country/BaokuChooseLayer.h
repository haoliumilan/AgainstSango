//
//  BaokuChooseLayer.h
//  AgainstWar
//
//  Created by user on 13-7-22.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__BaokuChooseLayer__
#define __AgainstWar__BaokuChooseLayer__

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
#include "Dialog2.h"
#include "CommDlg.h"
#include "NoFriendLayer.h"
#include "TipsPopInfo.h"

USING_NS_CC;

class BaokuChooseRet : public CCObject
{
public:
    
    string          strItemId;
    string          strItemName;
    string          strImgName;
    long            lrewardNum;
    
    int             m_iType;
    
};

class BaokuChooseLayer : public CCLayer, CCTextFieldDelegate
{
    CCObject*               m_pListener;
    SEL_CallFuncO           m_pfnSelector;
    BaokuChooseRet          m_ret;
    
    CListLayer*             m_listBaoKu;      //宝库列表
    Dialog2*                m_dlgTip;
    Dialog1*                m_dlgAlert;
    
    NoFriendLayer*          m_dlgNoTip;
    
    string                  m_strCurItemId;
    long                    m_lCurbuyNum;
    CMenuBtn*               m_pCurMBtn;
    
    CCTextFieldTTF*         m_textNumFiled;
    int                     m_iBtnSel;
    int                     m_iTotal;
    
    int m_iTouchSel;

    void showAlertCanotXuanShang();
    void cbAlertDlg();
    
    void insertBaoKuItemByIdx(CCObject* pObj);
    void showBaoKuChooseList();
    
    void cbList(CCObject* pObj);
    void cbRewardItem(CCObject* pObj);
    
    void showAlertNoTaskDlg();
    void cbAlertDlg2();
    
    void cbListBegin(CCObject* pObj);
    void cbListEnd(CCObject* pObj);
    void cbListRemove(CCObject* pObj);
    
    void moveDown();
    void moveUp(float offy);
    
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    virtual void afterUpdate();
    
public:
    BaokuChooseLayer(CCObject* target, SEL_CallFuncO selector, int total);
    ~BaokuChooseLayer();
};

#endif /* defined(__AgainstWar__BaokuChooseLayer__) */
