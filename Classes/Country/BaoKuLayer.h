//
//  BaoKuLayer.h
//  AgainstWar
//
//  Created by user on 13-7-18.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__BaoKuLayer__
#define __AgainstWar__BaoKuLayer__

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
#include "TipsPopInfo.h"

USING_NS_CC;

enum enBaoKuReqType
{
    enReqBaoKuItemBuy = 0,
};


class BaoKuRet : public CCObject
{
public:
    int     iBtnSel;
};

class UseResultDialog;
class CommDlg;
class Dialog1;

class BaoKuLayer : public CCLayer, CCTextFieldDelegate
{
    CCObject*               m_pListener;
    SEL_CallFuncO           m_pfnSelector;
    BaoKuRet                m_ret;
    
    CListLayer*             m_listBaoKu;      //宝库列表
    Dialog2*                m_dlgTip;
    Dialog1*                m_dlgAlert;
    
    string                  m_strCurItemId;
    long                    m_lCurbuyNum;
    CMenuBtn*               m_pCurMBtn;
    
    CCTextFieldTTF*         m_textNumFiled;
    int                     m_iBtnSel;
    
    CommDlg*                m_commDlg;
    enBaoKuReqType          m_curReqType;
    
    int m_iTouchSel;
    
    void update(ccTime dt);
    void cbCommDlg(CCObject* pObj);
    void showAlertBuySuccessDlg();
    void cbAlertDlg();
    
    void insertBaoKuItemByIdx(CCObject* pObj);
    void showBaoKuList();
    
    void cbListRemove(CCObject* pObj);
    void cbListEnd(CCObject* pObj);
    void cbListBegin(CCObject* pObj);
    void cbList(CCObject* pObj);
    void cbBuyItem(CCObject* pObj);
    void ShowCanNotBuy(int iType);
    void cbCanNotBuy(CCObject *pObj);
    void BuyGuildItemShop();
    
    void moveDown();
    void moveUp(float offy);
    
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    virtual void afterUpdate();
    
    
public:
    BaoKuLayer(CCObject* target, SEL_CallFuncO selector);
    ~BaoKuLayer();
};


#endif /* defined(__AgainstWar__BaoKuLayer__) */
