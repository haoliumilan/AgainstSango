//
//  MailListItem.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-1-14.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__MailListItem__
#define __AgainstWar__MailListItem__

#include "cocos2d.h"
#include "CommonDef.h"
#include <iostream>


#define kMailRead        1001
#define kMailSender      1002


using namespace cocos2d;

class CMailSenderInf;
class CUserCardInfo;
class CShowCardInf;
class MailListItemRet : public CCObject
{
public:
    void SetInt0(int a);
    int  GetInt0();
    void SetMailText(const char* a);
    const char* GetMailText();
    void SetMailSender(const char* a);
    const char* GetMailSender();
    void SetItemCount(int a);
    int GetItemCount();
    void SetShowcardinfo(CShowCardInf* a);
    CShowCardInf* GetShowcarinfo();
private:
    int m_int0; // 1001 -> open/read the mail
    int m_item_count;   
    const char* m_mailtext;
    const char* m_mailsender;
    CShowCardInf* m_showcardinfo;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class MailListItem : public CCLayer
{
private:
    bool                m_bShowing;
    string              m_mail_text;
    string              m_mail_sender;
    MailListItemRet     m_ret;
    CMailSenderInf*     m_c_mail_sender_info;
    CUserCardInfo*      m_general_card;
    
public:
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    virtual void onEnter();
    virtual void onExit();
    MailListItem(CCObject *listner,SEL_CallFuncO selector);
    // string 替换
    string replaceString(string in_str, char src, string dest);
    
    void show(CMailSenderInf* pSendInf, const char* szMailText);
    void hide();
    virtual bool ccTouchBegan(CCTouch* ptouch, CCEvent* pevent);
    virtual void ccTouchEnded(CCTouch* ptouch, CCEvent* pevent);
    void SetMailText(const char* ch);
    void SetMailSender(const char* ch);    
};
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class NoMailLayer : public CCLayer
{
public:
    NoMailLayer(CCObject *listner,SEL_CallFuncN selector);
    
private:
    CCObject* m_plistner;
    SEL_CallFuncN m_pselector;
    void ShowNoMail();
    void cbNoMailLayer();
    
    
};


#endif /* defined(__AgainstWar__MailListItem__) */
