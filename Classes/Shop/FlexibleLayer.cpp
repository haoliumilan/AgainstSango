//
//  FlexibleLayer.cpp
//  AgainstWar
//
//  Created by user on 13-6-4.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "FlexibleLayer.h"
#include "SimpleAudioEngine.h"
#include "CommDlg.h"
#include "OcProxy.h"

using namespace CocosDenshion;

FlexibleLayer::FlexibleLayer()
{
    m_FlexType = CGameData::Inst()->getCommonInfo()->charge_type;
    bgBar = NULL;
    m_moveBar = NULL;
    m_commDlg = NULL;
    itemLabel3 = NULL;
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -200, true);
    
    if (m_FlexType == 1)
    {
        showAllChildren();
    }
    else
    {
        showAllChildren2();
    }
}

FlexibleLayer::~FlexibleLayer()
{

}

void FlexibleLayer::huodongRefresh(CCObject* pSender)
{
    if (CGameData::Inst()->reqChargeAward())
    {
        m_iReqType = enFlexReqType_Refresh;
        scheduleUpdate();
    }
}

void FlexibleLayer::checkHuodong(CCObject* pSender)
{
    OcProxy::Inst()->linkToSafriWithUrlStr(CGameData::Inst()->getCommonInfo()->charge_url);
}

void FlexibleLayer::showAllChildren2()
{
    CCSprite* bgCurSort = CCSprite::spriteWithFile("cur_purchase_pass.png");
    bgCurSort->setPosition(ccp(308,660));
    addChild(bgCurSort);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    TextNode *itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("shopTip24"), kBtnTitleHeight);
    itemLabel->setPosition(ccp(77, 42));
    itemLabel->setShadowColor(ccBLACK);
    spMenu1->addChild(itemLabel);
    spMenu1->setPosition(ccp(130,638));
    addChild(spMenu1,0,1306051);
    
    CCSprite* menu2 = CCSprite::spriteWithFile("huodongbtn.png");
    
    TextNode *itemLabel2 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("shopTip25"), 31);
    itemLabel2->setPosition(ccp(103, 38));
    itemLabel2->setShadowColor(ccBLACK);
    menu2->addChild(itemLabel2);
    addChild(menu2,0,1306052);
    menu2->setPosition(ccp(458,640));
    
    ChargeAward* pChargeAward = CGameData::Inst()->getChargeAward();
    
    char cBuff[100];
    sprintf(cBuff, CGameData::Inst()->getLanguageValue("shopTip28"),pChargeAward->charge_sum);
    itemLabel3 = TextNode::textWithString(cBuff, 32);
    itemLabel3->setAnchorPoint(ccp(0.0f, 0.5f));
    itemLabel3->setPosition(ccp(38, 106));
    itemLabel3->setShadowColor(ccBLACK);
    bgCurSort->addChild(itemLabel3);
}

void FlexibleLayer::showAllChildren()
{
    CCSprite* bgCurSort = CCSprite::spriteWithFile("cur_purchase_pass.png");
    bgCurSort->setPosition(ccp(308,660));
    addChild(bgCurSort);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    TextNode *itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("shopTip24"), kBtnTitleHeight);
    itemLabel->setPosition(ccp(77, 42));
    itemLabel->setShadowColor(ccBLACK);
    spMenu1->addChild(itemLabel);
    spMenu1->setPosition(ccp(130,638));
    addChild(spMenu1,0,1306051);
    
    CCSprite* menu2 = CCSprite::spriteWithFile("huodongbtn.png");
    
    TextNode *itemLabel2 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("shopTip25"), 31);
    itemLabel2->setPosition(ccp(103, 38));
    itemLabel2->setShadowColor(ccBLACK);
    menu2->addChild(itemLabel2);
    addChild(menu2,0,1306052);
    menu2->setPosition(ccp(458,640));
    
    ChargeAward* pChargeAward = CGameData::Inst()->getChargeAward();
    
    char cBuff[100];
    sprintf(cBuff, CGameData::Inst()->getLanguageValue("shopTip26"),pChargeAward->m_iOidTotalNum);
    itemLabel3 = TextNode::textWithString(cBuff, 32);
    itemLabel3->setAnchorPoint(ccp(0.0f, 0.5f));
    itemLabel3->setPosition(ccp(38, 106));
    itemLabel3->setShadowColor(ccBLACK);
    bgCurSort->addChild(itemLabel3);
    
    CCSprite* bgOrderRecord = CCSprite::spriteWithFile("cur_purchase_sort.png");
    bgOrderRecord->setPosition(ccp(308, 544));
    addChild(bgOrderRecord);
    
    TextNode *itemLabel4 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("shopTip27"), 33);
    itemLabel4->setAnchorPoint(ccp(0.0f, 0.5f));
    itemLabel4->setPosition(ccp(38, 78));
    itemLabel4->setColor(ccc3(85, 53, 15));
    bgOrderRecord->addChild(itemLabel4);
    
    m_MoveInfoLayer = CCLayer::node();
    addChild(m_MoveInfoLayer);
    
    drawOrderSort();
    
    showScrollBar();
}

void FlexibleLayer::showScrollBar()
{
    if (m_fTotalStrWidth <= 500)
    {
        return;
    }
    
    bgBar = CCSprite::spriteWithFile("cur_purchase_scorll.png");
    bgBar->setAnchorPoint(ccp(0.0f, 0.5f));
    bgBar->setPosition(ccp(50.0f, 508.0f));
    addChild(bgBar);
    
    m_moveBar = CCSprite::spriteWithFile("cur_purchase_scorllbar.png");
    m_moveBar->setAnchorPoint(ccp(0.0f, 0.5f));
    m_moveBar->setPosition(ccp(53.0f, 508.0f));
    addChild(m_moveBar);
    m_moveBar->setScaleX(((bgBar->getContentSize().width-2)/m_moveBar->getContentSize().width)
                         *(500/m_fTotalStrWidth)
                         );
}


void FlexibleLayer::drawOrderSort()
{
    m_MoveInfoLayer->removeAllChildrenWithCleanup(true);
    
    ChargeAward* pChargeAward = CGameData::Inst()->getChargeAward();
    
    CCPoint pt = CCPoint(50, 532);
    
    CCPoint stPt = CCPoint(ccp(pt.x,pt.y));
    
    float deltaX = 10.0f;
    
    
    CCPoint pos(0,0);
    float fFactor = 1.0;
    CCSize winsz = CCDirector::sharedDirector()->getWinSizeInPixels();
    
    if(winsz.width > 1000)
    {
        fFactor = 4.0;
        pos = CCPointMake(128, 64);
    }
    else if(winsz.width == 768)
    {
        fFactor = 2.0;
        pos = CCPointMake(64, 32);
    }
    else if(winsz.width == 640)
    {
        fFactor = 2.0;
    }
    
    
    for (int i=0; i<pChargeAward->m_iAwadrNum; i++)
    {
        char buf[50];
        if (i == pChargeAward->m_iOidTotalNum-1)
        {
            snprintf(buf, 49,"%d",pChargeAward->m_iAwadrIds[i]);
        }
        else
        {
            snprintf(buf, 49,"%d,",pChargeAward->m_iAwadrIds[i]);
        }
        TextNode *itemLabel = TextNode::textWithString(buf, 25);
        itemLabel->setAnchorPoint(ccp(0.0f, 0.5f));
        itemLabel->setPosition(pt);
        itemLabel->setColor(ccc3(247, 23, 0));
        m_MoveInfoLayer->addChild(itemLabel);
        itemLabel->setClipRect(1, 15*fFactor+pos.x, 0, 278*fFactor, 3000);
        pt.x += itemLabel->getContentSize().width + deltaX;
    }
    
    for (int i=0; i<pChargeAward->m_iOidNum; i++)
    {
        char buf[50];
        if (i == pChargeAward->m_iOidNum -1)
        {
            snprintf(buf, 49,"%d",pChargeAward->m_iOidIds[i]);
        }
        else
        {
            snprintf(buf, 49,"%d,",pChargeAward->m_iOidIds[i]);
        }
        TextNode *itemLabel = TextNode::textWithString(buf, 23);
        itemLabel->setAnchorPoint(ccp(0.0f, 0.5f));
        itemLabel->setPosition(pt);
        itemLabel->setColor(ccc3(85, 53, 15));
        m_MoveInfoLayer->addChild(itemLabel);
        
        itemLabel->setClipRect(1, 15*fFactor+pos.x, 0, 278*fFactor, 3000);
        
        pt.x += itemLabel->getContentSize().width + deltaX;
    }
    
    m_fTotalStrWidth = pt.x - stPt.x;
}

void FlexibleLayer::cbCommDlg(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(isOk == true){
        switch (m_iReqType)
        {
            case enFlexReqType_Refresh:
                if (CGameData::Inst()->reqChargeAward())
                {
                    scheduleUpdate();
                }
                break;
                
            default:
                break;
        }
    }
    else
    {
        CGameData::Inst()->clearReqStat();
        
        MainScene::Inst()->showBottomPanel(true);
        MainScene::Inst()->showUsrInfPanel(true);
    }
}

void FlexibleLayer::reFreshSumCharge()
{
    ChargeAward* pChargeAward = CGameData::Inst()->getChargeAward();
    char cBuff[100];
    sprintf(cBuff, CGameData::Inst()->getLanguageValue("shopTip28"),pChargeAward->charge_sum);
    itemLabel3->setString(cBuff);
}

void FlexibleLayer::reFreshOrder()
{
    m_MoveInfoLayer->setPosition(CCPoint(0,0));
    drawOrderSort();
    if (m_fTotalStrWidth>500 && m_moveBar)
    {
        m_moveBar->setPosition(CCPoint(53.0f, 508.0f));
    }
}

void FlexibleLayer::update(ccTime dt)
{
    
    int comState = CGameData::Inst()->getReqStat();
    
    if(comState == kReqStatRequesting)
    {
        if(m_commDlg == NULL)
        {
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(FlexibleLayer::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
        return;
    }
    
    if(m_commDlg != NULL)
    {
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
        MainScene::Inst()->enableBottomPanel(true);
    }
    
    unscheduleUpdate();
    
    if(comState == kReqStatOk)
    {
        MainScene::Inst()->showTips();
        switch (m_iReqType)
        {
            case enFlexReqType_Refresh:
            {
                if (m_FlexType == 1)
                {
                   reFreshOrder();
                }
                else if (m_FlexType == 2)
                {
                    reFreshSumCharge();
                }
            }
                break;
                
            default:
                break;
        }
    }
    else
    {
        if(m_commDlg == NULL)
        {
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(FlexibleLayer::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            m_commDlg->setNoCancel();
        }
    }
    
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool FlexibleLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    if(sz.width == 320)
        touchLocation = ccpMult(touchLocation, 2);
    if(sz.width < 1000)
        touchLocation = ccpMult(touchLocation, CC_CONTENT_SCALE_FACTOR());
    
    //ipad, iphone5
    touchLocation.x -= MainScene::Inst()->getPosition().x;
    touchLocation.y -= MainScene::Inst()->getPosition().y;
    
    if(m_FlexType == 1
       &&((touchLocation.y > 720 || touchLocation.y < 480))
       )
        return false;
    else if (m_FlexType == 2
             &&((touchLocation.y > 720 || touchLocation.y < 590))
             )
        return false;
    
    else
        return true;
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void FlexibleLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if (m_fTotalStrWidth <= 500)
    {
        return;
    }
    
    if (m_FlexType == 2)
        return;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint prevLoaction = touch->previousLocationInView(touch->view());
    prevLoaction = CCDirector::sharedDirector()->convertToGL( prevLoaction );

    m_MoveInfoLayer ->setPosition(ccp(m_MoveInfoLayer->getPosition().x + (touchLocation.x-prevLoaction.x),0));
    
    if (m_MoveInfoLayer->getPosition().x > 0.0f)
    {
        m_MoveInfoLayer->setPosition(ccp(0.0f,0.0f));
    }
    
    if (m_MoveInfoLayer->getPosition().x < -(m_fTotalStrWidth - 500.0f))
    {
        m_MoveInfoLayer->setPosition(ccp(-(m_fTotalStrWidth - 500.0f),0.0f));
    }
    
    float fTotalL = 28 + m_fTotalStrWidth;
    
    float x = m_MoveInfoLayer->getPosition().x * ((bgBar->getContentSize().width-2)/fTotalL );
    
    m_moveBar->setPosition(ccp(54+(-x),508.0f));
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void FlexibleLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    CCPoint pos = convertToNodeSpace( touchLocation );

    CCSprite* tSp = (CCSprite*)getChildByTag(1306051);
    
    if (tSp && CCRect::CCRectContainsPoint(CCRectMake(tSp->getPosition().x - tSp->getContentSize().width * 0.5,
                                                      tSp->getPosition().y - tSp->getContentSize().height * 0.5,
                                                      tSp->getContentSize().width,
                                                      tSp->getContentSize().height),
                                           pos))
    {
        huodongRefresh(NULL);
    }
    
    
    tSp = (CCSprite*)getChildByTag(1306052);
    
    if (tSp && CCRect::CCRectContainsPoint(CCRectMake(tSp->getPosition().x - tSp->getContentSize().width * 0.5,
                                                      tSp->getPosition().y - tSp->getContentSize().height * 0.5,
                                                      tSp->getContentSize().width,
                                                      tSp->getContentSize().height),
                                           pos))
    {
        checkHuodong(NULL);
    }

    
    
    
}


