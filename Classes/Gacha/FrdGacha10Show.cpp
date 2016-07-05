//
//  FrdGacha10Show.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 12-11-14.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "FrdGacha10Show.h"
#include "CShowCardInf.h"
#include "CSkillLib.h"
#include "SimpleAudioEngine.h"
#include "GachaGet.h"
#include "MainScene.h"
#include "CGamedata.h"
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
#include "OcProxy.h"

#define kRareGacha10TagBase 100

//******************************************************************************
// Constructor
//******************************************************************************
FrdGacha10Show::FrdGacha10Show(CCObject* listener,SEL_CallFuncO selector)
: m_isGachaMaterial(false)
{
    m_isGachaMaterial = false;
    m_showCard = NULL;
    m_pListener=listener;
    m_pfnSelector=selector;
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool FrdGacha10Show::ccTouchBegan(CCTouch* touch, CCEvent* event)
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
void FrdGacha10Show::show()
{
    removeAllChildrenWithCleanup(true);
    CCSprite *plusIcon = NULL;
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 20);
    m_showCard->release();
    m_showCard->setIsVisible(false);
    m_showCard->setShowBtmAfterHide(false);
    
    this->setIsVisible(true);

    
    
    
    //分享按钮
    sharestar=0;
    
    CCSprite *tempSprite1 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    CCSprite *tempSprite2 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
    
    m_shareBtn =  CCMenuItemImage::itemFromNormalSprite(tempSprite1, tempSprite2, this, menu_selector(FrdGacha10Show::shareSanGuo));
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

    //各种定义，数值准备
    CCMutableArray<CGachaRet *> * pGachaRetArr = CGameData::Inst()->getMultiGachaRet();
    CGachaRet * pGachaRet;
    for(int i=0;i<pGachaRetArr->count();i++)
    {
        CGachaRet *tempRet;
        tempRet = pGachaRetArr->getObjectAtIndex(i);
        if(tempRet->is_compGachaMaterial)
        {
            m_isGachaMaterial = true;
            break;
        }
    }
    
    CUserCardInfo* pUsrCardInfo;
    
    m_CardCnt = pGachaRetArr->count();
    if(m_CardCnt == 0){
        return;
    }

    char buf[100];
    CCSprite* spLingpai;
    CCPoint ptCard(100, 620);
    float dt = 0.0;
    CCSprite* spCard;
    CCSprite* spHeadw;
    
    //动画
    for(int i = 0; i < m_CardCnt; i++){
        if(i == 5){
            ptCard.x = 100;
            ptCard.y = 510;
        }
        if(i == 10){
            ptCard.x = 100;
            ptCard.y = 400;
        }
        
        pGachaRet = pGachaRetArr->getObjectAtIndex(i);
        
        if(pGachaRet == NULL)
            continue;
        
        pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(pGachaRet->ucid);
        
        if(pUsrCardInfo == NULL)
            continue;
        
        sp = CCSprite::spriteWithFile("box_none.png");
        sp->setPositionInPixels(ccp(700, ptCard.y));
        addChild(sp, 0, kRareGacha10TagBase + i);

        if(i >= 5)
            sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.1),
                                              CCMoveTo::actionWithDuration(0.3, ptCard),
                                              NULL));
        else
            sp->runAction(CCMoveTo::actionWithDuration(0.3, ptCard));
        
        if (pUsrCardInfo->star > 5) {
            snprintf(buf, 99, "droplv5.png");
        }
        else {
            snprintf(buf, 99, "droplv%d.png", pUsrCardInfo->star);
        }
        //yanghui
        if (pUsrCardInfo->star > 2) {
            sharestar= pUsrCardInfo->star;
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
                spLingpai->addChild(plusIcon);
                plusIcon->setPosition(ccp(60, 20));
               // plusIcon->setOpacity(0);
            }
            
            spLingpai->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt + 0.2),
                                                     CCFadeOut::actionWithDuration(0.1),
                                                     NULL) );
            if(plusIcon)
            {
                plusIcon->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt + 0.2),
                                                         CCFadeOut::actionWithDuration(0.1),
                                                         NULL) );
            }
        }
        

        spHeadw = CCSprite::spriteWithFile("headw.png");
        if(spHeadw){
            sp->addChild(spHeadw, 100);
            spHeadw->setOpacity(0);
            spHeadw->setPosition(ccp(sp->getContentSize().width * 0.5,
                                     sp->getContentSize().height * 0.5));
            spHeadw->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
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
            spCard->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt + 0.2),
                                                  CCFadeIn::actionWithDuration(0.1),
                                                  NULL) );
        }
        
        dt += 0.3;
        ptCard.x += 110;
    }
    
    //文字
    sprintf(buf,
            "%s%ld",
            CGameData::Inst()->getLanguageValue("gachaDlgTip3"),
            CGameData::Inst()->getUsrInfo()->gacha_pt);
    
    TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(560, 130), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(960, 150));
    addChild(lbText, 1);
    
    lbText->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt + 0.2),
                                          CCMoveTo::actionWithDuration(0.3, CCPointMake(320, 150)),
                                          NULL) );
    
    sprintf(buf,
            "%s%ld%s",
            CGameData::Inst()->getLanguageValue("gachaDlgTip4"),
            CGameData::Inst()->getUsrInfo()->gacha_pt / CGameData::Inst()->getCommonInfo()->gacha_cost_pt,
            CGameData::Inst()->getLanguageValue("gachaDlgTip5"));
    
    lbText = TextNode::textWithString(buf, CCSizeMake(560, 130), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(960, 100));
    addChild(lbText, 1);
    
    lbText->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt + 0.2),
                                          CCMoveTo::actionWithDuration(0.3, CCPointMake(320, 100)),
                                          CCCallFunc::actionWithTarget(this, callfunc_selector(FrdGacha10Show::cbActionEnd)),
                                          NULL) );
}

//******************************************************************************
// hide
//******************************************************************************
void FrdGacha10Show::hide()
{
    removeAllChildrenWithCleanup(true);
    
    setIsTouchEnabled(false);
    
    this->setIsVisible(false);
}

//******************************************************************************
// cbActionEnd
//******************************************************************************
void FrdGacha10Show::cbActionEnd()
{
    
    if (CGameData::Inst()->getCommonInfo()->sina_voice_fg|CGameData::Inst()->getCommonInfo()->qq_voice_fg |CGameData::Inst()->getCommonInfo()->weixin_voice_fg)
    {
        if (sharestar>2) {
            m_shareBtn->setPosition(ccp(1000, 740));
            m_shareBtn->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(530, 730)),0.8f));
        }
    }
    
 
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    if(m_isGachaMaterial)
    {
        CCSprite *tempSprite1 = CCSprite::spriteWithFile("button_dianjiang.png");
        CCSprite * tempSprite2 = CCSprite::spriteWithFile("button_dianjiang.png");
        CCMenuItem *goCompGachaBtn = CCMenuItemImage::itemFromNormalSprite(tempSprite1, tempSprite2,this, menu_selector(FrdGacha10Show::goCompGacha));
        goCompGachaBtn ->setAnchorPoint(CCPointZero);
        goCompGachaBtn->setIsVisible(CGameData::Inst()->getCommonInfo()->compgacha_fg);
        CCMenu *menu = CCMenu::menuWithItem(goCompGachaBtn);
        menu->setPosition(ccp(160, 220));
        addChild(menu);
    }
    m_isGachaMaterial = false;
    
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


void FrdGacha10Show::goCompGacha()
{
    (m_pListener->*m_pfnSelector)(&m_ret);

}
//******************************************************************************
// onExit
//******************************************************************************
void FrdGacha10Show::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}




void FrdGacha10Show::shareToWeibo()
{
    
    if(CGameData::Inst()->isSoundOn)
        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if(CGameData::Inst()->getLoginType()==enLoginReqType_OcLogin)
    {
        Dialog2 *dialog = new Dialog2(this,
                                      callfuncO_selector(FrdGacha10Show::dl2CallBack),
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
void FrdGacha10Show::shareSanGuo(){
    
    OcProxy::Inst()->sharetoSanguo(this, callfunc_selector(FrdGacha10Show::shareToWeibo));
    
}


void FrdGacha10Show::dl2CallBack(CCObject *sender)
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




