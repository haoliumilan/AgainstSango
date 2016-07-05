//
//  MailLayer.cpp
//  AgainstWar
//
//  Created by caojiye on 13-1-10.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "MailLayer.h"
#include "TextNode.h"
#include "OcProxy.h"
#include "MailView.h"
#include "MainScene.h"



MailLayer::MailLayer(CCObject *listner,SEL_CallFuncO selector, bool can_reply)
{
    m_pListener   = listner;
    m_pfnSelector = selector;
    m_sender_name = "";
    m_sender_fid  = "";
    m_reply       = can_reply;
    this->SetBackground();
}

MailLayer::~MailLayer()
{

}

void MailLayer::SetSender(const char* ch)
{
    if (!m_reply)
    {
        char buf[20];
        snprintf(buf, 19, "%s", CGameData::Inst()->getLanguageValue("mail_none"));
        TextNode* text1 = TextNode::textWithString(buf, CCSizeMake(250, 30), CCTextAlignmentCenter, kBtnTitleHeight);
        text1 -> setPosition(CCPointMake(320, 660));
        text1 -> setColor(ccc3(83, 48, 15));
        addChild(text1);
    }else{
        m_sender_name = (string)ch;
        CCLabelTTF* sender_name = CCLabelTTF::labelWithString(ch, CCSizeMake(30*strlen(ch), 30), CCTextAlignmentCenter,  "SRHeitiSC-Medium", kBtnTitleHeight);
        sender_name->setPosition(CCPointMake(320, 660));
        sender_name->setColor(ccc3(83, 48, 15));
        addChild(sender_name);
    }
}
void MailLayer::SetBackground()
{
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    sp->setPosition(ccp(320, 480));
    if(sp){
        sp->setScale(2);
        addChild(sp);
    }
    item1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    item2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    item3 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    addChild(item1, 10);
    addChild(item2, 10);
    addChild(item3, 10);
    item1->setPosition(CCPointMake(150, 300));
    item2->setPosition(CCPointMake(330, 300));
    item3->setPosition(CCPointMake(510, 300));
    item1->setTag(11);
    item2->setTag(12);
    item3->setTag(13);
    char buf[20];
    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("deleteTip"));
    TextNode* label1 = TextNode::textWithString(buf, CCSizeMake(127, 30), CCTextAlignmentRight, kBtnTitleHeight);
    label1->setColor(ccWHITE);
    label1->setShadowColor(ccBLACK);
    label1->setPosition(CCPointMake(52, 42));
    item1->addChild(label1);
    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("mail_reply"));
    TextNode* label2 = TextNode::textWithString(buf, CCSizeMake(127, 30), CCTextAlignmentRight, kBtnTitleHeight);
    label2->setColor(ccWHITE);
    label2->setShadowColor(ccBLACK);
    label2->setPosition(CCPointMake(52, 42));
    item2->addChild(label2);
    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("mail_close"));
    TextNode* label3 = TextNode::textWithString(buf, CCSizeMake(127, 30), CCTextAlignmentRight, kBtnTitleHeight);
    label3->setColor(ccWHITE);
    label3->setShadowColor(ccBLACK);
    label3->setPosition(CCPointMake(54, 42));
    item3->addChild(label3);
    
    if (!m_reply)
    {
        item2 -> setPosition(CCPointMake(2000, 2000));
        item1 -> setPosition(CCPointMake(200, 300));
        item3 -> setPosition(CCPointMake(460, 300));
    }
    
}
void MailLayer::onEnter()
{
    CCLayer::onEnter();

    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -100, true);
    MainScene::Inst()->enableBottomPanel(false);
    
}
void MailLayer::onExit()
{
    MainScene::Inst()->enableBottomPanel(true);
    DeleteMail();
    
    removeAllChildrenWithCleanup(true);
    CCLayer::onExit();
}
bool MailLayer::ccTouchBegan(CCTouch* ptouch, CCEvent* pevent)
{
    CCPoint touch_location = CCDirector::sharedDirector()->convertToGL(ptouch->locationInView(ptouch->view()));
    touch_location = convertToNodeSpace(touch_location);
    
    CCPoint p1 = item1->getPosition();
    CCPoint p2 = item2->getPosition();
    CCPoint p3 = item3->getPosition();
    CCSize  s1 = item1->getContentSize();
    CCSize  s2 = item2->getContentSize();
    CCSize  s3 = item3->getContentSize();
    
    bool b1 = CCRect::CCRectContainsPoint(CCRectMake(p1.x-s1.width/2, p1.y-s1.height/2, s1.width, s1.height), touch_location);
    bool b2 = CCRect::CCRectContainsPoint(CCRectMake(p2.x-s2.width/2, p2.y-s2.height/2, s2.width, s2.height), touch_location);
    bool b3 = CCRect::CCRectContainsPoint(CCRectMake(p3.x-s3.width/2, p3.y-s3.height/2, s3.width, s3.height), touch_location);
    
    if (b1)
        TouchItem1();
    if (b2)
        TouchItem2();
    if (b3)
        TouchItem3();

    return true;
}
void MailLayer::ccTouchEnded(CCTouch* ptouch, CCEvent* pevent)
{
    
}
void MailLayer::ShowWithMail(const char* char_sender, const char* char_mail_texture)
{
    this->SetSender(char_sender);
    MailView::Inst()->ShowMailTexture(char_mail_texture);
}
void MailLayer::DeleteMail()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    MailView::Inst()->RemoveView();
}
void MailLayer::TouchItem1()
{
    m_ret.SetInt1(kMailLayerDelete);
    (m_pListener->*m_pfnSelector)(&m_ret);
}
void MailLayer::TouchItem2()
{
    m_ret.SetInt1(kMailLayerReply);
    (m_pListener->*m_pfnSelector)(&m_ret);
}
void MailLayer::TouchItem3()
{
    m_ret.SetInt1(kMailLayerOK);
    (m_pListener->*m_pfnSelector)(&m_ret);
}


//----------------------- class maillayerret -----------------------------------------------
void MailLayerRet::SetInt1(int a)
{
    int1 = a;
}
int MailLayerRet::GetInt1()
{
    return int1;
}

