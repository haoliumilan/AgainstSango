//
//  RewardRelease.h
//  AgainstWar
//
//  Created by user on 13-7-22.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__RewardRelease__
#define __AgainstWar__RewardRelease__

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
#include "BaokuChooseLayer.h"

USING_NS_CC;

enum enXuanShangFaBuReqType
{
    enReqXuanShangFaBu = 0,
};

class RewardReleaseRet : public CCObject
{
public:
    int     iBtnSel;
};

class RewardRelease : public CCLayer,CCTextFieldDelegate
{
    CCObject*               m_pListener;
    SEL_CallFuncO           m_pfnSelector;
    RewardReleaseRet        m_ret;
    
    
    str_mubiaoGx            m_mubiaoGx;
    
    CCSprite*               m_spBG1;
    CCTextFieldTTF*         m_textfiled1;
    CCSprite*               m_spBG2;
    CCTextFieldTTF*         m_textfiled2;
    CCSprite*               m_spBG3;
    CCTextFieldTTF*         m_textfiled3;
    CCSprite*               m_spBG4;
    CCTextFieldTTF*         m_textfiled4;
    
    CCSprite*               m_spJiangli;
    CCSprite*               m_spDisMenu;
    
    BaokuChooseLayer*       m_listBaokuChoose;
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    
    void OpenATextField(int idx);
    void layerMoveUpOrDown(bool up);
    
    void drawAllChildren();
    
    void querenFabu(CCObject* pObj);
    
    void cbFromChooseBaoku(CCObject* pObj);
    
    long m_lcontribution;
    int  m_ideadLine;
    int  m_iPerson;
    string m_strItemId;
    string m_strItemName;
    int  m_iItemCount;
    int  m_iBaozhengCoin;
    
    int m_iCurChoose;
    
    CommDlg*                        m_commDlg;
    enXuanShangFaBuReqType          m_curReqType;
    void update(ccTime dt);
    void cbCommDlg(CCObject* pObj);
    
    void reqFabuXuanshang();
    void showAlertItemCount();
    void cbShowAlertItemCount(CCObject* pObj);
    
    bool bCanFabu();
    void checkButtonFabu();
    
public:
    RewardRelease(CCObject* target, SEL_CallFuncO selector);
    ~RewardRelease();
    
    void removeChooseLayer();
};



#endif /* defined(__AgainstWar__RewardRelease__) */
