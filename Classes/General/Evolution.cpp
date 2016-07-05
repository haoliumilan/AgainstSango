//
//  Evolution.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-1.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "Evolution.h"
#include "CGamedata.h"
#include "MainScene.h"
#include "CSkillLib.h"
#include "CShowCardInf.h"
#include "TextNode.h"
#include "Dialog2.h"
#include "SimpleAudioEngine.h"
#include "OcProxy.h"

#include "Arrow.h"

using namespace CocosDenshion;

#define kSevoRuleDlgTag 7895
#define kWakeRuleDlgTag 5566

//******************************************************************************
// Constructor
//******************************************************************************
Evolution::Evolution(CCObject* target, SEL_CallFuncO selector, int type)
{
    m_pListener = target;
    m_pfnSelector = selector;
    m_plus = 0;
    m_type = type;
    m_bFoodInTeam = false;
    m_bFoodLocked = false;
    m_dlgAlertLock = NULL;
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 20);
    m_showCard->release();
    m_showCard->setIsVisible(false);
    
    m_bReady = true;
    
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
    
    m_bEluEvo = (type == 2);//(pUsrCardInf->elu_flg > 0);
    m_bWake = (type == 1);//(pUsrCardInf->wk_flg > 0);
    
    if (m_bWake)
    {
        showWakeRule();
    }
    else if (m_bEluEvo)
    {
        showSevoRule();
    }
    else{
        setIsTouchEnabled(true);
        CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    }
    
    initBaseCard(pUsrCardInf);
    initFoodCard(pUsrCardInf);
    initTargetCard(pUsrCardInf);
    
    
    TextNode* lbText;
    CCPoint pt;
    CCSize sz;
    m_spOk = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    if(m_spOk != NULL){
        m_spOk->setPosition(CCPointMake(320, 180));
        addChild(m_spOk);
        
        pt = m_spOk->getPosition();
        sz = m_spOk->getContentSize();
        
        if (m_bWake) {
            CCSprite* sp = CCSprite::spriteWithSpriteFrameName("wake_btntitle.png");
            if (sp) {
                m_spOk->addChild(sp);
                sp->setPosition(CCPointMake(sz.width*0.5-4, sz.height*0.5+4));
            }
        }
        else if (m_bEluEvo) {
            CCSprite* sp = CCSprite::spriteWithSpriteFrameName("sevo_btntitle.png");
            if (sp) {
                m_spOk->addChild(sp);
                sp->setPosition(CCPointMake(sz.width*0.5-4, sz.height*0.5+4));
            }
        }
        else{
            lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("evolution_title"), CCSizeMake(sz.width, 30), CCTextAlignmentCenter, kBtnTitleHeight);
            lbText->setPosition(pt);
            lbText->setColor(ccWHITE);
            lbText->setShadowColor(ccBLACK);
            addChild(lbText, 1);
        }
    }
    
    if(m_bReady == false){
        CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
        if(sp){
            sp->setPosition(CCPointMake(320, 180));
            addChild(sp, 2);
        }
    }
    
    
    if(MainScene::Inst()->new_FirstZhuanSheng)
    {
        Arrow* pArrow = new Arrow(30, 255);
        
        pArrow->setPosition(CCPointMake(231, 180));
        
        
        
        pArrow->setRotation(180);
        //    pArrow->setScale(0.6f);
        
        MainScene::Inst()->addChild(pArrow, 50000, 95999);
        
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));

    }
    
}

void Evolution::waitShowInfo()
{
    char * ucid = CGameData::Inst()->getCombineCard(0);
    //进化前卡
    CUserCardInfo* pUsrCardInf = CGameData::Inst()->getUsrCardInfByUcid(ucid);
    
    CCSprite *sp = (CCSprite *)getChildByTag(111);
    CCPoint pos = convertToNodeSpace(beganPoint);
    CCPoint pt = sp->getPosition();
    CCSize sz = sp->getContentSize();
    
    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                       pt.y - sz.height * 0.5, 
                                                       sz.width, 
                                                       sz.height), 
                                            pos);
    if(bRet)
    {
        m_showCard->show(pUsrCardInf);
    }
    //进化后卡
    sp = (CCSprite *)getChildByTag(112);
    pt = sp->getPosition();
    bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                       pt.y - sz.height * 0.5, 
                                                       sz.width, 
                                                       sz.height), 
                                            pos);

    CCardBaseInfo* pUsrCardInf2;
    
    if (m_bWake)
        pUsrCardInf2 = CGameData::Inst()->getCardBaseInfByCid(pUsrCardInf->wk_target);
    else if (m_bEluEvo)
        pUsrCardInf2 = CGameData::Inst()->getCardBaseInfByCid(pUsrCardInf->elu_target);
    else
        pUsrCardInf2 = CGameData::Inst()->getCardBaseInfByCid(pUsrCardInf->ugp_target);

    if(bRet)
    {
        // show base card info
        m_showCard->show(pUsrCardInf2, pUsrCardInf->pl_hp, pUsrCardInf->pl_attack, pUsrCardInf->pl_recover);
    }
    
    //进化素材
    CCPoint ptStart(97, 459);
   // CCMutableArray<CUserCardInfo*> *pUserCardArr = CGameData::Inst()->getUserCards();
   // CUserCardInfo* pinfo;
    for (int i =0 ; i<5; i++) {
       // sp = (CCSprite *)getChildByTag(120+i);
        //pt = sp->getPosition();
        int cid;
        
        if (m_bWake)
            cid = pUsrCardInf->wk_need[i];
        else if (m_bEluEvo)
            cid = pUsrCardInf->elu_need[i];
        else
            cid = pUsrCardInf->ugp_need[i];

        if(cid == 0)
            continue;
        char UcidStr[50];
        strcpy(UcidStr, CGameData::Inst()->getCombineCard(i+1));
        pt = CCPoint(ptStart.x + 112 * i, ptStart.y);
        bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                      pt.y - sz.height * 0.5, 
                                                      sz.width, 
                                                      sz.height), 
                                           pos);
            
        if(bRet)
        {
            CCardBaseInfo *tempInfp = CGameData::Inst()->getCardBaseInfByCid(cid);
            m_showCard->show(tempInfp);
        }

    }
    unschedule(schedule_selector(Evolution::waitShowInfo));

}
//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool Evolution::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if(m_dlgAlertLock)
        return false;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    beganPoint = touchLocation;
    schedule(schedule_selector(Evolution::waitShowInfo), 0.3);
    if(!m_bReady)
        return true;
    
    m_bTouchMvFlg=false;
    
    return true;
}

void Evolution::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    m_bTouchMvFlg = true;
}

void Evolution::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchLocation = pTouch->locationInView( pTouch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    if(m_bTouchMvFlg && ccpDistance(touchLocation, beganPoint) > kListBoxMinMovY)
        return;
    if(!m_bReady)
        return ;

    CCPoint pos = convertToNodeSpace(touchLocation);
    CCPoint pt = m_spOk->getPosition();
    CCSize sz = m_spOk->getContentSize();
    
    bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                       pt.y - sz.height * 0.5, 
                                                       sz.width, 
                                                       sz.height), 
                                            pos);
    
    if(bRet){        
        if (m_bFoodLocked) {
            // 当前选中的素材中有被加锁的
            showAlertLock();
        }
        else if (m_bFoodInTeam)
            // 当前选中的素材中有在编队中的
            MainScene::Inst()->showFoodInTeam();
        else
        {
            EvoluTypeRet ret;
            ret.iType = m_type;
            (m_pListener->*m_pfnSelector)(&ret);
        }
    }
    
}
//******************************************************************************
// initBaseCard
//******************************************************************************
void Evolution::initBaseCard(CUserCardInfo* pUsrCardInf)
{
    CCSprite* sp = CCSprite::spriteWithFile("pa_generals.png");
    if(sp != NULL){
        sp->setPosition(CCPointMake(320, 638));
        addChild(sp);
    }
    
    CCPoint ptCard(238, 633);
    char buf[100];
    sp = CGameData::Inst()->getHeadSprite(pUsrCardInf->cid);
    sp->setTag(111);
    if(sp != NULL){
        sp->setPosition(ptCard);
        addChild(sp);
    }
    
    if (pUsrCardInf->plus > 0) {
        CCSprite* spTmp = CCSprite::spriteWithFile("plusnum.png");

        if (spTmp) {
            CCSize sz = spTmp->getContentSize();
            snprintf(buf, 99, "/%d", pUsrCardInf->plus);
            CCLabelAtlas* lbPlus = CCLabelAtlas::labelWithString(buf, "plusnum.png", sz.width / 11, sz.height, '/');
            lbPlus->setAnchorPoint(CCPointMake(1, 0));
            lbPlus->setPosition(CCPointMake(95, 10));
            sp->addChild(lbPlus);
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(1.0),
                                                                               CCFadeOut::actionWithDuration(0.2),
                                                                               CCFadeIn::actionWithDuration(0.2),
                                                                               NULL);            
            lbPlus->runAction(CCRepeatForever::actionWithAction(action1));
            m_plus = pUsrCardInf->plus;
        }
    }
    
    CGameData::Inst()->setCidBeforeEvo(pUsrCardInf->cid);
    
    if (m_bWake)
        CGameData::Inst()->setCidAfterEvo(pUsrCardInf->wk_target);
    else if (m_bEluEvo)
        CGameData::Inst()->setCidAfterEvo(pUsrCardInf->elu_target);
    else
        CGameData::Inst()->setCidAfterEvo(pUsrCardInf->ugp_target);


    CCSprite* spSkill;
    if(pUsrCardInf->skid == 0)
        spSkill = CCSprite::spriteWithSpriteFrameName("fr_skill gray.png");
    else
        spSkill = CCSprite::spriteWithSpriteFrameName("fr_skill.png");
    
    if(spSkill != NULL){
        spSkill->setPosition(CCPointMake(ptCard.x - 28, ptCard.y - 62));
        addChild(spSkill);
    }
    
    CCSprite* spLSkill;
    if(pUsrCardInf->leader_skid == 0)
        spLSkill = CCSprite::spriteWithSpriteFrameName("fr_lskill gray.png");
    else
        spLSkill = CCSprite::spriteWithSpriteFrameName("fr_lskill.png");
    
    if(spLSkill != NULL){
        spLSkill->setPosition(CCPointMake(ptCard.x + 26, ptCard.y - 62));
        addChild(spLSkill);
    }
    
    //數值
    TextNode* lbText = TextNode::textWithString("Lv:", CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y + 26));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);

    
    if(pUsrCardInf->lv != pUsrCardInf->max_lv){
        sprintf(buf, "%d", pUsrCardInf->lv);
    }
    else {
        sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("max"));
    }
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y + 26));
    lbText->setAnchorPoint(CCPointZero);
    
    if (m_bWake) {
        lbText->setColor(ccWHITE);
    }
    else{
        if (((m_bEluEvo) && (pUsrCardInf->lv < CGameData::Inst()->getCommonInfo()->evolution_lv))
            || ((!m_bEluEvo) && (pUsrCardInf->lv != pUsrCardInf->max_lv))){
            m_bReady = false;
            lbText->setColor(ccRED);
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.2, 128),
                                                                               CCFadeTo::actionWithDuration(0.2, 255),
                                                                               NULL);
            lbText->runActionInside(CCRepeatForever::actionWithAction(action));
        }
        else {
            lbText->setColor(ccWHITE);
        }
    }
    
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    if (pUsrCardInf->skid > 0) {
        snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("skill_lv"));
        lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
        lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y + 2));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
        
        stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(pUsrCardInf->skid);
        if (pUsrCardInf->sk_lv == pManSkill->cMaxLv) {
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("max"));
        }
        else {
            sprintf(buf, "%d", pUsrCardInf->sk_lv);
        }
        lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
        lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y + 2));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
    }
        
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("hp"));
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y - 22));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
//    sprintf(buf, "%ld", pUsrCardInf->hp);
    sprintf(buf, "%ld", pUsrCardInf->hp.get());
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y - 22));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("attack"));
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y - 46));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
//    sprintf(buf, "%lu", pUsrCardInf->attack);
    sprintf(buf, "%lu", pUsrCardInf->attack.get());
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y - 46));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("recover"));
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y - 70));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    sprintf(buf, "%ld", pUsrCardInf->recover);
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y - 70));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
}

//******************************************************************************
// initFoodCard
//******************************************************************************
void Evolution::initFoodCard(CUserCardInfo* pUsrCardInf)
{
    CCSprite* sp = CCSprite::spriteWithFile("pa_material_box.png");
    if(sp != NULL){
        sp->setPosition(CCPointMake(320, 486));
        addChild(sp, -1);
    }
    
    int k = 0;
    char buf[100];
    CCPoint ptStart(97, 459);
    CCMutableArray<CUserCardInfo*> *pUserCardArr = CGameData::Inst()->getUserCards();
    
    
    CUserCardInfo* pinfo = NULL;
    for(int i = 0; i < 5; i++){
        bool isFoodInTeam = false;
        int cid;
        
        if (m_bWake)
            cid = pUsrCardInf->wk_need[i];
        else if (m_bEluEvo)
            cid = pUsrCardInf->elu_need[i];
        else
            cid = pUsrCardInf->ugp_need[i];

        if(cid == 0){
            break;
        }

        sp = CGameData::Inst()->getHeadSprite(cid);
        if(sp != NULL){
            sp->setPosition(CCPointMake(ptStart.x + 112 * i, ptStart.y));
            addChild(sp);
            sp->setTag(120+i);
        }

        bool isFoodLocked = false;   // 素材是否有被加锁
        int foodSelectedNo = -1;       // 记录当前选中的素材的序数k(加这变量，是针对这种情况：A转生素材是两个B，当前背包里面只有一个B，同时B在编队中)
        
        for(k = 0; k < pUserCardArr->count(); k++) {
            pinfo = pUserCardArr->getObjectAtIndex(k);
            
            if (strcmp(pUsrCardInf->ucid, pinfo->ucid) == 0) {
                continue;   // 基础武将不能在素材中
            }

            if(pinfo->bChecked) // 该素材已经被放到转生素材的格子里
                continue;
            
            if(pinfo->cid == cid) {
                if (!m_bFoodInTeam) {
                    // 当前没有素材在编队中
                    if (CGameData::Inst()->isOneOfTeam(pinfo->ucid)) {
                        // 该素材在编队中
                        if (foodSelectedNo == -1) { // 此判断是针对这种情况：A转生需要一个B，当前有两个B，同时都在编队中，一个加锁，一个没有加锁
                            // 第一次找到符合的素材，同事它在编队中
                            isFoodInTeam = true;
                            foodSelectedNo = k;
                            CGameData::Inst()->setCombineQueue(i+1, pinfo->ucid);
                            
                            if (CGameData::Inst()->getIsLock(pinfo->ucid)) {
                                isFoodLocked = true;
                            }
                        }
                        else {
                            CUserCardInfo *pinfoInTeam = pUserCardArr->getObjectAtIndex(foodSelectedNo);
                            if (CGameData::Inst()->getIsLock(pinfoInTeam->ucid)){
                                if (!CGameData::Inst()->getIsLock(pinfo->ucid)) {
                                    // 前一次找到的素材在编队中，同时被加锁，后一次找到的素材在编队中，同时没有加锁
                                    foodSelectedNo = k;
                                    CGameData::Inst()->setCombineQueue(i+1, pinfo->ucid);
                                }
                            }
                        }
                    }
                    else {
                        isFoodInTeam = false;
                        CGameData::Inst()->setCombineQueue(i+1, pinfo->ucid);
                        foodSelectedNo = k;
                        
                        if (CGameData::Inst()->getIsLock(pinfo->ucid)) {
                            isFoodLocked = true;
                        }
                        else {
                            isFoodLocked = false;
                            break;
                        }
                    }
                }
                else {
                    CGameData::Inst()->setCombineQueue(i+1, pinfo->ucid);
                    foodSelectedNo = k;
                    
                    if (CGameData::Inst()->getIsLock(pinfo->ucid)) {
                        isFoodLocked = true;
                    }
                    else
                    {
                        isFoodLocked = false;
                        break;
                    }
                }
            }
        }
         
        if(k == pUserCardArr->count() && !isFoodInTeam && !isFoodLocked){
            // 该素材不在背包中
            sp = CCSprite::spriteWithSpriteFrameName("fr_black.png");
            sp->setPosition(CCPointMake(ptStart.x + 112 * i, ptStart.y));
            addChild(sp);
            m_bReady = false;
        }
        else {
            CUserCardInfo *pinfoSelected = pUserCardArr->getObjectAtIndex(foodSelectedNo);
            //  选中的素材
            pinfoSelected->bChecked = true;
            
            if (isFoodInTeam) {
                // 选中的素材有在编队中
                m_bFoodInTeam = true;
            }

            if (CGameData::Inst()->getIsLock(pinfoSelected->ucid)) {
                //  选中的素材被加锁
                m_bFoodLocked = true;
                CCSprite *spLock = CCSprite::spriteWithSpriteFrameName("fr_lock.png");
                spLock->setAnchorPoint(CCPointMake(1, 1));
                spLock->setPosition(CCPointMake(ptStart.x + 112 * i + 56, ptStart.y + 56));
                addChild(spLock);
                
                CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCDelayTime::actionWithDuration(0.2),
                                                                                   CCFadeOut::actionWithDuration(0.5),
                                                                                   CCFadeIn::actionWithDuration(0.5),
                                                                                   NULL);
                spLock->runAction(CCRepeatForever::actionWithAction(action));
            }
        }

    }
    
    for(k = 0; k < pUserCardArr->count(); k++){
        pinfo = pUserCardArr->getObjectAtIndex(k);
        pinfo->bChecked = false;
    }
    
    
    
    //pUsrCardInf2 = CGameData::Inst()->getCardBaseInfByCid(pUsrCardInf->ugp_target);
    //test
//    int nCountSum = pUserCardArr->count();
//    for (int i = 0; i < nCountSum; ++i)
//    {
//        if (CUserCardInfo *pUserCardInfo = pUserCardArr->getObjectAtIndex(i))
//        {
//            if (CCardBaseInfo *pCardBaseInfo = CGameData::Inst()->getCardBaseInfByCid(pUserCardInfo->ugp_target))
//            {
//                if (std::string(pUserCardInfo->name)!=std::string(pCardBaseInfo->name))
//                {
//                    printf("ID【%d】存在名字不一样:【%s】->【%s】\n",pUserCardInfo->cid,pUserCardInfo->name,pCardBaseInfo->name);
//                }
//            }
//        }
//    }
    
//    for (int i = 0; i < kMaxCardNum; ++i)
//    {
//        if (CCardBaseInfo *pCardBaseInfo1 = CGameData::Inst()->getCardBaseInfByCid(i))
//        {
//            if (CCardBaseInfo *pCardBaseInfo2 = CGameData::Inst()->getCardBaseInfByCid(pCardBaseInfo1->ugp_target))
//            {
//                 if (std::string(pCardBaseInfo1->name)!=std::string(pCardBaseInfo2->name))
//                 {
//                     printf("ID【%d】存在名字不一样:【%s】->【%s】\n",pCardBaseInfo1->cid,pCardBaseInfo1->name,pCardBaseInfo2->name);
//                 }
//            }
//            
//        }
//    }
    /*
    const int array_size = 5;
    CUserCardInfo *array_temp[array_size];
    memset(array_temp, 0, sizeof(array_temp));
    int con_size = pUserCardArr->count();
    for (int i = 0; i < array_size; ++i)
    {
        unsigned short cid = 0;
        if (m_bWake) cid = pUsrCardInf->wk_need[i];
        else if (m_bEluEvo) cid = pUsrCardInf->elu_need[i];
        else cid = pUsrCardInf->ugp_need[i];
        if(cid == 0)continue;
        
        for (int j = 0; j < con_size; ++j)
        {
            if (CUserCardInfo *pCardTemp = pUserCardArr->getObjectAtIndex(j))
            {
                if (cid==pCardTemp->cid)
                {
                    bool b = false;
                    for (int k = 0; k < array_size; ++k)        //存在两个
                    {
                        if (array_temp[k]==pCardTemp)
                        {
                            b = true;
                            break;
                        }
                    }
                    if (b)continue;
                    
                    if (array_temp[i])
                    {
                        //锁上
                        if (CGameData::Inst()->getIsLock(pCardTemp->ucid))
                        {
                            continue;
                        }
                        //编队
                        if (CGameData::Inst()->isOneOfTeam(pCardTemp->ucid))
                        {
                            continue;
                        }
                        //比较
                        if (!CGameData::Inst()->getIsLock(array_temp[i]->ucid) &&
                            !CGameData::Inst()->isOneOfTeam(array_temp[i]->ucid))
                        {
//                            //加蛋/值
//                            if (pCardTemp->plus>array_temp[i]->plus)
//                            {
//                                continue;
//                            }
//                            else if (pCardTemp->plus==array_temp[i]->plus)
//                            {
//                                //武将技
//                                if (pCardTemp->sk_lv>array_temp[i]->sk_lv)
//                                {
//                                    continue;
//                                }
//                                else if (pCardTemp->sk_lv==array_temp[i]->sk_lv)
//                                {
//                                    //等级
//                                    if (pCardTemp->lv>array_temp[i]->lv)
//                                    {
//                                        continue;
//                                    }
//                                    else if (pCardTemp->lv==array_temp[i]->lv)
//                                    {
//                                        //星级
//                                        if (pCardTemp->star>array_temp[i]->star)
//                                        {
//                                            continue;
//                                        }
//                                    }
//                                }
//                            }
                        }
                        //if (strcmp(pUsrCardInf->ucid, pCardTemp->ucid) == 0) {
                       //     continue;   // 基础武将不能在素材中
                       // }
                        array_temp[i] = pCardTemp;
                    }
                    else
                    {
                       // if (strcmp(pUsrCardInf->ucid, pCardTemp->ucid) == 0) {
                       //     continue;   // 基础武将不能在素材中
                      //  }
                        array_temp[i] = pCardTemp;
                    }
                }
            }
        }
    }
    
    CUserCardInfo* pinfo = NULL;
    for(int i = 0; i < 5; i++){
        bool isFoodInTeam = false;
        int cid;
        
        if (m_bWake)
            cid = pUsrCardInf->wk_need[i];
        else if (m_bEluEvo)
            cid = pUsrCardInf->elu_need[i];
        else
            cid = pUsrCardInf->ugp_need[i];
        
        if(cid == 0){
            break;
        }
        
        sp = CGameData::Inst()->getHeadSprite(cid);
        if(sp != NULL){
            sp->setPosition(CCPointMake(ptStart.x + 112 * i, ptStart.y));
            addChild(sp);
            sp->setTag(120+i);
        }
        
        bool isFoodLocked = false;   // 素材是否有被加锁
        int foodSelectedNo = -1;       // 记录当前选中的素材的序数k(加这变量，是针对这种情况：A转生素材是两个B，当前背包里面只有一个B，同时B在编队中)
        
        for(k = 0; k < array_size; k++) {
            pinfo = array_temp[k];
            if (!pinfo)continue;
            
            if (strcmp(pUsrCardInf->ucid, pinfo->ucid) == 0) {
                continue;   // 基础武将不能在素材中
            }
            
            if(pinfo->bChecked) // 该素材已经被放到转生素材的格子里
                continue;
            
            if(pinfo->cid == cid) {
                if (!m_bFoodInTeam) {
                    // 当前没有素材在编队中
                    if (CGameData::Inst()->isOneOfTeam(pinfo->ucid)) {
                        // 该素材在编队中
                        if (foodSelectedNo == -1) { // 此判断是针对这种情况：A转生需要一个B，当前有两个B，同时都在编队中，一个加锁，一个没有加锁
                            // 第一次找到符合的素材，同事它在编队中
                            isFoodInTeam = true;
                            foodSelectedNo = k;
                            CGameData::Inst()->setCombineQueue(i+1, pinfo->ucid);
                            
                            if (CGameData::Inst()->getIsLock(pinfo->ucid)) {
                                isFoodLocked = true;
                            }
                        }
                        else
                        {
                            if (0<=foodSelectedNo && array_size>foodSelectedNo)
                            {
                                CUserCardInfo *pinfoInTeam = array_temp[foodSelectedNo];
                                if (CGameData::Inst()->getIsLock(pinfoInTeam->ucid))
                                {
                                    if (!CGameData::Inst()->getIsLock(pinfo->ucid))
                                    {
                                        // 前一次找到的素材在编队中，同时被加锁，后一次找到的素材在编队中，同时没有加锁
                                        foodSelectedNo = k;
                                        CGameData::Inst()->setCombineQueue(i+1, pinfo->ucid);
                                    }
                                }
                            }
                        }
                    }
                    else {
                        isFoodInTeam = false;
                        CGameData::Inst()->setCombineQueue(i+1, pinfo->ucid);
                        foodSelectedNo = k;
                        
                        if (CGameData::Inst()->getIsLock(pinfo->ucid)) {
                            isFoodLocked = true;
                        }
                        else {
                            isFoodLocked = false;
                            break;
                        }
                    }
                }
                else {
                    CGameData::Inst()->setCombineQueue(i+1, pinfo->ucid);
                    foodSelectedNo = k;
                    
                    if (CGameData::Inst()->getIsLock(pinfo->ucid)) {
                        isFoodLocked = true;
                    }
                    else
                    {
                        isFoodLocked = false;
                        break;
                    }
                }
            }
        }
        
        if(k == array_size && !isFoodInTeam && !isFoodLocked)
        {
            // 该素材不在背包中
            sp = CCSprite::spriteWithSpriteFrameName("fr_black.png");
            sp->setPosition(CCPointMake(ptStart.x + 112 * i, ptStart.y));
            addChild(sp);
            m_bReady = false;
        }
        else
        {
            if (0<=foodSelectedNo && array_size>foodSelectedNo)
            {
                CUserCardInfo *pinfoSelected = array_temp[foodSelectedNo];
                //  选中的素材
                pinfoSelected->bChecked = true;
                
                if (isFoodInTeam)
                {
                    // 选中的素材有在编队中
                    m_bFoodInTeam = true;
                }
                
                if (CGameData::Inst()->getIsLock(pinfoSelected->ucid))
                {
                    //  选中的素材被加锁
                    m_bFoodLocked = true;
                    CCSprite *spLock = CCSprite::spriteWithSpriteFrameName("fr_lock.png");
                    spLock->setAnchorPoint(CCPointMake(1, 1));
                    spLock->setPosition(CCPointMake(ptStart.x + 112 * i + 56, ptStart.y + 56));
                    addChild(spLock);
                    
                    CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCDelayTime::actionWithDuration(0.2),
                                                                                       CCFadeOut::actionWithDuration(0.5),
                                                                                       CCFadeIn::actionWithDuration(0.5),
                                                                                       NULL);
                    spLock->runAction(CCRepeatForever::actionWithAction(action));
                }
            }
            
        }
    }
    
    for (int i = 0; i < array_size; ++i)
    {
        if (array_temp[i])
        {
            array_temp[i]->bChecked = false;
        }
    }
    */
    /////////////

    
    //text
    snprintf(buf, 99, "%s: %ld", CGameData::Inst()->getLanguageValue("coin_need"), pUsrCardInf->upg_gold);
    TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(400, 30), CCTextAlignmentRight, 24);
    lbText->setPosition(CCPointMake(603, 528));
    lbText->setAnchorPoint(CCPointMake(1, 0.5));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);
    
    CUserInfo* pUsrInf = CGameData::Inst()->getUsrInfo();
    if(pUsrCardInf->upg_gold > pUsrInf->gold){
        lbText->setColor(ccRED);
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.2, 128),
                                                                           CCFadeTo::actionWithDuration(0.2, 255),
                                                                           NULL);
        lbText->runActionInside(CCRepeatForever::actionWithAction(action));
        m_bReady = false;
    }
}

//******************************************************************************
// initTargetCard
//******************************************************************************
void Evolution::initTargetCard(CUserCardInfo* pUsrCardInf)
{
    int cid;
    
    if (m_bWake)
        cid = pUsrCardInf->wk_target;
    else if (m_bEluEvo)
        cid = pUsrCardInf->elu_target;
    else
        cid = pUsrCardInf->ugp_target;
    
    int sklv = pUsrCardInf->sk_lv;
    int plus_hp = pUsrCardInf->pl_hp;
    int plus_attack = pUsrCardInf->pl_attack;
    int plus_recover = pUsrCardInf->pl_recover;
    
    if(cid == 0)
        return;
    
    CCSprite* sp = CCSprite::spriteWithFile("pa_result.png");
    if(sp != NULL){
        sp->setPosition(CCPointMake(320, 306));
        addChild(sp);
    }
    
    CCardBaseInfo* pCardInf = CGameData::Inst()->getCardBaseInfByCid(cid);
    
    CCPoint ptCard(238, 300);
    char buf[100];
    sp = CGameData::Inst()->getHeadSprite(cid);
    if(sp != NULL){
        sp->setPosition(ptCard);
        addChild(sp);
        sp->setTag(112);
        
        //究极金边
        CCSprite* sp1 = NULL;
        if (pCardInf->bIsEluTgt) {
            sp1 = CCSprite::spriteWithSpriteFrameName("GoldFrm.png");
        }
        else if (pCardInf->bIsWkTgt) {
            sp1 = CCSprite::spriteWithSpriteFrameName("wakeFrame.png");
        }
        
        if (sp1) {
            sp->addChild(sp1);
            sp1->setAnchorPoint(CCPointMake(0, 1));
            sp1->setPosition(ccp(4, sp->getContentSize().height-4));
        }
    }
    
    // +?
    if (m_bEluEvo){
        TextNode* lbText = TextNode::textWithString("+?", 22);
        lbText->setPosition(CCPointMake(ptCard.x + 22, ptCard.y - 30));
        lbText->setColor(ccGREEN);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
        
        CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(1.0),
                                                                           CCFadeOut::actionWithDuration(0.2),
                                                                           CCFadeIn::actionWithDuration(0.2),
                                                                           NULL);
        lbText->runActionInside(CCRepeatForever::actionWithAction(action1));
    }
    else if (m_plus > 0) {
        CCSprite* spTmp = CCSprite::spriteWithFile("plusnum.png");
        
        if (spTmp) {
            CCSize sz = spTmp->getContentSize();
            snprintf(buf, 99, "/%d", m_plus);
            CCLabelAtlas* lbPlus = CCLabelAtlas::labelWithString(buf, "plusnum.png", sz.width / 11, sz.height, '/');
            lbPlus->setAnchorPoint(CCPointMake(1, 0));
            lbPlus->setPosition(CCPointMake(95, 10));
            sp->addChild(lbPlus);
            
            CCActionInterval* action1 = (CCActionInterval*)CCSequence::actions(CCDelayTime::actionWithDuration(1.0),
                                                                               CCFadeOut::actionWithDuration(0.2),
                                                                               CCFadeIn::actionWithDuration(0.2),
                                                                               NULL);
            lbPlus->runAction(CCRepeatForever::actionWithAction(action1));
        }
    }
    
    CCSprite* spSkill;
    if(pCardInf->skid == 0)
        spSkill = CCSprite::spriteWithSpriteFrameName("fr_skill gray.png");
    else
        spSkill = CCSprite::spriteWithSpriteFrameName("fr_skill.png");
    
    if(spSkill != NULL){
        spSkill->setPosition(CCPointMake(ptCard.x - 28, ptCard.y - 62));
        addChild(spSkill);
    }
    
    CCSprite* spLSkill;
    if(pCardInf->leader_skid == 0)
        spLSkill = CCSprite::spriteWithSpriteFrameName("fr_lskill gray.png");
    else
        spLSkill = CCSprite::spriteWithSpriteFrameName("fr_lskill.png");
    
    if(spLSkill != NULL){
        spLSkill->setPosition(CCPointMake(ptCard.x + 26, ptCard.y - 62));
        addChild(spLSkill);
    }
    
    
    //數值
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("lv"));
    
    TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y + 26));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    sprintf(buf, "%d", 1);
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y + 26));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    if (m_bEluEvo) {
        lbText->setString("?");
        lbText->setColor(ccGREEN);
    }
    else if(m_bWake)
    {
        //snprintf(buf, sizeof(buf), "%d", pUsrCardInf->lv);
        
        if(pUsrCardInf->lv < pCardInf->max_lv)
        {
            sprintf(buf, "%d", pUsrCardInf->lv);
        }
        else
        {
            sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("max"));
        }
        
        lbText->setString(buf);
    }
    
    if (pCardInf->skid > 0) {
        snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("skill_lv"));
        lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
        lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y + 2));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
        
        if (pUsrCardInf->skid != pCardInf->skid) {
            sprintf(buf, "%d", 1);
        }
        else {
            stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(pCardInf->skid);
            if (sklv == pManSkill->cMaxLv) {
                sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("max"));
            }
            else {
                if (m_bEluEvo)
                    sprintf(buf, "?");
                else
                    sprintf(buf, "%d", sklv);
            }
        }
        
        lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
        lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y + 2));
        lbText->setAnchorPoint(CCPointZero);
        if (buf[0] == '?')
            lbText->setColor(ccGREEN);
        else
            lbText->setColor(ccWHITE);
        lbText->setShadowColor(ccBLACK);
        addChild(lbText, 1);
    }
        
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("hp"));
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y - 22));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    if (m_bWake) {
        long hp = pCardInf->hp + (pUsrCardInf->lv - 1) * pCardInf->hp_growth + pUsrCardInf->pl_hp*HPPLUSGROWTH;
        sprintf(buf, "%ld", hp);
    }
    else{
        sprintf(buf, "%ld", pCardInf->hp + plus_hp * HPPLUSGROWTH);
    }
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y - 22));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("attack"));
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y - 46));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    if (m_bWake) {
        long attack = pCardInf->attack + (pUsrCardInf->lv - 1) * pCardInf->attack_growth + pUsrCardInf->pl_attack*ATKPLUSGROWTH;
        sprintf(buf, "%ld", attack);
    }
    else{
        sprintf(buf, "%lu", pCardInf->attack + plus_attack * ATKPLUSGROWTH);
    }
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y - 46));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("recover"));
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y - 70));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    if (m_bWake) {
        long recover = pCardInf->recover + (pUsrCardInf->lv - 1) * pCardInf->recover_growth + pUsrCardInf->pl_recover*RECPLUSGROWTH;
        sprintf(buf, "%ld", recover);
    }
    else{
        sprintf(buf, "%ld", pCardInf->recover + plus_recover * RECPLUSGROWTH);
    }
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
    lbText->setPosition(CCPointMake(ptCard.x + 58, ptCard.y - 70));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
}

//******************************************************************************
// showAlertLock
//******************************************************************************
void Evolution::showAlertLock()
{
    if (m_dlgAlertLock == NULL) {
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
        
        const char* title = CGameData::Inst()->getLanguageValue("alert");
        char buf[100];
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("evolution_lock_alert"));
        
        m_dlgAlertLock = new Dialog2(this, callfuncO_selector(Evolution::cbAlertLock), title, buf, enDl2OneBtn);
        addChild(m_dlgAlertLock, 100);
        m_dlgAlertLock->setPosition(CCPointMake(320, 480 - this->getPosition().y));
        m_dlgAlertLock->release();
    }
}

//******************************************************************************
// cbAlertLock
//******************************************************************************
void Evolution::cbAlertLock(CCObject* pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    removeChild(m_dlgAlertLock, true);
    m_dlgAlertLock = NULL;
}

//******************************************************************************
// showSevoRule
//******************************************************************************
void Evolution::showSevoRule()
{
    CCLayerColor* lc = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 128), 640, 960);
    if (lc) {
        addChild(lc, 100, kSevoRuleDlgTag);
        
        CCSprite* spDlg = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
        if(spDlg){
            spDlg->setScale(2.0);
            lc->addChild(spDlg);
            
            spDlg->setPosition(ccp(320, 480));
        }
        
        CCSprite* spTitle = CCSprite::spriteWithSpriteFrameName("sevo_ruletitle.png");
        if (spTitle) {
            lc->addChild(spTitle);
            spTitle->setPosition(ccp(320, 656));
        }
        
        CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if (spMenu1 && spMenu2) {
            CCMenuItem *sureItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(Evolution::cbSevoRule));
            
            TextNode* itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"), kBtnTitleHeight);
            itemLabel->setColor(ccc3(255, 255, 255));
            itemLabel->setShadowColor(ccBLACK);
            itemLabel->setPosition(ccp(spMenu1->getContentSize().width*0.5, spMenu1->getContentSize().height*0.5 + 2));
            sureItem->addChild(itemLabel);
            
            sureItem->setAnchorPoint(ccp(0, 0));
            sureItem->setPosition(ccp(240, 258));
            CCMenu *menu = CCMenu::menuWithItem(sureItem);
            menu->setPosition(CCPointZero);
            lc->addChild(menu);
        }
        
        OcProxy::Inst()->showWebWithType(enWebviewLoadTypeRequest,
                                         CGameData::Inst()->getUrlDepDevice((char*)(CGameData::Inst()->getCommonInfo()->evolution_notice.c_str())),
                                         CCRect(36*2, 176*2, 248*2, 122*2),
                                         true);
    }
}

//******************************************************************************
// cbSevoRule
//******************************************************************************
void Evolution::cbSevoRule(CCObject *pSender)
{
    OcProxy::Inst()->removeWebview();
    
    removeChildByTag(kSevoRuleDlgTag, true);
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}

//******************************************************************************
// showWakeRule
//******************************************************************************
void Evolution::showWakeRule()
{
    CCLayerColor* lc = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 128), 640, 960);
    if (lc) {
        addChild(lc, 100, kWakeRuleDlgTag);
        
        CCSprite* spDlg = CCSprite::spriteWithSpriteFrameName("fr_window2.png");
        if(spDlg){
            spDlg->setScale(2.0);
            lc->addChild(spDlg);
            
            spDlg->setPosition(ccp(320, 480));
        }
        
        CCSprite* spTitle = CCSprite::spriteWithSpriteFrameName("wake_ruletitle.png");
        if (spTitle) {
            lc->addChild(spTitle);
            spTitle->setPosition(ccp(320, 656));
        }
        
        CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        if (spMenu1 && spMenu2) {
            CCMenuItem *sureItem = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(Evolution::cbWakeRule));
            
            TextNode* itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"), kBtnTitleHeight);
            itemLabel->setColor(ccc3(255, 255, 255));
            itemLabel->setShadowColor(ccBLACK);
            itemLabel->setPosition(ccp(spMenu1->getContentSize().width*0.5, spMenu1->getContentSize().height*0.5 + 2));
            sureItem->addChild(itemLabel);
            
            sureItem->setAnchorPoint(ccp(0, 0));
            sureItem->setPosition(ccp(240, 258));
            CCMenu *menu = CCMenu::menuWithItem(sureItem);
            menu->setPosition(CCPointZero);
            lc->addChild(menu);
        }
        
        OcProxy::Inst()->showWebWithType(enWebviewLoadTypeRequest,
                                         CGameData::Inst()->getUrlDepDevice((char*)(CGameData::Inst()->getCommonInfo()->wake_notice.c_str())),
                                         CCRect(36*2, 176*2, 248*2, 122*2),
                                         true);
    }
}

//******************************************************************************
// cbWakeRule
//******************************************************************************
void Evolution::cbWakeRule(CCObject *pSender)
{
    OcProxy::Inst()->removeWebview();
    
    removeChildByTag(kWakeRuleDlgTag, true);
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
}

//******************************************************************************
// onExit
//******************************************************************************
void Evolution::onExit()
{
    OcProxy::Inst()->removeWebview();
    
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}