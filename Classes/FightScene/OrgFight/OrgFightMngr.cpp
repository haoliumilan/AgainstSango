//
//  OrgFightMngr.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 13-1-6.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "OrgFightMngr.h"
#include "OrgStoneGrid.h"
#include "CPlayerBlood.h"
#include "CMonsterBlood.h"
#include "CCardInfoDlg.h"
#include "FightPauseMenu.h"
#include "CCShake.h"
#include "OrgStoreEnergy.h"
#include "CommDlg.h"
#include "HttpComm.h"
#include "Dialog1.h"
#include "OrgSaveData.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

#define kCardTagBase    199
#define kCardBkTagBase  19
#define kEnemyCvrTag    874
#define kBossDieDlgTag  888
#define kFinalComboTag  4455
#define kJumpComboTag   5566

OrgFightMngr* OrgFightMngr::s_pcIf = NULL;

//******************************************************************************
// init
//******************************************************************************
void OrgFightMngr::init(CCObject* target, SEL_CallFuncO selector, string dg_id, long lEnemyCid ,bool isTMLS)
{
    if(m_bInitFlg)
        return;
    
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_lEnemyCid = lEnemyCid;
    m_iMaxCombo = 0;
    
    //back dark cover
    m_lcCover = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 0), 640, 960);
    addChild(m_lcCover, -100);
    
    //stonegrid
    m_stoneGrid = new OrgStoneGrid(this, callfunc_selector(OrgFightMngr::cbStoneGrid));
    addChild(m_stoneGrid, 10);
    m_stoneGrid->release();
    
    //card info dlg
    m_cardInfoDlg = CCardInfoDlg::initDlg();
    addChild(m_cardInfoDlg, 10);
    m_cardInfoDlg->setPosition(CCPointMake(320, 815));
    m_cardInfoDlg->setIsVisible(false);
    
    //menu
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("menu.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("menu.png");
    m_menu = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(OrgFightMngr::menuCallback));
    CCMenu* menu = CCMenu::menuWithItems( m_menu,  NULL);
    addChild(menu, 10);
    m_menu->setAnchorPoint(ccp(0, 0));
    m_menu->setPosition(CCPointMake(544, 930));
    menu->setPosition(CCPointZero);
    m_menu->setIsVisible(false);
    
    m_menuLayer = NULL;
    
    m_bInitFlg = true;
    
    m_iHP = 0;
    m_iBaseHP = 0;
    m_iCardCnt = 0;
    m_storeEngy = NULL;
    m_commDlg = NULL;
    m_strDgId = dg_id;
    m_bIsTongMengLianShou = isTMLS;
    
    
    m_playerPos = CCPointMake(320, 545);
    
    memset(m_OrgCardInfArr, 0, sizeof(m_OrgCardInfArr));
}

//******************************************************************************
// startNewFight
//******************************************************************************
void OrgFightMngr::startNewFight()
{
    if(m_bFighting)
        return;
    
    m_lcCover->setOpacity(0);
    m_lcCover->runAction(CCSequence::actions(CCFadeIn::actionWithDuration(0.5),
                                             CCCallFunc::actionWithTarget(this, callfunc_selector(OrgFightMngr::cbStartFight)),
                                             NULL));
    
    m_bFighting = true;
}

//******************************************************************************
// enterArenaEnd
//******************************************************************************
void OrgFightMngr::enterArenaEnd()
{
    reqDgEnd();
}

//******************************************************************************
// cbStartFight
//******************************************************************************
void OrgFightMngr::cbStartFight()
{
    if (!OrgSaveData::Inst()->hasValidSavedData())
    {
        OrgSaveData::Inst()->initOrgFightInfo();
    }
    
    m_stoneGrid->initMatrix();
    m_stoneGrid->startNewRound();
    CGameState::Instance()->setState(enStateWaitTouch);
    
    placeFightScene();
    
    if (OrgSaveData::Inst()->hasValidSavedData())
    {
        placeCardsFromSaveData();

        placeHpBar();
        
        m_iComboIdx = OrgSaveData::Inst()->getComboIndex();
        m_bIsTongMengLianShou = OrgSaveData::Inst()->getIsTongMengLianShouChang();
        m_iMaxCombo = OrgSaveData::Inst()->getMaxCombo();
        if (m_iMaxCombo > 0) {
            showFinalCombo(m_iMaxCombo);
        }
        
        if (m_iComboIdx > 0)
            procStepAnim();
    }
    else{
        placeCards();
        placeHpBar();
        
        m_iComboIdx = 0;
    }
    
    placeEnemy();
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -10, true);
    
    m_menu->setIsVisible(true);
    
    saveCurFightInfo();
    
    m_iCurCombo = 0;
}

//******************************************************************************
// cbStoneGrid
//******************************************************************************
void OrgFightMngr::cbStoneGrid()
{
    if(m_iComboIdx < kComboArrLen){
        switch (CGameState::Instance()->getState()) {
            case enStatePlayerAttackPrepared:
            case enStateEnemyAttack:
                m_iCurCombo = 0;
                m_iComboIdx++;
                procStepAnim();
                break;
                
            case enStateTouchEnd:
                m_iCurCombo++;
                showJumpCombo(m_iCurCombo);
                break;
                
            case enStateClrCombo:
                hideJumpCombo();
                showFinalCombo(m_iCurCombo);
                break;
                
            default:
                break;
        }
    }
}

//******************************************************************************
// showJumpCombo
//******************************************************************************
void OrgFightMngr::showJumpCombo(int iCombo)
{
    if (iCombo < 2) {
        return;
    }
    
    CCNode* spInk = getChildByTag(kJumpComboTag);
    if (spInk == NULL) {
        spInk = CCSprite::spriteWithFile("OrgInk.png");
        if (spInk) {
            spInk->setPosition(ccp(480, 100));
            addChild(spInk, 20, kJumpComboTag);
            
            CCSprite* spCombo = CCSprite::spriteWithFile("OrgCombo.png");
            if (spCombo) {
                spInk->addChild(spCombo);
                spCombo->setAnchorPoint(ccp(1, 0.5));
                spCombo->setPosition(ccp(spInk->getContentSize().width, spInk->getContentSize().height*0.5));
            }
        }
    }
    
    if (spInk) {
        spInk->removeChildByTag(iCombo-1, true);
        
        CCSprite *sp = CCSprite::spriteWithFile("OrgCmbAtlas.png");
        if (sp) {
            char buf[10];
            snprintf(buf, 9, "%d", iCombo);
            
            CCLabelAtlas *lbb = CCLabelAtlas::labelWithString(buf,
                                                              "OrgCmbAtlas.png",
                                                              sp->getContentSize().width / 10,
                                                              sp->getContentSize().height, '0');
            spInk->addChild(lbb, 0, iCombo);
            lbb->setPosition(ccp(70, 90));
            lbb->setAnchorPoint(ccp(0.5, 0.5));
            lbb->runAction(CCMoveTo::actionWithDuration(0.1, ccp(50, 66)));
        }
    }
}

//******************************************************************************
// hideJumpCombo
//******************************************************************************
void OrgFightMngr::hideJumpCombo()
{
    removeChildByTag(kJumpComboTag, true);
}

//******************************************************************************
// showFinalCombo
//******************************************************************************
void OrgFightMngr::showFinalCombo(int iCombo)
{
    if (getChildByTag(kFinalComboTag) != NULL) {
        //return;
        hideFinalCombo();
    }
    
    CCSprite* spCombo = CCSprite::spriteWithFile("OrgCombo.png");
    if (spCombo) {
        addChild(spCombo, 0, kFinalComboTag);
        spCombo->setScale(0.5);
        spCombo->setPosition(ccp(582, 724));
        
        CCSprite *sp = CCSprite::spriteWithFile("OrgCmbAtlas.png");
        if (sp) {
            char buf[10];
            snprintf(buf, 9, "%d", MAX(iCombo, m_iMaxCombo.get()));
            
            CCLabelAtlas *lbb = CCLabelAtlas::labelWithString(buf,
                                                              "OrgCmbAtlas.png",
                                                              sp->getContentSize().width / 10,
                                                              sp->getContentSize().height, '0');
            spCombo->addChild(lbb);
            lbb->setAnchorPoint(ccp(0.5, 0));
            lbb->setScale(2.0);
            lbb->setPosition(ccp(spCombo->getContentSize().width * 0.5, spCombo->getContentSize().height));
            
            if (iCombo > m_iMaxCombo) {
                lbb->runAction(CCSequence::actions(CCScaleTo::actionWithDuration(0.1, 2.0, 2.2),
                                                   CCScaleTo::actionWithDuration(0.1, 2.0, 2.0),
                                                   NULL));
                
                m_iMaxCombo = iCombo;
            }
        }
    }
}

//******************************************************************************
// hideFinalCombo
//******************************************************************************
void OrgFightMngr::hideFinalCombo()
{
    removeChildByTag(kFinalComboTag, true);
}

//******************************************************************************
// placeFightScene
//******************************************************************************
void OrgFightMngr::placeFightScene()
{
    CCSprite* sp = CCSprite::spriteWithFile("cj_org.jpg");
    if(sp){
        sp->setPosition(CCPointMake(320, 761));
        sp->setScale(1/0.79);
        addChild(sp, -20);
        sp->setOpacity(0);
        sp->runAction(CCFadeIn::actionWithDuration(1.0));
    }
}

//******************************************************************************
// placeCardsFromSaveData
//******************************************************************************
void OrgFightMngr::placeCardsFromSaveData()
{    
    int iPosIdx = 0;
    m_iCardCnt = 0;
    
    for(int i = 0; i < 5; i++){
        OrgSaveData_Card* cardInfo = OrgSaveData::Inst()->getCardInfo(i);
        if (cardInfo->cid == 0)
            continue;
                        
        m_iBaseHP += cardInfo->hp;
        
        m_OrgCardInfArr[iPosIdx].xpos = 55 + 106*iPosIdx;
        m_OrgCardInfArr[iPosIdx].ypos = 615;
        m_OrgCardInfArr[iPosIdx].width = 0;
        m_OrgCardInfArr[iPosIdx].height = 0;
        m_OrgCardInfArr[iPosIdx].iElem = cardInfo->element;
        
        //card sprite
        CCSprite* spHead = CGameData::Inst()->getHeadSprite(cardInfo->cid);
        if(spHead){
            spHead->setPositionInPixels(CCPointMake(55 + 106*iPosIdx, 615));
            addChild(spHead, 0, kCardTagBase+iPosIdx);
            
            CCSprite* spHeadBk = CCSprite::spriteWithFile("headw.png");
            if(spHeadBk){
                spHeadBk->setPositionInPixels(CCPointMake(55 + 106*iPosIdx, 615));
                addChild(spHeadBk, 1, kCardBkTagBase+iPosIdx);
                spHeadBk->setOpacity(0);
            }
            
            m_OrgCardInfArr[iPosIdx].width = spHead->getContentSize().width;
            m_OrgCardInfArr[iPosIdx].height = spHead->getContentSize().height;
        }
        
        iPosIdx++;
        m_iCardCnt++;
    }
        
    m_iHP = m_iBaseHP;
}

//******************************************************************************
// placeCards
//******************************************************************************
void OrgFightMngr::placeCards()
{
    stUid* pTeamInfo = CGameData::Inst()->getCurTeamInf();
    
    CUserCardInfo* pUsrCardInf;
    int iPosIdx = 0;
    m_iCardCnt = 0;
    
    for(int i = 0; i < 5; i++){
        if(pTeamInfo[i].uid[0] == 0)
            continue;
        
        pUsrCardInf = CGameData::Inst()->getUsrCardInfByUcid(pTeamInfo[i].uid);
        
        if(pUsrCardInf == NULL)
            continue;
        
        m_iBaseHP += pUsrCardInf->hp;

        m_OrgCardInfArr[iPosIdx].xpos = 55 + 106*iPosIdx;
        m_OrgCardInfArr[iPosIdx].ypos = 615;
        m_OrgCardInfArr[iPosIdx].width = 0;
        m_OrgCardInfArr[iPosIdx].height = 0;
        m_OrgCardInfArr[iPosIdx].iElem = pUsrCardInf->ctype;
        strncpy(m_OrgCardInfArr[iPosIdx].ucid, pTeamInfo[i].uid, 99);

        //card sprite
        CCSprite* spHead = CGameData::Inst()->getHeadSprite(pUsrCardInf->cid);
        if(spHead){
            spHead->setPositionInPixels(CCPointMake(55 + 106*iPosIdx, 615));
            addChild(spHead, 0, kCardTagBase+iPosIdx);
            
            CCSprite* spHeadBk = CCSprite::spriteWithFile("headw.png");
            if(spHeadBk){
                spHeadBk->setPositionInPixels(CCPointMake(55 + 106*iPosIdx, 615));
                addChild(spHeadBk, 1, kCardBkTagBase+iPosIdx);
                spHeadBk->setOpacity(0);
            }
            
            m_OrgCardInfArr[iPosIdx].width = spHead->getContentSize().width;
            m_OrgCardInfArr[iPosIdx].height = spHead->getContentSize().height;
        }
        
        iPosIdx++;
        m_iCardCnt++;
    }
    
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    m_iBaseHP = m_iBaseHP * (1 + cntryInfo->buffInfo[enBuffType_Hp].effect);

    m_iHP = m_iBaseHP;
}

//******************************************************************************
// placeBlood
//******************************************************************************
void OrgFightMngr::placeHpBar()
{
    m_blood = new CPlayerBlood(m_iHP, m_iBaseHP);
    m_blood->setPosition(m_playerPos);
    addChild(m_blood, 2);
    if(m_storeEngy == NULL){
        //蓄力条
        m_storeEngy = new OrgStoreEnergy();
        m_blood->addChild(m_storeEngy,-1);
        
        m_storeEngy->setPosition(ccp(530, 558));
    }
    m_blood->release();
}

//******************************************************************************
// placeEnemy
//******************************************************************************
void OrgFightMngr::placeEnemy()
{
    m_spEnemy = CGameData::Inst()->getBodySprite(m_lEnemyCid);
    if(m_spEnemy){
        m_EnemyPos = CCPointMake(320, 660 + m_spEnemy->getContentSize().height * 0.5);
        m_spEnemy->setPosition(m_EnemyPos);
        addChild(m_spEnemy);
        m_spEnemy->setOpacity(0);
        m_spEnemy->runAction(CCFadeIn::actionWithDuration(0.5));
        
        char buf[32];
        snprintf(buf, 31, "W_%ld_body.png", m_lEnemyCid);
        CCSprite* spEnemyCvr = CCSprite::spriteWithFile(buf);
        if (spEnemyCvr) {
            addChild(spEnemyCvr, 0, kEnemyCvrTag);
            
            spEnemyCvr->setPositionInPixels(m_EnemyPos);
            spEnemyCvr->setOpacity(0);
        }
        
        m_spRound = CCSprite::spriteWithSpriteFrameName("huihe.png");
        if (m_spRound) {
            m_spRound->setAnchorPoint(CCPointMake(0, 1));
            m_spRound->setPosition(ccp(m_EnemyPos.x - m_spEnemy->getContentSize().width*0.5, 916));
            addChild(m_spRound);
            m_spRound->setOpacity(0);
            m_spRound->runAction(CCFadeIn::actionWithDuration(0.5));
            
            CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
            if (spTemp) {
                char buf[32];
                snprintf(buf, 31, "%d", 3-m_iComboIdx);
                m_lbRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                m_lbRound->setAnchorPoint(CCPointMake(0, 1));
                m_lbRound->setPosition(ccp(m_EnemyPos.x - m_spEnemy->getContentSize().width*0.5 + m_spRound->getContentSize().width, 916));
                addChild(m_lbRound);
                m_lbRound->setOpacity(0);
                m_lbRound->runAction(CCFadeIn::actionWithDuration(0.5));
            }
        }
    }
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool OrgFightMngr::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    return showCardInf(touch);
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void OrgFightMngr::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    showCardInf(touch);
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void OrgFightMngr::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    m_cardInfoDlg->setIsVisible(false);
}

//******************************************************************************
// showCardInf
//******************************************************************************
bool OrgFightMngr::showCardInf(CCTouch *touch)
{
    bool bRet = false;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint nodePos = convertToNodeSpace( touchLocation );
    
    for (int i = 0; i < 5; i++) {
        if(m_OrgCardInfArr[i].xpos == 0)
            break;
        
        bRet = CCRect::CCRectContainsPoint(CCRectMake(m_OrgCardInfArr[i].xpos - m_OrgCardInfArr[i].width * 0.5,
                                                      m_OrgCardInfArr[i].ypos - m_OrgCardInfArr[i].height * 0.5,
                                                      m_OrgCardInfArr[i].width, m_OrgCardInfArr[i].height),
                                           nodePos);
        
        if(bRet){
            if (OrgSaveData::Inst()->hasValidSavedData())
            {
                OrgSaveData_Card* cardInfo = OrgSaveData::Inst()->getCardInfo(i);
                m_cardInfoDlg->setCardInfo(cardInfo->element,
                                           cardInfo->attack,
                                           cardInfo->card_name);
                m_cardInfoDlg->setIsVisible(true);
            }
            else
            {
                CUserCardInfo* pInf = CGameData::Inst()->getUsrCardInfByUcid(m_OrgCardInfArr[i].ucid);
                if(pInf){
                    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
                    
                    m_cardInfoDlg->setCardInfo(pInf->ctype,
                                               pInf->attack * (1 + cntryInfo->buffInfo[enBuffType_Attack].effect),
                                               pInf->name);
                    m_cardInfoDlg->setIsVisible(true);
                }
            }
            break;
        }
    }
    
    return bRet;
}

//******************************************************************************
// menuCallback
//******************************************************************************
void OrgFightMngr::menuCallback(CCObject* sender)
{
    if(CGameState::Instance()->getState() == enStateWaitTouch){
        if(m_menuLayer == NULL){
            m_menuLayer = new FightPauseMenu(this,menu_selector(OrgFightMngr::cbMenuLayer));
            addChild(m_menuLayer, 100);
            m_menuLayer->setTitleString("");
        }
        
        m_menu->setIsEnabled(false);
    }
}

//******************************************************************************
// cbMenuLayer
//******************************************************************************
void OrgFightMngr::cbMenuLayer(CCObject* sender)
{
    FightPauseMenuRet *iRet = (FightPauseMenuRet *)sender;

    switch (iRet->iBtnSel) {
        case 11:
        case 12:
            break;
        case 13:
            m_menuLayer->removeAllChildrenWithCleanup(true);
            m_menuLayer->removeFromParentAndCleanup(true);
            m_menuLayer = NULL;
            m_menu->setIsEnabled(true);

            break;
        case 21:
            QuitFight();
            break;
        default:
            break;
    }
}

//******************************************************************************
// cbTipsDlg
//******************************************************************************
void OrgFightMngr::QuitFight()
{
    (m_pListener->*m_pfnSelector)(&m_ret);
}

//******************************************************************************
// shoot
//******************************************************************************
void OrgFightMngr::shoot(const CCPoint& src, const CCPoint& dst, float dt)
{
    CCSprite* spBult = CCSprite::spriteWithSpriteFrameName("bullet.png");
    spBult->setPosition(src);
    addChild(spBult, 2);
    
    CCPoint bezierPos1 = dst;
    CCPoint bezierPos2 = dst;
    bezierPos1.x = (src.x + dst.x) * 0.5;
    bezierPos2.x = (src.x + dst.x) * 0.5;
    ccBezierConfig bezier;
    bezier.controlPoint_1 = bezierPos1;
    bezier.controlPoint_2 = bezierPos2;
    bezier.endPosition = dst;
    spBult->setIsVisible(false);
    spBult->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                          CCShow::action(),
                                          CCBezierTo::actionWithDuration(0.3, bezier),
                                          CCHide::action(),
                                          NULL) );
}

//******************************************************************************
// attackEnemy
//******************************************************************************
void OrgFightMngr::attackEnemy(int atkpt, float dt)
{
    schedule(schedule_selector(OrgFightMngr::PlayerAtkSndEffct), dt);
    
    const int iAnimStartXpos = 100;
    int iPosGap = (640 - iAnimStartXpos * 2) / (m_iCardCnt + 1);
    for (int i = 0; i < 5; i++) {
        if(m_OrgCardInfArr[i].xpos == 0)
            break;
        
        shoot(ccp(m_OrgCardInfArr[i].xpos, m_OrgCardInfArr[i].ypos), m_EnemyPos, dt);
        
        int ixpos = iAnimStartXpos + iPosGap * (i+1);
        runAtkAnim(m_OrgCardInfArr[i].iElem, ixpos, dt + 0.3);
    }
    
    m_spEnemy->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt + 0.3),
                                             CCShake::actionWithDuration(0.5, m_spEnemy->getPosition()),
                                             NULL));
    
    CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
    if(spTemp){
        CCLabelAtlas* lbAtkNum = CCLabelAtlas::labelWithString("0",
                                                               "rednum.png",
                                                               spTemp->getContentSize().width/10,
                                                               spTemp->getContentSize().height, '0');
        lbAtkNum->setPosition(m_spEnemy->getPosition());
        addChild(lbAtkNum, 6);
        lbAtkNum->setAnchorPoint(CCPointMake(0.5, 0.5));
        lbAtkNum->setIsVisible(false);
        
        char buf[100];
        snprintf(buf, 99, "%d", atkpt);
        lbAtkNum->setString(buf);
        lbAtkNum->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt + 0.3),
                                                CCShow::action(),
                                                CCScaleTo::actionWithDuration(0.1, 1.2),
                                                CCScaleTo::actionWithDuration(0.3, 1.0),
                                                CCFadeOut::actionWithDuration(1),
                                                NULL));
    }
}

//******************************************************************************
// attackPlayer
//******************************************************************************
void OrgFightMngr::attackPlayer(float dt)
{
    schedule(schedule_selector(OrgFightMngr::BossAtkSndEffct), dt);
    
    animHalo(m_EnemyPos, 2.0, dt);
    
    shoot(m_EnemyPos, m_playerPos, dt + 0.6);
    
    m_blood->setPosition(m_playerPos);
    m_blood->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt+0.2),
                                           CCShake::actionWithDuration(0.5, m_blood->getPosition()),
                                           NULL));
    
    
    m_blood->setBlood(0, dt+0.8,false);
    
    char buf[100];
    sprintf(buf, "/%d", m_iBaseHP);
    
    CCSprite *sp = CCSprite::spriteWithFile("fire_number.png");
    if(sp){
        CCLabelAtlas *lbb = CCLabelAtlas::labelWithString("0", "fire_number.png",sp->getContentSize().width / 11, sp->getContentSize().height, '/');
        lbb->setPosition(m_playerPos);
        lbb->setAnchorPoint(CCPointMake(0.5, 0.5));
        lbb->setString(buf);
        addChild(lbb, 3);
        lbb->setIsVisible(false);

        lbb->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt+0.8),
                                           CCShow::action(),
                                           CCScaleTo::actionWithDuration(0.1, 1.2),
                                           CCScaleTo::actionWithDuration(0.3, 1.0),
                                           CCSpawn::actions(CCMoveBy::actionWithDuration(1.0, CCPointMake(0, 50)),
                                                            CCFadeOut::actionWithDuration(1.0),
                                                            NULL),
                                           NULL));
    }
    
    CCNode* node = getChildByTag(kEnemyCvrTag);
    if (node) {
        node->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt+0.3),
                                            CCCallFunc::actionWithTarget(this, callfunc_selector(OrgFightMngr::BossBlinkSndEffct)),
                                            CCFadeTo::actionWithDuration(0.1, 128),
                                            CCFadeOut::actionWithDuration(0.1),
                                            CCCallFunc::actionWithTarget(this, callfunc_selector(OrgFightMngr::BossBlinkSndEffct)),
                                            CCFadeTo::actionWithDuration(0.1, 128),
                                            CCFadeOut::actionWithDuration(0.1),
                                            NULL));
    }

//    schedule(schedule_selector(OrgFightMngr::cbAttackPlayer), dt + 1.2);
}

//******************************************************************************
// cbAttackPlayer
//******************************************************************************
//void OrgFightMngr::cbAttackPlayer(ccTime dt)
//{
//    unschedule(schedule_selector(OrgFightMngr::cbAttackPlayer));
//}


//******************************************************************************
// runAtkAnim
//******************************************************************************
void OrgFightMngr::runAtkAnim(int iElem, int iXpos, float dt)
{
    CCAnimation* animation;
	char frameName[100] = {0};
	CCActionInterval*  action;
    CCSprite* spTemp;
    int i;
    
    animation = CCAnimation::animation();
    for(i=1;i<100;i++){
        switch (iElem) {
            case enElementWood:
                sprintf(frameName, "wood00%02d.png", i);
                break;
                
            case enElementWater:
                sprintf(frameName, "ice00%02d.png", i);
                break;
                
            case enElementLight:
                sprintf(frameName, "light00%02d.png", i);
                break;
                
            case enElementDark:
                sprintf(frameName, "dark00%02d.png", i);
                break;
                
            case enElementHeart:
                sprintf(frameName, "heart00%02d.png", i);
                break;
                
            case enElementFire:
            default:
                sprintf(frameName, "fire00%02d.png", i);
                break;
        }
        
        spTemp = CCSprite::spriteWithSpriteFrameName(frameName);
        if(spTemp == NULL)
            break;
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( frameName );
        animation->addFrame(frame);
    }
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    animation->addFrame(frame);
    action = CCAnimate::actionWithDuration(1.0 * i / 20, animation, false);
    
    CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("blank.png");
    addChild(spAnim);
    spAnim->setAnchorPoint(CCPointMake(0.5, 0));
    CCPoint pt = m_spEnemy->getPosition();
    pt.y -= (m_spEnemy->getContentSize().height * 0.5);
    pt.x = iXpos;
    spAnim->setPosition(pt);
    spAnim->setScale(2.0);
    spAnim->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                          action,
                                          NULL));
}

//******************************************************************************
// runSucAnim
//******************************************************************************
void OrgFightMngr::runSucAnim(float dt)
{
    CCAnimation* animation;
	char frameName[100] = {0};
	CCActionInterval*  action;
    CCSprite* spTemp;
    int i;
    
    animation = CCAnimation::animation();
    for(i=1;i<100;i++){
        sprintf(frameName, "taofa00%02d.png", i);

        spTemp = CCSprite::spriteWithSpriteFrameName(frameName);
        if(spTemp == NULL)
            break;
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( frameName );
        animation->addFrame(frame);
    }
    
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    animation->addFrame(frame);
    action = CCAnimate::actionWithDuration(1.0 * i / 20, animation, false);
    
    CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("blank.png");
    addChild(spAnim, 100);

    spAnim->setPosition(ccp(320, 800));
    spAnim->setScale(2.0);
    spAnim->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                          action,
                                          NULL));
    
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("tfcg.png");
    if(sp){
        addChild(sp, 100);
        
        sp->setPosition(ccp(320, 800));
        sp->setScale(0.4);
        sp->setIsVisible(false);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt+0.3),
                                          CCShow::action(),
                                          CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.2, 1.0), 0.6f),
                                          CCDelayTime::actionWithDuration(0.5),
                                          CCFadeOut::actionWithDuration(0.5),
                                          NULL));
    }
}

//******************************************************************************
// runFailAnim
//******************************************************************************
void OrgFightMngr::runFailAnim(float dt)
{
    CCSprite* sp = CCSprite::spriteWithFile("fail.png");

    if(sp){
        addChild(sp, 100);
        sp->setAnchorPoint(CCPointMake(0.5, 0));
        sp->setPosition(CCPointMake(320, 1500));
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                          CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(1.0f, ccp(320, 700)), 0.6f),
                                          NULL));
    }
}

//******************************************************************************
// reqDgEnd
//******************************************************************************
void OrgFightMngr::reqDgEnd()
{
    CGameData::Inst()->cntryDgEndReq(m_strDgId, m_strMaxCombo,m_bIsTongMengLianShou);
    scheduleUpdate();
}

//******************************************************************************
// update
//******************************************************************************
void OrgFightMngr::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting)
    {
        if(m_commDlg == NULL){
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(OrgFightMngr::cbCommDlg),
                                    enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480));
            m_commDlg->release();
        }
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    unscheduleUpdate();
    
    if(comState == kReqStatOk)
    {
        OrgSaveData::Inst()->destorySavedFile();
        procDgEnd();
    }
    else{
        if (403 == comState)
        {
            MainScene::Inst()->reloginWeibo(2);
            return;
        }
        
        if(m_commDlg == NULL){
            m_commDlg = new CommDlg(this, callfuncO_selector(OrgFightMngr::cbCommDlg), enCommDlgTp_retry);
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
// cbCommDlg
//******************************************************************************
void OrgFightMngr::cbCommDlg(cocos2d::CCObject * pObj)
{
    CommDlgRet* pCommDlgRet = (CommDlgRet*)pObj;
    bool bRet = pCommDlgRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    if(bRet){
        reqDgEnd();
    }else{
        CGameData::Inst()->clearReqStat();
        QuitFight();
    }
}

//******************************************************************************
// procDgEnd
//******************************************************************************
void OrgFightMngr::procDgEnd()
{
    m_ret.bCashin = true;
    
    int int1 = CGameData::Inst()->getCntryDgEndInf()->whokillwho;
    
    if (int1 == 0)      // 被其他人打死
        BossDie();
    else if (int1 == 1)  // 我打死boss
        YouWin();
    else if (int1 == 2)  // 我没打死boss
        YouLose();
}

//******************************************************************************
// country fight end state
//******************************************************************************
void OrgFightMngr::YouWin()
{
    m_storeEngy->useEnergy(1.2);
    
    for (int i = 0; i < 5; i++) {
        if(m_OrgCardInfArr[i].xpos == 0)
            break;
        
        CCNode* node = getChildByTag(kCardTagBase+i);
        CCPoint pt;
        if (node) {
            pt = node->getPositionInPixels();
            
            node->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.2),
                                                CCMoveBy::actionWithDuration(0.15, ccp(0, 5)),
                                                CCMoveTo::actionWithDuration(0.15, pt),
                                                NULL));
        }
        
        node = getChildByTag(kCardBkTagBase+i);
        if(node){
            node->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.3),
                                                CCFadeTo::actionWithDuration(0.15, 128),
                                                CCFadeOut::actionWithDuration(0.15),
                                                CCFadeTo::actionWithDuration(0.15, 128),
                                                CCFadeOut::actionWithDuration(0.15),
                                                CCFadeTo::actionWithDuration(0.15, 128),
                                                CCFadeOut::actionWithDuration(0.15),
                                                
                                                NULL));
            
        }

        animBom(pt, 1.0, 1.2);
    }
    
//    animHol(ccp(550, 758), 0.64, 1.2);
    animHol(ccp(596, 663), 1.0, 1.3);
//    animHol(ccp(550, 568), 0.64, 1.4);
    
    attackEnemy(CGameData::Inst()->getCntryDgEndInf()->playAtk, 1.5);

    //拉幕
    CCLayerColor* lc = CCLayerColor::layerWithColorWidthHeight(ccc4(255, 0, 0, 255), 640, 960);
    addChild(lc, 1000);
    lc->setIsVisible(false);
    
    lc->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(2.5),
                                      CCShow::action(),
                                      CCDelayTime::actionWithDuration(0.05),
                                      CCTintTo::actionWithDuration(0.05, 255, 255, 255),
                                      CCTintTo::actionWithDuration(0.05, 255, 0, 0),
                                      CCTintTo::actionWithDuration(0.05, 0, 0, 0),
                                      CCHide::action(),
                                      NULL));
    
    lc = CCLayerColor::layerWithColorWidthHeight(ccc4(255, 255, 255, 0), 640, 960);
    addChild(lc, 1000);
    
    lc->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(2.7),
                                      CCFadeIn::actionWithDuration(1.0),
                                      NULL));

    
    schedule(schedule_selector(OrgFightMngr::cbYouWin), 4.0);
}

void OrgFightMngr::cbYouWin(ccTime dt)
{
    unscheduleAllSelectors();
    
    QuitFight();
}

void OrgFightMngr::YouLose()
{
    m_storeEngy->useEnergy(1.2);
    
    for (int i = 0; i < 5; i++) {
        if(m_OrgCardInfArr[i].xpos == 0)
            break;
        
        CCNode* node = getChildByTag(kCardTagBase+i);
        CCPoint pt;
        if (node) {
            pt = node->getPositionInPixels();
            
            node->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.2),
                                                CCMoveBy::actionWithDuration(0.15, ccp(0, 5)),
                                                CCMoveTo::actionWithDuration(0.15, pt),
                                                NULL));
        }
        
        node = getChildByTag(kCardBkTagBase+i);
        if(node){
            node->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.3),
                                                CCFadeTo::actionWithDuration(0.15, 128),
                                                CCFadeOut::actionWithDuration(0.15),
                                                CCFadeTo::actionWithDuration(0.15, 128),
                                                CCFadeOut::actionWithDuration(0.15),
                                                CCFadeTo::actionWithDuration(0.15, 128),
                                                CCFadeOut::actionWithDuration(0.15),
                                                
                                                NULL));
            
        }

        animBom(pt, 1.0, 1.2);
    }
    
//    animHol(ccp(550, 758), 0.64, 1.2);
    animHol(ccp(596, 663), 1.0, 1.3);
//    animHol(ccp(550, 568), 0.64, 1.4);
    
    attackEnemy(CGameData::Inst()->getCntryDgEndInf()->playAtk, 1.5);
    attackPlayer(2.5);
    
    //拉幕
    CCLayerColor* lc = CCLayerColor::layerWithColorWidthHeight(ccc4(255, 0, 0, 255), 640, 960);
    addChild(lc, 1000);
    lc->setIsVisible(false);
    
    lc->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(4.5),
                                      CCShow::action(),
                                      CCDelayTime::actionWithDuration(0.05),
                                      CCTintTo::actionWithDuration(0.05, 0, 0, 0),
                                      CCTintTo::actionWithDuration(0.05, 255, 255, 255),
                                      CCHide::action(),
                                      NULL));
    
    lc = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 255), 640, 480);
    addChild(lc, 1000);

    lc->setPositionInPixels(ccp(0, 960));
    lc->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(4.7),
                                      CCMoveTo::actionWithDuration(0.25, ccp(0, 480)),
                                      NULL));
    
    lc = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 255), 640, 480);
    addChild(lc, 1000);

    lc->setPositionInPixels(ccp(0, -480));
    lc->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(4.7),
                                      CCMoveTo::actionWithDuration(0.25, ccp(0, 0)),
                                      NULL));
    
    schedule(schedule_selector(OrgFightMngr::cbYouLose), 5.2);
}

void OrgFightMngr::cbYouLose(ccTime dt)
{
    unscheduleAllSelectors();
    
    QuitFight();
}

//******************************************************************************
// BossDie
//******************************************************************************
void OrgFightMngr::BossDie()
{
    Dialog1* bossdieDlg = new Dialog1(this,
                                      callfuncO_selector(OrgFightMngr::cbBossDieDlg),
                                      CGameData::Inst()->getLanguageValue("cntrydgend_killedbyother"),
                                      false);
    addChild(bossdieDlg, 500, kBossDieDlgTag);
    bossdieDlg->release();
    bossdieDlg->setPosition(CCPointMake(320, 480));
    bossdieDlg->setScale(0.0);
    bossdieDlg->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
}

void OrgFightMngr::cbBossDieDlg(CCObject* pObj)
{
    CCNode* bossdieDlg = getChildByTag(kBossDieDlgTag);
    if(bossdieDlg)
        removeChild(bossdieDlg, true);
    
    QuitFight();
}

//******************************************************************************
// dostep
//******************************************************************************
void OrgFightMngr::procStepAnim()
{
    switch (m_iComboIdx) {
        case 1:
            dostep1();
            m_storeEngy->setStep(m_iComboIdx, 0.2);
            break;
            
        case 2:
            dostep2();
            m_storeEngy->setStep(m_iComboIdx, 0.2);
            break;
            
        case 3:
            {
                dostep3();
                m_storeEngy->setStep(m_iComboIdx, 0.2);
                
                char buf[16];
                snprintf(buf, sizeof(buf), "%09d", m_iMaxCombo.get());
                m_strMaxCombo = buf;
                reqDgEnd();
                
                setIsTouchEnabled(false);
            }
            break;
            
        default:
            break;
    }
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(OrgEnergyUp));
}

void OrgFightMngr::dostep1()
{
    for (int i = 0; i < 5; i++) {
        if(m_OrgCardInfArr[i].xpos == 0)
            break;
        
        CCNode* node = getChildByTag(kCardBkTagBase+i);
        if(node){
            node->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.3),
                                                CCFadeTo::actionWithDuration(0.15, 128),
                                                CCFadeOut::actionWithDuration(0.15),
                                                NULL));
            shoot(node->getPosition(), ccp(589, 663), 0);
        }
    }
    
    CCNode* node = getChildByTag(kEnemyCvrTag);
    if (node) {
        node->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.6),
                                            CCCallFunc::actionWithTarget(this, callfunc_selector(OrgFightMngr::BossBlinkSndEffct)),
                                            CCFadeTo::actionWithDuration(0.15, 128),
                                            CCFadeOut::actionWithDuration(0.15),
                                            NULL));
    }
    
    schedule(schedule_selector(OrgFightMngr::decEnemyRound), 0.6);
    schedule(schedule_selector(OrgFightMngr::cbStepOvr), 1.0);
}

void OrgFightMngr::dostep2()
{
    for (int i = 0; i < 5; i++) {
        if(m_OrgCardInfArr[i].xpos == 0)
            break;
        
        CCNode* node = getChildByTag(kCardBkTagBase+i);
        if(node){
            node->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.3),
                                                CCFadeTo::actionWithDuration(0.15, 128),
                                                CCFadeOut::actionWithDuration(0.15),
                                                NULL));
            shoot(node->getPosition(), ccp(589, 663), 0);
        }
    }
    
    CCNode* node = getChildByTag(kEnemyCvrTag);
    if (node) {
        node->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.6),
                                            CCCallFunc::actionWithTarget(this, callfunc_selector(OrgFightMngr::BossBlinkSndEffct)),
                                            CCFadeTo::actionWithDuration(0.15, 128),
                                            CCFadeOut::actionWithDuration(0.15),
                                            CCCallFunc::actionWithTarget(this, callfunc_selector(OrgFightMngr::BossBlinkSndEffct)),
                                            CCFadeTo::actionWithDuration(0.15, 128),
                                            CCFadeOut::actionWithDuration(0.15),
                                            NULL));
    }
    
    schedule(schedule_selector(OrgFightMngr::decEnemyRound), 0.6);
    schedule(schedule_selector(OrgFightMngr::cbStepOvr), 1.5);
}

void OrgFightMngr::cbStepOvr(ccTime dt)
{
    unschedule(schedule_selector(OrgFightMngr::cbStepOvr));
    
    if(m_iComboIdx < kComboArrLen){
        CGameState::Instance()->setState(enStateWaitTouch);
        m_stoneGrid->startNewRound();
        saveCurFightInfo();
    }
}

void OrgFightMngr::dostep3()
{
    for (int i = 0; i < 5; i++) {
        if(m_OrgCardInfArr[i].xpos == 0)
            break;
        
        CCNode* node = getChildByTag(kCardBkTagBase+i);
        if(node){
            shoot(node->getPosition(), ccp(589, 663), 0);
        }
    }
}

void OrgFightMngr::decEnemyRound(ccTime dt)
{
    unschedule(schedule_selector(OrgFightMngr::decEnemyRound));
    
    char buf[20];
    sprintf(buf, "%d", (3-m_iComboIdx));
    m_lbRound->setString(buf);
    m_lbRound->setScale(1.5);
    m_lbRound->runAction(CCScaleTo::actionWithDuration(0.5, 1.0));
}

//******************************************************************************
// animation
//******************************************************************************
void OrgFightMngr::animBom(const CCPoint& pos, float fScale, float dt)
{
    animComm("bom%02d.png", pos, fScale, dt);
}

void OrgFightMngr::animHol(const CCPoint& pos, float fScale, float dt)
{
    animComm("hol00%02d.png", pos, fScale, dt);
}

void OrgFightMngr::animHalo(const CCPoint& pos, float fScale, float dt)
{
    animComm("halo00%02d.png", pos, fScale, dt);
}

void OrgFightMngr::animComm(const char* szFilename, const CCPoint& pos, float fScale, float dt)
{
    CCAnimation* animation;
	char frameName[100] = {0};
	CCActionInterval*  action;
    CCSprite* spTemp;
    int i;
    
    animation = CCAnimation::animation();
    for(i=1;i<100;i++){
        sprintf(frameName, szFilename, i);
        
        spTemp = CCSprite::spriteWithSpriteFrameName(frameName);
        if(spTemp == NULL)
            break;
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( frameName );
        animation->addFrame(frame);
    }
    
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    animation->addFrame(frame);
    action = CCAnimate::actionWithDuration(1.0 * i / 20, animation, false);
    
    CCSprite* sp = CCSprite::spriteWithSpriteFrameName("blank.png");
    if (sp) {
        addChild(sp, 10);
        sp->setPositionInPixels(pos);
        sp->setScale(fScale);
        sp->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                          action,
                                          NULL));
    }
}

//******************************************************************************
// saveCurFightInfo
//******************************************************************************
void OrgFightMngr::saveCurFightInfo()
{

    OrgSaveData::Inst()->setComboIndex(m_iComboIdx);
    OrgSaveData::Inst()->setMaxCombo(m_iMaxCombo);
    OrgSaveData::Inst()->setIsTongMengLianShouChang(m_bIsTongMengLianShou);

    OrgSaveData::Inst()->flush();
}


//******************************************************************************
// PlayerAtkSndEffct
//******************************************************************************
void OrgFightMngr::PlayerAtkSndEffct()
{
    unschedule(schedule_selector(OrgFightMngr::PlayerAtkSndEffct));
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(OrgPlayerAtk));
}

//******************************************************************************
// BossAtkSndEffct
//******************************************************************************
void OrgFightMngr::BossAtkSndEffct()
{
    unschedule(schedule_selector(OrgFightMngr::BossAtkSndEffct));
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(OrgBossAtk));
}

//******************************************************************************
// BossBlinkSndEffct
//******************************************************************************
void OrgFightMngr::BossBlinkSndEffct()
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(OrgBossBlink));
}





