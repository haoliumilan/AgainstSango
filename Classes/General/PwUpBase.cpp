//
//  PwUpBase.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-30.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "PwUpBase.h"
#include "CGamedata.h"
#include "CSkillLib.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "Arrow.h"
#include "MsgBox.h"
#include "CShowCardInf.h"
#include "TextNode.h"

using namespace CocosDenshion;

#define kNewbieEndTag           82
#define kNoFoodInPackageTag     83
#define kNiePanTargetBaseTag    1000

//******************************************************************************
// Constructor
//******************************************************************************
PwUpBase::PwUpBase(CCObject* target, SEL_CallFuncO selector, bool bNiePan, bool bOver)
{
    m_bCanNiePan = false;
    m_bNiePan = bNiePan;
    m_msgbox = NULL;
    m_spBtnStart = NULL;
    m_spBtnAdd = NULL;
    m_bMoved = false;
    tempLv = 0;
    aCount = 0;
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_showCard = new CShowCardInf();
    addChild(m_showCard, 1000);
    m_showCard->release();
    m_showCard->setIsVisible(false);
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,kCCMenuTouchPriority+1, true);
    
    m_iFoodExp = 0;
    m_bOver = bOver;
    m_bGoldEnough = false;

    if (m_bNiePan && !m_bOver)
    {
        showNiePanInfo();
    }
    else
    {
        if(bOver == false){
            initButton();
            showFood();
        }else {
            if (!CGameData::Inst()->chkLogin()) {
                MainScene::Inst()->showTouchTip();
            }
        }
        
        showBaseCardInf();
        
        if((!m_bGoldEnough) && (m_spBtnStart)){
            CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_long button black.png");
            sp->setPosition(m_spBtnStart->getPosition());
            addChild(sp, 10);
        }
    }    
}

void PwUpBase::showNiePanInfo()
{
    showNiePanFood();
    
    char buf[100];
    char *ucidBase = CGameData::Inst()->getCombineCard(0);
    CUserCardInfo *pBaseCard = CGameData::Inst()->getUsrCardInfByUcid(ucidBase);
    snprintf(buf, 99, CGameData::Inst()->getLanguageValue("niepan_dsp"), pBaseCard->np_need_num);
    TextNode* tnDsp = TextNode::textWithString(buf, 22);
    tnDsp->setColor(ccc3(138, 218, 68));
    tnDsp->setShadowColor(ccBLACK);
    tnDsp->setPosition(CCPointMake(320, 375));
    addChild(tnDsp);
    
    showNiePanTarget();
    showBaseCardInf();
    showNiePanBtn();
}

void PwUpBase::waitShowInfo()
{
    CCLog("hahaha");
    if(!MainScene::Inst()->new_FirstPowerUp)
    isLongPress = true;
    CCPoint pos = convertToNodeSpace(beganPoint);
    
    CCSize size;
    CCPoint ptCard;
    bool bRet = false;
    unschedule(schedule_selector(PwUpBase::waitShowInfo));
        
    //主卡
    
    CCSprite *sp = (CCSprite *)getChildByTag(121);
    CCPoint pt = sp->getPosition();
    CCSize sz = sp->getContentSize();
    
     bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                       pt.y - sz.height * 0.5, 
                                                       sz.width, 
                                                       sz.height), 
                                            pos);
    if(bRet)
    {
        char *uid = CGameData::Inst()->getCombineCard(0);
        CUserCardInfo* pUserCardInfo = CGameData::Inst()->getUsrCardInfByUcid(uid);
        m_showCard->show(pUserCardInfo);
        return;
    }

    if (m_bNiePan)
    {        
        for(int i = 1; i < 6; i++){
            CCSprite* spTarget = (CCSprite*)getChildByTag(kNiePanTargetBaseTag + i);
            if (!spTarget)
                continue;
            
            CCPoint targetPos = spTarget->getPosition();
            CCSize targetSz = spTarget->getContentSize();
            
            bRet = CCRect::CCRectContainsPoint(CCRectMake(targetPos.x - targetSz.width * 0.5,
                                                          targetPos.y - targetSz.height * 0.5,
                                                          targetSz.width,
                                                          targetSz.height),
                                               pos);
            
            if(bRet){
                char *ucidBase = CGameData::Inst()->getCombineCard(0);
                CUserCardInfo *pBaseCard = CGameData::Inst()->getUsrCardInfByUcid(ucidBase);
                int index = 1;
                for (list<short>::iterator itCid = pBaseCard->np_list_target.begin();
                     itCid != pBaseCard->np_list_target.end();
                     ++itCid) {
                    if (index == i)
                    {
                        short tempCid = *itCid;
                        CCardBaseInfo* pCardBaseInfo = CGameData::Inst()->getCardBaseInfByCid(tempCid);
                        m_showCard->show(pCardBaseInfo);
                        break;
                    }
                    
                    index++;
                }
            }
        }
    }
    else
    {
        CCPoint ptStart(97, 363);
        
        for(int i = 1; i < 6; i++){
            
            CCPoint foodPos = (CCPointMake(ptStart.x + 112 * (i - 1), ptStart.y));
            
            
            bRet = CCRect::CCRectContainsPoint(CCRectMake(foodPos.x - sz.width * 0.5,
                                                          foodPos.y - sz.height * 0.5,
                                                          sz.width, sz.height),
                                               pos);
            
            if(bRet){
                char *uid = CGameData::Inst()->getCombineCard(i);
                if(*uid==0)
                    return;
                CUserCardInfo* pUserCardInfo = CGameData::Inst()->getUsrCardInfByUcid(uid);
                m_showCard->show(pUserCardInfo);
                break;
            }
        }
    }
}
//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool PwUpBase::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    beganPoint = touchLocation;
    
    CCSize size;
    CCPoint ptCard;
    
    if(!MainScene::Inst()->new_FirstPowerUp)
    {
        if (!m_bOver) {
            schedule(schedule_selector(PwUpBase::waitShowInfo),0.3);
        }
    }
    

    return true;
}

void PwUpBase::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) 
{
    if(isLongPress)
        return;
    unschedule(schedule_selector(PwUpBase::waitShowInfo));
    m_bMoved = true;
}

void PwUpBase::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    unschedule(schedule_selector(PwUpBase::waitShowInfo));
    
    CCPoint touchLocation = pTouch->locationInView( pTouch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    
    if(m_bMoved && ccpDistance(touchLocation, beganPoint) > kListBoxMinMovY)
    {
        m_bMoved =false;
        return;
    }
    if(isLongPress)
    {
        isLongPress = false;
        return;
    }
    
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    bool bRet = false;
    
    
    if(m_bOver){
        //新手引导
        if(CGameData::Inst()->chkLogin() == false){
            MainScene::Inst()->removeTouchTip();
            MainScene::Inst()->enableBottomPanel(false);
            
            Dialog1* newbieEndDlg = new Dialog1(this, callfuncO_selector(PwUpBase::newbieEnd), 
                                                CGameData::Inst()->getLanguageValue("newBieLayerTip5"),false);
            addChild(newbieEndDlg, 500, kNewbieEndTag);
            newbieEndDlg->release();
            newbieEndDlg->setPosition(CCPointMake(320, 300));
            newbieEndDlg->setScale(0.0);
            newbieEndDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
            
            return ;
        }
        
        unschedule(schedule_selector(PwUpBase::lvlUpAnim));
        SimpleAudioEngine::sharedEngine()->stopAllEffects();
        
        m_ret.iSel = 2;
        (m_pListener->*m_pfnSelector)(&m_ret);
        return ;
    }
    
    
    CCPoint pt = m_spBtnAdd->getPosition();
    CCSize sz = m_spBtnAdd->getContentSize();
    
    bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5, 
                                                  pt.y - sz.height * 0.5, 
                                                  sz.width, 
                                                  sz.height), 
                                       pos);
    if(bRet){ // 追加
        if (m_bNiePan
            && !CGameData::Inst()->getIsNiePanFoodInPackage())
        {
            showNoFoodInPackage();
            return;
        }
        
        m_ret.iSel = 0;
        (m_pListener->*m_pfnSelector)(&m_ret);
        return;
    }
    
    pt = m_spBtnStart->getPosition();
    sz = m_spBtnStart->getContentSize();
    bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5,
                                                  pt.y - sz.height * 0.5,
                                                  sz.width,
                                                  sz.height),
                                       pos);
    
    if(bRet){   // 合成
        if (!m_bGoldEnough)
            return;
        
        if (m_bNiePan)  // 涅槃
        {
            if (m_bCanNiePan)
            {
                m_ret.iSel = 1;
                (m_pListener->*m_pfnSelector)(&m_ret);
            }
        }
        else    // 强化
        {
            if((CGameData::Inst()->getCombineCardCnt() > 0))
            {
                bool isHasThreeStars = false;
                for (int i = 1; i < 6; i++) {
                    char *uid = CGameData::Inst()->getCombineCard(i);
                    if (uid[0] != 0) {
                        CUserCardInfo* pUserCardInfo = CGameData::Inst()->getUsrCardInfByUcid(uid);
                        if (pUserCardInfo->star >= 3) {
                            isHasThreeStars = true;
                            break;
                        }
                    }
                    
                }
                
                if (isHasThreeStars && !MainScene::Inst()->new_FirstPowerUp) {
                    // 所选材料超过三星
                    MainScene::Inst()->showFoodUpThreeStars();
                }
                else {
                    m_ret.iSel = 1;
                    (m_pListener->*m_pfnSelector)(&m_ret);
                }
            }
        }
    }
    
}

//******************************************************************************
// showNoFoodInPackage
//******************************************************************************
void PwUpBase::showNoFoodInPackage()
{
    Dialog1* NoFoodDlg = new Dialog1(this, callfuncO_selector(PwUpBase::cbNoFoodInPackage),
                                    CGameData::Inst()->getLanguageValue("nipan_nofood"),false);
    addChild(NoFoodDlg, 500, kNoFoodInPackageTag);
    NoFoodDlg->release();
    NoFoodDlg->setPosition(CCPointMake(320, 480));
    NoFoodDlg->setScale(0.0);
    NoFoodDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
}

void PwUpBase::cbNoFoodInPackage()
{
    CCNode* node = getChildByTag(kNoFoodInPackageTag);
    if (node)
    {
        node->removeFromParentAndCleanup(true);
        node = NULL;
    }
}

//******************************************************************************
// showBaseCardInf
//******************************************************************************
void PwUpBase::showBaseCardInf()
{
    float niepanValue = 0.0;
    if (m_bNiePan)
        niepanValue = 25;
    
    tempLv = 0;
    CCSprite* spExpFrm = CCSprite::spriteWithFile("pa_upgrade.png");
    if(spExpFrm != NULL){
        addChild(spExpFrm, 1);
        spExpFrm->setPosition(CCPointMake(400, 596 + niepanValue));
    }
    
    CCSprite* spBaseCardFrm = CCSprite::spriteWithFile("pa_generals.png");
    if(spBaseCardFrm != NULL){
        addChild(spBaseCardFrm, 1);
        spBaseCardFrm->setPosition(CCPointMake(170, 610 + niepanValue));
    }
    
    
    char* ucid = CGameData::Inst()->getCombineCard(0);
    
    CUserCardInfo* pUsrCardInf = CGameData::Inst()->getUsrCardInfByUcid(ucid);
    
    if(m_bOver  == false)
        CGameData::Inst()->preUpCardInfo = *pUsrCardInf;
    else {
        cur_cardInfo = pUsrCardInf;
    }
    if(pUsrCardInf == NULL)
        return;
    
    char buf[100];
    CCSprite* sp = CGameData::Inst()->getHeadSprite(pUsrCardInf->cid);
    if(sp != NULL){
        sp->setPosition(CCPointMake(90, 605 + niepanValue));
        sp->setTag(121);
        addChild(sp, 1);
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
        }
    }
    
    CCSprite* spSkill;
    if(pUsrCardInf->skid == 0)
        spSkill = CCSprite::spriteWithSpriteFrameName("fr_skill gray.png");
    else
        spSkill = CCSprite::spriteWithSpriteFrameName("fr_skill.png");
    
    if(spSkill != NULL){
        spSkill->setPosition(CCPointMake(62, 543 + niepanValue));
        addChild(spSkill, 1);
    }
    
    CCSprite* spLSkill;
    if(pUsrCardInf->leader_skid == 0)
        spLSkill = CCSprite::spriteWithSpriteFrameName("fr_lskill gray.png");
    else
        spLSkill = CCSprite::spriteWithSpriteFrameName("fr_lskill.png");
    
    if(spLSkill != NULL){
        spLSkill->setPosition(CCPointMake(116, 543 + niepanValue));
        addChild(spLSkill, 1);
    }
    
    
    //數值
    TextNode* lbText = TextNode::textWithString("Lv:", CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
    lbText->setPosition(CCPointMake(148, 631 + niepanValue));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);

    if (pUsrCardInf->lv == pUsrCardInf->max_lv) {
        sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("max"));
    }
    else {
        sprintf(buf, "%d", pUsrCardInf->lv);
    }
    m_lvLabel = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
    m_lvLabel->setPosition(CCPointMake(148, 631 + niepanValue));
    m_lvLabel->setAnchorPoint(CCPointZero);
    m_lvLabel->setColor(ccWHITE);
    addChild(m_lvLabel, 1);
    
    if (pUsrCardInf->skid > 0) {
        sprintf(buf, "%s:", CGameData::Inst()->getLanguageValue("skill_lv"));
        lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
        lbText->setPosition(CCPointMake(148, 607 + niepanValue));
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
        lbText->setPosition(CCPointMake(148, 607 + niepanValue));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccWHITE);
        addChild(lbText, 1);
    }
        
    sprintf(buf, "%s:", CGameData::Inst()->getLanguageValue("hp"));
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
    lbText->setPosition(CCPointMake(148, 583 + niepanValue));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
//    sprintf(buf, "%ld", pUsrCardInf->hp);
     sprintf(buf, "%ld", pUsrCardInf->hp.get());
    m_hpLabel = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
    m_hpLabel->setPosition(CCPointMake(148, 583 + niepanValue));
    m_hpLabel->setAnchorPoint(CCPointZero);
    m_hpLabel->setColor(ccWHITE);
    addChild(m_hpLabel, 1);
    
    sprintf(buf, "%s:", CGameData::Inst()->getLanguageValue("attack"));
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
    lbText->setPosition(CCPointMake(148, 559 + niepanValue));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
//    sprintf(buf, "%lu", pUsrCardInf->attack);
    sprintf(buf, "%lu", pUsrCardInf->attack.get());
    m_atLabel = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
    m_atLabel->setPosition(CCPointMake(148, 559 + niepanValue));
    m_atLabel->setAnchorPoint(CCPointZero);
    m_atLabel->setColor(ccWHITE);
    addChild(m_atLabel, 1);
    
    sprintf(buf, "%s:", CGameData::Inst()->getLanguageValue("recover"));
    lbText = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentLeft, 22);
    lbText->setPosition(CCPointMake(148, 535 + niepanValue));
    lbText->setAnchorPoint(CCPointZero);
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 1);
    
    sprintf(buf, "%ld", pUsrCardInf->recover);
    m_recLabel = TextNode::textWithString(buf, CCSizeMake(156, 24), CCTextAlignmentRight, 22);
    m_recLabel->setPosition(CCPointMake(148, 535 + niepanValue));
    m_recLabel->setAnchorPoint(CCPointZero);
    m_recLabel->setColor(ccWHITE);
    addChild(m_recLabel, 1);
        
    showExpPanel(pUsrCardInf);
}

//******************************************************************************
// newbieAllEnd
//******************************************************************************
void PwUpBase::newbieEnd(CCObject* pObj)
{
    CCNode* newbieEndDlg = getChildByTag(kNewbieEndTag);
    if(newbieEndDlg)
        removeChild(newbieEndDlg, true);
    
    MainScene::Inst()->reLogin();
}

//******************************************************************************
// showExpPanel
//******************************************************************************
void PwUpBase::showExpPanel(CUserCardInfo* pUsrCardInf)
{
    float niepanValue = 0.0;
    if (m_bNiePan)
        niepanValue = 25;

    char buf[100];
    snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("pwUp_exp_current"));
    TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(270, 24), CCTextAlignmentLeft, 22);
    lbText->setPosition(CCPointMake(342, 650 + niepanValue));
    lbText->setAnchorPoint(CCPointMake(0, 1));
    lbText->setColor(ccBLACK);
    addChild(lbText, 1);
    
    snprintf(buf, 99, "%lu", pUsrCardInf->exp);
    m_expLabel = TextNode::textWithString(buf, CCSizeMake(270, 24), CCTextAlignmentRight, 22);
    m_expLabel->setPosition(CCPointMake(342, 650 + niepanValue));
    m_expLabel->setAnchorPoint(CCPointMake(0, 1));
    m_expLabel->setColor(ccBLACK);
    addChild(m_expLabel, 1);
    
    if (!m_bOver) {
        sprintf(buf, "%s:", CGameData::Inst()->getLanguageValue("coin"));
        lbText = TextNode::textWithString(buf, CCSizeMake(270, 24), CCTextAlignmentLeft, 22);
        lbText->setPosition(CCPointMake(342, 540 + niepanValue));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccBLACK);
        addChild(lbText, 1);
        
        int iGold = 0;
        int iCardCnt = CGameData::Inst()->getCombineCardCnt();
        if (iCardCnt != 0) {
            int iBasePlus = pUsrCardInf->plus;
            int iAllPlus = iBasePlus;
            char* ucid;
            CUserCardInfo *pFoodCardInfo;
            for (int i = 0; i < iCardCnt; i++) {
                ucid = CGameData::Inst()->getCombineCard(i + 1);
                pFoodCardInfo = CGameData::Inst()->getUsrCardInfByUcid(ucid);
                iAllPlus += pFoodCardInfo->plus;
            }
            
            ucid = CGameData::Inst()->getCombineCard(0);
            pUsrCardInf = CGameData::Inst()->getUsrCardInfByUcid(ucid);
            
            if (iAllPlus > iBasePlus)
                iGold = pUsrCardInf->lv * 100 * iCardCnt + 1000 * iAllPlus;
            else
                iGold = pUsrCardInf->lv * 100 * iCardCnt;
        }
        
        CUserInfo* pUsrInf = CGameData::Inst()->getUsrInfo();
        m_bGoldEnough = (pUsrInf->gold >= iGold);
        
        snprintf(buf, 99, "%d", iGold);
        TextNode *text = TextNode::textWithString(buf, CCSizeMake(270, 24), CCTextAlignmentRight, 22);
        text->setPosition(CCPointMake(342, 540 + niepanValue));
        text->setAnchorPoint(CCPointZero);
        text->setColor(ccBLACK);
        addChild(text, 1);
    
        if(!m_bGoldEnough){
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.2, 128),
                                                                               CCFadeTo::actionWithDuration(0.2, 255),
                                                                               NULL);
            text->setColor(ccRED);
            text->setShadowColor(ccBLACK);
            text->runActionInside(CCRepeatForever::actionWithAction(action));
        }
    }
    else {
        //合成成功，大成功，超成功......
        char pwUpSucTypeChar[100];
        switch (CGameData::Inst()->getPwUpSucType()) {
            case enPwupSucType_Normal:
                sprintf(pwUpSucTypeChar, "%s", CGameData::Inst()->getLanguageValue("pwUp_success_normal"));
                break;
            case enPwupSucType_Big:
                sprintf(pwUpSucTypeChar, "%s", CGameData::Inst()->getLanguageValue("pwUp_success_big"));
                break;
            case enPwupSucType_Super:
                sprintf(pwUpSucTypeChar, "%s", CGameData::Inst()->getLanguageValue("pwUp_success_super"));
                break;
                
            default:
                sprintf(pwUpSucTypeChar, "%s", CGameData::Inst()->getLanguageValue("pwUp_success_normal"));
                break;
        }
        
        lbText = TextNode::textWithString(pwUpSucTypeChar, CCSizeMake(270, 24), CCTextAlignmentLeft, 22);
        lbText->setPosition(CCPointMake(362, 540 + niepanValue));
        lbText->setAnchorPoint(CCPointZero);
        lbText->setColor(ccBLACK);
        addChild(lbText, 1);
        
        if(CGameData::Inst()->getPwUpSucType() == enPwupSucType_Big ||
           CGameData::Inst()->getPwUpSucType() == enPwupSucType_Super){
            TextNode *text = TextNode::textWithString(pwUpSucTypeChar, CCSizeMake(270, 24), CCTextAlignmentLeft, 22);
            text->setPosition(CCPointMake(362, 540 + niepanValue));
            text->setAnchorPoint(CCPointZero);
            if (CGameData::Inst()->getPwUpSucType() == enPwupSucType_Big) {
                text->setColor(ccc3(0, 192, 255));
            }
            else if (CGameData::Inst()->getPwUpSucType() == enPwupSucType_Super) {
                text->setColor(ccc3(255, 0, 0));
            }
            addChild(text, 1);
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.2, 128),
                                                                               CCFadeTo::actionWithDuration(0.2, 255),
                                                                               NULL);
            text->runActionInside(CCRepeatForever::actionWithAction(action));
        }
    }
     
    unsigned long ulCurExpBase = CGameData::Inst()->getExpByType(pUsrCardInf->exp_type, pUsrCardInf->lv);
    unsigned long ulNextLvExp = CGameData::Inst()->getExpByType(pUsrCardInf->exp_type, pUsrCardInf->lv+1);
    unsigned long ulNeedExp = ulNextLvExp - pUsrCardInf->exp;
    float fBarPerc = (pUsrCardInf->exp - ulCurExpBase) * 1.0 / (ulNextLvExp - ulCurExpBase);
    fBarPerc = (fBarPerc > 1.0)?1.0: fBarPerc;
    float fRedBarPerc = (pUsrCardInf->exp + m_iFoodExp - ulCurExpBase) * 1.0 / (ulNextLvExp - ulCurExpBase);
    fRedBarPerc = (fRedBarPerc > 1.0)?1.0: fRedBarPerc;
    
    if(pUsrCardInf->lv == pUsrCardInf->max_lv){
        m_spExpFrm = NULL;
        m_spExpBar = NULL;
        
        lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("pwUp_lv_max"), CCSizeMake(270, 24), CCTextAlignmentLeft, 22);
        lbText->setPosition(CCPointMake(342, 620 + niepanValue));
        lbText->setAnchorPoint(CCPointMake(0, 1));
        lbText->setColor(ccBLACK);
        addChild(lbText, 1);
    }
    else{
        snprintf(buf, 99, "%s:", CGameData::Inst()->getLanguageValue("pwUp_exp_need"));
        m_upExpTitleLabel = TextNode::textWithString(buf, CCSizeMake(270, 24), CCTextAlignmentLeft, 22);
        m_upExpTitleLabel->setPosition(CCPointMake(342, 620 + niepanValue));
        m_upExpTitleLabel->setAnchorPoint(CCPointMake(0, 1));
        m_upExpTitleLabel->setColor(ccBLACK);
        addChild(m_upExpTitleLabel, 1);
        
        snprintf(buf, 99, "%lu", ulNeedExp);
        m_upExpLabel = TextNode::textWithString(buf, CCSizeMake(270, 24), CCTextAlignmentRight, 22);
        m_upExpLabel->setPosition(CCPointMake(342, 620 + niepanValue));
        m_upExpLabel->setAnchorPoint(CCPointMake(0, 1));
        m_upExpLabel->setColor(ccBLACK);
        addChild(m_upExpLabel, 1);
        
        spRedBar = CCSprite::spriteWithFile("pa_bar_red.png");
        if(spRedBar != NULL){
            addChild(spRedBar, 9);
            spRedBar->setPosition(CCPointMake(343, 580 + niepanValue));
            spRedBar->setAnchorPoint(CCPointMake(0, 0.5));
            spRedBar->setScaleX(fRedBarPerc);
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCFadeTo::actionWithDuration(0.3, 128),
                                                                               CCFadeTo::actionWithDuration(0.3, 255),
                                                                               NULL);
            spRedBar->runAction(CCRepeatForever::actionWithAction(action));
        }
        
        m_spExpFrm = CCSprite::spriteWithFile("pa_bar.png");
        if(m_spExpFrm != NULL){
            addChild(m_spExpFrm, 1);
            m_spExpFrm->setPosition(CCPointMake(332, 580 + niepanValue));
            m_spExpFrm->setAnchorPoint(CCPointMake(0, 0.5));
        }
        
        m_spExpBar = CCSprite::spriteWithSpriteFrameName("id_exp_bar_pwup.png");
        if(m_spExpBar != NULL){
            addChild(m_spExpBar, 10);
            m_spExpBar->setPosition(CCPointMake(343, 580 + niepanValue));
            m_spExpBar->setAnchorPoint(CCPointMake(0, 0.5));
            m_spExpBar->setScaleX(fBarPerc);
        }
    }
    
    if(pUsrCardInf->exp!=CGameData::Inst()->preUpCardInfo.exp && m_bOver == true 
       && pUsrCardInf->lv != CGameData::Inst()->preUpCardInfo.max_lv
       && pUsrCardInf->lv != pUsrCardInf->max_lv)
    {
        schedule(schedule_selector(PwUpBase::lvlUpAnim),0.05);
        //if(CGameData::Inst()->isSoundOn)
         //   SimpleAudioEngine::sharedEngine()->playEffect("addExp.wav", true);
        if(spRedBar)
        {
//            spRedBar->stopAllActions();
            spRedBar->setIsVisible(false);
        }
        tempLv = 0;
         
    }
    
    if (pUsrCardInf->sk_lv > CGameData::Inst()->preUpCardInfo.sk_lv)
    {
        CCSprite* lvUpSp = CCSprite::spriteWithFile("skillupsuccess.png");
        addChild(lvUpSp,100);
        lvUpSp->setPosition(ccp(320, 300));
        
        if (pUsrCardInf->i_AwakeNum > CGameData::Inst()->preUpCardInfo.i_AwakeNum)
            lvUpSp->setPosition(ccp(320, 360));
    }
    
     if (pUsrCardInf->i_AwakeNum > CGameData::Inst()->preUpCardInfo.i_AwakeNum)
     {
         CCSprite* lvUpSp = CCSprite::spriteWithFile("awakeupsuccess.png");
         addChild(lvUpSp,100);
         lvUpSp->setPosition(ccp(320, 300));
         
         if (pUsrCardInf->sk_lv > CGameData::Inst()->preUpCardInfo.sk_lv)
             lvUpSp->setPosition(ccp(320, 190));
     }
}

void  PwUpBase::setBaseCardInf(CUserCardInfo *pUsrCardInf)
{
    char buf[50];
    
    if (pUsrCardInf->lv == pUsrCardInf->max_lv) {
        sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("max"));
    }
    else {
        sprintf(buf, "%d", pUsrCardInf->lv);
    }
    
    m_lvLabel->setString(buf);
    
//    sprintf(buf, "%ld", pUsrCardInf->hp);
    sprintf(buf, "%ld", pUsrCardInf->hp.get());
    
    m_hpLabel->setString(buf);
    
//    sprintf(buf, "%lu", pUsrCardInf->attack);
    sprintf(buf, "%lu", pUsrCardInf->attack.get());
    m_atLabel->setString(buf);

    sprintf(buf, "%ld", pUsrCardInf->recover);
    m_recLabel->setString(buf);
    
    sprintf(buf, "%ld",pUsrCardInf->exp);
    m_expLabel->setString(buf); 
    
}
 void PwUpBase::lvlUpAnim()
{
    
    
    if(aCount%20 == 0)
    {
        SimpleAudioEngine::sharedEngine()->playEffect("addExp.wav");
        if(aCount == 0)
        {
            setBaseCardInf(&CGameData::Inst()->preUpCardInfo);
        }
        //动画到达升级地方时 
        int delta = cur_cardInfo->lv - CGameData::Inst()->preUpCardInfo.lv;
        if(tempLv == delta)
            
        {
            SimpleAudioEngine::sharedEngine()->stopAllEffects();

            if(cur_cardInfo->lv == cur_cardInfo->max_lv)
            {
                char buf[50];
                m_spExpBar->setScaleX(1);
                sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("max"));
                m_lvLabel->setString(buf);
                
            }else {
                m_spExpBar->setScaleX(0);
                unsigned long ulCurExpBase = CGameData::Inst()->getExpByType(cur_cardInfo->exp_type, cur_cardInfo->lv);
                unsigned long ulNextLvExp = CGameData::Inst()->getExpByType(cur_cardInfo->exp_type, cur_cardInfo->lv+1);
                float per = (cur_cardInfo->exp-ulCurExpBase)*1.0/(ulNextLvExp-ulCurExpBase);
                m_spExpBar->runAction(CCScaleTo::actionWithDuration(0.75, per, 1));
                tempLv = 0;
                setBaseCardInf(cur_cardInfo);
                unschedule(schedule_selector(PwUpBase::lvlUpAnim));
            }
        }else {//一般情况
            m_spExpBar->setScaleX(0);
            m_spExpBar->runAction(CCScaleTo::actionWithDuration(0.75, 1, 1));
            
            CCardBaseInfo* pBaseInf = CGameData::Inst()->getCardBaseInfByCid(cur_cardInfo->cid);
        
            
            int  lv =  CGameData::Inst()->preUpCardInfo.lv+tempLv;
            unsigned long exp;
            
            if (tempLv == 0) {
                exp = CGameData::Inst()->preUpCardInfo.exp;
            }
            else{
                exp = CGameData::Inst()->getExpByType(cur_cardInfo->exp_type,lv);
            }
            
            long   attack = pBaseInf->attack + pBaseInf->attack_growth * (lv -1) + cur_cardInfo->pl_attack * ATKPLUSGROWTH;
            int  hp = pBaseInf->hp + pBaseInf->hp_growth * (lv -1) + cur_cardInfo->pl_hp * HPPLUSGROWTH;
            int  recover = pBaseInf->recover + pBaseInf->recover_growth * (lv -1) + cur_cardInfo->pl_recover * RECPLUSGROWTH;
            
            char buf[50];
            
            if (lv == cur_cardInfo->max_lv) {
                sprintf(buf, "%s", CGameData::Inst()->getLanguageValue("max"));
            }
            else {
                sprintf(buf, "%d", lv);
            }
            m_lvLabel->setString(buf);
            sprintf(buf, "%d", hp);
            m_hpLabel->setString(buf);
            sprintf(buf, "%ld", attack);
            m_atLabel->setString(buf);            
            sprintf(buf, "%d",recover);
            m_recLabel->setString(buf);  
            sprintf(buf, "%ld",exp);
            m_expLabel->setString(buf);
            tempLv++;
        }

    }

    aCount++;

}

void PwUpBase::showNiePanTarget()
{
    char *ucidBase = CGameData::Inst()->getCombineCard(0);
    CUserCardInfo *pBaseCard = CGameData::Inst()->getUsrCardInfByUcid(ucidBase);
    int targetCount = pBaseCard->np_list_target.size();

    CCSprite* spFrameMiddle = CCSprite::spriteWithFile("np_target_middle.png");
    addChild(spFrameMiddle);
    spFrameMiddle->setPosition(CCPointMake(320, 290));
    
    float middleWidth = targetCount * 112.0 + 20;
    float middleScale = middleWidth / spFrameMiddle->getContentSize().width;
    spFrameMiddle->setScaleX(middleScale);

    CCSprite* spFrameLeft = CCSprite::spriteWithFile("np_target_left.png");
    addChild(spFrameLeft);
    spFrameLeft->setPosition(CCPointMake(320 - middleWidth * 0.49, 290));
    
    CCSprite* spFrameRight = CCSprite::spriteWithFile("np_target_right.png");
    addChild(spFrameRight);
    spFrameRight->setPosition(CCPointMake(320 + middleWidth * 0.49, 290));
    
    int i = 1;
    CCSprite* spTemp;
    CCPoint ptStart(376 - (middleWidth - 20) * 0.5, 290);
    for (list<short>::iterator itCid = pBaseCard->np_list_target.begin();
         itCid != pBaseCard->np_list_target.end();
         ++itCid) {
        short tempCid = *itCid;
        
        spTemp = CGameData::Inst()->getHeadSprite(tempCid);
        if(spTemp != NULL){
            spTemp->setPosition(CCPointMake(ptStart.x + 112 * (i - 1), ptStart.y));
            addChild(spTemp, 1, kNiePanTargetBaseTag + i);
            i++;
        }
    }
}

void PwUpBase::showNiePanFood()
{
    m_iFoodExp = 0;
    
    //素材卡背景框
    CCSprite* spMaterialFrm = CCSprite::spriteWithFile("pa_material_box.png");
    if(spMaterialFrm != NULL){
        addChild(spMaterialFrm);
        spMaterialFrm->setPosition(CCPointMake(320, 480));
    }
    
    if (CGameData::Inst()->getCombineCardCnt() > 0)
    {
        char *ucidBase = CGameData::Inst()->getCombineCard(0);
        CUserCardInfo *pBaseCard = CGameData::Inst()->getUsrCardInfByUcid(ucidBase);
        
        if (CGameData::Inst()->getCombineCardCnt() == pBaseCard->np_need_num)
            m_bCanNiePan = true;
        else
            m_bCanNiePan = false;
        
        showNiePanHaveFood();
    }
    else
    {
        m_bCanNiePan = false;
        showNiePanNeedFood();
    }
    
    //text
    char buf[100];
    snprintf(buf, 99, "%s: %d", CGameData::Inst()->getLanguageValue("pwUp_exp_get"), m_iFoodExp);
    TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(400, 30), CCTextAlignmentRight, 24);
    lbText->setPosition(CCPointMake(603, 522));
    lbText->setAnchorPoint(CCPointMake(1, 0.5));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);
    
}

//******************************************************************************
// showNiePanHaveFood
//******************************************************************************
void PwUpBase::showNiePanHaveFood()
{
    CUserCardInfo* pUsrCard;
    CCSprite* sp;
    char* ucid;
    char buf[100];
    CCPoint ptStart(97, 453);
        
    char *ucidBase = CGameData::Inst()->getCombineCard(0);
    CUserCardInfo *pBaseCard = CGameData::Inst()->getUsrCardInfByUcid(ucidBase);
    int baseCardCType = pBaseCard->ctype;
    int npNeedNum = pBaseCard->np_need_num;
    
    for(int i = 1; i <= npNeedNum; i++){
        ucid = CGameData::Inst()->getCombineCard(i);
        if(ucid[0] == 0)
            break;
        
        pUsrCard = CGameData::Inst()->getUsrCardInfByUcid(ucid);
        sp = CGameData::Inst()->getHeadSprite(pUsrCard->cid);
        if(sp != NULL){
            sp->setPosition(CCPointMake(ptStart.x + 112 * (i - 1), ptStart.y));
            addChild(sp);
        }
        
        if (pUsrCard->plus > 0) {
            CCSprite* spTmp = CCSprite::spriteWithFile("plusnum.png");
            
            if (spTmp) {
                CCSize sz = spTmp->getContentSize();
                snprintf(buf, 99, "/%d", pUsrCard->plus);
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
        
        if (pUsrCard->ctype == baseCardCType) {
            m_iFoodExp += (pUsrCard->base_exp * pUsrCard->lv) * 1.5;
        }
        else {
            m_iFoodExp += (pUsrCard->base_exp * pUsrCard->lv);
        }
    }
}

//******************************************************************************
// showNiePanNeedFood
//******************************************************************************
void PwUpBase::showNiePanNeedFood()
{
    CCSprite* sp;
    CCPoint ptStart(97, 453);

    char *ucidBase = CGameData::Inst()->getCombineCard(0);
    CUserCardInfo *pBaseCard = CGameData::Inst()->getUsrCardInfByUcid(ucidBase);
    int i = 1;
    list<short> listNpNeed = pBaseCard->np_list_show;
    
    for (list<short>::iterator itCid = listNpNeed.begin();
         itCid != listNpNeed.end();
         ++itCid) {
        short tempCid = *itCid;
        
        sp = CGameData::Inst()->getHeadSprite(tempCid);
        if(sp != NULL){
            sp->setPosition(CCPointMake(ptStart.x + 112 * (i - 1), ptStart.y));
            addChild(sp);
            i++;
            
            CCSprite* spShadow = CCSprite::spriteWithSpriteFrameName("fr_black.png");
            spShadow->setPosition(sp->getPosition());
            addChild(spShadow);
        }
    }
}

//******************************************************************************
// showFood
//******************************************************************************
void PwUpBase::showFood()
{
    CUserCardInfo* pUsrCard;
    CCSprite* sp;
    char* ucid;
    char buf[100];
    CCPoint ptStart(97, 363);
    m_iFoodExp = 0;
    
    //素材卡背景框
    CCSprite* spMaterialFrm = CCSprite::spriteWithFile("pa_material_box.png");
    if(spMaterialFrm != NULL){
        addChild(spMaterialFrm);
        spMaterialFrm->setPosition(CCPointMake(320, 390));
    }
    
    char *ucidBase = CGameData::Inst()->getCombineCard(0);
    CUserCardInfo *pBaseCard = CGameData::Inst()->getUsrCardInfByUcid(ucidBase);
    int baseCardCType = pBaseCard->ctype;
    
    for(int i = 1; i < 6; i++){
        ucid = CGameData::Inst()->getCombineCard(i);
        if(ucid[0] == 0)
            break;
        
        pUsrCard = CGameData::Inst()->getUsrCardInfByUcid(ucid); 
        sp = CGameData::Inst()->getHeadSprite(pUsrCard->cid);
        if(sp != NULL){
            sp->setPosition(CCPointMake(ptStart.x + 112 * (i - 1), ptStart.y));
            addChild(sp);
        }
        
        if (pUsrCard->plus > 0) {
            CCSprite* spTmp = CCSprite::spriteWithFile("plusnum.png");
            
            if (spTmp) {
                CCSize sz = spTmp->getContentSize();
                snprintf(buf, 99, "/%d", pUsrCard->plus);
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
        
        if (pUsrCard->ctype == baseCardCType) {
            m_iFoodExp += (pUsrCard->base_exp * pUsrCard->lv) * 1.5;
        }
        else {
            m_iFoodExp += (pUsrCard->base_exp * pUsrCard->lv);
        }
    }
    
    //text
    snprintf(buf, 99, "%s: %d", CGameData::Inst()->getLanguageValue("pwUp_exp_get"), m_iFoodExp);
    TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(400, 30), CCTextAlignmentRight, 24);
    lbText->setPosition(CCPointMake(603, 432));
    lbText->setAnchorPoint(CCPointMake(1, 0.5));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);
    
    //无饲料是按钮灰
    if(CGameData::Inst()->getCombineCardCnt() == 0){
        CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_long button black.png");
        if(sp != NULL){
            addChild(sp, 3);
            sp->setPosition(CCPointMake(490, 206));
        }
    }
    
    if(CGameData::Inst()->isNewbie()){
        Arrow* pArrow = new Arrow(30, 255);
        pArrow->setRotation(-90);
        addChild(pArrow, 10);
        pArrow->release();
        
        if(CGameData::Inst()->getCombineCardCnt() == 0){
            pArrow->setPosition(CCPointMake(148, 240));
        }
        else{
            pArrow->setPosition(CCPointMake(482, 240));
        }

        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));
    }
    if(MainScene::Inst()->new_FirstPowerUp)
    {
        Arrow* pArrow = new Arrow(30, 255);
        pArrow->setRotation(-90);
        addChild(pArrow, 10);
        pArrow->release();
        
        if(CGameData::Inst()->getCombineCardCnt() == 0){
            pArrow->setPosition(CCPointMake(148, 240));
        }
        else{
            pArrow->setPosition(CCPointMake(482, 240));
        }
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                           NULL);
        pArrow->runAction(CCRepeatForever::actionWithAction(action));
    }
}

void PwUpBase::showNiePanBtnAnim()
{
    CGameData::Inst()->addBigImageByRGBA4444("shandian.plist");
    
    float button_width = 0;
    float button_height = 0;
        
    CCSprite* sp_yellow_left = CCSprite::spriteWithSpriteFrameName("guangyun1.png");
    CCSprite* sp_yellow_right = CCSprite::spriteWithSpriteFrameName("guangyun1.png");
    if (m_spBtnStart)
    {
        button_width = m_spBtnStart->getContentSize().width;
        button_height = m_spBtnStart->getContentSize().height;
        
        m_spBtnStart->addChild(sp_yellow_left, 1);
        sp_yellow_left -> setPosition(CCPointMake(53, button_height/2+3));
        sp_yellow_left -> setScale(2.5);
        m_spBtnStart->addChild(sp_yellow_right, 1);
        sp_yellow_right-> setPosition(CCPointMake(100, button_height/2+3));
        sp_yellow_right -> setScale(2.5);
    }
    
    // 2个黄色的光
    if (sp_yellow_right && sp_yellow_left)
    {
        sp_yellow_left->runAction(ScaleAction());
        sp_yellow_right->runAction(ScaleAction());
    }
    
    // 外边的闪电  播放好了再图片转180度播放一边
    CCSprite* sp_flash = CCSprite::spriteWithSpriteFrameName("blank.png");
    if (sp_flash)
    {
        m_spBtnStart->addChild(sp_flash);
        sp_flash->setPosition(CCPointMake(button_width/2-3, button_height/2+3));
        sp_flash->runAction(FlashAction());
    }
}

CCAction* PwUpBase::ScaleAction()
{
    CCScaleTo* scale_small = CCScaleTo::actionWithDuration(0.5, 1);
    CCScaleTo* scale_big   = CCScaleTo::actionWithDuration(0.5, 2.5);
    CCFiniteTimeAction* action_big_small = CCSequence::actions(scale_small, scale_big, NULL);
    CCAction* rt = CCRepeatForever::actionWithAction((CCActionInterval*)action_big_small);
    return rt;
}

CCAction* PwUpBase::FlashAction()
{
    CCFiniteTimeAction* action = getActionFromFile("labelniepan00%02d.png", 10.f);
    CCRotateBy* rotate180 = CCRotateBy::actionWithDuration(0, 180);
    CCFiniteTimeAction* action_once = CCSequence::actions(action, rotate180, action, rotate180, NULL);
    
    CCRepeatForever* rt = CCRepeatForever::actionWithAction((CCActionInterval*)action_once);
    return rt;
}


CCActionInterval* PwUpBase::getActionFromFile(const char* filename)
{
    return getActionFromFile(filename, 20.f);
}

CCActionInterval* PwUpBase::getActionFromFile(const char* filename, float delta_time)
{
    char buf[100];
    CCAnimation* animation;
	CCActionInterval*  action;
    CCSpriteFrame* frame;
    
    animation = CCAnimation::animation();
    int i = 1;
    while(1) {
        snprintf(buf, 99, filename, i);
        
        CCSprite* tmpSP = CCSprite::spriteWithSpriteFrameName(buf);
        if (tmpSP){
            frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(buf);
            animation->addFrame(frame);
        }
        else
            break;
        i++;
    }
    
    frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    animation->addFrame(frame);
    action = CCAnimate::actionWithDuration(i/delta_time, animation, false);
        
    return action;
}

void PwUpBase::showNiePanBtn()
{
    //追加素材按钮
    m_spBtnAdd = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    if(m_spBtnAdd != NULL){
        addChild(m_spBtnAdd);
        m_spBtnAdd->setPosition(CCPointMake(150, 182));
    }
    
    TextNode* lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("pwUp_button_add"), CCSizeMake(120, 30), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(m_spBtnAdd->getContentSize().width * 0.5 - 2,
                                    m_spBtnAdd->getContentSize().height * 0.5 + 2));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    m_spBtnAdd->addChild(lbText, 2);
    
    //涅槃开始按钮
    m_spBtnStart = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    if(m_spBtnStart != NULL){
        addChild(m_spBtnStart);
        m_spBtnStart->setPosition(CCPointMake(490, 182));
    }
    
    CCSprite* spNiePan = CCSprite::spriteWithFile("title_niepan.png");
    spNiePan->setPosition(CCPointMake(m_spBtnStart->getContentSize().width * 0.5 - 2,
                                      m_spBtnStart->getContentSize().height * 0.5 + 2));
    m_spBtnStart->addChild(spNiePan, 2);
    
    if (m_bCanNiePan && m_bGoldEnough)
    {
        showNiePanBtnAnim();
    }
    else
    {
        CCSprite* spShadow = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
        spShadow->setPosition(m_spBtnStart->getPosition());
        addChild(spShadow);
    }
}

//******************************************************************************
// initButton
//******************************************************************************
void PwUpBase::initButton()
{
    //追加素材按钮
    m_spBtnAdd = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    if(m_spBtnAdd != NULL){
        addChild(m_spBtnAdd);
        m_spBtnAdd->setPosition(CCPointMake(150, 206));
    }
    
    TextNode* lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("pwUp_button_add"), CCSizeMake(120, 30), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(150, 206));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);
    
    //合成开始按钮
    m_spBtnStart = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    if(m_spBtnStart != NULL){
        addChild(m_spBtnStart);
        m_spBtnStart->setPosition(CCPointMake(490, 206));
    }
    
    lbText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("pwUp_button_pwUp"), CCSizeMake(120, 30), CCTextAlignmentCenter, 30);
    lbText->setPosition(CCPointMake(490, 206));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    addChild(lbText, 2);
}

//******************************************************************************
// onExit
//******************************************************************************
void PwUpBase::onExit()
{
    if (m_bNiePan && !m_bOver)
    {
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("shandian.plist");
    }

    
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}
