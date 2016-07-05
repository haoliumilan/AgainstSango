//
//  MailListItem.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 13-1-14.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "MailListItem.h"
#include "TextNode.h"
#include "CGameData.h"
#include "MailLayer.h"
#include "MainScene.h"
#include "MailBoxAgt.h"
#include "CShowCardInf.h"


#define kMailLstItemTagBase 113 // refer to maillistlayer.cpp



//******************************************************************************
// init
//******************************************************************************

MailListItem::MailListItem(CCObject *listner,SEL_CallFuncO selector)
{
    m_pListener   = listner;
    m_pfnSelector = selector;
    m_bShowing    = false;
}
//******************************************************************************
// show
//******************************************************************************
void MailListItem::show(CMailSenderInf* pSendInf, const char* szMailText)
{
    if(m_bShowing)
        return;
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -1, true);
    this->SetMailText(szMailText);   
    this->SetMailSender(pSendInf->senderInf.name);
    this->m_c_mail_sender_info = pSendInf;
    
    // frame
    CCSprite* sp_frame =CCSprite::spriteWithSpriteFrameName("ma_colour dark .png");
    if (sp_frame)
    {
        sp_frame -> setAnchorPoint(CCPointMake(0.5, 0.5));
        sp_frame -> setPosition(CCPointZero);
        addChild(sp_frame, 0, 0);
    }
    
    // senderBG
    CCSprite* sp_senderbg = CCSprite::spriteWithSpriteFrameName("fr_black.png");
    if (sp_senderbg)
    {
        sp_senderbg -> setAnchorPoint(CCPointZero);
        sp_senderbg -> setPosition(CCPointZero);
        sp_frame  -> addChild(sp_senderbg);
    }
    
    // sender
    CCSprite* sp_sender;
    if (pSendInf->senderInf.fid[0] != 0)    
        sp_sender = CGameData::Inst()->getHeadSprite(pSendInf->senderInf.lcard_cid);
    else
        sp_sender = CCSprite::spriteWithSpriteFrameName("ma_frame_oneclick.png");
    if (sp_sender)
    {
        sp_sender -> setPosition(CCPointZero);
        sp_sender -> setAnchorPoint(CCPointZero);
        sp_frame  -> addChild(sp_sender, 0, 2);
}
    
    // button background
    CCSprite* sp_btn_bg = CCSprite::spriteWithSpriteFrameName("fr_button_recieveMail.png");
    if (sp_btn_bg)
    {
        sp_btn_bg -> setPosition(CCPointMake(410, 40));
        sp_btn_bg -> setAnchorPoint(CCPointZero);
        sp_frame  -> addChild(sp_btn_bg, 0, 1);
    }

    // send mail time
    unsigned long ulCurTime = CGameData::Inst()->getCurrentTime();
    long lTimeOffset = ulCurTime - pSendInf->senderInf.rcv_time;
    char buf[100];
    
    if(lTimeOffset < 0)
        lTimeOffset = 0;
    
    int hour = lTimeOffset / 3600;
    int minute = lTimeOffset / 60;
    int day = lTimeOffset / (3600*24);
    
    if(day > 0){
        sprintf(buf, "%d>?", day);
    }
    else if(hour > 0){
        sprintf(buf, "%d<=?", hour);
    }
    else {
        sprintf(buf, "%d:;?", minute);
    }
    
    CCSprite* spTmp = CCSprite::spriteWithFile("timenum_login.png");
    if(spTmp){
        CCSize sz = spTmp->getContentSize();
        CCLabelAtlas* lbLoginTime = CCLabelAtlas::labelWithString(buf, "timenum_login.png", sz.width / 21, sz.height, '+');
        lbLoginTime->setAnchorPoint(CCPointMake(0.5, 0.5));
        lbLoginTime->setPosition(CCPointMake(485, 30));
        sp_frame->addChild(lbLoginTime, 1);
    }

    // sender name
    CCLabelTTF* name1;
    if (pSendInf->senderInf.fid[0] != 0)
        name1 = CCLabelTTF::labelWithString(pSendInf->senderInf.name, CCSizeMake(250, 50), CCTextAlignmentLeft, "SRHeitiSC-Medium", kBtnTitleHeight-15);
    else
        name1 = CCLabelTTF::labelWithString(CGameData::Inst()->getLanguageValue("mail_none"), CCSizeMake(250, 50), CCTextAlignmentLeft, "SRHeitiSC-Medium", kBtnTitleHeight-15);
    name1    -> setPosition(CCPointMake(115, 49));
    name1    -> setAnchorPoint(CCPointZero);
    name1    -> setColor(ccc3(83, 48, 15));
    sp_frame -> addChild(name1);
 
    string subString(szMailText);
    subString = replaceString(subString, '\r', "");
    subString = replaceString(subString, '\n', "");
    // printf("--------->%s", subString.c_str());

    // 切取邮件内容前10个字符作为预览
    string ttlString = CGameData::Inst()->getFirstXChars(subString, 10);

    CCLabelTTF* label1 = CCLabelTTF::labelWithString(ttlString.c_str(), CCSizeMake(250, 50), CCTextAlignmentLeft, "SRHeitiSC-Medium",  kBtnTitleHeight-15);
    
    label1    -> setPosition(CCPointMake(110, 7));
    label1    -> setAnchorPoint(CCPointZero);
    label1    -> setColor(ccc3(185, 15, 6));
    sp_frame  -> addChild(label1);
    
    // if new mail
    if ( pSendInf->senderInf.mail_status == 1)
    {
        CCSprite* sp_new = CCSprite::spriteWithSpriteFrameName("ma_new.png");
        if (sp_new)
        {
            sp_new -> setPosition(CCPointMake(50, 70));
            sp_new ->setAnchorPoint(CCPointZero);
            sp_new ->setScale(0.8);
            sp_frame->addChild(sp_new);
            CCBlink*  b1  = CCBlink::actionWithDuration(1, 1);
            CCAction* ac1 = CCRepeatForever::actionWithAction(b1);
            sp_new -> runAction(ac1);
        }
    }
    
    m_bShowing = true;
}

// string 替换
string MailListItem::replaceString(string in_str, char src, string dest)
{
    string out_str = in_str;//初始化输出字符串
    int pos = in_str.find(src);
    while (pos != string::npos) {  //string::npos表示没查到
        out_str = in_str.replace(pos, 1, dest);//如果查到了就用函数replace替换
        //replace的一参表示要替换起始位置，二参表示要替换的长度，三参表示要替换成什么
        pos = in_str.find(src); //find表示在in_str内查找src_str
    }
    return out_str;
}

void MailListItem::onEnter()
{
    CCLayer::onEnter();
}
void MailListItem::onExit()
{
    CCLayer::onExit();
    this->hide();
}
bool MailListItem::ccTouchBegan(CCTouch* ptouch, CCEvent* pevent)
{
    CCPoint touchLocation = ptouch->locationInView( ptouch->view());
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );  // pos is the relative position to each item
    CCSize  s1 = this->getChildByTag(0)->getChildByTag(1)->getContentSize();    
    CCSize  s2 = this->getChildByTag(0)->getContentSize();                      
    CCPoint p1 = this->getChildByTag(0)->getChildByTag(1)->getPosition();
    CCSize  s3 = this->getChildByTag(0)->getChildByTag(2)->getContentSize();
    
    if (pos.x+s2.width/2 > p1.x && pos.x+s2.width/2 < p1.x+s1.width
            && pos.y+s2.height/2 > p1.y && pos.y+s2.height/2 < p1.y+s1.height )      // if click read mail
    {
        return true;
    }
    
    if (pos.x + s2.width/2 > 0 && pos.x +s2.width/2 < s3.width
        && pos.y+s2.height/2 > 0 && pos.y+s2.height/2 < s3.height )             // if click sender.png
    {
        return true;
    }
    return false;
}
void MailListItem::ccTouchEnded(CCTouch* ptouch, CCEvent* pevent)
{
    CCPoint touchLocation = ptouch->locationInView( ptouch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    CCSize  s1 = this->getChildByTag(0)->getChildByTag(1)->getContentSize();
    CCSize  s2 = this->getChildByTag(0)->getContentSize();
    CCPoint p1 = this->getChildByTag(0)->getChildByTag(1)->getPosition();
    CCSize  s3 = this->getChildByTag(0)->getChildByTag(2)->getContentSize();
    if (pos.x+s2.width/2 > p1.x && pos.x+s2.width/2 < p1.x+s1.width
        && pos.y+s2.height/2 > p1.y && pos.y+s2.height/2 < p1.y+s1.height )      // if click read mail
    {
        m_ret.SetInt0(kMailRead);
        m_ret.SetItemCount(this->getTag()-kMailLstItemTagBase);
        m_ret.SetMailText(m_mail_text.c_str());
        m_ret.SetMailSender(m_mail_sender.c_str());
        (m_pListener->*m_pfnSelector)(&m_ret);
        return;
    }
    if (pos.x + s2.width/2 > 0 && pos.x +s2.width/2 < s3.width
        && pos.y+s2.height/2 > 0 && pos.y+s2.height/2 < s3.height )             // if click sender.png
    {
        m_ret.SetInt0(kMailSender);
        m_ret.SetItemCount(this->getTag()-kMailLstItemTagBase);
        (m_pListener->*m_pfnSelector)(&m_ret);
        return;
    }
}
//******************************************************************************
// hide
//******************************************************************************
void MailListItem::hide()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    if (this->getChildByTag(0))
    {
        this->getChildByTag(0)->removeAllChildrenWithCleanup(true);
        removeAllChildrenWithCleanup(true);
    }
    m_bShowing = false;
}
//******************************************************************************
// mail-text
//******************************************************************************
void MailListItem::SetMailText(const char* ch0)
{
    m_mail_text = string(ch0);
}
void MailListItem::SetMailSender(const char* ch0)
{
    m_mail_sender = string(ch0);
}
//******************************************************************************
// retclass
//******************************************************************************
void MailListItemRet::SetInt0(int a)
{
    m_int0 = a;
}
int MailListItemRet::GetInt0()
{
    return m_int0;
}
void MailListItemRet::SetMailSender(const char* a)
{
    m_mailsender = a;
}
const char* MailListItemRet::GetMailSender()
{
    return m_mailsender;
}
void MailListItemRet::SetMailText(const char* a)
{
    m_mailtext = a;
}
const char* MailListItemRet::GetMailText()
{
    return m_mailtext;
}
void MailListItemRet::SetItemCount(int a)
{
    m_item_count = a;
}
int MailListItemRet::GetItemCount()
{
    return m_item_count;
}
void MailListItemRet::SetShowcardinfo(CShowCardInf* a)
{
    m_showcardinfo = a;
}
CShowCardInf* MailListItemRet::GetShowcarinfo()
{
    return m_showcardinfo;
}
//******************************************************************************
// nomaillayer
//******************************************************************************

NoMailLayer::NoMailLayer(CCObject *listner,SEL_CallFuncN selector)
{
    m_plistner   = listner;
    m_pselector  = selector;
    this->ShowNoMail();
}
void NoMailLayer::ShowNoMail()
{
    CCSprite* sp = CCSprite::spriteWithFile("fr_window.png");
    if(sp){
        sp->setScale(2);
        addChild(sp, 0, 0);
    }
    // button
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem* btnback = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(NoMailLayer::cbNoMailLayer));
    btnback->setAnchorPoint(CCPointZero);
    CCMenu* menu0 = CCMenu::menuWithItem(btnback);
    menu0->setAnchorPoint(CCPointZero);
    TextNode* itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"),
                                                   CCSizeMake(127, 30),
                                                   CCTextAlignmentCenter,
                                                   kBtnTitleHeight);
    itemLabel->setColor(ccc3(255, 255, 255));
    itemLabel->setShadowColor(ccBLACK);
    itemLabel->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                       spMenu1->getContentSize().height * 0.5 + 2));
    btnback->addChild(itemLabel);
    addChild(menu0);
    menu0->setPosition(CCPointMake(-80, -130));
    
    TextNode* infoLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("mail_nomail"), 30);
    infoLabel->setColor(ccc3(122, 37, 8));
    infoLabel->setPosition(CCPointMake(0, 40));
    addChild(infoLabel);

    
}
void NoMailLayer::cbNoMailLayer()
{
    (m_plistner->*m_pselector)(0);
}




















