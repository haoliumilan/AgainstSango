//
//  PublishWeiboLayer.cpp
//  AgainstWar
//
//  Created by 海桅 王 on 12-7-10.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "PublishWeiboLayer.h"
#include "CommonDef.h"
#include "MainScene.h"
#include "CGamedata.h"
#include "CommonDef.h"
#include "CommDlg.h"
#include "TextNode.h"
#include "CommonPublishView.h"
#include "OcProxy.h"

PublishWeiboLayer::PublishWeiboLayer(CCObject *listner,SEL_CallFuncO selector)
{
    isPublish = false;
    m_commDlg = NULL;
    m_pListener = listner;
    m_pfnSelector = selector;

    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
    sp->setPosition(ccp(320, 480));
    if(sp){
        sp->setScale(2.0);
        addChild(sp);
    }
    
    TextNode *lbText;
    lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("publishWeiboTip"), CCSizeMake(500, 40), CCTextAlignmentCenter, 55);
    lbText->setPosition(CCPointMake(320, 650));
    lbText->setColor(kTitleColor);
    addChild(lbText, 1);
    
    backItem = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    publishItem = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    sureItem = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    addChild(backItem,10);
    addChild(publishItem,10);
    addChild(sureItem,10);
    
    sureItem->setPosition(ccp(320, 300));
    backItem->setPosition(ccp(240, 285));
    publishItem->setPosition(ccp(440, 285));
    backItem->setTag(11);
    publishItem->setTag(12);
    sureItem->setTag(13);
    
    
    CCPoint point = backItem->getPosition();
    point.x-=30;
    point.y+=15;
    backItem->setPosition(point);
    
    point.x+=220;
    publishItem->setPosition(point);
    
    char buf[50];
    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("backTip"));
    TextNode* itemLabel = TextNode::textWithString(buf,
                                                   CCSizeMake(127, 30),
                                                   CCTextAlignmentCenter,
                                                   kBtnTitleHeight);
    itemLabel->setColor(ccc3(255, 255, 255));
    itemLabel->setShadowColor(ccBLACK);
    itemLabel->setPosition(CCPointMake(backItem->getContentSize().width * 0.5 - 2,
                                       backItem->getContentSize().height * 0.5 + 2));
    
    backItem->addChild(itemLabel);
        

    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("publishTip"));
    TextNode* itemLabel1 = TextNode::textWithString(buf,
                                                    CCSizeMake(127, 30),
                                                    CCTextAlignmentCenter,
                                                    kBtnTitleHeight);
    itemLabel1->setColor(ccc3(255, 255, 255));
    itemLabel1->setShadowColor(ccBLACK);
    itemLabel1->setPosition(CCPointMake(publishItem->getContentSize().width * 0.5 - 2,
                                        publishItem->getContentSize().height * 0.5 + 2));
    
    publishItem->addChild(itemLabel1);
    
    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("sureTip"));
    TextNode* itemLabel3 = TextNode::textWithString(buf,
                                                    CCSizeMake(127, 30),
                                                    CCTextAlignmentCenter,
                                                    kBtnTitleHeight);
    itemLabel3->setColor(ccc3(255, 255, 255));
    itemLabel3->setShadowColor(ccBLACK);
    itemLabel3->setPosition(CCPointMake(sureItem->getContentSize().width * 0.5 - 2,
                                        sureItem->getContentSize().height * 0.5 + 2));

    sureItem->addChild(itemLabel3);
    
    tipLabel = TextNode::textWithString("", CCSizeMake(400, 150), CCTextAlignmentCenter, 30);
    addChild(tipLabel,10);
    tipLabel->setColor(kInfoColor);
    tipLabel->setPosition(ccp(320, 480));
    tipLabel->setIsVisible(false);
    m_iReqType = 0;
    
    sureItem->setIsVisible(false);

    
}

void PublishWeiboLayer::showPublishViewWithType(enWeiboType type)
{
    isPublish = false;
    if(m_commDlg!=NULL)
    {
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    m_weiboType = type;
    tipLabel->setIsVisible(false);
    sureItem->setIsVisible(false);
    publishItem->setIsVisible(true);
    backItem->setIsVisible(true);


    setIsTouchEnabled(true);
    MainScene::Inst()->enableBottomPanel(false);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, kCCMenuTouchPriority-1, true);

    OcProxy::Inst()->showPublishView(type);

     
}

void PublishWeiboLayer::showWeiboBackTips()
{
    CGameData::Inst()->clearReqStat();
    sureItem->setIsVisible(true);
    publishItem->setIsVisible(false);
    backItem->setIsVisible(false);
    tipLabel->setIsVisible(true);
  //  tipBack->setIsVisible(true);
    tipLabel->setString(CGameData::Inst()->getWeiboBackStr());
}
//******************************************************************************
// update
//******************************************************************************
void PublishWeiboLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(PublishWeiboLayer::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
           
        }
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
       // MainScene::Inst()->enableBottomPanel(true);
    }
    
    unscheduleUpdate();
    
    if(comState == kReqStatOk){
        if(m_iReqType == 1)
        {
            showWeiboBackTips();
            m_iReqType = 0;
        }
    }else {
        
            if(m_commDlg == NULL){
                MainScene::Inst()->enableBottomPanel(false);
                m_commDlg = new CommDlg(this, callfuncO_selector(PublishWeiboLayer::cbCommDlg), enCommDlgTp_retry);
                addChild(m_commDlg, 1000);
                m_commDlg->setPosition(CCPointMake(320, 480));
                m_commDlg->release();
                m_commDlg->setScale(0.0);
                m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            }
        

    }

}


bool PublishWeiboLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCNode* node1 = getChildByTag(11);
    CCNode* node2 = getChildByTag(12);
    if(sureItem->getIsVisible() == false&&isPublish == false)
    {
        if(node1 == NULL && node2 == NULL)
            return true;
        
        CCPoint pt = node1->getPosition();
        CCSize sz = node1->getContentSize();
        
        CCPoint pt2 = node2->getPosition();
        CCSize sz2 = node2->getContentSize();
        
        bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                           pt.y - sz.height * 0.5, 
                                                           sz.width, 
                                                           sz.height), 
                                                pos);
        bool bRet2 = CCRect::CCRectContainsPoint(CCRectMake(pt2.x - sz2.width * 0.5, 
                                                           pt2.y - sz2.height * 0.5, 
                                                           sz2.width, 
                                                           sz2.height), 
                                                pos);
        
        
        if(bRet){
            m_ret.iSel = node1->getTag();
            (m_pListener->*m_pfnSelector)(&m_ret);
        }
        
        if(bRet2)//发布微博
        {
            m_ret.iSel = node2->getTag();
            (m_pListener->*m_pfnSelector)(&m_ret);
            OcProxy::Inst()->setPublishViewHidden(true);
         //   [publishView setHidden:YES];
            //OcProxy::Inst()->set
            CGameData::Inst()->reqWeiboBackRequest();
            OcProxy::Inst()->sendWeibo(OcProxy::Inst()->getWeiboContent(), true);

            scheduleUpdate();
            m_iReqType = 1;
            isPublish = true;
            // else
        }
    }else {
        CCNode *node3 = getChildByTag(13);
        if(node3 == NULL)
            return true;
        
        CCPoint pt = node3->getPosition();
        CCSize sz = node3->getContentSize();
        
        bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                           pt.y - sz.height * 0.5, 
                                                           sz.width, 
                                                           sz.height), 
                                                pos);
        if(bRet)//返回
        {
            MainScene::Inst()->enableBottomPanel(true);
            m_ret.iSel = node1->getTag();
            (m_pListener->*m_pfnSelector)(&m_ret);
        }
    }
    return true;
}

void PublishWeiboLayer::removePublishView()
{
    setIsTouchEnabled(false);
    OcProxy::Inst()->removePublishView();
    MainScene::Inst()->enableBottomPanel(true);
}

void PublishWeiboLayer::cbCommDlg(CCObject *sender)
{
    CommDlgRet* pRet = (CommDlgRet*)sender;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(isOk == true){
        OcProxy::Inst()->setPublishViewHidden(true);
        CGameData::Inst()->reqWeiboBackRequest();
     //   CGameData::Inst()->sendWeiBoWithText(OcProxy::Inst()->getWeiboContent(),true);
        OcProxy::Inst()->sendWeibo(OcProxy::Inst()->getWeiboContent(), true);
        scheduleUpdate();
        
        m_iReqType = 1;

    }else {
        CGameData::Inst()->clearReqStat();
//        [publishView setHidden:NO];
        OcProxy::Inst()->setPublishViewHidden(false);
        OcProxy::Inst()->setPublishViewHidden(false);
        MainScene::Inst()->enableBottomPanel(false);
        isPublish = false;
    }
}

void PublishWeiboLayer::menuCallback(CCObject *sender)
{
    CCMenuItem *item = (CCMenuItem *)sender;
    m_ret.iSel = item->getTag();
    (m_pListener->*m_pfnSelector)(&m_ret);
}