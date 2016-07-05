//
//  MailListLayer.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-1-14.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__MailListLayer__
#define __AgainstWar__MailListLayer__

#include "cocos2d.h"
#include "CommonDef.h"
#include "MailListItem.h"
#include "MailLayer.h"

using namespace cocos2d;

enum  enMailReqType
{
    en_DeleteMail  = 1001,
    en_delete_mail_all,
    enMailListLayerSendMail,
};



class CMailSenderInf;
class CScrollBar;
class CommDlg;
class Dialog2;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class MailListLayer : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;
    
    int             m_iItemCnt;
    float           m_fUpLimitY;
    
    float           m_lastYm;
    CCPoint         last_moveTouch;
    bool            m_bTouchMvFlg;
    CCPoint         m_ptStartTouch;
    CCPoint         m_ptStartGl;
    int             m_iPreThisPosY;
    
    CScrollBar*     m_pScrollBar;
    
    CommDlg*        m_commDlg;
    Dialog2*        m_dlg_delete_all_mail;
    CCMenuItem*     m_clear_all;
    
    int             m_iReqType;
    
    string          m_strMailTxt;
public:
    MailListLayer(CCObject* target, SEL_CallFunc selector);
    
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    void cbMailBtn(CCObject* a);
    void cbMailLayer(CCObject* a);
    virtual void onEnter();
    virtual void onExit();
    
private:
    MailLayer* m_maillayer;
    void ShowMailText(CCObject* o_maillistitemret);
    void setThisPosition(const CCPoint& pt, bool bRedraw = false);
    void innerExit();
    void Refresh();
    void MailReply();
    void MailReplyUpdate(ccTime dt);
    void MailDelete();
    void MailOK();
    int  m_mail_reading_now;
    int  m_general_showing_now;
    void ShowGeneral(CCObject* a);
    void inertiaMove(float dt);
    void moveY(float dt);
    void AddBackMenu();
    CCSprite* GetClearAllSprite();
    void cbMenuBack();
    void cbNoMail(int a);
    
    void update(ccTime dt);
    
    void loadMail();
    
    void cbCommDlg(CCObject* );
    
    void updateAddChild(ccTime time);
    void DeleteAllMail();
    void cbDeleteAllMail(CCObject* pObj);
};

#endif /* defined(__AgainstWar__MailListLayer__) */
