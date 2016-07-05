//
//  ItemCardLayer.cpp
//  AgainstWar
//
//  Created by user on 13-4-12.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "ItemCardLayer.h"
#include "UseResultDialog.h"
#include "CommDlg.h"
#include "Dialog1.h"
#include "SimpleAudioEngine.h"

#include "Arrow.h"

#define ITEMCARDDELTA   99
#define kUseItemCard    999
#define KUseBatchCard   1000

#define kNewbie         9898

USING_NS_CC;
using namespace CocosDenshion;

ItemCardLayer::ItemCardLayer(CCObject* target, SEL_CallFuncO selector)
{
    m_rtyTime = 0;
    m_menu = NULL;
    m_spTitle = NULL;
    m_cntryDialog = NULL;
    pGachaLayer = NULL;
    m_isGachaPlus = false;
    m_pListener = target;
    m_pfnSelector = selector;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    m_dlgResult = NULL;
    m_pScrollBar = NULL;
    m_spMaxCard = NULL;
    m_lbMaxCard = NULL;
    m_commDlg = NULL;
    m_dlgAlert = NULL;
    m_curReqType = enReqType_Null;
    fDownLimitY = 192.0;
    m_iPreThisPosY = 0;
    
    
    
    
    if(CGameData::Inst()->getUsrInfo()->guild_item == 0 )//随身军袋
    {
        //引导
        
        if(CGameData::Inst()->newGiveWuJiangRequest("4"))
        {
            
            MainScene::Inst()->new_FirstItem = true;
            
//            MainScene::Inst()->showTouchTip();
            MainScene::Inst()->enableBottomPanel(false);
            MainScene::Inst()->showBottomPanel(false);
            
            m_curReqType = enReqType_GiveWuJiang_Item;
            scheduleUpdate();
        }
    }
    else
    {
        m_preStatus = CGameData::Inst()->getCntryInfo()->status;
        showItemCard();
    }
    
    
    m_showCard = NULL;
    m_RareGacha10Show = NULL;
}


ItemCardLayer::~ItemCardLayer()
{
    
}

void ItemCardLayer::RareGacha10ShowCb()
{
    
}

void ItemCardLayer::setTouchEnable()
{
    if (!getIsTouchEnabled()) {
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    }
}

void ItemCardLayer::showItemCard()
{
    removeAllChildrenWithCleanup(true);
    
    fDownLimitY = 192.0;
    m_iPreThisPosY = 0;
    
    CCSprite* tempSp = CCSprite::spriteWithFile("itemcard1.png");
    m_cardSize = tempSp->getContentSize();
    m_iColumn = 640 / m_cardSize.width;
    m_iGap = (640 - m_iColumn * m_cardSize.width) / (m_iColumn + 1);
    
    m_ptStart.x = m_iGap;
    m_ptStart.y = 720;
    
    CCPoint pt = ccp(0,0);
    
    CCMutableArray<ItemCardInf* > * itemCardInf =  CGameData::Inst()->getItemCardPkInf();
    
    if(MainScene::Inst()->new_FirstItem && !MainScene::Inst()->new_FirstItem_Second)
    {
        for (int i = 0 ; i < itemCardInf->count(); i ++)
        {
            
            
            if(itemCardInf->getObjectAtIndex(i)->strItemId.compare("601_item") == 0)
            {
                
                Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(ItemCardLayer::newbie),
                                                    CGameData::Inst()->getLanguageValue("item_new20"),false);
                addChild(newbieEndDlg, 500, kNewbie);
                newbieEndDlg->release();
                newbieEndDlg->setPosition(CCPointMake(320, 300));
                newbieEndDlg->setScale(0.0);
                newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                
                
                
                Arrow* pArrow = new Arrow(30, 255);
                
                pArrow->setPosition(CCPointMake(231, 245));
                
                
                
                pArrow->setRotation(180);
                //    pArrow->setScale(0.6f);
                
                MainScene::Inst()->addChild(pArrow, 50000, kNewbie + 99999);
                
                
                CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                                   CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                                   NULL);
                pArrow->runAction(CCRepeatForever::actionWithAction(action));
                
                
                
                if(i == 0)
                    break;
                
                itemCardInf->insertObjectAtIndex(itemCardInf->getObjectAtIndex(i), 0);
                
                itemCardInf->removeObjectAtIndex(i + 1);
                
                break;
            }
            
            
        }
    }
    
    
    
    
    for (int i = 0 ; i < itemCardInf->count(); i ++) //i < itemCardInf->count()
    {
        ItemCard* tempItem = ItemCard::spriteWithFile(itemCardInf->getObjectAtIndex(i)->strResName.c_str(),itemCardInf->getObjectAtIndex(i)->strItemId, itemCardInf->getObjectAtIndex(i)->lCnt);
        addChild(tempItem,1,ITEMCARDDELTA+i);
        
        
        
        pt.x = m_ptStart.x + ((i ) % m_iColumn) * (m_iGap + m_cardSize.width);
        pt.y = m_ptStart.y - ((i ) / m_iColumn) * (m_iGap + m_cardSize.height);
        tempItem->setAnchorPoint(ccp(0,1));
        tempItem->setPosition(pt);
    }
    
    m_fUpLimitY = fDownLimitY - (pt.y - m_cardSize.height);
    
    if (itemCardInf->count() <= 0)
    {
        m_fUpLimitY = -100.0f;
        
        CCSprite* m_background = CCSprite::spriteWithFile("fr_window.png");
        m_background->setScale(2.0);
        addChild(m_background);
        m_background->setPosition(ccp(320, 520));
        
        TextNode* tText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_noitemcard"), 30);
        tText->setColor(ccc3(122, 37, 8)); 
        addChild(tText);
        tText->setPosition(ccp(320, 570));
        
        CCSprite* button = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        addChild(button,1,6789);
        button->setPosition(ccp(320, 432));
        
        TextNode* tText2 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"), kBtnTitleHeight);
        button->addChild(tText2);
        tText2->setPosition(ccp(button->getContentSize().width/2-1, button->getContentSize().height/2+2));
        tText2->setColor(ccWHITE);
        tText2->setShadowColor(ccBLACK);
    }
    
    if(m_fUpLimitY > 0)
    {
        m_pScrollBar = new CScrollBar();
        addChild(m_pScrollBar);
        m_pScrollBar->release();
        
        float fBarSize = 1.0 / ((m_fUpLimitY / (720 - 192)) + 2);
        m_pScrollBar->setBarSize(fBarSize);
        m_pScrollBar->setPercent(0.0);
        
        m_pScrollBar->setPosition(CCPointMake(630, 435));
        
        
        if(MainScene::Inst()->new_FirstItem)
            m_pScrollBar->setIsVisible(false);
    }
    
    
    m_lastYm = 0;
}



//******************************************************************************
// newbie
//******************************************************************************
void ItemCardLayer::newbie(CCObject* pObj)
{
    CCNode* newbieEndDlg = getChildByTag(kNewbie);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);
    
    
    Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(kNewbie + 99999);
    if(temp)
        temp->removeFromParentAndCleanup(true);
    
    
    

    Arrow* pArrow = new Arrow(30, 255);
    pArrow->setPosition(CCPointMake( 74, 720 - 100));
    
    
    pArrow->setRotation(90);
    addChild(pArrow, 9, 9898);
    pArrow->release();
    
    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                       NULL);
    pArrow->runAction(CCRepeatForever::actionWithAction(action));

    
    

    
}

//******************************************************************************
// newbie2
//******************************************************************************
void ItemCardLayer::newbie2(CCObject* pObj)
{
    CCNode* newbieEndDlg = getChildByTag(kNewbie);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);
    
    
    Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(kNewbie + 99999);
    if(temp)
        temp->removeFromParentAndCleanup(true);
    
    
    
    

    
    MainScene::Inst()->new_FirstItem = false;
    MainScene::Inst()->new_FirstItem_Second = false;
    
    MainScene::Inst()->removeTouchTip();
    MainScene::Inst()->enableBottomPanel(true);
    
    MainScene::Inst()->showBottomPanel(true);
    
    
    
}



void ItemCardLayer::BtnCallBack(CCObject* pSender)
{
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if (m_RareGacha10Show != NULL)
    {
        m_RareGacha10Show->removeAllChildrenWithCleanup(true);
        removeChild(m_RareGacha10Show, true);
        m_RareGacha10Show = NULL;
    }
    
    if (m_showCard != NULL)
    {
        m_showCard->removeAllChildrenWithCleanup(true);
        removeChild(m_showCard, true);
        m_showCard = NULL;
    }
    
    MainScene::Inst()->showBottomPanel(true);
    
    if (m_spTitle!=NULL)
    {
        m_spTitle->setPosition(ccp(-3000,-3000));
        removeChild(m_spTitle, true);
        m_spTitle = NULL;
    }
    
    if (m_menu!=NULL)
    {
        removeChild(m_menu, true);
        m_menu = NULL;
    }
    
    
    m_ret.iActType = enMoveTitelHere;
    (m_pListener->*m_pfnSelector)(&m_ret);
    
    showUseResultDlg();
}



void ItemCardLayer::inertiaMove(float dt)
{
    float f = 1;
    if (m_lastYm < 0) {
        f = -1;
    }
    
    m_lastYm -= dt*f*45;
    if (m_lastYm*f > .1f)
    {
        moveY(0.7*m_lastYm);
    }
    else
    {
        m_lastYm = 0;
        
        unschedule(schedule_selector(ItemCardLayer::inertiaMove));
    }
}

void ItemCardLayer::moveY(float dt)
{
    CCPoint thisPt = this->getPosition();
    
    thisPt.y += dt;
    
    if(thisPt.y > m_fUpLimitY)
        thisPt.y = m_fUpLimitY;
    
    if(thisPt.y < 0.0)
        thisPt.y = 0.0;
    
    setThisPosition(thisPt);
    
    if(m_fUpLimitY > 0)
    {
        float fPercent = thisPt.y / m_fUpLimitY;
        
        m_pScrollBar->setPercent(fPercent);
        
        m_pScrollBar->setPosition(CCPointMake(630, 435 - thisPt.y));
        
      //  redrawCapacity();
    }
}


void ItemCardLayer::setThisPosition(const CCPoint& pt, bool bRedraw)
{
    this->setPosition(pt);
    
    //不强制要求重画，而且移动范围小于100
    if((!bRedraw) && (abs(m_iPreThisPosY - (int)pt.y) < 100))
        return;
    
    m_iPreThisPosY = pt.y;
    
    //改变m_pScrollBar位置
    //    CCLOG("m_pScrollBarY=%f",m_pScrollBarY);
    if (m_pScrollBar) {
        m_pScrollBar->setPosition(CCPointMake(630, 435 - pt.y));
     //   redrawCapacity();
        m_pScrollBar->setPercent(pt.y/m_fUpLimitY);
      //  redrawCapacity();
    }
    
    CCNode* node;
    for(int i = 0; i < kMaxCardNum; i++){
        node = getChildByTag(ITEMCARDDELTA + i);
        
        if(node)
        {
            ItemCard* card = (ItemCard*) node;
            CCPoint ptCard = card->getPosition();
            
            if((ptCard.y + pt.y < 960) && (ptCard.y + pt.y > 0))
            {
            //    card->show();
            }
            else{
            //    card->hide();
            }
        }
    }
    
    //CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}


//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool ItemCardLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    unschedule(schedule_selector(ItemCardLayer::inertiaMove));
    m_lastYm = 0;
    m_bTouchMvFlg = false;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    m_ptStartNode = convertToNodeSpace( touchLocation );
    last_moveTouch = touchLocation;
    m_ptStartGl = touchLocation;
    
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    if(sz.width == 320)
        touchLocation = ccpMult(touchLocation, 2);
    if(sz.width < 1000)
        touchLocation = ccpMult(touchLocation, CC_CONTENT_SCALE_FACTOR());
    
    //ipad, iphone5
    touchLocation.x -= MainScene::Inst()->getPosition().x;
    touchLocation.y -= MainScene::Inst()->getPosition().y;
    
    if((touchLocation.y > 720) || (touchLocation.y < 160))
        return false;
    else
        return true;
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void ItemCardLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    m_bTouchMvFlg = true;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    m_lastYm = touchLocation.y - last_moveTouch.y;
    last_moveTouch = touchLocation;
    
    float fOffsetY = pos.y - m_ptStartNode.y;
    
    CCPoint thisPt = this->getPosition();
    
    thisPt.y += fOffsetY;
    
    if(thisPt.y > m_fUpLimitY)
        thisPt.y = m_fUpLimitY;
    
    if(thisPt.y < 0.0)
        thisPt.y = 0.0;
    
    
    if(!MainScene::Inst()->new_FirstItem)
    setThisPosition(thisPt);
    
    if(m_fUpLimitY > 0)
    {
        float fPercent = thisPt.y / m_fUpLimitY;
        
        m_pScrollBar->setPercent(fPercent);
        
        m_pScrollBar->setPosition(CCPointMake(630, 435 - thisPt.y));
    }
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void ItemCardLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    if(m_bTouchMvFlg == true && ccpDistance(touchLocation, m_ptStartGl) > kListBoxMinMovY)
    {
        m_lastYm *= 1.5;
        if(fabs(m_lastYm) >100)
            m_lastYm = m_lastYm/fabs(m_lastYm) *100;
        
        if(!MainScene::Inst()->new_FirstItem)
        schedule(schedule_selector(ItemCardLayer::inertiaMove));
        return;
    }
    
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    
    if (CGameData::Inst()->getItemCardPkInf()->count() <= 0)
    {
        CCSprite* tSp = (CCSprite*)getChildByTag(6789);
        
        if (tSp && CCRect::CCRectContainsPoint(CCRectMake(tSp->getPosition().x - tSp->getContentSize().width * 0.5,
                                                   tSp->getPosition().y - tSp->getContentSize().height * 0.5,
                                                   tSp->getContentSize().width,
                                                   tSp->getContentSize().height),
                                        pos))
            {
                m_ret.iActType = enItmCardLyQuit;
                (m_pListener->*m_pfnSelector)(&m_ret);
            }

        
        
        return;
    }
    
    
    
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    if(sz.width == 320)
        touchLocation = ccpMult(touchLocation, 2);
    if(sz.width < 1000)
        touchLocation = ccpMult(touchLocation, CC_CONTENT_SCALE_FACTOR());
    
    int y = (m_ptStart.y - pos.y) / (m_iGap + m_cardSize.height);
    int x = pos.x / (m_iGap + m_cardSize.width);
    
    
    
    if((m_ptStart.y - pos.y) > (y * (m_iGap + m_cardSize.height) + m_cardSize.height))
        return;
    
    if((pos.x - m_ptStart.x) < (x * (m_iGap + m_cardSize.width) + m_iGap))
        return;
    
    int iSel = y * m_iColumn + x;
    if (iSel >= CGameData::Inst()->getItemCardPkInf()->count())
    {
        return;
    }
    
    m_ret.iBtnSel = iSel;
    
    
    
    if(MainScene::Inst()->new_FirstItem && iSel != 0)
        return;
    
    
    if(MainScene::Inst()->new_FirstItem)
    {
        CCMutableArray<ItemCardInf* > * itemCardInf =  CGameData::Inst()->getItemCardPkInf();
        
        
        
        for (int i = 0 ; i < itemCardInf->count(); i ++)
        {
            
            
            if(itemCardInf->getObjectAtIndex(i)->strItemId.compare("601_item") == 0)
            {
                iSel = i;
                
                m_ret.iBtnSel = iSel;
                
                
                
                removeChildByTag(9898, true);
                
                
                Arrow* pArrow = new Arrow(30, 255);
                
                pArrow->setPosition(CCPointMake(231, 425 ));
                
                
                
                pArrow->setRotation(180);
                
                MainScene::Inst()->addChild(pArrow, 50000, 93999);
                
                
                CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                                   CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                                   NULL);
                pArrow->runAction(CCRepeatForever::actionWithAction(action));
                
                
                break;
            }
            
            
        }
    }
    
    
    CCMutableArray<ItemCardInf* > * itemCardInfArr = CGameData::Inst()->getItemCardPkInf();
    ItemCardInf* itemCardInf = itemCardInfArr->getObjectAtIndex(m_ret.iBtnSel);
    m_useCardType = itemCardInf->strType;
    m_useCardId = itemCardInf->strItemId;
    m_useCardName = itemCardInf->strItemName;
    
    
    
    
    

    UseItemCard* pCfm = new UseItemCard(this, callfuncO_selector(ItemCardLayer::cbUseItemCard), CGameData::Inst()->getItemCardPkInf()->getObjectAtIndex(iSel),m_useCardType,cardCanBatch(m_useCardType));
    addChild(pCfm, 10, kUseItemCard);
    pCfm->setPosition(ccp(getPosition().x, -getPosition().y));
    setIsTouchEnabled(false);
}

int ItemCardLayer::cardCanBatch(string cardTypeStr)
{
    if (cardTypeStr == IMCARDSTRTYPE_RENAME
        ||cardTypeStr == IMCARDSTRTYPE_CD
        ||cardTypeStr == IMCARDSTRTYPE_GACHA
        ||cardTypeStr == IMCARDSTRTYPE_GACHA_MULTI
        ||cardTypeStr ==IMCARDSTRTYPE_COST_UP
        
        )
    {
        return 0;
    }
    else if(cardTypeStr ==IMCARDSTRTYPE_COMPOUND)
    {
        return 2;
    }
    else
    {
        return 1;
    }
}

void ItemCardLayer::Dlg2Callback(CCObject* pSender)
{
    Dialog2Ret* ret = (Dialog2Ret*) pSender;
    
    if (ret->bOk)
    {
        
        if(MainScene::Inst()->new_FirstItem)
        MainScene::Inst()->removeChildByTag(93998, true);
        
        useItemCard();
        removeChildByTag(888, true);
    }
    else
    {
        if(!MainScene::Inst()->new_FirstItem)
        removeChildByTag(888, true);
    }
}

void ItemCardLayer::GoToYuanbao(CCObject* pSender)
{
    Dialog2Ret* ret = (Dialog2Ret*) pSender;
    
    if (ret->bOk)
    {
        MainScene::Inst()->GoToYuanbaoPage();
    }
    else
    {
        removeChildByTag(888, true);
    }
}

//******************************************************************************
// cbUsebatchItemCard
//******************************************************************************
void ItemCardLayer::cbUsebatchItemCard(CCObject* pSender)
{
    removeChildByTag(kUseItemCard, true);
    removeChildByTag(KUseBatchCard, true);
    setTouchEnable();
    
    BatchUseCardRet* ret = (BatchUseCardRet*)pSender;
    m_useCardCount = ret->iBtnSel;
    
    if (m_useCardCount == 0)
        return;
    
    CCMutableArray<ItemCardInf* > *itemCardArr = CGameData::Inst()->getItemCardTotalInf();
    int itemCardCount = 0;
    for (int i = 0; i < itemCardArr->count(); i++) {
        ItemCardInf* itemCardInfo = itemCardArr->getObjectAtIndex(i);
        if (itemCardInfo->strItemId == m_useCardId)
        {
            itemCardCount = itemCardInfo->lCnt;
            break;
        }
    }
    
    if (m_useCardCount > itemCardCount && m_useCardType!="stone")
        showAlertNotEnoughDlg();
    else if (m_useCardType == "card"
             && m_useCardCount > 10)
        showAlertMoreThan10Dlg();
    else{
        if (m_useCardType==IMCARDSTRTYPE_STONE) {
            if((m_useCardCount>CGameData::Inst()->getUsrInfo()->can_contribute_coin+itemCardCount)
               ||(m_useCardCount>CGameData::Inst()->getUsrInfo()->coin+itemCardCount)){
                Dialog2* dlg2 = new Dialog2(this, callfuncO_selector(ItemCardLayer::GoToYuanbao), CGameData::Inst()->getLanguageValue("cntry_querenuse"), CGameData::Inst()->getLanguageValue("cntry_stone_yuanbaobuzu"),enDl2TwoBtn);
                addChild(dlg2,15,888);
                dlg2->release();
                dlg2->setPosition(ccp(320,500 - getPosition().y));;
            }else{
                useItemCard();
            }
        }else{
            showSureUseCardDlg();
        }
    }
}

//******************************************************************************
//showAlertNotEnoughDlg
//******************************************************************************
void ItemCardLayer::showAlertNotEnoughDlg()
{
    if (m_dlgAlert == NULL)
    {
        m_dlgAlert = new Dialog1(this,
                                 callfuncO_selector(ItemCardLayer::cbAlertDlg),
                                 CGameData::Inst()->getLanguageValue("cntry_item_use_notEnough"),
                                 false);
        addChild(m_dlgAlert, 100);
        m_dlgAlert->release();
    }
    
    m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
    m_dlgAlert->setScale(0.0);
    m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

//******************************************************************************
// showAlertMoreThan10Dlg
//******************************************************************************
void ItemCardLayer::showAlertMoreThan10Dlg()
{
    if (m_dlgAlert == NULL)
    {
        m_dlgAlert = new Dialog1(this,
                                 callfuncO_selector(ItemCardLayer::cbAlertDlg),
                                 CGameData::Inst()->getLanguageValue("cntry_item_use_moreThan10"),
                                 false);
        addChild(m_dlgAlert, 100);
        m_dlgAlert->release();
    }
    
    m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
    m_dlgAlert->setScale(0.0);
    m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

//******************************************************************************
// cbUseItemCard
//******************************************************************************
void ItemCardLayer::cbAlertDlg()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if (m_dlgAlert)
    {
        m_dlgAlert->removeFromParentAndCleanup(true);
        m_dlgAlert = NULL;
    }
}

//******************************************************************************
// showSureUseCardDlg
//******************************************************************************
void ItemCardLayer::showSureUseCardDlg()
{
    if(m_useCardType == "card"
       && CGameData::Inst()->getUsrInfo()->max_card_num <= CGameData::Inst()->getUserCards()->count())
    {
        MainScene::Inst()->showFullPackDlg(true);
        return;
    }
    
    char buf[100];
    Dialog2* dlg2 = NULL;
    
    if(m_useCardType == "stone")
    {
        snprintf(buf, 99,
                 CGameData::Inst()->getLanguageValue("cntry_askjuanxianmore"),
                 m_useCardCount, m_useCardName.c_str());
        dlg2 = new Dialog2(this, callfuncO_selector(ItemCardLayer::Dlg2Callback), CGameData::Inst()->getLanguageValue("cntry_querenjuanxian"), buf,enDl2TwoBtn);
    }
    else if ("cost_up"==m_useCardType)
    {
        if (CUserInfo *pUserInfo = CGameData::Inst()->getUsrInfo())
        {
            time_t curTime = time(NULL);
            if (curTime<pUserInfo->hours_endTime && 0<pUserInfo->hours_append_cost)
            {
                dlg2 = new Dialog2(this, callfuncO_selector(ItemCardLayer::Dlg2Callback), CGameData::Inst()->getLanguageValue("cntry_querenuse"),
                                   CGameData::Inst()->getLanguageValue("cntry_askuseappendcsot"),enDl2TwoBtn);
            }
            else
            {
                snprintf(buf, 99,
                         CGameData::Inst()->getLanguageValue("cntry_askusemore"),
                         m_useCardCount, m_useCardName.c_str());
                dlg2 = new Dialog2(this, callfuncO_selector(ItemCardLayer::Dlg2Callback), CGameData::Inst()->getLanguageValue("cntry_querenuse"), buf,enDl2TwoBtn);
            }
        }
    }
    else
    {
        snprintf(buf, 99,
                 CGameData::Inst()->getLanguageValue("cntry_askusemore"),
                 m_useCardCount, m_useCardName.c_str());
        dlg2 = new Dialog2(this, callfuncO_selector(ItemCardLayer::Dlg2Callback), CGameData::Inst()->getLanguageValue("cntry_querenuse"), buf,enDl2TwoBtn);
    }
    addChild(dlg2,15,888);
    dlg2->release();
    dlg2->setPosition(ccp(320,500 - getPosition().y));
}

//******************************************************************************
// cbUseItemCard
//******************************************************************************
void ItemCardLayer::cbUseItemCard(CCObject* pSender)
{
    UseItemCardRet* ret = (UseItemCardRet*)pSender;
    
    if(MainScene::Inst()->new_FirstItem && ret->iBtnSel != 0)
        return;
    
    if(MainScene::Inst()->new_FirstItem)
    {
        MainScene::Inst()->removeChildByTag(93999, true);
        
        
        Arrow* pArrow = new Arrow(30, 255);
        pArrow->setPosition(CCPointMake( 160, 300));
        
        
        pArrow->setRotation(90);
        MainScene::Inst()->addChild(pArrow, 50000, 93998);
        pArrow->release();
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));
        
        

    }
    
    switch (ret->iBtnSel)
    {
        case 0:
        {
            CCMutableArray<ItemCardInf* > * itemCardInfArr = CGameData::Inst()->getItemCardPkInf();
            ItemCardInf* itemCardInf = itemCardInfArr->getObjectAtIndex(m_ret.iBtnSel);
            
            if (itemCardInf->lNeedLv > CGameData::Inst()->getUsrInfo()->lv)
            {
                if (m_dlgAlert == NULL)
                {
                    m_dlgAlert = new Dialog1(this,
                                             callfuncO_selector(ItemCardLayer::cbAlertDlg),
                                             CGameData::Inst()->getLanguageValue("cntry_item_use_notlvneed"),
                                             false);
                    addChild(m_dlgAlert, 100);
                    m_dlgAlert->release();
                }
                
                m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
                m_dlgAlert->setScale(0.0);
                m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                return;
            }
            else if (itemCardInf->lNeedCbu > CGameData::Inst()->getCntryInfo()->user_contribution)
            {
                if (m_dlgAlert == NULL)
                {
                    m_dlgAlert = new Dialog1(this,
                                             callfuncO_selector(ItemCardLayer::cbAlertDlg),
                                             CGameData::Inst()->getLanguageValue("cntry_item_use_notcbuneed"),
                                             false);
                    addChild(m_dlgAlert, 100);
                    m_dlgAlert->release();
                }
                
                m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
                m_dlgAlert->setScale(0.0);
                m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                return;
            }
            
            m_useCardCount = 1;
            showSureUseCardDlg();
//            removeChildByTag(kUseItemCard, true);
        }
            break;
            
        case 1:
        {
            CCMutableArray<ItemCardInf* > * itemCardInfArr = CGameData::Inst()->getItemCardPkInf();
            ItemCardInf* itemCardInf = itemCardInfArr->getObjectAtIndex(m_ret.iBtnSel);
            
            
//            if (itemCardInf->strType =="compound")
//                return;
            
            if (itemCardInf->lNeedLv > CGameData::Inst()->getUsrInfo()->lv)
            {
                if (m_dlgAlert == NULL)
                {
                    m_dlgAlert = new Dialog1(this,
                                             callfuncO_selector(ItemCardLayer::cbAlertDlg),
                                             CGameData::Inst()->getLanguageValue("cntry_item_use_notlvneed"),
                                             false);
                    addChild(m_dlgAlert, 100);
                    m_dlgAlert->release();
                }
                
                m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
                m_dlgAlert->setScale(0.0);
                m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                return;
            }
            else if (itemCardInf->lNeedCbu > CGameData::Inst()->getCntryInfo()->user_contribution)
            {
                if (m_dlgAlert == NULL)
                {
                    m_dlgAlert = new Dialog1(this,
                                             callfuncO_selector(ItemCardLayer::cbAlertDlg),
                                             CGameData::Inst()->getLanguageValue("cntry_item_use_notcbuneed"),
                                             false);
                    addChild(m_dlgAlert, 100);
                    m_dlgAlert->release();
                }
                
                m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
                m_dlgAlert->setScale(0.0);
                m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
                return;
            }
            
            BatchUseCard* m_batchUseLayer = new BatchUseCard(this, callfuncO_selector(ItemCardLayer::cbUsebatchItemCard),ret->lCnt,m_useCardType);
            m_batchUseLayer->setPosition(CCPointMake(0, 565 - getPosition().y));
            
            addChild(m_batchUseLayer,11,KUseBatchCard);
            m_batchUseLayer->release();
            
        }
            break;
            
        case 2:
        {
            setTouchEnable();
            removeChildByTag(kUseItemCard, true);
        }
            break;
            
        default:
            break;
    }
}

//******************************************************************************
// showUseResultDlg
//******************************************************************************
void ItemCardLayer::showUseResultDlg()
{
    setTouchEnable();
    
    if(MainScene::Inst()->new_FirstItem)
    {
        MainScene::Inst()->new_FirstItem_Second = true;
        
        Arrow* pArrow = new Arrow(30, 255);
        
        pArrow->setPosition(CCPointMake(231, 400));
        
        
        
        pArrow->setRotation(180);
        //    pArrow->setScale(0.6f);
        
        MainScene::Inst()->addChild(pArrow, 50000, 93996);
        
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));
    }
    
    
    
    showItemCard();
    
    if (m_dlgResult == NULL)
    {
        m_dlgResult = new UseResultDialog(this,
                                          menu_selector(ItemCardLayer::cbUseResultDlg));
        addChild(m_dlgResult, 50);
        m_dlgResult->setPosition(CCPointMake(0, -getPosition().y));
        m_dlgResult->release();
    }
    m_dlgResult->showUseResultDialog(m_useCardType,m_useCardCount);
}

//******************************************************************************
// cbUseResultDlg
//******************************************************************************
void ItemCardLayer::cbUseResultDlg()
{
    if (m_dlgResult)
    {
        m_dlgResult->removeFromParentAndCleanup(true);
        m_dlgResult = NULL;
    }
    
    
    
    if(MainScene::Inst()->new_FirstItem)
    {
        MainScene::Inst()->removeChildByTag(93996 , true);
        
        Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(ItemCardLayer::newbie2),
                                            CGameData::Inst()->getLanguageValue("item_new21"),false);
        addChild(newbieEndDlg, 500, kNewbie);
        newbieEndDlg->release();
        newbieEndDlg->setPosition(CCPointMake(320, 300));
        newbieEndDlg->setScale(0.0);
        newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        
        
        
        Arrow* pArrow = new Arrow(30, 255);
        
        pArrow->setPosition(CCPointMake(231, 245));
        
        
        
        pArrow->setRotation(180);
        //    pArrow->setScale(0.6f);
        
        MainScene::Inst()->addChild(pArrow, 50000, kNewbie + 99999);
        
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));
    }
    
}

//******************************************************************************
// useItemCard
//******************************************************************************
void ItemCardLayer::useItemCard()
{    
    if (m_useCardType == IMCARDSTRTYPE_RENAME)
    {
        if (m_cntryDialog == NULL) {
            m_cntryDialog = new CountryDialog(this, callfunc_selector(ItemCardLayer::cbSetNameCardDlg));
            addChild(m_cntryDialog,20);
            m_cntryDialog->release();
            m_cntryDialog->setPosition(ccp(getPosition().x,-getPosition().y));
        }
        m_cntryDialog->showSetNameCardDialog();
    }
    else if (m_useCardType == IMCARDSTRTYPE_GACHA)
    {
        m_rtyTime = CGameData::Inst()->getCurrentTime();
        if(CGameData::Inst()->chargeGachaRequest(false, m_useCardId, m_rtyTime, false))
        {
            scheduleUpdate();
            m_curReqType = enReqType_ChargeGacha;
        }
    }
    else if (m_useCardType == IMCARDSTRTYPE_GACHA_MULTI)
    {
        if(CGameData::Inst()->multiChargeGachaRequest(m_useCardId))
        {
            scheduleUpdate();
            m_curReqType = enReqType_MutiChargeGacha;
        }
    }
    else if (m_useCardType == IMCARDSTRTYPE_STONE)
    {
        if(CGameData::Inst()->useStoneReq(m_useCardId, m_useCardCount))
        {
            m_curReqType = enReqType_Stone;
            scheduleUpdate();
        }
    }
    else if (m_useCardType == IMCARDSTRTYPE_COMPOUND)
    {
        
//        if(CGameData::Inst()->useStoneReq(m_useCardId, m_useCardCount))
//        {
//            m_curReqType = enReqType_COMPOUND;
//            scheduleUpdate();
//        }"guild_item_config""compound_item""compound_card"
        
        /*
         
         "guild_item_config" : {
         "1001_item" : {
         "content" : "单次求将卡",
         "item_dsp" : "获得1位武将",
         "itemid" : "itemcard8.png",
         "type" : "gacha"
         },
         
         
         
        "compound_card" : {
            "1" : {
                "coin" : 30,
                "element" : {
                    "1402_item" : 2
                },
                "end_time" : "2014-02-01 00:00:00",
                "gold" : 1000000,
                "lv" : 1,
                "rate" : 1,
                "start_time" : "2014-01-22 00:00:00",
                "target" : "1_card",
                "wealth" : 100000
            },
            "2" : {
                "coin" : 30,
                "element" : {
                    "1402_item" : 2
                },
                "end_time" : "2014-02-01 00:00:00",
                "gold" : 1000000,
                "lv" : 1,
                "rate" : 1,
                "start_time" : "2014-01-22 00:00:00",
                "target" : "2_card",
                "wealth" : 100000
            }
        },
        "compound_item" : {
            "1" : {
                "coin" : 30,
                "element" : {
                    "1401_item" : 2
                },
                "end_time" : "2014-02-01 00:00:00",
                "gold" : 1000000,
                "rate" : 1,
                "start_time" : "2014-01-22 00:00:00",
                "target" : "1001_item",
                "wealth" : 100000
            },
            "2" : {
                "coin" : 30,
                "element" : {
                    "1401_item" : 22
                },
                "end_time" : "2014-02-01 00:00:00",
                "gold" : 1000000,
                "rate" : 1,
                "start_time" : "2014-01-22 00:00:00",
                "target" : "1002_item",
                "wealth" : 100000
            }
        },
        "dungeon" : {
            
            */
    }
    else if (CGameData::Inst()->useItemReq(m_useCardId, m_useCardCount))
    {
        m_curReqType = enReqType_UseItemCard;
        scheduleUpdate();
    }
}

void ItemCardLayer::cbSetNameCardDlg()
{
    showSureNameCardTips();
}

void ItemCardLayer::showSureNameCardTips()
{
    removeChild(m_cntryDialog, true);
    m_cntryDialog = NULL;
    
    if (m_cntryDialog == NULL) {
        m_cntryDialog = new CountryDialog(this, callfuncO_selector(ItemCardLayer::cbSureNameCardTips));
        addChild(m_cntryDialog,50);
        m_cntryDialog->release();
        m_cntryDialog->setPosition(ccp(getPosition().x,-getPosition().y));
    }
    
    m_cntryDialog->showSureNameCardDialog();
}


void ItemCardLayer::cbSureNameCardTips(CCObject* pObj)
{
    CntryDlgRet* ret = (CntryDlgRet*)pObj;
    if (ret->bOk)
    {
        setNameCard();
        removeChild(m_cntryDialog, true);
        m_cntryDialog = NULL;
    }
    
    else
    {
        removeChild(m_cntryDialog, true);
        m_cntryDialog = NULL;
    }
}

void ItemCardLayer::setNameCard()
{
    if (CGameData::Inst()->useItemCardRename(m_useCardId));
    {
        m_curReqType = enReqType_SetNameCard;
        scheduleUpdate();
    }
}

//******************************************************************************
// getAllCards
//******************************************************************************
void ItemCardLayer::getAllCards()
{
    if (CGameData::Inst()->allCardRequest())
    {
        m_curReqType = enReqType_GetAllCards;
        scheduleUpdate();
    }
}



void  ItemCardLayer::showInf()
{
    if (m_showCard == NULL)
    {
        m_showCard = new CShowCardInf();
        addChild(m_showCard, 150);
        m_showCard->release();
    }
    
    if(CGameData::Inst()->getUsrCardInfByUcid(ucidbuf))
        m_showCard->setPosition(ccp(getPosition().x,-getPosition().y));
        m_showCard->show(CGameData::Inst()->getUsrCardInfByUcid(ucidbuf));
    m_showCard->setOnExitCB(this,
                            callfunc_selector(ItemCardLayer::cbShowCard));
}

void ItemCardLayer::cbShowCard()
{
    if (m_showCard != NULL)
    {
        m_showCard->removeAllChildrenWithCleanup(true);
        removeChild(m_showCard, true);
        m_showCard = NULL;
    }
    
    m_ret.iActType = enMoveTitelHere;
    (m_pListener->*m_pfnSelector)(&m_ret);
    
    showUseResultDlg();
}

void ItemCardLayer::backEnter()
{
    if (pGachaLayer) {
        pGachaLayer->removeFromParentAndCleanup(true);
        pGachaLayer = NULL;
    }
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    showInf();
}

void ItemCardLayer::gachaGetCallback(CCObject* pSender)
{
    GachaGetRet *ret = (GachaGetRet*)pSender;
    
    switch (ret->gachaType)
    {
        case UnusualGachaType:
            {
                backEnter();
            }
            break;
            
        case UnusualGacha10Type:
        {
            if (pGachaLayer) {
                pGachaLayer->removeFromParentAndCleanup(true);
                pGachaLayer = NULL;
            }
   
            CCTextureCache::sharedTextureCache()->removeUnusedTextures();
            MainScene::Inst()->showUsrInfPanel(true);
            
            m_spTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
            addChild(m_spTitle, 300);
            
            CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
            CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
            CCMenuItem* m_miBack = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(ItemCardLayer::BtnCallBack) );
            m_miBack->setTag(4);
            m_miBack->setAnchorPoint(CCPointZero);
            m_menu = CCMenu::menuWithItems( m_miBack, NULL);
            addChild(m_menu, 300);
            m_menu->setPosition(CCPointZero);
            
            int deltaY = -getPosition().y;
            
            m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("gachaTip6"));
            m_spTitle->setPosition(CCPointMake(-210, 766 + deltaY));
            m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766 + deltaY)), 0.8f));
            
            m_miBack->setPosition(CCPointMake(-529, 731 + deltaY));
            m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731 + deltaY)), 0.8f));
            
            if (m_RareGacha10Show == NULL)
            {
                m_RareGacha10Show = new RareGacha10Show(this,callfuncO_selector(ItemCardLayer::RareGacha10ShowCb));
                addChild(m_RareGacha10Show,150);
                m_RareGacha10Show->release();
            }
            
            m_RareGacha10Show->setPosition(ccp(getPosition().x, -getPosition().y));
            m_RareGacha10Show->show(true);
        }
            break;
            
        default:
            break;
    }
}

//******************************************************************************
// update
//******************************************************************************
void ItemCardLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(ItemCardLayer::cbCommDlg),
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
    
    if(comState == kReqStatOk)
    {
        switch (m_curReqType) {
            case enReqType_UseItemCard:
            {
                if (m_useCardType == "card")
                    getAllCards();
                else
                    showUseResultDlg();
            }
                break;
                
            case enReqType_SetNameCard:
            {
                removeChildByTag(kUseItemCard, true);
                showUseResultDlg();
            }
                break;
            
            case enReqType_GetAllCards:
                showUseResultDlg();
                break;
            
            case enReqType_Stone:
                showUseResultDlg();
                break;
                
            case enReqType_ChargeGacha:
            {
                removeChildByTag(kUseItemCard, true);
                
                strcpy(ucidbuf, CGameData::Inst()->getGachaRet()->ucid);
                if(CGameData::Inst()->getGachaRet()->plus>0)
                    m_isGachaPlus = true;
                else
                    m_isGachaPlus = false;
                
                if(CGameData::Inst()->allCardRequest())
                {
                    scheduleUpdate();
                    m_curReqType = enReqType_ChargeGachaOver;
                }
            }
                break;
                
            case enReqType_ChargeGachaOver:
            {
                pGachaLayer = new GachaGet(UnusualGachaType,ucidbuf,this,callfuncO_selector(ItemCardLayer::gachaGetCallback),m_isGachaPlus,true);
                addChild(pGachaLayer,200);
                pGachaLayer->release();
                pGachaLayer->setPosition(ccp(getPosition().x, -getPosition().y));
                MainScene::Inst()->showBottomPanel(false);
                MainScene::Inst()->showUsrInfPanel(false);
                
                m_ret.iActType = enMoveTitleLeave;
                (m_pListener->*m_pfnSelector)(&m_ret);
            }
                break;
                
            case enReqType_MutiChargeGacha:
            {
                removeChildByTag(kUseItemCard, true);
                
                CGameData::Inst()->getMultiGachaRet();
                if(CGameData::Inst()->allCardRequest())
                {
                    m_curReqType = enReqType_MutiChargeGachaOver;
                    scheduleUpdate();
                }
            }
                break;
                
                
                
            case enReqType_GiveWuJiang_Item:
                
                //guild_item
                if(CGameData::Inst()->newChangeRequest("guild_item"))
                {
                    
                    m_curReqType = enReqType_card_update_Item;
                    scheduleUpdate();
                }
                
                break;
                
            case enReqType_card_update_Item:
                
                if(CGameData::Inst()->allCardRequest())
                {
                    
                    m_curReqType = enReqType_GiveWuJiangAllCards_Item;
                    scheduleUpdate();
                }
                
                break;
                
            case enReqType_GiveWuJiangAllCards_Item:
                
                m_preStatus = CGameData::Inst()->getCntryInfo()->status;
                showItemCard();

                

                
                break;
                
                
                
                
            case enReqType_MutiChargeGachaOver:
            {
                pGachaLayer = new GachaGet(UnusualGacha10Type,ucidbuf,this,callfuncO_selector(ItemCardLayer::gachaGetCallback),m_isGachaPlus,true);
                addChild(pGachaLayer,200);
                pGachaLayer->release();
                pGachaLayer->setPosition(ccp(getPosition().x, -getPosition().y));
                MainScene::Inst()->showBottomPanel(false);
                MainScene::Inst()->showUsrInfPanel(false);
                
                m_ret.iActType = enMoveTitleLeave;
                (m_pListener->*m_pfnSelector)(&m_ret);
            }
                break;
                
            default:
                break;

        }
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(ItemCardLayer::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480 - getPosition().y));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}

//******************************************************************************
// cbCommDlg
//******************************************************************************
void ItemCardLayer::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    MainScene::Inst()->enableBottomPanel(true);
    
    if(isOk == true){
        switch (m_curReqType) {
            case enReqType_UseItemCard:
                useItemCard();
                break;
                
            case enReqType_SetNameCard:
            {
                setNameCard();
            }
                break;
                
            case enReqType_GetAllCards:
                getAllCards();
                break;
                
            case enReqType_ChargeGacha:
            {
                if(CGameData::Inst()->chargeGachaRequest(false, m_useCardId, m_rtyTime, false))
                {
                    scheduleUpdate();
                }
            }
                break;
                
            case enReqType_ChargeGachaOver:
            {
                if(CGameData::Inst()->allCardRequest())
                {
                    scheduleUpdate();
                }
            }
                break;
                
            case enReqType_MutiChargeGacha:
            {
                if(CGameData::Inst()->multiChargeGachaRequest(m_useCardId))
                {
                    scheduleUpdate();
                }
            }
                break;
                
                
                
            case enReqType_GiveWuJiang_Item:
                
                if(CGameData::Inst()->newGiveWuJiangRequest("4"))
                {
                    
                    m_curReqType = enReqType_GiveWuJiang_Item;
                    scheduleUpdate();
                }
                
                break;
                
            case enReqType_card_update_Item:
                
                 //guild_item
                if(CGameData::Inst()->newChangeRequest("guild_item"))
                {
                    
                    m_curReqType = enReqType_card_update_Item;
                    scheduleUpdate();
                }
                
                break;
                
            case enReqType_GiveWuJiangAllCards_Item:
                
               
                if(CGameData::Inst()->allCardRequest())
                {
                    
                    m_curReqType = enReqType_GiveWuJiangAllCards_Item;
                    scheduleUpdate();
                }
                
                break;
                



                
            case enReqType_MutiChargeGachaOver:
            {
                if(CGameData::Inst()->allCardRequest())
                {
                    scheduleUpdate();
                }
            }
                break;
                
            default:
                break;
        }
    }
    else {
        CGameData::Inst()->clearReqStat();
        if (CGameData::Inst()->getCntryInfo()->status != m_preStatus)
        {
            m_ret.iActType = enItmCardLyQuit;
            (m_pListener->*m_pfnSelector)(&m_ret);
        }
    }
}

//******************************************************************************
// onExit
//******************************************************************************
void ItemCardLayer::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}











