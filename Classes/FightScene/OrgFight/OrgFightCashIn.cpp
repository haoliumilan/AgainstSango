//
//  OrgFightCashIn.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 13-2-6.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "OrgFightCashIn.h"
#include "TextNode.h"
#include "ActNumTTF.h"
#include "CGameData.h"
#include "TitleBar.h"
#include "MainScene.h"

//******************************************************************************
// Constructor
//******************************************************************************
OrgFightCashIn::OrgFightCashIn(CCObject* target, SEL_CallFunc selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_lbFightName = NULL;
    m_spLastAtk = NULL;
    m_lbMyAtk = NULL;
    m_lbGold = NULL;
    m_lbWealth = NULL;
    m_lbCntrbt = NULL;
    m_numMyAtk = NULL;
    m_numGold = NULL;
    m_numWealth = NULL;
    m_numCntrbt = NULL;
    m_showDetailLayer = NULL;
    m_bTxtAnimOver = false;
    m_bItmAnimOver = false;
}

//******************************************************************************
// show
//******************************************************************************
void OrgFightCashIn::show(const char* szFightName)
{
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    TitleBar* title = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
    addChild(title);
    title->setTitleString(CGameData::Inst()->getLanguageValue("cashInLayerTip9"));
    title->setPosition(CCPointMake(-210, 766));
    title->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(210, 766)), 0.8f));
    
    if (szFightName) {
        m_lbFightName = TextNode::textWithString(szFightName, CCSizeMake(600, 42), CCTextAlignmentLeft, 36);
        m_lbFightName->setAnchorPoint(ccp(0, 0.5));
        addChild(m_lbFightName, 1);
        
        m_lbFightName->setPosition(ccp(-500, 700));
        m_lbFightName->runAction(CCMoveTo::actionWithDuration(0.4, ccp(100, 690)));
    }
    
    if (CGameData::Inst()->getCntryDgEndInf()->whokillwho == 1) {
        if (m_spLastAtk == NULL) {
            m_spLastAtk = CCSprite::spriteWithFile("orglastatk.png");
            addChild(m_spLastAtk, 1);
            
            m_spLastAtk->setPosition(ccp(-300, 430));
        }
        
        CCSprite* sp = CCSprite::spriteWithFile("tfcg.png");
        if(sp){
            addChild(sp);
            sp->setPosition(ccp(320, 560));
        }
        
        MainScene::Inst()->switchBgMusic(enBgMusicOrgWin);
    }
    else{
        CCSprite* sp = CCSprite::spriteWithFile("mjsb.png");
        if(sp){
            addChild(sp);
            sp->setPosition(ccp(320, 560));
        }
        
        MainScene::Inst()->switchBgMusic(enBgMusicOrgFail);
    }
    
    if(m_lbMyAtk == NULL){
        m_lbMyAtk = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntrydgend_atk"), 30);
        addChild(m_lbMyAtk, 1);
        
        m_lbMyAtk->setAnchorPoint(ccp(0, 0.5));
        m_lbMyAtk->setPosition(ccp(-300, 380));
        
        if(m_numMyAtk == NULL){
            m_numMyAtk = ActNumTTF::initNumber(CCSizeMake(300, 30), CCTextAlignmentLeft, 30, ccc3(255,255,255));
            m_numMyAtk->setPosition(CCPointMake(500, 380));
            addChild(m_numMyAtk, 1);
        }
    }
    
    if(m_lbWealth == NULL){
        m_lbWealth = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntrydgend_wealth"), 30);
        addChild(m_lbWealth, 1);
        
        m_lbWealth->setAnchorPoint(ccp(0, 0.5));
        m_lbWealth->setPosition(ccp(-300, 330));
        
        if(m_numWealth == NULL){
            m_numWealth = ActNumTTF::initNumber(CCSizeMake(300, 30), CCTextAlignmentLeft, 30, ccc3(255,255,255));
            m_numWealth->setPosition(CCPointMake(500, 330));
            addChild(m_numWealth, 1);
        }
    }
    
    if(m_lbCntrbt == NULL){
        m_lbCntrbt = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntrydgend_cntrbt"), 30);
        addChild(m_lbCntrbt, 1);
        
        m_lbCntrbt->setAnchorPoint(ccp(0, 0.5));
        m_lbCntrbt->setPosition(ccp(-300, 280));
        
        if(m_numCntrbt == NULL){
            m_numCntrbt = ActNumTTF::initNumber(CCSizeMake(300, 30), CCTextAlignmentLeft, 30, ccc3(255,255,255));
            m_numCntrbt->setPosition(CCPointMake(500, 280));
            addChild(m_numCntrbt, 1);
        }
    }
    
    if(m_lbGold == NULL){
        m_lbGold = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntrydgend_coin"), 30);
        addChild(m_lbGold, 1);
        
        m_lbGold->setAnchorPoint(ccp(0, 0.5));
        m_lbGold->setPosition(ccp(-300, 230));
        
        if(m_numGold == NULL){
            m_numGold = ActNumTTF::initNumber(CCSizeMake(300, 30), CCTextAlignmentLeft, 30, ccc3(255,255,255));
            m_numGold->setPosition(CCPointMake(500, 230));
            addChild(m_numGold, 1);
        }
    }
    
    schedule(schedule_selector(OrgFightCashIn::onTimer), 1.0);
}

//******************************************************************************
// onTimer
//******************************************************************************
void OrgFightCashIn::onTimer(ccTime dt)
{
    bool bAction = false;
    
    //最后一击
    if((m_spLastAtk)
       && (m_spLastAtk->numberOfRunningActions() == 0)
       && (m_spLastAtk->getPosition().x < 0)){
        bAction = true;
        m_spLastAtk->runAction(CCMoveTo::actionWithDuration(0.4, ccp(320, 430)));
    }
    
    if (bAction)
        return;
    
    //造成伤害
    if((m_lbMyAtk)
       && (m_lbMyAtk->numberOfRunningActions() == 0)
       && (m_lbMyAtk->getPosition().x < 0)){
        bAction = true;
        
        m_lbMyAtk->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.4, ccp(100, 380)),
                                                 CCCallFunc::actionWithTarget(this, callfunc_selector(OrgFightCashIn::cbLbMyAtk)),
                                                 NULL));
    }
    
    if (bAction)
        return;
    //获得粮饷
    if((m_lbWealth)
       && (m_lbWealth->numberOfRunningActions() == 0)
       && (m_lbWealth->getPosition().x < 0)){
        bAction = true;
        
        m_lbWealth->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.4, ccp(100, 330)),
                                                  CCCallFunc::actionWithTarget(this, callfunc_selector(OrgFightCashIn::cbLbWealth)),
                                                  NULL));
    }
    
    if (bAction)
        return;
    //获得个人贡献
    if((m_lbCntrbt)
       && (m_lbCntrbt->numberOfRunningActions() == 0)
       && (m_lbCntrbt->getPosition().x < 0)){
        bAction = true;
        
        m_lbCntrbt->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.4, ccp(100, 280)),
                                                CCCallFunc::actionWithTarget(this, callfunc_selector(OrgFightCashIn::cbLbCntrbt)),
                                                NULL));
    }
    
    if (bAction)
        return;
    //获得铜钱
    if((m_lbGold)
       && (m_lbGold->numberOfRunningActions() == 0)
       && (m_lbGold->getPosition().x < 0)){
        bAction = true;
        
        m_lbGold->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.4, ccp(100, 230)),
                                                CCCallFunc::actionWithTarget(this, callfunc_selector(OrgFightCashIn::cbLbGold)),
                                                NULL));
    }
    
    if (!bAction) {
        unschedule(schedule_selector(OrgFightCashIn::onTimer));
        m_bTxtAnimOver = true;
        showItemCard();
    }
}

void OrgFightCashIn::cbLbMyAtk()
{
    if(m_numMyAtk){
        m_numMyAtk->setNumber(CGameData::Inst()->getCntryDgEndInf()->playAtk, 0.5);
    }
}

void OrgFightCashIn::cbLbWealth()
{
    if(m_numWealth){
        m_numWealth->setNumber(CGameData::Inst()->getCntryDgEndInf()->wealth, 0.5);
    }
}

void OrgFightCashIn::cbLbCntrbt()
{
    if(m_numCntrbt){
        m_numCntrbt->setNumber(CGameData::Inst()->getCntryDgEndInf()->contribution, 0.5);
    }
}

void OrgFightCashIn::cbLbGold()
{
    if(m_numGold){
        m_numGold->setNumber(CGameData::Inst()->getCntryDgEndInf()->gold, 0.5);
    }
}


void OrgFightCashIn::crtShowDetailLayer()
{
    m_showDetailLayer = CCSprite::spriteWithSpriteFrameName("propertyTip_dlg.png");
    m_showDetailLayer->setPosition(ccp(220, 160+(m_showDetailLayer->getContentSize().height/2)));
    addChild(m_showDetailLayer, 100);
    m_showDetailLayer->setScale(0);
}

void OrgFightCashIn::ccTouchEnded(CCTouch* touch,CCEvent* event)
{
    if (m_showDetailLayer != NULL) {
        m_showDetailLayer->removeFromParentAndCleanup(true);
        m_showDetailLayer = NULL;
    }
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool OrgFightCashIn::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    
    if (m_bTxtAnimOver) {
        if (m_bItmAnimOver) {
            CntryDgEndInf* pDgEndInf = CGameData::Inst()->getCntryDgEndInf();
            int iItemCnt = pDgEndInf->lstItem.size();
            if (iItemCnt > 0) {
                int gap_table[] = {0, 80, 60, 40, 16};
                CCSprite* spTmp = CCSprite::spriteWithFile("itemcard1.png");
                if (spTmp) {
                    CCPoint ptStart;
                    ptStart.x = (640 - (spTmp->getContentSizeInPixels().width * iItemCnt + (iItemCnt - 1) * gap_table[iItemCnt - 1])) * 0.5;
                    ptStart.y = 50;
                    CCPoint touchLocation = touch->locationInView(touch->view());
                    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
                    CCPoint pos = convertToNodeSpace(touchLocation);
                    bool bRet = false;
                    int  num = 0;
                     for (list<ItemCardInf>::iterator it = pDgEndInf->lstItem.begin(); it != pDgEndInf->lstItem.end(); ++it){
                         {
                              bRet = CCRect::CCRectContainsPoint(CCRectMake( ptStart.x + (spTmp->getContentSizeInPixels().width + gap_table[iItemCnt - 1]) * num , ptStart.y, spTmp->getContentSize().width, spTmp->getContentSize().height), pos);
                             if (bRet) {
                                 crtShowDetailLayer();
                                 ItemCardInf* itemCardInf = CGameData::Inst()->getItemCardByID(it->strItemId);
                                 
                                 char buf[512] = {0};
                                 sprintf(buf, "%sx%ld",itemCardInf->strItemName.c_str(),it->lCnt);
                                 
                                 TextNode * gname = TextNode::textWithString(buf, CCSize(400,100), CCTextAlignmentCenter, 25);
                                 m_showDetailLayer->addChild(gname, 10);
                                 gname->setPosition(ccp(175,76));
                                 
                                 TextNode * curLevel = TextNode::textWithString(itemCardInf->strItemDsp.c_str(), CCSize(400,100), CCTextAlignmentCenter, 23);
                                 m_showDetailLayer->addChild(curLevel, 10);
                                 curLevel->setPosition(ccp(173,34));
                                 m_showDetailLayer->setPosition(ccp(320 , 230));
                                 m_showDetailLayer->runAction(CCScaleTo::actionWithDuration(0.1f, 1.0f));
                                 return true;
                             }
                             num++;
                         }
                   }
               }
              
            }
            
            (m_pListener->*m_pfnSelector)();
            return true;
        }
        else{
            if(showItemCard()){
                return true;
            }
            else{
                (m_pListener->*m_pfnSelector)();
                return true;
            }
        }
    }
    else{
        unscheduleAllSelectors();
        
        if (m_spLastAtk) {
            m_spLastAtk->stopAllActions();
            m_spLastAtk->setPosition(ccp(320, 430));
        }
        
        if(m_lbMyAtk){
            m_lbMyAtk->stopAllActions();
            m_lbMyAtk->setPosition(ccp(100, 380));
        }
        
        if(m_lbWealth){
            m_lbWealth->stopAllActions();
            m_lbWealth->setPosition(ccp(100, 330));
        }
        
        if(m_lbCntrbt){
            m_lbCntrbt->stopAllActions();
            m_lbCntrbt->setPosition(ccp(100, 280));
        }
        
        if(m_lbGold){
            m_lbGold->stopAllActions();
            m_lbGold->setPosition(ccp(100, 230));
        }

        if(m_numMyAtk){
            m_numMyAtk->setNumber(CGameData::Inst()->getCntryDgEndInf()->playAtk, 0);
        }

        if(m_numWealth){
            m_numWealth->setNumber(CGameData::Inst()->getCntryDgEndInf()->wealth, 0);
        }

        if(m_numCntrbt){
            m_numCntrbt->setNumber(CGameData::Inst()->getCntryDgEndInf()->contribution, 0);
        }
        
        if(m_numGold){
            m_numGold->setNumber(CGameData::Inst()->getCntryDgEndInf()->gold, 0);
        }

        m_bTxtAnimOver = true;
        showItemCard();
        
        return false;
    }
}


//******************************************************************************
// showItemCard
//******************************************************************************
bool OrgFightCashIn::showItemCard()
{
    CntryDgEndInf* pDgEndInf = CGameData::Inst()->getCntryDgEndInf();
    int iItemCnt = pDgEndInf->lstItem.size();
    
    if (iItemCnt > 0) {
        setIsTouchEnabled(false);
        
        int gap_table[] = {0, 80, 60, 40, 16};
        
        CCSprite* spTmp = CCSprite::spriteWithFile("itemcard1.png");
        if (spTmp) {
            CCPoint ptStart;
            ptStart.x = (640 - (spTmp->getContentSizeInPixels().width * iItemCnt + (iItemCnt - 1) * gap_table[iItemCnt - 1])) * 0.5;
            ptStart.y = 50;
            
            int i = 0;
            for (list<ItemCardInf>::iterator it = pDgEndInf->lstItem.begin(); it != pDgEndInf->lstItem.end(); ++it){
                ItemCardInf ItmInf = *it;
                
//              CCLOG("\nM%s-------M%s-----------M%s----------M%s-----------M%s\n",ItmInf.strItemName.c_str(),ItmInf.strItemId.c_str(),ItmInf.strResName.c_str(),ItmInf.strType.c_str(),ItmInf.strItemDsp.c_str());
                
                CCSprite* spItem = CCSprite::spriteWithFile(ItmInf.strResName.c_str());
                if (spItem) {
                    addChild(spItem);
                    
                    spItem->setAnchorPoint(ccp(0, 0));
                    
                    CCPoint ptItem;
                    ptItem.x = ptStart.x + (spTmp->getContentSizeInPixels().width + gap_table[iItemCnt - 1]) * i;
                    ptItem.y = ptStart.y;
                    
                    spItem->setPositionInPixels(ccp(ptItem.x + 700, ptItem.y));
                    spItem->runAction(CCMoveTo::actionWithDuration(0.3, ptItem));
                    
                    char buf[30];
                    sprintf(buf, "%ld",ItmInf.lCnt);
                    
                    CCLabelTTF* textNum = CCLabelTTF::labelWithString(buf, FONTSTHEITISCM, 25);
                    textNum->setAnchorPoint(ccp(0.5,0.5));
                    spItem->addChild(textNum,0,9);
                    textNum->setPosition(ccp(89,88));
                    textNum->setColor(ccc3(35, 80, 4));
                }
                
                i++;
            }
            
            schedule(schedule_selector(OrgFightCashIn::cbShowItemCard), 0.3);
        }
        return true;
    }
    else{
        m_bItmAnimOver = true;
        return false;
    }
    
    
}

//******************************************************************************
// cbShowItemCard
//******************************************************************************
void OrgFightCashIn::cbShowItemCard(ccTime dt)
{
    unschedule(schedule_selector(OrgFightCashIn::cbShowItemCard));
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    m_bItmAnimOver = true;
}



