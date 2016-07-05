//
//  MailLayer.h
//  AgainstWar
//
//  Created by caojiye on 13-1-10.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__MailLayer__
#define __AgainstWar__MailLayer__

#include <iostream>

#include "cocos2d.h"
#include "CGamedata.h"

#define kMailLayerDelete 1
#define kMailLayerReply  2
#define kMailLayerOK     3

using namespace cocos2d;
class MailLayerRet : public CCObject
{
public:
    void SetInt1(int a);
    int  GetInt1();
private:
    int     int1;

};


class MailLayer : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    bool            m_reply;
public:
    MailLayer(CCObject *listner,SEL_CallFuncO selector, bool can_reply);
    ~MailLayer();
    virtual void onEnter();
    virtual void onExit();
    void ShowWithMail(const char* char_sender, const char* char_mail_texture);
    void DeleteMail();
private:
    virtual bool ccTouchBegan(CCTouch* ptouch, CCEvent* pevent);
    virtual void ccTouchEnded(CCTouch* ptouch, CCEvent* pevent);
    void TouchItem1();
    void TouchItem2();
    void TouchItem3();
    void SetBackground();
    CCSprite* item1 = NULL;    // delete
    CCSprite* item2 = NULL;    // reply
    CCSprite* item3 = NULL;    // sure
    MailLayerRet m_ret;
    string m_sender_name;
    string m_sender_fid;
    void SetSender(const char* ch_sender);
    
};

#endif /* defined(__AgainstWar__MailLayer__) */
