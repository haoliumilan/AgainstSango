//
//  CardShop.cpp
//  AgainstWar
//
//  Created by user on 13-12-21.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "CardShop.h"
#include "TextNode.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "Dialog2.h"

#include "CShowCardInf.h"

static float gfMoveDownLimitY = 192.0;

CardShop::CardShop(CCObject* listener, SEL_CallFunc selector)
{
    m_pListener = listener;
    m_pfSelector = selector;
    
    
    m_pScrollBar = NULL;
   
    m_iPreThisPosY = 0;
    isLongPress = false;
    
    m_pUserCardInfo = NULL;
    m_dlgAlert = NULL;
    m_cfmLayer = NULL;
    m_iSelNo = 0;
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
        
    pShopCardsArr = CGameData::Inst()->getShopCards();
    
    showCardItem();
    
    //新增
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 20);
    m_showCard->release();
    m_showCard->setIsVisible(false);
    
    setPosition(ccp(-1000, 0));
    runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero), 0.8f));
}

CardShop::~CardShop()
{
    if (m_pUserCardInfo)
    {
        m_pUserCardInfo->release();
        m_pUserCardInfo = NULL;
    }
}

void CardShop::cbCfm(CCObject* pSender)
{
    CardConfirmRet* ret = (CardConfirmRet*)pSender;
    switch (ret->iBtnSel)
    {
        case 0:
        {
            CShopCardInfo* pShopCardInfo = CGameData::Inst()->getShopCards()->getObjectAtIndex(m_iSelNo);
            CCardBaseInfo* pBaseCardInfo = CGameData::Inst()->getCardBaseInfByCid(pShopCardInfo->sCid);
            
            char buf[100];
            snprintf(buf, 99, CGameData::Inst()->getLanguageValue("card_shop_tips_1"),
                     pShopCardInfo->iPrice, pBaseCardInfo->star, pBaseCardInfo->name);
            Dialog2* dlg2 = new Dialog2(this, callfuncO_selector(CardShop::Dlg2Callback),
                                        CGameData::Inst()->getLanguageValue("card_shop_tips_2"),
                                        buf, enDl2TwoBtn);
            addChild(dlg2, 15,888);
            dlg2->release();
            dlg2->setPosition(ccp(320, 480-getPosition().y));
        }
            break;
            
        case 1:
            waitShowInfo();

            break;
            
        case 2:
            removeChild(m_cfmLayer, true);
            m_cfmLayer = NULL;
            break;
            
        default:
            break;
    }
}

void CardShop::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(CardShop::cbCommDlg),
                                    enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480 - getPosition().y));
            m_commDlg->release();
        }
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
        MainScene::Inst()->enableBottomPanel(true);
    }
    
    unscheduleUpdate();
    
    if (comState == kReqStatOk)
    {
        switch (m_ReqType)
        {
            case enReqSCBuy:
                if (CGameData::Inst()->allCardRequest())
                {
                    m_ReqType = enReqSCGetAllCard;
                    scheduleUpdate();
                }
                break;
                
            case enReqSCGetAllCard:
                showSuccessAlert();
                break;
                
            default:
                break;
        }
    }
    else
    {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(CardShop::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480 - getPosition().y));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}

void CardShop::cbCommDlg(CCObject* pSender)
{
    CommDlgRet* pRet = (CommDlgRet*)pSender;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }

    if (pRet->bOk)
    {
        switch (m_ReqType)
        {
            case enReqSCBuy:
                buyCard();
                break;
                
            case enReqSCGetAllCard:
                if (CGameData::Inst()->allCardRequest())
                    scheduleUpdate();
                break;
                
            default:
                break;
        }
    }
    else
    {
        CGameData::Inst()->clearReqStat();
        MainScene::Inst()->enableBottomPanel(true);
    }
}

void CardShop::showSuccessAlert()
{
    if (m_dlgAlert == NULL)
    {
        m_dlgAlert = new Dialog1(this,
                                 callfuncO_selector(CardShop::cbAlertDlg),
                                 CGameData::Inst()->getLanguageValue("card_shop_tips_3"),
                                 false);
        addChild(m_dlgAlert, 100);
        m_dlgAlert->release();
    }
    
    m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
    m_dlgAlert->setScale(0.0);
    m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void CardShop::cbAlertDlg(CCObject* pSender)
{
    if (m_dlgAlert)
    {
        m_dlgAlert->removeFromParentAndCleanup(true);
        m_dlgAlert = NULL;
    }
    
    if (m_cfmLayer)
    {
        removeChild(m_cfmLayer, true);
        m_cfmLayer = NULL;
    }
}

void CardShop::buyCard()
{
    short sCardId = CGameData::Inst()->getShopCards()->getObjectAtIndex(m_iSelNo)->sCid;
    if (CGameData::Inst()->reqBuyCard(sCardId))
    {
        m_ReqType = enReqSCBuy;
        scheduleUpdate();
    }
}

void CardShop::Dlg2Callback(CCObject* pSender)
{
    Dialog2Ret* ret = (Dialog2Ret*) pSender;
    
    if (ret->bOk)
    {
        buyCard();
        removeChildByTag(888, true);
    }
    else
    {
        removeChildByTag(888, true);
    }
}

void CardShop::showCardItem()
{
    CCSprite* sp = CGameData::Inst()->getHeadSprite(1);
    m_cardSize = sp->getContentSize();
    
    m_iColumn = 640 / m_cardSize.width;
    m_iGap = (640 - m_iColumn * m_cardSize.width) / (m_iColumn + 1);
    
    m_ptStart.x = m_iGap;
    m_ptStart.y = 720;
    
    CCPoint pt;
    m_iRmBtn = 0;
    
    int npIndex = 0;
    for (int i=0; i<pShopCardsArr->count(); i++)
    {
        CShopCardInfo* pShopCard = pShopCardsArr->getObjectAtIndex(i);
        pt.x = m_ptStart.x + ((npIndex + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width);
        pt.y = m_ptStart.y - ((npIndex + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height);
        
        sp = CGameData::Inst()->getHeadSprite(pShopCard->sCid);
        addChild(sp);
        sp->setAnchorPoint(ccp(0,1));
        sp->setPosition(pt);
        sp->setTag(CARD_TAG + i);
        
        
        
        CCSprite* spState = NULL;
        switch (pShopCard->status)
        {
            case 1:
                spState = CCSprite::spriteWithFile("card_shop_hot.png");
                break;
                
            case 2:
                spState = CCSprite::spriteWithFile("card_shop_new.png");
                break;
                
            case 3:
            default:
                spState = CCSprite::spriteWithFile("card_shop_sale.png");
                break;
        }
        
        sp->addChild(spState);
        spState->setPosition(ccp(82, 82));
        
        CCSprite* spPriceBG = CCSprite::spriteWithFile("card_shop_coin.png");
        sp->addChild(spPriceBG);
        spPriceBG->setPosition(ccp(56, 6));
        
        char buf[50];
        snprintf(buf, 49, "%d",pShopCard->iPrice);
        TextNode* textPrice = TextNode::textWithString(buf, 16);
        spPriceBG->addChild(textPrice);
        textPrice->setAnchorPoint(ccp(0.5, 0));
        textPrice->setPosition(ccp(68, 10));
        textPrice->setColor(ccc3(250, 237, 66));
        textPrice->setShadowColor(ccBLACK);
        
        npIndex++;
    }
    m_shopCardNum = npIndex;
    
    m_fUpLimitY = gfMoveDownLimitY - (pt.y - m_cardSize.height);
    if (m_fUpLimitY > 0) {
        m_pScrollBar = new CScrollBar();
        addChild(m_pScrollBar);
        m_pScrollBar->setTag(1);
        m_pScrollBar->release();
        float dBarSize = 1.0/((m_fUpLimitY / (m_ptStart.y - gfMoveDownLimitY))+2);
        m_pScrollBar->setBarSize(dBarSize);
        m_pScrollBar->setPercent(0.0);
        m_pScrollBar->setPosition(CCPointMake(630, 435));
        m_pScrollBar->setScaleY(1);
      
    }
}


void CardShop::removem_cfmLayer()
{
    removeChild(m_cfmLayer, true);
    m_cfmLayer = NULL;

}

bool CardShop::isExistForcfmLayer()
{
    return (m_cfmLayer && m_cfmLayer->getIsVisible());
}


//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool CardShop::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    m_bTouchMoveFlg = false;
    unschedule(schedule_selector(CardShop::inertiaMove));
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    beganPoint = touchLocation;
    m_ptStartTouch = convertToNodeSpace( touchLocation );
    last_moveTouch = touchLocation;
    m_lastXm = 0;
    
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    
    schedule(schedule_selector(CardShop::waitShowInfo), 0.3);
    
    if(sz.width == 320)
        touchLocation = ccpMult(touchLocation, 2);
    if(sz.width < 1000)
        touchLocation = ccpMult(touchLocation, CC_CONTENT_SCALE_FACTOR());
    
    //ipad, iphone5
    int offsetY = MainScene::Inst()->getPosition().y;
    
    if((touchLocation.y > (m_ptStart.y + offsetY)) || (touchLocation.y < (160 + offsetY)))
    {
        unschedule(schedule_selector(CardShop::waitShowInfo));
        return false;
    }
    
    else
        return true;

    
}



//******************************************************************************
// ccTouchMoved
//******************************************************************************
void CardShop::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    
    unschedule(schedule_selector(CardShop::waitShowInfo));
    if(isLongPress == true)
    {
        //isLongPress = false;
        return;
    }
    m_bTouchMoveFlg = true;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    m_lastXm = touchLocation.x - last_moveTouch.x;
    m_lastYm = touchLocation.y - last_moveTouch.y;
    last_moveTouch = touchLocation;
    
    float fOffsetY = pos.y - m_ptStartTouch.y;
    
    CCPoint thisPt = this->getPosition();
    
    thisPt.y += fOffsetY;
    
    if(thisPt.y > m_fUpLimitY)
        thisPt.y = m_fUpLimitY;
    
    if(thisPt.y < 0.0)
        thisPt.y = 0.0;
    
    setThisPosition(thisPt);
    
    if(m_fUpLimitY > 0){
        float fPercent = thisPt.y / m_fUpLimitY;
        
        m_pScrollBar->setPercent(fPercent);
        
      m_pScrollBar->setPosition(CCPointMake(630, 452 - thisPt.y));
        
    }


}




//******************************************************************************
// ccTouchEnded
//******************************************************************************
void CardShop::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    unschedule(schedule_selector(CardShop::waitShowInfo));
    if(isLongPress == true)
    {
        isLongPress = false;
        return;
    }
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    
    if(m_bTouchMoveFlg == true && ccpDistance(touchLocation, beganPoint) > kListBoxMinMovY)
    {
    if (kListBoxMinMovY <= fabs(m_lastYm) && fabs(m_lastXm)<=fabs(m_lastYm))
    {
        if(fabs(m_lastYm) >100)
            m_lastYm = m_lastYm/fabs(m_lastYm) *100;
        schedule(schedule_selector(CardShop::inertiaMove));
        return;
        
    }
    if (14.0f>fabsf(m_lastXm))return;
    }
    
    
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    if(sz.width == 320)
        touchLocation = ccpMult(touchLocation, 2);
    if(sz.width < 1000)
        touchLocation = ccpMult(touchLocation, CC_CONTENT_SCALE_FACTOR());
    
    //ipad, iphone5
    touchLocation.x -= MainScene::Inst()->getPosition().x;
    touchLocation.y -= MainScene::Inst()->getPosition().y;
    
    if (CCRect::CCRectContainsPoint(CCRectMake(0, 148, 640, 84), touchLocation))
        return;
    
    int y = (m_ptStart.y - pos.y) / (m_iGap + m_cardSize.height);
    int x = pos.x / (m_iGap + m_cardSize.width);
    
    if((m_ptStart.y - pos.y) > (y * (m_iGap + m_cardSize.height) + m_cardSize.height))
        return;
    
    if((pos.x - m_ptStart.x) < (x * (m_iGap + m_cardSize.width) + m_iGap))
        return;
    
    int iSel = y * m_iColumn + x;
    
    if (iSel>=0 && iSel<=CGameData::Inst()->getShopCards()->count())
        showChooseCard(iSel);


}


void CardShop::showChooseCard(int idx)
{
    m_iSelNo = idx;
    CShopCardInfo* pShopCard = pShopCardsArr->getObjectAtIndex(idx);
    
    if (m_cfmLayer == NULL)
    {
        m_cfmLayer = new CardConfirm(this, callfuncO_selector(CardShop::cbCfm), pShopCard);
        addChild(m_cfmLayer);
        m_cfmLayer->setPosition(CCPointMake(0, -getPosition().y));
        m_cfmLayer->release();
    }
    
    
}

void CardShop::setThisPosition(const CCPoint& pos, bool bRedraw)
{
    
    CCPoint pt = pos;
    if (pt.y > m_fUpLimitY) {
        pt.y = m_fUpLimitY;
    }
    
    if (pt.y < 0) {
        pt.y = 0;
    }
    
    this->setPosition(pt);
    
    //  m_dlgPoint = CCPoint(320,480-pt.y);
    
    //不强制要求重画，而且移动范围小于100
    if((!bRedraw) && (abs(m_iPreThisPosY - (int)pt.y) < 100))
        return;
    //改变m_pScrollBar位置
    //    CCLOG("m_pScrollBarY=%f",m_pScrollBarY);
    if (m_pScrollBar) {
        
        m_pScrollBar->setPosition(CCPointMake(630, 452 - pt.y));
        
        m_pScrollBar->setPercent(pt.y/m_fUpLimitY);
        
    }
    
    m_iPreThisPosY = pt.y;
    
}

CUserCardInfo* CardShop::getNewUserCardFromShopCard(CShopCardInfo* pShopCardInfo)
{
    CCardBaseInfo * pBaseCardInfo = CGameData::Inst()->getCardBaseInfByCid(pShopCardInfo->sCid);
    if (m_pUserCardInfo)
    {
        m_pUserCardInfo->release();
        m_pUserCardInfo = NULL;
    }
    
    m_pUserCardInfo = new CUserCardInfo();
    
    snprintf(m_pUserCardInfo->name, 99, "%s", pBaseCardInfo->name);
    m_pUserCardInfo->cid = pBaseCardInfo->cid;
    m_pUserCardInfo->category = pBaseCardInfo->category;
    m_pUserCardInfo->sub_category = pBaseCardInfo->sub_category;
    m_pUserCardInfo->ugp_target = pBaseCardInfo->ugp_target;
    m_pUserCardInfo->ugp_max_star = pBaseCardInfo->ugp_max_star;
    m_pUserCardInfo->exp_type = pBaseCardInfo->exp_type;
    m_pUserCardInfo->lv = pShopCardInfo->iStartLv;
    m_pUserCardInfo->exp = CGameData::Inst()->getExpByType(m_pUserCardInfo->exp_type, m_pUserCardInfo->lv);
    m_pUserCardInfo->base_exp = pBaseCardInfo->base_exp;
    m_pUserCardInfo->sk_lv = 1;
    m_pUserCardInfo->upd_time = 0;
    m_pUserCardInfo->upg_gold = pBaseCardInfo->upg_gold;
    m_pUserCardInfo->star = pBaseCardInfo->star;
    m_pUserCardInfo->hp = pBaseCardInfo->hp + pBaseCardInfo->hp_growth * (m_pUserCardInfo->lv - 1);
    m_pUserCardInfo->skid = pBaseCardInfo->skid;
    m_pUserCardInfo->ctype = pBaseCardInfo->ctype;
    m_pUserCardInfo->stype = pBaseCardInfo->stype;
    m_pUserCardInfo->attack = pBaseCardInfo->attack + pBaseCardInfo->attack_growth * (m_pUserCardInfo->lv - 1);
    m_pUserCardInfo->cost = pBaseCardInfo->cost;
    m_pUserCardInfo->leader_skid = pBaseCardInfo->leader_skid;
    m_pUserCardInfo->sell_gold = pBaseCardInfo->sell_gold;
    m_pUserCardInfo->recover = pBaseCardInfo->recover + pBaseCardInfo->recover_growth * (m_pUserCardInfo->lv - 1);
    m_pUserCardInfo->max_lv = pBaseCardInfo->max_lv;
    m_pUserCardInfo->upg_flg = 0;
    m_pUserCardInfo->bChecked = false;
    m_pUserCardInfo->pl_hp = 0;
    m_pUserCardInfo->pl_attack = 0;
    m_pUserCardInfo->pl_recover = 0;
    m_pUserCardInfo->plus = 0;
    for (int i = 0; i < MaxAwakeSkillCnt; i++) {
        m_pUserCardInfo->AwakeSkillArr[i] = pBaseCardInfo->AwakeSkillArr[i];
    }
    m_pUserCardInfo->i_AwakeNum = 0;
    m_pUserCardInfo->find_me = pBaseCardInfo->find_me;

    
    return m_pUserCardInfo;
}

void CardShop::waitShowInfo()
{
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    CCPoint touchLocation = beganPoint;
    CCPoint pos = convertToNodeSpace( touchLocation );
    if(sz.width < 1000)
        touchLocation = ccpMult(touchLocation, CC_CONTENT_SCALE_FACTOR());
    
    //ipad, iphone5
    touchLocation.x -= MainScene::Inst()->getPosition().x;
    touchLocation.y -= MainScene::Inst()->getPosition().y;
    
    int y = (m_ptStart.y - pos.y) / (m_iGap + m_cardSize.height);
    int x = (pos.x - m_ptStart.x) / (m_iGap + m_cardSize.width);
    
    if((m_ptStart.y - pos.y) > (y * (m_iGap + m_cardSize.height) + m_cardSize.height))
        return;
    
    if((pos.x - m_ptStart.x) < (x * (m_iGap + m_cardSize.width) + m_iGap))
        return;
    
    int iSel = y * m_iColumn + x;
    
    isLongPress = true;
    if (iSel < pShopCardsArr->count()) {
        if(m_showCard->getIsVisible() == false){
            CShopCardInfo * cshopcardinfo = (CShopCardInfo*)pShopCardsArr->getObjectAtIndex(iSel);
            getNewUserCardFromShopCard(cshopcardinfo);
            
            m_showCard->setPosition(CCPointMake(0, -this->getPosition().y));
            m_showCard->show(m_pUserCardInfo);
        }
        
        
    }
    
    
    
    unschedule(schedule_selector(CardShop::waitShowInfo));
    
}



void CardShop::inertiaMove(float dt){
    float f = 1;
    if (m_lastYm < 0) {
        f = -1;
    }
    
    m_lastYm -= dt*f*45;
    //printf("m_lastYm %f",m_lastYm);
    // m_lastYm -= dt*
    if (m_lastYm*f > .1f) {
        moveY(0.7*m_lastYm);
    } else {
        m_lastYm = 0;
        
        unschedule(schedule_selector(CardShop::inertiaMove));
        // m_scrollBar->setIsVisible(false);
    }
    
}


void CardShop::moveY(float dt)
{
    CCPoint thisPt = this->getPosition();
    
    thisPt.y += dt;
    
    if(thisPt.y > m_fUpLimitY)
        thisPt.y = m_fUpLimitY;
    
    if(thisPt.y < 0.0)
        thisPt.y = 0.0;
    
    setThisPosition(thisPt);
    
    if(m_fUpLimitY > 0){
        float fPercent = thisPt.y / m_fUpLimitY;
        
        m_pScrollBar->setPercent(fPercent);
        m_pScrollBar->setPosition(CCPointMake(630, 452 - thisPt.y));
        
        
    }
}








