//
//  Package.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-25.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "Package.h"
#include "CGamedata.h"
#include "PackCard.h"
#include "CScrollBar.h"
#include "CShowCardInf.h"
#include "MainScene.h"
#include "CGeneralLayer.h"
#include "CConfirmLayer.h"
#include "PackSelBtn.h"
#include "Dialog2.h"
#include "CommDlg.h"
#include "SimpleAudioEngine.h"
#include "Arrow.h"
#include "TextNode.h"
#include "CSkillLib.h"
#include "OcProxy.h"
#include "ScrollViewLayer.h"
#include "Dialog1.h"


//#include "ui_maa_basic.h"

#define cfmLayerChild   -990
#define lcCoverChild    -991
#define kCardTagBase    1000
#define kPackArrowTag   100

#define kNewbie         99999

using namespace CocosDenshion;

static float gfDownLimitY = 192.0;

static int g_nTickCount = 0;
//******************************************************************************
// Constructor
//******************************************************************************
int Package::getCardTagBase()
{
    return kCardTagBase;
}

int Package::getTickCount()
{
    return g_nTickCount;
}

Package::Package(int iLookTp, CCObject* target, SEL_CallFuncO selector, int costCanUse)
{
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(Package::NtyGoTop),  // 处理的消息的回调函数
                                                           kNotifyGoTop,                    // 感兴趣的消息名称
                                                           NULL);
    
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(Package::NtyGoBottom),  // 处理的消息的回调函数
                                                           kNotifyGoBottom,                    // 感兴趣的消息名称
                                                           NULL);
    
    arrowpoint=ccp(-1000,-1000);
    testCount = 0;
    isLongPress = false;
    m_commDlg = NULL;
    m_cfmLayer = NULL;
    m_pScrollBar = NULL;
    m_lcCover = NULL;
    m_spMaxCard = NULL;
    m_lbMaxCard = NULL;
    m_packSelBtn = NULL;
    m_sellDlg = NULL;
    m_niepanDlg = NULL;
    m_sellUpThreeStarsDlg = NULL;
    m_pScrollViewLayer = NULL;
    m_iLookTp = iLookTp;
    m_iSelCnt = 0;
    m_usrCardNum = 0;
    m_iPreThisPosY = 0;
    m_noMainGeneralDlg = NULL;
    
    m_pListener = target;
    m_pfnSelector = selector;
    m_dlgPoint = CCPoint(320,480);
    memset(m_ret.uid, 0, sizeof(m_ret.uid));
    
    m_iTotalCost = CGameData::Inst()->getUsrInfo()->cost;
    m_iCurCost = CGameData::Inst()->getCurTeamCost();
    
    if (FiveTeamLayer != NULL) {
        removeAllChildrenWithCleanup(true);
        removeChild(FiveTeamLayer, true);
        FiveTeamLayer = NULL;
    }
    
    m_pUserCardArr = CGameData::Inst()->getUserCards();
    m_packCardArr = new CCMutableArray<PackCard *>();
    m_usrCardNum = m_pUserCardArr->count();
    CUserCardInfo* pUsrCard;
    CUserCardInfo* pBaseCard;
    CUserInfo* pUsrInf = CGameData::Inst()->getUsrInfo();
    
    char buf[100];
    snprintf(buf, 99, "%d/%d", m_usrCardNum, pUsrInf->max_card_num);
    if(enPkLkTp_PwUpFood == iLookTp
       || enPkLkTp_NiePanFood == iLookTp){
        char* ucidBase = CGameData::Inst()->getCombineCard(0);
        pBaseCard = CGameData::Inst()->getUsrCardInfByUcid(ucidBase);
        
        m_iBaseCardCType = pBaseCard->ctype;
        
        m_packSelBtn = new PackSelBtn(buf, pBaseCard->lv * 100, pBaseCard->plus);
        addChild(m_packSelBtn, 10);
        m_packSelBtn->release();
    }
    else if(enPkLkTp_Sell == iLookTp){
        m_packSelBtn = new PackSelBtn(buf);
        addChild(m_packSelBtn, 10);
        m_packSelBtn->release();
    }
    else if (enPkLkTp_TeamReset == iLookTp) {
        FiveTeamLayer = CCSprite::spriteWithFile("fr_substrate.png");
        FiveTeamLayer->setPosition(CCPointMake(320, 190));
        addChild(FiveTeamLayer, 10);
        
        char buf[100];
        CCSprite *spMenu1 = CCSprite::spriteWithFile("fr_substrate.png");
        CCSprite *spMenu2 = CCSprite::spriteWithFile("fr_substrate.png");
        CCMenuItem *menuBG = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(Package::cbFiveTeamLayerBG));
        menuBG->setAnchorPoint(CCPointZero);
        menuBG->setPosition(CCPointMake(0,0));
        
        spMenu1 = CCSprite::spriteWithFile("fr_button2.png");
        spMenu2 = CCSprite::spriteWithFile("fr_button2.png");
        CCMenuItem *reset = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(Package::cbFiveTeamLayerReset));
        reset->setAnchorPoint(CCPointZero);
        reset->setPosition(CCPointMake(5, 8));
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("helperFiveTeamResetTip"));
        TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(100, 30), CCTextAlignmentCenter, kBtnTitleHeight);
        lbText->setPosition(CCPointMake(10, 20));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        reset->addChild(lbText, 1);
        
        spMenu1 = CCSprite::spriteWithFile("fr_button2.png");
        spMenu2 = CCSprite::spriteWithFile("fr_button2.png");
        CCMenuItem *sure = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(Package::cbFiveTeamLayerSure));
        sure->setAnchorPoint(CCPointZero);
        sure->setPosition(CCPointMake(515, 8));
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("helperFiveTeamSureTip"));
        lbText = TextNode::textWithString(buf, CCSizeMake(100, 30), CCTextAlignmentCenter, kBtnTitleHeight);
        lbText->setPosition(CCPointMake(10, 20));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        sure->addChild(lbText, 1);
        
        
        FiveTeamBGMenu = CCMenu::menuWithItems(menuBG, NULL);
        FiveTeamMenu = CCMenu::menuWithItems(reset, sure, NULL);
        FiveTeamLayer->addChild(FiveTeamBGMenu, 0);
        FiveTeamLayer->addChild(FiveTeamMenu, 1);
        FiveTeamMenu->setPosition(CCPointZero);
        FiveTeamBGMenu->setPosition(CCPointZero);
    }
    else{
        initCapacity(buf);
    }

    //添加卡片
    CCSprite* sp = CGameData::Inst()->getHeadSprite(1);
    m_cardSize = sp->getContentSize();
    
    m_iColumn = 640 / m_cardSize.width;
    m_iGap = (640 - m_iColumn * m_cardSize.width) / (m_iColumn + 1);
    
    m_ptStart.x = m_iGap;
    m_ptStart.y = 720;

    CCPoint pt;
    PackCard* card;
    m_iRmBtn = 0;
    
    
    
    
    
    
    
    if(m_iLookTp == enPkLkTp_TeamEditSub){
        sp = CCSprite::spriteWithSpriteFrameName("fr_box.png");
        addChild(sp);
        sp->setAnchorPoint(CCPointMake(0, 1));
        sp->setPosition(m_ptStart);
        
        TextNode* lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("package_remove"), sp->getContentSize(), CCTextAlignmentCenter, 30);
        lbText->setAnchorPoint(CCPointMake(0, 1));
        lbText->setPosition(m_ptStart);
        lbText->setColor(ccWHITE);
        addChild(lbText, 2);
        
        m_iRmBtn = 1;
    }
    
    int iMaxCnt = MAX(m_usrCardNum, pUsrInf->max_card_num);
    int npIndex = 0;
    
     bool have = false;
    
    for(int i = 0; i < iMaxCnt; i++)
    {
        if(i >= m_usrCardNum){
            if(m_iLookTp == enPkLkTp_JustLook){
                pt.x = m_ptStart.x + ((i + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width);
                pt.y = m_ptStart.y - ((i + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height);
                
                sp = CCSprite::spriteWithSpriteFrameName("fr_box.png");
                addChild(sp);
                sp->setOpacity(128);
                sp->setAnchorPoint(CCPointMake(0, 1));
                sp->setPosition(pt);
            }
            else {
                break;
            }
        }
        else{
            pUsrCard = m_pUserCardArr->getObjectAtIndex(i);
            
            if (m_iLookTp == enPkLkTp_NiePanFood)
            {
                if (!CGameData::Inst()->getIsNiePanFoodByCid(pUsrCard->cid))
                    continue;
            }
            
            pt.x = m_ptStart.x + ((npIndex + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width);
            pt.y = m_ptStart.y - ((npIndex + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height);
            
            if(MainScene::Inst()->new_FirstZhuanSheng)
            {
                if(!have)
                {
                    have = true;
                    
                    MainScene::Inst()->enableBottomPanel(false);
                    
                    Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(Package::newbie),
                                                        CGameData::Inst()->getLanguageValue("zhuanSheng_new15"),false);
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
            
            
            if(MainScene::Inst()->new_FirstPowerUp)
            {
                
                MainScene::Inst()->removeTouchTip();
                    
                if(MainScene::Inst()->new_FirstPowerUpSecond)
                {
                    if(( (m_iLookTp == enPkLkTp_PwUpBase)) && !have){
                        Arrow* pArrow = new Arrow(30, 255);
                        pArrow->setPosition(CCPointMake(pt.x + 54, pt.y - 100));
                        
                        
                        pArrow->setRotation(90);
                        addChild(pArrow, 9, kPackArrowTag);
                        pArrow->release();
                        
                        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                                           NULL);
                        pArrow->runAction(CCRepeatForever::actionWithAction(action));
                        
                        have = true;
                    }
                    
                    if((m_iLookTp == enPkLkTp_PwUpFood) && !have){
                        Arrow* pArrow = new Arrow(30, 255);
                        pArrow->setPosition(CCPointMake(pt.x + 180 , pt.y - 100));
                        pArrow->setRotation(90);
                        addChild(pArrow, 9, kPackArrowTag);
                        pArrow->release();
                        
                        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                                           NULL);
                        pArrow->runAction(CCRepeatForever::actionWithAction(action));
                        
                        have = true;
                    }
                }
                else
                {
                    if(( (m_iLookTp == enPkLkTp_PwUpBase)) && !have){
                        
                        
                        
                        MainScene::Inst()->enableBottomPanel(false);
                        
                        Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(Package::newbie4),
                                                            CGameData::Inst()->getLanguageValue("qiangHua_new22"),false);
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

                        have = true;
                    }
                    
                    if((m_iLookTp == enPkLkTp_PwUpFood) && !have){
                        Arrow* pArrow = new Arrow(30, 255);
                        pArrow->setPosition(CCPointMake(pt.x + 180 + 126, pt.y - 100));
                        pArrow->setRotation(90);
                        addChild(pArrow, 9, kPackArrowTag);
                        pArrow->release();
                        
                        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                                           NULL);
                        pArrow->runAction(CCRepeatForever::actionWithAction(action));
                        
                        have = true;
                    }
                }
            }
            
            if(CGameData::Inst()->isNewbie()){
                if(((m_iLookTp == enPkLkTp_TeamEditSub) || (m_iLookTp == enPkLkTp_PwUpBase))
                   && (pUsrCard->cid == 49)){
                    Arrow* pArrow = new Arrow(30, 255);
                    pArrow->setPosition(CCPointMake(pt.x + 54, pt.y - 100));
                    pArrow->setRotation(90);
                    addChild(pArrow, 9, kPackArrowTag);
                    pArrow->release();
                    
                    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                                       NULL);
                    pArrow->runAction(CCRepeatForever::actionWithAction(action));
                }
                
                if((m_iLookTp == enPkLkTp_PwUpFood) && (pUsrCard->cid == 87)){//    87  183
                    Arrow* pArrow = new Arrow(30, 255);
                    pArrow->setPosition(CCPointMake(pt.x + 54, pt.y - 100));
                    pArrow->setRotation(90);
                    addChild(pArrow, 9, kPackArrowTag);
                    pArrow->release();
                    
                    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                                       NULL);
                    pArrow->runAction(CCRepeatForever::actionWithAction(action));
                }

            }
            
            
            
            if(m_iLookTp == enPkLkTp_JustLook){
                card = new PackCard(pUsrCard, false, false,false);}
            else{
                if((m_iLookTp == enPkLkTp_PwUpBase)
                    ||(m_iLookTp == enPkLkTp_Evolution))
                {
                    if(MainScene::Inst()->new_FirstPowerUp || MainScene::Inst()->new_FirstZhuanSheng)
                    {
                        card = new PackCard(pUsrCard, false, false,false);
                    }
                    else
                        card = new PackCard(pUsrCard, true, false,false);
                }
                
                else if(m_iLookTp == enPkLkTp_PwUpFood
                        || m_iLookTp == enPkLkTp_NiePanFood)
                {
                    if(MainScene::Inst()->new_FirstPowerUp || MainScene::Inst()->new_FirstZhuanSheng)
                        card = new PackCard(pUsrCard, false, true,false);
                    else
                    card = new PackCard(pUsrCard, true, true,false);
                    
                    for (int i = 1; i <= 5; i++) {
                        if (strcmp(CGameData::Inst()->getCombineCard(i), card->getUcid()) == 0) {
                            card->setSel(true);
                            m_iSelCnt++;
                            if(m_packSelBtn != NULL){
                                if(m_iBaseCardCType == card->getcType())
                                    m_packSelBtn->add1cardUp(card->getEatExp() * 1.5, card->getPlus());
                                else
                                    m_packSelBtn->add1cardUp(card->getEatExp(), card->getPlus());
                            }
                            break;
                        }
                    }
                }
                else if (m_iLookTp == enPkLkTp_TeamEditLeader || enPkLkTp_TeamEditSub == m_iLookTp)
                {
                     card = new PackCard(pUsrCard, true, true,true);
                }
                else if (m_iLookTp == enPkLkTp_TeamReset)
                {
                    card = new PackCard(pUsrCard, false, false,false);
                }
                else
                    card = new PackCard(pUsrCard, true, true,false);
            }
            
            addChild(card);
            m_packCardArr->addObject(card);
            card->release();
            card->setPosition(pt);
            card->setTag(kCardTagBase + npIndex);
            
            
            //check gray
            switch (m_iLookTp) {
                case enPkLkTp_TeamEditLeader:
                case enPkLkTp_TeamEditSub:
//                case enPkLkTp_TeamReset:
                    if(pUsrCard->cost > costCanUse)
                    {
                        card->setCostEnough(false);
                        card->setGray(true);
                    }
                    break;
                    
                case enPkLkTp_PwUpBase:
                {
                    char sklv_max = CSkillLib::Ins()->getManSkill(pUsrCard->skid)->cMaxLv;
                    if (pUsrCard->category == 7 || pUsrCard->category == 8
                        || (pUsrCard->lv == pUsrCard->max_lv
                            && pUsrCard->sk_lv == sklv_max
                            && pUsrCard->plus == 297
                            && pUsrCard->i_AwakeNum >= card->getMaxAwake()
                            && pUsrCard->bIsNPTgt == false))
                    {
                        card->setGray(true);
                    }
                }
                    break;
                    
                case enPkLkTp_PwUpFood:
                    if(strncmp(pBaseCard->ucid, pUsrCard->ucid, 99) == 0)
                    {
                        card->setGray(true);
                        card->setPowUp(true);
                    }
                    else if (CGameData::Inst()->getIsLock(pUsrCard->ucid))
                    {
                        card->setGray(true);
                    }
                    break;
                
                case enPkLkTp_NiePanFood:
                    if(strncmp(pBaseCard->ucid, pUsrCard->ucid, 99) == 0)
                    {
                        card->setGray(true);
                        card->setPowUp(true);
                    }
                    else if (CGameData::Inst()->getIsLock(pUsrCard->ucid))
                    {
                        card->setGray(true);
                    }
                    break;
                    
                case enPkLkTp_Evolution:
                    if((pUsrCard->upg_flg == 2) || (pUsrCard->elu_flg == 2) || (pUsrCard->wk_flg == 2)){
                        //卡亮
                    }
                    else if((pUsrCard->upg_flg == 1) || (pUsrCard->elu_flg == 1) || (pUsrCard->wk_flg == 1)){
                        card->justSetGray();
                    }
                    else{
                        card->setGray(true);
                    }
                    break;
                    
                case enPkLkTp_Sell:
                    if (CGameData::Inst()->getIsLock(pUsrCard->ucid)) {
                        card->setGray(true);
                    }
                    break;
                default:
                    break;
            }
            npIndex++;
        }
    }
    m_usrCardNum = npIndex;

    gfDownLimitY = 192.0;
    
    if((enPkLkTp_PwUpFood == iLookTp
        || enPkLkTp_NiePanFood == iLookTp)
        ||(enPkLkTp_Sell == iLookTp)
        ||(enPkLkTp_TeamReset == iLookTp))
        gfDownLimitY = 244.0;
    
    m_fUpLimitY = gfDownLimitY - (pt.y - m_cardSize.height);
    
    if(m_fUpLimitY > 0){
        m_pScrollBar = new CScrollBar();
        addChild(m_pScrollBar);
        m_pScrollBar->setTag(1);
        
        m_pScrollBar->release();

        float fBarSize = 1.0 / ((m_fUpLimitY / (m_ptStart.y - gfDownLimitY)) + 2);
        m_pScrollBar->setBarSize(fBarSize);
        m_pScrollBar->setPercent(0.0);
        
        if((enPkLkTp_PwUpFood == iLookTp)
           || (enPkLkTp_NiePanFood == iLookTp)
           || (enPkLkTp_Sell == iLookTp)
           || (enPkLkTp_TeamReset == iLookTp)){
            m_pScrollBar->setPosition(CCPointMake(630, 482));
            m_pScrollBar->setScaleY(0.84);
        }
        else{
            m_pScrollBar->setPosition(CCPointMake(630, 452));
            m_pScrollBar->setScaleY(0.94);
        }
    }
    
    
    
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 20);
    m_showCard->release();
    m_showCard->setIsVisible(false);
    
    m_pScrollViewLayer = CScrollViewLayer::create(this);
    addChild(m_pScrollViewLayer,21);
    //m_pScrollViewLayer->show((unsigned int)0);
    
    switch (m_iLookTp) {
        case enPkLkTp_JustLook:
        case enPkLkTp_TeamEditLeader:
        case enPkLkTp_TeamEditSub:
        case enPkLkTp_PwUpBase:
        case enPkLkTp_Evolution:
            m_cfmLayer = new CConfirmLayer(this, callfuncO_selector(Package::cbCfm));
            addChild(m_cfmLayer, 10,cfmLayerChild);
            m_cfmLayer->release();
            break;

        default:
            break;
    }
    
    if (m_iLookTp == enPkLkTp_NiePanFood)
    {
        char* ucidBase = CGameData::Inst()->getCombineCard(0);
        CUserCardInfo* pBaseCard = CGameData::Inst()->getUsrCardInfByUcid(ucidBase);
        int maxCount = pBaseCard->np_need_num;
        
        if (m_iSelCnt == maxCount)
            setAllCardGray(true);
    }
    else
    {
        if (m_iSelCnt == 5) {
            setAllCardGray(true);
        }
    }
    
    if(CGameData::Inst()->isNewbie() == false)
    {
        if(MainScene::Inst()->new_FirstPowerUp || MainScene::Inst()->new_FirstZhuanSheng  || MainScene::Inst()->new_CardAdd )
        {
            if(m_pScrollBar)
            m_pScrollBar->setIsVisible(false);
            rangePackageCardWithType(enCardRangeTypeGet);
            
           
        }
        else
            rangePackageCardWithType(CGameData::Inst()->getDefaultRangeType());
    }
    
    
    if(MainScene::Inst()->new_CardAdd)
    {
        CCMutableArray<PackCard *> *s_packCardArray =  CCMutableArray<PackCard *>::arrayWithArray(m_packCardArr);
        
        
        for(int i=0;i<s_packCardArray->count()-1;i++)
        {
            for (int j=0;j< s_packCardArray->count()-1-i; j++) {
                //                char *uid1 = s_packCardArray->getObjectAtIndex(j)->getUcid();
                //                char *uid2 = s_packCardArray->getObjectAtIndex(j+1)->getUcid();
                unsigned long updTime1 = s_packCardArray->getObjectAtIndex(j)-> getUpdTime();
                unsigned long updTime2 = s_packCardArray->getObjectAtIndex(j+1)->getUpdTime();
                //CCLog("atack 1 %s 2 %s",s_packCardArray->getObjectAtIndex(0)->getUcid(),s_packCardArray->getObjectAtIndex(1)->getUcid());
                if(updTime1 < updTime2)
                {
                    PackCard *tempCard1 = s_packCardArray->getObjectAtIndex(j);
                    PackCard *tempCard2 = s_packCardArray->getObjectAtIndex(j+1);
                    s_packCardArray->replaceObjectAtIndex(j, tempCard2);
                    s_packCardArray->replaceObjectAtIndex(j+1, tempCard1);
                }
            }
        }

        for(int i=0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->getPlus() > 0)
            {
                
                
                pt.x = m_ptStart.x + (i % m_iColumn - 1) * (m_iGap + m_cardSize.width);
                pt.y = m_ptStart.y - (i / m_iColumn) * (m_iGap + m_cardSize.height);
                
                
                Arrow* pArrow = new Arrow(30, 255);
                pArrow->setPosition(CCPointMake(pt.x + 180 , pt.y - 100));
                pArrow->setRotation(90);
                addChild(pArrow, 9, kPackArrowTag);
                pArrow->release();
                
                CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                                   CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                                   NULL);
                pArrow->runAction(CCRepeatForever::actionWithAction(action));


                m_NewCardID = i;
                
                break;
                
            }
            
            
        }
    }
//    else
//        setThisPosition(CCPointZero, true);
    
    if (enPkLkTp_TeamReset == iLookTp) { // 初始化当前队伍对应的选中状态
        resetIGeneral();
        
        stUid* pTeamInfo = CGameData::Inst()->getCurTeamInf();
        CUserCardInfo* pUsrCardInf;
        
        int index = 0;
        for(int i = 0; i < 5; i++){
            if(pTeamInfo[i].uid[0] == 0)
                continue;
            pUsrCardInf = CGameData::Inst()->getUsrCardInfByUcid(pTeamInfo[i].uid);
            if(pUsrCardInf == NULL)
                continue;
            for(int j = 0; j < m_usrCardNum; j++){
                CCNode* node = getChildByTag(kCardTagBase + j);
                PackCard* card = (PackCard*) node;
                if (!card->isSelected() && !card->isUsed() && !card->isPowUp()) {
                    if (strcmp(card->getUcid(), pUsrCardInf->ucid) == 0) {
                        card->setNumber(index, true);
                        FiveTeamMemberSelected(card->getUcid());
                        index++;
                        break;
                    }
                }
            }
        }
    }
    
    schedule(schedule_selector(Package::tickTimeCardText), 1.5f);
    MainScene::Inst()->exitArenaLayer();
}


void Package::FiveTeamMemberSelected(string ucid)
{
    bool haveSelected = false;
    for (int i = 0; i < 5; i++) {
        if (strcmp(m_iGeneral[i].c_str(), ucid.c_str()) == 0) {  // 去除已选中标记
            haveSelected = true;
            m_iGeneral[i] = "";
            break;
        }
    }
    if (!haveSelected) { // 添加已选中标记
        for (int i = 0; i < 5; i++) {
            if (strcmp(m_iGeneral[i].c_str(), "") == 0) {
                m_iGeneral[i] = ucid;
                break;
            }
        }
        int num = 0;
        for (int i = 0; i < 5; i++) {
            if (strcmp(m_iGeneral[i].c_str(), "") != 0) {
                num++;
            }
        }
        if (num == 5) { // 已经选满5个 -> 选中的cell正常显示，没有选中的变灰
            for(int i = 0; i < m_usrCardNum; i++){
                CCNode* node = getChildByTag(kCardTagBase + i);
                PackCard* card = (PackCard*) node;
                
                bool isDone = false;
                for (int i = 0; i < 5; i++) {
                    if (strcmp(m_iGeneral[i].c_str(), card->getUcid()) == 0) {
                        isDone = true;
                        card->setNumber(card->getIndex(), false);
                        break;
                    }
                }
                if (!isDone) {
                    card->setNumber(-1, true);
                }
            }
        }
    } else {
        int num = 0;
        for (int i = 0; i < 5; i++) {
            if (strcmp(m_iGeneral[i].c_str(), "") != 0) {
                num++;
            }
        }
        if (num == 4) { // 由已选满变为没有 -> 选中的cell变灰，没有选中的正常显示
            for(int i = 0; i < m_usrCardNum; i++){
                CCNode* node = getChildByTag(kCardTagBase + i);
                PackCard* card = (PackCard*) node;
                
                bool isDone = false;
                for (int i = 0; i < 5; i++) {
                    if (strcmp(m_iGeneral[i].c_str(), card->getUcid()) == 0) {
                        isDone = true;
                        card->setNumber(card->getIndex(), true);
                        break;
                    }
                }
                if (!isDone) {
                    card->setNumber(-1, false);
                }
            }
        }
    }
    refreshFiveTeamMember();
    checkCostEnough();
}

void Package::checkCostEnough()
{
    int leftCost = m_iTotalCost - m_iCurCost;
    for(int i = 0; i < m_usrCardNum; i++){
        CCNode* node = getChildByTag(kCardTagBase + i);
        PackCard* card = (PackCard*) node;
        
        if (card->getIndex() == -1) {
            if (card->getCost() > leftCost) {
                card->setCostEnough(false);
                card->setGray(true);
            } else {
                card->setCostEnough(true);
                int num = 0;
                for (int i = 0; i < 5; i++) {
                    if (strcmp(m_iGeneral[i].c_str(), "") != 0) {
                        num++;
                    }
                }
                if (num < 5) {
                    card->setGray(false);
                }
                
            }
        } else {
            card->setCostEnough(true);
        }
    }
}


void Package::refreshFiveTeamMember()
{
    char buf[100];
    
    if (FiveTeam1) {
        FiveTeamLayer->removeChild(FiveTeam1, true);
    }
    if (m_iGeneral[0] != "") {
        CUserCardInfo* pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(m_iGeneral[0].c_str());
        snprintf(buf, 99, "%i_head.png", pUsrCardInfo->cid);
        FiveTeam1 = CCSprite::spriteWithFile(buf);
    } else {
        FiveTeam1 = CCSprite::spriteWithSpriteFrameName("fr_box.png");
    }
    FiveTeam1->setAnchorPoint(CCPointZero);
    FiveTeam1->setScale(0.7);
    FiveTeam1->setPosition(CCPointMake(128 + 77*0, 14));
    FiveTeamLayer->addChild(FiveTeam1, 10);
    
    if (FiveTeam2) {
        FiveTeamLayer->removeChild(FiveTeam2, true);
    }
    if (m_iGeneral[1] != "") {
        CUserCardInfo* pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(m_iGeneral[1].c_str());
        snprintf(buf, 99, "%i_head.png", pUsrCardInfo->cid);
        FiveTeam2 = CCSprite::spriteWithFile(buf);
    } else {
        FiveTeam2 = CCSprite::spriteWithSpriteFrameName("fr_box.png");
    }
    FiveTeam2->setAnchorPoint(CCPointZero);
    FiveTeam2->setScale(0.7);
    FiveTeam2->setPosition(CCPointMake(128 + 77*1, 14));
    FiveTeamLayer->addChild(FiveTeam2, 10);
    
    if (FiveTeam3) {
        FiveTeamLayer->removeChild(FiveTeam3, true);
    }
    if (m_iGeneral[2] != "") {
        CUserCardInfo* pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(m_iGeneral[2].c_str());
        snprintf(buf, 99, "%i_head.png", pUsrCardInfo->cid);
        FiveTeam3 = CCSprite::spriteWithFile(buf);
    } else {
        FiveTeam3 = CCSprite::spriteWithSpriteFrameName("fr_box.png");
    }
    FiveTeam3->setAnchorPoint(CCPointZero);
    FiveTeam3->setScale(0.7);
    FiveTeam3->setPosition(CCPointMake(128 + 77*2, 14));
    FiveTeamLayer->addChild(FiveTeam3, 10);
    
    if (FiveTeam4) {
        FiveTeamLayer->removeChild(FiveTeam4, true);
    }
    if (m_iGeneral[3] != "") {
        CUserCardInfo* pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(m_iGeneral[3].c_str());
        snprintf(buf, 99, "%i_head.png", pUsrCardInfo->cid);
        FiveTeam4 = CCSprite::spriteWithFile(buf);
    } else {
        FiveTeam4 = CCSprite::spriteWithSpriteFrameName("fr_box.png");
    }
    FiveTeam4->setAnchorPoint(CCPointZero);
    FiveTeam4->setScale(0.7);
    FiveTeam4->setPosition(CCPointMake(128 + 77*3, 14));
    FiveTeamLayer->addChild(FiveTeam4, 10);
    
    if (FiveTeam5) {
        FiveTeamLayer->removeChild(FiveTeam5, true);
    }
    if (m_iGeneral[4] != "") {
        CUserCardInfo* pUsrCardInfo = CGameData::Inst()->getUsrCardInfByUcid(m_iGeneral[4].c_str());
        snprintf(buf, 99, "%i_head.png", pUsrCardInfo->cid);
        FiveTeam5 = CCSprite::spriteWithFile(buf);
    } else {
        FiveTeam5 = CCSprite::spriteWithSpriteFrameName("fr_box.png");
    }
    FiveTeam5->setAnchorPoint(CCPointZero);
    FiveTeam5->setScale(0.7);
    FiveTeam5->setPosition(CCPointMake(128 + 77*4, 14));
    FiveTeamLayer->addChild(FiveTeam5, 10);
}

void Package::cbFiveTeamLayerBG(CCObject* sender)
{
    printf("bg");
}

void Package::cbFiveTeamLayerReset(CCObject* sender)
{
    resetIGeneral();
    for(int i = 0; i < m_usrCardNum; i++){
        CCNode* node = getChildByTag(kCardTagBase + i);
        PackCard* card = (PackCard*) node;
        card->setNumber(-1, false);
    }
    refreshFiveTeamMember();
}

void Package::cbFiveTeamLayerSure(CCObject* sender)
{
    if (strcmp(m_iGeneral[0].c_str(), "") == 0) {
        // 弹框提示 没有选择 主将
        if (m_noMainGeneralDlg == NULL)
        {
            m_noMainGeneralDlg = new Dialog1(this,
                                     callfuncO_selector(Package::cbMainGeneralDlg),
                                     CGameData::Inst()->getLanguageValue("teamInf_reset_alert"),
                                     false);
            addChild(m_noMainGeneralDlg, 100);
            m_noMainGeneralDlg->release();
        }
        
        m_noMainGeneralDlg->setPosition(CCPointMake(320, 480 - getPosition().y));
        m_noMainGeneralDlg->setScale(0.0);
        m_noMainGeneralDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));

    } else {
        if (m_iLookTp == enPkLkTp_TeamReset) {
            for (int i = 0; i < 5; i++) {
                if (strcmp(m_iGeneral[i].c_str(), "") != 0) {
                    strncpy(m_ret.uid[i].uid, m_iGeneral[i].c_str(), 99);
                }
            }
            (m_pListener->*m_pfnSelector)(&m_ret);
        }
    }
}

void Package::cbMainGeneralDlg()
{
    if (m_noMainGeneralDlg) {
        m_noMainGeneralDlg->removeFromParentAndCleanup(true);
        m_noMainGeneralDlg = NULL;
    }
}

void Package::resetIGeneral()
{
    for (int i = 0; i < kHelperMaxCount; i++)
    {
        m_iGeneral[i] = "";
    }
}


//长按显示详细信息
void Package::waitShowInfo()
{
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    CCPoint touchLocation = beganPoint;
    CCPoint pos = convertToNodeSpace( touchLocation );
    if(sz.width < 1000)
        touchLocation = ccpMult(touchLocation, CC_CONTENT_SCALE_FACTOR());
    
    //ipad, iphone5
    touchLocation.x -= MainScene::Inst()->getPosition().x;
    touchLocation.y -= MainScene::Inst()->getPosition().y;
    
    if(checkBtn(touchLocation))
        return;
    
    int y = (m_ptStart.y - pos.y) / (m_iGap + m_cardSize.height);
    int x = (pos.x - m_ptStart.x) / (m_iGap + m_cardSize.width);
    
    if((m_ptStart.y - pos.y) > (y * (m_iGap + m_cardSize.height) + m_cardSize.height))
        return;
    
    if((pos.x - m_ptStart.x) < (x * (m_iGap + m_cardSize.width) + m_iGap))
        return;
    
    int iSel = y * m_iColumn + x;
    
    
    
    if(MainScene::Inst()->new_FirstPowerUpSeeYuXi)
    {
        if(iSel != 2)
            return;
        else
        {
            MainScene::Inst()->removeTouchTip();
        }
    }
    
    
    if(!MainScene::Inst()->new_FirstPowerUp && !MainScene::Inst()->new_FirstZhuanSheng && !MainScene::Inst()->new_CardAdd )
    isLongPress = true;
    if(m_iLookTp == enPkLkTp_TeamEditSub){
        iSel--;
    }
    
    if((iSel >= 0) && (iSel < m_usrCardNum)){
        CCNode* node = getChildByTag(kCardTagBase + iSel);
        PackCard* card = (PackCard*) node;
        m_pUsrCardSel = card->getUserCardInfo();
        
        
        if (m_pScrollViewLayer && !CGameData::Inst()->isNewbie())
        {
            if (!m_pScrollViewLayer->getIsVisible())
            {
                m_pScrollViewLayer->setPosition(CCPointMake(0, -this->getPosition().y));
                m_pScrollViewLayer->show(m_pUsrCardSel);
            }
        }
        else
        {
            if(m_showCard->getIsVisible() == false){
                
                m_showCard->setPosition(CCPointMake(0, -this->getPosition().y));
                m_showCard->show(m_pUsrCardSel);
            }
        }
    }
    unschedule(schedule_selector(Package::waitShowInfo));
}



void Package::newbie(CCObject* pObj)
{
    CCNode* newbieEndDlg1 = getChildByTag(kNewbie);
    if(newbieEndDlg1)
        removeChild(newbieEndDlg1, true);
    
    
    Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(kNewbie + 99999);
    if(temp)
        temp->removeFromParentAndCleanup(true);
    
    
    
    
    Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(Package::newbie2),
                                        CGameData::Inst()->getLanguageValue("zhuanSheng_new16"),false);
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



void Package::newbie4(CCObject* pObj)
{
    CCNode* newbieEndDlg1 = getChildByTag(kNewbie);
    if(newbieEndDlg1)
        removeChild(newbieEndDlg1, true);
    
    
    Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(kNewbie + 99999);
    if(temp)
        temp->removeFromParentAndCleanup(true);
    
    
    
    
    Arrow* pArrow = new Arrow(30, 255);
    pArrow->setPosition(CCPointMake(20 + 54, 720 - 100));
    
    
    pArrow->setRotation(90);
    addChild(pArrow, 9, kPackArrowTag);
    pArrow->release();
    
    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                       NULL);
    pArrow->runAction(CCRepeatForever::actionWithAction(action));

    
}


void Package::newbie2(CCObject* pObj)
{
    CCNode* newbieEndDlg1 = getChildByTag(kNewbie);
    if(newbieEndDlg1)
        removeChild(newbieEndDlg1, true);
    
    
    Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(kNewbie + 99999);
    if(temp)
        temp->removeFromParentAndCleanup(true);
    
    
    
    
    
    Arrow* pArrow1 = new Arrow(30, 255);
    pArrow1->setPosition(CCPointMake(16 + 54, 720 - 100));
    
    
    pArrow1->setRotation(90);
    addChild(pArrow1, 9, kPackArrowTag);
    pArrow1->release();
    
    CCActionInterval* action1 = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                        CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                        NULL);
    pArrow1->runAction(CCRepeatForever::actionWithAction(action1));
    
    
    
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool Package::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    
    m_bTouchMvFlg = false;
    unschedule(schedule_selector(Package::inertiaMove));

    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    beganPoint = touchLocation;
    m_ptStartTouch = convertToNodeSpace( touchLocation );
    last_moveTouch = touchLocation;
    m_lastXm = 0;
    
    CCSize sz = CCDirector::sharedDirector()->getWinSizeInPixels();
    //this->schedule(menu_selector(Package::waitShowInfo), 1.0);
    
    if(!MainScene::Inst()->new_FirstPowerUp && !MainScene::Inst()->new_FirstZhuanSheng && !MainScene::Inst()->new_CardAdd )
    schedule(schedule_selector(Package::waitShowInfo), 0.3);
    
    
    if(MainScene::Inst()->new_FirstPowerUpSeeYuXi)
    {
        Package::waitShowInfo();
        
        return false;
    }
    
    
    if(sz.width == 320)
        touchLocation = ccpMult(touchLocation, 2);
    if(sz.width < 1000)
        touchLocation = ccpMult(touchLocation, CC_CONTENT_SCALE_FACTOR());
    
    //ipad, iphone5
    int offsetY = MainScene::Inst()->getPosition().y;
    
    if((touchLocation.y > (m_ptStart.y + offsetY)) || (touchLocation.y < (160 + offsetY)))
    {
        unschedule(schedule_selector(Package::waitShowInfo));
        return false;
    }
        
    else
        return true;
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void Package::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    
    unschedule(schedule_selector(Package::waitShowInfo));
    if(isLongPress == true)
    {
        //isLongPress = false;
        return;
    }
    m_bTouchMvFlg = true;
    
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
    
    if(!MainScene::Inst()->new_FirstPowerUp && !MainScene::Inst()->new_FirstZhuanSheng && !MainScene::Inst()->new_CardAdd )
    setThisPosition(thisPt);
    
    if(m_fUpLimitY > 0){
        float fPercent = thisPt.y / m_fUpLimitY;
    
        m_pScrollBar->setPercent(fPercent);

        if((enPkLkTp_PwUpFood == m_iLookTp)
           || (enPkLkTp_NiePanFood == m_iLookTp)
           || (enPkLkTp_Sell == m_iLookTp)
           || (enPkLkTp_TeamReset == m_iLookTp))
            m_pScrollBar->setPosition(CCPointMake(630, 482 - thisPt.y));
        else
            m_pScrollBar->setPosition(CCPointMake(630, 452 - thisPt.y));
        
        redrawCapacity();
    }
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void Package::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    unschedule(schedule_selector(Package::waitShowInfo));
    if(isLongPress == true && !MainScene::Inst()->new_FirstPowerUp  && !MainScene::Inst()->new_CardAdd && !MainScene::Inst()->new_FirstZhuanSheng)
    {
        isLongPress = false;
        return;
    }
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    
    //float fMovedAddX = 11.0f, fMovedDelX = -9.f;
    if(m_bTouchMvFlg == true && ccpDistance(touchLocation, beganPoint) > kListBoxMinMovY)
    {
        //printf("\t\tm_lastYm = %f\n",m_lastYm);
  //      float fabsY = fabs(m_lastYm);
//        if ((0<m_lastYm && m_lastYm<m_lastXm) || (0>m_lastYm && m_lastXm > m_lastYm))
//        {
//        
//        }
        if (kListBoxMinMovY <= fabs(m_lastYm) && fabs(m_lastXm)<=fabs(m_lastYm))
        {
            if(fabs(m_lastYm) >100)
                m_lastYm = m_lastYm/fabs(m_lastYm) *100;
            
            if(!MainScene::Inst()->new_FirstPowerUp  && !MainScene::Inst()->new_CardAdd && !MainScene::Inst()->new_FirstZhuanSheng)
            schedule(schedule_selector(Package::inertiaMove));
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

    if(checkBtn(touchLocation))
    {
        return;
    }
    
    int y = (m_ptStart.y - pos.y) / (m_iGap + m_cardSize.height);
    int x = pos.x / (m_iGap + m_cardSize.width);
    
    //if (enCardRangeTypeProp==((enCardRangeType)testCount)&&14.0f<=fabsf(m_lastXm))
    if (enCardRangeTypeProp==((enCardRangeType)testCount)&&(11.0f<=m_lastXm || -9.0f>=m_lastXm) )
    {
        if (CCNode* pNode = getChildByTag(kCardTagBase + y * m_iColumn + x))
        {
            if (0==m_lastYm || (0.0f<fabs(m_lastYm) && 2.0f<fabs((m_lastXm/m_lastYm))))
            {
                thisPositionX((PackCard*)pNode,0.0f<m_lastXm);
            }
            m_lastXm= 0.0f;
            return;
        }
    }
    
    if((m_ptStart.y - pos.y) > (y * (m_iGap + m_cardSize.height) + m_cardSize.height))
        return;
    
    if((pos.x - m_ptStart.x) < (x * (m_iGap + m_cardSize.width) + m_iGap))
        return;
    
    int iSel = y * m_iColumn + x;
    
    if(m_iLookTp == enPkLkTp_TeamEditSub){
        if((iSel == 0) && (CGameData::Inst()->isNewbie() == false)){
            m_ret.uid[0].uid[0] = 0;
            (m_pListener->*m_pfnSelector)(&m_ret);
        }

        iSel--;
    }
    
    if((iSel >= 0) && (iSel < m_usrCardNum)){
        CCNode* node = getChildByTag(kCardTagBase + iSel);
        if (node == NULL)
            return;
        
        PackCard* card = (PackCard*) node;        
        m_pUsrCardSel = card->getUserCardInfo();
        
//        CCLog("%d" , m_pUsrCardSel->cid  );

        
        if(CGameData::Inst()->isNewbie()){
            if(((m_iLookTp == enPkLkTp_TeamEditSub) || (m_iLookTp == enPkLkTp_PwUpBase))
               && (m_pUsrCardSel->cid != 49)){
                
                
                
                return;
            }
            
            if((m_iLookTp == enPkLkTp_PwUpFood) && (m_pUsrCardSel->cid != 86)){
                return;
            }
        }
        
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
        
        if(m_iLookTp == enPkLkTp_PwUpFood){
            CCNode* arrow = getChildByTag(kPackArrowTag);
   
            if((card != NULL) && (card->isGray() == false)){
                if(card->isSelected()){
                    card->setSel(false);
                    m_iSelCnt--;
                    if(m_packSelBtn != NULL){
                        if(m_iBaseCardCType == card->getcType())
                            m_packSelBtn->rmv1cardUp(card->getEatExp() * 1.5, card->getPlus());
                        else
                            m_packSelBtn->rmv1cardUp(card->getEatExp(), card->getPlus());
                    }
                    
                    if (m_iSelCnt == 4) {
                        setAllCardGray(false);
                    }
                    
                    if(arrow){
                        CCPoint pt = card->getPosition();
                        arrow->stopAllActions();
                        arrow->setPosition(CCPointMake(pt.x + 54, pt.y - 100));
                        arrow->setRotation(90);
                        
                        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                                           NULL);
                        arrow->runAction(CCRepeatForever::actionWithAction(action));
                    }
                }
                else{
                    
                    if(MainScene::Inst()->new_FirstPowerUp && iSel != 2 && !MainScene::Inst()->new_FirstPowerUpSecond)
                    {

                        return;
                    }
                    
                    if(MainScene::Inst()->new_FirstPowerUp && iSel != 1 && MainScene::Inst()->new_FirstPowerUpSecond)
                    {
                        
                        return;
                    }
                    
                    
                    if(m_iSelCnt < 5){
                        card->setSel(true);
            
                        //yang--
                        if (arrowpoint.x==-1000) {
                            arrowpoint=card->getPosition();
                        }

                        m_iSelCnt++;
                        if(m_packSelBtn != NULL){
                            if(m_iBaseCardCType == card->getcType())
                                m_packSelBtn->add1cardUp(card->getEatExp() * 1.5, card->getPlus());
                            else
                                m_packSelBtn->add1cardUp(card->getEatExp(), card->getPlus());
                        }
                        
                        if (m_iSelCnt == 5) {
                            setAllCardGray(true);
                        }
                        
                        if(arrow){
                            arrow->stopAllActions();
                            arrow->setPosition(CCPointMake(540, 238));
                            arrow->setRotation(-90);
                            
                            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                                               CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                                               NULL);
                            arrow->runAction(CCRepeatForever::actionWithAction(action));
                        }
                    }
                }
            }
        }
        else if (m_iLookTp == enPkLkTp_NiePanFood)
        {
            char* ucidBase = CGameData::Inst()->getCombineCard(0);
            CUserCardInfo* pBaseCard = CGameData::Inst()->getUsrCardInfByUcid(ucidBase);
            int maxCount = pBaseCard->np_need_num;
            
            if((card != NULL) && (card->isGray() == false)){
                if(card->isSelected()){
                    card->setSel(false);
//                    setCardGrayByCid(card->getCardId(), false);
                    
                    m_iSelCnt--;
                    if(m_packSelBtn != NULL){
                        if(m_iBaseCardCType == card->getcType())
                            m_packSelBtn->rmv1cardUp(card->getEatExp() * 1.5, card->getPlus());
                        else
                            m_packSelBtn->rmv1cardUp(card->getEatExp(), card->getPlus());
                    }
                    
                    if (m_iSelCnt == maxCount - 1) {
                        setAllCardGray(false);
                    }                    
                }
                else{
                    if(m_iSelCnt < maxCount){
                        card->setSel(true);
//                        setCardGrayByCid(card->getCardId(), true);
                        
                        m_iSelCnt++;
                        if(m_packSelBtn != NULL){
                            if(m_iBaseCardCType == card->getcType())
                                m_packSelBtn->add1cardUp(card->getEatExp() * 1.5, card->getPlus());
                            else
                                m_packSelBtn->add1cardUp(card->getEatExp(), card->getPlus());
                        }
                        
                        if (m_iSelCnt == maxCount) {
                            setAllCardGray(true);
                        }                        
                    }
                }
            }
        }
        else if(m_iLookTp == enPkLkTp_Sell){
            if((card != NULL) && (card->isGray() == false)){
                if(card->isSelected())
                {
                    card->setSel(false);
                    m_iSelCnt--;
                    if(m_packSelBtn != NULL){
                        m_packSelBtn->rmv1cardSell(card->getSellGold());
                    }
                    if (m_iSelCnt == 9) {
                        setAllCardGray(false);
                    }
                }
                else{
                    if(m_iSelCnt < 10){
                        card->setSel(true);
                        m_iSelCnt++;
                        if(m_packSelBtn != NULL){
                            m_packSelBtn->add1cardSell(card->getSellGold());
                        }
                        if (m_iSelCnt == 10) {
                            setAllCardGray(true);
                        }
                    }
                }
            }
        }
        else if(m_iLookTp == enPkLkTp_TeamReset){
            if ((card->isGray() && card->getIndex() != -1) || !(card->isGray())) {
                CUserCardInfo* pUsrCardInf = CGameData::Inst()->getUsrCardInfByUcid(card->getUcid());
                if(pUsrCardInf != NULL) {
                    int haveSelected = false;
                    for (int i = 0; i < 5; i++) {
                        if (strcmp(m_iGeneral[i].c_str(), pUsrCardInf->ucid) == 0) {
                            haveSelected = true;
                            break;
                        }
                    }
                    if (haveSelected) {
                        if (!card->isSelected() && !card->isUsed() && !card->isPowUp()) {
                            card->setNumber(-1, false);
                            m_iCurCost -= card->getCost();
                            FiveTeamMemberSelected(card->getUcid());
                        }
                    } else {
                        int index = -1;
                        for (int i = 0; i < 5; i++) {
                            if (strcmp("", m_iGeneral[i].c_str()) == 0) {
                                index = i;
                                break;
                            }
                        }
                        if (!card->isSelected() && !card->isUsed() && !card->isPowUp()) {
                            card->setNumber(index, true);
                            m_iCurCost += card->getCost();
                            FiveTeamMemberSelected(card->getUcid());
                        }
                    }
                }
            }
        }
        else{
            m_iSelNo = iSel;
            
            
            if(MainScene::Inst()->new_FirstZhuanSheng)
            {
                if(iSel != 0)
                    return;
                else
                {
                    CCNode* arrow = getChildByTag(kPackArrowTag);
                    
                    arrow->removeFromParentAndCleanup(true);
                    
                    
                    
                    
                    
                    
                    Arrow* pArrow = new Arrow(30, 255);
                    
                    pArrow->setPosition(CCPointMake(231, 425));
                    
                    
                    
                    pArrow->setRotation(180);
                    //    pArrow->setScale(0.6f);
                    
                    MainScene::Inst()->addChild(pArrow, 50000, 97999);
                    
                    
                    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                                       NULL);
                    pArrow->runAction(CCRepeatForever::actionWithAction(action));
                }
            }
            if(MainScene::Inst()->new_CardAdd )
            {
                if(iSel != m_NewCardID)
                    return;
                else
                {
                    CCNode* arrow = getChildByTag(kPackArrowTag);
                    
                    arrow->removeFromParentAndCleanup(true);
                    
                    
                    
                    
                    
                    
                    Arrow* pArrow = new Arrow(30, 255);
                    
                    pArrow->setPosition(CCPointMake(231, 425 - 100));
                    
                    
                    
                    pArrow->setRotation(180);
                    //    pArrow->setScale(0.6f);
                    
                    MainScene::Inst()->addChild(pArrow, 50000, 97999);
                    
                    
                    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                                       NULL);
                    pArrow->runAction(CCRepeatForever::actionWithAction(action));

                }
            }
            
            if(MainScene::Inst()->new_FirstPowerUp)
            {
                if(iSel != 0)
                    return;
                else
                {
                    
                    MainScene::Inst()->new_FirstPowerUpSeeYuXi = true;
                    
                    
                    CCNode* arrow = getChildByTag(kPackArrowTag);
                    
                    arrow->removeFromParentAndCleanup(true);
                    
                    
                    
                    
                    
                    
                    Arrow* pArrow = new Arrow(30, 255);
                    
                    pArrow->setPosition(CCPointMake(231, 425));
                    
                    
                    
                    pArrow->setRotation(180);
                    //    pArrow->setScale(0.6f);
                    
                    MainScene::Inst()->addChild(pArrow, 50000, 97999);
                    
                    
                    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                                       NULL);
                    pArrow->runAction(CCRepeatForever::actionWithAction(action));
                    
                }
                
                

            }
            
            if(((card != NULL) && (card->isGray()))
               || (m_cfmLayer == NULL)){
               
                if (m_pScrollViewLayer)
                {
                    if (!m_pScrollViewLayer->getIsVisible())
                    {
                        m_pScrollViewLayer->setPosition(CCPointMake(0, -this->getPosition().y));
                        m_pScrollViewLayer->show(m_pUsrCardSel);
//                        MainScene::Inst()->showBottomPanel(false);
                    }
                }
                else if(m_showCard->getIsVisible() == false){
                    
                    m_showCard->setPosition(CCPointMake(0, -this->getPosition().y));
                    m_showCard->show(m_pUsrCardSel);
                    
                    MainScene::Inst()->showBottomPanel(false);
                }
            }
            else{
                m_bCmfFourBtn = false;
                if (CCNode *pNode = getChildByTag(lcCoverChild))
                {
                    pNode->setIsVisible(true);
                }
                else
                {
                    
                    m_lcCover = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 196), 640, 960);
                    addChild(m_lcCover, 9,lcCoverChild);
                }
                
                m_lcCover->setPosition(CCPointMake(0, -this->getPosition().y));
                m_cfmLayer->setPosition(CCPointMake(0, -this->getPosition().y));
                
                ccTouchEndedLookTpBtnSel();
            }
        }
    }
}

void Package::moveY(float dt)
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
        
        if((enPkLkTp_PwUpFood == m_iLookTp)
           || (enPkLkTp_NiePanFood == m_iLookTp)
           || (enPkLkTp_Sell == m_iLookTp)
           || (enPkLkTp_TeamReset == m_iLookTp))
            m_pScrollBar->setPosition(CCPointMake(630, 482 - thisPt.y));
        else
            m_pScrollBar->setPosition(CCPointMake(630, 452 - thisPt.y));
        
        redrawCapacity();
    }
}

void Package::inertiaMove(float dt) {
    
    
    
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
        
        unschedule(schedule_selector(Package::inertiaMove));
        // m_scrollBar->setIsVisible(false);
    }
}
//******************************************************************************
// onExit
//******************************************************************************
bool Package::goback()
{
    if(m_commDlg != NULL){
        return false;
    }
    if (m_pScrollViewLayer && m_pScrollViewLayer->getIsVisible())
    {
//        PackCard *pStartPackCard = m_pScrollViewLayer->getStartPackCard();
        m_pScrollViewLayer->hide();
        if (getChildByTag(cfmLayerChild) && m_cfmLayer && m_cfmLayer->getIsVisible())
        {
            m_cfmLayer->hide();
            if (PackCard *pCurPackCard = m_pScrollViewLayer->getCurPackCard())
            {
                if (CUserCardInfo *pCurUserCardInfo = pCurPackCard->getUserCardInfo())
                {
                    if (!pCurPackCard->isGray())
                    {
//                        if (pStartPackCard && pCurPackCard && pStartPackCard!=pCurPackCard)
//                        {
//                            setThisPosition(ccp(getPosition().x,(m_cardSize.height+m_iGap)*((pStartPackCard->getTag()-kCardTagBase)/m_iColumn)), true);
//                        }
                        
                        if (CCNode *pNode = getChildByTag(lcCoverChild))
                        {
                            pNode->setIsVisible(true);
                        }
                        else
                        {
                            m_lcCover = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 196), 640, 960);
                            addChild(m_lcCover, 9,lcCoverChild);
                            m_lcCover->setPosition(CCPointMake(0, -this->getPosition().y));
                        }
                        m_bCmfFourBtn = false;
                        m_iSelNo = pCurPackCard->getTag()-kCardTagBase;
                        m_pUsrCardSel = pCurUserCardInfo;
                        //m_cfmLayer->show(m_pUsrCardSel, NULL, enCConfirmLayer_LOCK);
                        m_cfmLayer->setPosition(CCPointMake(0, -this->getPosition().y));
                        //MainScene::Inst()->setGenRangeBtnIsVisble(false);
                        ccTouchEndedLookTpBtnSel();
                        
                    }
                    else
                    {
                        if (CCNode *pNode = getChildByTag(lcCoverChild))
                        {
                            pNode->setIsVisible(false);
                            m_cfmLayer->setIsVisible(false);
                        }
                    }
                    
                }
            }
        }
        return false;
    }
    else if(m_showCard->getIsVisible() == true){
        m_showCard->hide();
        return false;
    }
    else if((m_cfmLayer) && (m_cfmLayer->getIsVisible() == true)){
        MainScene::Inst()->setGenRangeBtnIsVisble(true);
        m_cfmLayer->hide();
        removeChild(m_lcCover, true);
        return false;
    }
    else {
        if(m_iLookTp == enPkLkTp_PwUpFood
           || m_iLookTp == enPkLkTp_NiePanFood) {
            int idx = 1;
            for(int i = 0; i < m_usrCardNum; i++){
                CCNode* node = getChildByTag(kCardTagBase + i);
                PackCard* card = (PackCard*) node;
                if((card != NULL) && (card->isSelected())){
                    CGameData::Inst()->setCombineQueue(idx, card->getUcid());
                    idx++;
                }
            }
            
            for (; idx < MaxCombineCnt; idx++) {
                CGameData::Inst()->setCombineQueue(idx, 0);
            }
        }
        return true;
    }
}

//******************************************************************************
// cbCfm
//******************************************************************************
void Package::cbCfm(CCObject* pObj)
{
    CCfmLayerRet* pRet = (CCfmLayerRet*) pObj;
    
    
    if(MainScene::Inst()->new_FirstPowerUp && MainScene::Inst()->getChildByTag(97999))
    {
        if(pRet->iBtnSel != 0)
            return;
        else
        {
            MainScene::Inst()->removeChildByTag(97999, true);
        }
    }
    
    if(MainScene::Inst()->new_CardAdd && MainScene::Inst()->getChildByTag(97999))
    {
        if(pRet->iBtnSel != 1)
            return;
        else
        {
            MainScene::Inst()->removeChildByTag(97999, true);
        }
    }
    
    
    if(MainScene::Inst()->new_FirstZhuanSheng && MainScene::Inst()->getChildByTag(97999))
    {
        if(pRet->iBtnSel != 0)
            return;
        else
        {
            MainScene::Inst()->removeChildByTag(97999, true);
        }
    }
    
    MainScene::Inst()->setGenRangeBtnIsVisble(true);
    m_ret.bNiePan = false;
    m_ret.type = 0;
    
    
    

    

    if (m_bCmfFourBtn)
    {
        switch (pRet->iBtnSel) {
            case 0:
                m_cfmLayer->hide();
                removeChild(m_lcCover, true);
                
                if (pRet->iConfirmType == enCConfirmLayer_nEVO_Wake) {
                    m_ret.type = 1;
                }
                
                strncpy(m_ret.uid[0].uid, m_pUsrCardSel->ucid, 99);
                (m_pListener->*m_pfnSelector)(&m_ret);
                break;
                
            case 1:
                m_cfmLayer->hide();
                removeChild(m_lcCover, true);
                
                if (pRet->iConfirmType == enCConfirmLayer_nEVO_Wake)
                {
                    strncpy(m_ret.uid[0].uid, m_pUsrCardSel->ucid, 99);
                    (m_pListener->*m_pfnSelector)(&m_ret);
                }
                else
                {
                    showDlgNiePan();
                }
                break;
                
            case 2:
                m_showCard->setPosition(ccpSub(CCPointZero, this->getPosition()));
                m_showCard->show(m_pUsrCardSel);
                m_showCard->setForbShowRangeBtn(true);
                MainScene::Inst()->showBottomPanel(false);
                break;
                
            case 3:
            default:
                m_cfmLayer->hide();
                removeChild(m_lcCover, true);
                break;
                
        }
    }
    else
    {
        switch (pRet->iBtnSel) {
            case 0:
            {
                m_cfmLayer->hide();
                removeChild(m_lcCover, true);
                
                if (m_iLookTp == enPkLkTp_JustLook) {
                    CCNode* node = getChildByTag(kCardTagBase + m_iSelNo);
                    PackCard* card = (PackCard*) node;
                    
                    if (CGameData::Inst()->getIsLock(m_pUsrCardSel->ucid)) {
                        card->setIsLockCard(false);
                    }
                    else {
                        card->setIsLockCard(true);
                    }
                    if (CGameData::Inst()->getDefaultRangeType() == enCardRangeTypeLock) {
                        rangePackageCardWithType(enCardRangeTypeLock);
                    }
                }
                else if (m_iLookTp == enPkLkTp_PwUpBase
                         && m_pUsrCardSel->bIsNPTgt)
                    showDlgNiePan();
                else
                {
//                    CUserCardInfo* pUsrCardInf = NULL;
//                    CCMutableArray<CUserCardInfo*> *  pCardsArr = CGameData::Inst()->getUserCards();
//                    
//                    for (int i = 0; i < pCardsArr->count(); i++)
//                    {
//                        pUsrCardInf = pCardsArr->getObjectAtIndex(i);
//                        
//                        if (strncmp(m_pUsrCardSel->ucid, pUsrCardInf->ucid, 99) == 0)
//                            break;
//                        else
//                            pUsrCardInf = NULL;
//                    }
//                    
//                    if(pUsrCardInf == NULL)
//                        return;
                    
                    if (m_pUsrCardSel->elu_flg > 0) m_ret.type = 2;
                    if (m_pUsrCardSel->wk_flg > 0) m_ret.type = 1;
                    
                     strncpy(m_ret.uid[0].uid, m_pUsrCardSel->ucid, 99);
                    (m_pListener->*m_pfnSelector)(&m_ret);
                }
            }
                break;
                
            case 1:
            {
                if (m_pScrollViewLayer && !CGameData::Inst()->isNewbie())
                {
                    m_pScrollViewLayer->setPosition(ccpSub(CCPointZero, this->getPosition()));
                    
//                    struct timeval tv;
//                    gettimeofday(&tv,NULL);
//                    
//                    CCLog("%ld **************** %ld" , tv.tv_sec , tv.tv_usec);
                    
                    
                    m_pScrollViewLayer->show(m_pUsrCardSel);
                    
                    
//                    struct timeval tv1;
//                    gettimeofday(&tv1,NULL);
//                    
//                    CCLog("%ld +++++++++ %ld" , tv1.tv_sec , tv1.tv_usec);
                    
                    
                    MainScene::Inst()->showBottomPanel(false);
                }
                else
                {
                    m_showCard->setPosition(ccpSub(CCPointZero, this->getPosition()));
                    m_showCard->show(m_pUsrCardSel);
                    m_showCard->setForbShowRangeBtn(true);
                    MainScene::Inst()->showBottomPanel(false);
                }
                break;
            }
                
            case 2:
            default:
                m_cfmLayer->hide();
                removeChild(m_lcCover, true);
                break;
                
        }
    }
}

//******************************************************************************
// cbSellDlg
//******************************************************************************
void Package::cbSellDlg(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    unschedule(schedule_selector(Package::waitShowInfo));

    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if(pRet->bOk == true){
        if(CGameData::Inst()->sellRequest()){
            m_iReqType = 1;
            scheduleUpdate();
        }
    }
    else {
        
    }
    
    removeChild(m_sellDlg, true);
    m_sellDlg = NULL;
}

//******************************************************************************
// cbSellUpThreeStarsDlg
//******************************************************************************
void Package::cbSellUpThreeStarsDlg(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    unschedule(schedule_selector(Package::waitShowInfo));

    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if(pRet->bOk == true){
        showDlgIsSell();
    }
    else {
        
    }
    
    removeChild(m_sellUpThreeStarsDlg, true);
    m_sellUpThreeStarsDlg = NULL;
}

//******************************************************************************
// cbCommDlg
//******************************************************************************
void Package::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    unschedule(schedule_selector(Package::waitShowInfo));
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }

    if(isOk == true){
        if(m_iReqType == 1){
            if(CGameData::Inst()->sellRequest()){
                scheduleUpdate();
            }
        }
        else if(m_iReqType == 2){
            if(CGameData::Inst()->allCardRequest()){
                scheduleUpdate();
            }
        }
    }
    else {
        CGameData::Inst()->clearReqStat();
        MainScene::Inst()->enableBottomPanel(true);
    }
}

//******************************************************************************
// initButton
//******************************************************************************
void Package::initButton()
{
    
}

//******************************************************************************
// initCapacity
//******************************************************************************
void Package::initCapacity(char* buf)
{
    m_spMaxCard = CCSprite::spriteWithSpriteFrameName("fr_another.png");
    m_spMaxCard->setPosition(CCPointMake(535, 167));
    addChild(m_spMaxCard, 10);
    
    m_lbMaxCard = TextNode::textWithString(buf, CCSizeMake(200, 25), CCTextAlignmentCenter, 24);
    m_lbMaxCard->setPosition(m_spMaxCard->getPosition());
    m_lbMaxCard->setColor(ccWHITE);
    m_lbMaxCard->setShadowColor(ccBLACK);
    addChild(m_lbMaxCard, 12);
}

//******************************************************************************
// redrawCapacity
//******************************************************************************
void Package::redrawCapacity()
{
    if(m_spMaxCard == NULL)
        return;
    m_spMaxCard->setPosition(CCPointMake(535, 167 - this->getPosition().y));
    m_lbMaxCard->setPosition(m_spMaxCard->getPosition());
}

//******************************************************************************
// setAllCardGray
//******************************************************************************
void Package::setAllCardGray(bool bGray) {
    for(int i = 0; i < m_usrCardNum; i++){
        CCNode* node = getChildByTag(kCardTagBase + i);
        PackCard* card = (PackCard*) node;
        if (!card->isSelected() && !card->isUsed() && !card->isPowUp()
            && !CGameData::Inst()->getIsLock(card->getUcid())) {
            if (enPkLkTp_NiePanFood == m_iLookTp)
            {
                if (bGray)
                    card->setGray(true);
                else
                {
                    if (!card->getIsGrayByNiePanFood())
                        card->setGray(false);
                }
            }
            else
                card->setGray(bGray);
        }
    }
}

//******************************************************************************
// setCardGrayByCid
//******************************************************************************
void Package::setCardGrayByCid(short cid, bool bGray)
{
    for(int i = 0; i < m_usrCardNum; i++){
        CCNode* node = getChildByTag(kCardTagBase + i);
        PackCard* card = (PackCard*) node;
        
        if (card->getCardId() != cid)
            continue;
        
        if (!card->isSelected() && !card->isUsed() && !card->isPowUp()
            && !CGameData::Inst()->getIsLock(card->getUcid())) {
            card->setIsGrayByNiePanFood(bGray);
        }
    }
}

//******************************************************************************
// checkBtn
//******************************************************************************
bool Package::checkBtn(const CCPoint& pt)
{
//    CCLog("touch postion x: %f, y: %f", pt.x, pt.y);
    if(m_packSelBtn == NULL)
        return false;
    
    bool bRet;
    
    //判断点击是否在按钮所在的panel区
    bRet = CCRect::CCRectContainsPoint(CCRectMake(0, 148, 640, 84), pt);
    
    if(bRet == false)
        return false;
    
    if(m_iSelCnt == 0)
        return true;
    
    //ok
//    bRet = CCRect::CCRectContainsPoint(CCRectMake(453, 160, 188, 80), pt);
    bRet = CCRect::CCRectContainsPoint(CCRectMake(451, 150, 188, 79), pt);
    if(bRet == true)
    {
        int idx = 1;
        for(int i = 0; i < m_usrCardNum; i++){
            CCNode* node = getChildByTag(kCardTagBase + i);
            PackCard* card = (PackCard*) node;
            if((card != NULL) && (card->isSelected()))
            {
                CGameData::Inst()->setCombineQueue(idx, card->getUcid());
                idx++;
            }
        }
        
        for (; idx < MaxCombineCnt; idx++)
        {
            CGameData::Inst()->setCombineQueue(idx, 0);
        }
        
        if(m_iLookTp == enPkLkTp_Sell)
        {
            bool isHasThreeStars = false;
            for (int i = 1; i < (10+1); i++) {
                char *uid = CGameData::Inst()->getCombineCard(i);
                if (uid[0] != 0)
                {
                    CUserCardInfo* pUserCardInfo = CGameData::Inst()->getUsrCardInfByUcid(uid);
                    if (pUserCardInfo->star >= 3)
                    {
                        isHasThreeStars = true;
                        break;
                    }
                }
                
            }
            
            if (isHasThreeStars)
            {
                // 所选材料超过三星
                showDlgSellUpThreeStars();
            }
            else
            {
                showDlgIsSell();
            }
        }
        else {
            (m_pListener->*m_pfnSelector)(&m_ret);
        }
    }
    
    //clear
    if(bRet == false){
        
//        bRet = CCRect::CCRectContainsPoint(CCRectMake(10, 160, 188, 80), pt);
        bRet = CCRect::CCRectContainsPoint(CCRectMake(1, 150, 188, 79), pt);
        if(bRet == true){
            
            if(CGameData::Inst()->isSoundOn)
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
            
            for(int i = 0; i < m_usrCardNum; i++){
                CCNode* node = getChildByTag(kCardTagBase + i);
                PackCard* card = (PackCard*) node;
                if((card != NULL) && (card->isSelected())){
                    card->setSel(false);
//                    if (m_iLookTp == enPkLkTp_NiePanFood)
//                        setCardGrayByCid(card->getCardId(), false);
                }
            }
            for (int i = 1; i < MaxCombineCnt; i++)
            {
                CGameData::Inst()->setCombineQueue(i, 0);
            }
            m_iSelCnt = 0;
            m_packSelBtn->clear();
            setAllCardGray(false);
            
            //yang--
            if (arrowpoint.x!=-1000) {
                CCNode* arrow = getChildByTag(kPackArrowTag);
                
                if(arrow){
                    
                    arrow->stopAllActions();
                    arrow->setPosition(CCPointMake(arrowpoint.x+54, arrowpoint.y-100));
                    arrow->setRotation(90);
                    
                    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                                       NULL);
                    arrow->runAction(CCRepeatForever::actionWithAction(action));
                }
                
                CCLOG("yang---%f-----%f\n",arrowpoint.x,arrowpoint.y);
                
                
            }

        }
    }
    
    return true;
}

//******************************************************************************
// update
//******************************************************************************
void Package::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(Package::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(m_dlgPoint);
            m_commDlg->release();
            //m_commDlg->setScale(0.0);
            //m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
        MainScene::Inst()->enableBottomPanel(true);
    }
    
    unscheduleUpdate();
    
    if(comState == kReqStatOk){
        if(m_iReqType == 1)
        {
            if(CGameData::Inst()->allCardRequest())
            {
                scheduleUpdate();
                m_iReqType = 2;
                
                m_usrCardNum -= m_iSelCnt;
                CUserInfo *pUsrInf = CGameData::Inst()->getUsrInfo();
                char buf[100] = {0};
                snprintf(buf, 99, "%d/%d", m_usrCardNum, pUsrInf->max_card_num);
        
                m_packSelBtn->setCardNum(buf);
                m_packSelBtn->clear();
                
                CGameData::Inst()->clearCombineQueue();
                
                int i = 0, j = 0;
                while (1) {
                    CCNode* node = getChildByTag(kCardTagBase + i);
                    if(node == NULL)
                        break;
                    
                    PackCard* card = (PackCard*) node;
                    
                    if(card->isSelected()){
                        removeChild(card, true);
                        m_packCardArr->removeObject(card);

                    }
                    else{
                        card->setTag(kCardTagBase + j);
                        CCPoint pt;
                        pt.x = m_ptStart.x + (j % m_iColumn) * (m_iGap + m_cardSize.width);
                        pt.y = m_ptStart.y - (j / m_iColumn) * (m_iGap + m_cardSize.height);
                        card->setPosition(pt);
                        j++;
                    }

                    i++;
                }
                if (m_iSelCnt == 10)
                {
                    setAllCardGray(false);
                }
                m_iSelCnt = 0;
                
                updateScreenWithCardCntChg();
            }
        }
        else if(m_iReqType == 2){
        }
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(Package::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(m_dlgPoint);
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}


void Package::changePackgeCardRange()
{
    testCount ++;
    rangePackageCardWithType((enCardRangeType)(testCount%encardRangeTypeMax));
}

//bool _sortCategory(PackCard *pPackCard1,PackCard *pPackCard2)
//{
////    if (pPackCard1==pPackCard2)
////    {
////        return true;
////    }
//    if (pPackCard1 && pPackCard2)
//    {
//        //优先神将
////        int nCategory1 = pPackCard1->getCategory();
////        int nCategory2 = pPackCard2->getCategory();
//        if (pPackCard1->getCategory() > pPackCard2->getCategory() && enGeneralTp_God==pPackCard2->getCategory())
//        {
//            return true;
//        }
////        if (nCategory1==nCategory2)
////        {
////            return false;
////        }
////        if (nCategory1==nCategory2)
////        {
////            //使用中
////            if (!pPackCard1->beCurTeam() && pPackCard2->beCurTeam())
////            {
////                return true;
////            }
////            //属性
////        }
//        return pPackCard1->getCategory() < pPackCard2->getCategory();
//    }
//    return false;
//}

//排序君
void Package::rangePackageCardWithType(enCardRangeType type)
{
    //先把武将按cid排序，确保相同的武将在一起
    for (int i = 0; i < m_packCardArr->count() - 1; i++) {
        for (int j = i+1; j < m_packCardArr->count(); j++) {
            PackCard* iCard = m_packCardArr->getObjectAtIndex(i);
            PackCard* jCard = m_packCardArr->getObjectAtIndex(j);
            
            if (iCard->getCardId() > jCard->getCardId()) {
                iCard->retain();
                m_packCardArr->replaceObjectAtIndex(i, jCard);
                m_packCardArr->replaceObjectAtIndex(j, iCard);
                iCard->release();
            }
        }
    }
    
    testCount = type;
    if(!MainScene::Inst()->new_FirstPowerUp && !MainScene::Inst()->new_FirstZhuanSheng && !MainScene::Inst()->new_CardAdd)
    CGameData::Inst()->setDefaultRangeType(type);
    //StopwatchStart();
    if(type == enCardRangeTypeLock)//加锁
    {
        CCMutableArray<PackCard *> *s_packCardArray =  CCMutableArray<PackCard *>::arrayWithArray(m_packCardArr);
        CCMutableArray<PackCard*> *tempCardArray1 = new CCMutableArray<PackCard*>();
        CCMutableArray<PackCard*> *tempCardArray2 = new CCMutableArray<PackCard*>();
         
        // 按不同属性
        for(int t = enElementHeart; t < enElementMax;t++)
        {
            for(int i=0;i<s_packCardArray->count();i++)
            {
                    
                PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
                if((cuPackCard->getcType() == t )&& (cuPackCard->getCategory()<enGeneralTp_ZhuanSheng))
                {
                    tempCardArray1->addObject(cuPackCard);
                }
            }
            
            for(int i=0;i<s_packCardArray->count();i++)
            {
                
                PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
                if((cuPackCard->getcType() == t)&&(cuPackCard->getCategory()>=enGeneralTp_ZhuanSheng))
                {
                    tempCardArray1->addObject(cuPackCard);
                }
            }
            
        }
        s_packCardArray->removeAllObjects();
        s_packCardArray->release();
        
        //同一属性 排星星
        for(int i=0;i<tempCardArray1->count()-1;i++)
        {
            for (int j=0;j< tempCardArray1->count()-1-i; j++) {
                PackCard *card1 = tempCardArray1->getObjectAtIndex(j);
                PackCard *card2 = tempCardArray1->getObjectAtIndex(j+1);
                int star1 = card1->getStar();
                int star2 = card2->getStar();
                if((star1 < star2)&&
                   (card1->getcType()==card2->getcType())&&
                   (card1->isUsed()==false)&&
                   (card2->isUsed()==false)&&
                   (card1->getCategory()>enGeneralTp_ZhuanSheng)&&
                   (card2->getCategory()>enGeneralTp_ZhuanSheng))
                {
                    PackCard *tempCard1 = tempCardArray1->getObjectAtIndex(j);
                    PackCard *tempCard2 = tempCardArray1->getObjectAtIndex(j+1);
                    tempCardArray1->replaceObjectAtIndex(j, tempCard2);
                    tempCardArray1->replaceObjectAtIndex(j+1, tempCard1);
                }
                
                if((star1 < star2)&&
                   (card1->getcType()==card2->getcType())&&
                   (card1->isUsed()==false)&&
                   (card2->isUsed()==false)&&
                   (card1->getCategory()<=enGeneralTp_ZhuanSheng)&&
                   (card2->getCategory()<=enGeneralTp_ZhuanSheng))
                {
                    PackCard *tempCard1 = tempCardArray1->getObjectAtIndex(j);
                    PackCard *tempCard2 = tempCardArray1->getObjectAtIndex(j+1);
                    tempCardArray1->replaceObjectAtIndex(j, tempCard2);
                    tempCardArray1->replaceObjectAtIndex(j+1, tempCard1);
                }
            }
        }
        
        // 根据是否在使用中，是否加锁，将排好序的卡牌分成不同部分
        int usingCardNum = 0; // 适用中的卡牌数量
        int lockCardNum = 0; // 加锁的卡牌数量
                
        //  使用中&&加锁的
        for(int i =0;i<tempCardArray1->count();i++)
        {
            PackCard *cuPackCard = tempCardArray1->getObjectAtIndex(i);
            
            if (cuPackCard->isUsed()) {
                usingCardNum++;
                if(CGameData::Inst()->getIsLock(cuPackCard->getUcid()))
                {
                    tempCardArray2->addObject(cuPackCard);
                }
            }
            
            if (CGameData::Inst()->getIsLock(cuPackCard->getUcid())) {
                lockCardNum++;
            }
        }
        
        //  使用中&&不加锁的
        int usingUnlockCardNum = usingCardNum - tempCardArray2->count(); // 使用中&&不加锁的卡牌数量
        for (int i = 0; i < tempCardArray1->count(); i++) {
            if (tempCardArray2->count() == usingCardNum) {
                // 适用中&&不加锁的卡牌全部找到
                break;
            }
            PackCard *cuPackCard = tempCardArray1->getObjectAtIndex(i);
            if (cuPackCard->isUsed()
                && !CGameData::Inst()->getIsLock(cuPackCard->getUcid())) {
                tempCardArray2->addObject(cuPackCard);
            }
        }
        
        //  不使用&&加锁的
        for (int i = 0; i < tempCardArray1->count(); i++) {
            if (tempCardArray2->count() == lockCardNum + usingUnlockCardNum) {
                // 不使用&&加锁的卡牌全部找到
                break;
            }
            PackCard *cuPackCard = tempCardArray1->getObjectAtIndex(i);
            if (!cuPackCard->isUsed()
                && CGameData::Inst()->getIsLock(cuPackCard->getUcid())) {
                tempCardArray2->addObject(cuPackCard);
            }
        }
        
        // 不使用&&不加锁的
        for (int i = 0; i < tempCardArray1->count(); i++) {
            if (tempCardArray2->count() == tempCardArray1->count()) {
                // 不使用&&不加锁的卡牌全部找到
                break;
            }
            PackCard *cuPackCard = tempCardArray1->getObjectAtIndex(i);
            if (!cuPackCard->isUsed()
                && !CGameData::Inst()->getIsLock(cuPackCard->getUcid())) {
                tempCardArray2->addObject(cuPackCard);
            }
        }
        
        tempCardArray1->removeAllObjects();
        tempCardArray1->release();
    
        for(int i = 0;i < tempCardArray2->count();i++)
        {
            PackCard *cuPackCard = tempCardArray2->getObjectAtIndex(i);
            cuPackCard->setPosition(ccp(m_ptStart.x + ((i + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width),
                                        m_ptStart.y - ((i + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height)));
            cuPackCard->setTag(kCardTagBase+i);
        }
        
        tempCardArray2->removeAllObjects();
        tempCardArray2->release();
    
    }else if(type == enCardRangeTypeRec) {//按恢复力排序
        CCMutableArray<PackCard *> *s_packCardArray =  CCMutableArray<PackCard *>::arrayWithArray(m_packCardArr);
        CCMutableArray<PackCard*> *tempCardArray = new CCMutableArray<PackCard*>();
        
        for(int i=0;i<s_packCardArray->count()-1;i++)
        {
            for (int j=0;j< s_packCardArray->count()-1-i; j++) {
                int recover1 = s_packCardArray->getObjectAtIndex(j)->getRecover();
                int recover2 = s_packCardArray->getObjectAtIndex(j+1)->getRecover();
                if(recover1 < recover2)
                {
                    PackCard *tempCard1 = s_packCardArray->getObjectAtIndex(j);
                    PackCard *tempCard2 = s_packCardArray->getObjectAtIndex(j+1);
                    s_packCardArray->replaceObjectAtIndex(j, tempCard2);
                    s_packCardArray->replaceObjectAtIndex(j+1, tempCard1);
                }
            }
        }
        
        for(int i=0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->isUsed())
            {
                tempCardArray->addObject(cuPackCard);
            }
        }
        for(int i=0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->isUsed()== false)
                tempCardArray->addObject(cuPackCard);
        }
        for(int i=0;i<tempCardArray->count();i++)
        {
            PackCard *cuPackCard = tempCardArray->getObjectAtIndex(i);
            cuPackCard->setPosition(ccp(m_ptStart.x + ((i + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width),
                                        m_ptStart.y - ((i + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height)));
            cuPackCard->setTag(kCardTagBase+i);
            //  preCard
        }
        s_packCardArray->removeAllObjects();
        s_packCardArray->release();
        tempCardArray->removeAllObjects();
        tempCardArray->release();
        
    }else if(type == enCardRangeTypeAtta){ //按攻击力排序
        CCMutableArray<PackCard *> *s_packCardArray =  CCMutableArray<PackCard *>::arrayWithArray(m_packCardArr);
        CCMutableArray<PackCard*> *tempCardArray = new CCMutableArray<PackCard*>();
        
        for(int i=0;i<s_packCardArray->count()-1;i++)
        {
            for (int j=0;j< s_packCardArray->count()-1-i; j++) {
                int attack1 = s_packCardArray->getObjectAtIndex(j)->getAttack();
                int attack2 = s_packCardArray->getObjectAtIndex(j+1)->getAttack();
                if(attack1 < attack2)
                {
                    PackCard *tempCard1 = s_packCardArray->getObjectAtIndex(j);
                    PackCard *tempCard2 = s_packCardArray->getObjectAtIndex(j+1);
                    s_packCardArray->replaceObjectAtIndex(j, tempCard2);
                    s_packCardArray->replaceObjectAtIndex(j+1, tempCard1);
                }
            }
        }

        for(int i=0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->isUsed())
            {
                tempCardArray->addObject(cuPackCard);
            }
        }
        for(int i=0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->isUsed()== false)
                tempCardArray->addObject(cuPackCard);
        }
        for(int i=0;i<tempCardArray->count();i++)
        {
            PackCard *cuPackCard = tempCardArray->getObjectAtIndex(i);
            cuPackCard->setPosition(ccp(m_ptStart.x + ((i + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width),
                                        m_ptStart.y - ((i + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height)));
            cuPackCard->setTag(kCardTagBase+i);
            //  preCard
        }
        s_packCardArray->removeAllObjects();
        s_packCardArray->release();
        tempCardArray->removeAllObjects();
        tempCardArray->release();
        
    }else if(type == enCardRangeTypeHp) { //按血量排序
        CCMutableArray<PackCard *> *s_packCardArray =  CCMutableArray<PackCard *>::arrayWithArray(m_packCardArr);
        CCMutableArray<PackCard*> *tempCardArray = new CCMutableArray<PackCard*>();
        
        for(int i=0;i<s_packCardArray->count()-1;i++)
        {
            for (int j=0;j< s_packCardArray->count()-1-i; j++) {
                //                char *uid1 = s_packCardArray->getObjectAtIndex(j)->getUcid();
                //                char *uid2 = s_packCardArray->getObjectAtIndex(j+1)->getUcid();
                int hp1 = s_packCardArray->getObjectAtIndex(j)->getHp();
                int hp2 = s_packCardArray->getObjectAtIndex(j+1)->getHp();
                //CCLog("atack 1 %s 2 %s",s_packCardArray->getObjectAtIndex(0)->getUcid(),s_packCardArray->getObjectAtIndex(1)->getUcid());
                if(hp1 < hp2)
                {
                    PackCard *tempCard1 = s_packCardArray->getObjectAtIndex(j);
                    PackCard *tempCard2 = s_packCardArray->getObjectAtIndex(j+1);
                    s_packCardArray->replaceObjectAtIndex(j, tempCard2);
                    s_packCardArray->replaceObjectAtIndex(j+1, tempCard1);
                }
            }
        }
        
        for(int i=0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->isUsed())
            {
                tempCardArray->addObject(cuPackCard);
            }
        }
        for(int i=0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->isUsed()== false)
                tempCardArray->addObject(cuPackCard);
        }
        for(int i=0;i<tempCardArray->count();i++)
        {
            PackCard *cuPackCard = tempCardArray->getObjectAtIndex(i);
            cuPackCard->setPosition(ccp(m_ptStart.x + ((i + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width),
                                        m_ptStart.y - ((i + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height)));
            cuPackCard->setTag(kCardTagBase+i);
            //  preCard
        }
        s_packCardArray->removeAllObjects();
        s_packCardArray->release();
        tempCardArray->removeAllObjects();
        tempCardArray->release();
        
    }else if (type == enCardRangeTypeGet) {//入手顺序
        CCMutableArray<PackCard *> *s_packCardArray =  CCMutableArray<PackCard *>::arrayWithArray(m_packCardArr);
        CCMutableArray<PackCard*> *tempCardArray = new CCMutableArray<PackCard*>();
        
        for(int i=0;i<s_packCardArray->count()-1;i++)
        {
            for (int j=0;j< s_packCardArray->count()-1-i; j++) {
                //                char *uid1 = s_packCardArray->getObjectAtIndex(j)->getUcid();
                //                char *uid2 = s_packCardArray->getObjectAtIndex(j+1)->getUcid();
                unsigned long updTime1 = s_packCardArray->getObjectAtIndex(j)-> getUpdTime();
                unsigned long updTime2 = s_packCardArray->getObjectAtIndex(j+1)->getUpdTime();
                //CCLog("atack 1 %s 2 %s",s_packCardArray->getObjectAtIndex(0)->getUcid(),s_packCardArray->getObjectAtIndex(1)->getUcid());
                if(updTime1 < updTime2)
                {
                    PackCard *tempCard1 = s_packCardArray->getObjectAtIndex(j);
                    PackCard *tempCard2 = s_packCardArray->getObjectAtIndex(j+1);
                    s_packCardArray->replaceObjectAtIndex(j, tempCard2);
                    s_packCardArray->replaceObjectAtIndex(j+1, tempCard1);
                }
            }
        }
        
        for(int i=0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->isUsed())
            {
                tempCardArray->addObject(cuPackCard);
            }
        }
        for(int i=0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->isUsed()== false)
                tempCardArray->addObject(cuPackCard);
        }
        for(int i=0;i<tempCardArray->count();i++)
        {
            PackCard *cuPackCard = tempCardArray->getObjectAtIndex(i);
            cuPackCard->setPosition(ccp(m_ptStart.x + ((i + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width),
                                        m_ptStart.y - ((i + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height)));
            cuPackCard->setTag(kCardTagBase+i);
            //  preCard
        }
        s_packCardArray->removeAllObjects();
        s_packCardArray->release();
        tempCardArray->removeAllObjects();
        tempCardArray->release();
    }
    else if (type == enCardRangeTypePlus) {//+值顺序
        CCMutableArray<PackCard *> *s_packCardArray =  CCMutableArray<PackCard *>::arrayWithArray(m_packCardArr);
        CCMutableArray<PackCard*> *tempCardArray = new CCMutableArray<PackCard*>();
        
        for(int i=0;i<s_packCardArray->count()-1;i++)
        {
            for (int j=i+1;j< s_packCardArray->count(); j++) {
                
                int plus1 = s_packCardArray->getObjectAtIndex(i)->getPlus();
                int plus2 = s_packCardArray->getObjectAtIndex(j)->getPlus();
                
                if(plus1 < plus2)
                {
                    PackCard *tempCard1 = s_packCardArray->getObjectAtIndex(i);
                    PackCard *tempCard2 = s_packCardArray->getObjectAtIndex(j);
                    tempCard1->retain();
                    s_packCardArray->replaceObjectAtIndex(i, tempCard2);
                    s_packCardArray->replaceObjectAtIndex(j, tempCard1);
                    tempCard1->release();
                }
            }
        }
        
        for(int i=0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->isUsed())
            {
                tempCardArray->addObject(cuPackCard);
            }
        }
        for(int i=0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->isUsed()== false)
                tempCardArray->addObject(cuPackCard);
        }
        for(int i=0;i<tempCardArray->count();i++)
        {
            PackCard *cuPackCard = tempCardArray->getObjectAtIndex(i);
            cuPackCard->setPosition(ccp(m_ptStart.x + ((i + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width),
                                        m_ptStart.y - ((i + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height)));
            cuPackCard->setTag(kCardTagBase+i);
            //  preCard
        }
        s_packCardArray->removeAllObjects();
        s_packCardArray->release();
        tempCardArray->removeAllObjects();
        tempCardArray->release();
    }
    else if (type == enCardRangeTypeCost) {//统御力顺序
        CCMutableArray<PackCard *> *s_packCardArray =  CCMutableArray<PackCard *>::arrayWithArray(m_packCardArr);
        CCMutableArray<PackCard*> *tempCardArray = new CCMutableArray<PackCard*>();
        
        for(int i=0;i<s_packCardArray->count()-1;i++)
        {
            for (int j=i+1;j< s_packCardArray->count(); j++) {

                int cost1 = s_packCardArray->getObjectAtIndex(i)->getCost();
                int cost2 = s_packCardArray->getObjectAtIndex(j)->getCost();

                if(cost1 < cost2)
                {
                    PackCard *tempCard1 = s_packCardArray->getObjectAtIndex(i);
                    PackCard *tempCard2 = s_packCardArray->getObjectAtIndex(j);
                    tempCard1->retain();
                    s_packCardArray->replaceObjectAtIndex(i, tempCard2);
                    s_packCardArray->replaceObjectAtIndex(j, tempCard1);
                    tempCard1->release();
                }
            }
        }
        
        for(int i=0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->isUsed())
            {
                tempCardArray->addObject(cuPackCard);
            }
        }
        for(int i=0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->isUsed()== false)
                tempCardArray->addObject(cuPackCard);
        }
        for(int i=0;i<tempCardArray->count();i++)
        {
            PackCard *cuPackCard = tempCardArray->getObjectAtIndex(i);
            cuPackCard->setPosition(ccp(m_ptStart.x + ((i + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width),
                                        m_ptStart.y - ((i + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height)));
            cuPackCard->setTag(kCardTagBase+i);
            //  preCard
        }
        s_packCardArray->removeAllObjects();
        s_packCardArray->release();
        tempCardArray->removeAllObjects();
        tempCardArray->release();
    }
    else if (enCardRangeTypeCategory==type)
    {
//        CCMutableArray<PackCard*>::sortArray(m_packCardArr,_sortCategory);
//        int nCount = m_packCardArr->count();
//        for (int i = 0; i < nCount; ++i)
//        {
//            if (PackCard *pPackCard = m_packCardArr->getObjectAtIndex(i))
//            {
//                pPackCard->setPosition(ccp(m_ptStart.x + ((i + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width),
//                                            m_ptStart.y - ((i + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height)));
//                pPackCard->setTag(kCardTagBase+i);
//            }
//        }
        
        static std::map<int,int>   mapTemp;
        if (mapTemp.empty())
        {
            mapTemp[enGeneralTp_God] = 0;
            mapTemp[enGeneralTp_mojiang] = 1;
            mapTemp[enGeneralTp_GuoShi] = 2;
            mapTemp[enGeneralTp_GongShou] = 3;
            mapTemp[enGeneralTp_ShengSheng] = 4;
            mapTemp[enGeneralTp_XiaoYong] = 5;
            mapTemp[enGeneralTp_JinShi] = 6;
            mapTemp[enGeneralTp_ZhuanSheng] = 7;
            mapTemp[enGeneralTp_QiangHua] = 8;
        
        }
        
        int nCategory1 = 0,nCategory2 = 0;
        bool bUse1 = false,bUse2 = false;
//        bool bCost1 = false,bCost2 = false;
        PackCard *pPackCard1 = NULL,*pPackCard2 = NULL;
        std::map<int,int>::iterator it,end = mapTemp.end();
        const int nConstCount = m_packCardArr->count();
        for (int i = 0; i < nConstCount-1; ++i)
        {
            for (int j = i+1; j < nConstCount; ++j)
            {
                pPackCard1 = m_packCardArr->getObjectAtIndex(i);
                pPackCard2 = m_packCardArr->getObjectAtIndex(j);
                if (pPackCard1 && pPackCard2)
                {
//                    bUse1 = pPackCard1->beUseNow();
//                    bUse2 = pPackCard2->beUseNow();
//
//                    bCost1 = pPackCard1->beCostEnough();
//                    bCost2 = pPackCard2->beCostEnough();
//                    bCost1 = bUse1? true:bCost1;
//                    bCost2 = bUse2? true:bCost2;
//                    
//                    it = mapTemp.find(pPackCard1->getCategory());
//                    if (it != end) nCategory1 = it->second;
//                    
//                    it = mapTemp.find(pPackCard2->getCategory());
//                    if (it != end) nCategory2 = it->second;
//                    
//                    if ((!bUse1 && bUse2)||
//                        (bCost1 && !bCost2 && !bUse1 && !bUse2)||
//                        (!bUse1 && !bUse2 && nCategory1 > nCategory2))
//                    {
//                        pPackCard1->retain();
//                        m_packCardArr->replaceObjectAtIndex(i,pPackCard2);
//                        m_packCardArr->replaceObjectAtIndex(j,pPackCard1);
//                        pPackCard1->release();
//                    }
                    
                    bUse1 = pPackCard1->isUsed();
                    bUse2 = pPackCard2->isUsed();
                    
                    it = mapTemp.find(pPackCard1->getCategory());
                    if (it != end) nCategory1 = it->second;
                    
                    it = mapTemp.find(pPackCard2->getCategory());
                    if (it != end) nCategory2 = it->second;
                    
                    if ((!bUse1 && bUse2) ||
                        (!bUse1 && !bUse1 && nCategory1 > nCategory2))
                    {
                        pPackCard1->retain();
                        m_packCardArr->replaceObjectAtIndex(i,pPackCard2);
                        m_packCardArr->replaceObjectAtIndex(j,pPackCard1);
                        pPackCard1->release();
                    }
                }
            }
        }
        
        for (int i = 0; i < nConstCount; ++i)
        {
            if (PackCard *pPackCard = m_packCardArr->getObjectAtIndex(i))
            {
                pPackCard->setPosition(ccp(m_ptStart.x + ((i + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width),
                                           m_ptStart.y - ((i + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height)));
                pPackCard->setTag(kCardTagBase+i);
            }
        }
        
    }
    else if (enCardRangeTypeCountryPorterty == type)
    {
        static std::map<int,int>   mapTemp;
        if (mapTemp.empty())
        {
            mapTemp[enCountryTp_Null] = 0;
            mapTemp[enCountryTp_dahan] = 1;
            mapTemp[enCountryTp_weiguo] = 2;
            mapTemp[enCountryTp_shuguo] = 3;
            mapTemp[enCountryTp_wuguo] = 4;
            mapTemp[enCountryTp_qunying] = 5;
            mapTemp[enCountryTp_jindai] = 6;
        }
        
        int nCountryPorperty1 = 0,nCountryPorperty2 = 0;
        bool bUse1 = false,bUse2 = false;
        //        bool bCost1 = false,bCost2 = false;
        PackCard *pPackCard1 = NULL,*pPackCard2 = NULL;
        std::map<int,int>::iterator it,end = mapTemp.end();
        const int nConstCount = m_packCardArr->count();
        for (int i = 0; i < nConstCount-1; ++i)
        {
            for (int j = i+1; j < nConstCount; ++j)
            {
                pPackCard1 = m_packCardArr->getObjectAtIndex(i);
                pPackCard2 = m_packCardArr->getObjectAtIndex(j);
                if (pPackCard1 && pPackCard2)
                {
                    bUse1 = pPackCard1->isUsed();
                    bUse2 = pPackCard2->isUsed();
                    
                    it = mapTemp.find(pPackCard1->getCountryPoerperty());
                    if (it != end) nCountryPorperty1 = it->second;
                    
                    it = mapTemp.find(pPackCard2->getCountryPoerperty());
                    if (it != end) nCountryPorperty2 = it->second;
                    
                    if ((!bUse1 && bUse2) ||
                        (!bUse1 && !bUse1 && nCountryPorperty1 > nCountryPorperty2))
                    {
                        pPackCard1->retain();
                        m_packCardArr->replaceObjectAtIndex(i,pPackCard2);
                        m_packCardArr->replaceObjectAtIndex(j,pPackCard1);
                        pPackCard1->release();
                    }
                }
            }
        }
        
        for (int i = 0; i < nConstCount; ++i)
        {
            if (PackCard *pPackCard = m_packCardArr->getObjectAtIndex(i))
            {
                pPackCard->setPosition(ccp(m_ptStart.x + ((i + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width),
                                           m_ptStart.y - ((i + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height)));
                pPackCard->setTag(kCardTagBase+i);
            }
        }
        
    }

       else {  // 属性
        CCMutableArray<PackCard *> *s_packCardArray =  CCMutableArray<PackCard *>::arrayWithArray(m_packCardArr);
        CCMutableArray<PackCard*> *tempCardArray = new CCMutableArray<PackCard*>();
        
        //将使用中的排在前列
        for(int i =0;i<s_packCardArray->count();i++)
        {
            PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);
            if(cuPackCard->isUsed())
            {
                tempCardArray->addObject(cuPackCard);
            }
        }
        //按属性添加
        for(int t = enElementHeart; t < enElementMax;t++)
        {
            for(int i=0;i<s_packCardArray->count();i++)
            {
                
                PackCard *cuPackCard = s_packCardArray->getObjectAtIndex(i);

                if((cuPackCard->getcType() == t) &&
                   cuPackCard&&
                   (cuPackCard->isUsed()==false))
                {
                    tempCardArray->addObject(cuPackCard);
                    // s_packCardArray->removeObjectAtIndex(i);
                }
            }
        }
        s_packCardArray->removeAllObjects();
        s_packCardArray->release();
        
 
        //同一属性 牌星星
        for(int i=0;i<tempCardArray->count()-1;i++)
        {
            for (int j=0;j< tempCardArray->count()-1-i; j++) {
                PackCard *card1 = tempCardArray->getObjectAtIndex(j);
                PackCard *card2 = tempCardArray->getObjectAtIndex(j+1);
                int star1 = card1->getStar();
                int star2 = card2->getStar();
                if((star1 < star2)&&
                   (card1->getcType()==card2->getcType())&&
                   (card1->isUsed()==false)&&
                   (card2->isUsed()==false))
                {
                    PackCard *tempCard1 = tempCardArray->getObjectAtIndex(j);
                    PackCard *tempCard2 = tempCardArray->getObjectAtIndex(j+1);
                    tempCardArray->replaceObjectAtIndex(j, tempCard2);
                    tempCardArray->replaceObjectAtIndex(j+1, tempCard1);
                }

            }
        }
        //按照category分布
        
        for(int i=0;i<tempCardArray->count()-1;i++)
        {
            for (int j=0;j< tempCardArray->count()-1-i; j++) {
                PackCard *card1 = tempCardArray->getObjectAtIndex(j);
                PackCard *card2 = tempCardArray->getObjectAtIndex(j+1);
                
                int cat1 = card1->getCategory();
                if(cat1 == enGeneralTp_God)
                    cat1 = -1;
                
                int cat2 = card2->getCategory();
                if(cat2 == enGeneralTp_God)
                    cat2 = -1;
                
                if(cat1>cat2&&card1->getcType()==card2->getcType()
                   &&card1->getStar() == card2->getStar()
                   &&card1->isUsed() == false
                   &&card2->isUsed() == false)
                {
                    tempCardArray->replaceObjectAtIndex(j, card2);
                    tempCardArray->replaceObjectAtIndex(j+1, card1);
                }
            }
            
        }

      
        
        for(int i=0;i<tempCardArray->count();i++)
        {
            PackCard *cuPackCard = tempCardArray->getObjectAtIndex(i);
            cuPackCard->setPosition(ccp(m_ptStart.x + ((i + m_iRmBtn) % m_iColumn) * (m_iGap + m_cardSize.width),
                                        m_ptStart.y - ((i + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height)));
            cuPackCard->setTag(kCardTagBase+i);
            //  preCard
        }
        tempCardArray->removeAllObjects();
        tempCardArray->release();
    }
    //StopwatchStop();
    setThisPosition(this->getPosition(), true);
    if(m_pChildren && m_pChildren->count() > 0)
	{
        CCObject* pChild = NULL;
        CCARRAY_FOREACH(m_pChildren, pChild)
        {
            if (PackCard *pCard = dynamic_cast<PackCard*>(pChild))
            {
                pCard->hintText(testCount);
            }
		}
	}
}

int Package::getRangeType()
{
    return  (testCount)%encardRangeTypeMax;
}

//******************************************************************************
// showDlgNiePan
//******************************************************************************
void Package::showDlgNiePan()
{
    if(m_niepanDlg == NULL){
        m_niepanDlg = new Dialog2(this,
                                  callfuncO_selector(Package::cbDlgNiePan),
                                  CGameData::Inst()->getLanguageValue("cntry_hint"),
                                  "",
                                  enDl2TwoBtn);
        addChild(m_niepanDlg, 100);
        m_niepanDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
        m_niepanDlg->release();
    }
   
    OcProxy::Inst()->showWebWithType(enWebviewLoadTypeRequest,
                                     CGameData::Inst()->getUrlDepDevice(CGameData::Inst()->getCommonInfo()->niepan_notice),
                                     CCRect(36*2, 176*2, 248*2, 122*2),
                                     false);
}

//******************************************************************************
// cbDlgNiePan
//******************************************************************************
void Package::cbDlgNiePan(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    unschedule(schedule_selector(Package::waitShowInfo));
   
    OcProxy::Inst()->removeWebview();
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    removeChild(m_niepanDlg, true);
    m_niepanDlg = NULL;

    if(pRet->bOk == true){
        strncpy(m_ret.uid[0].uid, m_pUsrCardSel->ucid, 99);
        m_ret.bNiePan = true;
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
    else {
        
    }
}

//******************************************************************************
// ccTouchEndedLookTpBtnSel
//******************************************************************************
void Package::ccTouchEndedLookTpBtnSel()
{
    if (m_iLookTp == enPkLkTp_JustLook) {
        if (CGameData::Inst()->getIsLock(m_pUsrCardSel->ucid)){
            m_cfmLayer->show(m_pUsrCardSel, NULL, enCConfirmLayer_UNLOCK);
        }
        else {
            m_cfmLayer->show(m_pUsrCardSel, NULL, enCConfirmLayer_LOCK);
        }
        
        MainScene::Inst()->setGenRangeBtnIsVisble(false);
    }
    else if (m_iLookTp == enPkLkTp_Evolution)
    {
        if (m_pUsrCardSel->upg_flg>0 && m_pUsrCardSel->wk_flg > 0)
        {
            m_bCmfFourBtn = true;
            m_cfmLayer->show(m_pUsrCardSel, NULL, enCConfirmLayer_nEVO_Wake);
        }
        else if (m_pUsrCardSel->wk_flg > 0)
            m_cfmLayer->show(m_pUsrCardSel, NULL, enCConfirmLayer_Wake);
        else if (m_pUsrCardSel->elu_flg > 0)
            m_cfmLayer->show(m_pUsrCardSel, NULL, enCConfirmLayer_SEVO);
        else
            m_cfmLayer->show(m_pUsrCardSel, NULL, enCConfirmLayer_EVO);
        
        MainScene::Inst()->setGenRangeBtnIsVisble(false);
    }
    
    else if (m_iLookTp == enPkLkTp_PwUpBase)
    {
        if (m_pUsrCardSel->bIsNPTgt)
        {
            m_bCmfFourBtn = true;
            m_cfmLayer->show(m_pUsrCardSel, NULL, enCConfirmLayer_POWUPNIEPAN);
        }
        else
            m_cfmLayer->show(m_pUsrCardSel, NULL, enCConfirmLayer_POWUP);
        MainScene::Inst()->setGenRangeBtnIsVisble(false);
    }
    else {
        m_cfmLayer->show(m_pUsrCardSel, NULL);
        MainScene::Inst()->setGenRangeBtnIsVisble(false);
    }
}

//******************************************************************************
// showDlgIsSell
//******************************************************************************
void Package::showDlgIsSell() {
    if(m_sellDlg == NULL){
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
        
        char titleBuf[100];
        snprintf(titleBuf, 99, "%s!!!", CGameData::Inst()->getLanguageValue("sell_title"));
        char buf[100];
        snprintf(buf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("sell_alert_1"), m_packSelBtn->getGoldSell(), CGameData::Inst()->getLanguageValue("sell_alert_2"));
        
        m_sellDlg = new Dialog2(this, callfuncO_selector(Package::cbSellDlg), titleBuf, buf,enDl2TwoBtn);
        addChild(m_sellDlg, 100);
        m_sellDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
        m_sellDlg->release();
    }
}
//******************************************************************************
// showDlgSellUpThreeStars
//******************************************************************************
void Package::showDlgSellUpThreeStars() {
    if(m_sellUpThreeStarsDlg == NULL){
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
        
        char buf[100];
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("sell_alert_stars"));
        
        m_sellUpThreeStarsDlg = new Dialog2(this, callfuncO_selector(Package::cbSellUpThreeStarsDlg), CGameData::Inst()->getLanguageValue("alert"), buf, enDl2TwoBtn);
        addChild(m_sellUpThreeStarsDlg, 100);
        m_sellUpThreeStarsDlg->setPosition(CCPointMake(320, 480 - this->getPosition().y));
        m_sellUpThreeStarsDlg->release();
    }
}


//******************************************************************************
// setThisPosition
//******************************************************************************
void Package::setThisPosition(const CCPoint& pos, bool bRedraw)
{
    CCPoint pt = pos;
    if (pt.y > m_fUpLimitY) {
        pt.y = m_fUpLimitY;
    }
    
    if (pt.y < 0) {
        pt.y = 0;
    }
    
    this->setPosition(pt);
    
    if(m_packSelBtn != NULL)
        m_packSelBtn->setPosition(CCPointMake(0, - pt.y));
    
    if(FiveTeamLayer != NULL)
        FiveTeamLayer->setPosition(CCPointMake(320, 190 - pt.y));
    
    m_dlgPoint = CCPoint(320,480-pt.y);

    //不强制要求重画，而且移动范围小于100
    if((!bRedraw) && (abs(m_iPreThisPosY - (int)pt.y) < 100))
        return;
    //改变m_pScrollBar位置
//    CCLOG("m_pScrollBarY=%f",m_pScrollBarY);
    if (m_pScrollBar) {
        if((enPkLkTp_PwUpFood == m_iLookTp)
           || (enPkLkTp_NiePanFood == m_iLookTp)
           || (enPkLkTp_Sell == m_iLookTp)
           || (enPkLkTp_TeamReset == m_iLookTp))
            m_pScrollBar->setPosition(CCPointMake(630, 482 - pt.y));
        else
            m_pScrollBar->setPosition(CCPointMake(630, 452 - pt.y));
        redrawCapacity();
        m_pScrollBar->setPercent(pt.y/m_fUpLimitY);
        redrawCapacity();
    }
    
//    CCNode* node;
//    for(int i = 0; i < m_usrCardNum; i++){
//        node = getChildByTag(kCardTagBase + i);
//        if(node){
//            PackCard* card = (PackCard*) node;
//            CCPoint ptCard = card->getPosition();
//            
//            if((ptCard.y + pt.y < 960) && (ptCard.y + pt.y > 0)){
//                card->show(testCount);
//            }
//            else{
//                card->hide();
//            }
//        }
//    }
//    m_iPreThisPosY = pt.y;
    
    if(m_pChildren && m_pChildren->count() > 0)
	{
        CCObject* pChild = NULL;
        CCARRAY_FOREACH(m_pChildren, pChild)
        {
            if (PackCard *pCard = dynamic_cast<PackCard*>(pChild))
            {
                CCPoint ptCard = pCard->getPosition();
                if((ptCard.y + pt.y < 960) && (ptCard.y + pt.y > 0))
                {
                    pCard->show(testCount);
                }
                else
                {
                    pCard->hide();
                }
            }
		}
	}
    m_iPreThisPosY = pt.y;
}

//******************************************************************************
// updateScreenWithCardCntChg
//******************************************************************************
void Package::updateScreenWithCardCntChg()
{
    float fLastYPos = m_ptStart.y - ((m_usrCardNum - 1 + m_iRmBtn) / m_iColumn) * (m_iGap + m_cardSize.height);
    
    m_fUpLimitY = gfDownLimitY - (fLastYPos - m_cardSize.height);
    m_fUpLimitY = MAX(0, m_fUpLimitY);
    
    CCPoint thisPt = this->getPosition();
    thisPt.y = MIN(thisPt.y, m_fUpLimitY);
    
    if(m_fUpLimitY > 0){
        float fBarSize = 1.0 / ((m_fUpLimitY / (720 - gfDownLimitY)) + 2);
        m_pScrollBar->setBarSize(fBarSize);
        
		float fPercent = thisPt.y / m_fUpLimitY;
        
        m_pScrollBar->setPercent(fPercent);
        
        if((enPkLkTp_PwUpFood == m_iLookTp)
           || (enPkLkTp_NiePanFood == m_iLookTp)
           || (enPkLkTp_Sell == m_iLookTp)
           || (enPkLkTp_TeamReset == m_iLookTp))
            m_pScrollBar->setPosition(CCPointMake(630, 482 - thisPt.y));
        else
            m_pScrollBar->setPosition(CCPointMake(630, 452 - thisPt.y));
    }
    else{
        if(m_pScrollBar)
            m_pScrollBar->setIsVisible(false);
    }
    
    
    setThisPosition(thisPt, true);
}

//******************************************************************************
// NtyGoTop
//******************************************************************************
void Package::NtyGoTop(CCObject* obj)
{
    if (m_cfmLayer && m_cfmLayer->getIsVisible())
        return;
    
    if (m_niepanDlg || m_sellDlg || m_sellUpThreeStarsDlg)
        return;
    
    stopAllActions();
    setThisPosition(ccp(0,0));
}

//******************************************************************************
// NtyGoBottom
//******************************************************************************
void Package::NtyGoBottom(CCObject* obj)
{
    if (m_cfmLayer && m_cfmLayer->getIsVisible())
        return;
    
    if (m_niepanDlg || m_sellDlg || m_sellUpThreeStarsDlg)
        return;

    stopAllActions();
    setThisPosition(ccp(0,m_fUpLimitY));
}

//******************************************************************************
// onExit
//******************************************************************************
void Package::onExit()
{
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this, kNotifyGoTop);
    CCNotificationCenter::sharedNotifCenter()->removeObserver(this, kNotifyGoBottom);
    
    if (m_pScrollViewLayer && m_pScrollViewLayer->getIsVisible())
    {
        m_pScrollViewLayer->hide();
    }
    
    OcProxy::Inst()->removeWebview();
    m_packCardArr->removeAllObjects();
    m_packCardArr->release();
    if (m_noMainGeneralDlg) {
        m_noMainGeneralDlg->removeFromParentAndCleanup(true);
        m_noMainGeneralDlg = NULL;
    }
    
    removeAllChildrenWithCleanup(true);
        
    CCLayer::onExit();
}

//******************************************************************************
// thisPositionX
//******************************************************************************
void Package::thisPositionX(PackCard *pPackCard,bool bGo)
{
    if (!pPackCard || 0<numberOfRunningActions())return;
    //printf("thisPositionX-----------%d----------",numberOfRunningActions());
    //assert(14.0f<=fabsf(m_lastXm));
    
    printf("PackCard[type] = %d\n",pPackCard->getcType());
    int nChangeType = 0;
    int nCount = m_packCardArr->count();
    for (int i = 1; i <enElementMax; ++i)
    {
        if (bGo)
        {
            nChangeType = (pPackCard->getcType()+i)%enElementMax;       //切换下一个自己有的类型
        }
        else
        {
            nChangeType = ((pPackCard->getcType()-i)+enElementMax)%enElementMax;   //切换上一个自己有的类型
        }
        
        for (int i = 0; i < nCount; ++i)
        {
            if (PackCard *pPackCard = (PackCard*)getChildByTag(kCardTagBase+i))
            {
                if (nChangeType==pPackCard->getcType() && !pPackCard->beUseNow())
                {
                    setThisPosition(ccp(getPosition().x,(m_cardSize.height+m_iGap)*((pPackCard->getTag()-kCardTagBase)/m_iColumn)), true);
                    return;
                }
            }
        }
    }
}

void Package::tickTimeCardText(ccTime dt)
{
    if(m_pChildren && 0<m_pChildren->count())
	{
        CCObject* pChild = NULL;
        CCARRAY_FOREACH(m_pChildren, pChild)
        {
            if (PackCard *pCard = dynamic_cast<PackCard*>(pChild))
            {
                pCard->updateText(g_nTickCount);
            }
		}
	}
    g_nTickCount++;
}

bool Package::isShowScrollViewLayer()
{
    return m_pScrollViewLayer && m_pScrollViewLayer->getIsVisible();
}











