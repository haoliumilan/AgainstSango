//
//  RareGacha10Show.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 12-11-13.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//
#include "CGamedata.h"
#include "CShowCardInf.h"
#include "RareGacha10Show.h"
#include "CSkillLib.h"
#include "SimpleAudioEngine.h"
#include "GachaGet.h"
#include "MainScene.h"
#include "CommDlg.h"
#include "TitleBar.h"
#include "Arrow.h"
#include "DialogForGacha.h"
#include "GachaWeb.h"
#include "Dialog1.h"
#include "SimpleAudioEngine.h"
#include "TextNode.h"
#include "Dialog2.h"
#include "CompGachaWeb.h"
#include "CListLayer.h"
#include "OcProxy.h"

#define kRareGacha10TagBase 100

//******************************************************************************
// Constructor
//******************************************************************************
RareGacha10Show::RareGacha10Show(CCObject* listener,SEL_CallFuncO selector)
: m_isGachaMaterial(false)
{
    m_showCard = NULL;
    m_pListener=listener;
    m_pfnSelector=selector;
}

//******************************************************************************
// onEnter
//******************************************************************************
void RareGacha10Show::onEnter()
{
    CCLayer::onEnter();
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool RareGacha10Show::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint nodePos = convertToNodeSpace( touchLocation );
    
    CCNode* pTouchedCard = NULL;
    
    for(int i = 0; i < m_CardCnt; i++){
        CCNode* pCard = getChildByTag(kRareGacha10TagBase + i);
        
        if(pCard){
            CCPoint ptCard = pCard->getPosition();
            CCSize szCard = pCard->getContentSize();
            
            szCard.width *= pCard->getScale();
            szCard.height *= pCard->getScale();
            
            bool bTouched = CCRect::CCRectContainsPoint(CCRectMake(ptCard.x - szCard.width * 0.5,
                                                                   ptCard.y - szCard.height * 0.5,
                                                                   szCard.width,
                                                                   szCard.height),
                                                        nodePos);
            
            if(bTouched){
                pTouchedCard = pCard;
                break;
            }
        }
    }
    
    if(pTouchedCard){
        CCMutableArray<CGachaRet *> * pGachaRetArr = CGameData::Inst()->getMultiGachaRet();
        CGachaRet * pGachaRet;
        CUserCardInfo* pUsrCardInfo;
        
        pGachaRet = pGachaRetArr->getObjectAtIndex(pTouchedCard->getTag() - kRareGacha10TagBase);
        if(pGachaRet){
            pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(pGachaRet->ucid);
            
            if((pUsrCardInfo) && (m_showCard->getIsVisible() == false)){
                m_showCard->show(pUsrCardInfo);
            }
        }
    }
    
    return true;
}

//******************************************************************************
// show
//******************************************************************************
void RareGacha10Show::show(bool isFromItemCard)
{
    removeAllChildrenWithCleanup(true);
    
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 20);
    m_showCard->release();
    m_showCard->setIsVisible(false);
    m_showCard->setShowBtmAfterHide(false);
    
    this->setIsVisible(true);
    
    
    
    //分享按钮
   
    CCSprite *tempSprite1 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    CCSprite *tempSprite2 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    
    m_shareBtn =  CCMenuItemImage::itemFromNormalSprite(tempSprite1, tempSprite2, this, menu_selector(RareGacha10Show::shareSanGuo));
    m_shareBtn->setAnchorPoint(CCPointZero);
    CCMenu *shareMenu = CCMenu::menuWithItem(m_shareBtn);
    addChild(shareMenu,2);
    shareMenu->setPosition(CCPointZero);
    TextNode *shareLabel;
    shareLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("shareTip"), 30);
    shareLabel->setPosition(ccp(53, 35));
    shareLabel->setShadowColor(ccBLACK);
    m_shareBtn->addChild(shareLabel);
    m_shareBtn->setPosition(CCPoint(-1000, -1000));
    
 
    
    CCSprite* sp;
    
//背景图
    sp = CCSprite::spriteWithFile("10gachabg.jpg");
    sp->setPositionInPixels(ccp(320, 480));
    addChild(sp, -1);
    
    sp->setOpacity(0);
    sp->runAction(CCFadeIn::actionWithDuration(1.0));
    
//各种定义，数值准备
    CCMutableArray<CGachaRet *> * pGachaRetArr = CGameData::Inst()->getMultiGachaRet();
    CGachaRet * pGachaRet;
    if (!isFromItemCard)
    {
        for(int i=0;i<pGachaRetArr->count();i++)
        {
            CGachaRet *tempRet;
            tempRet = pGachaRetArr->getObjectAtIndex(i);
            if(tempRet->is_compGachaMaterial)
            {
                m_isGachaMaterial = tempRet->is_compGachaMaterial;
                break;
            }
        }
    }
    CUserCardInfo* pUsrCardInfo;
    
    m_CardCnt = pGachaRetArr->count();
    if(m_CardCnt == 0){
        return;
    }
    
    //find best card
    int iBestCardIdx = 0;
    int iStar = 0;
    for(int i = 0; i < m_CardCnt; i++){
        pGachaRet = pGachaRetArr->getObjectAtIndex(i);
        if(pGachaRet){
            pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(pGachaRet->ucid);
            
            if(pUsrCardInfo){
                if((pUsrCardInfo->star > iStar)
                   ||((pUsrCardInfo->star == iStar) && (pUsrCardInfo->category == enGeneralTp_God))){
                    iStar = pUsrCardInfo->star;
                    iBestCardIdx = i;
                }
            }
        }
    }
    
    float dtAngle = -3.1415926 * 2 / (m_CardCnt -1);
    float curAngle = 3.1415926 / 2;
    float fRadius = 220.0;
    float dtTime = 0.0;
    float dtLcTime = m_CardCnt * 0.1 + 0.5;
    CCPoint dtPt;
    CCPoint ptCenter(320, 400);
    CCSprite* spLingpai;
    CCSprite* spHeadw;
    CCSprite* spCard;
    CCSprite *plusIcon = NULL;
    char buf[100];
    
    
    
//动画
    for(int i = 0; i < m_CardCnt; i++){
        if(i == iBestCardIdx)
            continue;
        
        pGachaRet = pGachaRetArr->getObjectAtIndex(i);
        
        if(pGachaRet == NULL)
            continue;
        
        pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(pGachaRet->ucid);
        
        if(pUsrCardInfo == NULL)
            continue;
        
        dtPt.x = fRadius * cos(curAngle);
        dtPt.y = fRadius * sin(curAngle);
        
        sp = CCSprite::spriteWithFile("box_none.png");
        sp->setPositionInPixels(ccpAdd(ptCenter, dtPt));
        addChild(sp, 0, kRareGacha10TagBase + i);
        
        if (pUsrCardInfo->star > 5) {
            snprintf(buf, 99, "droplv5.png");
        }
        else {
            snprintf(buf, 99, "droplv%d.png", pUsrCardInfo->star);
        }
        
        
        spLingpai = CCSprite::spriteWithFile(buf);
        if(spLingpai){
            spLingpai->setAnchorPoint(CCPointZero);
            spLingpai->setScale(0.8);
            spLingpai->setPositionInPixels(ccp(20, 16));
            sp->addChild(spLingpai);
            
            plusIcon = NULL;
            if(pGachaRet->plus >0)
            {
                plusIcon= CCSprite::spriteWithSpriteFrameName("icon_plus.png");
                sp->addChild(plusIcon);
                plusIcon->setPosition(ccp(60, 20));
                plusIcon->setOpacity(0);
            }
            
            spLingpai->setOpacity(0);
            spLingpai->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dtTime),
                                                     CCFadeIn::actionWithDuration(0.01),
                                                     CCDelayTime::actionWithDuration(dtLcTime - dtTime),
                                                     CCFadeOut::actionWithDuration(0.01),
                                                     NULL));
            //+1
            if(plusIcon)
            {
                plusIcon->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dtTime),
                                                         CCFadeIn::actionWithDuration(0.01),
                                                         CCDelayTime::actionWithDuration(dtLcTime - dtTime),
                                                         CCFadeOut::actionWithDuration(0.01),
                                                         NULL));
            }
        }
        
        sp->setScale(2.0);
        sp->setOpacity(0);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dtTime),
                                          CCFadeIn::actionWithDuration(0.01),
                                          CCScaleTo::actionWithDuration(0.1, 1.0),
                                          NULL));
        
        
        
        spHeadw = CCSprite::spriteWithFile("headw.png");
        if(spHeadw){
            sp->addChild(spHeadw, 100);
            spHeadw->setOpacity(0);
            spHeadw->setPosition(ccp(sp->getContentSize().width * 0.5,
                                     sp->getContentSize().height * 0.5));
            spHeadw->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dtLcTime),
                                                   CCFadeIn::actionWithDuration(0.2),
                                                   CCFadeOut::actionWithDuration(0.2),
                                                   NULL) );
        }
        
        
        spCard = CGameData::Inst()->getHeadSprite(pUsrCardInfo->cid);
        if(spCard){
            sp->addChild(spCard, 50);
            spCard->setOpacity(0);
            spCard->setPosition(ccp(spCard->getContentSize().width / 2 - 4,
                                    spCard->getContentSize().height / 2 + 1));
            spCard->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dtLcTime + 0.2),
                                                  CCFadeIn::actionWithDuration(0.1),
                                                  NULL) );
        }
        
        dtTime += 0.1;
        dtLcTime += 0.1;
        curAngle += dtAngle;
    }
    
    pGachaRet = pGachaRetArr->getObjectAtIndex(iBestCardIdx);
    
    if(pGachaRet == NULL)
        return;
    
    pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(pGachaRet->ucid);
    
    if(pUsrCardInfo == NULL)
        return;
    
    sp = CCSprite::spriteWithFile("box_none.png");
    addChild(sp, 0, kRareGacha10TagBase + iBestCardIdx);
    sp->setPositionInPixels(ptCenter);
    sp->setScale(4.0);
    sp->setOpacity(0);
    
    if (pUsrCardInfo->star > 5) {
        snprintf(buf, 99, "droplv5.png");
    }
    else {
        snprintf(buf, 99, "droplv%d.png", pUsrCardInfo->star);
    }
    

    
    spLingpai = CCSprite::spriteWithFile(buf);
    if(spLingpai){
        spLingpai->setAnchorPoint(CCPointZero);
        spLingpai->setScale(0.8);
        spLingpai->setPositionInPixels(ccp(20, 16));
        sp->addChild(spLingpai);
        plusIcon = NULL;
        if(pGachaRet->plus >0)
        {
            plusIcon= CCSprite::spriteWithSpriteFrameName("icon_plus.png");
            sp->addChild(plusIcon);
            plusIcon->setPosition(ccp(60, 20));
            plusIcon->setOpacity(0);
        }
        
        spLingpai->setOpacity(0);
        spLingpai->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dtTime),
                                                 CCFadeIn::actionWithDuration(0.01),
                                                 CCDelayTime::actionWithDuration(dtLcTime - dtTime),
                                                 CCFadeOut::actionWithDuration(0.01),
                                                 NULL));
        
        if(plusIcon)
        {
            plusIcon->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dtTime),
                                                     CCFadeIn::actionWithDuration(0.01),
                                                     CCDelayTime::actionWithDuration(dtLcTime - dtTime),
                                                     CCFadeOut::actionWithDuration(0.01),
                                                     NULL));
        }
    }
    
    sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dtTime),
                                      CCFadeIn::actionWithDuration(0.01),
                                      CCScaleTo::actionWithDuration(0.1, 2.0),
                                      NULL));
    
    
    
    spHeadw = CCSprite::spriteWithFile("headw.png");
    if(spHeadw){
        sp->addChild(spHeadw, 100);
        spHeadw->setOpacity(0);
        spHeadw->setPosition(ccp(sp->getContentSize().width * 0.5,
                                 sp->getContentSize().height * 0.5));
        spHeadw->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dtLcTime),
                                               CCFadeIn::actionWithDuration(0.2),
                                               CCFadeOut::actionWithDuration(0.2),
                                               NULL) );
    }
    
    spCard = CGameData::Inst()->getHeadSprite(pUsrCardInfo->cid);
    if(spCard){
        sp->addChild(spCard, 50);
        spCard->setOpacity(0);
        spCard->setPosition(ccp(spCard->getContentSize().width / 2 - 4,
                                spCard->getContentSize().height / 2 + 1));
        spCard->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dtLcTime + 0.2),
                                              CCFadeIn::actionWithDuration(0.1),
                                              CCCallFunc::actionWithTarget(this, callfunc_selector(RareGacha10Show::cbActionEnd)),
                                              NULL) );
    }
}

//******************************************************************************
// hide
//******************************************************************************
void RareGacha10Show::hide()
{
    removeAllChildrenWithCleanup(true);
    
    setIsTouchEnabled(false);
    
    this->setIsVisible(false);
}

//******************************************************************************
// cbActionEnd  结束
//******************************************************************************
void RareGacha10Show::cbActionEnd()
{
    if (CGameData::Inst()->getCommonInfo()->sina_voice_fg|CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg)
    {
        m_shareBtn->setPosition(ccp(1000, 740));
        m_shareBtn->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(530, 730)),0.8f));
    }
   
       
    if(m_isGachaMaterial)
    {
        CCSprite *tempSprite1 = CCSprite::spriteWithFile("button_dianjiang.png");
        CCSprite * tempSprite2 = CCSprite::spriteWithFile("button_dianjiang.png");
        CCMenuItem *goCompGachaBtn = CCMenuItemImage::itemFromNormalSprite(tempSprite1, tempSprite2,this, menu_selector(RareGacha10Show::goCompGacha));
        goCompGachaBtn ->setAnchorPoint(CCPointZero);
        goCompGachaBtn->setIsVisible(CGameData::Inst()->getCommonInfo()->compgacha_fg);
        CCMenu *menu = CCMenu::menuWithItem(goCompGachaBtn);
        addChild(menu);
        menu->setPosition(ccp(160, 0));
    }
    m_isGachaMaterial = false;
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    
    //append item
    std::string szTemp;
    CGameData::Inst()->getGuildAwardText(szTemp);
    if (!szTemp.empty())
    {
        if (CCSprite *pSprite = CCSprite::spriteWithFile("append_item_text_box.png"))
        {
            CCLayer *pLayer = CCLayer::node();
            pLayer->addChild(pSprite);
//            pLayer->setPosition(ccp(320,50));
             pLayer->setPosition(ccp(320,700));
            addChild(pLayer,10);
            
            TextNode *pText = TextNode::textWithString(szTemp.c_str(),22);
            pText->setAnchorPoint(ccp(0.5,0.5));
            pText->setColor(ccc3(255,211,65));
            pText->setShadowColor(ccBLACK);
            pLayer->addChild(pText);
        }
    }
    
    

}


void RareGacha10Show::goCompGacha()
{
    (m_pListener->*m_pfnSelector)(&m_ret);

}
//******************************************************************************
// onExit
//******************************************************************************
void RareGacha10Show::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}

void RareGacha10Show::shareToWeibo()
{
    
    if(CGameData::Inst()->isSoundOn)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if(CGameData::Inst()->getLoginType()==enLoginReqType_OcLogin)
    {
        Dialog2 *dialog = new Dialog2(this,
                                      callfuncO_selector(RareGacha10Show::dl2CallBack),
                                      CGameData::Inst()->getLanguageValue("weibo_authened"),
                                      CGameData::Inst()->getLanguageValue("weibo_authened_info"),
                                      enDl2TwoBtn);
        addChild(dialog,100);
        dialog->setTag(111);
        dialog->setPosition(ccp(320, 480));
        dialog->release();
    }
    else
    {
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCRenderTexture *target = CCRenderTexture::renderTextureWithWidthAndHeight(winSize.width, winSize.height);
        target->begin();
        MainScene::Inst()->visit();
        target->end();
        target->saveBuffer(kCCImageFormatPNG,"captureImage.png");
        MainScene::Inst()->showWeiboPublishInfoWithType(6);
    }
}



//******************************************************************************
// shareSanguo--yang
//******************************************************************************
void RareGacha10Show::shareSanGuo(){
    
    OcProxy::Inst()->sharetoSanguo(this, callfunc_selector(RareGacha10Show::shareToWeibo));

}


void RareGacha10Show::dl2CallBack(CCObject *sender)
{
    Dialog2Ret *ret = (Dialog2Ret *)sender;
    Dialog2 *dialog = (Dialog2 *)getChildByTag(111);
    if(ret->bOk == true)
    {
        MainScene::Inst()-> gotoOtherMain();
    }else {
        dialog->removeFromParentAndCleanup(true);
    }
}

