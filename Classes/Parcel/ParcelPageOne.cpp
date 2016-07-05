//
//  ParcelPageOne.cpp
//  AgainstWar
//
//  Created by AllenSK on 14-2-11.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#include "ParcelPageOne.h"

#include "CMenuBtn.h"
#include "CListLayer.h"
#include "CScrollBar.h"
#include "TeamInf.h"
#include "PwUpBase.h"
#include "MainScene.h"
#include "Evolution.h"
#include "EvoOverShow.h"
#include "CommDlg.h"
#include "TitleBar.h"
#include "SimpleAudioEngine.h"
#include "Arrow.h"
#include "Dialog2.h"
#include "TextNode.h"
#include "OcProxy.h"
#include "TeamInfoScrollViewLayer.h"

#include "ParcelBackPage.h"

using namespace CocosDenshion;


#define ITEMCARDDELTA   9999
#define kNewbieEndTag           82
#define kGeneralBackBtn     3
#define kGeneralArrowTag    888
#define kGeneralNiePanTag   999

#define kNewbie             99999
//******************************************************************************
// Construct
//******************************************************************************
ParcelPageOne::ParcelPageOne(CCObject* target, SEL_CallFuncO selector)
{
//    newBool = false;
//    
//    m_SortBtnLayer = NULL;
//    //初始化记录位置arraypackageY
//    for (int i = 0; i < enPkLkTp_Max; i++) {
//        arraypackageY[i]=0;
//    }
//    
//    memset(&m_teamInfoUId, 0, sizeof(m_teamInfoUId));
//    m_PreCurTeamIdx = 0;
    
    
    m_pListener = target;
    m_pfnSelector = selector;
    
    
    
    
    
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(ParcelPageOne::NtyGoItem),  // 处理的消息的回调函数
                                                           kNotifyItem,                    // 感兴趣的消息名称
                                                           NULL);
    
    CCNotificationCenter::sharedNotifCenter()->addObserver(this,
                                                           callfuncO_selector(ParcelPageOne::NtyGoChild),  // 处理的消息的回调函数
                                                           kNotifyChild,                    // 感兴趣的消息名称
                                                           NULL);

    
    

    
    
    
}

void ParcelPageOne::NtyGoItem()
{
    m_choiceChild = false;
    
    CCMutableArray<ParcelChildInfo* > * parcelChildInfoArr =  CGameData::Inst()->getParcelChildInfoAry();
    
    for (int i = 0 ; i < parcelChildInfoArr->count(); i ++) //i < itemCardInf->count()
    {

        CCSprite * tempItem = (CCSprite*)getChildByTag(ITEMCARDDELTA+i);//头像显示
        
        if(tempItem)
            tempItem->removeFromParentAndCleanup(true);
    
    }
    
    CScrollBar*     pScrollBar = (CScrollBar*)getChildByTag(9988);
    if(pScrollBar)
        pScrollBar->removeFromParentAndCleanup(true);
    
    
    this->setPosition(CCPointMake(0, 0));
    
    showItemCard();
    
}

void ParcelPageOne::NtyGoChild()
{
    m_choiceChild = true;
    

    CCMutableArray<ParcelItemInfo* > * parcelItemInfoArr =  CGameData::Inst()->getParcelItemInfoAry();
    
    for (int i = 0 ; i < parcelItemInfoArr->count(); i ++) //i < itemCardInf->count()
    {
        
        CCSprite * tempItem = (CCSprite*)getChildByTag(ITEMCARDDELTA+i);//头像显示
        
        if(tempItem)
            tempItem->removeFromParentAndCleanup(true);
        
    }
    

    CScrollBar*     pScrollBar = (CScrollBar*)getChildByTag(9988);
    if(pScrollBar)
        pScrollBar->removeFromParentAndCleanup(true);
    
    
    this->setPosition(CCPointMake(0, 0));
    
    
    showChildCard();
}



void ParcelPageOne::shareToWeibo()
{
    if(CGameData::Inst()->getLoginType()==enLoginReqType_OcLogin)
    {
        Dialog2 *dialog = new Dialog2(this,
                                      callfuncO_selector(ParcelPageOne::dl2CallBack),
                                      CGameData::Inst()->getLanguageValue("weibo_authened"),
                                      CGameData::Inst()->getLanguageValue("weibo_authened_info"),
                                      enDl2TwoBtn);
        addChild(dialog);
        dialog->setTag(111);
        dialog->setPosition(ccp(320, 480));
        dialog->release();
    }
    else
    {
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        CCRenderTexture *target = CCRenderTexture::renderTextureWithWidthAndHeight(winSize.width, winSize.height);
        target->begin();
        MainScene::Inst()->visit();
        target->end();
        target->saveBuffer(kCCImageFormatPNG,"captureImage.png");
        MainScene::Inst()->showWeiboPublishInfoWithType(4);
    }
    
}

void ParcelPageOne::dl2CallBack(CCObject *sender)
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
//******************************************************************************
// enter
//******************************************************************************
void ParcelPageOne::enter()
{
    m_choiceChild = false;
    

    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -127, true);
    
    
    
    showItemCard();
    
    return;
    
    if (CGameData::Inst()->isFighting())
        return;
    m_bFromHome = false;
    m_bNiePan = false;
    m_package = NULL;
    m_teamInf = NULL;
    m_pwUpBase = NULL;
    m_evolution = NULL;
    m_evoOvrShow = NULL;
    m_commDlg = NULL;
    m_itemCardLayer = NULL;
    m_SortBtnLayer = NULL;
    m_iTeamMemberSel = kInvalidValue;
    
    //title
    m_spTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
    m_spTitle->setPosition(CCPointMake(210, 766));
    addChild(m_spTitle, 100);
    
    rangeLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("range_property"),CCSizeMake(150,80),CCTextAlignmentCenter  , 25);
    m_spTitle->addChild(rangeLabel);
    rangeLabel->setPosition(ccp(335, 15));
    rangeLabel->setIsVisible(false);
    
    //main menu
    m_listMain = new CListLayer(this, callfuncO_selector(ParcelPageOne::cbList));
    addChild(m_listMain, -1);
    
//    CCSprite *tempSprite1 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
//    CCSprite *tempSprite2 = CCSprite::spriteWithSpriteFrameName("id_Reset.png");
//    
//    m_rangeBtn = CCMenuItemImage::itemFromNormalSprite(tempSprite1, tempSprite2, this, menu_selector(ParcelPageOne::cbChangePackRange));
//    m_rangeBtn->setAnchorPoint(CCPointZero);
//    CCMenu *rangeMenu = CCMenu::menuWithItem(m_rangeBtn);
//    addChild(rangeMenu,2);
//    rangeMenu->setPosition(CCPointZero);
    
    m_lbRangeTitle = TextNode::textWithString("", 26);
    m_lbRangeTitle->setPosition(ccp(53, 10));
    
    m_lbRangeTitleBg = TextNode::textWithString("", 26);
    m_rangeBtn->addChild(m_lbRangeTitleBg);
    m_lbRangeTitleBg->setPosition(ccp(53+2, 10-2));
    m_lbRangeTitleBg->setColor(ccc3(0, 0, 0));
    
    m_rangeBtn->addChild(m_lbRangeTitle);
    
    CMenuBtn* btn;
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_biandui"),
                       CCTextAlignmentCenter);
    m_listMain->push_back(btn);
    btn->release();
    
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_qianghua"),
                       CCTextAlignmentCenter);
    
    
    if (CGameData::Inst()->getCommonInfo()->skill_lv_up_starttime != -1)
    {
        unsigned long ulCurTime = CGameData::Inst()->getCurrentTime();
        
        if (ulCurTime > CGameData::Inst()->getCommonInfo()->skill_lv_up_starttime
            && ulCurTime < CGameData::Inst()->getCommonInfo()->skill_lv_up_endtime
            )
        {
            long lTimeOffset = CGameData::Inst()->getCommonInfo()->skill_lv_up_endtime - ulCurTime;
            
            if(lTimeOffset < 0)
                lTimeOffset = 0;
            
            int hour = lTimeOffset / 3600;
            int minute = lTimeOffset / 60;
            int day = lTimeOffset / (3600*24);
            
            char tBuf[100];
            
            if(hour >=24)
            {
                snprintf(tBuf, 99, "%s%d%s",CGameData::Inst()->getLanguageValue("left"), day, CGameData::Inst()->getLanguageValue("left_day"));
            }
            else if(hour > 0&&hour<24)
                snprintf(tBuf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("left"), hour, CGameData::Inst()->getLanguageValue("left_hour"));
            else
                snprintf(tBuf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("left"), minute, CGameData::Inst()->getLanguageValue("left_minute"));
            
            
            TextNode* subText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sk_lv_up_double"), 26);
            subText->setPosition(CCPointMake(-262, 39));
            subText->setShadowColor(ccBLACK);
            subText->setColor(ccYELLOW);
            subText->setAnchorPoint(ccp(0, 1));
            btn->addChild(subText,3);
            
            TextNode* subText2 = TextNode::textWithString(tBuf, 26);
            subText2->setPosition(CCPointMake(252, -30));
            subText2->setShadowColor(ccBLACK);
            subText2->setAnchorPoint(ccp(1, 0));
            btn->addChild(subText2,3);
            
        }
        
    }
    
    
    m_listMain->push_back(btn);
    btn->release();
    
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_zhuansheng"),
                       CCTextAlignmentCenter);
    m_listMain->push_back(btn);
    btn->release();
    
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_sell"),
                       CCTextAlignmentCenter);
    m_listMain->push_back(btn);
    btn->release();
    
    btn = new CMenuBtn("ma_button.png",
                       CGameData::Inst()->getLanguageValue("title_box"),
                       CCTextAlignmentCenter);
    m_listMain->push_back(btn);
    btn->release();
    
    //back button
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    m_miBack = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(ParcelPageOne::btnCallback) );
    m_miBack->setTag(kGeneralBackBtn);
    m_miBack->setAnchorPoint(CCPointZero);
    
    //随身军袋
    spMenu1 = CCSprite::spriteWithFile("suishenjundai.png");
    spMenu2 = CCSprite::spriteWithFile("suishenjundai.png");
    m_JunDaiItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(ParcelPageOne::jdTitleBtn));
    m_JunDaiItem->setAnchorPoint(CCPointZero);
    m_JunDaiItem->setPosition(CCPointMake(520, 730));
    
    CCMenu* menu = CCMenu::menuWithItems( m_miBack, m_JunDaiItem, NULL);
    addChild(menu, 30);
    menu->setPosition(CCPointZero);
    
    
    m_miBack->setPosition(CCPointMake(-1000, -1000));
    m_spTitle->setPosition(CCPointMake(-1000, -1000));
    m_listMain->setPosition(CCPointMake(-1000, -1000));
    m_rangeBtn->setPosition(ccp(-1000,-1000));
    
    m_rangeBtn->setIsVisible(true);
    
    innerEnter();
    //分享开关
}

//******************************************************************************
// innerEnter
//******************************************************************************
void ParcelPageOne::innerEnter()
{
    if(getChildrenCount() == 0)
        return;
    
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_wujiang"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(210, 766)), 0.8f));
    
    m_JunDaiItem->setIsVisible(true);
    m_JunDaiItem->setPosition(CCPointMake(800, 730));
    m_JunDaiItem->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(425, 730)),0.8f));
    
    m_listMain->setPosition(CCPointMake(640, 0));
    m_listMain->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero),0.8f));
    
    if(CGameData::Inst()->isNewbie()){
        stUid* pTeamInf = CGameData::Inst()->getCurTeamInf();
        if(pTeamInf[0].uid[0] == 0){
            CGameData::Inst()->newbieRequest(0, 0, NULL);
            m_iReqType = enPackReqType_NewbieInf_ParcelPageOne;
            scheduleUpdate();
        }
        else{
            initNewbieArrow();
        }
        
        m_rangeBtn->setIsVisible(false);
    }
    
    //    char buf[100];
    //    snprintf(buf, 99, "naviText_general_%d", int(CCRANDOM_0_1() * 2) + 1);
    
    
    
    MainScene::Inst()->showBottomPromptInfo("naviText_general_");
    //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    setRangeTypeString();
    
    
    //    if(CGameData::Inst()->isNewbie())
    //    {
    //        if(CGameData::Inst()->getNewbieStep() == 4){
    //            CCLog("adfsadfsadfsadfsadfs");
    //        }
    //        else {
    //            CCLog("adfsadfsadfsadfsadfs789");
    //        }
    //    }
    
}

//******************************************************************************
// exit
//******************************************************************************
void ParcelPageOne::exit()
{
    innerExit();
//    if(m_curScreen == enPkSc_TeamInf_ParcelPageOne){
//        if(CGameData::Inst()->isNewbie()){
//            
//        }
//        else{
//            if (isCommitTeamInfo() && !newBool) {
//                if(CGameData::Inst()->commitTeamRequest())
//                {
//                    
//                    m_iReqType = enPackReqType_TeamChg_ParcelPageOne;
//                    scheduleUpdate();
//                }
//            }
//        }
//    }
//    
//    
//    m_rangeBtn->setPosition(ccp(-1000, -1000));
//    rangeLabel->setIsVisible(false);
//
    
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
//
//    m_curScreen = enPkSc_Null_ParcelPageOne;
}


void ParcelPageOne::visit()
{
    CCSize winsz = CCDirector::sharedDirector()->getWinSizeInPixels();
//    CCLog("%f , %f" , winsz.width , winsz.height);
    
    
    
    
    /*
     
     640 960
     
     640 1136
     
     768 1024

    
    */
    
    int gap_height = ((int)winsz.height % 480)/2;//场景上下的空隙高度
    int scene_height = (int)winsz.height - gap_height * 2;//场景实际的宽度，不包括上下无用的边框
    float scissor_y_ratio = 97.0/480;
    float scissor_height_ratio = 263.0/480;
    
    glEnable(GL_SCISSOR_TEST);
    glScissor(0,
              scene_height*scissor_y_ratio+gap_height,
              CCDirector::sharedDirector()->getWinSizeInPixels().width,
              scissor_height_ratio*scene_height);//x, y, w, h
    CCLayer::visit();//调用自己的类方法
    glDisable(GL_SCISSOR_TEST);
    
//    if(winsz.height == 1136)
//    {
//        glEnable(GL_SCISSOR_TEST);
//        glScissor(0,
//                  (CCDirector::sharedDirector()->getWinSizeInPixels().height- OcProxy::Inst()->deltaHeightOf568()*2)*132.0/480.0,//shikun_ios5
//                  
//                  CCDirector::sharedDirector()->getWinSizeInPixels().width,
//                  
//                  ((250.0)/480.0*(CCDirector::sharedDirector()->getWinSizeInPixels().height-OcProxy::Inst()->deltaHeightOf568()*2)));//x, y, w, h
//        CCLayer::visit();//调用自己的类方法
//        glDisable(GL_SCISSOR_TEST);
//    }
//    else if(winsz.height == 2048)
//    {
//        glEnable(GL_SCISSOR_TEST);
//        glScissor(0,
//                  (CCDirector::sharedDirector()->getWinSizeInPixels().height- OcProxy::Inst()->deltaHeightOf568()*2)*132.0/480.0 - 120,//
//                  
//                  CCDirector::sharedDirector()->getWinSizeInPixels().width,
//                  
//                  ((250.0)/480.0*(CCDirector::sharedDirector()->getWinSizeInPixels().height-OcProxy::Inst()->deltaHeightOf568()*2)));//x, y, w, h
//        CCLayer::visit();//调用自己的类方法
//        glDisable(GL_SCISSOR_TEST);
//
//    }
//    else
//    {
//        glEnable(GL_SCISSOR_TEST);
//        glScissor(0,
//                  (CCDirector::sharedDirector()->getWinSizeInPixels().height- OcProxy::Inst()->deltaHeightOf568()*2)*132.0/480.0 - 60,//
//                  
//                  CCDirector::sharedDirector()->getWinSizeInPixels().width,
//                  
//                  ((250.0)/480.0*(CCDirector::sharedDirector()->getWinSizeInPixels().height-OcProxy::Inst()->deltaHeightOf568()*2)));//x, y, w, h
//        CCLayer::visit();//调用自己的类方法
//        glDisable(GL_SCISSOR_TEST);
//    }
    

    //CCLog("height %f",(250.0)/480.0*CCDirector::sharedDirector()->getWinSize().height);
}



//******************************************************************************
// innerExit
//******************************************************************************
void ParcelPageOne::innerExit()
{
//    m_spTitle->stopAllActions();
//    m_listMain->stopAllActions();
//    m_rangeBtn->stopAllActions();
//    m_miBack->stopAllActions();
//    m_JunDaiItem->stopAllActions();
//    m_JunDaiItem->setIsVisible(false);
//    
//    m_miBack->setPosition(CCPointMake(-1000, -1000));
//    m_spTitle->setPosition(CCPointMake(-1000, -1000));
//    m_listMain->setPosition(CCPointMake(-1000, -1000));
//    m_rangeBtn->setPosition(ccp(-1000, -1000));
//    rangeLabel->setIsVisible(false);
//    
//    if(m_package != NULL){
//        //推出前 保存m_package Y值
//        //保存到不同的iLookY中
//        if (m_package) {
//            arraypackageY[iLookY] = m_package->getPosition().y;
//        }
//        removeChild(m_package, true);
//        m_package = NULL;
//    }
//    
//    
//    if(m_teamInf != NULL){
//        removeChild(m_teamInf, true);
//        m_teamInf = NULL;
//    }
//    
//    if (m_pTeamInfoScrollViewLayer)
//    {
//        removeChild(m_pTeamInfoScrollViewLayer,true);
//        m_pTeamInfoScrollViewLayer = NULL;
//    }
//    
//    if(m_pwUpBase != NULL){
//        removeChild(m_pwUpBase, true);
//        m_pwUpBase = NULL;
//    }
//    
//    if(m_evolution != NULL){
//        removeChild(m_evolution, true);
//        m_evolution = NULL;
//    }
//    
//    
//    if(m_evoOvrShow != NULL){
//        removeChild(m_evoOvrShow, true);
//        m_evoOvrShow = NULL;
//    }
//    
//    if (m_itemCardLayer != NULL) {
//        m_itemCardLayer->removeAllChildrenWithCleanup(true);
//        m_itemCardLayer->removeFromParentAndCleanup(true);
//        m_itemCardLayer = NULL;
//    }
//    
//    if (m_SortBtnLayer) {
//        m_SortBtnLayer->stopAllActions();
//        removeChild(m_SortBtnLayer, true);
//        m_SortBtnLayer = NULL;
//    }
    
    unschedule(schedule_selector(ParcelPageOne::inertiaMove));
    
    MainScene::Inst()->m_pParcelBackPage->exit();
    
    if(MainScene::Inst()->m_pParcelPageTwo != NULL)
    MainScene::Inst()->m_pParcelPageTwo->exit();

    
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}


//******************************************************************************
// cbList
//******************************************************************************
void ParcelPageOne::cbList(CCObject* pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if(CGameData::Inst()->isNewbie()){
        if((pRet->iBtnSel == 0) && ((CGameData::Inst()->getNewbieStep() == 4))){
            CCNode* pArrow = getChildByTag(kGeneralArrowTag);
            if(pArrow)
                removeChild(pArrow, true);
            
            enterTeamInf();
        }
        if((pRet->iBtnSel == 1) && ((CGameData::Inst()->getNewbieStep() == 5))){
            CCNode* pArrow = getChildByTag(kGeneralArrowTag);
            if(pArrow)
                removeChild(pArrow, true);
            
            enterPwUpSel();
        }
        else {
            return;
        }
    }
    
    
    if(MainScene::Inst()->new_CardAdd && pRet->iBtnSel != 4)
        return;
    else
    {
        Arrow * temp = (Arrow *)MainScene::Inst()->getChildByTag(98997);
        if(temp)
            temp->removeFromParentAndCleanup(true);
        
        
    }
    
    switch (pRet->iBtnSel) {
        case 0:
            saveTeamInfo();
            enterTeamInf();
            break;
        case 1:
            enterPwUpSel();
            break;
        case 2:
            enterEvoSel();
            break;
        case 3:
            enterCardSell();
            break;
        case 4:{
            m_curScreen = enPkSc_PackInf_ParcelPageOne;
            m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_box_title"));
            enterPackage(enPkLkTp_JustLook);
            //            char buf[100];
            //            snprintf(buf, 99, "naviText_package_%d", int(CCRANDOM_0_1() * 3) + 1);
            
            
            MainScene::Inst()->showBottomPromptInfo("naviText_package_");
            //    MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
        }
            break;
        default:
            break;
    }
}

//******************************************************************************
// cbItemCardLayer
//******************************************************************************

void ParcelPageOne::cbItemCardLayer(CCObject *pSender)
{
    ItemCarSelRet* ret = (ItemCarSelRet*)pSender;
    
    switch (ret->iActType)
    {
        case enMoveTitleLeave:
            m_spTitle->setPosition(ccp(-1000, -1000));
            m_miBack->setPosition(ccp(-1000, -1000));
            break;
            
        case enMoveTitelHere:
            m_spTitle->setPosition(ccp(320, 766));
            m_miBack->setPosition(ccp(1, 731));
            break;
            
        case enItmCardLyQuit:
            if (m_bFromHome)
                MainScene::Inst()->GoToHomePage();
            else
            {
                innerExit();
                innerEnter();
            }
            break;
            
        default:
            break;
    }
}

//******************************************************************************
// showItemCard
//******************************************************************************

void ParcelPageOne::showChildCard()
{
    
    CCNode * tempN = this->getChildByTag(999333);
    if(tempN)
        tempN ->removeFromParentAndCleanup(true);
    
    
    CCNode * tempN2 = this->getChildByTag(999444);
    if(tempN2)
        tempN2 ->removeFromParentAndCleanup(true);
    
    fDownLimitY = 192.0;
    m_iPreThisPosY = 0;
    
    CCSprite* tempSp = CCSprite::spriteWithFile("itemcard1.png");
    m_cardSize = tempSp->getContentSize();
    m_iColumn = 640 / m_cardSize.width;//个数
    m_iGap = (640 - m_iColumn * m_cardSize.width) / (m_iColumn + 1) - 10;//间距
    
    m_ptStart.x = m_iGap + 30;//初始位置
    m_ptStart.y = 720 - 10;
    
    CCPoint pt = ccp(0,0);
    
    CCMutableArray<ParcelChildInfo* > * parcelChildInfoArr =  CGameData::Inst()->getParcelChildInfoAry();
    
    for (int i = 0 ; i < parcelChildInfoArr->count(); i ++) //i < itemCardInf->count()
    {
        
        ParcelChildInfo * tempPar = parcelChildInfoArr->getObjectAtIndex(i);
        
        
        short tempSht = atoi(tempPar->targetId.c_str());
        
        CCardBaseInfo * tempBaseInfo = (CCardBaseInfo*)CGameData::Inst()->getCardBaseInfByCid(tempSht);
        
        
        CCSprite* tempItem = CGameData::Inst()->getHeadSprite(tempBaseInfo->cid);//头像显示
        
        
        

        
        addChild(tempItem,50,ITEMCARDDELTA+i);
        
        pt.x = m_ptStart.x + ((i ) % m_iColumn) * (m_iGap + m_cardSize.width);
        pt.y = m_ptStart.y - ((i ) / m_iColumn) * (m_iGap + m_cardSize.height);
        tempItem->setAnchorPoint(ccp(0,1));
        tempItem->setPosition(pt);
        
        
        
        
        unsigned long ulCurTime = CGameData::Inst()->getCurrentTime();
        
        long lTimeOffset = tempPar->end_time - ulCurTime;
        
        
        if(tempPar -> longTime)
        {
            
        }
        else
        {
            CCSprite * tempItem1 = CCSprite::spriteWithFile("parcal_timeBack.png");
            tempItem1->setPosition(CCPointMake(53, 20));
            tempItem->addChild(tempItem1,1);

            
            if(lTimeOffset < 0)
                lTimeOffset = 0;
            
            int hour = lTimeOffset / 3600;
            int minute = lTimeOffset / 60;
            int day = lTimeOffset / (3600*24);
            
            char tBuf[100];
            
            if(hour >=24)
            {
                snprintf(tBuf, 99, "%s%d%s",CGameData::Inst()->getLanguageValue("Parcel_sheng"), day, CGameData::Inst()->getLanguageValue("left_day"));
            }
            else if(hour > 0&&hour<24)
                snprintf(tBuf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("Parcel_sheng"), hour, CGameData::Inst()->getLanguageValue("left_hour"));
            else
                snprintf(tBuf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("Parcel_sheng"), minute, CGameData::Inst()->getLanguageValue("left_minute"));
            
            
            TextNode* subText2 = TextNode::textWithString(tBuf, 20);
            subText2->setPosition(CCPointMake(95, 10));
            subText2->setShadowColor(ccBLACK);
            subText2->setAnchorPoint(ccp(1, 0));
            tempItem->addChild(subText2,3);
        }
        
        
        
        

        

        
        
    }
    
    
    
    m_fUpLimitY = fDownLimitY - (pt.y - m_cardSize.height);
    
    if (parcelChildInfoArr->count() <= 0)
    {
        m_fUpLimitY = -100.0f;
        
        CCSprite* m_background = CCSprite::spriteWithFile("fr_window.png");
        m_background->setScale(2.0);
        addChild(m_background);
        m_background->setTag(999333);
        m_background->setPosition(ccp(320, 520));
        
        TextNode* tText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("Parcel_noItem"), 30);
        tText->setColor(ccc3(122, 37, 8));
        addChild(tText);
        tText->setTag(999444);
        tText->setPosition(ccp(320, 570));
        
//        CCSprite* button = CCSprite::spriteWithSpriteFrameName("fr_button.png");
//        addChild(button,1,6789);
//        button->setPosition(ccp(320, 432));
//        
//        TextNode* tText2 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"), kBtnTitleHeight);
//        button->addChild(tText2);
//        tText2->setPosition(ccp(button->getContentSize().width/2-1, button->getContentSize().height/2+2));
//        tText2->setColor(ccWHITE);
//        tText2->setShadowColor(ccBLACK);
    }
    
    if(m_fUpLimitY > 0)
    {
        CScrollBar*     pScrollBar = new CScrollBar();
        addChild(pScrollBar , 9988);
        pScrollBar->setTag(9988);
        
        float fBarSize = 1.0 / ((m_fUpLimitY / (720 - 192)) + 2);
        pScrollBar->setBarSize(fBarSize);
        pScrollBar->setPercent(0.0);
        
        pScrollBar->setPosition(CCPointMake(630, 435 + 30));
        
        
//        m_pScrollBar = new CScrollBar();
//        addChild(m_pScrollBar);
//        m_pScrollBar->release();
//        
//        float fBarSize = 1.0 / ((m_fUpLimitY / (720 - 192)) + 2);
//        m_pScrollBar->setBarSize(fBarSize);
//        m_pScrollBar->setPercent(0.0);
//        
//        m_pScrollBar->setPosition(CCPointMake(630, 435 + 30));
        

    }
    
    
    m_lastYm = 0;
    
    
    return;
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("ttl_suishenjundai"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    if(m_itemCardLayer == NULL)
    {
        m_itemCardLayer = new ItemCardLayer(this,callfuncO_selector(ParcelPageOne::cbItemCardLayer));
        addChild(m_itemCardLayer);
        m_itemCardLayer->release();
        
        m_itemCardLayer->setPosition(CCPoint(640, 0));
        CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 0));
        m_itemCardLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
    
}



void ParcelPageOne::showItemCard()
{
    
    
    CCNode * tempN = this->getChildByTag(999333);
    if(tempN)
        tempN ->removeFromParentAndCleanup(true);
    
    
    CCNode * tempN2 = this->getChildByTag(999444);
    if(tempN2)
        tempN2 ->removeFromParentAndCleanup(true);
    
    
    
    fDownLimitY = 192.0;
    m_iPreThisPosY = 0;
    
    CCSprite* tempSp = CCSprite::spriteWithFile("itemcard1.png");
    m_cardSize = tempSp->getContentSize();
    m_iColumn = 640 / m_cardSize.width;//个数
    m_iGap = (640 - m_iColumn * m_cardSize.width) / (m_iColumn + 1) - 10;//间距
    
    m_ptStart.x = m_iGap + 30;//初始位置
    m_ptStart.y = 720 - 10;
    
    CCPoint pt = ccp(0,0);
    
    CCMutableArray<ParcelItemInfo* > * parcelItemInfoArr =  CGameData::Inst()->getParcelItemInfoAry();
    
    for (int i = 0 ; i < parcelItemInfoArr->count(); i ++) //i < itemCardInf->count()
    {
        
        ParcelItemInfo * tempPar = parcelItemInfoArr->getObjectAtIndex(i);
        
        
        ItemCardInf * tempBaseInfo = (ItemCardInf*)CGameData::Inst()->getItemCardByID(tempPar->targetId);
        
        CCSprite* tempItem = CCSprite::spriteWithFile(tempBaseInfo->strResName.c_str());//头像显示
        


        addChild(tempItem,50,ITEMCARDDELTA+i);
        
        pt.x = m_ptStart.x + ((i ) % m_iColumn) * (m_iGap + m_cardSize.width);
        pt.y = m_ptStart.y - ((i ) / m_iColumn) * (m_iGap + m_cardSize.height);
        tempItem->setAnchorPoint(ccp(0,1));
        tempItem->setPosition(pt);
        
        
        
        
        
        unsigned long ulCurTime = CGameData::Inst()->getCurrentTime();
        
        long lTimeOffset = tempPar->end_time - ulCurTime;
        
        if(tempPar -> longTime)
        {
            
        }
        else
        {
            CCSprite * tempItem1 = CCSprite::spriteWithFile("parcal_timeBack.png");
            tempItem1->setPosition(CCPointMake(53, 20));
            tempItem->addChild(tempItem1,1);
            
            if(lTimeOffset < 0)
                lTimeOffset = 0;
            
            int hour = lTimeOffset / 3600;
            int minute = lTimeOffset / 60;
            int day = lTimeOffset / (3600*24);
            
            char tBuf[100];
            
            if(hour >=24)
            {
                snprintf(tBuf, 99, "%s%d%s",CGameData::Inst()->getLanguageValue("Parcel_sheng"), day, CGameData::Inst()->getLanguageValue("left_day"));
            }
            else if(hour > 0&&hour<24)
                snprintf(tBuf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("Parcel_sheng"), hour, CGameData::Inst()->getLanguageValue("left_hour"));
            else
                snprintf(tBuf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("Parcel_sheng"), minute, CGameData::Inst()->getLanguageValue("left_minute"));
            
            
            
            
            TextNode* subText2 = TextNode::textWithString(tBuf, 20);
            subText2->setPosition(CCPointMake(95, 10));
            subText2->setShadowColor(ccBLACK);
            subText2->setAnchorPoint(ccp(1, 0));
            tempItem->addChild(subText2,3);
        }
        
        
        
        
    }
    
    
    
    m_fUpLimitY = fDownLimitY - (pt.y - m_cardSize.height);
    
    if (parcelItemInfoArr->count() <= 0)
    {
        m_fUpLimitY = -100.0f;
        
        CCSprite* m_background = CCSprite::spriteWithFile("fr_window.png");
        m_background->setScale(2.0);
        addChild(m_background);
        m_background->setTag(999333);
        m_background->setPosition(ccp(320, 520));
        
        TextNode* tText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("Parcel_noItem"), 30);
        tText->setColor(ccc3(122, 37, 8));
        addChild(tText);
        tText->setTag(999444);
        tText->setPosition(ccp(320, 570));
        
//        CCSprite* button = CCSprite::spriteWithSpriteFrameName("fr_button.png");
//        addChild(button,1,6789);
//        button->setPosition(ccp(320, 432));
//        
//        TextNode* tText2 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"), kBtnTitleHeight);
//        button->addChild(tText2);
//        tText2->setPosition(ccp(button->getContentSize().width/2-1, button->getContentSize().height/2+2));
//        tText2->setColor(ccWHITE);
//        tText2->setShadowColor(ccBLACK);
    }
    
    if(m_fUpLimitY > 0)
    {
        
        CScrollBar*     pScrollBar = new CScrollBar();
        addChild(pScrollBar , 9988);
        pScrollBar->setTag(9988);
        
        float fBarSize = 1.0 / ((m_fUpLimitY / (720 - 192)) + 2);
        pScrollBar->setBarSize(fBarSize);
        pScrollBar->setPercent(0.0);
        
        pScrollBar->setPosition(CCPointMake(630, 435 + 30));
        
        
//        m_pScrollBar = new CScrollBar();
//        addChild(m_pScrollBar);
//        m_pScrollBar->release();
//        
//        float fBarSize = 1.0 / ((m_fUpLimitY / (720 - 192)) + 2);
//        m_pScrollBar->setBarSize(fBarSize);
//        m_pScrollBar->setPercent(0.0);
//        
//        m_pScrollBar->setPosition(CCPointMake(630, 435 + 30));
        
        
    }
    
    
    m_lastYm = 0;
    
    
    return;
    
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("ttl_suishenjundai"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    if(m_itemCardLayer == NULL)
    {
        m_itemCardLayer = new ItemCardLayer(this,callfuncO_selector(ParcelPageOne::cbItemCardLayer));
        addChild(m_itemCardLayer);
        m_itemCardLayer->release();
        
        m_itemCardLayer->setPosition(CCPoint(640, 0));
        CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 0));
        m_itemCardLayer->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
    
}




//******************************************************************************
// enterPackage
//******************************************************************************
void ParcelPageOne::enterPackage(int iLookTp)
{
    
    innerExit();
    m_lbRangeTitle->setString(CGameData::Inst()->getLanguageValue("range_title"));
    m_lbRangeTitleBg->setString(CGameData::Inst()->getLanguageValue("range_title"));
    m_rangeBtn->setTag(enPkSc_PackInf_ParcelPageOne);
    m_rangeBtn->setPosition(ccp(1000, 740));
    m_rangeBtn->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(530, 730)),0.8f));
    
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    if(m_package == NULL){
        int costCanUse = 0;
        if (iLookTp == enPkLkTp_TeamEditLeader || iLookTp == enPkLkTp_TeamEditSub) {
            int costMax = CGameData::Inst()->getUsrInfo()->cost;
            int costTeam = CGameData::Inst()->getCurTeamCost();
            int costSel = (CGameData::Inst()->getCurTeamInf())[m_iTeamMemberSel].cost;
            costCanUse = costMax - (costTeam - costSel);
        }
        m_package = new Package(iLookTp, this, callfuncO_selector(ParcelPageOne::cbPackage), costCanUse);
        addChild(m_package);
        m_package->release();
        rangeLabel->setIsVisible(true);
    }
    //CCLOG("------%d--%d",arraypackageY[iLookTp],iLookTp);
    iLookY=iLookTp;
    //不同进入iLookTp的位置不同
    if (m_package){
        m_package->setThisPosition(CCPointMake(640, arraypackageY[iLookTp]),true);
    }
    
    m_package->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, m_package->getPosition().y)), 0.8f));
}


//******************************************************************************
// enterTeamInf
//******************************************************************************
void ParcelPageOne::enterTeamInf()
{
    innerExit();
    m_lbRangeTitle->setString(CGameData::Inst()->getLanguageValue("teamInf_reset"));
    m_lbRangeTitleBg->setString(CGameData::Inst()->getLanguageValue("teamInf_reset"));
    m_rangeBtn->setTag(enPkSc_TeamInf_ParcelPageOne);
    m_rangeBtn->setPosition(ccp(1000, 740));
    m_rangeBtn->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(530, 730)),0.8f));
    
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_biandui_title"));
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    if (!m_pTeamInfoScrollViewLayer)
    {
        m_pTeamInfoScrollViewLayer = CTeamInfoScrollViewLayer::create(this,callfuncO_selector(ParcelPageOne::cbTeamInf));
        m_pTeamInfoScrollViewLayer->show(CGameData::Inst()->getCurTeamIdx());
        addChild(m_pTeamInfoScrollViewLayer,101);
        m_pTeamInfoScrollViewLayer->release();
    }
    
    if(CGameData::Inst()->isNewbie() == false){
        int totalCost = CGameData::Inst()->getCurTeamCost();
        if(totalCost > CGameData::Inst()->getUsrInfo()->cost)
        {
            //            MainScene::Inst()->showBottomPanel(true);
            //            MainScene::Inst()->enableBottomPanel(false);
        }
        else {
            MainScene::Inst()->enableBottomPanel(true);
        }
    }
    
    m_pTeamInfoScrollViewLayer->setPosition(CCPointMake(640, 0));
    m_pTeamInfoScrollViewLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero), 0.8f));
    
    m_curScreen = enPkSc_TeamInf_ParcelPageOne;
    //    char buf[100];
    //    snprintf(buf, 99, "naviText_teamInfo_%d", int(CCRANDOM_0_1() * 3) + 1);
    
    
    MainScene::Inst()->showBottomPromptInfo("naviText_teamInfo_");
    //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}

//******************************************************************************
// isCommitTeamInfo
//******************************************************************************
bool ParcelPageOne::isCommitTeamInfo()
{
    if (m_PreCurTeamIdx != CGameData::Inst()->getCurTeamIdx())
    {
        return true;
    }
    
    stUid* pTeamInf = CGameData::Inst()->getCurTeamInf();
    for (int i = 0; i < 5; i++) {
        if (strncmp(m_teamInfoUId[i].uid, pTeamInf[i].uid, 99) != 0)
        {
            return true;
        }
    }
    
    return false;
}

//******************************************************************************
// saveTeamInfo
//******************************************************************************
void ParcelPageOne::saveTeamInfo() {
    stUid* pTeamInf = CGameData::Inst()->getCurTeamInf();
    for (int i = 0; i < 5; i++) {
        memcpy(m_teamInfoUId[i].uid, pTeamInf[i].uid, 99);
    }
    
    m_PreCurTeamIdx = CGameData::Inst()->getCurTeamIdx();
}

//******************************************************************************
// enterPwUp
//******************************************************************************
void ParcelPageOne::enterPwUpSel()
{
    
    if(CGameData::Inst()->getUsrInfo()->card_update == 0)
    {
        //引导
        //    87  183
        
        if(CGameData::Inst()->newGiveWuJiangRequest("1"))
        {
            
            m_iReqType = enPackReqType_GiveWuJiang_ParcelPageOne;
            scheduleUpdate();
        }
    }
    else
    {
        
        m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_basecard"));
        
        m_curScreen = enPkSc_PwUpBaseSel_ParcelPageOne;
        
        m_bNiePan = false;
        
        enterPackage(enPkLkTp_PwUpBase);
        //    char buf[100];
        //    snprintf(buf, 99, "naviText_pwUpSel_%d", int(CCRANDOM_0_1() * 2) + 1);
        
        
        
        MainScene::Inst()->showBottomPromptInfo("naviText_pwUpSel_");
        //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
        
        CGameData::Inst()->clearCombineQueue();
    }
    
    
    
}

//******************************************************************************
// enterPwUpBase
//******************************************************************************
void ParcelPageOne::enterPwUpBase()
{
    innerExit();
    
    m_curScreen = enPkSc_PwUpBase_ParcelPageOne;
    
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    if(m_pwUpBase == NULL){
        m_pwUpBase = new PwUpBase(this, callfuncO_selector(ParcelPageOne::cbPwUpBase), m_bNiePan);
        addChild(m_pwUpBase);
        m_pwUpBase->release();
    }
    m_pwUpBase->setPosition(CCPointMake(640, 0));
    m_pwUpBase->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero), 0.8f));
    
    
    if (m_bNiePan)
    {
        m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_niepan_png"));
        MainScene::Inst()->showBottomPromptInfo("naviText_niepan_base_1");
        //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    }
    else
    {
        m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_qianghua_title"));
        //        char buf[100];
        //        snprintf(buf, 99, "naviText_pwUpBase_%d", int(CCRANDOM_0_1() * 3) + 1);
        
        
        
        MainScene::Inst()->showBottomPromptInfo("naviText_pwUpBase_");
        //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    }
}

//******************************************************************************
// enterNiePanFood
//******************************************************************************
void ParcelPageOne::enterNiePanFood()
{
    m_curScreen = enPkSc_PwUpFood_ParcelPageOne;
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_materialchose"));
    
    enterPackage(enPkLkTp_NiePanFood);
    
    MainScene::Inst()->showBottomPromptInfo("naviText_niepan_food_1");
    //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}

//******************************************************************************
// enterPwUpFood
//******************************************************************************
void ParcelPageOne::enterPwUpFood()
{
    m_curScreen = enPkSc_PwUpFood_ParcelPageOne;
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_materialchose"));
    
    enterPackage(enPkLkTp_PwUpFood);
    //    char buf[100];
    //    snprintf(buf, 99, "naviText_pwUpFood_%d", int(CCRANDOM_0_1() * 3) + 1);
    
    
    
    MainScene::Inst()->showBottomPromptInfo("naviText_pwUpFood_");
    //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}

//******************************************************************************
// showNiePanEffect
//******************************************************************************
void ParcelPageOne::showNiePanEffect()
{
    CGameData::Inst()->addBigImageByRGBA4444("exchange.plist");
    CGameData::Inst()->addBigImageByRGBA4444("niepan.plist");
    
    //白色的光效   图层40
    CCSprite* sp = CCSprite::spriteWithFile("evolutionlight.png");
    CCActionInterval* action = NULL;
    if(sp != NULL){
        sp->setScale(1);
        sp->setPosition(CCPointMake(320, 480));
        addChild(sp, 40, kGeneralNiePanTag);
        sp->setOpacity(0);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.6),
                                          CCSpawn::actions(CCFadeIn::actionWithDuration(0.3), CCScaleTo::actionWithDuration(0.6, 5), NULL),
                                          
                                          CCDelayTime::actionWithDuration(0.2),
                                          CCFadeOut::actionWithDuration(0.6),
                                          NULL));
    }
    
    
    // 图层30 29 28 31  紫色的一条一条的
    
    //  图层21  4个分开的紫色的火焰
    
    // 图层20 紫色的球？ 带闪电???
    
    // 38 紫色的圈 变小(同时自转)
    
    // 37 紫色的圈 变大(同时自转)  37开始的大小比38的开始大小要小
    
    // 9  白色的很淡的圈 然后 周围的星星
    
    // 10 白色的很淡的圈 （和 9 累死）
    
    // 11  地图的人的变化
    
    // 11,20,3837 其余的打包niepan里
    action = getActionFromPlist("niepana00%02d.png");
    
    sp = CCSprite::spriteWithFile("evolutionlight.png");   // 回叠加起来的
    if(sp != NULL){
        sp->setPosition(CCPointMake(320, 480));
        sp->setScale(2);
        addChild(sp, 8, kGeneralNiePanTag);
        sp->runAction(action);
    }
    
    // 变大的圈
    sp = CCSprite::spriteWithSpriteFrameName("ziquan.png");
    if(sp)
    {
        sp->setScale(1.2);
        sp->setPosition(CCPointMake(320, 480));
        addChild(sp, 37, kGeneralNiePanTag);
        
        CCScaleTo* scale_big = CCScaleTo::actionWithDuration(1.f, 2.2);
        CCRotateBy* rotate_to = CCRotateBy::actionWithDuration(1.f, 180);
        CCFiniteTimeAction* action_rotate_scale = CCSpawn::actions(scale_big, rotate_to, NULL);
        CCFadeIn*    fade_in = CCFadeIn::actionWithDuration(0.1);
        CCFadeTo*   f1 = CCFadeTo::actionWithDuration(0.1, 200);
        CCFadeTo*   f2 = CCFadeTo::actionWithDuration(0.7, 150);
        CCFadeTo*   fade_out = CCFadeTo::actionWithDuration(0.1, 0);
        CCFiniteTimeAction* action_fade_in_out = CCSequence::actions(fade_in, f1, f2, fade_out, NULL);
        
        sp -> runAction(CCSpawn::actions(action_rotate_scale, action_fade_in_out,NULL));
    }
    
    // 变小的圈
    sp = CCSprite::spriteWithSpriteFrameName("ziquan.png");
    if(sp)
    {
        sp->setScale(2.2);
        sp->setPosition(CCPointMake(320, 480));
        addChild(sp, 37, kGeneralNiePanTag);
        
        CCScaleTo* scale_big = CCScaleTo::actionWithDuration(1.f, 1.2);
        CCRotateBy* rotate_to = CCRotateBy::actionWithDuration(1.f, 180);
        CCFiniteTimeAction* action_rotate_scale = CCSpawn::actions(scale_big, rotate_to, NULL);
        CCFadeIn*    fade_in = CCFadeIn::actionWithDuration(0.1);
        CCFadeTo*   f1 = CCFadeTo::actionWithDuration(0.1, 200);
        CCFadeTo*   f2 = CCFadeTo::actionWithDuration(0.7, 150);
        CCFadeTo*   fade_out = CCFadeTo::actionWithDuration(0.1, 0);
        CCFiniteTimeAction* action_fade_in_out = CCSequence::actions(fade_in, f1, f2, fade_out, NULL);
        
        sp -> runAction(CCSpawn::actions(action_rotate_scale, action_fade_in_out,NULL));
    }
    
    // 图层22  黄色的 往中间收
    sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    action = getActionFromPlist("goldlight00%02d.png", true);
    if (sp)
    {
        sp->setScale(2);
        sp->setPosition(CCPointMake(320, 450));
        addChild(sp, 22, kGeneralNiePanTag);
        CCDelayTime* delay_time = CCDelayTime::actionWithDuration(0.4f);
        sp->runAction(CCSequence::actions(delay_time, action, NULL));
    }
    
    // 4个火焰
    float fire_delay = 0.2;
    action = getActionFromPlist("zihuo00%02d.png");
    sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    if (sp)
    {
        sp->setScale(2);
        sp->setPosition(CCPointMake(320, 350));  //下
        addChild(sp, 21, kGeneralNiePanTag);
        CCDelayTime* delay_time = CCDelayTime::actionWithDuration(fire_delay);
        sp->runAction(CCSequence::actions( delay_time, action, NULL));
    }
    sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    action = getActionFromPlist("zihuo00%02d.png");
    if (sp)
    {
        sp->setScale(2);
        sp->setPosition(CCPointMake(320, 740));  // 上
        addChild(sp, 21, kGeneralNiePanTag);
        CCDelayTime* delay_time = CCDelayTime::actionWithDuration(fire_delay);
        sp->runAction(CCSequence::actions( delay_time, action, NULL));
    }
    sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    action = getActionFromPlist("zihuo00%02d.png");
    if (sp)
    {
        sp->setScale(2);
        sp->setPosition(CCPointMake(520, 540));  // 右
        addChild(sp, 21, kGeneralNiePanTag);
        CCDelayTime* delay_time = CCDelayTime::actionWithDuration(fire_delay);
        sp->runAction(CCSequence::actions( delay_time, action, NULL));
    }
    sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    action = getActionFromPlist("zihuo00%02d.png");
    if (sp)
    {
        sp->setScale(2);
        sp->setPosition(CCPointMake(120, 540));   // 左
        addChild(sp, 21, kGeneralNiePanTag);
        CCDelayTime* delay_time = CCDelayTime::actionWithDuration(fire_delay);
        sp->runAction(CCSequence::actions( delay_time, action, NULL));
    }
    
    // 星星
    sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    action = getActionFromPlist("star00%02d.png");
    if (sp)
    {
        sp->setScale(2);
        sp->setPosition(CCPointMake(320, 480));
        addChild(sp, 9, kGeneralNiePanTag);
        CCDelayTime* delay_time = CCDelayTime::actionWithDuration(2.f);
        sp->runAction(CCSequence::actions(delay_time, action, NULL));
    }
    
    // 武将变身
    sp = CGameData::Inst()->getBodySprite(m_NiePanBaseCid);
    if (sp)
    {
        sp -> setPosition(CCPointMake(320, 480));
        addChild(sp, 0, kGeneralNiePanTag);
        sp -> runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.4),
                                            CCHide::action(), NULL));
    }
    
    const char* ucidTarget = CGameData::Inst()->getNiePanResultUcid();
    CGameData::Inst()->setCombineQueue(0, (char*)ucidTarget);
    CUserCardInfo* pTargetCard = CGameData::Inst()->getUsrCardInfByUcid(ucidTarget);
    sp = CGameData::Inst()->getBodySprite(pTargetCard->cid);
    if (sp)
    {
        sp -> setOpacity(0);
        sp -> setPosition(CCPointMake(320, 480));
        addChild(sp, 0, kGeneralNiePanTag);
        sp -> runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.4),
                                            CCFadeIn::actionWithDuration(0.1),
                                            CCDelayTime::actionWithDuration(1.5),
                                            CCCallFunc::actionWithTarget(this, callfunc_selector(ParcelPageOne::enterPwUpOver)),
                                            NULL));
    }
    
}

// ------------ reference ------------
CCActionInterval* ParcelPageOne::getActionFromPlist(const char* filename)
{
    return this->getActionFromPlist(filename, false);
}
CCActionInterval* ParcelPageOne::getActionFromPlist(const char* filename, bool isReverse)
{
    char buf[100];
    CCAnimation* animation;
	CCActionInterval*  action;
    CCSpriteFrame* frame;
    CCArray* arr_frames = CCArray::array();
    
    animation = CCAnimation::animation();
    int i = 1;
    while(1) {
        snprintf(buf, 99, filename, i);
        
        CCSprite* tmpSP = CCSprite::spriteWithSpriteFrameName(buf);
        if (tmpSP){
            frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buf);
            animation->addFrame(frame);
            arr_frames->addObject(frame);
        }
        else
            break;
        i++;
    }
    
    if (isReverse)
    {
        animation = CCAnimation::animation();
        while (arr_frames->count() > 0)
        {
            animation->addFrame((CCSpriteFrame*)arr_frames->lastObject());
            arr_frames->removeLastObject();
        }
    }
    
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    animation->addFrame(frame);
    action = CCAnimate::actionWithDuration(i/20.f, animation, false);
    
    return action;
}

CCActionInterval* ParcelPageOne::getActionFromFile(const char* filename)
{
    char buf[100];
    CCAnimation* animation;
	CCActionInterval*  action;
    
    animation = CCAnimation::animation();
    int i = 1;
    while(1) {
        snprintf(buf, 99, filename, i);
        
        CCSprite* tmpSP = CCSprite::spriteWithFile(buf);
        if (tmpSP)
            animation->addFrameWithFileName(buf);
        else
            break;
        
        i++;
    }
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    animation->addFrame(frame);
    action = CCAnimate::actionWithDuration(i/20.f, animation, false);
    
    return action;
}

//******************************************************************************
// showPwUpEffect
//******************************************************************************
void ParcelPageOne::showPwUpEffect() {
    CGameData::Inst()->addBigImageByRGBA4444("pwUp.plist");
    
    char* ucid = CGameData::Inst()->getCombineCard(0);
    CUserCardInfo* pUsrCardInf = CGameData::Inst()->getUsrCardInfByUcid(ucid);
    
    m_pwUpCardSp = CGameData::Inst()->getHeadSprite(pUsrCardInf->cid);
    if(m_pwUpCardSp != NULL){
        m_pwUpCardSp->setPosition(CCPointMake(320, 480));
        m_pwUpCardSp->setScale(3.0);
        addChild(m_pwUpCardSp, 1);
        
        CCAnimation *pwUpAnim = CCAnimation::animation();
        char pwUpAnimName[100] = {0};
        for (int i = 1; i <= 13 ; i++) {
            sprintf(pwUpAnimName, "pwUpEffect_00%02d.png", i);
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( pwUpAnimName );
            pwUpAnim->addFrame(frame);
        }
        pwUpAnim->setDelay(0.5f / 13);
        m_pwUpAction = CCAnimate::actionWithAnimation(pwUpAnim, true);
        m_pwUpAction->retain();
        
        float scaleNum = 1.5f;
        switch (CGameData::Inst()->getPwUpSucType()) {
            case enPwupSucType_Normal:
                scaleNum = 2.5f;
                break;
            case enPwupSucType_Big:
                scaleNum = 3.0f;
                break;
            case enPwupSucType_Super:
                scaleNum = 3.5f;
                break;
                
            default:
                break;
        }
        
        
        m_pwUpCardSp->runAction(CCSequence::actions(CCEaseBounceOut::actionWithAction(CCScaleTo::actionWithDuration(0.5f, 1.5f)),
                                                    CCSpawn::actions(CCCallFunc::actionWithTarget(this, callfunc_selector(ParcelPageOne::showPwUpLightEffect)),
                                                                     CCDelayTime::actionWithDuration(0.5f),
                                                                     NULL),
                                                    CCSpawn::actions(CCSequence::actions(CCEaseExponentialIn::actionWithAction(CCScaleTo::actionWithDuration(0.5f, scaleNum)),
                                                                                         CCDelayTime::actionWithDuration(0.1f),
                                                                                         CCScaleTo::actionWithDuration(0.2f, 1.5f),
                                                                                         NULL),
                                                                     CCCallFunc::actionWithTarget(this, callfunc_selector(ParcelPageOne::hidePwUpLightEffect)),
                                                                     CCCallFunc::actionWithTarget(this, callfunc_selector(ParcelPageOne::showPwUpStars)),
                                                                     NULL),
                                                    CCSpawn::actions(CCCallFunc::actionWithTarget(this, callfunc_selector(ParcelPageOne::showPwUpType)),
                                                                     CCCallFunc::actionWithTarget(this, callfunc_selector(ParcelPageOne::hidePwUpStars)),
                                                                     NULL),
                                                    CCDelayTime::actionWithDuration(1.5f),
                                                    CCCallFunc::actionWithTarget(this, callfunc_selector(ParcelPageOne::enterPwUpOver)),
                                                    NULL));
    }
    
}

void ParcelPageOne::showPwUpLightEffect() {
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(dark));
    m_pwUpLightSp = CCSprite::spriteWithSpriteFrameName("blank.png");
    m_pwUpLightSp->setPosition(CCPointMake(320, 480));
    m_pwUpLightSp->setScale(4.0f);
    addChild(m_pwUpLightSp, 2);
    
    m_pwUpLightSp->runAction(m_pwUpAction);
    m_pwUpAction->release();
}

void ParcelPageOne::hidePwUpLightEffect() {
    //    m_pwUpLightSp->removeFromParentAndCleanup(true);
}

void ParcelPageOne::showPwUpStone() {
    m_pwUpStoneSp = CCSprite::spriteWithSpriteFrameName("pwUpStonesEffect.png");
    m_pwUpStoneSp->setPosition(CCPointMake(320, 480));
    addChild(m_pwUpStoneSp);
    
    m_pwUpStoneSp->runAction(CCSpawn::actions(CCScaleBy::actionWithDuration(0.6f, 0.3f),
                                              CCRotateBy::actionWithDuration(0.6f, -90.0f),
                                              NULL));
}

void ParcelPageOne::hidePwUpStone() {
    m_pwUpStoneSp->removeFromParentAndCleanup(true);
}

void ParcelPageOne::showPwUpType() {
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(upgrade));
    char pwUpSucTypeChar[100];
    switch (CGameData::Inst()->getPwUpSucType()) {
        case enPwupSucType_Normal:
            sprintf(pwUpSucTypeChar, "pwUpSuc_normal.png");
            m_pwUpTypeSp = CCSprite::spriteWithFile(pwUpSucTypeChar);
            break;
        case enPwupSucType_Big:
            sprintf(pwUpSucTypeChar, "pwUpSuc_big.png");
            m_pwUpTypeSp = CCSprite::spriteWithFile(pwUpSucTypeChar);
            break;
        case enPwupSucType_Super:
            sprintf(pwUpSucTypeChar, "pwUpSuc_super.png");
            m_pwUpTypeSp = CCSprite::spriteWithFile(pwUpSucTypeChar);
            break;
        default:
            break;
    }
    
    m_pwUpTypeSp->setPosition(CCPointMake(320, 700));
    m_pwUpTypeSp->setScale(0.5f);
    addChild(m_pwUpTypeSp);
    
    m_pwUpTypeSp->runAction(CCScaleTo::actionWithDuration(0.2f, 1.0f));
    
}

void ParcelPageOne::showPwUpStars() {
    m_pwUpStars = CCParticleStar::node();
    m_pwUpStars->setPosition(CCPointMake(320, 480));
    m_pwUpStars->setTexture( CCTextureCache::sharedTextureCache()->addImage("particle_stars.png"));
	addChild(m_pwUpStars);
}

void ParcelPageOne::hidePwUpStars() {
    m_pwUpStars->removeFromParentAndCleanup(true);
}


//******************************************************************************
// jdTitleBtn
//******************************************************************************
void ParcelPageOne::jdTitleBtn(CCObject* sender)
{
    if (!CGameData::Inst()->isNewbie() && !MainScene::Inst()->new_CardAdd)
    {
        innerExit();
        m_curScreen = enPkSc_JunDaiSel_ParcelPageOne;
        showItemCard();
    }
}

//******************************************************************************
// enterSuiShenJunDai
//******************************************************************************
void ParcelPageOne::enterSuiShenJunDai()
{
    m_bFromHome = true;
    innerExit();
    m_curScreen = enPkSc_JunDaiSel_ParcelPageOne;
    showItemCard();
}

void ParcelPageOne::setIsFromHome(bool isFromHome)
{
    m_bFromHome = isFromHome;
}

CCLayer * ParcelPageOne::getTeamInfoScrollViewLayer()
{
    return m_pTeamInfoScrollViewLayer;
}

//******************************************************************************
// btnCallback
//******************************************************************************
void ParcelPageOne::btnCallback(CCObject* sender)
{
    //新手引导时禁止后退
    if(!CGameData::Inst()->chkLogin())
        return;
    
    if(MainScene::Inst()->new_FirstZhuanSheng)//第一次进入转生，禁止后退
    {
        return;
    }
    
    if(MainScene::Inst()->new_FirstItem)//第一次进入军袋，禁止后退
    {
        return;
    }
    
    if(MainScene::Inst()->new_FirstPowerUp)//第一次进入强化，禁止后退
    {
        if(MainScene::Inst()->new_FirstPowerUpSeeYuXi && MainScene::Inst()->getChildByTag(96996))
        {
            MainScene::Inst()->new_FirstPowerUpSeeYuXi = false;
            MainScene::Inst()->removeChildByTag(96996, true);
            
            
            
        }
        else
            return;
    }
    
    
    CCMenuItem* mi = (CCMenuItem*) sender;
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    if(mi->getTag() == kGeneralBackBtn) {
        switch (m_curScreen) {
            case enPkSc_TeamInf_ParcelPageOne:
            {
                if (MainScene::Inst()->onCurTeamCostCall(m_pTeamInfoScrollViewLayer))
                {
                    break;
                }
                
                if (m_bFromHome)
                {
                    if (m_package && m_package->isShowScrollViewLayer())
                    {
                        m_package->goback();
                    }
                    else
                    {
                        MainScene::Inst()->GoToHomePage();
                    }
                }
                else
                {
                    if(m_package != NULL){
                        if(m_package->goback()){
                            enterTeamInf();
                        }
                    }
                    else{
                        int totalCost = CGameData::Inst()->getCurTeamCost();
                        if(totalCost > CGameData::Inst()->getUsrInfo()->cost) {
                            //                        MainScene::Inst()->showFullCostDlg();
                        }
                        else {
                            m_curScreen = enPkSc_Null_ParcelPageOne;
                            innerExit();
                            
                            if (isCommitTeamInfo())
                            {
                                if(CGameData::Inst()->commitTeamRequest()){
                                    m_iReqType = enPackReqType_TeamChg_ParcelPageOne;
                                    scheduleUpdate();
                                }
                            }
                            else{
                                innerEnter();
                            }
                            
                            
                        }
                    }
                }
                break;
            }
            case enPkSc_PackInf_ParcelPageOne:
                if(m_package != NULL){
                    if(m_package->goback()){
                        innerExit();
                        innerEnter();
                    }
                }
                break;
                
            case enPkSc_PwUpBase_ParcelPageOne:
                enterPwUpSel();
                break;
                
            case enPkSc_JunDaiSel_ParcelPageOne:
                if (m_bFromHome)
                    MainScene::Inst()->GoToHomePage();
                else
                {
                    innerExit();
                    innerEnter();
                }
                break;
                
                
            case enPkSc_PwUpBaseSel_ParcelPageOne:
                if(m_package != NULL){
                    if(m_package->goback()){
                        innerExit();
                        innerEnter();
                    }
                }
                break;
                
            case enPkSc_PwUpFood_ParcelPageOne:
                if(m_package != NULL){
                    if(m_package->goback()){
                        enterPwUpBase();
                    }
                }
                break;
            case enPkSc_PwUpOver_ParcelPageOne:
            {
                for(int i = 1; i < MaxCombineCnt; i++)
                    CGameData::Inst()->setCombineQueue(i, 0);
                m_bNiePan = false;
                enterPwUpBase();
                MainScene::Inst()->showBottomPanel(true);
            }
                break;
                
            case enPkSc_EvoSel_ParcelPageOne:
                if(m_package != NULL){
                    if(m_package->goback()){
                        innerExit();
                        innerEnter();
                    }
                }
                break;
                
            case enPkSc_Evo_ParcelPageOne:
                enterEvoSel();
                break;
                
            case enPkSc_EvoOver_ParcelPageOne:
            {
                // 更新编队中升级的武将
                CGameData::Inst()->updateTeamInfo();
                int totalCost = CGameData::Inst()->getCurTeamCost();
                if(totalCost > CGameData::Inst()->getUsrInfo()->cost)
                {
                    MainScene::Inst()->showFullCostDlg();
                }
                else {
                    MainScene::Inst()->showBottomPanel(true);
                    enterEvoSel();
                }
            }
                break;
                
            case enPkSc_Sell_ParcelPageOne:
                if(m_package != NULL){
                    if(m_package->goback()){
                        innerExit();
                        innerEnter();
                    }
                }
                break;
            default:
                break;
        }
    }
    
    
    
    
    if(MainScene::Inst()->new_FirstPowerUp)
        MainScene::Inst()->enableBottomPanel(false);
}

//******************************************************************************
// newbieAllEnd
//******************************************************************************
void ParcelPageOne::newbieEnd(CCObject* pObj)
{
    CCNode* newbieEndDlg = getChildByTag(kNewbieEndTag);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);
    
    MainScene::Inst()->reLogin();
}



//******************************************************************************
// cbTeamInf
//******************************************************************************
void ParcelPageOne::cbTeamInf(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    TeamInfRet* pRet = (TeamInfRet*)pObj;
    
    //newbie
    if(pRet->iSel == kInvalidValue){
        innerExit();
        
        
        if(CGameData::Inst()->chkLogin() == false)
        {
            if(CGameData::Inst()->isNewbie()){
                if(CGameData::Inst()->newbieRequestPwUp()){
                    scheduleUpdate();
                    m_iReqType = enPackReqType_NewbiePwUp_ParcelPageOne;
                }
            }
            
            
            return;
        }
        else
        {
            innerEnter();
        }
        
        
        
        
    }
    else
    {
        m_iTeamMemberSel = pRet->iSel;
        if (m_iTeamMemberSel>=5 && m_iTeamMemberSel<=9)
        {
            //m_teamInf->showTeamInfoList(m_iTeamMemberSel-5);
            if (m_pTeamInfoScrollViewLayer)
            {
                m_pTeamInfoScrollViewLayer->show(m_iTeamMemberSel-5);
            }
            else if (m_teamInf)
            {
                m_teamInf->showTeamInfoList(m_iTeamMemberSel-5);
            }
        }
        else
        {
            if(m_iTeamMemberSel == 0)
                enterPackage(enPkLkTp_TeamEditLeader);
            else
                enterPackage(enPkLkTp_TeamEditSub);
            
            
            //            char buf[100];
            //            snprintf(buf, 99, "naviText_selectCard_%d", int(CCRANDOM_0_1() * 3) + 1);
            
            
            
            MainScene::Inst()->showBottomPromptInfo("naviText_selectCard_");
            //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
        }
    }
}

//******************************************************************************
// cbPackage
//******************************************************************************
void ParcelPageOne::cbPackage(CCObject* pObj)
{
    CardSelRet* pRet = (CardSelRet*) pObj;
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    switch (m_curScreen) {
        case enPkSc_TeamInf_ParcelPageOne:
            CGameData::Inst()->setTeamInfo(kMaxTeamCnt, m_iTeamMemberSel, pRet->uid[0].uid);
            enterTeamInf();
            break;
            
        case enPkSc_PwUpBaseSel_ParcelPageOne:
            m_bNiePan = pRet->bNiePan;
            CGameData::Inst()->setCombineQueue(0, pRet->uid[0].uid);
            enterPwUpBase();
            
            
            //shikun temp
            
            
            break;
            
        case enPkSc_PwUpFood_ParcelPageOne:
            enterPwUpBase();
            break;
            
        case enPkSc_EvoSel_ParcelPageOne:
            CGameData::Inst()->setCombineQueue(0, pRet->uid[0].uid);
            enterEvo(pRet->type);
            break;
            
        default:
            break;
    }
}

//******************************************************************************
// beginPwUp
//******************************************************************************
void ParcelPageOne::beginPwUp() {
    innerExit();
    MainScene::Inst()->showBottomPanel(false);
    
    if(CGameData::Inst()->isNewbie()){
        if(CGameData::Inst()->newbieRequestPwUp()){
            scheduleUpdate();
            m_iReqType = enPackReqType_NewbiePwUp_ParcelPageOne;
        }
    }
    else{
        if(CGameData::Inst()->powerUpRequest()){
            scheduleUpdate();
            m_iReqType = enPackReqType_PwUp_ParcelPageOne;
        }
    }
    
    if(MainScene::Inst()->new_FirstPowerUp)
        MainScene::Inst()->showTouchTip();
    
    
    //shikun temp
}

//******************************************************************************
// beginNiePan
//******************************************************************************
void ParcelPageOne::beginNiePan()
{
    innerExit();
    MainScene::Inst()->showBottomPanel(false);
    
    if (CGameData::Inst()->niepanRequest())
    {
        char* ucidBase = CGameData::Inst()->getCombineCard(0);
        CUserCardInfo* pBaseCard = CGameData::Inst()->getUsrCardInfByUcid(ucidBase);
        m_NiePanBaseCid = pBaseCard->cid;
        m_iReqType = enPackReqType_NiePan_ParcelPageOne;
        scheduleUpdate();
    }
}

//******************************************************************************
// cbPwUpBase
//******************************************************************************
void ParcelPageOne::cbPwUpBase(CCObject* pObj)
{
    PwUpBaseRet* pRet = (PwUpBaseRet*)pObj;
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    if(pRet->iSel == 0){ //追加
        if (m_bNiePan)
            enterNiePanFood();
        else
            enterPwUpFood();
    }
    else if(pRet->iSel == 1){ //開始
        if (m_bNiePan)
            beginNiePan();
        else
            beginPwUp();
    }
    else if(pRet->iSel == 2)
    { //合成结束
        
        
        
        if(MainScene::Inst()->new_FirstPowerUpSecond)
        {
            Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(ParcelPageOne::newbie),
                                                CGameData::Inst()->getLanguageValue("qiangHua_new9"),false);
            addChild(newbieEndDlg, 500, kNewbie);
            newbieEndDlg->release();
            newbieEndDlg->setPosition(CCPointMake(320, 300));
            newbieEndDlg->setScale(0.0);
            newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
            
        }
        else
        {
            for(int i = 1; i < MaxCombineCnt; i++)
                CGameData::Inst()->setCombineQueue(i, 0);
            m_bNiePan = false;
            enterPwUpBase();
            
            if(!MainScene::Inst()->new_FirstPowerUp){
                MainScene::Inst()->showBottomPanel(true);
                MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
            }
            else
            {
                MainScene::Inst()->new_FirstPowerUpSecond = true;
                MainScene::Inst()->showBottomPanel(true);
                MainScene::Inst()->enableBottomPanel(false);
            }
        }
        
        
    }
}


//******************************************************************************
// newbie
//******************************************************************************



void ParcelPageOne::newbie2(CCObject* pObj)
{
    CCNode* newbieEndDlg = getChildByTag(kNewbie);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);
    
    MainScene::Inst()->removeChildByTag(kNewbie + 99999, true );
    
    
    
    MainScene::Inst()->new_FirstZhuanSheng = false;
    MainScene::Inst()->new_FirstZhuanSheng_Second = false;
    
    MainScene::Inst()->removeTouchTip();
    MainScene::Inst()->enableBottomPanel(true);
    
    
    
    
    MainScene::Inst()->showBottomPanel(true);
    enterEvoSel();
    
    
    
}

void ParcelPageOne::newbie(CCObject* pObj)
{
    CCNode* newbieEndDlg = getChildByTag(kNewbie);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);
    
    
    MainScene::Inst()->new_FirstPowerUp = false;
    MainScene::Inst()->new_FirstPowerUpSecond = false;
    
    MainScene::Inst()->removeTouchTip();
    MainScene::Inst()->enableBottomPanel(true);
    
    
    
    for(int i = 1; i < MaxCombineCnt; i++)
        CGameData::Inst()->setCombineQueue(i, 0);
    m_bNiePan = false;
    enterPwUpBase();
    
    MainScene::Inst()->showBottomPanel(true);
    
    
    
}


//******************************************************************************
// cbEvolution
//******************************************************************************
void ParcelPageOne::cbEvolution(CCObject* obj)
{
    
    if(MainScene::Inst()->new_FirstZhuanSheng && MainScene::Inst()->getChildByTag(95999))
    {
        MainScene::Inst()->removeChildByTag(95999, true);
    }
    
    
    EvoluTypeRet* ret = (EvoluTypeRet*)obj;
    
    innerExit();
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    
    char * ucid = CGameData::Inst()->getCombineCard(0);
    CUserCardInfo* pUsrCardInf = NULL;
    
    CCMutableArray<CUserCardInfo*> *  pCardsArr = CGameData::Inst()->getUserCards();
    
    for (int i = 0; i < pCardsArr->count(); i++) {
        pUsrCardInf = pCardsArr->getObjectAtIndex(i);
        
        if (strncmp(ucid, pUsrCardInf->ucid, 99) == 0)
            break;
        else
            pUsrCardInf = NULL;
    }
    
    if(pUsrCardInf == NULL)
        return;
    
    MainScene::Inst()->showBottomPanel(false);
    
    if(ret->iType == 1)
    {
        m_iEvoType = enEvoType_Wake;
        if(CGameData::Inst()->wakeRequest()){
            scheduleUpdate();
            m_iReqType = enPackReqType_Wake_ParcelPageOne;
        }
    }
    else if(ret->iType == 2)
    {
        m_iEvoType = enEvoType_Sevo;
        if(CGameData::Inst()->sevoRequest()){
            scheduleUpdate();
            m_iReqType = enPackReqType_SEVO_ParcelPageOne;
        }
    }
    else
    {
        m_iEvoType = enEvoType_Nml;
        if(CGameData::Inst()->evolutionRequest()){
            scheduleUpdate();
            m_iReqType = enPackReqType_Evolution_ParcelPageOne;
        }
    }
}

//******************************************************************************
// cbEvoOver
//******************************************************************************
void ParcelPageOne::cbEvoOver()
{
    if(MainScene::Inst()->new_FirstZhuanSheng && !MainScene::Inst()->new_FirstZhuanSheng_Second)
        return;
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    // 更新编队中升级的武将
    CGameData::Inst()->updateTeamInfo();
    int totalCost = CGameData::Inst()->getCurTeamCost();
    if(totalCost > CGameData::Inst()->getUsrInfo()->cost)
    {
        MainScene::Inst()->showFullCostDlg();
    }
    else {
        
        
        if(MainScene::Inst()->new_FirstZhuanSheng)
        {
            Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(ParcelPageOne::newbie2),
                                                CGameData::Inst()->getLanguageValue("zhuanSheng_new18"),false);
            addChild(newbieEndDlg, 500, kNewbie);
            newbieEndDlg->release();
            newbieEndDlg->setPosition(CCPointMake(320, 130));
            newbieEndDlg->setScale(0.0);
            newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
            
            
            Arrow* pArrow = new Arrow(30, 255);
            
            pArrow->setPosition(CCPointMake(231, 245 - 170));
            
            
            
            pArrow->setRotation(180);
            //    pArrow->setScale(0.6f);
            
            MainScene::Inst()->addChild(pArrow, 50000, kNewbie + 99999);
            
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                               CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                               NULL);
            pArrow->runAction(CCRepeatForever::actionWithAction(action));
        }
        else
        {
            MainScene::Inst()->showBottomPanel(true);
            enterEvoSel();
        }
        
    }
}

//******************************************************************************
// update
//******************************************************************************
void ParcelPageOne::enterPwUpOver()
{
    if (m_bNiePan)
    {
        while (getChildByTag(kGeneralNiePanTag)) {
            CCSprite* spNode = (CCSprite*)getChildByTag(kGeneralNiePanTag);
            spNode->removeFromParentAndCleanup(true);
        }
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("exchange.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("niepan.plist");
    }
    else
    {
        m_pwUpCardSp->removeFromParentAndCleanup(true);
        m_pwUpTypeSp->removeFromParentAndCleanup(true);
        
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("pwUp.plist");
    }
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();
    
    innerExit();
    
    m_curScreen = enPkSc_PwUpOver_ParcelPageOne;
    
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    if(m_pwUpBase == NULL){
        m_pwUpBase = new PwUpBase(this, callfuncO_selector(ParcelPageOne::cbPwUpBase), m_bNiePan, true);
        addChild(m_pwUpBase);
        m_pwUpBase->release();
    }
    m_pwUpBase->setPosition(CCPointMake(640, 0));
    m_pwUpBase->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero), 0.8f));
}


//******************************************************************************
// update
//******************************************************************************
void ParcelPageOne::enterEvoSel()
{
    
    if(CGameData::Inst()->getUsrInfo()->card_upgrade == 0 )
    {
        //引导
        
        if(CGameData::Inst()->newGiveWuJiangRequest("3"))
        {
            
            m_iReqType = enPackReqType_GiveWuJiang_ZhuanSheng_ParcelPageOne;
            scheduleUpdate();
        }
        
    }
    else
    {
        m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_basecard2"));
        m_curScreen = enPkSc_EvoSel_ParcelPageOne;
        
        CGameData::Inst()->clearCombineQueue();
        
        enterPackage(enPkLkTp_Evolution);
        //    char buf[100];
        //    snprintf(buf, 99, "naviText_evoSel_%d", int(CCRANDOM_0_1() * 3) + 1);
        
        
        MainScene::Inst()->showBottomPromptInfo("naviText_evoSel_");
        //   MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
    }
    
    
}

//******************************************************************************
// update
//******************************************************************************
void ParcelPageOne::enterEvo(int type)
{
    
    
    
    innerExit();
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_zhuansheng_title"));
    
    m_curScreen = enPkSc_Evo_ParcelPageOne;
    
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    if(m_evolution == NULL){
        m_evolution = new Evolution(this, callfuncO_selector(ParcelPageOne::cbEvolution), type);
        addChild(m_evolution);
        m_evolution->release();
    }
    m_evolution->setPosition(CCPointMake(640, 0));
    m_evolution->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointZero), 0.8f));
    //    char buf[100];
    //    snprintf(buf, 99, "naviText_evo_%d", int(CCRANDOM_0_1() * 3) + 1);
    
    
    
    MainScene::Inst()->showBottomPromptInfo("naviText_evo_");
    //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}

//******************************************************************************
// enterEvoOver
//******************************************************************************
void ParcelPageOne::enterEvoOver()
{
    
    
    if(MainScene::Inst()->new_FirstZhuanSheng)
    {
        MainScene::Inst()->removeTouchTip();
    }
    
    
    innerExit();
    
    m_curScreen = enPkSc_EvoOver_ParcelPageOne;
    
    m_spTitle->setPosition(CCPointMake(-210, 766));
    m_spTitle->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 766)), 0.8f));
    
    m_miBack->setPosition(CCPointMake(-529, 731));
    m_miBack->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(1, 731)), 0.8f));
    
    if(m_evoOvrShow == NULL){
        m_evoOvrShow = new EvoOverShow(this, callfunc_selector(ParcelPageOne::cbEvoOver), m_iEvoType);
        addChild(m_evoOvrShow);
        m_evoOvrShow->release();
    }
}

//******************************************************************************
// enterCardSell
//******************************************************************************
void ParcelPageOne::enterCardSell()
{
    m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_cardsell"));
    
    m_curScreen = enPkSc_Sell_ParcelPageOne;
    
    CGameData::Inst()->clearCombineQueue();
    
    enterPackage(enPkLkTp_Sell);
    //    char buf[100];
    //    snprintf(buf, 99, "naviText_sell_%d", int(CCRANDOM_0_1() * 3) + 1);
    
    
    
    MainScene::Inst()->showBottomPromptInfo("naviText_sell_");
    //  MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
}

//******************************************************************************
// cbCommDlg
//******************************************************************************
void ParcelPageOne::cbCommDlg(CCObject* pObj)
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
        switch (m_iReqType) {
            case enPackReqType_PwUp_ParcelPageOne:  //强化response
                if(CGameData::Inst()->powerUpRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enPackReqType_NiePan_ParcelPageOne:  // 涅槃
                if (CGameData::Inst()->niepanRequest()) {
                    scheduleUpdate();
                }
                break;
                
            case enPackReqType_GiveWuJiang_ParcelPageOne:  // 送武将
                
                if(CGameData::Inst()->newGiveWuJiangRequest("1"))
                {
                    
                    m_iReqType = enPackReqType_GiveWuJiang_ParcelPageOne;
                    scheduleUpdate();
                }
                
                break;
            case enPackReqType_GiveWuJiang_ZhuanSheng_ParcelPageOne:  // 送武将
                
                if(CGameData::Inst()->newGiveWuJiangRequest("3"))
                {
                    
                    m_iReqType = enPackReqType_GiveWuJiang_ZhuanSheng_ParcelPageOne;
                    scheduleUpdate();
                }
                
                break;
                
            case enPackReqType_PwUpAllCards_ParcelPageOne: //取得强化后的所有卡片
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enPackReqType_GiveWuJiangAllCards_ParcelPageOne:
            {
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enPackReqType_GiveWuJiangAllCards_ParcelPageOne;
                }
            }
                break;
                
            case enPackReqType_GiveWuJiangAllCards_ZhuanSheng_ParcelPageOne:
            {
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enPackReqType_GiveWuJiangAllCards_ZhuanSheng_ParcelPageOne;
                }
            }
                break;
                
            case enPackReqType_card_update_ParcelPageOne:
            {
                if(CGameData::Inst()->newChangeRequest("card_update"))
                {
                    
                    m_iReqType = enPackReqType_card_update_ParcelPageOne;
                    scheduleUpdate();
                }
            }
                break;
            case enPackReqType_card_update_ZhuanSheng_ParcelPageOne:
            {
                //                if(CGameData::Inst()->newChangeRequest("card_upgrade"))
                if(CGameData::Inst()->newChangeRequest("card_upgrade"))
                {
                    
                    m_iReqType = enPackReqType_card_update_ZhuanSheng_ParcelPageOne;
                    scheduleUpdate();
                }
            }
                break;
                
                
                
            case enPackReqType_Evolution_ParcelPageOne: //进化的response
                if(CGameData::Inst()->evolutionRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enPackReqType_SEVO_ParcelPageOne: //究极进化
                if(CGameData::Inst()->sevoRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enPackReqType_Wake_ParcelPageOne: //觉醒
                if(CGameData::Inst()->wakeRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enPackReqType_EvoAllCards_ParcelPageOne: //取得进化后的所有卡片
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enPackReqType_NewbieInf_ParcelPageOne: //新手引导
                CGameData::Inst()->newbieRequest(0, 0, NULL);
                scheduleUpdate();
                break;
                
            case enPackReqType_TeamChg_ParcelPageOne:
                if(CGameData::Inst()->commitTeamRequest()){
                    scheduleUpdate();
                }
                break;
                
            case enPackReqType_NewbiePwUp_ParcelPageOne:
                if(CGameData::Inst()->newbieRequestPwUp()){
                    scheduleUpdate();
                }
                break;
            default:
                break;
        }
    }
    else {
        CGameData::Inst()->clearReqStat();
        
        switch (m_iReqType) {
            case enPackReqType_PwUp_ParcelPageOne:  //强化response
            case enPackReqType_NiePan_ParcelPageOne:  // 涅槃
                enterPwUpBase();
                break;
            case enPackReqType_GiveWuJiang_ParcelPageOne:
                
                break;
            case enPackReqType_GiveWuJiang_ZhuanSheng_ParcelPageOne:
                
                break;
                
            case enPackReqType_PwUpAllCards_ParcelPageOne: //取得强化后的所有卡片
                enterPwUpSel();
                break;
            case enPackReqType_GiveWuJiangAllCards_ParcelPageOne:
                break;
            case enPackReqType_card_update_ParcelPageOne:
                
                break;
                
            case enPackReqType_GiveWuJiangAllCards_ZhuanSheng_ParcelPageOne:
                break;
            case enPackReqType_card_update_ZhuanSheng_ParcelPageOne:
                
                break;
            case enPackReqType_Evolution_ParcelPageOne: //进化的response
                enterEvo(0);
                break;
                
            case enPackReqType_SEVO_ParcelPageOne:
                enterEvo(2);
                break;
                
            case enPackReqType_Wake_ParcelPageOne:
                enterEvo(1);
                break;
                
            case enPackReqType_EvoAllCards_ParcelPageOne: //取得进化后的所有卡片
                enterEvoSel();
                break;
                
            case enPackReqType_TeamChg_ParcelPageOne:
                break;
            default:
                break;
        }
        
        if(CGameData::Inst()->isNewbie() == false)
            MainScene::Inst()->showBottomPanel(true);
    }
}

//******************************************************************************
// update
//******************************************************************************
void ParcelPageOne::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(ParcelPageOne::cbCommDlg), enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            //m_commDlg->setScale(0.0);
            //m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
        
        if(CGameData::Inst()->isNewbie() == false)
            MainScene::Inst()->enableBottomPanel(true);
    }
    
    unscheduleUpdate();
    
    if(comState == kReqStatOk){
        switch (m_iReqType) {
            case enPackReqType_PwUp_ParcelPageOne:  //强化response
            case enPackReqType_NiePan_ParcelPageOne:  // 涅槃
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enPackReqType_PwUpAllCards_ParcelPageOne;
                }
                break;
                
            case enPackReqType_GiveWuJiang_ParcelPageOne:
                
                
                //刷所有武将
                
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enPackReqType_GiveWuJiangAllCards_ParcelPageOne;
                }
                
                break;
            case enPackReqType_GiveWuJiang_ZhuanSheng_ParcelPageOne:
                
                
                //刷所有武将
                
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enPackReqType_GiveWuJiangAllCards_ZhuanSheng_ParcelPageOne;
                }
                
                break;
                
            case enPackReqType_card_update_ZhuanSheng_ParcelPageOne:
                
                MainScene::Inst()->new_FirstZhuanSheng = true;
                
                //                MainScene::Inst()->showTouchTip();
                MainScene::Inst()->enableBottomPanel(false);
                
                
                m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_basecard2"));
                m_curScreen = enPkSc_EvoSel_ParcelPageOne;
                
                CGameData::Inst()->clearCombineQueue();
                
                enterPackage(enPkLkTp_Evolution);
                //    char buf[100];
                //    snprintf(buf, 99, "naviText_evoSel_%d", int(CCRANDOM_0_1() * 3) + 1);
                
                
                MainScene::Inst()->showBottomPromptInfo("naviText_evoSel_");
                MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
            case enPackReqType_card_update_ParcelPageOne:
                
                
                MainScene::Inst()->new_FirstPowerUp = true;
                
                //                MainScene::Inst()->showTouchTip();
                MainScene::Inst()->enableBottomPanel(false);
                
                
                m_spTitle->setTitleSprite(CGameData::Inst()->getLanguageValue("title_basecard"));
                
                m_curScreen = enPkSc_PwUpBaseSel_ParcelPageOne;
                
                m_bNiePan = false;
                
                enterPackage(enPkLkTp_PwUpBase);
                //    char buf[100];
                //    snprintf(buf, 99, "naviText_pwUpSel_%d", int(CCRANDOM_0_1() * 2) + 1);
                
                
                
                MainScene::Inst()->showBottomPromptInfo("naviText_pwUpSel_");
                MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                
                CGameData::Inst()->clearCombineQueue();
                
                
                
                
                
                break;
            case enPackReqType_GiveWuJiangAllCards_ParcelPageOne:
                
                if(CGameData::Inst()->newChangeRequest("card_update"))
                {
                    
                    
                    
                    m_iReqType = enPackReqType_card_update_ParcelPageOne;
                    scheduleUpdate();
                }
                
                
                break;
            case enPackReqType_GiveWuJiangAllCards_ZhuanSheng_ParcelPageOne:
                
                if(CGameData::Inst()->newChangeRequest("card_upgrade"))
                {
                    
                    
                    
                    m_iReqType = enPackReqType_card_update_ZhuanSheng_ParcelPageOne;
                    scheduleUpdate();
                }
                
                
                break;
                
            case enPackReqType_NewbiePwUp_ParcelPageOne:
            {
                newBool = true;
                
                
                
                if(CGameData::Inst()->commitTeamRequest())
                {
                    m_iReqType = enPackReqType_TeamChg_ParcelPageOne;
                    scheduleUpdate();
                }
                
                
                
                
            }
                break;
            case enPackReqType_PwUpAllCards_ParcelPageOne: //取得强化后的所有卡片
                if (m_bNiePan)
                    showNiePanEffect();
                else
                    showPwUpEffect();
                break;
                
            case enPackReqType_Evolution_ParcelPageOne: //进化的response
            case enPackReqType_SEVO_ParcelPageOne:
            case enPackReqType_Wake_ParcelPageOne:
                if(CGameData::Inst()->allCardRequest()){
                    scheduleUpdate();
                    m_iReqType = enPackReqType_EvoAllCards_ParcelPageOne;
                }
                break;
                
            case enPackReqType_EvoAllCards_ParcelPageOne: //取得进化后的所有卡片
                enterEvoOver();
                MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                break;
                
            case enPackReqType_NewbieInf_ParcelPageOne: //新手引导
                CGameData::Inst()->initAfterLogin();
                initNewbieArrow();
                break;
                
            case enPackReqType_TeamChg_ParcelPageOne:
                
                if(newBool)
                {
                    MainScene::Inst()->removeTouchTip();
                    MainScene::Inst()->enableBottomPanel(false);
                    
                    Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(ParcelPageOne::newbieEnd),
                                                        CGameData::Inst()->getLanguageValue("newBieLayerTip5"),false);
                    addChild(newbieEndDlg, 500, kNewbieEndTag);
                    newbieEndDlg->release();
                    newbieEndDlg->setPosition(CCPointMake(320, 300));
                    newbieEndDlg->setScale(0.0);
                    newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                }
                else
                {
                    CGameData::Inst()->saveAllTeamInfo();
                    saveTeamInfo();
                    
                    innerEnter();
                    MainScene::Inst()->showTips_gonggao(CGameData::Inst()->getUsrInfo());//add1.16
                }
                
                break;
            default:
                break;
        }
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(ParcelPageOne::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
            
            if(m_iReqType == enPackReqType_TeamChg_ParcelPageOne){
                m_commDlg->setNoCancel();
            }
        }
    }
}

void ParcelPageOne::cbChangePackRange()
{
    
    if(MainScene::Inst()->new_FirstPowerUp || MainScene::Inst()->new_FirstZhuanSheng || MainScene::Inst()->new_CardAdd)
        return;
    
    if (m_rangeBtn->getTag() == enPkSc_TeamInf_ParcelPageOne) {
        // 重置编队
        if (m_pTeamInfoScrollViewLayer)
        {
            if (TeamInf *pTeamInf = m_pTeamInfoScrollViewLayer->getCurTeamInfo())
            {
                pTeamInf->showResetDlg();
            }
        }
        else if (m_teamInf)
        {
            m_teamInf->showResetDlg();
        }
        
    }
    else { // 改变package中武将顺序
        
        const char* szStringTbl[] = {
            "sortbtn_prop",
            "sortbtn_atk",
            "sortbtn_hp",
            "sortbtn_rcv",
            "sortbtn_get",
            "sortbtn_lock",
            "sortbtn_plus",
            "sortbtn_cost",
            "sortbtn_category",
            "sortbtn_countryportery",
        };
        
        CCSprite* spTemp = CCSprite::spriteWithFile("sortbtn.png");
        CCSize szBtn(0,0);
        if (spTemp)
            szBtn = spTemp->getContentSize();
        
        CCPoint ptBase(632 - szBtn.width, 720 - szBtn.height);
        
        if (m_SortBtnLayer == NULL) {
            m_SortBtnLayer = CCLayer::node();
            addChild(m_SortBtnLayer);
            
            for (int i = enCardRangeTypeProp; i < encardRangeTypeMax; i++) {
                CCSprite* spMenu1 = CCSprite::spriteWithFile("sortbtn.png");
                CCSprite* spMenu2 = CCSprite::spriteWithFile("sortbtn.png");
                CCMenuItem* pMI = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(ParcelPageOne::onSortBtnTouch) );
                pMI->setTag(i);
                pMI->setAnchorPoint(CCPointZero);
                
                TextNode* txt = TextNode::textWithString(CGameData::Inst()->getLanguageValue(szStringTbl[i]),24);
                txt->setPosition(CCPointMake(szBtn.width * 0.5 + 6, szBtn.height * 0.5));
                txt->setColor(ccWHITE);
                txt->setShadowColor(ccBLACK);
                pMI->addChild(txt);
                
                CCMenu* menu = CCMenu::menuWithItems(pMI, NULL);
                m_SortBtnLayer->addChild(menu, 30);
                menu->setPosition(CCPointZero);
                
                pMI->setPosition(ccp(ptBase.x, ptBase.y - (szBtn.height + 4) * i));
            }
            
            m_SortBtnLayer->setPosition(ccp(640-ptBase.x, 0));
            m_SortBtnLayer->runAction(CCEaseIn::actionWithAction(CCMoveTo::actionWithDuration(0.2, CCPointZero), 0.3));
        }
        else if(m_SortBtnLayer->numberOfRunningActions() == 0){
            if (m_SortBtnLayer->getPosition().x > 0) {
                m_SortBtnLayer->runAction(CCEaseIn::actionWithAction(CCMoveTo::actionWithDuration(0.2, CCPointZero), 0.3));
            }
            else{
                m_SortBtnLayer->runAction(CCSequence::actions(CCEaseOut::actionWithAction(CCMoveTo::actionWithDuration(0.2, ccp(640-ptBase.x, 0)), 0.3),
                                                              CCCallFunc::actionWithTarget(this, callfunc_selector(ParcelPageOne::onSortBtnLayerExit)),
                                                              NULL));
            }
        }
    }
}

void ParcelPageOne::setRangeTypeString()
{
    
    char buf[50];
    switch (CGameData::Inst()->getDefaultRangeType()) {
        case enCardRangeTypeAtta:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_attack"));
            break;
        case enCardRangeTypeGet:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_get"));
            break;
        case enCardRangeTypeHp:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_hp"));
            break;
        case enCardRangeTypeProp:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_property"));
            break;
        case enCardRangeTypeRec:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_recover"));
            break;
        case enCardRangeTypeLock:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_lock"));
            break;
        case enCardRangeTypePlus:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_plus"));
            break;
        case enCardRangeTypeCost:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_cost"));
            break;
        case enCardRangeTypeCategory:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_category"));
            break;
            case enCardRangeTypeCountryPorterty:
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("range_countryporte"));
            break;
        default:
            break;
    }
    
    rangeLabel->setString(buf);
}
void ParcelPageOne::initNewbieArrow()
{
    Arrow* pArrow = new Arrow(30, 255);
    
    if(CGameData::Inst()->getNewbieStep() == 4){
        pArrow->setPosition(CCPointMake(486, 670));
    }
    else {
        pArrow->setPosition(CCPointMake(486, 554));
    }
    
    addChild(pArrow, 500, kGeneralArrowTag);
    pArrow->release();
    
    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                       CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                       NULL);
    pArrow->runAction(CCRepeatForever::actionWithAction(action));
}

void ParcelPageOne::setRangeBtnIsVisble(bool isShow)
{
    if((m_rangeBtn) && (CGameData::Inst()->isNewbie() == false))
    {
        m_rangeBtn->setIsVisible(isShow);
    }
}




//******************************************************************************
// shareSanguo--yang
//******************************************************************************
void ParcelPageOne::shareSanGuo(){
    
    OcProxy::Inst()->sharetoSanguo(this, callfunc_selector(ParcelPageOne::shareToWeibo));
    
    
}

//******************************************************************************
// reset
//******************************************************************************
void ParcelPageOne::reset()
{
    for (int i = 0; i < enPkLkTp_Max; i++) {
        arraypackageY[i]=0;
    }
    
    iLookY=0;
}


//******************************************************************************
// onSortBtnLayerExit
//******************************************************************************
void ParcelPageOne::onSortBtnLayerExit()
{
    if (m_SortBtnLayer) {
        removeChild(m_SortBtnLayer, true);
        m_SortBtnLayer = NULL;
    }
}

//******************************************************************************
// onSortBtnTouch
//******************************************************************************
void ParcelPageOne::onSortBtnTouch(CCObject* sender)
{
    CCMenuItem* mi = (CCMenuItem*) sender;
    
    m_package->rangePackageCardWithType((enCardRangeType) (mi->getTag()));
    setRangeTypeString();
    cbChangePackRange();
}


void ParcelPageOne::inertiaMove(float dt)
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
        
        unschedule(schedule_selector(ParcelPageOne::inertiaMove));
    }
}


void ParcelPageOne::moveY(float dt)
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
        
        
        CScrollBar*     pScrollBar = (CScrollBar*)getChildByTag(9988);

        
        pScrollBar->setPercent(fPercent);
        
        pScrollBar->setPosition(CCPointMake(630, 435 + 30 - thisPt.y));

        
//        m_pScrollBar->setPercent(fPercent);
//        
//        m_pScrollBar->setPosition(CCPointMake(630, 435 + 30 - thisPt.y));
        
        //  redrawCapacity();
    }
}


void ParcelPageOne::setThisPosition(const CCPoint& pt, bool bRedraw)
{
    this->setPosition(pt);
    
    

    
//    //不强制要求重画，而且移动范围小于100
    if((!bRedraw) && (abs(m_iPreThisPosY - (int)pt.y) < 100))
        return;
    
    m_iPreThisPosY = pt.y;
    
    //改变m_pScrollBar位置
    //    CCLOG("m_pScrollBarY=%f",m_pScrollBarY);
    
    CScrollBar*     pScrollBar = (CScrollBar*)getChildByTag(9988);
    
    if (pScrollBar) {
        pScrollBar->setPosition(CCPointMake(630, 435 + 30 - pt.y));
        //   redrawCapacity();
        pScrollBar->setPercent(pt.y/m_fUpLimitY);
        //  redrawCapacity();
    }
    
    
//    if (m_pScrollBar) {
//        m_pScrollBar->setPosition(CCPointMake(630, 435 + 30 - pt.y));
//        //   redrawCapacity();
//        m_pScrollBar->setPercent(pt.y/m_fUpLimitY);
//        //  redrawCapacity();
//    }
    
//    CCNode* node;
//    for(int i = 0; i < kMaxCardNum; i++){
//        node = getChildByTag(ITEMCARDDELTA + i);
//        
//        if(node)
//        {
////            ItemCard* card = (ItemCard*) node;
////            CCPoint ptCard = card->getPosition();
//            
////            if((ptCard.y + pt.y < 740) && (ptCard.y + pt.y > 250))
////            {
////                card->setIsVisible(true);
////            }
////            else{
////                card->setIsVisible(false);
////            }
//        }
//    }
    
    //CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool ParcelPageOne::ccTouchBegan(CCTouch* touch, CCEvent* event)
{

    
    unschedule(schedule_selector(ParcelPageOne::inertiaMove));
    
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
void ParcelPageOne::ccTouchMoved(CCTouch* touch, CCEvent* event)
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
    
    
    setThisPosition(thisPt);
    
    if(m_fUpLimitY > 0)
    {
        float fPercent = thisPt.y / m_fUpLimitY;
        
        CScrollBar*     pScrollBar = (CScrollBar*)getChildByTag(9988);

        
        pScrollBar->setPercent(fPercent);
        
        pScrollBar->setPosition(CCPointMake(630, 435 + 30 - thisPt.y));
        
        
//        m_pScrollBar->setPercent(fPercent);
//        
//        m_pScrollBar->setPosition(CCPointMake(630, 435 + 30 - thisPt.y));
    }
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void ParcelPageOne::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    if(m_bTouchMvFlg == true && ccpDistance(touchLocation, m_ptStartGl) > kListBoxMinMovY)
    {
        m_lastYm *= 1.5;
        if(fabs(m_lastYm) >100)
            m_lastYm = m_lastYm/fabs(m_lastYm) *100;
        
        if(!MainScene::Inst()->new_FirstItem)
            schedule(schedule_selector(ParcelPageOne::inertiaMove));
        return;
    }
    
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    
    if(m_choiceChild)
    {
        if (CGameData::Inst()->getParcelChildInfoAry()->count() <= 0)
        {
            
            return;
            
        }
    }
    else
    {
        if (CGameData::Inst()->getParcelItemInfoAry()->count() <= 0)
        {
            
            return;
            
        }
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
    
    
    if(m_choiceChild)
    {
        if (iSel >= CGameData::Inst()->getParcelChildInfoAry()->count())
        {
            
            return;
            
        }
    }
    else
    {
        if (iSel >= CGameData::Inst()->getParcelItemInfoAry()->count())
        {
            
            return;
            
        }
    }
    

    ParcelSelRet   m_ret;
    
    
    if(m_choiceChild)
        m_ret.iType = 1;
    else
        m_ret.iType = 0;
    
    m_ret.iBtnSel = iSel;
    
    (m_pListener->*m_pfnSelector)(&m_ret);

    
    
    
//    CCLog("%d" , iSel);
    
    
}







