//
//  MailListLayer.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 13-1-14.
//  Copyright (c) 2013Âπ?OneClick Co.,Ltd. All rights reserved.
//

#include "MailListLayer.h"
#include "MailBoxAgt.h"
#include "CScrollBar.h"
#include "MailListItem.h"
#include "CGameData.h"
#include "CommDlg.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "CShowCardInf.h"
#include "TitleBar.h"
#include "OcProxy.h"

using namespace CocosDenshion;

# define kMailLstItemTagBase 113     // refer to maillistitem.cpp
# define kBtnBack            49 
# define kTitleMail          50

//******************************************************************************
// Construct
//******************************************************************************
MailListLayer::MailListLayer(CCObject* target, SEL_CallFunc selector)
{
    m_pListener = target;
    m_pfnSelector = selector;    
    m_pScrollBar = NULL;
    m_maillayer  = NULL;
    m_commDlg = NULL;
    m_dlg_delete_all_mail = NULL;
    m_mail_reading_now = -1;
    m_clear_all = NULL;
    m_strMailTxt = "";    
}

//******************************************************************************
// onEnter && onExit
//******************************************************************************
void MailListLayer::onEnter()
{
    CCLayer::onEnter();
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    this->Refresh();
}
void MailListLayer::onExit()
{
    innerExit();
    setIsTouchEnabled(false);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);

    CCLayer::onExit();
}
//******************************************************************************
// add back menu
//******************************************************************************
void MailListLayer::AddBackMenu()
{    
    //back button
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    CCMenuItem* btnback = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(MailListLayer::cbMenuBack) );
    btnback->setPosition(CCPointMake(-529, 731));
    btnback->setAnchorPoint(CCPointZero);
    
    // 清空按钮
    m_clear_all = CCMenuItemImage::itemFromNormalSprite(GetClearAllSprite(), GetClearAllSprite(), this, menu_selector(MailListLayer::DeleteAllMail));
    m_clear_all -> setPosition(CCPointMake(530+640, 730));
    m_clear_all -> setAnchorPoint(CCPointZero);
    
    CCMenu* menu = CCMenu::menuWithItems( btnback, m_clear_all, NULL);
    this->getParent()-> addChild(menu, 30, kBtnBack);
    menu->setPosition(CCPointZero);

    // title
    TitleBar* spTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
    spTitle->setTitleString(CGameData::Inst()->getLanguageValue("mail_title"));
    spTitle->setPosition(CCPointMake(-210, 766));
    this->getParent()->addChild(spTitle, 100, kTitleMail);
    spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    m_clear_all->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(530, 730)), 0.8f));
    btnback->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
}
void MailListLayer::cbMenuBack()
{
    (m_pListener->*m_pfnSelector)();
}

CCSprite* MailListLayer::GetClearAllSprite()
{
    CCSprite* rt = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    char buf[20];
    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("mail_clear_all"));
    TextNode* label1 = TextNode::textWithString(buf, CCSizeMake(127, 30), CCTextAlignmentRight, kBtnTitleHeight-10);
    label1->setColor(ccWHITE);
    label1->setShadowColor(ccBLACK);
    label1->setPosition(CCPointMake(24, 38));
    rt->addChild(label1);
    return rt;
}


//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool MailListLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (!m_pScrollBar) {
        return false;
    }
    m_bTouchMvFlg = false;
    unschedule(schedule_selector(MailListLayer::inertiaMove));
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    last_moveTouch = touchLocation;
    m_ptStartGl = touchLocation;
    m_ptStartTouch = convertToNodeSpace( touchLocation );
    
    CCPoint ptOffset = ccpSub(CCPointMake(320, 666 + kMenuBtnHeight * 0.5), m_ptStartTouch);
    
    if((ptOffset.x > kMenuBtnWidth*0.5) || (ptOffset.x < -kMenuBtnWidth*0.5))
        return false;
    
    if((ptOffset.y < 0) || (ptOffset.y > m_iItemCnt * kMenuBtnHeight))
        return false;
    
    return true;
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void MailListLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if (!m_pScrollBar) {
        return;
    }
    
    m_bTouchMvFlg = true;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    m_lastYm = touchLocation.y - last_moveTouch.y;
    last_moveTouch = touchLocation;
    float fOffsetY = pos.y - m_ptStartTouch.y;
    
    CCPoint thisPt = this->getPosition();
    
    thisPt.y += fOffsetY;
    
    setThisPosition(thisPt);
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void MailListLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    if(m_bTouchMvFlg == true && ccpDistance(touchLocation, m_ptStartGl) > kListBoxMinMovY)
    {
        if(fabs(m_lastYm) >50)
            m_lastYm = m_lastYm/fabs(m_lastYm) *50;
        schedule(schedule_selector(MailListLayer::inertiaMove));
        return;
    }
    
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    if (pos.x>500)
    {
        // dianji kan xin
        
    }
    
    CCPoint ptOffset = ccpSub(CCPointMake(320, 666 + kMenuBtnHeight * 0.5), pos);
    
    if((ptOffset.x > kMenuBtnWidth*0.5) || (ptOffset.x < -kMenuBtnWidth*0.5))
        return;
    
    if((ptOffset.y < 0) || (ptOffset.y > m_iItemCnt * kMenuBtnHeight))
        return;
    
    int iBtnSel = ptOffset.y / kMenuBtnHeight;
    CCLOG("iBtnSel= %d", iBtnSel);
}

//******************************************************************************
// setThisPosition
//******************************************************************************
void MailListLayer::setThisPosition(const CCPoint& pt, bool bRedraw)
{
    CCPoint pos = pt;
    
    pos.y = MIN(pos.y, m_fUpLimitY);
    pos.y = MAX(pos.y, 0);
    
    this->setPosition(pos);
    
    if (m_pScrollBar) {
        float fPercent = pos.y / m_fUpLimitY;
        m_pScrollBar->setPercent(fPercent);
        
        m_pScrollBar->setPosition(CCPointMake(622, 438 - pos.y));
    }
    
    if((!bRedraw) && (abs(m_iPreThisPosY - (int)pos.y) < kMenuBtnHeight))
        return;
    
    m_iPreThisPosY = pt.y;

    for (int i = 0; i < m_iItemCnt; i++) {
        float fYpos = 666 - kMenuBtnHeight * i;
        
        CCNode* node = getChildByTag(kMailLstItemTagBase+i);
        
        if(node == NULL)
            continue;
        
        MailListItem* pItem = (MailListItem*)node;
        
        if((fYpos + pos.y < 0) || (fYpos + pos.y > 960)){
            pItem->hide();
        }
        else{
            CMailSenderInf* pSenderInf = MailBoxAgt::Inst()->getMailInfAtIdx(i);
            string strMailText = MailBoxAgt::Inst()->getMailTextAtIdx(i);
            pItem->show(pSenderInf, strMailText.c_str());

        }
    }
}

//******************************************************************************
// inertiaMove
//******************************************************************************
void MailListLayer::inertiaMove(float dt) {
    float f = 1;
    if (m_lastYm < 0) {
        f = -1;
    }
    
    m_lastYm -= dt*f*45;

    if (m_lastYm*f > .1f) {
        moveY(0.7*m_lastYm);
    } else {
        m_lastYm = 0;
        unschedule(schedule_selector(MailListLayer::inertiaMove));
    }
}

//******************************************************************************
// moveY
//******************************************************************************
void MailListLayer::moveY(float dt)
{
    CCPoint thisPt = this->getPosition();
    
    thisPt.y += dt;
    
    setThisPosition(thisPt);
}


//******************************************************************************
// callback from maillistitem.cpp
//******************************************************************************
void MailListLayer::cbMailBtn(CCObject* a)
{
    MailListItemRet* aa =(MailListItemRet*) a;
    switch (aa->GetInt0()) {
        case kMailRead:      // show the whole mail
            this->ShowMailText(a);
            break;
        case kMailSender:      // show the general
            if (MailBoxAgt::Inst()->getMailInfAtIdx(aa->GetItemCount())->senderInf.fid[0] != 0)
                this->ShowGeneral(a);
            break;
            
        default:
            break;
    }
}
//******************************************************************************
// show general
//******************************************************************************
void MailListLayer::ShowGeneral(CCObject* a)
{
    MailListItemRet* aa =(MailListItemRet*) a;
    CMailSenderInf* pSenderInf = MailBoxAgt::Inst()->getMailInfAtIdx(aa->GetItemCount());
    CShowCardInf * show_card_info = new CShowCardInf();
    show_card_info->autorelease();
    CUserCardInfo* user_card_info;
    user_card_info = CGameData::Inst()->getUsrCardInfByMailInf(pSenderInf);
    this->addChild(show_card_info);
    if (m_pScrollBar)
        show_card_info -> setPosition(CCPointMake(0, m_pScrollBar->getPosition().y - 438));
    else
        show_card_info -> setPosition(CCPointZero);
    show_card_info -> show(user_card_info, false);
}

//******************************************************************************
// show the mail
//******************************************************************************
void MailListLayer::ShowMailText(CCObject* a)
{
    innerExit();
    MailListItemRet* aa =(MailListItemRet*) a;
    m_mail_reading_now = aa->GetItemCount();
    MailBoxAgt::Inst()->setRead(m_mail_reading_now);
    bool reply = false;
    if (MailBoxAgt::Inst()->getMailInfAtIdx(aa->GetItemCount())->senderInf.fid[0] != 0)
        reply = true;
    if (m_maillayer == NULL)
    {
        m_maillayer = new MailLayer(this, callfuncO_selector(MailListLayer::cbMailLayer), reply);
        m_maillayer->ShowWithMail(aa->GetMailSender(), aa->GetMailText());
        m_maillayer->setPosition(CCPointMake(0, -this->getPosition().y));
        
        schedule(schedule_selector(MailListLayer::updateAddChild));
    }
}

//******************************************************************************
// updateAddChild
//******************************************************************************
void MailListLayer::updateAddChild(ccTime time)
{
    unschedule(schedule_selector(MailListLayer::updateAddChild));

    addChild(m_maillayer);
    m_maillayer->release();

}

//******************************************************************************
// callback no mail layer
//******************************************************************************
void MailListLayer::cbNoMail(int a)
{
    if (a == 0)
        this->cbMenuBack();
}

//******************************************************************************
// callback from maillayer
//******************************************************************************
void MailListLayer::cbMailLayer(CCObject* a)
{
    MailLayerRet* aa =(MailLayerRet* ) a;
    switch (aa->GetInt1()) {
        case kMailLayerDelete:
            MailDelete();
            break;
        case kMailLayerReply:
            MailReply();
            break;
        case kMailLayerOK:
            MailOK();
            break;
        default:
            break;
    }
}
//******************************************************************************
// do something after reading a mail
//******************************************************************************
void MailListLayer::MailDelete()
{
    m_iReqType =  en_DeleteMail;
    
    //点击删除按钮时 删除邮件详细内容的view
    m_maillayer->removeFromParentAndCleanup(true);
    m_maillayer = NULL;
    
    //调用GameData 删除 服务器端的邮件
    CMailSenderInf*   sendInfo  = MailBoxAgt::Inst() ->getMailInfAtIdx(m_mail_reading_now);
    CGameData::Inst() ->deleteMailRequest(sendInfo->senderInf.fid, sendInfo->senderInf.rcv_time);
    
    scheduleUpdate();
}
void MailListLayer::MailReply()
{
    char who_to_reply[100];
    snprintf(who_to_reply, 99, "%s",
             MailBoxAgt::Inst()->getMailInfAtIdx(m_mail_reading_now)->senderInf.name);
    OcProxy::Inst()->showMailViewController(who_to_reply);
    schedule(schedule_selector(MailListLayer::MailReplyUpdate));
    
    innerExit();
}
void MailListLayer::MailReplyUpdate(ccTime dt)
{
    if (OcProxy::Inst()->getIsHasSentMail())
    {
        unschedule(schedule_selector(MailListLayer::MailReplyUpdate));
        
        m_strMailTxt = OcProxy::Inst()->getMailText();
        if (!m_strMailTxt.empty())
        {
            if(CGameData::Inst()->sendMailRequest(MailBoxAgt::Inst()->getMailInfAtIdx(m_mail_reading_now)->senderInf.fid,
                                                  m_strMailTxt.c_str())){
                m_iReqType = enMailListLayerSendMail;
                scheduleUpdate();
            }
        }
        else
            Refresh();

        OcProxy::Inst()->deleteMailViewController();
    }
}
void MailListLayer::MailOK()
{
    if (m_maillayer) {
        m_maillayer -> removeFromParentAndCleanup(true);
        m_maillayer -> DeleteMail();
        m_mail_reading_now = -1;
        m_maillayer = NULL;
    }

    Refresh();
}
//******************************************************************************
// inner Exit
//******************************************************************************
void MailListLayer::innerExit()
{
    this->removeAllChildrenWithCleanup(true);
    this->getParent()->removeChildByTag(kBtnBack, true);
    this->getParent()->removeChildByTag(kTitleMail, true);
    
    m_pScrollBar = NULL;
    m_maillayer = NULL;
    m_commDlg = NULL;
    m_dlg_delete_all_mail = NULL;
}
void MailListLayer::Refresh()
{
    MainScene::Inst()->showTips();
    m_iItemCnt = MailBoxAgt::Inst()->getMailCnt();
    
    long lastTime = CGameData::Inst()->getUsrInfo()->last_mail_time;
    long localTime =  MailBoxAgt::Inst()->getLastRcvTime();
    
    if((lastTime > localTime)
       &&(CGameData::Inst()->rcvMailRequest())){    // after ten minutes, return true
        scheduleUpdate();
    }
    else{
        //??ΩΩMailListItem
        loadMail();
        m_mail_reading_now = -1;
        this->AddBackMenu();
        if (m_iItemCnt <= 0)  // 如果没有邮件 清空按钮应该是不显示的
        {
            m_clear_all->setIsVisible(false);
        }
        else
        {
            m_clear_all->setIsVisible(true);
        }
    }
}
//******************************************************************************
// update
//******************************************************************************
void MailListLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(MailListLayer::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    unscheduleUpdate();
    
    MainScene::Inst()->enableBottomPanel(true);
    
    if(comState == kReqStatOk)
    {
        if ( m_iReqType ==  en_DeleteMail)
        {
            //如果服务器端删除成功，则删除本地的邮件
            MailBoxAgt::Inst()->delMailAtIdx(m_mail_reading_now);
            m_mail_reading_now = -1;
            
            Refresh();
        }
        else if (m_iReqType == en_delete_mail_all)
        {
            // 删除全部的邮件
            MailBoxAgt::Inst()->DeleteAllMail();
            Refresh();
        }
        else if(m_iReqType == enMailListLayerSendMail){
            Refresh();
        }
        else
        {
            CGameData::Inst()->setRcvMailReqTime();
            MailBoxAgt::Inst()->updateMailBox();
            Refresh();
        }
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(MailListLayer::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}

//******************************************************************************
// cbCommDlg
//******************************************************************************
void MailListLayer::cbCommDlg(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }

    if(isOk)
    {
        if (m_iReqType == en_DeleteMail)
        {
            //调用GameData 删除 服务器端的邮件
            CMailSenderInf*   sendInfo  = MailBoxAgt::Inst() ->getMailInfAtIdx(m_mail_reading_now);
            CGameData::Inst() ->deleteMailRequest(sendInfo->senderInf.fid, sendInfo->senderInf.rcv_time);
            
            scheduleUpdate();
        }
        else if(m_iReqType == enMailListLayerSendMail){
            if (!m_strMailTxt.empty())
            {
                if(CGameData::Inst()->sendMailRequest(MailBoxAgt::Inst()->getMailInfAtIdx(m_mail_reading_now)->senderInf.fid,
                                                      m_strMailTxt.c_str())){
                    m_iReqType = enMailListLayerSendMail;
                    scheduleUpdate();
                    innerExit();
                }
            }
        }
        else
        {
            if(CGameData::Inst()->rcvMailRequest())
            {
                scheduleUpdate();
            }
            else
            {
                loadMail();
            }
        }
    }
    else
    {
        CGameData::Inst()->clearReqStat();
        MainScene::Inst()->enableBottomPanel(true);
        
        Refresh();
    }

}

//******************************************************************************
// loadMail
//******************************************************************************
void MailListLayer::loadMail()
{
    m_iItemCnt = MailBoxAgt::Inst()->getMailCnt();
    if (m_iItemCnt <= 0)
    {
        m_fUpLimitY  = 0;
        m_pScrollBar = NULL;
        NoMailLayer* no_mail = new NoMailLayer(this, callfuncN_selector(MailListLayer::cbNoMail));
        addChild(no_mail);
        no_mail->release();
        no_mail->setAnchorPoint(CCPointZero);
        no_mail->setPosition(CCPointMake(1000, 530));
        no_mail->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 530)), 0.8f));
        return;
    }
    
    
    if(m_iItemCnt > 5){
        m_pScrollBar = new CScrollBar();
        m_pScrollBar->autorelease();
        addChild(m_pScrollBar);
        m_pScrollBar->setPosition(CCPointMake(622, 438));
        
        float fBarSize = 1.0 / ((m_iItemCnt / 5) + 1);
        m_pScrollBar->setBarSize(fBarSize);
        m_pScrollBar->setPercent(0.0);
        
        m_fUpLimitY = (m_iItemCnt - 5) * kMenuBtnHeight;
    }
    else{
        m_fUpLimitY = 0;
        m_pScrollBar = NULL;
    }
    
    CCPoint pt(640+320, 666);
    for (int i = 0; i < m_iItemCnt; i++) {
        MailListItem* pItem = new MailListItem(this, callfuncO_selector(MailListLayer::cbMailBtn));
        pItem->autorelease();
        addChild(pItem, 0, kMailLstItemTagBase+i);
        
        
        pt.y = 666 - kMenuBtnHeight * i;
        
        pItem->setPosition(pt);
    }
    for (int i=0; i<m_iItemCnt; i++)
    {
         this->getChildByTag(kMailLstItemTagBase+i)->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 666 - kMenuBtnHeight*i)),0.8f));
    }
    
    setThisPosition(CCPointZero, true);
}

//******************************************************************************
// clear all the mails
//******************************************************************************
void MailListLayer::DeleteAllMail()
{
    innerExit();
    char alert_title[99];
    snprintf(alert_title, 99, "%s", CGameData::Inst()->getLanguageValue("mail_clear_all_title"));
    char alert_content[99];
    snprintf(alert_content, 99, "%s", CGameData::Inst()->getLanguageValue("mail_clear_all_content"));
    m_dlg_delete_all_mail = new Dialog2(this, callfuncO_selector(MailListLayer::cbDeleteAllMail), alert_title, alert_content, enDl2TwoBtn);
    addChild(m_dlg_delete_all_mail, 100);
    m_dlg_delete_all_mail->setPosition(CCPointMake(320, -this->getPosition().y+480));
    m_dlg_delete_all_mail->setAnchorPoint(CCPointZero);
    m_dlg_delete_all_mail->release();
}

void MailListLayer::cbDeleteAllMail(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if(pRet->bOk == true)
    {
        //点击清空邮件详细信息的确定按钮时 删除此view
        m_dlg_delete_all_mail -> removeFromParentAndCleanup(true);
        m_dlg_delete_all_mail = NULL;
        
        m_iReqType = en_delete_mail_all;
        // 向服务器请求
        CGameData::Inst()->deleteMailAllRequest();
        scheduleUpdate();
    }
    else
    {
        removeChild(m_dlg_delete_all_mail, true);
        m_dlg_delete_all_mail = NULL;
        Refresh();
    }
}





