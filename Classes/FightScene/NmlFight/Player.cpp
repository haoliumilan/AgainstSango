//
//  Player.cpp
//  CardDemo
//
//  Created by Hao Zhang on 12-4-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "Player.h"
#include "CJumpNum.h"
#include "CSkillLib.h"
#include "SimpleAudioEngine.h"
#include "CGamedata.h"
#include "MsgBox.h"
#include "Arrow.h"
#include "TextNode.h"
#include "MainScene.h"
#include "CCShake.h"
#include "SaveData.h"
#include "FightMngr.h"
#include "ShiningFrame.h"
#include "RecordHelper.h"
#include "BulletLight.h"
#include "OcProxy.h"

#define kArenaArrowTag          234

using namespace CocosDenshion;

static const char* gStrMsg[] = {
    "playerTip1",
    "playerTip2",
    "playerTip3",
    "playerTip4",
    "playerTip5",
    "playerTip6",
    "playerTip7"
};

typedef struct{
    const char* filename;
    CCPoint pos;
}newbieCover;

static newbieCover gNewbieCover[] = {
    {"newbiecover-coin.png",        CCPointMake(40,  890)},
    {"newbiecover-token.png",       CCPointMake(442, 890)},
    {"newbiecover-menu.png",        CCPointMake(585, 890)},
    {"newbiecover-enemy_frame.png", CCPointMake(320, 794)},
    {"newbiecover-heroes.png",      CCPointMake(214 + 50, 630)},
    {"newbiecover-friend.png",      CCPointMake(584, 630)},
    {"newbiecover-hp.png",          CCPointMake(478, 590)}
};

#define NewBieChooseEnemyPos  ccp(442.000000, 764.000000)
#define NewBieChooseEnemySz   CCSize(110.000000,156.000000)
//******************************************************************************
// Constructor
//******************************************************************************
Player::Player(CCObject* target,SEL_CallFuncO selector)
{
    m_featureAnim = NULL;
    m_pMsgBox = NULL;
    m_pArrow = NULL;
    m_tempBoolNew = false;
    m_tlabel = NULL;
    m_bTouchEnable = true;

    m_pSkillHighlight = NULL;
    m_bHasHeartPlus = false;
    m_bHasWaterPlus = false;
    m_bHasFirePlus = false;
    m_bHasWoodPlus = false;
    m_bHasLightPlus = false;
    m_bHasDarkPlus = false;
    

    m_iRcvAttack = 0;
    m_iTouchedCard = kInvalidValue;
    m_iCurManSkillID = enManSkill_NULL;
    
    m_bSkillCDYanChi = false;
    m_bCanFei = false;
    m_iSndTag = 0;

    m_iNewbieCombStepIdx = 1;
    
    memset(m_buffArr, 0, sizeof(m_buffArr));
    
    m_bIsManSkill_129 = false;      //
    m_stSickBuffGrp.bIsManSkill_257 = false;      //
    m_bIsOver6Fire = false;         //
    m_i258_259ManSkillID = 0;       //

    
    m_iAwakeSkill_33 = 0;

    m_EnemyAtkArr = new CCMutableArray<CAttackInfo*>();
    
    m_pListener = target;
    m_pfnSelector = selector;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -10, true);
    
    m_iRecoverPt = 0;
    m_iBaseRecoverPt = 0;
    m_iBaseHP = 0;
    
    resetAwakeAtk();
    
    m_stoneGrid = new StoneGrid(this, callfunc_selector(Player::cbStoneGrid));
    addChild(m_stoneGrid, 10);
    m_stoneGrid->release();
    
    initCard();
    
    //blood
    m_blood = new CPlayerBlood(m_iBaseHP, m_iHP);
    m_blood->setPosition(CCPointMake(320, 545));
    addChild(m_blood);
    m_blood->release();
    
    m_bloodNum = CJumpNum::initNumber("greennum.png");
    m_bloodNum->setPosition(m_blood->getPosition());
    m_bloodNum->setNumber(0, 0, 0);
    addChild(m_bloodNum, 20);
    m_bloodNum->setJump(false);
    m_bloodNum->setIsVisible(false);
    
    //card info dialog
    m_cardInfoDlg = CCardInfoDlg::initDlg();
    addChild(m_cardInfoDlg);
    m_cardInfoDlg->setPosition(CCPointMake(320, 815));
    m_cardInfoDlg->setIsVisible(false);
    
    //skill dialog
    m_skillDlg = CSkillDlg::initDlg(this, callfuncO_selector(Player::cbSkillDlg));
    addChild(m_skillDlg, 10);
    m_skillDlg->setPosition(CCPointMake(320, 325));
    m_skillDlg->setIsVisible(false);

    if(CGameData::Inst()->isNewbie()){
        m_iNewbieCovIdx = 3;//shikun_new
        for(int i = 0; i < kMaxNewbieCoverNum; i++)
            m_newbieCover[i] = NULL;
    }
    else {
        m_iNewbieCovIdx = kMaxNewbieCoverNum;
    }
    
    initBuffFromSaveData();
}

void Player::resetCard()
{
    for(int i = 0; i < kMaxCardCntInBattle; i++)
    {
        if (m_card[i])
        {
            m_card[i]->removeFromParentAndCleanup(true);
            m_card[i] = NULL;
        }
    }
    
    if (m_pSkillHighlight)
    {
        m_pSkillHighlight->removeFromParentAndCleanup(true);
        m_pSkillHighlight = NULL;
    }
}

void Player::changeTeamInfo()
{
    int preBaseHp = m_iBaseHP;
    resetCard();
    resetBuff();
    initCard();
    
    if (preBaseHp != m_iBaseHP)
    {
        updateBlood(true);
    }
}

void Player::initCard()
{
    for(int i = 0; i < kMaxCardCntInBattle; i++)
    {
        // 编队中对应位置没有武将
        if (SaveData::Inst()->getCardInfo(i)->cid == 0) {
            m_card[i] = NULL;
            continue;
        }
        
        m_card[i] = new Card(i);
        addChild(m_card[i]);
        m_card[i]->release();
        
        initJumpNum(i, true);
        initJumpNum(i, false);
    }
    
    updateNumByAwakeId();
    
    //延时，数值改变类被动技
    doAutoSkillWhileStarting();
    
    doAwakeSkillWhileStarting();
    
    m_iBaseRecoverPt = 0;
    m_iBaseHP = 0;
    for(int i = 0; i < kMaxCardCntInBattle; i++){
        if(m_card[i] == NULL)
            continue;
        
        m_iBaseRecoverPt += m_card[i]->getRecover();
        m_iBaseHP += m_card[i]->getBaseHp();
    }
    
    if (m_card[0]->getAutoSkId() == enAutoSkill_384) {
        m_iBaseHP *= 0.6;
    }
    if( m_card[5]->getAutoSkId() == enAutoSkill_384)
    {
         m_iBaseHP *= 0.6;
    }
    
    if (m_card[0]->getAutoSkId() == enAutoSkill_438|| m_card[0]->getAutoSkId() == enAutoSkill_439) {
        m_iBaseHP *= 0.5;
    }
    if(m_card[5]->getAutoSkId() == enAutoSkill_438 || m_card[5]->getAutoSkId() == enAutoSkill_439) {
         m_iBaseHP *= 0.5;
    }
   
    
    //总回复力不小于0
    m_iBaseRecoverPt = (m_iBaseRecoverPt < 0)? 0 : m_iBaseRecoverPt;
    
    if (SaveData::Inst()->getIsUseSaveData())
    {
        m_bCanFei = SaveData::Inst()->getPlayerCanfei();
        m_iBaseHP = SaveData::Inst()->getPlayerBaseHp();
        m_iHP = SaveData::Inst()->getPlayerCurHp();
    }
    else {
        m_iHP = m_iBaseHP;
    }
    
    m_iPreBaseHp = m_iBaseHP;
    m_iPrevHp = m_iHP;
    

    updateRoundStartHp();
    
    //初始化每个武将的主动技
    for(int i = 0; i < kMaxCardCntInBattle; i++){
        if(m_card[i] == NULL)
            continue;
        
        m_card[i]->initSkillAtk();
    }
    
    //检查技能是否跳起  显示光圈
    updateSkillHighlight();
}

void Player::resetBuff()
{
    memset(m_buffArr, 0, sizeof(m_buffArr));
    
    m_stArmorBuffGrp.iCD = 0;
    m_stArmorBuffGrp.fEffect = 1.0;
    
    if (m_stArmorBuffGrp.lb)
    {
        m_stArmorBuffGrp.lb->removeFromParentAndCleanup(true);
        m_stArmorBuffGrp.lb = NULL;
    }
    
    if (m_stArmorBuffGrp.sp)
    {
        m_stArmorBuffGrp.sp->removeFromParentAndCleanup(true);
        m_stArmorBuffGrp.sp = NULL;
    }
    
    m_stRcvBuffGrp.iCD = 0;
    m_stRcvBuffGrp.fEffect = 0.0;
    
    if (m_stRcvBuffGrp.lb)
    {
        m_stRcvBuffGrp.lb->removeFromParentAndCleanup(true);
        m_stRcvBuffGrp.lb = NULL;
    }
    
    if (m_stRcvBuffGrp.sp)
    {
        m_stRcvBuffGrp.sp->removeFromParentAndCleanup(true);
        m_stRcvBuffGrp.sp = NULL;
    }
    
    // 回复力提升
    m_stRcvWuJiangBuffGrp.iCD = 0;
    m_stRcvWuJiangBuffGrp.fFactor = 0;
    
    if (m_stRcvWuJiangBuffGrp.lb)
    {
        m_stRcvWuJiangBuffGrp.lb->removeFromParentAndCleanup(true);
        m_stRcvWuJiangBuffGrp.lb = NULL;
    }
    
    if (m_stRcvWuJiangBuffGrp.sp)
    {
        m_stRcvWuJiangBuffGrp.sp->removeFromParentAndCleanup(true);
        m_stRcvWuJiangBuffGrp.sp = NULL;
    }
    
    // 无敌
    m_stWudiBuffGrp.iCD = 0;
    m_stWudiBuffGrp.iElem = 0;
    m_stWudiBuffGrp.enType = enWudiType_Normal;        // 无敌类型

    if (m_stWudiBuffGrp.lb)
    {
        m_stWudiBuffGrp.lb->removeFromParentAndCleanup(true);
        m_stWudiBuffGrp.lb = NULL;
    }
    
    if (m_stWudiBuffGrp.sp)
    {
        m_stWudiBuffGrp.sp->removeFromParentAndCleanup(true);
        m_stWudiBuffGrp.sp = NULL;
    }
    
    // 用于对应属性武将攻击力增加
    m_stEnhanceBuffGrp.iCD = 0;
    m_stEnhanceBuffGrp.iElem = 0;
    m_stEnhanceBuffGrp.fFactor = 1.0;
    
    if (m_stEnhanceBuffGrp.lb)
    {
        m_stEnhanceBuffGrp.lb->removeFromParentAndCleanup(true);
        m_stEnhanceBuffGrp.lb = NULL;
    }
    
    if (m_stEnhanceBuffGrp.sp)
    {
        m_stEnhanceBuffGrp.sp->removeFromParentAndCleanup(true);
        m_stEnhanceBuffGrp.sp = NULL;
    }
    
    // 横向消 6 个珠子 ，属性加强
    m_stEnhanceH6BuffGrp.iCD = 0;
    m_stEnhanceH6BuffGrp.iElem = enElementInvalid;
    m_stEnhanceH6BuffGrp.fFactor = 0.0;
    
    if (m_stEnhanceH6BuffGrp.lb)
    {
        m_stEnhanceH6BuffGrp.lb->removeFromParentAndCleanup(true);
        m_stEnhanceH6BuffGrp.lb = NULL;
    }

    if (m_stEnhanceH6BuffGrp.sp)
    {
        m_stEnhanceH6BuffGrp.sp->removeFromParentAndCleanup(true);
        m_stEnhanceH6BuffGrp.sp = NULL;
    }
    
    //用于武将类型攻击增加
    m_stEnWujiangBuffGrp.iCD = 0;
    m_stEnWujiangBuffGrp.iType = enGeneralTp_Null;
    m_stEnWujiangBuffGrp.fFactor = 1.0;
    
    if (m_stEnWujiangBuffGrp.lb)
    {
        m_stEnWujiangBuffGrp.lb->removeFromParentAndCleanup(true);
        m_stEnWujiangBuffGrp.lb = NULL;
    }
    
    if (m_stEnWujiangBuffGrp.sp)
    {
        m_stEnWujiangBuffGrp.sp->removeFromParentAndCleanup(true);
        m_stEnWujiangBuffGrp.sp = NULL;
    }
    
    //用于国家武将类型的增加
    m_stCountryWujiangBuffGrp.iCD = 0;
    m_stCountryWujiangBuffGrp.iType = enCountryTp_Invalid;
    m_stCountryWujiangBuffGrp.fFactor = 1.0;
    
    if (m_stCountryWujiangBuffGrp.lb)
    {
        m_stCountryWujiangBuffGrp.lb->removeFromParentAndCleanup(true);
        m_stCountryWujiangBuffGrp.lb = NULL;
    }
    
    if (m_stCountryWujiangBuffGrp.sp)
    {
        m_stCountryWujiangBuffGrp.sp->removeFromParentAndCleanup(true);
        m_stCountryWujiangBuffGrp.sp = NULL;
    }
    
    // 普通攻击变为aoe
    m_stAOEBuffGrp.iCD = 0;
    
    if (m_stAOEBuffGrp.lb)
    {
        m_stAOEBuffGrp.lb->removeFromParentAndCleanup(true);
        m_stAOEBuffGrp.lb = NULL;
    }
    
    if (m_stAOEBuffGrp.sp)
    {
        m_stAOEBuffGrp.sp->removeFromParentAndCleanup(true);
        m_stAOEBuffGrp.sp = NULL;
    }
    
    // 反击
    m_stFanJiBuffGrp.bMianSiFanJi = false;
    m_stFanJiBuffGrp.iCD = 0;
    m_stFanJiBuffGrp.fFactor = 1.0;
    m_stFanJiBuffGrp.iElem = enElementInvalid;
    m_stFanJiBuffGrp.iJueDiCardIdx = 0;
    if (m_stFanJiBuffGrp.lb)
    {
        m_stFanJiBuffGrp.lb->removeFromParentAndCleanup(true);
        m_stFanJiBuffGrp.lb = NULL;
    }
    
    if (m_stFanJiBuffGrp.sp)
    {
        m_stFanJiBuffGrp.sp->removeFromParentAndCleanup(true);
        m_stFanJiBuffGrp.sp = NULL;
    }
    
    // 虚弱状态
    m_stSickBuffGrp.bIsManSkill_257 = false;
    m_bIsManSkill_129 = false;
    if (m_stSickBuffGrp.sp)
    {
        m_stSickBuffGrp.sp->removeFromParentAndCleanup(true);
        m_stSickBuffGrp.sp = NULL;
    }
}

void Player::initBuffFromSaveData()
{
    if (SaveData::Inst()->getIsUseSaveData()) {
        // 加护甲
        m_stArmorBuffGrp.iCD = SaveData::Inst()->getSheildCd();
        m_stArmorBuffGrp.fEffect = SaveData::Inst()->getArmorEffect();
        
        if(m_stArmorBuffGrp.iCD > 0){
            addArmorBuff();
        }
        
        // 无敌
        m_stWudiBuffGrp.iCD = SaveData::Inst()->getWudiCd();
        m_stWudiBuffGrp.iElem = SaveData::Inst()->getWudiElement();
        m_stWudiBuffGrp.enType = (enWudiTypeDef)SaveData::Inst()->getWudiType();
        
        if(m_stWudiBuffGrp.iCD > 0)
            addWudiBuff();
        
        // 反击
        m_stFanJiBuffGrp.iCD = SaveData::Inst()->getFanjiCd();
        m_stFanJiBuffGrp.fFactor = SaveData::Inst()->getFanjiEffect();
        m_stFanJiBuffGrp.iElem = SaveData::Inst()->getFanjiElement();
        m_stFanJiBuffGrp.bMianSiFanJi = SaveData::Inst()->getIsFanjiMiansi();
        
        if (m_stFanJiBuffGrp.iCD > 0)
            addJdfjBuff();
        
        // aoe
        m_stAOEBuffGrp.iCD = SaveData::Inst()->getAOECd();
        
        if (m_stAOEBuffGrp.iCD > 0)
            addAOEBuff();
        
        // 如沐春风
        m_stRcvBuffGrp.iCD = SaveData::Inst()->getRecoverCd();
        m_stRcvBuffGrp.fEffect = SaveData::Inst()->getRecoverEffect();
        
        if(m_stRcvBuffGrp.iCD > 0)
            addRcvBuff();
        
        m_stRcvWuJiangBuffGrp.iCD = SaveData::Inst()->getRecoverWuHiangCd();
        m_stRcvWuJiangBuffGrp.fFactor = SaveData::Inst()->getRecoverWuJiangEffect();
        if (m_stRcvWuJiangBuffGrp.iCD > 0) {
            m_iBaseRecoverPt *= m_stRcvWuJiangBuffGrp.fFactor;
            addRcvWuJiangBuff();
        }
                
        m_stCountryWujiangBuffGrp.iCD = SaveData::Inst()->getPlayerCountryWujiangCD();
        m_stCountryWujiangBuffGrp.iType = SaveData::Inst()->getPlayerCountryWujiangType();
        m_stCountryWujiangBuffGrp.fFactor = SaveData::Inst()->getPlayerCountryWujiangFactor();
        if (m_stCountryWujiangBuffGrp.iCD > 0) {
            addCountryWujiangBuff();
        }
        
        m_stEnhanceBuffGrp.iCD = SaveData::Inst()->getPlayerEnhanceCD();
        m_stEnhanceBuffGrp.iElem = SaveData::Inst()->getPlayerEnhanceElem();
        m_stEnhanceBuffGrp.fFactor = SaveData::Inst()->getPlayerEnhanceFactor();
        if (m_stEnhanceBuffGrp.iCD > 0)
            addEnhanceBuff();
        
        m_stEnWujiangBuffGrp.iCD = SaveData::Inst()->getPlayerEnWujiangCD();
        m_stEnWujiangBuffGrp.iType = SaveData::Inst()->getPlayerEnWujiangType();
        m_stEnWujiangBuffGrp.fFactor= SaveData::Inst()->getPlayerEnWujiangFactor();
        if (m_stEnWujiangBuffGrp.iCD > 0)
            addEnWujiangBuff();
        
        m_bIsManSkill_129 = SaveData::Inst()->isManSkill_129();
    }
}

void Player::initJumpNum(int index, bool bMainElem)
{
    enElementDef element;
    if (bMainElem)
    {
        element = (enElementDef)m_card[index]->getElement();
    }
    else
    {
        element = (enElementDef)m_card[index]->getSubElement();
    }
    
    if (element == enElementInvalid)
    {
        return;
    }
    
    CJumpNum* jmpNum = NULL;
    switch (element) {
        case enElementHeart:
            jmpNum = CJumpNum::initNumber("pinknum.png", bMainElem);
            break;
            
        case enElementFire:
            jmpNum = CJumpNum::initNumber("rednum.png", bMainElem);
            break;
            
        case enElementWood:
            jmpNum = CJumpNum::initNumber("greennum.png", bMainElem);
            break;
            
        case enElementWater:
            jmpNum = CJumpNum::initNumber("bluenum.png", bMainElem);
            break;
            
        case enElementLight:
            jmpNum = CJumpNum::initNumber("yellownum.png", bMainElem);
            break;
            
        case enElementDark:
        default:
            jmpNum = CJumpNum::initNumber("purplenum.png", bMainElem);
            break;
    }
    
    addChild(jmpNum, 10);
    m_card[index]->setJumpNumber(jmpNum, bMainElem);
}

void Player::resetAwakeAtk()
{
    for (int i=0; i<6; i++)
    {
        m_fAwakeAtk[i] = 0.0f;
    }
}

void Player::updateAwakeAtk(float f, int idx)
{
    m_fAwakeAtk[idx] += f;
}

float Player::getAwakeValueById(int idx)
{
    if (idx < 0 || idx > 5)
    {
        return 0.0f;
    }
    
    return m_fAwakeAtk[idx];
}

int Player::getNumByAwakeId(int idx)
{
    return m_iAwakeCout[idx];
}

void Player::updateNumByAwakeId()
{
    m_iAwakeCout[0] = 0;
    for (int i=1; i<enAwakeSkill_Max; i++)
    {
        m_iAwakeCout[i] = 0;
        for(int j = 0; j < kMaxCardCntInBattle; j++)
        {
            if(m_card[j])
            {
                m_iAwakeCout[i] += m_card[j]->getCanUseNumByAwakeId(i);
            }
        }
    }
}

//******************************************************************************
// setCardsNoCd
//******************************************************************************
void Player::setCardsNoCd()
{
    for(int i = 0; i < kMaxCardCntInBattle; i++){
        if(m_card[i] == NULL)
            continue;
        
        m_card[i]->setNoCD();
    }
    
    updateSkillHighlight();
}

int Player::getCurCombo()
{
    return m_ulCombo;
}

void Player::startFromMove()
{
    StartUpdate();
    
    //delete unused sprite
    CCNode* node = this->getChildByTag(kTagToBeDel);
    
    while (node != NULL) {
        removeChild(node, true);
        node = this->getChildByTag(kTagToBeDel);
    }
    
    m_iCurManSkillID = enManSkill_NULL;
    m_bErased = false;
    m_AtkInfo.clear();
    m_EnemyAtkArr->removeAllObjects();
    memset(m_StoneEraseCnt, 0, sizeof(m_StoneEraseCnt));
    memset(m_bElementAttck, 0, sizeof(m_bElementAttck));
    
    m_bIsMainAtk = true;
    
    for (int i = 0;  i < kMaxCardCntInBattle; i++) {
        if (m_card[i] == NULL) {
            continue;
        }
        m_card[i]->setisTouchOffAwakeSkill_28(true);
    }
    
    m_iCurAttackCard = 0;
    m_iRecoverPt = 0;
    m_iComboBaseRcvPt = 0;
    updateRoundStartHp();
    m_ulCombo = 0;
    
    m_bloodNum->setIsVisible(false);
    m_card[0]->resetSkillElementList();
    m_card[5]->resetSkillElementList();
    
    resetAwakeAtk();
    
    reorderChild(m_blood, 0);
    
    
    if(CGameState::Instance()->getState() != enStateTouching)
        CGameState::Instance()->setState(enStateWaitTouch);
        
    m_stoneGrid->startNewRound();
}

//******************************************************************************
// startNewRound
//******************************************************************************
void Player::startNewRound(bool first)
{
    StartUpdate();
    
    //delete unused sprite
    CCNode* node = this->getChildByTag(kTagToBeDel);
    
    while (node != NULL) {
        removeChild(node, true);
        node = this->getChildByTag(kTagToBeDel);
    }

    m_iCurManSkillID = enManSkill_NULL;
    m_bErased = false;
    m_AtkInfo.clear();
    m_EnemyAtkArr->removeAllObjects();
    memset(m_StoneEraseCnt, 0, sizeof(m_StoneEraseCnt));
    memset(m_bElementAttck, 0, sizeof(m_bElementAttck));

    m_bIsMainAtk = true;
    m_iCurAttackCard = 0;
    m_iRecoverPt = 0;
    m_iComboBaseRcvPt = 0;
    m_ulCombo = 0;
    
    for (int i = 0;  i < kMaxCardCntInBattle; i++) {
        if (m_card[i] == NULL) {
            continue;
        }
        m_card[i]->setisTouchOffAwakeSkill_28(true);
    }
    
    //
    m_bHasHeartPlus = false;
    m_bHasWaterPlus = false;
    m_bHasFirePlus = false;
    m_bHasWoodPlus = false;
    m_bHasLightPlus = false;
    m_bHasDarkPlus = false;
    
    m_bloodNum->setIsVisible(false);
    m_card[0]->resetSkillElementList();
    m_card[5]->resetSkillElementList();
    
    resetAwakeAtk();
    
    updateRoundStartHp();
    //
    getStoneGrid()->setCurEraseCnt(0);
    
    //减buff cd
    decBuffCD();

    reorderChild(m_blood, 0);
    
    if(m_iNewbieCovIdx == 3){//shikun_new
        m_newbieCover[m_iNewbieCovIdx] = CCSprite::spriteWithFile(gNewbieCover[m_iNewbieCovIdx].filename);
        addChild(m_newbieCover[m_iNewbieCovIdx]);
        m_newbieCover[m_iNewbieCovIdx]->setPosition(gNewbieCover[m_iNewbieCovIdx].pos);
        m_newbieCover[m_iNewbieCovIdx]->runAction(CCBlink::actionWithDuration(100, 500));
   
        MainScene::Inst()->showTouchTip();
    }else if(m_iNewbieStep == 4)
    {
        m_pMsgBox = new MsgBox("commdlg.png", CGameData::Inst()->getLanguageValue("comboTip1"), ccc3(255, 255, 255));
        addChild(m_pMsgBox, 10);
        m_pMsgBox->release();
        m_pMsgBox->setPosition(CCPointMake(320, 268));
        m_pMsgBox->setScale(0.0);
        m_pMsgBox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        
        m_pArrow = new Arrow(80, 160);
        m_pArrow->setPosition(ccp(450, 750));
        addChild(m_pArrow, 501);
        m_pArrow->release();
        
        CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                           CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                           NULL);
        m_pArrow->runAction(CCRepeatForever::actionWithAction(action));
        
     //   MainScene::Inst()->showTouchTip();
    }
    else if(((m_iNewbieStep == 5) && (m_iNewbieMsgIdx == 3 ))){
        m_pMsgBox = new MsgBox("commdlg.png", getPlayerTip(), ccc3(255, 255, 255));
        addChild(m_pMsgBox, 10);
        m_pMsgBox->release();
        m_pMsgBox->setPosition(CCPointMake(320, 268));
        m_pMsgBox->setScale(0.0);
        m_pMsgBox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
    
        MainScene::Inst()->showTouchTip();
    }
    else {
        
        
        if(CGameState::Instance()->getState() != enStateTouching)
        {
            if (first)
            {   // 判断限制攻击，怪攻击
                CGameState::Instance()->setState(enStateEnemyAttack);
            }
            else
            {
                CGameState::Instance()->setState(enStateWaitTouch);
            }
        }
       
        m_stoneGrid->startNewRound();
    }
}

//******************************************************************************
// changeZhuJiang  按传入的index1 和 index2 交换两武将的位置（一般都是把主将换掉）
//******************************************************************************
void Player::changeZhuJiang(int idx1, int idx2)
{
    CGameState::Instance()->setState(enStateTouchEnd);
    Card* card1;
    Card* card2;
    int index1 = idx1;
    int index2 = idx2;

    card1 = m_card[index1];
    card2 = m_card[index2];
    
    if (card1->getState()!=EN_CARD_STATE_FNEGYIN) {
        fengYinAutoSkill(0);
    }
    
    ccBezierConfig bezier1;
    bezier1.controlPoint_1 = ccp(55, 100);
    bezier1.controlPoint_2 = ccp(100*index2, 100);
    bezier1.endPosition = ccp(106*index2,0);
    card1->runAction(CCSequence::actions(CCBezierBy::actionWithDuration(1, bezier1),
                                         CCMoveBy::actionWithDuration(0, ccp(-106*index2, 0)),
                                         CCCallFuncND::actionWithTarget(this,callfuncND_selector(Player::cbEndChange),(void*)(index2)),
                                         NULL));
    
    card1->setCardIndex(index2);
    m_card[index2] = card1;
    
    ccBezierConfig bezier2;
    bezier2.controlPoint_1 = ccp(-55, 100);
    bezier2.controlPoint_2 = ccp(-100*index2, 100);
    bezier2.endPosition = ccp(-106*index2,0);
    card2->runAction(CCSequence::actions(CCBezierBy::actionWithDuration(1, bezier2),
                                         CCMoveBy::actionWithDuration(0, ccp(106*index2, 0)),
                                         CCCallFuncND::actionWithTarget(this,callfuncND_selector(Player::cbEndChange),(void*)(index1)),
                                         NULL));
    card2->setCardIndex(index1);
    m_card[index1] = card2;
    
    if (card2->getState()!=EN_CARD_STATE_FNEGYIN) {
        unFengYinAutoSkill(0);
    }
    
    SaveData_Card cardInfo1 = SaveData::Inst()->getCardInfo2(index1);
    SaveData_Card cardInfo2 = SaveData::Inst()->getCardInfo2(index2);
    
    SaveData::Inst()->setCardInfo(index1,cardInfo2);
    SaveData::Inst()->setCardInfo(index2,cardInfo1);
}


//******************************************************************************
// getPosition
//******************************************************************************
const CCPoint& Player::getPosition()
{
    return m_blood->getPosition();
}

Card* Player::getCardByIdx(int idx)
{
    return m_card[idx];
}

Card * Player::getCardCID(unsigned short unCID)const
{
    for (int i = 0; i < kMaxCardCntInBattle; ++i)
    {
        if (m_card[i] &&
            m_card[i]->getCardID()==unCID)
        {
            return m_card[i];
        }
    }
    return NULL;
}

//******************************************************************************
// chkComboEffect
//******************************************************************************
void Player::chkComboEffect()
{
    int iSkillId1 = chkAutoSkillAvail(m_card[0]->getAutoSkId());
    int iSkillId2 = chkAutoSkillAvail(m_card[5]->getAutoSkId());
    stAutoSkill* pAutoSkill1 = CSkillLib::Ins()->getAutoSkill(iSkillId1);
    stAutoSkill* pAutoSkill2 = CSkillLib::Ins()->getAutoSkill(iSkillId2);
    
    float fayEffect[enElementMax+1] = {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};
    
    //if (pAutoSkill1->cElement>enElementMax || pAutoSkill1->cElement>enElementMax)return;
    
    int iType1 = (pAutoSkill1)? pAutoSkill1->iType : enAutoSkillTp_null;
    int iType2 = (pAutoSkill2)? pAutoSkill2->iType : enAutoSkillTp_null;
    
    
    if (enElementMax<pAutoSkill1->cElement)
    {
    }
    else if (iType1 == enAutoSkillTp_atk_cmb
        //&& pAutoSkill1->cElement == enElementMax
        )
    {   // 根据combo数对攻击加成
        if (pAutoSkill1->cOdds <= m_stoneGrid->getComboNum())
        {
            if (iSkillId1 >= enAutoSkill_Cmb6HuoAtk35 && iSkillId1 <= enAutoSkill_Cmb6AnAtk35)
            {
                fayEffect[pAutoSkill1->cElement] *= 3.5;
            }
            else if (iSkillId1 == enAutoSkill_387) {
                
            }
            else
            {
                fayEffect[pAutoSkill1->cElement] *= pAutoSkill1->fEffect;
            }
        }
        else if (enAutoSkill_391==iSkillId1 && m_stoneGrid->getComboNum() >= 3) {
            if (m_stoneGrid->getComboNum() > 20) {
                fayEffect[pAutoSkill1->cElement] *= 0.5 * 20;
            } else {
                fayEffect[pAutoSkill1->cElement] *= 0.5 * m_stoneGrid->getComboNum();
            }
        }
        else if (enAutoSkill_430==iSkillId1 && m_stoneGrid->getComboNum() >= 4) {
            if (m_stoneGrid->getComboNum() > 7) {
                fayEffect[pAutoSkill1->cElement] *= 2.5+(7-4)*0.5;
            } else {
                fayEffect[pAutoSkill1->cElement] *= 2.5+(m_stoneGrid->getComboNum()-4)*0.5;
            }
        }
    }
    else if (iType1 == enAutoSkillTp_atk_elm_cnt)
    {   // 根据攻击属性个数对攻击加成
        if (pAutoSkill1->cOdds <= getAttackElemCnt())
        {
            if (iSkillId1==enAutoSkill_Elm5Atk4) {
                if (pAutoSkill1->cOdds+1<=getAttackElemCnt())
                    fayEffect[pAutoSkill1->cElement] *= pAutoSkill1->fEffect+1;
                else
                    fayEffect[pAutoSkill1->cElement] *= pAutoSkill1->fEffect;
            }else
                fayEffect[pAutoSkill1->cElement] *= pAutoSkill1->fEffect;
        }
    }
    else if (iType1 == enAutoSkillTp_atk_elm_type)
    {   // 根据攻击属性类型对攻击加成
        if (m_bElementAttck[element3Atk25[pAutoSkill1->cOdds][0]]
            && m_bElementAttck[element3Atk25[pAutoSkill1->cOdds][1]]
            && m_bElementAttck[element3Atk25[pAutoSkill1->cOdds][2]])
        {
            fayEffect[pAutoSkill1->cElement] *= pAutoSkill1->fEffect;
        }
    }
    else if (enAutoSkillTp_atk_elm_type4==iType1)
    {
        if (m_bElementAttck[elementAtk4[pAutoSkill1->cOdds][0]] &&
            m_bElementAttck[elementAtk4[pAutoSkill1->cOdds][1]] &&
            m_bElementAttck[elementAtk4[pAutoSkill1->cOdds][2]] &&
            m_bElementAttck[elementAtk4[pAutoSkill1->cOdds][3]])
        {
            fayEffect[pAutoSkill1->cElement] *= pAutoSkill1->fEffect;
        }
    }

    if (enElementMax<pAutoSkill2->cElement)
    {
    }
    else if (iType2 == enAutoSkillTp_atk_cmb
        //&& pAutoSkill2->cElement == enElementMax
        )
    {   // 根据combo数对攻击加成
        if (pAutoSkill2->cOdds <= m_stoneGrid->getComboNum())
        {
            if (iSkillId2 >= enAutoSkill_Cmb6HuoAtk35 && iSkillId2 <= enAutoSkill_Cmb6AnAtk35)
            {
                fayEffect[pAutoSkill2->cElement] *= 3.5;
            }
            else if (iSkillId2 == enAutoSkill_387) {
                
            }
            else
            {
                fayEffect[pAutoSkill2->cElement] *= pAutoSkill2->fEffect;
            }
        }
        else if (enAutoSkill_391==iSkillId2 && m_stoneGrid->getComboNum() >= 3) {
            if (m_stoneGrid->getComboNum() > 20) {
                fayEffect[pAutoSkill2->cElement] *= 0.5 * 20;
            } else {
                fayEffect[pAutoSkill2->cElement] *= 0.5 * m_stoneGrid->getComboNum();
            }
        }
        else if (enAutoSkill_430==iSkillId2 && m_stoneGrid->getComboNum() >= 4) {
            if (m_stoneGrid->getComboNum() > 7) {
                fayEffect[pAutoSkill1->cElement] *= 2.5+(7-4)*0.5;
            } else {
                fayEffect[pAutoSkill1->cElement] *= 2.5+(m_stoneGrid->getComboNum()-4)*0.5;
            }
        }
    }
    else if (iType2 == enAutoSkillTp_atk_elm_cnt)
    {   // 根据攻击属性个数对攻击加成
        if (pAutoSkill2->cOdds <= getAttackElemCnt())
        {
            if (iSkillId2==enAutoSkill_Elm5Atk4) {
                if (pAutoSkill2->cOdds+1<=getAttackElemCnt())
                    fayEffect[pAutoSkill2->cElement] *= pAutoSkill2->fEffect+1;
                else
                    fayEffect[pAutoSkill2->cElement] *= pAutoSkill2->fEffect;
            }else
                fayEffect[pAutoSkill2->cElement] *= pAutoSkill2->fEffect;
        }
    }
    else if (iType2 == enAutoSkillTp_atk_elm_type)
    {   // 根据攻击属性类型对攻击加成
        if (m_bElementAttck[element3Atk25[pAutoSkill2->cOdds][0]]
            && m_bElementAttck[element3Atk25[pAutoSkill2->cOdds][1]]
            && m_bElementAttck[element3Atk25[pAutoSkill2->cOdds][2]])
        {
            fayEffect[pAutoSkill2->cElement] *= pAutoSkill2->fEffect;
        }
    }
    else if (enAutoSkillTp_atk_elm_type4==iType2)
    {
        if (m_bElementAttck[elementAtk4[pAutoSkill2->cOdds][0]] &&
            m_bElementAttck[elementAtk4[pAutoSkill2->cOdds][1]] &&
            m_bElementAttck[elementAtk4[pAutoSkill2->cOdds][2]] &&
            m_bElementAttck[elementAtk4[pAutoSkill2->cOdds][3]])
        {
            fayEffect[pAutoSkill2->cElement] *= pAutoSkill2->fEffect;
        }
    }
    
    CCPoint point = m_stoneGrid->getCurErasePos();
    for (int i = 0; i <= enElementMax; ++i)
    {
        m_stoneGrid->showSkillEffect(point,fayEffect[i],(enElementDef)i);
    }
}

//******************************************************************************
// cbStoneGrid
//******************************************************************************
void Player::cbStoneGrid()
{
    if((CGameState::Instance()->getState() == enStatePlayerAttackPrepared)
       ||(CGameState::Instance()->getState() == enStateEnemyAttack)){
        //觉醒技34
        int cout = getNumByAwakeId(enAwakeSkill_34);
        stAwakeSkill* pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(enAwakeSkill_34);
        FightMngr* fm = (FightMngr*) m_pListener;
        if (SaveData::Inst()->getrand()%100+1 <= pAwakeSkill->cOdds*cout && m_StoneEraseCnt[pAwakeSkill->cElement] > 0 && !(fm->isEnemyDelayed())) {
            for (int i = 0; i < fm->getEnemyGrp()->getEnemyCount(); i++) {
                Enemy *tempEnemy = fm->getEnemyGrp()->getEnemyByIdx(i);
                if (tempEnemy) {
                    tempEnemy->setCurRound(tempEnemy->getCurRound() + (int)(pAwakeSkill->fEffect));
                    char buf[20];
                    sprintf(buf, "%d", tempEnemy->getCurRound());
                    tempEnemy->getLbRound()->setString(buf);
                    tempEnemy->getLbRound()->setScale(1.5);
                    tempEnemy->getLbRound()->runAction(CCScaleTo::actionWithDuration(0.5, 1.0));
                }
            }
        }
        
        m_iHP += m_iRecoverPt;
        m_iRcvAttack = 0;
        if(m_iHP > m_iBaseHP.get())
        {
            FightMngr* temp = (FightMngr*)m_pListener;
            if (temp->getEnemyGrp()->isBuffEffect(enEnemyBuff_aytaipingyaoshu))
                m_iRcvAttack = m_iHP - m_iBaseHP.get();
            m_iHP = m_iBaseHP;
        }
        
        updateBlood();
        
        SaveData::Inst()->setCurCombo(m_ulCombo);
        
        if(m_stoneGrid->isUserDid())
            m_iCurManSkillID = enManSkill_NULL;
        
        schedule(schedule_selector(Player::attack), 0.6);
        
        if(doAutoSkillBeforeAttacking() == false)
            attack(0);
        
        resetAwakeAtk();
    }
    else if(CGameState::Instance()->getState() == enStateClrCombo){
        m_ulCombo++;
        
        m_iRecoverPt += m_iComboBaseRcvPt * 0.25;
        m_bloodNum->setNumber(m_iRecoverPt, 0.6, 0.0);

        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] == NULL)
                continue;
            
            m_card[i]->comboNotify();
        }
        if(CGameData::Inst()->isSoundOn){
            if(m_iSndTag == enAutoSkill_JiangNan)
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(jumpnumjn));
            else
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(jumpnum));
        }
    }
    else{
        m_ulCombo = 1;
        m_bErased = true;
        
        int iElem = m_stoneGrid->getCurEraseElement();
        
        int iCnt = m_stoneGrid->getCurEraseCnt();
        
        if (iElem == enElementFire && iCnt >= 6)
            m_bIsOver6Fire = true;
            
        if((iElem >= 0) && (iElem < enElementMax))
            m_StoneEraseCnt[iElem] += iCnt;
        
        if(iElem == enElementHeart){
            m_iRecoverPt += (getRealBaseRecoverPt() * ((iCnt - 3) * 0.25 + 1));
            shoot(m_stoneGrid->getCurErasePos(), m_blood->getPosition(), iElem);
            
            m_bloodNum->setIsVisible(true);
            m_bloodNum->setNumber(m_iRecoverPt, 0.3, 0.2);
            
            m_iComboBaseRcvPt = m_iRecoverPt;
            
            if(m_iNewbieStep == 5){
                m_pMsgBox = new MsgBox("commdlg.png", getPlayerTip(), ccc3(255, 255, 255));
                addChild(m_pMsgBox, 10);
                m_pMsgBox->release();
                m_pMsgBox->setPosition(CCPointMake(320, 268));
                m_pMsgBox->setScale(0.0);
                m_pMsgBox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                
                MainScene::Inst()->showTouchTip();
            }
        }
        // 纪录有＋效果的属性
        if (m_stoneGrid->getCurErasePlus() > 1.0) {
            switch (m_stoneGrid->getCurEraseElement()) {
                case enElementHeart:
                    m_bHasHeartPlus = true;
                    break;
                case enElementWater:
                    m_bHasWaterPlus = true;
                    break;
                case enElementFire:
                    m_bHasFirePlus = true;
                    break;
                case enElementWood:
                    m_bHasWoodPlus = true;
                    break;
                case enElementLight:
                    m_bHasLightPlus = true;
                    break;
                case enElementDark:
                    m_bHasDarkPlus = true;
                    break;
                default:
                    break;
            }
        }
        
        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] == NULL)
                continue;
            
            if (m_card[i]->getState() == EN_CARD_STATE_FNEGYIN)
            {
                continue;
            }

            if(iElem == m_card[i]->getElement())
            {
                m_bElementAttck[iElem] = true;
                m_card[0]->showSkillElement((enElementDef)iElem);
                m_card[5]->showSkillElement((enElementDef)iElem);
                shoot(m_stoneGrid->getCurErasePos(), m_card[i]->getCardPos(), iElem);
                bool bAOE = false;
                if (m_stAOEBuffGrp.iCD > 0)
                    bAOE = true;
                m_card[i]->stoneEraseNotify(iCnt, m_stoneGrid->getCurErasePlus(), bAOE,true);
            }
            
            if(iElem != enElementInvalid
               && iElem == m_card[i]->getSubElement())
            {
                m_bElementAttck[iElem] = true;
                m_card[0]->showSkillElement((enElementDef)iElem);
                m_card[5]->showSkillElement((enElementDef)iElem);
                shoot(m_stoneGrid->getCurErasePos(), m_card[i]->getCardPos(),iElem);
                bool bAOE = false;
                if (m_stAOEBuffGrp.iCD > 0)
                    bAOE = true;
                m_card[i]->stoneEraseNotify(iCnt, m_stoneGrid->getCurErasePlus(), bAOE,false);
            }
        }
        
        chkComboEffect();
    }
}

//******************************************************************************
// getElementHasPlus
//******************************************************************************
bool Player::getElementHasPlus(int element)
{
    switch (element) {
        case enElementHeart:
            return m_bHasHeartPlus;
            break;
        case enElementWater:
            return m_bHasWaterPlus;
            break;
        case enElementFire:
            return m_bHasFirePlus;
            break;
        case enElementWood:
            return m_bHasWoodPlus;
            break;
        case enElementLight:
            return m_bHasLightPlus;
            break;
        case enElementDark:
            return m_bHasDarkPlus;
            break;
        default:
            return false;
            break;
    }
}

//******************************************************************************
// cbSkillDlg
//******************************************************************************
void Player::cbSkillDlg(CCObject* obj)
{
    CSkillDlgRet* pSkillDlgRet = (CSkillDlgRet*)obj;
        
    m_skillDlg->show(false);
    
    if(pSkillDlgRet->bUse){
        RecordControl(false, 3);
        
        m_stoneGrid->enableTouch(false);
        
        m_bTouchEnable = false;
        
        if (CGameData::Inst()->isSkillOn && SaveData::Inst()->getCardInfo(m_iTouchedCard)->star >= 5)
        {
            CCTexture2DPixelFormat oldFormat = CCTexture2D::defaultAlphaPixelFormat();
            CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);
            CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("featureanim.plist");
            CCTexture2D::setDefaultAlphaPixelFormat(oldFormat);

            if (m_featureAnim == NULL)
                m_featureAnim = new FeatureAnim(CGameData::Inst()->getBodyFileName(SaveData::Inst()->getCardInfo(m_iTouchedCard)->cid),true);
            addChild(m_featureAnim,100);
            m_featureAnim->setPosition(CCPointMake(0, 340));
            
            schedule(schedule_selector(Player::useSkill), 2.0);
        }
        else
        {
            m_AtkInfo = m_card[m_iTouchedCard]->useManSkill();
            m_iCurManSkillID = m_AtkInfo.iManSkillID;
            schedule(schedule_selector(Player::cbSkillDlg2), 0.7);

            stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(m_iCurManSkillID);
            if(pManSkill->iType == enManSkillTp_fanji)
            {
                m_stFanJiBuffGrp.iJueDiCardIdx = m_iTouchedCard;
            }
            
            updateSkillHighlight();
        }
    }
}



void Player::useSkill(ccTime dt)
{
    unschedule(schedule_selector(Player::useSkill));
    
    if (m_featureAnim != NULL)
    {
        removeChild(m_featureAnim, true);
        m_featureAnim = NULL;
        CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("featureanim.plist");
    }
    
    m_AtkInfo = m_card[m_iTouchedCard]->useManSkill();
    m_iCurManSkillID = m_AtkInfo.iManSkillID;
    schedule(schedule_selector(Player::cbSkillDlg2), 0.7);
    
    
    
    stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(m_iCurManSkillID);
    if(pManSkill->iType == enManSkillTp_fanji){
        m_stFanJiBuffGrp.iJueDiCardIdx = m_iTouchedCard;
    }
    
    updateSkillHighlight();
}

//******************************************************************************
// cbSkillDlg2
//******************************************************************************
void Player::cbSkillDlg2() {
    unschedule(schedule_selector(Player::cbSkillDlg2));
    
    SaveData::Inst()->IncUseSkillCnt();
    
    stManualSkill* pManSkill;
    // 随机使用一种强力武将技
    // 纪录下原来是258／259
    int iRandManSkillId;
    if (m_AtkInfo.iManSkillID == enManSkill_258) {
        iRandManSkillId = enManSkill_258;
        pManSkill = CSkillLib::Ins()->getManSkill(m_i258_259ManSkillID);
        m_card[m_iTouchedCard]->setManSkillId(m_i258_259ManSkillID);
        m_card[m_iTouchedCard]->initSkillAtk();
        m_AtkInfo = m_card[m_iTouchedCard]->useManSkill();
        m_iCurManSkillID = m_AtkInfo.iManSkillID;
    } else if (m_AtkInfo.iManSkillID == enManSkill_259) {
        iRandManSkillId = enManSkill_259;
        pManSkill = CSkillLib::Ins()->getManSkill(m_i258_259ManSkillID);
        m_card[m_iTouchedCard]->setManSkillId(m_i258_259ManSkillID);
        m_card[m_iTouchedCard]->initSkillAtk();
        m_AtkInfo = m_card[m_iTouchedCard]->useManSkill();
        m_iCurManSkillID = m_AtkInfo.iManSkillID;
    } else {
        pManSkill = CSkillLib::Ins()->getManSkill(m_AtkInfo.iManSkillID);
    }
    
    if(m_pArrow){
        removeChild(m_pArrow, true);
        m_pArrow = NULL;
    }
    
    m_AtkInfo.iEnemyIdx = kInvalidValue;
    
    if(pManSkill){
        switch (pManSkill->iType) {
            case enManSkillTp_null:
                if(m_AtkInfo.iManSkillID == enManSkill_GuaGuLiaoShang){
                    //回满血
                    m_iHP = m_iBaseHP;
                    // 满血状态下攻击翻倍技能特殊处理，针对消除之前使用回血技能
                    updateRoundStartHp();
                    updateBlood();
                    
                    //5%重力
                    (m_pListener->*m_pfnSelector)(&m_AtkInfo);
                }

                break;

            case enManSkillTp_fanji:
                if (m_AtkInfo.iManSkillID == enManSkill_JueDiFj)
                    m_stFanJiBuffGrp.bMianSiFanJi = true;
                else
                    m_stFanJiBuffGrp.bMianSiFanJi = false;
                m_stFanJiBuffGrp.iCD = pManSkill->cContinue;
                m_stFanJiBuffGrp.fFactor = pManSkill->fEffect;
                m_stFanJiBuffGrp.iElem = pManSkill->cElement;
                
                SaveData::Inst()->setIsFanjiMiansi(m_stFanJiBuffGrp.bMianSiFanJi);
                SaveData::Inst()->setFanjiCd(m_stFanJiBuffGrp.iCD);
                SaveData::Inst()->setFanjiEffect(m_stFanJiBuffGrp.fFactor);
                SaveData::Inst()->setFanjiElement(m_stFanJiBuffGrp.iElem);
                
                addJdfjBuff();
                
                break;
            
            case enManSkillTp_aoe:
                m_stAOEBuffGrp.iCD = pManSkill->cContinue;
                
                SaveData::Inst()->setAOECd(m_stAOEBuffGrp.iCD);
                
                addAOEBuff();
                
                break;
                
            case enManSkillTp_atk:
            case enManSkillTp_gravity:
            {
                if(m_AtkInfo.iManSkillID == enManSkill_HateAtk
                   || m_AtkInfo.iManSkillID == enManSkill_HateAtk_2){
                    m_AtkInfo.iAttackPt = CSkillLib::Ins()->getHateAtkPt(m_iHP, m_iBaseHP);
                }
    
                (m_pListener->*m_pfnSelector)(&m_AtkInfo);
                
                if (m_AtkInfo.iManSkillID >= enManSKill_HuoXinAttack
                    && m_AtkInfo.iManSkillID <= enManSKill_AnXinAttack)
                {
                    CAttackInfo secondAtkInfo;
                    secondAtkInfo.iAttackPt = m_AtkInfo.iAttackPt * 3;
                    secondAtkInfo.iElement = enElementHeart;
                    secondAtkInfo.iTargetCnt = 1;
                    secondAtkInfo.srcPt = CCPointMake(m_AtkInfo.srcPt.x, m_AtkInfo.srcPt.y);
                    secondAtkInfo.iManSkillID = m_AtkInfo.iManSkillID;
                    secondAtkInfo.bAtkOvr = m_AtkInfo.bAtkOvr;
                    
                    (m_pListener->*m_pfnSelector)(&secondAtkInfo);
                }
                else if (m_AtkInfo.iManSkillID == enManSkill_xunjiezhadan)
                {
                    if (((FightMngr*)m_pListener)->isEnemyDelayed() == false
                        && SaveData::Inst()->getrand() < 20)
                    {
                        CAttackInfo secondAtkInfo;
                        secondAtkInfo.iAttackPt = 0;
                        secondAtkInfo.iElement = m_AtkInfo.iElement;
                        secondAtkInfo.iTargetCnt = 1;
                        secondAtkInfo.srcPt = CCPointMake(m_AtkInfo.srcPt.x, m_AtkInfo.srcPt.y);
                        secondAtkInfo.iManSkillID = enManSkill_YanChang2;
                        secondAtkInfo.bAtkOvr = m_AtkInfo.bAtkOvr;
                        
                        (m_pListener->*m_pfnSelector)(&secondAtkInfo);
                    }
                }
                else if ((m_AtkInfo.iManSkillID == enManSkill_HP1Atk50
                        ||m_AtkInfo.iManSkillID == enManSkill_HP1Atk50_2
                        ||m_AtkInfo.iManSkillID == enManSkill_FuXiao
                        ||m_AtkInfo.iManSkillID == enManSkill_EndOfWorld
                        ||m_AtkInfo.iManSkillID == enManSkill_236))
                {
                    m_iHP = 1;
                    updateBlood();
                }
                else if (m_AtkInfo.iManSkillID == enManSkill_280) //5万点光属性全体伤害
                {
                    CAttackInfo secondAtkInfo;
                    secondAtkInfo.iAttackPt = m_AtkInfo.iAttackPt * 0.5;
                    secondAtkInfo.iElement = enElementLight;
                    secondAtkInfo.iTargetCnt = 1;
                    secondAtkInfo.srcPt = CCPointMake(m_AtkInfo.srcPt.x, m_AtkInfo.srcPt.y);
                    secondAtkInfo.iManSkillID = m_AtkInfo.iManSkillID;
                    secondAtkInfo.bAtkOvr = m_AtkInfo.bAtkOvr;
                    
                    (m_pListener->*m_pfnSelector)(&secondAtkInfo);
                }
                
                break;
            }
            case enManSkillTp_delay:
            {
                if (m_iCurManSkillID == enManSkill_255) {
                    m_stEnhanceH6BuffGrp.iCD = 1;
                    m_stEnhanceH6BuffGrp.iElem = enElementFire;
                    m_stEnhanceH6BuffGrp.fFactor += 0.3;
                    if (m_stEnhanceH6BuffGrp.sp == NULL) {
                        addEnhanceBuffH6();
                    } else {
                        char buf[50];
                        sprintf(buf, "%.0f%%", m_stEnhanceH6BuffGrp.fFactor*100);
                        m_stEnhanceH6BuffGrp.lb->setString(buf);
                    }
                }
                FightMngr* fm = (FightMngr*) m_pListener;
                if (!(fm->isEnemyDelayed())) {
                    (m_pListener->*m_pfnSelector)(&m_AtkInfo);
                }
                break;
            }
            case enManSkillTp_break:
                (m_pListener->*m_pfnSelector)(&m_AtkInfo);
                
                break;
                
            case enManSkillTp_sick:
            case enManSkillTp_poison:
                (m_pListener->*m_pfnSelector)(&m_AtkInfo);
                break;
                
            case enManSkillTp_mabi:
                (m_pListener->*m_pfnSelector)(&m_AtkInfo);
                break;
                
            case enManSkillTp_chgElm:{
                (m_pListener->*m_pfnSelector)(&m_AtkInfo);
                if (m_AtkInfo.iManSkillID == enManSkill_285) {
                    m_stoneGrid->chgStone2(enElementMax, (enElementLight<<4 | enElementHeart));
                }
            }
                break;
                
            case enManSkillTp_recover:
                if (m_AtkInfo.iManSkillID == enManSkill_RuMuCf) {
                    m_stRcvBuffGrp.iCD = pManSkill->cContinue;
                    m_stRcvBuffGrp.fEffect = pManSkill->fEffect;

                    addRcvBuff();

                    SaveData::Inst()->setRecoverCd(m_stRcvBuffGrp.iCD);
                    SaveData::Inst()->setRecoverEffect(m_stRcvBuffGrp.fEffect);
                }
                else if (m_AtkInfo.iManSkillID == enManSkill_257) {
                    m_stRcvBuffGrp.iCD = pManSkill->cContinue - 1;    //  先减去1，因为接下来就会消耗一次
                    m_stSickBuffGrp.bIsManSkill_257 = true;
                    
                    addRcvBuff();
                    
                    addSickBuff();
                    
                    m_bloodNum->setNumber(0, 0.1, 0.1);
                    m_bloodNum->setIsVisible(true);
                    m_bloodNum->setNumber(m_AtkInfo.iAttackPt, 0.3, 0.2);
                    schedule(schedule_selector(Player::hideBloodNum), 1.0);
                    m_iHP += m_AtkInfo.iAttackPt;
                    if(m_iHP > m_iBaseHP.get())
                        m_iHP = m_iBaseHP;
                    
                    // 满血状态下攻击翻倍技能特殊处理，针对消除之前使用回血技能
                    updateRoundStartHp();
                    
                    updateBlood();
                    
                    SaveData::Inst()->setRecoverCd(m_stRcvBuffGrp.iCD);   //
                }
                else if (m_AtkInfo.iManSkillID == enManSkill_263)//血量完全回复，木属性珠子攻击加强
                {
                    m_bloodNum->setNumber(0, 0.1, 0.1);
                    m_bloodNum->setIsVisible(true);
                    m_bloodNum->setNumber(m_AtkInfo.iAttackPt, 0.3, 0.2);
                    schedule(schedule_selector(Player::hideBloodNum), 1.0);
                    m_iHP += m_AtkInfo.iAttackPt;
                    if(m_iHP > m_iBaseHP.get())
                        m_iHP = m_iBaseHP;
                    
                    // 满血状态下攻击翻倍技能特殊处理，针对消除之前使用回血技能
                    updateRoundStartHp();
                    
                    updateBlood();
                    m_stoneGrid->giveStonePlus(enElementWood | 0xF0);

                }
                else {
                    int tempRecoverPt = 0;
                    if(pManSkill->iEffectBase == 6) {
                        tempRecoverPt= m_card[m_iTouchedCard]->getRecover() * pManSkill->fEffect;;
                    } else {
                        tempRecoverPt= pManSkill->iEffectBase;
                    }
                    m_bloodNum->setNumber(0, 0.1, 0.1);
                    m_bloodNum->setIsVisible(true);
                    m_bloodNum->setNumber(tempRecoverPt, 0.3, 0.2);
                    schedule(schedule_selector(Player::hideBloodNum), 1.0);
                    m_iHP += tempRecoverPt;
                    if(m_iHP > m_iBaseHP.get())
                        m_iHP = m_iBaseHP;
                    
                    // 满血状态下攻击翻倍技能特殊处理，针对消除之前使用回血技能
                    updateRoundStartHp();
                    
                    updateBlood();
                }
                break;

            case enManSkillTp_armor:
            {
                m_stArmorBuffGrp.iCD = pManSkill->cContinue;
                m_stArmorBuffGrp.fEffect = pManSkill->fEffect;
                    
                addArmorBuff();
                    
                SaveData::Inst()->setSheildCd(m_stArmorBuffGrp.iCD);
                SaveData::Inst()->setArmorEffect(m_stArmorBuffGrp.fEffect);
            }
                break;
                
            case enManSkillTp_wudi:
            {
                m_stWudiBuffGrp.iCD = pManSkill->cContinue;
                m_stWudiBuffGrp.iElem = pManSkill->cElement;
                m_stWudiBuffGrp.enType = (enWudiTypeDef)pManSkill->iEffectBase;
                
                addWudiBuff();
                
                SaveData::Inst()->setWudiCd(m_stWudiBuffGrp.iCD);
                SaveData::Inst()->setWudiElement(m_stWudiBuffGrp.iElem);
                SaveData::Inst()->setWudiType(m_stWudiBuffGrp.enType);
             
                if (enManSkill_RcvStnEfct==m_AtkInfo.iManSkillID)
                {   // 卧龙的武将技能127，木宝石都变为无效宝石
                    m_stoneGrid->chgStone(m_stWudiBuffGrp.iElem | 0xF0, enElementInvalid | 0xF0);
                }

                break;
            }
                
            case enManSkillTp_EnWuJiangType:
            {
                if(m_stEnhanceBuffGrp.iCD <= 0 && m_stEnWujiangBuffGrp.iCD<=0)
                {
                    m_stEnWujiangBuffGrp.iCD = pManSkill->cContinue;
                    m_stEnWujiangBuffGrp.iType = pManSkill->cElement;
                    m_stEnWujiangBuffGrp.fFactor = pManSkill->fEffect;
                    if (m_AtkInfo.iManSkillID == enManSkill_SklCdDwn) { //  129号武将技
                        m_bIsManSkill_129 = true;
                    }
                    
                    addEnWujiangBuff();
                    FightMngr* fm = (FightMngr*) m_pListener;
                    if(fm->isEnemyPoisned() == false){
                       if (m_AtkInfo.iManSkillID == enManSkill_284) {
                       (m_pListener->*m_pfnSelector)(&m_AtkInfo);
                      }
                  }
                }
            }
                break;
            case enManSkillTp_ReCover_WuJiangType:{//m_bIsManSkill_264

                m_stRcvWuJiangBuffGrp.iCD = pManSkill->cContinue;
                m_stRcvWuJiangBuffGrp.fFactor = pManSkill->fEffect;
                m_iBaseRecoverPt *= m_stRcvWuJiangBuffGrp.fFactor;
                for (int i = 0; i < kMaxCardCntInBattle; i++) {
                    if (m_card[i] == NULL) {
                        continue;
                    }
                    m_card[i]->setRecoverFactor(m_stRcvWuJiangBuffGrp.fFactor);
                }
                addRcvWuJiangBuff();
           
                SaveData::Inst()->setRecoverWuJiangCd(m_stRcvWuJiangBuffGrp.iCD);
                SaveData::Inst()->setRecoverWuJiangEffect(m_stRcvWuJiangBuffGrp.fFactor);
           
                break;
            }
            case enManSkillTp_CountryWujiangType:{
                m_stCountryWujiangBuffGrp.iCD = pManSkill->cContinue;
                m_stCountryWujiangBuffGrp.iType = pManSkill->cElement;
                m_stCountryWujiangBuffGrp.fFactor = pManSkill->fEffect;
                addCountryWujiangBuff();
                
                
                break;
            }
            case enManSkillTp_Enhance:
            {
                if(m_stEnhanceBuffGrp.iCD <= 0 && m_stEnWujiangBuffGrp.iCD<=0)
                {
                    m_stEnhanceBuffGrp.iCD = pManSkill->cContinue;
                    m_stEnhanceBuffGrp.iElem = pManSkill->cElement;
                    m_stEnhanceBuffGrp.fFactor = pManSkill->fEffect;
                    addEnhanceBuff();
                }
                
                if (m_AtkInfo.iManSkillID == enManSkill_279) {    // 279武将技 特殊处理（1+1）
                    m_stoneGrid->giveStonePlus(pManSkill->cElement | 0xF0);
                }
                if (m_AtkInfo.iManSkillID == enManSkill_282) {
                    m_stoneGrid->ignitFreeMode(10.0f);
                }
                
            }
                break;
                
            case enManSkillTp_stonetrans:
                if (pManSkill->fEffect == 1)
                    m_stoneGrid->chgStone2(pManSkill->cElement, pManSkill->iEffectBase);
                else
                    m_stoneGrid->chgStone(pManSkill->cElement, pManSkill->iEffectBase);
                
                if (m_AtkInfo.iManSkillID == enManSkill_256) {    // 256武将技 特殊处理（1+1）
                    m_stoneGrid->giveStonePlus(enElementWater | 0xF0);
                }
                if (m_AtkInfo.iManSkillID == enManSkill_276) {
                    m_stoneGrid->ignitFreeMode(pManSkill->fEffect);
                }
                break;
            case enManSkillTp_stonerand:
                if (pManSkill->fEffect == 1)
                    m_stoneGrid->chgStone2(pManSkill->cElement, pManSkill->iEffectBase);
                else
                    m_stoneGrid->chgStone(pManSkill->cElement, pManSkill->iEffectBase);
                break;
            case enManSkillTp_luanshi:
                m_stoneGrid->ignitFreeMode(pManSkill->fEffect);
                break;
                
            case enManSkillTp_StnPlus:
                m_stoneGrid->giveStonePlus(pManSkill->cElement);
                break;
            case enManSkillTp_stoneAtk:
                m_AtkInfo.iAttackPt = m_AtkInfo.iAttackPt*m_stoneGrid->clearStone(pManSkill->cElement, pManSkill->iEffectBase);
                (m_pListener->*m_pfnSelector)(&m_AtkInfo);
                
                break;
            case enManSkillTp_chglocal:{
                CGameState::Instance()->setState(enStateTouchEnd);
                Card* card1;
                Card* card2;
                int index1 = 0;
                int index2 = m_iTouchedCard;
                if (!SaveData::Inst()->isReplace()) {
                    index2 = m_iTouchedCard;
                    SaveData::Inst()->setReplace(true);
                    SaveData::Inst()->setPosition(index2);
                }else{
                    if (m_iTouchedCard == 0) {
                        index2 = SaveData::Inst()->getPosition();
                        SaveData::Inst()->setReplace(false);
                        SaveData::Inst()->setPosition(0);
                    }else{
                        index2 = m_iTouchedCard;
                        SaveData::Inst()->setReplace(true);
                        SaveData::Inst()->setPosition(index2);
                    }
                }
                card1 = m_card[index1];
                card2 = m_card[index2];
                
                if (card1->getState()!=EN_CARD_STATE_FNEGYIN) {
                    fengYinAutoSkill(0);
                }
                
                ccBezierConfig bezier1;
                bezier1.controlPoint_1 = ccp(55, 100);
                bezier1.controlPoint_2 = ccp(100*index2, 100);
                bezier1.endPosition = ccp(106*index2,0);
                card1->runAction(CCSequence::actions(CCBezierBy::actionWithDuration(1, bezier1),
                                                     CCMoveBy::actionWithDuration(0, ccp(-106*index2, 0)),
                                                         CCCallFuncND::actionWithTarget(this,callfuncND_selector(Player::cbEndChange),(void*)(index2)),
                                                         NULL));
                
                card1->setCardIndex(index2);
                m_card[index2] = card1;
                
                ccBezierConfig bezier2;
                bezier2.controlPoint_1 = ccp(-55, 100);
                bezier2.controlPoint_2 = ccp(-100*index2, 100);
                bezier2.endPosition = ccp(-106*index2,0);
                card2->runAction(CCSequence::actions(CCBezierBy::actionWithDuration(1, bezier2),
                                                     CCMoveBy::actionWithDuration(0, ccp(106*index2, 0)),
                                                     CCCallFuncND::actionWithTarget(this,callfuncND_selector(Player::cbEndChange),(void*)(index1)),
                                                     NULL));
                card2->setCardIndex(index1);
                m_card[index1] = card2;
                
                if (card2->getState()!=EN_CARD_STATE_FNEGYIN) {
                    unFengYinAutoSkill(0);
                }
                
                SaveData_Card cardInfo1 = SaveData::Inst()->getCardInfo2(index1);
                SaveData_Card cardInfo2 = SaveData::Inst()->getCardInfo2(index2);
                
                SaveData::Inst()->setCardInfo(index1,cardInfo2);
                SaveData::Inst()->setCardInfo(index2,cardInfo1);
                
                break;
            }
            default:
                break;
        }
    }
    // 还原武将技能
    if (iRandManSkillId == enManSkill_258) {
        m_card[m_iTouchedCard]->setManSkillId(enManSkill_258);
        m_card[m_iTouchedCard]->initSkillAtk();
        m_AtkInfo = m_card[m_iTouchedCard]->useManSkill();
        m_iCurManSkillID = m_AtkInfo.iManSkillID;
    } else if (iRandManSkillId == enManSkill_259) {
        m_card[m_iTouchedCard]->setManSkillId(enManSkill_259);
        m_card[m_iTouchedCard]->initSkillAtk();
        m_AtkInfo = m_card[m_iTouchedCard]->useManSkill();
        m_iCurManSkillID = m_AtkInfo.iManSkillID;
    }
    m_stoneGrid->enableTouch(true);
    m_bTouchEnable = true;
}

void Player::cbEndChange(CCObject* obj,int tag)
{
    m_card[tag]->resetCardPos();
    if(CGameState::Instance()->getState() != enStateTouching)
        CGameState::Instance()->setState(enStateWaitTouch);
}

void Player::hideBloodNum()
{
    unschedule(schedule_selector(Player::hideBloodNum));
    if (m_bloodNum->getIsVisible()) {
        m_bloodNum->setNumber(0, 0.1, 0.1);
        m_bloodNum->setIsVisible(false);
    }
}

//******************************************************************************
// updateBlood
//******************************************************************************
void Player::updateBlood(bool bUpdate)
{
    if(m_iHP > m_iBaseHP.get())
        m_iHP = m_iBaseHP;
    
    m_blood->setBaseBlood(m_iBaseHP);

    if((m_iPrevHp.get() != m_iHP)
       || (m_iPreBaseHp.get() != m_iBaseHP)
       || bUpdate)
    {
        reorderChild(m_blood, 10);
        m_blood->setBlood(m_iHP,0.3,(m_iPreBaseHp.get() != m_iBaseHP));
    }
    
    if(m_iHP < 0)
        m_iHP = 0;
    
    m_iPrevHp = m_iHP;
    m_iPreBaseHp = m_iBaseHP;
}

void Player::fengYinUpdate(int cd)
{
    for(int i = 0; i < kMaxCardCntInBattle; i++)
    {
        if(m_card[i] == NULL)
            continue;
        
        if (m_card[i]->getState() == EN_CARD_STATE_FNEGYIN)
        {
            m_card[i]->fengYinUpdate(cd);
        }
    }
}

int Player::getBaseRecoverPt()const
{
    return m_iBaseRecoverPt;
}

void Player::setBaseRecoverPt(int nBaseRecoverPt)
{
    m_iBaseRecoverPt = nBaseRecoverPt;
}

int Player::getRealBaseRecoverPt()
{
    int realBaseRecoverpt = m_iBaseRecoverPt;
    if (m_card[0]->getAutoSkId() == enAutoSkill_385 && m_card[5]->getAutoSkId() == enAutoSkill_385) {
        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] == NULL)
                continue;
            if (m_card[i]->getCategory() == enGeneralTp_GuoShi || m_card[i]->getSubCategory() == enGeneralTp_GuoShi) {
                // 回复力提升
                if (m_iHP < 0.5*m_iBaseHP) {
                    int preCardRecover = m_card[i]->getRecover();
                    //m_card[i]->setRecoverFactor(2);
                    int curCardRecover = preCardRecover*4;
                    int coverOff = curCardRecover - preCardRecover;
                    realBaseRecoverpt += coverOff;
                }
            }
        }
    } else if (m_card[0]->getAutoSkId() == enAutoSkill_385 || m_card[5]->getAutoSkId() == enAutoSkill_385) {
        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] == NULL)
                continue;
            if (m_card[i]->getCategory() == enGeneralTp_GuoShi || m_card[i]->getSubCategory() == enGeneralTp_GuoShi) {
                // 回复力提升
                if (m_iHP < 0.5*m_iBaseHP) {
                    int preCardRecover = m_card[i]->getRecover();
                    //m_card[i]->setRecoverFactor(2);
                    int curCardRecover = preCardRecover*2;
                    int coverOff = curCardRecover - preCardRecover;
                    realBaseRecoverpt += coverOff;
                }
            }
        }
    }
    return realBaseRecoverpt;
}

//******************************************************************************
// attackOver
//******************************************************************************
void Player::attackOver()
{
    m_AtkInfo.bAtkOvr = true;
    m_AtkInfo.iElement = kInvalidValue;
    m_bIsOver6Fire = false;
    m_iAwakeSkill_33 = 0;
    
    //fengYinCD--
    fengYinUpdate(1);
    
    if(m_bErased){
        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] != NULL){
                m_card[i]->decCD();
                updateSkillHighlight();
                
                m_card[i]->reset();
            }
        }
        m_bloodNum->setIsVisible(false);
    }
    
    unschedule(schedule_selector(Player::attack));
}

//resetCardSkillCD
void Player::resetSkillCD()
{
    for(int i = 0; i < kMaxCardCntInBattle; i++)
    {
        if(m_card[i] != NULL)
        {
            m_card[i]->resetSkillCD();
            updateSkillHighlight();
        }
    }
}

//******************************************************************************
// attack
//******************************************************************************
void Player::makeRcvAttack()
{
    m_AtkInfo.iAutoSkillID = kInvalidValue;
    m_AtkInfo.iManSkillID = kInvalidValue;
    m_AtkInfo.bAtkOvr = false;
    m_AtkInfo.iTargetCnt = 1;
    m_AtkInfo.srcPt = m_blood->getPosition();
    m_AtkInfo.iElement = enElementRecover; 
    m_AtkInfo.iAttackPt = m_iRcvAttack;
    m_iRcvAttack = 0;
}

//******************************************************************************
// attack
//******************************************************************************
void Player::attack(ccTime dt)
{
    bool bDoAutoSkillAfterAttacking = false;
    
    unschedule(schedule_selector(Player::attack));
    
    schedule(schedule_selector(Player::attack), 0.2);

    while(m_iCurAttackCard < kMaxCardCntInBattle)
    {
        
        if(m_bIsMainAtk
           &&(m_card[m_iCurAttackCard] != NULL)
           && (m_card[m_iCurAttackCard]->getAttackInfo().iAttackPt > 0))
        {
            break;
        }
        
        if((!m_bIsMainAtk)
           &&(m_card[m_iCurAttackCard] != NULL)
           && (m_card[m_iCurAttackCard]->getSubAttackInfo().iAttackPt > 0))
        {
            break;
        }
        
        m_iCurAttackCard++;
        
        if (m_iCurAttackCard == kMaxCardCntInBattle && m_bIsMainAtk)
        {
            m_iCurAttackCard = 0;
            m_bIsMainAtk = false;
        }
    }
    
    if(CGameState::Instance()->getState() >= enStateEnemyDead)
    {
        attackOver();
    }
    else
    //被动技施放结束
    if(m_iCurAttackCard > kMaxCardCntInBattle && (!m_bIsMainAtk))
    {
        if (m_iRcvAttack > 0
            && m_iCurAttackCard == kMaxCardCntInBattle + 1)
            makeRcvAttack();
        else
            attackOver();
    }
    //有被动技时使用被动技
    else if(m_iCurAttackCard == kMaxCardCntInBattle && (!m_bIsMainAtk))
    {
        m_iCurAttackCard++;
        bDoAutoSkillAfterAttacking = doAutoSkillAfterAttacking();
        if(bDoAutoSkillAfterAttacking == false)
        {
            if (m_iRcvAttack > 0)
                makeRcvAttack();
            else
                attackOver();
        }
    }
    else
    {
        if (m_bIsMainAtk)
        m_AtkInfo = m_card[m_iCurAttackCard]->getAttackInfo();
        else
        m_AtkInfo = m_card[m_iCurAttackCard]->getSubAttackInfo();
            
        if(m_iCurManSkillID == enManSkill_SuiJi)
            m_AtkInfo.iManSkillID = enManSkill_SuiJi;
        if(m_iCurManSkillID == enManSkill_260)
            m_AtkInfo.iManSkillID = enManSkill_260;
        
        m_card[m_iCurAttackCard]->attack();
        if (m_card[m_iCurAttackCard]->getCanUseNumByAwakeId(enAwakeSkill_28)>0 && m_card[m_iCurAttackCard]->bisTouchOffAwakeSkill_28()) {//判断是否触发破坏神这个能力突破(做两次攻击)
            if (stAwakeSkill *pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(enAwakeSkill_28))
            {
                int count = m_card[m_iCurAttackCard]->getCanUseNumByAwakeId(enAwakeSkill_28);
                int nOdds = count * (pAwakeSkill->cOdds);
                if (SaveData::Inst()->getrand()<nOdds)
                {
                    m_card[m_iCurAttackCard]->setisTouchOffAwakeSkill_28(false);
                    m_iCurAttackCard--;
                }
            }
        }
          m_iCurAttackCard++;
        if (m_iCurAttackCard == kMaxCardCntInBattle && m_bIsMainAtk)
        {
            m_iCurAttackCard = 0;
            m_bIsMainAtk = false;
            for (int i = 0;  i < kMaxCardCntInBattle; i++) {
                if (m_card[i] == NULL) {
                    continue;
                }
                m_card[i]->setisTouchOffAwakeSkill_28(true);
            }

        }
    }
    
    if(CGameData::Inst()->isSoundOn){
        switch (m_AtkInfo.iElement) {
            case enElementWood:
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(wood));
                break;
                
            case enElementWater:
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(water));
                break;
                
            case enElementLight:
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(light));
                break;
                
            case enElementDark:
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(dark));
                break;
                
            case enElementFire:
                SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(fire));
                break;
                
            case enElementHeart:
            default:
                break;
        }
    }
    
    if (m_AtkInfo.bAtkOvr) {
        
        m_iRoundEndHp = m_iHP;
        
        if (m_stRcvBuffGrp.iCD > 0) {
           
            m_iHP += m_iBaseHP * m_stRcvBuffGrp.fEffect;
            
            updateBlood();
          
        }
        
    }
    
    
    if((m_iNewbieStep == 1) && (m_AtkInfo.bAtkOvr)){//shikun_new
        //新手引导第一步打完之后要停下来出个提示框
//        m_pMsgBox = new MsgBox("commdlg.png", getPlayerTip(), ccc3(255, 255, 255));
//        addChild(m_pMsgBox, 10);
//        m_pMsgBox->release();
//        m_pMsgBox->setPosition(CCPointMake(320, 268));
//        
//        m_pMsgBox->setScale(0.0);
//        m_pMsgBox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
//    
//        MainScene::Inst()->showTouchTip();
        
        m_iNewbieCovIdx = 0;
        m_newbieCover[m_iNewbieCovIdx] = CCSprite::spriteWithFile(gNewbieCover[m_iNewbieCovIdx].filename);
        addChild(m_newbieCover[m_iNewbieCovIdx]);
        m_newbieCover[m_iNewbieCovIdx]->setPosition(gNewbieCover[m_iNewbieCovIdx].pos);
        m_newbieCover[m_iNewbieCovIdx]->runAction(CCBlink::actionWithDuration(100, 500));
        
        MainScene::Inst()->showTouchTip();
    } else if((m_iNewbieStep == 2 ) && (m_AtkInfo.bAtkOvr)){
        //新手引导第一步打完之后要停下来出个提示框
        
        m_iNewbieCovIdx = 1;//shikun_new
        m_newbieCover[m_iNewbieCovIdx] = CCSprite::spriteWithFile(gNewbieCover[m_iNewbieCovIdx].filename);
        addChild(m_newbieCover[m_iNewbieCovIdx] );
        m_newbieCover[m_iNewbieCovIdx]->setPosition(gNewbieCover[m_iNewbieCovIdx].pos);
        m_newbieCover[m_iNewbieCovIdx]->runAction(CCBlink::actionWithDuration(100, 500));
        
        
        MainScene::Inst()->showTouchTip();
//        m_pMsgBox = new MsgBox("commdlg.png", getPlayerTip(), ccc3(255, 255, 255));
//        addChild(m_pMsgBox, 10);
//        m_pMsgBox->release();
//        m_pMsgBox->setPosition(CCPointMake(320, 268));
//        m_pMsgBox->setScale(0.0);
//        m_pMsgBox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
//        
//        MainScene::Inst()->showTouchTip();
    }
    else
    {
        if((m_iCurManSkillID == enManSkill_SuiJi || m_iCurManSkillID == enManSkill_260) && (m_AtkInfo.bAtkOvr))
        {
            if(CGameState::Instance()->getState() != enStateEnemyDead
               && CGameState::Instance()->getState() != enStateEnemyDeadGameOver
               )
            {
                // 随机转珠子技能与神吕布技能冲突
                updateRoundStartHp();
                m_iCurManSkillID = enManSkill_NULL;
                m_bErased = false;
                m_AtkInfo.clear();
                
                memset(m_StoneEraseCnt, 0, sizeof(m_StoneEraseCnt));
                memset(m_bElementAttck, 0, sizeof(m_bElementAttck));
                
                m_bIsMainAtk = true;
                m_iCurAttackCard = 0;
                m_iRecoverPt = 0;
                m_iComboBaseRcvPt = 0;
                
                m_bloodNum->setIsVisible(false);

                reorderChild(m_blood, 0);

                if(CGameState::Instance()->getState() != enStateTouching)
                    CGameState::Instance()->setState(enStateWaitTouch);
                
                m_stoneGrid->startNewRound();
            }
        }
        else
        {
            if(bDoAutoSkillAfterAttacking == false)
            {
                m_AtkInfo.iEnemyIdx = kInvalidValue;
                (m_pListener->*m_pfnSelector)(&m_AtkInfo);
            }
        }
        
    }
}

void Player::beDirAtk(CAttackInfo* atkinfo)
{
    //有防御buff
    if(m_stArmorBuffGrp.iCD > 0)
        atkinfo->iAttackPt *= m_stArmorBuffGrp.fEffect;
    
    //有无敌buff
    if(m_stWudiBuffGrp.iCD > 0){
        if (m_stWudiBuffGrp.enType == enWudiType_Hurt)
        {//按伤害次数无敌
            if (atkinfo->iAttackPt > 0)
            {
                m_stWudiBuffGrp.enType = enWudiType_Normal;
                SaveData::Inst()->setWudiType(m_stWudiBuffGrp.enType);
            }
        }
        if (m_stWudiBuffGrp.enType == enWudiType_HarfHurt) {
            atkinfo->iAttackPt *= 0.5;
        }else if(m_stWudiBuffGrp.enType == enWudiType_NoHurt){
            atkinfo->iAttackPt = 0;
            atkinfo->iManSkillID = 0;
        }else{
            if((m_stWudiBuffGrp.iElem == enElementMax) || (m_stWudiBuffGrp.iElem == atkinfo->iElement)){
                atkinfo->iAttackPt = 0;
                atkinfo->iManSkillID = 0;
            }
            
            if ((m_stWudiBuffGrp.enType == enWudiType_OneAndAll) && (m_stWudiBuffGrp.iElem != atkinfo->iElement))
            {
                atkinfo->iAttackPt *= 0.5;
            }
        }
    }
    
    if(atkinfo->iAttackPt > 0)
    {
        //...
        doAwakeSkillBeforeBeingAttacked(atkinfo);
        
        doAutoSkillBeforeBeingAttacked(atkinfo);
        
        if(atkinfo->iManSkillID == enEnemySkill_YiShan)
            m_stoneGrid->enemyResetStone();
    }
    
    updateBlood();
    showHpDownNum(atkinfo->iAttackPt);
    
    m_blood->setPosition(CCPointMake(320, 545));
    m_blood->runAction(CCShake::actionWithDuration(0.5, m_blood->getPosition()));
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(hpdown));

}

//******************************************************************************
// beAttacked
//******************************************************************************
void Player::beAttacked(CAttackInfo* atkinfo)
{
    if(m_iNewbieStep == 5)
    {
        atkinfo->iAttackPt = 100;
    }
    //有防御buff
    if(m_stArmorBuffGrp.iCD > 0){
        if (atkinfo->iManSkillID == enEnemySkill_528) {
            
        }else
        atkinfo->iAttackPt *= m_stArmorBuffGrp.fEffect;
    }
    
    //有无敌buff
    if(m_stWudiBuffGrp.iCD > 0){
        if (m_stWudiBuffGrp.enType == enWudiType_Hurt)
        {//按伤害次数无敌
            if (atkinfo->iAttackPt > 0)
            {
                m_stWudiBuffGrp.enType = enWudiType_Normal;
                SaveData::Inst()->setWudiType(m_stWudiBuffGrp.enType);
            }
        }
        if (m_stWudiBuffGrp.enType == enWudiType_HarfHurt) {
            atkinfo->iAttackPt *= 0.5;
        }else if(m_stWudiBuffGrp.enType == enWudiType_NoHurt){
            atkinfo->iAttackPt = 0;
            atkinfo->iManSkillID = 0;
        }else{
            if((m_stWudiBuffGrp.iElem == enElementMax) || (m_stWudiBuffGrp.iElem == atkinfo->iElement)){
                atkinfo->iAttackPt = 0;
                atkinfo->iManSkillID = 0;
            }
            
            if ((m_stWudiBuffGrp.enType == enWudiType_OneAndAll) && (m_stWudiBuffGrp.iElem != atkinfo->iElement))
            {
                atkinfo->iAttackPt *= 0.5;
            }
        }
    }
    
    if(atkinfo->iAttackPt > 0)
    {
        //...
        doAwakeSkillBeforeBeingAttacked(atkinfo);
        
        doAutoSkillBeforeBeingAttacked(atkinfo);
        
        if(atkinfo->iManSkillID == enEnemySkill_YiShan)
            m_stoneGrid->enemyResetStone();

    }
    
    updateBlood();
    showHpDownNum(atkinfo->iAttackPt);
    
    CAttackInfo* tmpatk = new CAttackInfo(*atkinfo);
    m_EnemyAtkArr->addObject(tmpatk);
    tmpatk->release();
    
    if(m_iNewbieStep == 5){
        m_pMsgBox = new MsgBox("commdlg.png", getPlayerTip(), ccc3(255, 255, 255));
        addChild(m_pMsgBox, 10);
        m_pMsgBox->release();
        m_pMsgBox->setPosition(CCPointMake(320, 268));
        m_pMsgBox->setScale(0.0);
        m_pMsgBox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
   
        MainScene::Inst()->showTouchTip();
    }
    
    m_blood->setPosition(CCPointMake(320, 545));
    m_blood->runAction(CCShake::actionWithDuration(0.5, m_blood->getPosition()));
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(hpdown));
}

//******************************************************************************
// isDead
//******************************************************************************
bool Player::isDead()
{
    if(m_iHP > 0)
        return false;
    else
        return true;
}

//******************************************************************************
// shoot
//******************************************************************************
void Player::shoot(const CCPoint& src, const CCPoint& dst, int iElem)
{
    ccColor4B colorTbl[] = {
        ccc4(243, 71, 198, 255),
        ccc4(60, 140, 232, 255),
        ccc4(181, 0, 0, 255),
        ccc4(105, 255, 0, 255),
        ccc4(252, 255, 0, 255),
        ccc4(86, 0, 255, 255),
    };
    
    BulletLight* pBulletLight = new BulletLight();
    addChild(pBulletLight, 20, kTagToBeDel);
    pBulletLight->release();
    
    pBulletLight->shoot(src,dst,colorTbl[iElem], 0.0, 0.3);
}


//******************************************************************************
// containsTouchLocation
//******************************************************************************
int Player::containsTouchLocation(CCTouch *touch)
{
    int iRet = kInvalidValue;
    bool bRet = false;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint nodePos = convertToNodeSpace( touchLocation );
    
    CCPoint ptCard;
    CCSize  szCard;
    
    for(int i = 0; i < kMaxCardCntInBattle; i++){
        if(m_card[i] == NULL)
            continue;
        
        ptCard = m_card[i]->getCardPos();
        szCard = m_card[i]->getContentSize();
        
        bRet = CCRect::CCRectContainsPoint(CCRectMake(ptCard.x - szCard.width * 0.5, 
                                                      ptCard.y - szCard.height * 0.5, 
                                                      szCard.width, szCard.height),
                                           nodePos);
        
        if(bRet == true){
            iRet = i;
            break;
        }
    }

    return iRet;
}


//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool Player::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if(!m_bTouchEnable)
        return false;
    
    //新手引导的蒙板提示
    if((m_iNewbieCovIdx < kMaxNewbieCoverNum) && (m_newbieCover[m_iNewbieCovIdx] != NULL))
    {
        m_newbieCover[m_iNewbieCovIdx]->stopAllActions();
        m_newbieCover[m_iNewbieCovIdx]->setIsVisible(true);
        
        if(m_iNewbieCovIdx == 5)//shikun_new
            m_iNewbieCovIdx = 7;
        else if(m_iNewbieCovIdx == 1)
        {
            m_pMsgBox = new MsgBox("commdlg.png", getPlayerTip(), ccc3(255, 255, 255));
            addChild(m_pMsgBox, 10);
            m_pMsgBox->release();
            m_pMsgBox->setPosition(CCPointMake(320, 268));
            m_pMsgBox->setScale(0.0);
            m_pMsgBox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
            
            MainScene::Inst()->showTouchTip();
            
            m_iNewbieCovIdx = kMaxNewbieCoverNum;
            
            return false;
        }
        else if(m_iNewbieCovIdx == 0)
        {
            
            Arrow* pArrow = new Arrow(30, 255);
            
            pArrow->setPosition(CCPointMake(561, 875));
            
            pArrow->setRotation(180);
            pArrow->setScale(0.6f);
            
            addChild(pArrow, 500, kArenaArrowTag);
            pArrow->release();
            
            CCActionInterval* action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(-20, 0)),
                                                                               CCMoveBy::actionWithDuration(0.5, CCPointMake(20, 0)),
                                                                               NULL);
            pArrow->runAction(CCRepeatForever::actionWithAction(action));
            
            
            
            
            m_pMsgBox = new MsgBox("commdlg.png", getPlayerTip(), ccc3(255, 255, 255));
            addChild(m_pMsgBox, 10);
            m_pMsgBox->release();
            m_pMsgBox->setPosition(CCPointMake(320, 268));
            
            m_pMsgBox->setScale(0.0);
            m_pMsgBox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
            
            MainScene::Inst()->showTouchTip();

            m_iNewbieCovIdx = kMaxNewbieCoverNum;
            
            return false;
        }
        else
        m_iNewbieCovIdx++;
        
        if(m_iNewbieCovIdx < kMaxNewbieCoverNum){
            m_newbieCover[m_iNewbieCovIdx] = CCSprite::spriteWithFile(gNewbieCover[m_iNewbieCovIdx].filename);
            addChild(m_newbieCover[m_iNewbieCovIdx]);
            m_newbieCover[m_iNewbieCovIdx]->setPosition(gNewbieCover[m_iNewbieCovIdx].pos);
            m_newbieCover[m_iNewbieCovIdx]->runAction(CCBlink::actionWithDuration(100, 500));
        
            MainScene::Inst()->showTouchTip();
        }
        else {
            MainScene::Inst()->removeTouchTip();

            m_iNewbieMsgIdx = 0;
            m_pMsgBox = new MsgBox("commdlg.png", getPlayerTip(), ccc3(255, 255, 255));
            addChild(m_pMsgBox, 10);
            m_pMsgBox->release();
            m_pMsgBox->setPosition(CCPointMake(320, 268));
            m_pMsgBox->setScale(0.0);
            m_pMsgBox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
        
            MainScene::Inst()->showTouchTip();
        }
    }
    //移除msgbox
    else if(m_pMsgBox){
        
        //第四次niebie
        if(m_iNewbieStep == 4 && m_iNewbieCombStepIdx<=2)
        {
            int isReturnFalse = true;
            //选择敌人
            if(m_iNewbieCombStepIdx == 1 )
            {
                CCPoint touchLocation = touch->locationInView( touch->view() );
                touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
                CCPoint nodePos = convertToNodeSpace( touchLocation );
                
                CCPoint posEnemy = NewBieChooseEnemyPos;
                CCSize  szEnemy= NewBieChooseEnemySz;
                
                if(ccpDistance(posEnemy, nodePos) < szEnemy.width)
                {
                    if(m_pArrow)
                    {
                        removeChild(m_pArrow, true);
                        m_pArrow = NULL;
                        isReturnFalse = false;
                    }
                }
                else
                    return true;
    
            }
        
            removeChild(m_pMsgBox, true);
            m_pMsgBox = NULL;
            MainScene::Inst()->removeTouchTip();
            
            char tipBuf[50];
            m_iNewbieCombStepIdx ++;
            
            sprintf(tipBuf, "comboTip%d",m_iNewbieCombStepIdx);
            m_pMsgBox = new MsgBox("commdlg.png", CGameData::Inst()->getLanguageValue(tipBuf), ccc3(255, 255, 255));
            addChild(m_pMsgBox, 10);
            m_pMsgBox->release();
            m_pMsgBox->setPosition(CCPointMake(320, 268));
            m_pMsgBox->setScale(0.0);
            m_pMsgBox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
            
            MainScene::Inst()->showTouchTip();
            return isReturnFalse;
        }else{
            
            
            
            if(m_iNewbieStep == 5 && m_iNewbieMsgIdx == 4 && m_tempBoolNew)
            {
                if(CGameState::Instance()->getState() != enStateTouching)
                    CGameState::Instance()->setState(enStateWaitTouch);
                //CCLog("start new round %d",m_iNewbieMsgIdx);
                
                m_stoneGrid->startNewRound();//shikun_new
                
                m_tempBoolNew = false;
                
                
                CCNode* pArrow = getChildByTag(kArenaArrowTag);
                if(pArrow)
                    removeChild(pArrow, true);
                
                removeChild(m_pMsgBox, true);
                m_pMsgBox = NULL;
                MainScene::Inst()->removeTouchTip();
                
                return false;
            }
            else
            {
                CCNode* pArrow = getChildByTag(kArenaArrowTag);
                if(pArrow)
                    removeChild(pArrow, true);
                
                removeChild(m_pMsgBox, true);
                m_pMsgBox = NULL;
                MainScene::Inst()->removeTouchTip();
            }


            

        }
        CCActionInterval* action;
        switch (m_iNewbieMsgIdx) {
            case 0:
//            case 2:
//                CCLog("msgIdx 2");
//                break;
            case 3:
                
                if(m_iNewbieStep == 5)
                {
                    m_tempBoolNew = true;
                    m_pMsgBox = new MsgBox("commdlg.png", getPlayerTip(), ccc3(255, 255, 255));
                    addChild(m_pMsgBox, 10);
                    m_pMsgBox->release();
                    m_pMsgBox->setPosition(CCPointMake(320, 268));
                    m_pMsgBox->setScale(0.0);
                    m_pMsgBox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
                    
                    MainScene::Inst()->showTouchTip();

                    
                }
                else
                {
                    if(CGameState::Instance()->getState() != enStateTouching)
                        CGameState::Instance()->setState(enStateWaitTouch);
                    //CCLog("start new round %d",m_iNewbieMsgIdx);
                    
                    m_stoneGrid->startNewRound();//shikun_new
                }
                

                break;
            case 1:
                m_AtkInfo.bAtkOvr = true;
                (m_pListener->*m_pfnSelector)(&m_AtkInfo);
                break;
                
            case 5:
                while (m_card[0]->getCD() > 0)
                    m_card[0]->decCD();
                
                updateSkillHighlight();

                m_pArrow = new Arrow(30, 255);
                m_pArrow->setPosition(CCPointMake(50, 678));
                addChild(m_pArrow, 500);
                m_pArrow->release();
                m_pArrow->setRotation(-90);
                
                action = (CCActionInterval*) CCSequence::actions(CCMoveBy::actionWithDuration(0.5, CCPointMake(0, -20)),
                                                                   CCMoveBy::actionWithDuration(0.5, CCPointMake(0, 20)),
                                                                   NULL);
                m_pArrow->runAction(CCRepeatForever::actionWithAction(action));

                m_iNewbieMsgIdx++;
                m_pMsgBox = new MsgBox("commdlg.png", getPlayerTip(), ccc3(255, 255, 255));
                addChild(m_pMsgBox, 10);
                m_pMsgBox->release();
                m_pMsgBox->setPosition(CCPointMake(320, 268));
                m_pMsgBox->setScale(0.0);
                m_pMsgBox->runAction(CCEaseElasticOut::actionWithAction(CCScaleTo::actionWithDuration(0.5, 1.0),0.8f));
               
                MainScene::Inst()->showTouchTip();
                
                break;
                
            default:
                break;
        }
        
            if(m_iNewbieStep!=4)
                m_iNewbieMsgIdx++;
        
        return true;
        
    } else if(!m_pMsgBox){
        

    }
    
    
    
    if(CGameState::Instance()->getState() > enStateWaitTouch)
        return false;
    
    m_iTouchedCard =  containsTouchLocation(touch);
    
    if(m_iTouchedCard < kMaxCardCntInBattle){
        m_cardInfoDlg->setCardInfo(m_card[m_iTouchedCard]);
        FightMngr* fm = (FightMngr*) m_pListener;
        
        
        if (fm->enemyGrpIsLive())
        {
            m_cardInfoDlg->setSkill(m_card[m_iTouchedCard]->getManSkId(),
                                m_card[m_iTouchedCard]->getCD(),
                                fm->isEnemyBuffYanChi());
        }
        
        m_cardInfoDlg->setIsVisible(true);
        
        return true;
    }
    
#ifdef _DEBUG
    OcProxy::Inst()->showUsedMemory();
#endif
    return false;
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void Player::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if(CGameState::Instance()->getState() > enStateWaitTouch)
        return;
    
    m_iTouchedCard =  containsTouchLocation(touch);
    
    if(m_iTouchedCard < kMaxCardCntInBattle){
        m_cardInfoDlg->setCardInfo(m_card[m_iTouchedCard]);
        FightMngr* fm = (FightMngr*) m_pListener;
        
        if (fm->enemyGrpIsLive())
        {
            m_cardInfoDlg->setSkill(m_card[m_iTouchedCard]->getManSkId(),
                                    m_card[m_iTouchedCard]->getCD(),
                                    fm->isEnemyBuffYanChi());
        }
        
        m_cardInfoDlg->setIsVisible(true);
    }
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void Player::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    m_cardInfoDlg->setIsVisible(false);
    
    if(CGameState::Instance()->getState() != enStateWaitTouch)
        return;
    
    if(m_pMsgBox)
        return;
    
    m_iTouchedCard =  containsTouchLocation(touch);
    
    if(m_iTouchedCard < kMaxCardCntInBattle){
        if(m_card[m_iTouchedCard]->getCD() == 0 && m_card[m_iTouchedCard]->getState() != EN_CARD_STATE_FNEGYIN)
        {
            int iManSkillID = m_card[m_iTouchedCard]->getManSkId();
            bool bUsable = isManSkillCanUse(iManSkillID);
            if(bUsable){
                // enManSkill_258 / enManSkill_259 先随机到确定的武将技
                if (iManSkillID == enManSkill_258) {            //258 随机使用下列主动技能:（36威压、58乱世天下、94火烧连营、34蚀骨剧毒、42青囊秘术）
                    int index = 0;
                    int iRand = 0;
                    while ((iRand = (SaveData::Inst()->getrand()) % 5 + 1)) {
                        switch (iRand) {
                            case 1:
                                index = enManSkill_weiya;//36;
                                break;
                            case 2:
                                index = enManSkill_LuanShiTX;//58;
                                break;
                            case 3:
                                index = enManSkill_HuoShaoLy;//94;
                                break;
                            case 4:
                                index = enManSkill_ShiGuJuDu;//34;
                                break;
                            case 5:
                            default:
                                index = enManSkill_qingnangmishu;//42;
                                break;
                        }
                        bUsable = isManSkillCanUse(index);
                        if (bUsable) {
                            m_i258_259ManSkillID = index;
                            break;
                        }
                    }
                } else if (iManSkillID == enManSkill_259) {     //259 随机使用下列主动技能:（124破军点钢矛、224勇者高阶强化、31鬼王咆哮、132蓄势待发·木、213风卷残云）
                    int index = 0;
                    int iRand = 0;
                    while ((iRand = (SaveData::Inst()->getrand()) % 5 + 1)) {
                        switch (iRand) {
                            case 1:
                                index = enManSkill_WoodEnhance;//124;
                                break;
                            case 2:
                                index = enManSkill_224;//224;
                                break;
                            case 3:
                                index = enManSkill_shiyizhongli;//31;
                                break;
                            case 4:
                                index = enManSkill_StnPlsMu;//132;
                                break;
                            case 5:
                            default:
                                index = enManSkill_ZhuBiTe;//213;
                                break;
                        }
                        bUsable = isManSkillCanUse(index);
                        if (bUsable) {
                            m_i258_259ManSkillID = index;
                            break;
                        }
                    }
                } else {
                    // nothing
                }
                if (bUsable) {
                    m_skillDlg->setSkillID(iManSkillID);
                    m_skillDlg->setCD(m_card[m_iTouchedCard]->getCDMax());
                    m_skillDlg->show(true);
                }
            }
        }
    }
    return;
}

//******************************************************************************
// isManSkillCanUse
//******************************************************************************

bool Player::isManSkillCanUse(int manSkillID)
{
    RecordControl(false,2);
    bool bUsable = true;
    int iManSkillID = manSkillID;
    stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(iManSkillID);
    
    FightMngr* fm = (FightMngr*) m_pListener;
    
    //判斷該技能是否可用
    //如果乱世进行中
    if(m_stoneGrid->isFreeMode()){
        bUsable = false;
    }
    else if (fm->isEnemyBuffChenMo())
        // 敌将沉默buff
        bUsable = false;
    else if(pManSkill){
        if (pManSkill->cTargetCnt == 2)
        {
            if (!fm->haveEnemyWithElemInstead(pManSkill->cElement))
            {
                bUsable = false;
            }
        }
        else
        {
            switch (pManSkill->iType) {
                    //瘟疫
                case enManSkillTp_sick:
                    if(fm->isEnemySick())
                        bUsable = false;
                    
                    if (fm->getEnemyGrp()->isBuffEffect(enEnemyBuff_hunyuandun))
                        bUsable = false;
                    
                    if (fm->getEnemyGrp()->isBuffEffect(enEnemyBuff_ysshenyuan))
                        bUsable = false;
                    
                    if (fm->getEnemyGrp()->isBuffEffect(enEnemyBuff_ystaipingdao))
                        bUsable = false;
                    
                    if (fm->getEnemyGrp()->isBuffEffect(enEnemyBuff_aytaipingyaoshu))
                        bUsable = false;
                    
                    if (fm->getEnemyGrp()->isBuffEffect(enEnemyBuff_77))
                        bUsable = false;
                    
                    break;
                    
                    //毒系
                case enManSkillTp_poison:
                    if(fm->isEnemyPoisned())
                        bUsable = false;
                    
                    if (fm->getEnemyGrp()->isBuffEffect(enEnemyBuff_hunyuandun))
                        bUsable = false;
                    
                    if (fm->getEnemyGrp()->isBuffEffect(enEnemyBuff_ysshenyuan))
                        bUsable = false;
                    
                    if (fm->getEnemyGrp()->isBuffEffect(enEnemyBuff_ystaipingdao))
                        bUsable = false;
                    
                    if (fm->getEnemyGrp()->isBuffEffect(enEnemyBuff_aytaipingyaoshu))
                        bUsable = false;
                    
                    if (fm->getEnemyGrp()->isBuffEffect(enEnemyBuff_77))
                        bUsable = false;
                    
                    break;
                    
                    //延時
                case enManSkillTp_delay:
                    if(fm->isEnemyDelayed())
                        bUsable = false;
                    if (iManSkillID == enManSkill_255) {
                        bUsable = true;
                    }
                    break;
                    
                    //破甲
                case enManSkillTp_break:
                    if(fm->isEnemyArmorBroken())
                        bUsable = false;
                    break;
                    
                    //加血
                case enManSkillTp_recover:
                    if(m_iHP == m_iBaseHP.get()) {
                        bUsable = false;
                        if (iManSkillID == enManSkill_263 && m_stoneGrid->canGiveStonePlus(enElementWood | 0xF0) == true) {
                            bUsable = true;
                        }
                    }
                    else {
                        if (iManSkillID == enManSkill_RuMuCf) {
                            if (m_stRcvBuffGrp.iCD > 0) {
                                bUsable = false;
                            }
                        } else if (iManSkillID == enManSkill_257) {
                            if (m_stRcvBuffGrp.iCD > 0) {
                                bUsable = false;
                            }
                        }
                    }
                    break;
                    
                    //防禦姿態
                case enManSkillTp_armor:
                    if(m_stArmorBuffGrp.iCD > 0)
                        bUsable = false;
                    break;
                    
                    //寶石變化(無對應寶石)
                case enManSkillTp_stonetrans:
                    if(m_stoneGrid->haveStone(pManSkill->cElement) == false)
                        bUsable = false;
                    
                    if (iManSkillID == enManSkill_AllBianHuo
                        || iManSkillID == enManSkill_ShenYu
                        || iManSkillID == enManSkill_ZhuBiTe) {
                        if (m_stoneGrid->isAllElem(pManSkill->iEffectBase)) {
                            bUsable = false;
                        }
                    }
                    
                    if (iManSkillID == enManSkill_256) {     // 256武将技 特殊处理（1+1）
                        if(m_stoneGrid->canGiveStonePlus(enElementWater | 0xF0) == true
                           || m_stoneGrid->haveStone(pManSkill->cElement) == true)
                            bUsable = true;
                        else
                            bUsable = false;
                    }
                    
                    if (iManSkillID == enManSkill_276)
                    {   // 没有木宝石也一样放乱世
                        bUsable = true;
                    }
                    break;
                    
                    //宝石加号
                case enManSkillTp_StnPlus:
                    if(m_stoneGrid->canGiveStonePlus(pManSkill->cElement) == false)
                        bUsable = false;
                    break;
                    
                    //无敌
                case enManSkillTp_wudi:
                    if(m_stWudiBuffGrp.iCD > 0)
                        bUsable = false;
                    else{
                        if((enWudiTypeDef)pManSkill->iEffectBase==enWudiType_NoHurt
                           &&m_stoneGrid->haveStone(pManSkill->cElement | 0xF0) == false){
                            bUsable = false;
                        }
                    }
                    break;
                    //武将类型攻击加强
                case enManSkillTp_EnWuJiangType:
                    if (m_stEnWujiangBuffGrp.iCD > 0 || m_stEnhanceBuffGrp.iCD>0)
                        bUsable = false;
                    break;
                case enManSkillTp_CountryWujiangType:
                    if (m_stCountryWujiangBuffGrp.iCD > 0 ) {
                        bUsable = false;
                    }
                    break;
                case enManSkillTp_ReCover_WuJiangType://武将类型回复力加强 m_bIsManSkill_264
                    if (m_stRcvWuJiangBuffGrp.iCD > 0) {
                        bUsable = false;
                    }

                    break;
                    //x属性加强
                case enManSkillTp_Enhance:
                    if(m_stEnhanceBuffGrp.iCD > 0 || m_stEnWujiangBuffGrp.iCD>0)
                        bUsable = false;
                    
                    if (iManSkillID == enManSkill_279) {     // 279武将技 特殊处理
                        if(m_stoneGrid->canGiveStonePlus(pManSkill->cElement | 0xF0) == true)
                            bUsable = true;
                    }

                    break;

                    //麻痹
                case enManSkillTp_mabi:
                    if((fm->isEnemyMabi()) || (fm->haveEnemyWithElem(pManSkill->cElement) == false))
                        bUsable = false;
                    break;
                    
                    //反击
                case enManSkillTp_fanji:
                    if (m_stFanJiBuffGrp.iCD > 0)
                        bUsable = false;
                    break;
                    
                    //aoe
                case enManSkillTp_aoe:
                    if (m_stAOEBuffGrp.iCD > 0)
                        bUsable = false;
                    break;
                    
                    //改变敌怪属性
                case enManSkillTp_chgElm:
                    if ((fm->isEnemyChangeElement())
                        || (fm->haveEnemyWithNotElem(pManSkill->cElement) == false))
                        bUsable = false;
                    break;
                    
                    //黑雷的诅咒 大于1血才能使用
                case enManSkillTp_atk:
                    if ((iManSkillID == enManSkill_HP1Atk50
                         ||iManSkillID == enManSkill_HP1Atk50_2
                         ||iManSkillID == enManSkill_FuXiao
                         ||iManSkillID == enManSkill_EndOfWorld
                         ||iManSkillID == enManSkill_236)
                        && m_iHP <= 1)
                        bUsable = false;
                    
                    break;
                    
                case enManSkillTp_stoneAtk:
                    if(m_stoneGrid->haveStone(pManSkill->cElement, pManSkill->iEffectBase) == false)
                        bUsable = false;
                    break;
                case enManSkillTp_chglocal:
                    if(m_iTouchedCard == 5 || (m_iTouchedCard == 0 && !SaveData::Inst()->isReplace())
                       )
                        bUsable = false;
                    
                    {
                        stAutoSkill *pAutoSkill = CSkillLib::Ins()->getAutoSkill(m_card[0]->getAutoSkId());
                        if (pAutoSkill->iType == enAutoSkillIp_money_exp_drop)
                            bUsable = false;
                    }
                    break;
                case enManSkillTp_null:
                default:
                    break;
            }
        }
    }
    return bUsable;
}

//******************************************************************************
// initMatrix
//******************************************************************************
void Player::initMatrix(int newbieStep)
{
    m_iNewbieStep = newbieStep;
    
    string strDgType = CGameData::Inst()->getCurDgType();
    int iCurFloor = CGameData::Inst()->getCurFloor();
    int iCurRoom = CGameData::Inst()->getCurRoom();
    
    if ((strDgType.compare("normal") == 0)
        && (iCurFloor == 1)){
        m_stoneGrid->initGuideMatrix(iCurRoom-1);
    }
    else
    {
        m_stoneGrid->initMatrix(newbieStep);
    }
}

void Player::fengYinAutoSkill(int idx)
{
    if (idx != 0 && idx != 5)
    {
        return;
    }
    
    int iSkillId = m_card[idx]->getAutoSkId();
    
    stAutoSkill* pAutoSkill = CSkillLib::Ins()->getAutoSkill(iSkillId);
    int iType = (pAutoSkill)? pAutoSkill->iType : enAutoSkillTp_null;
    
    
    int preBaseHp = m_iBaseHP;
    
    //宝石移动延时技能的处理
    if(iType == enAutoSkillTp_delay)
    {
        float fDelay = m_stoneGrid->getAdditionalDur();
        fDelay -= pAutoSkill->fEffect;
        fDelay = fDelay>0.0f?fDelay:0.0f;
        m_stoneGrid->setAdditionalDur(fDelay);
    }
    //数值改变
    else if (iType == enAutoSkillTp_valChg_hp_Elm2)
    {   // 双属性
        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] == NULL)
                continue;
            
            if (m_card[i]->getElement() == (pAutoSkill->cElement & 0x0F)
                || m_card[i]->getElement() == ((pAutoSkill->cElement >> 4) & 0x0F)
                ||m_card[i]->getSubElement() == (pAutoSkill->cElement & 0x0F)
                || m_card[i]->getSubElement() == ((pAutoSkill->cElement >> 4) & 0x0F)
                )
            {
                int preCardHp = m_card[i]->getBaseHp();
                m_card[i]->decHpFactor(pAutoSkill->fEffect);
                int curCardHp = m_card[i]->getBaseHp();
                int hpOff = preCardHp - curCardHp;
                m_iBaseHP -= hpOff;
            }
        }
    }
    else if(iType == enAutoSkillTp_valChg_hp || iType == enAutoSkillTp_valChg_rcv
            || iType == enAutoSkillTp_valChg_atkrcv || iType == enAutoSkillTp_valChg_all
            || iType == enAutoSkillTp_valChg_hprcv || iType == enAutoSkillTp_valChg_atkhp)
    {   // 单属性
        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] == NULL)
                continue;
            
            if(pAutoSkill->cGenTp == m_card[i]->getCategory()
               || pAutoSkill->cGenTp == m_card[i]->getSubCategory()
               || pAutoSkill->cElement == m_card[i]->getElement()
               || pAutoSkill->cElement == m_card[i]->getSubElement()
               )
            {  
                if (iType == enAutoSkillTp_valChg_hp
                    || iType == enAutoSkillTp_valChg_atkhp)
                {
                    int preCardHp = m_card[i]->getBaseHp();
                    m_card[i]->decHpFactor(pAutoSkill->fEffect);
                    int curCardHp = m_card[i]->getBaseHp();
                    int hpOff = preCardHp - curCardHp;
                    m_iBaseHP -= hpOff;
                }
                else if (iType == enAutoSkillTp_valChg_rcv
                         || iType == enAutoSkillTp_valChg_atkrcv)
                {
                    int preCardRecover = m_card[i]->getRecover();
                    m_card[i]->decRecoverFactor(pAutoSkill->fEffect);
                    int curCardRecover = m_card[i]->getRecover();
                    int coverOff = preCardRecover - curCardRecover;
                    m_iBaseRecoverPt -= coverOff;
                }
                else if (iType == enAutoSkillTp_valChg_all
                         || iType == enAutoSkillTp_valChg_hprcv)
                {
                    int preCardRecover = m_card[i]->getRecover();
                    m_card[i]->decRecoverFactor(pAutoSkill->fEffect);
                    int curCardRecover = m_card[i]->getRecover();
                    int coverOff = preCardRecover - curCardRecover;
                    m_iBaseRecoverPt -= coverOff;
                    
                    int preCardHp = m_card[i]->getBaseHp();
                    m_card[i]->decHpFactor(pAutoSkill->fEffect);
                    int curCardHp = m_card[i]->getBaseHp();
                    int hpOff = preCardHp - curCardHp;
                    m_iBaseHP -= hpOff;
                }
            }
            else if(pAutoSkill->cGenTp == enGeneralTp_LongShen)
            {// 龙&神
                if((enGeneralTp_GuoShi == m_card[i]->getCategory())
                   || (enGeneralTp_God == m_card[i]->getCategory())
                   || (enGeneralTp_GuoShi == m_card[i]->getSubCategory())
                   || (enGeneralTp_God == m_card[i]->getSubCategory())
                   )
                {
                    int preCardHp = m_card[i]->getBaseHp();
                    m_card[i]->decHpFactor(pAutoSkill->fEffect);
                    int curCardHp = m_card[i]->getBaseHp();
                    int hpOff = preCardHp - curCardHp;
                    m_iBaseHP -= hpOff;
                }
            }
        }
    }
    
    // enAutoSkill_386
    if (m_card[idx]->getAutoSkId() == enAutoSkill_386 || m_card[idx]->getAutoSkId() == enAutoSkill_387) {
        bool hasViceGeneral = false;
        for (int i = 1; i < 5; i++) {
            if (m_card[i]) {
                hasViceGeneral = true;
                break;
            }
        }
        if (!hasViceGeneral) {
            int preCardHp = m_card[idx]->getBaseHp();
            m_card[idx]->decHpFactor(3);
            int curCardHp = m_card[idx]->getBaseHp();
            int hpOff = curCardHp - preCardHp;
            m_iBaseHP -= hpOff;
            
            int preCardRecover = m_card[idx]->getRecover();
            m_card[idx]->decRecoverFactor(3);
            int curCardRecover = m_card[idx]->getRecover();
            int coverOff = curCardRecover - preCardRecover;
            m_iBaseRecoverPt -= coverOff;
        }
    }
    
    // enAutoSkill_384
    if (iSkillId == enAutoSkill_384) {
        m_iHP /= 0.6;
        m_iBaseHP /= 0.6;
    }
    
    //  enAutoSkill_438,439
    if (iSkillId == enAutoSkill_438 || iSkillId == enAutoSkill_439) {
        m_iHP /= 0.5;
        m_iBaseHP /= 0.5;
    }


    //enAutoSkillIp_time_atk
    //变背景音乐
    if(iSkillId == enAutoSkill_JiangNan)
    {
        int iSkillId2 = m_card[idx==0?5:0]->getAutoSkId();
        if (iSkillId2 != enAutoSkill_JiangNan)
        {
            m_iSndTag = 0;
            m_stoneGrid->setSoundTag(0);
        }
    }
    
    if (preBaseHp != m_iBaseHP)
    {
        updateBlood();
    }
}

void Player::unFengYinAutoSkill(int idx)
{
    if (idx != 0 && idx != 5)
    {
        return;
    }
    
    int iSkillId = m_card[idx]->carryAutoSkId();
    
    stAutoSkill* pAutoSkill = CSkillLib::Ins()->getAutoSkill(iSkillId);
    int iType = (pAutoSkill)? pAutoSkill->iType : enAutoSkillTp_null;
    
    
    int preBaseHp = m_iBaseHP;
    
    //宝石移动延时技能的处理
    if(iType == enAutoSkillTp_delay)
    {
        float fDelay = m_stoneGrid->getAdditionalDur();
        fDelay += pAutoSkill->fEffect;
        fDelay = fDelay>0.0f?fDelay:0.0f;
        m_stoneGrid->setAdditionalDur(fDelay);
    }
    
    //数值改变
    if (iType == enAutoSkillTp_valChg_hp_Elm2)
    {   // 双属性
        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] == NULL)
                continue;
            
            if (m_card[i]->getElement() == (pAutoSkill->cElement & 0x0F)
                || m_card[i]->getElement() == ((pAutoSkill->cElement >> 4) & 0x0F)
                || m_card[i]->getSubElement() == (pAutoSkill->cElement & 0x0F)
                || m_card[i]->getSubElement() == ((pAutoSkill->cElement >> 4) & 0x0F)
                )
            {
                int preCardHp = m_card[i]->getBaseHp();
                m_card[i]->setHpFactor(pAutoSkill->fEffect);
                int curCardHp = m_card[i]->getBaseHp();
                int hpOff = curCardHp - preCardHp;
                m_iBaseHP += hpOff;
            }
        }
    }
    else if(iType == enAutoSkillTp_valChg_hp || iType == enAutoSkillTp_valChg_rcv
            || iType == enAutoSkillTp_valChg_atkrcv || iType == enAutoSkillTp_valChg_all
            || iType == enAutoSkillTp_valChg_hprcv || iType == enAutoSkillTp_valChg_atkhp)
    {   // 单属性
        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] == NULL)
                continue;
            
            if (pAutoSkill->cGenTp == m_card[i]->getCategory()
                || pAutoSkill->cGenTp == m_card[i]->getSubCategory()
                || pAutoSkill->cElement == m_card[i]->getElement()
                || pAutoSkill->cElement == m_card[i]->getSubElement()
                )
            {
                if (iType == enAutoSkillTp_valChg_hp
                    || iType == enAutoSkillTp_valChg_atkhp)
                {
                    int preCardHp = m_card[i]->getBaseHp();
                    m_card[i]->setHpFactor(pAutoSkill->fEffect);
                    int curCardHp = m_card[i]->getBaseHp();
                    int hpOff = curCardHp - preCardHp;
                    m_iBaseHP += hpOff;
                }
                else if (iType == enAutoSkillTp_valChg_rcv
                         || iType == enAutoSkillTp_valChg_atkrcv)
                {
                    int preCardRecover = m_card[i]->getRecover();
                    m_card[i]->setRecoverFactor(pAutoSkill->fEffect);
                    int curCardRecover = m_card[i]->getRecover();
                    int coverOff = curCardRecover - preCardRecover;
                    m_iBaseRecoverPt += coverOff;
                }
                else if (iType == enAutoSkillTp_valChg_all
                         || iType == enAutoSkillTp_valChg_hprcv)
                {
                    int preCardHp = m_card[i]->getBaseHp();
                    m_card[i]->setHpFactor(pAutoSkill->fEffect);
                    int curCardHp = m_card[i]->getBaseHp();
                    int hpOff = curCardHp - preCardHp;
                    m_iBaseHP += hpOff;

                    int preCardRecover = m_card[i]->getRecover();
                    m_card[i]->setRecoverFactor(pAutoSkill->fEffect);
                    int curCardRecover = m_card[i]->getRecover();
                    int coverOff = curCardRecover - preCardRecover;
                    m_iBaseRecoverPt += coverOff;
                }
            }
            else if(pAutoSkill->cGenTp == enGeneralTp_LongShen)
            {// 龙&神
                if((enGeneralTp_GuoShi == m_card[i]->getCategory())
                   || (enGeneralTp_God == m_card[i]->getCategory())
                   || (enGeneralTp_GuoShi == m_card[i]->getSubCategory())
                   || (enGeneralTp_God == m_card[i]->getSubCategory())
                   )
                {
                    int preCardHp = m_card[i]->getBaseHp();
                    m_card[i]->setHpFactor(pAutoSkill->fEffect);
                    int curCardHp = m_card[i]->getBaseHp();
                    int hpOff = curCardHp - preCardHp;
                    m_iBaseHP += hpOff;
                }
            }
        }
    }

    // enAutoSkill_386
    if (m_card[idx]->getAutoSkId() == enAutoSkill_386 || m_card[idx]->getAutoSkId() == enAutoSkill_387) {
        bool hasViceGeneral = false;
        for (int i = 1; i < 5; i++) {
            if (m_card[i]) {
                hasViceGeneral = true;
                break;
            }
        }
        if (!hasViceGeneral) {
            int preCardHp = m_card[idx]->getBaseHp();
            m_card[idx]->setHpFactor(3);
            int curCardHp = m_card[idx]->getBaseHp();
            int hpOff = curCardHp - preCardHp;
            m_iBaseHP += hpOff;
            
            int preCardRecover = m_card[idx]->getRecover();
            m_card[idx]->setRecoverFactor(3);
            int curCardRecover = m_card[idx]->getRecover();
            int coverOff = curCardRecover - preCardRecover;
            m_iBaseRecoverPt += coverOff;
        }
    }

    
    // enAutoSkill_384
    if (iSkillId == enAutoSkill_384) {
        m_iHP *= 0.6;
        m_iBaseHP *= 0.6;
    }
    
    // enAutoSkill_438,439
    if (iSkillId == enAutoSkill_438 || iSkillId == enAutoSkill_439) {
        m_iHP *= 0.5;
        m_iBaseHP *= 0.5;
    }
    
    
    //变背景音乐
    if(iSkillId == enAutoSkill_JiangNan)
    {
        int iSkillId2 = m_card[idx==0?5:0]->getAutoSkId();
        if (iSkillId2 != enAutoSkill_JiangNan)
        {
            m_iSndTag = 0;
            m_stoneGrid->setSoundTag(0);
        }
    }
    
    if (preBaseHp != m_iBaseHP)
    {
        updateBlood();
    }
}

void Player::doAwakeSkillWhileStarting()
{
    float fDelay = (float)(getNumByAwakeId(enAwakeSkill_17) * 0.5f);
    m_stoneGrid->changeMaxDurByTime(fDelay);
    
    CUserInfo*  pUsrInf = CGameData::Inst()->getUsrInfo();
    bool isSame = true;
    short countryTag = pUsrInf->user_elem+1;
    for (int i = 0; i < kMaxCardCntInBattle; i++) {
        if (m_card[i] == NULL) {
            continue;
        }
        if (countryTag != m_card[i]->getCountryPorperty()) {
            isSame = false;
            break;
        }
        
    }
    if (isSame) {
        switch (countryTag) {
            case 2:{//国家为魏国时
                if (SaveData::Inst()->getIsUseSaveData()) {
                    break;
                }
                for(int i = 0; i < kMaxCardCntInBattle; i++){
                    if(m_card[i] == NULL)
                        continue;
                     m_card[i]->updateCd(-1);
                    if(m_card[i]->getCD() > 0)
                    {
                        m_card[i]->runAction(CCMoveTo::actionWithDuration(0.3, CCPointMake(0, 0)));
                        m_card[i]->showSkillAnimation();
                    }
                }

            }
                break;
            case 3:{//国家为蜀国时
               
            }
                break;
            case 4:{//国家为吴国时
                 m_stoneGrid->changeMaxDurByTime(1.0f);
            }
                break;
                
            default:
                break;
        }
    }
    
    if (!SaveData::Inst()->getIsUseSaveData())
    {
        for(int i = 0; i < kMaxCardCntInBattle; i++)
        {
            if(m_card[i] == NULL)
                continue;
            
            m_card[i]->updateCd(getNumByAwakeId(enAwakeSkill_18)*(-1));
            
            if (m_card[i]->getCanUseNumByAwakeId(enAwakeSkill_18) > 0)
            {
                m_card[i]->runAction(CCMoveTo::actionWithDuration(0.2, CCPointMake(0, 8)));
            }
        }
        
        schedule(schedule_selector(Player::doActForAwake), 0.3);
    }
    
    
}

void Player::doActForAwake()
{
    unschedule(schedule_selector(Player::doActForAwake));
    
    for(int i = 0; i < kMaxCardCntInBattle; i++){
        if(m_card[i] == NULL)
            continue;
        
        if(m_card[i]->getCanUseNumByAwakeId(enAwakeSkill_18) > 0 && m_card[i]->getCD() > 0)
        {
            m_card[i]->runAction(CCMoveTo::actionWithDuration(0.3, CCPointMake(0, 0)));
            m_card[i]->showSkillAnimation();
        }
    }
}

//******************************************************************************
// 一场战斗开启前检测被动技能
//******************************************************************************
void Player::doAutoSkillWhileStarting()
{
    int iSkillId1 = m_card[0]->getAutoSkId();
    int iSkillId2 = m_card[5]->getAutoSkId();

    stAutoSkill* pAutoSkill1 = CSkillLib::Ins()->getAutoSkill(iSkillId1);
    stAutoSkill* pAutoSkill2 = CSkillLib::Ins()->getAutoSkill(iSkillId2);
    
    int iType1 = (pAutoSkill1)? pAutoSkill1->iType : enAutoSkillTp_null;
    int iType2 = (pAutoSkill2)? pAutoSkill2->iType : enAutoSkillTp_null;
    
    //宝石移动延时技能的处理
    float fDelay = 0.0;
    if(iType1 == enAutoSkillTp_delay)
    {
        fDelay += pAutoSkill1->fEffect;
    }
    else if (enAutoSkillIp_del_half_time_atk2V2==iType1)
    {
        fDelay -= kStoneMvDur2*0.5f;
    }
    
    if(iType2 == enAutoSkillTp_delay)
    {
        fDelay += pAutoSkill2->fEffect;
    }
    if (enAutoSkillIp_del_half_time_atk2V2==iType1
        && enAutoSkillIp_del_half_time_atk2V2==iType2)
    {
        fDelay -= kStoneMvDur2*0.25f;
    }
    else if (enAutoSkillIp_del_half_time_atk2V2==iType2)
    {
        fDelay -= kStoneMvDur2*0.5f;
    }
    
    m_stoneGrid->setAdditionalDur(fDelay);
    
    //数值改变
    if (iType1 == enAutoSkillTp_valChg_hp_Elm2)
    {   // 双属性
        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] == NULL)
                continue;
            
            if (m_card[i]->getElement() == (pAutoSkill1->cElement & 0x0F)
                || m_card[i]->getElement() == ((pAutoSkill1->cElement >> 4) & 0x0F)
                ||m_card[i]->getSubElement() == (pAutoSkill1->cElement & 0x0F)
                || m_card[i]->getSubElement() == ((pAutoSkill1->cElement >> 4) & 0x0F)
                )
            {
                m_card[i]->setHpFactor(pAutoSkill1->fEffect);
            }
        }
    }
    else if(iType1 == enAutoSkillTp_valChg_hp || iType1 == enAutoSkillTp_valChg_rcv
            || iType1 == enAutoSkillTp_valChg_atkrcv || iType1 == enAutoSkillTp_valChg_all
            || iType1 == enAutoSkillTp_valChg_hprcv || iType1 == enAutoSkillTp_valChg_atkhp)
    {   // 单属性 
        for(int i = 0; i < kMaxCardCntInBattle; i++)
        {
            if(m_card[i] == NULL)
                continue;

            if(pAutoSkill1->cGenTp == m_card[i]->getCategory()
               || pAutoSkill1->cGenTp == m_card[i]->getSubCategory()
               || pAutoSkill1->cElement == m_card[i]->getElement()
               || pAutoSkill1->cElement == m_card[i]->getSubElement()
               )
            {
                if (iType1 == enAutoSkillTp_valChg_hp
                    || iType1 == enAutoSkillTp_valChg_atkhp)
                {
                    m_card[i]->setHpFactor(pAutoSkill1->fEffect);
                }
                else if (iType1 == enAutoSkillTp_valChg_rcv
                         || iType1 == enAutoSkillTp_valChg_atkrcv)
                {
                    m_card[i]->setRecoverFactor(pAutoSkill1->fEffect);
                }
                else if (iType1 == enAutoSkillTp_valChg_all
                         || iType1 == enAutoSkillTp_valChg_hprcv)
                {
                    m_card[i]->setRecoverFactor(pAutoSkill1->fEffect);
                    m_card[i]->setHpFactor(pAutoSkill1->fEffect);
                }
            }
            else if(pAutoSkill1->cGenTp == enGeneralTp_LongShen)
            {
                if((enGeneralTp_GuoShi == m_card[i]->getCategory())
                    || (enGeneralTp_God == m_card[i]->getCategory())
                    ||(enGeneralTp_GuoShi == m_card[i]->getSubCategory())
                    || (enGeneralTp_God == m_card[i]->getSubCategory())
                   )
                {
                    m_card[i]->setHpFactor(pAutoSkill1->fEffect);
                }
            }
        }
    }
    
    if (iType2 == enAutoSkillTp_valChg_hp_Elm2)
    {   // 双属性
        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] == NULL)
                continue;
            
            if (m_card[i]->getElement() == (pAutoSkill2->cElement & 0x0F)
                || m_card[i]->getElement() == ((pAutoSkill2->cElement >> 4) & 0x0F)
                || m_card[i]->getSubElement() == (pAutoSkill2->cElement & 0x0F)
                || m_card[i]->getSubElement() == ((pAutoSkill2->cElement >> 4) & 0x0F)
                )
            {
                m_card[i]->setHpFactor(pAutoSkill2->fEffect);
            }
        }
    }
    else if(iType2 == enAutoSkillTp_valChg_hp || iType2 == enAutoSkillTp_valChg_rcv
            || iType2 == enAutoSkillTp_valChg_atkrcv || iType2 == enAutoSkillTp_valChg_all
            || iType2 == enAutoSkillTp_valChg_hprcv || iType2 == enAutoSkillTp_valChg_atkhp)
    {   // 单属性
        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] == NULL)
                continue;
            
            if(pAutoSkill2->cGenTp == m_card[i]->getCategory()
               || pAutoSkill2->cGenTp == m_card[i]->getSubCategory()
               || pAutoSkill2->cElement == m_card[i]->getElement()
               || pAutoSkill2->cElement == m_card[i]->getSubElement()
               )
            {
                if (iType2 == enAutoSkillTp_valChg_hp
                    || iType2 == enAutoSkillTp_valChg_atkhp)
                {
                    m_card[i]->setHpFactor(pAutoSkill2->fEffect);
                }
                else if (iType2 == enAutoSkillTp_valChg_rcv
                         || iType2 == enAutoSkillTp_valChg_atkrcv)
                {
                    m_card[i]->setRecoverFactor(pAutoSkill2->fEffect);
                }
                else if (iType2 == enAutoSkillTp_valChg_all
                         || iType2 == enAutoSkillTp_valChg_hprcv)
                {
                    m_card[i]->setRecoverFactor(pAutoSkill2->fEffect);
                    m_card[i]->setHpFactor(pAutoSkill2->fEffect);
                }
            }
            else if(pAutoSkill2->cGenTp == enGeneralTp_LongShen)
            {
                if((enGeneralTp_GuoShi == m_card[i]->getCategory())
                   || (enGeneralTp_God == m_card[i]->getCategory())
                   || (enGeneralTp_GuoShi == m_card[i]->getSubCategory())
                   || (enGeneralTp_God == m_card[i]->getSubCategory())
                   )
                {
                    m_card[i]->setHpFactor(pAutoSkill2->fEffect);
                }
            }
        }
    }
  
    //变背景音乐
    if((iSkillId1 == enAutoSkill_JiangNan) || (iSkillId2 == enAutoSkill_JiangNan))
    {
        m_iSndTag = enAutoSkill_JiangNan;
        m_stoneGrid->setSoundTag(enAutoSkill_JiangNan);
    }
    
    // enAutoSkill_386/enAutoSkill_387
    if((iSkillId1 == enAutoSkill_386) || (iSkillId1 == enAutoSkill_387)) {
        bool hasViceGeneral = false;
        for (int i = 1; i < 5; i++) {
            if (m_card[i]) {
                hasViceGeneral = true;
                break;
            }
        }
        if (!hasViceGeneral) {
            m_card[0]->setRecoverFactor(3);
            m_card[0]->setHpFactor(3);
        }
    }
    if((iSkillId2 == enAutoSkill_386) || (iSkillId2 == enAutoSkill_387)) {
        bool hasViceGeneral = false;
        for (int i = 1; i < 5; i++) {
            if (m_card[i]) {
                hasViceGeneral = true;
                break;
            }
        }
        if (!hasViceGeneral) {
            m_card[5]->setRecoverFactor(3);
            m_card[5]->setHpFactor(3);
        }
    }
}

//******************************************************************************
// chkAutoSkillAvail
//******************************************************************************
int Player::chkAutoSkillAvail(int skid)
{
    int iRet = skid;
    
    stAutoSkill* pAutoSkill = CSkillLib::Ins()->getAutoSkill(skid);
    
    //50%~80%几率发动,同属性武将造成的普通攻击伤害变为原本的3倍
    if((skid >= enAutoSkill_FireAtk3)
       && (skid <= enAutoSkill_DarkAtk3)){
        
        int iOdds = 50;
        
        if(m_StoneEraseCnt[pAutoSkill->cElement] > 3)
            iOdds += (m_StoneEraseCnt[pAutoSkill->cElement] - 3);
        
        iOdds = (iOdds > 80)? 80 : iOdds;
        
        int iRand = SaveData::Inst()->getrand();
        if(iRand > iOdds){
            iRet = enAutoSkill_JianJiaHuo + skid - enAutoSkill_FireAtk3;
        }
    }
    //30%~50%几率发动,同属性武将造成的普通攻击伤害变为原本的4倍
    else if((skid >= enAutoSkill_FireAtk4)
            && (skid <= enAutoSkill_DarkAtk4)){
        
        int iOdds = 30;
        
        if(m_StoneEraseCnt[pAutoSkill->cElement] > 3)
            iOdds += (m_StoneEraseCnt[pAutoSkill->cElement] - 3);
        
        iOdds = (iOdds > 50)? 50 : iOdds;
        
        int iRand = SaveData::Inst()->getrand();
        if(iRand > iOdds){
            iRet = 0;
        }
    }
    else if ((skid >= enAutoSkill_JingRuiCritHuo)
             && (skid <= enAutoSkill_JingRuiCritAn))
    {
        int iRand = SaveData::Inst()->getrand();
        if (iRand > pAutoSkill->cOdds)
        {
            if (skid == enAutoSkill_JingRuiCritHuo)
                iRet = enAutoSkill_JingRuiHuo;
            else if (skid == enAutoSkill_JingRuiCritShui)
                iRet = enAutoSkill_JingRuiShui;
            else if (skid == enAutoSkill_JingRuiCritMu)
                iRet = enAutoSkill_JingRuiMu;
            else if (skid == enAutoSkill_JingRuiCritGuang)
                iRet = enAutoSkill_JingRuiGuang;
            else if (skid == enAutoSkill_JingRuiCritAn)
                iRet = enAutoSkill_JingRuiAn;
        }
    }
    
    return iRet;
}

//******************************************************************************
// bHasAOEOfElement
//******************************************************************************
bool Player::bHasAOEOfElement(enElementDef element)
{
    for(int i = 0; i < kMaxCardCntInBattle; i++)
    {
        if((m_card[i] == NULL) || (m_card[i]->getAttackInfo().iAttackPt == 0 && m_card[i]->getSubAttackInfo().iAttackPt == 0))
            continue;
        
        if (m_card[i]->getAttackInfo().iElement == element
            && m_card[i]->getAttackInfo().iTargetCnt == 1)
            return true;
        
        if (m_card[i]->getSubAttackInfo().iElement == element
            && m_card[i]->getSubAttackInfo().iTargetCnt == 1)
            return true;
    }
    
    return false;
}

bool Player::cardHasAtkByIdx(int idx)
{
    return m_card[idx]->getAttackInfo().iAttackPt == 0 && m_card[idx]->getSubAttackInfo().iAttackPt == 0;
}

//******************************************************************************
// doOneAutoAtkSkillBeforeAtk
//******************************************************************************

void Player::doOneAutoAtkSkillBeforeAtk(int iSkillCardId, int i,
                                float &fEffect, bool &bCard0Jmp , int skillID)
{
    // 主技能
    int iSkillId1 = skillID;
    stAutoSkill* pAutoSkill1 = CSkillLib::Ins()->getAutoSkill(iSkillId1);
    int iType1 = (pAutoSkill1)? pAutoSkill1->iType : enAutoSkillTp_null;
    
    // 副技能
    stAutoSkill* pViceAutoSkill1 = NULL;
    int iViceType1 = enAutoSkillTp_null;
    if (pAutoSkill1 && pAutoSkill1->sViceSkid != enAutoSkill_NULL)
    {
        pViceAutoSkill1 = CSkillLib::Ins()->getAutoSkill(pAutoSkill1->sViceSkid);
        iViceType1 = pViceAutoSkill1->iType;
    }
    
    if (pAutoSkill1 &&
        pAutoSkill1->AppendInfo.isMustGenIDList() &&
        !beAllGoGenIDList(&pAutoSkill1->AppendInfo))
    {
    }
    else if(iType1 == enAutoSkillTp_valChg_atkrcv
            || iType1 == enAutoSkillTp_valChg_atkhp
            || iType1 == enAutoSkillTp_valChg_all)
    {
        if(m_card[i]->getSubElement() == pAutoSkill1->cElement
           
           ||m_card[i]->getElement() == pAutoSkill1->cElement//shikun
           || m_card[i]->getCategory() == pAutoSkill1->cGenTp
           || m_card[i]->getSubCategory() == pAutoSkill1->cGenTp
           )
        {
            fEffect *= pAutoSkill1->fEffect;
            
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
    }
    else if (iType1 == enAutoSkillTp_atk_aoe
             && bHasAOEOfElement((enElementDef)pAutoSkill1->cElement))
    {
        if(m_card[i]->getElement() == pAutoSkill1->cElement
           
           || m_card[i]->getSubElement() == pAutoSkill1->cElement//shikun
           || m_card[i]->getCategory() == pAutoSkill1->cGenTp
           || m_card[i]->getSubCategory() == pAutoSkill1->cGenTp
           )
        {
            fEffect *= pAutoSkill1->fEffect;
            
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
    }
    else if(iType1 == enAutoSkillTp_atk)
    {
        if(pAutoSkill1->cElement == enElementMax)
        {
            if(pAutoSkill1->cGenTp == enGeneralTp_Null)
            {
                switch (iSkillId1) {
                        //冥王的邀请
                    case enAutoSkill_GhostInvite:
                        fEffect *= pAutoSkill1->fEffect;
                        
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        break;
                        //Teddy, Zeus
                    case enAutoSkill_Teddy:
                    case enAutoSkill_Zeus:
                        if(m_iRoundStartHp == m_iBaseHP){
                            fEffect *= pAutoSkill1->fEffect;
                            
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                        
                    case enAutoSkill_Hp10Attack3:
                        if (m_iRoundStartHp <= m_iBaseHP * 0.3)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                        
                    case enAutoSkill_Hp20Attack4:
                        if (m_iRoundStartHp <= m_iBaseHP * 0.2)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                        
                    case enAutoSkill_ZeusSuper:
                        if(m_iRoundStartHp == m_iBaseHP){
                            fEffect *= pAutoSkill1->fEffect;
                            
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        else if (m_iRoundStartHp == 1)
                        {
                            fEffect *= 4;
                            
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        
                        break;
                        
                        //瘟疫离宫
                    case enAutoSkill_AtkSick:
                    case enAutoSkill_AtkSick2:
                    {
                        FightMngr* fm = (FightMngr*) m_pListener;
                        if(fm->isEnemySick()){
                            fEffect *= pAutoSkill1->fEffect;
                            
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                    }
                        break;
                    case enAutoSkill_MagicWillHPBig50Atk2_5:
                    case enAutoSkill_2MagicWillHPBig50Atk2_5:
                    {
                        if (m_iBaseHP*0.5f <= m_iRoundStartHp)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                    }
                    case enAutoSkill_HPSmall80Atk3_5:
                    case enAutoSkill_2HPSmall80Atk3_5:
                    {
                        if (m_iBaseHP*0.8f >= m_iRoundStartHp)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                    }
                    case enAutoSkill_HpSmall30Atk3:
                    {
                        if (m_iBaseHP*0.3f >= m_iRoundStartHp)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                    }
                        //wuliang
                    case enAutoSkill_375:
                        if (m_iBaseHP*0.1f>=m_iRoundStartHp)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                    case enAutoSkill_386:
                    {
                        if (iSkillCardId == i) {
                            if (m_iBaseHP*0.5f <= m_iRoundStartHp)
                            {
                                fEffect *= pAutoSkill1->fEffect;
                                
                            }
                            bool hasViceGeneral = false;
                            for (int i = 1; i < 5; i++) {
                                if (m_card[i]) {
                                    hasViceGeneral = true;
                                    break;
                                }
                            }
                            if (!hasViceGeneral) {
                                fEffect *= 3;
                            }
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                    }
                    case enAutoSkill_397:
                        if (m_iBaseHP*0.5f<=m_iRoundStartHp)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                    case enAutoSkill_414:{
                        if (m_card[i]->getCountryPorperty() == enCountryTp_shuguo) {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                       
                    }
                        break;
                    case enAutoSkill_451:
                        if(m_iRoundStartHp <= m_iBaseHP * 0.8)
                        {
                            if((enGeneralTp_GuoShi == m_card[i]->getCategory())
                               || (enGeneralTp_GuoShi == m_card[i]->getSubCategory())
                               )
                            {
                                fEffect *= pAutoSkill1->fEffect;
                                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                            }

                        }
                        break;
                    default:
                        break;
                }
                
            }
            //同武将类型攻击加成
            else if(pAutoSkill1->cGenTp == m_card[i]->getCategory()
                    || pAutoSkill1->cGenTp == m_card[i]->getSubCategory()
                    )
            {
                if (enAutoSkill_WaterXiaoYong80Atk2_20Atk2_5==iSkillId1 ||
                    enAutoSkill_2WaterXiaoYong80Atk2_20Atk2_5==iSkillId1)
                {
                    if (80>SaveData::Inst()->getrand())
                    {
                        fEffect *= 2.0;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                    else
                    {
                        fEffect *= 2.5;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if (enAutoSkill_WaterXiaoYongAtk2_20Poison1LossUseGen3AtkHP==iSkillId1 ||
                         enAutoSkill_2WaterXiaoYongAtk2_20Poison1LossUseGen3AtkHP==iSkillId1)
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    if (pAutoSkill1->cOdds>=SaveData::Inst()->getrand())
                    {
                        m_card[i]->setSpecialAtk(EN_ENEMY_POISON,1,m_card[i]->getAttack()*2.0f);
                    }
                }
                else if (enAutoSkill_WaterXiaoYongAtk2_20Freeze1LossUseGenAtkHP==iSkillId1 ||
                         enAutoSkill_2WaterXiaoYongAtk2_20Freeze1LossUseGenAtkHP==iSkillId1)
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    if (pAutoSkill1->cOdds>=SaveData::Inst()->getrand())
                    {
                        m_card[i]->setSpecialAtk(EN_ENEMY_FREEZE,1,m_card[i]->getAttack()*2.0f);
                    }
                }
                else if (enAutoSkill_352==iSkillId1)
                {
                    if (m_iRoundStartHp>=m_iBaseHP*0.8f)
                    {
                        fEffect *= pAutoSkill1->fEffect;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if (enAutoSkill_353==iSkillId1)
                {
                    if (m_iBaseHP*0.8f>=m_iRoundStartHp)
                    {
                        fEffect *= pAutoSkill1->fEffect;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if (enAutoSkill_383==iSkillId1) {
                    if (m_bIsOver6Fire)
                    {
                        fEffect *= 4;
                        // m_bIsOver6Fire = false;  AttackOver中置false
                    } else
                        fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
                else if (enAutoSkill_385==iSkillId1)
                {
                    if (m_iBaseHP*0.8f<=m_iRoundStartHp)
                    {
                        fEffect *= pAutoSkill1->fEffect;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if  (enAutoSkill_392==iSkillId1) {
                    if (m_iBaseHP*0.8f<=m_iRoundStartHp)
                    {
                        fEffect *= pAutoSkill1->fEffect;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if  (enAutoSkill_400==iSkillId1) {
                    if (m_iBaseHP*0.8f>m_iRoundStartHp)
                    {
                        fEffect *= pAutoSkill1->fEffect;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if  (enAutoSkill_404==iSkillId1) {
                    if (m_iBaseHP==m_iRoundStartHp)
                    {
                        fEffect *= pAutoSkill1->fEffect;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if  (enAutoSkill_421==iSkillId1) {
                    if (m_iBaseHP==m_iRoundStartHp)
                    {
                        fEffect *= 4.0;
                    }else
                    {
                        fEffect *= pAutoSkill1->fEffect;
                    }
                    
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
                else
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            //龙神的愤怒,咆哮
            else if(pAutoSkill1->cGenTp == enGeneralTp_LongShen)
            {
                if((enGeneralTp_GuoShi == m_card[i]->getCategory())
                   || (enGeneralTp_God == m_card[i]->getCategory())
                   || (enGeneralTp_GuoShi == m_card[i]->getSubCategory())
                   || (enGeneralTp_God == m_card[i]->getSubCategory())
                   )
                    
                {
                    
                    fEffect *= pAutoSkill1->fEffect;
                    
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
        }
        //同属性攻击加成
        else if(m_card[i]->getElement() == pAutoSkill1->cElement || m_card[i]->getSubElement() == pAutoSkill1->cElement)//shikun
        {
            if (enAutoSkill_HPFullWaterAtk3_5_4Magic2000HP==iSkillId1 ||
                enAutoSkill_2HPFullWaterAtk3_5_4Magic2000HP==iSkillId1)
            {
                if (m_iRoundStartHp>=m_iBaseHP)
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            //wuliang
            else if (enAutoSkill_376 == iSkillId1) {
                if (m_iBaseHP*0.5f>=m_iRoundStartHp)
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            //wuliang
            else if (enAutoSkill_377 == iSkillId1) {
                if (m_iBaseHP*0.05f>=m_iRoundStartHp)
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            //wuliang
            else if (iSkillId1 == enAutoSkill_378)
            {
                if(m_iHP == m_iBaseHP.get())
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            else if (enAutoSkill_389 == iSkillId1) {
                if (m_iBaseHP*0.8f<=m_iRoundStartHp)
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            else if (iSkillId1 == enAutoSkill_415)
            {
                if(m_iHP == m_iBaseHP.get())
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            else if (iSkillId1 == enAutoSkill_417)
            {
                int signal = 0;
                if(pAutoSkill1)
                {
                    for (int i = 0; i < kMaxCardCntInBattle; ++i)
                    {
                        if (m_card[i])
                        {
                            if (m_card[i]->getCardID() == 772 || m_card[i]->getCardID() == 773) {
                                if (m_card[i]->getCardPlus() == 297) {
                                    signal = 1;
                                    if (m_stoneGrid->getComboNum() >= 6) {
                                        signal = 2;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                if (signal == 2) {
                    fEffect *= 4.5;
                } else if (signal == 1) {
                    fEffect *= 4;
                } else {
                    fEffect *= pAutoSkill1->fEffect;
                }
                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
            }
            else if (enAutoSkill_418 == iSkillId1)
            {
                float eff = 1.0;
                if (  m_iRoundStartHp == m_iBaseHP)
                {
                    eff = 4.0;
                }else if (m_iRoundStartHp >= m_iBaseHP*0.5f)
                {
                    eff = 3.5;
                }
                
                fEffect *= eff;
                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
            }
            else if (iSkillId1 == enAutoSkill_424)
            {
                int signal = 0;
                if(pAutoSkill1)
                {
                    for (int i = 0; i < kMaxCardCntInBattle; ++i)
                    {
                        if (m_card[i])
                        {
                            if (m_card[i]->getCardID() == 148 || m_card[i]->getCardID() == 149 || m_card[i]->getCardID() == 935) {
                                if (m_card[i]->getCardPlus() == 297) {
                                    signal = 1;
                                 }
                            }
                         }
                    }
                }
                
                if (m_iRoundStartHp == m_iBaseHP)
                {
                    signal = 2;
                }
                
                if (signal == 1) {
                    fEffect *= 4.0;
                }else if(signal == 2)
                {
                    fEffect *= 5.0;
                }else {
                    fEffect *= pAutoSkill1->fEffect;
                }
                
                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
            }
            else if (iSkillId1 == enAutoSkill_432)
            {
                int signal = 0;
                if(pAutoSkill1)
                {
                    for (int i = 0; i < kMaxCardCntInBattle; ++i)
                    {
                        if (m_card[i])
                        {
                            //武将ID=191、192、332、333、796、995
                            if (m_card[i]->getCardID() == 191 || m_card[i]->getCardID() == 192 || m_card[i]->getCardID() == 332
                                || m_card[i]->getCardID() == 333 || m_card[i]->getCardID() == 796 || m_card[i]->getCardID() == 995
                                ) {
                                 signal = 1;
                            }
                        }
                    }
                }
                
                if (signal == 1) {
                    fEffect *= pAutoSkill1->fEffect;
                }
                
                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
            }
            else
            {
                fEffect *= pAutoSkill1->fEffect;
                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
            }
        }
        //同武将类型攻击加成
        else if(pAutoSkill1->cGenTp == m_card[i]->getCategory()
                || pAutoSkill1->cGenTp == m_card[i]->getSubCategory()
                )
        {
            fEffect *= pAutoSkill1->fEffect;
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
        /*else
         {
         if (iSkillId1 == enAutoSkill_AnAndSelfAtk25
         && i == 0)
         {
         fEffect *= pAutoSkill1->fEffect;
         
         bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
         }
         }*/
    }
    //双属性攻击提升
    else if(iType1 == enAutoSkillTp_atk2){
        if(
           
           ((pAutoSkill1->cElement & 0x0F) == m_card[i]->getElement())
           
           || (((pAutoSkill1->cElement >> 4) & 0x0F) == m_card[i]->getElement())
           
           || ((pAutoSkill1->cElement & 0x0F & 0x0F) == m_card[i]->getSubElement())//shikun
           
           || (((pAutoSkill1->cElement >> 4) & 0x0F) == m_card[i]->getSubElement())//shikun
           
           ){
            fEffect *= pAutoSkill1->fEffect;
            
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
    }
    else if (iType1 == enAutoSkillTp_atk_cmb)
    {
        //        // 根据combo数对攻击加成
        //        if (pAutoSkill1->cOdds <= m_stoneGrid->getComboNum()
        //            && (pAutoSkill1->cElement == m_card[i]->getElement()
        //                || pAutoSkill1->cElement == enElementMax))
        //        {
        //            fEffect *= pAutoSkill1->fEffect;
        //
        //            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        //        }
        if ((enGeneralTp_Null==pAutoSkill1->cGenTp ||
             pAutoSkill1->cGenTp == m_card[i]->getCategory() ||
             pAutoSkill1->cGenTp == m_card[i]->getSubCategory())&&
            (enElementMax==pAutoSkill1->cElement ||
             pAutoSkill1->cElement == m_card[i]->getElement() ||
             pAutoSkill1->cElement == m_card[i]->getSubElement())
            )
        {
            if ((enAutoSkill_Water3Combo2_6Combo7_9Combo4==iSkillId1 ||
                 enAutoSkill_2Water3Combo2_6Combo7_9Combo4==iSkillId1))
            {
                if (9<=m_stoneGrid->getComboNum())
                {
                    fEffect *= 4.0f;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
                else if (6<=m_stoneGrid->getComboNum())
                {
                    fEffect *= 3.0f;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
                else if (3<=m_stoneGrid->getComboNum())
                {
                    fEffect *= 2.0f;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            else if (enAutoSkill_391==iSkillId1) {
                if (m_stoneGrid->getComboNum() >= 3) {
                    if (m_stoneGrid->getComboNum() > 20) {
                        fEffect *= 0.5f * 20;
                    } else {
                        fEffect *= 0.5f * m_stoneGrid->getComboNum();
                    }
                }
            }
            else if (enAutoSkill_387 == iSkillId1)
            {
                if (iSkillCardId == i) {
                    // 达到4连击或以上时，自身攻击力提升3.5倍；
                    if (pAutoSkill1->cOdds <= m_stoneGrid->getComboNum()) {
                        fEffect *= pAutoSkill1->fEffect;
                    }
                    bool hasViceGeneral = false;
                    for (int i = 1; i < 5; i++) {
                        if (m_card[i]) {
                            hasViceGeneral = true;
                            break;
                        }
                    }
                    if (!hasViceGeneral) {
                        fEffect *= 3;
                    }
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            else if (enAutoSkill_430==iSkillId1) {
                if (m_stoneGrid->getComboNum() >= 4) {
                    if (m_stoneGrid->getComboNum() > 7) {
                        fEffect *= 2.5f+(7-4)*0.5f;
                    } else {
                        fEffect *= 2.5f*(m_stoneGrid->getComboNum()-4)*0.5f;
                    }
                }
            }
            else if (pAutoSkill1->cOdds <= m_stoneGrid->getComboNum())
            {
                fEffect *= pAutoSkill1->fEffect;
                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
            }
        }
    }
    else if (iType1 == enAutoSkillTp_atk_elm_cnt)
    {   // 根据攻击属性个数对攻击加成
        if (pAutoSkill1->cOdds <= getAttackElemCnt())
        {
            if (iSkillId1==enAutoSkill_Elm5Atk4) {
                if (pAutoSkill1->cOdds+1<=getAttackElemCnt())
                    fEffect *= pAutoSkill1->fEffect+1;
                else
                    fEffect *= pAutoSkill1->fEffect;
            }else
                fEffect *= pAutoSkill1->fEffect;
            
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
    }
    else if (iType1 == enAutoSkillTp_atk_elm_type)
    {   // 根据攻击属性类型对攻击加成
        if (m_bElementAttck[element3Atk25[pAutoSkill1->cOdds][0]]
            && m_bElementAttck[element3Atk25[pAutoSkill1->cOdds][1]]
            && m_bElementAttck[element3Atk25[pAutoSkill1->cOdds][2]])
        {
            fEffect *= pAutoSkill1->fEffect;
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
        
    }
    else if (enAutoSkillTp_atk_elm_type4==iType1)
    {   // 根据攻击属性类型对攻击加成
        if (m_bElementAttck[elementAtk4[pAutoSkill1->cOdds][0]] &&
            m_bElementAttck[elementAtk4[pAutoSkill1->cOdds][1]] &&
            m_bElementAttck[elementAtk4[pAutoSkill1->cOdds][2]] &&
            m_bElementAttck[elementAtk4[pAutoSkill1->cOdds][3]])
        {
            fEffect *= pAutoSkill1->fEffect;
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
    }
    else if (enAutoSkillIp_del_half_time_atk2V2==iType1)
    {
        if (m_card[i]->getElement()==(pAutoSkill1->cElement & 0x0F) ||
            m_card[i]->getSubElement()==(pAutoSkill1->cElement & 0x0F))
        {
            fEffect *= pAutoSkill1->fEffect;
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
    }
    
    // 副技能
    if (iViceType1 == enAutoSkillTp_atk
        && (m_card[i]->getElement() == pViceAutoSkill1->cElement || m_card[i]->getSubElement() == pViceAutoSkill1->cElement))//shikun
    {
        fEffect *= pViceAutoSkill1->fEffect;
        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
    }
    
}

//******************************************************************************
// doOneSubAutoAtkSkillBeforeAtk
//******************************************************************************

void Player::doOneSubAutoAtkSkillBeforeAtk(int iSkillCardId, int i,
                                   float &fEffect, bool &bCard0Jmp , int skillID)
{
    
    
    // 主技能
    int iSkillId1 = skillID;
    stAutoSkill* pAutoSkill1 = CSkillLib::Ins()->getAutoSkill(iSkillId1);
    int iType1 = (pAutoSkill1)? pAutoSkill1->iType : enAutoSkillTp_null;
    
    // 副技能
    stAutoSkill* pViceAutoSkill1 = NULL;
    int iViceType1 = enAutoSkillTp_null;
    if (pAutoSkill1 && pAutoSkill1->sViceSkid != enAutoSkill_NULL)
    {
        pViceAutoSkill1 = CSkillLib::Ins()->getAutoSkill(pAutoSkill1->sViceSkid);
        iViceType1 = pViceAutoSkill1->iType;
    }
    
    if (pAutoSkill1 &&
        pAutoSkill1->AppendInfo.isMustGenIDList() &&
        !beAllGoGenIDList(&pAutoSkill1->AppendInfo))
    {
    }
    else if(iType1 == enAutoSkillTp_valChg_atkrcv
            || iType1 == enAutoSkillTp_valChg_atkhp
            || iType1 == enAutoSkillTp_valChg_all)
    {
        if(m_card[i]->getSubElement() == pAutoSkill1->cElement
           
           ||m_card[i]->getElement() == pAutoSkill1->cElement//shikun
           || m_card[i]->getCategory() == pAutoSkill1->cGenTp
           || m_card[i]->getSubCategory() == pAutoSkill1->cGenTp
           )
        {
            fEffect *= pAutoSkill1->fEffect;
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
    }
    else if (iType1 == enAutoSkillTp_atk_aoe
             && bHasAOEOfElement((enElementDef)pAutoSkill1->cElement))
    {
        if(m_card[i]->getSubElement() == pAutoSkill1->cElement
           
           || m_card[i]->getElement() == pAutoSkill1->cElement//shikun
           
           || m_card[i]->getCategory() == pAutoSkill1->cGenTp
           || m_card[i]->getSubCategory() == pAutoSkill1->cGenTp
           )
        {
            fEffect *= pAutoSkill1->fEffect;
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
    }
    else if(iType1 == enAutoSkillTp_atk)
    {
        if(pAutoSkill1->cElement == enElementMax){
            if(pAutoSkill1->cGenTp == enGeneralTp_Null){
                switch (iSkillId1) {
                        //冥王的邀请
                    case enAutoSkill_GhostInvite:
                        fEffect *= pAutoSkill1->fEffect;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        break;
                        //Teddy, Zeus
                    case enAutoSkill_Teddy:
                    case enAutoSkill_Zeus:
                        if(m_iRoundStartHp == m_iBaseHP){
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                        
                    case enAutoSkill_Hp10Attack3:
                        if (m_iRoundStartHp <= m_iBaseHP * 0.3)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                        
                    case enAutoSkill_Hp20Attack4:
                        if (m_iRoundStartHp <= m_iBaseHP * 0.2)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                        
                    case enAutoSkill_ZeusSuper:
                        if(m_iRoundStartHp == m_iBaseHP)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        else if (m_iRoundStartHp == 1)
                        {
                            fEffect *= 4;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        
                        break;
                        
                        //瘟疫离宫
                    case enAutoSkill_AtkSick:
                    case enAutoSkill_AtkSick2:
                    {
                        FightMngr* fm = (FightMngr*) m_pListener;
                        if(fm->isEnemySick())
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                    }
                        break;
                    case enAutoSkill_MagicWillHPBig50Atk2_5:
                    case enAutoSkill_2MagicWillHPBig50Atk2_5:
                    {
                        if (m_iBaseHP*0.5f <= m_iRoundStartHp)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                    }
                    case enAutoSkill_HPSmall80Atk3_5:
                    case enAutoSkill_2HPSmall80Atk3_5:
                    {
                        if (m_iBaseHP*0.8f >= m_iRoundStartHp)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                    }
                    case enAutoSkill_HpSmall30Atk3:
                    {
                        if (m_iBaseHP*0.3f >= m_iRoundStartHp)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                    }
                        //wuliang
                    case enAutoSkill_375:
                        if (m_iBaseHP*0.1f>=m_iRoundStartHp)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                    case enAutoSkill_386:
                    {
                        if (iSkillCardId == i) {
                            if (m_iBaseHP*0.5f <= m_iRoundStartHp)
                            {
                                fEffect *= pAutoSkill1->fEffect;
                                
                            }
                            bool hasViceGeneral = false;
                            for (int i = 1; i < 5; i++) {
                                if (m_card[i]) {
                                    hasViceGeneral = true;
                                    break;
                                }
                            }
                            if (!hasViceGeneral) {
                                fEffect *= 3;
                            }
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                    }
                    case enAutoSkill_397:
                        if (m_iBaseHP*0.5f<=m_iRoundStartHp)
                        {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                        break;
                    case enAutoSkill_414:{
                        if (m_card[i]->getCountryPorperty() == 3) {
                            fEffect *= pAutoSkill1->fEffect;
                            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                        }
                    }
                    break;
                    case enAutoSkill_451:
                        if(m_iRoundStartHp <= m_iBaseHP * 0.8)
                        {
                            if((enGeneralTp_GuoShi == m_card[i]->getCategory())
                               || (enGeneralTp_GuoShi == m_card[i]->getSubCategory())
                               )
                            {
                                fEffect *= pAutoSkill1->fEffect;
                                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                            }  
                        }
                        break;
                    default:
                        break;
                }
            }
            //同武将类型攻击加成
            else if(pAutoSkill1->cGenTp == m_card[i]->getCategory()
                    || pAutoSkill1->cGenTp == m_card[i]->getSubCategory()
                    )
            {
                if (enAutoSkill_WaterXiaoYong80Atk2_20Atk2_5==iSkillId1 ||
                    enAutoSkill_2WaterXiaoYong80Atk2_20Atk2_5==iSkillId1)
                {
                    if (80>SaveData::Inst()->getrand())
                    {
                        fEffect *= 2.0;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                    else
                    {
                        fEffect *= 2.5;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if (enAutoSkill_WaterXiaoYongAtk2_20Poison1LossUseGen3AtkHP==iSkillId1 ||
                         enAutoSkill_2WaterXiaoYongAtk2_20Poison1LossUseGen3AtkHP==iSkillId1)
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    if (pAutoSkill1->cOdds>=SaveData::Inst()->getrand())
                    {
                        m_card[i]->setSubSpecialAtk(EN_ENEMY_POISON,1,m_card[i]->getAttack()*2.0f);
                    }
                }
                else if (enAutoSkill_WaterXiaoYongAtk2_20Freeze1LossUseGenAtkHP==iSkillId1 ||
                         enAutoSkill_2WaterXiaoYongAtk2_20Freeze1LossUseGenAtkHP==iSkillId1)
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    if (pAutoSkill1->cOdds>=SaveData::Inst()->getrand())
                    {
                        m_card[i]->setSubSpecialAtk(EN_ENEMY_FREEZE,1,m_card[i]->getAttack()*2.0f);
                    }
                }
                else if (enAutoSkill_352==iSkillId1)
                {
                    if (m_iRoundStartHp>=m_iBaseHP*0.8f)
                    {
                        fEffect *= pAutoSkill1->fEffect;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if (enAutoSkill_353==iSkillId1)
                {
                    if (m_iBaseHP*0.8f>=m_iRoundStartHp)
                    {
                        fEffect *= pAutoSkill1->fEffect;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if (enAutoSkill_383==iSkillId1) {
                    if (m_bIsOver6Fire)
                    {
                        fEffect *= 4;
                        // m_bIsOver6Fire = false;  AttackOver中置false（这样 主属性 和 附属性 都能用到 m_bIsOver6Fire）
                    } else
                        fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
                else if (enAutoSkill_385==iSkillId1)
                {
                    if (m_iBaseHP*0.8f<=m_iRoundStartHp)
                    {
                        fEffect *= pAutoSkill1->fEffect;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if  (enAutoSkill_392==iSkillId1) {
                    if (m_iBaseHP*0.8f<=m_iRoundStartHp)
                    {
                        fEffect *= pAutoSkill1->fEffect;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if  (enAutoSkill_400==iSkillId1) {
                    if (m_iBaseHP*0.8f>m_iRoundStartHp)
                    {
                        fEffect *= pAutoSkill1->fEffect;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if  (enAutoSkill_404==iSkillId1) {
                    if (m_iBaseHP==m_iRoundStartHp)
                    {
                        fEffect *= pAutoSkill1->fEffect;
                        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                    }
                }
                else if  (enAutoSkill_421==iSkillId1) {
                    if (m_iBaseHP==m_iRoundStartHp)
                    {
                        fEffect *= 4.0;
                    }else
                    {
                        fEffect *= pAutoSkill1->fEffect;
                    }
                    
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
                else
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            //龙神的愤怒,咆哮
            else if(pAutoSkill1->cGenTp == enGeneralTp_LongShen)
            {
                if((enGeneralTp_GuoShi == m_card[i]->getCategory())
                   || (enGeneralTp_God == m_card[i]->getCategory())
                   || (enGeneralTp_GuoShi == m_card[i]->getSubCategory())
                   || (enGeneralTp_God == m_card[i]->getSubCategory())
                   )
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
        }
        //同属性攻击加成
        else if(m_card[i]->getSubElement() == pAutoSkill1->cElement || m_card[i]->getElement() == pAutoSkill1->cElement )//shikun
        {
            if (enAutoSkill_HPFullWaterAtk3_5_4Magic2000HP==iSkillId1 ||
                enAutoSkill_2HPFullWaterAtk3_5_4Magic2000HP==iSkillId1)
            {
                if (m_iRoundStartHp>=m_iBaseHP)
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            //wuliang
            else if (enAutoSkill_376 == iSkillId1) {
                if (m_iBaseHP*0.5f>=m_iRoundStartHp)
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            //wuliang
            else if (enAutoSkill_377 == iSkillId1) {
                if (m_iBaseHP*0.05f>=m_iRoundStartHp)
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            //wuliang
            else if (iSkillId1 == enAutoSkill_378)
            {
                if(m_iHP == m_iBaseHP.get())
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            else if (enAutoSkill_389 == iSkillId1) {
                if (m_iBaseHP*0.8f<=m_iRoundStartHp)
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            else if (iSkillId1 == enAutoSkill_415)
            {
                if(m_iHP == m_iBaseHP.get())
                {
                    fEffect *= pAutoSkill1->fEffect;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            else if (iSkillId1 == enAutoSkill_417)
            {
                int signal = 0;
                if(pAutoSkill1)
                {
                    for (int i = 0; i < kMaxCardCntInBattle; ++i)
                    {
                        if (m_card[i])
                        {
                            if (m_card[i]->getCardID() == 772 || m_card[i]->getCardID() == 773) {
                                if (m_card[i]->getCardPlus() == 297) {
                                    signal = 1;
                                    if (m_stoneGrid->getComboNum() >= 6) {
                                        signal = 2;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                if (signal == 2) {
                    fEffect *= 4.5;
                } else if (signal == 1) {
                    fEffect *= 4;
                } else {
                    fEffect *= pAutoSkill1->fEffect;
                }
                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
            }
            else if (enAutoSkill_418 == iSkillId1)
            {
                float eff = 1.0;
                if (  m_iRoundStartHp == m_iBaseHP)
                {
                    eff = 4.0;
                }else if (m_iRoundStartHp >= m_iBaseHP*0.5f)
                {
                    eff = 3.5;
                }
                
                fEffect *= eff;
                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
            }
            else if (iSkillId1 == enAutoSkill_424)
            {
                int signal = 0;
                if(pAutoSkill1)
                {
                    for (int i = 0; i < kMaxCardCntInBattle; ++i)
                    {
                        if (m_card[i])
                        {
                            if (m_card[i]->getCardID() == 148 || m_card[i]->getCardID() == 149 || m_card[i]->getCardID() == 935) {
                                if (m_card[i]->getCardPlus() == 297) {
                                    signal = 1;
                                }
                            }
                        }
                    }
                }
                
                if (m_iRoundStartHp == m_iBaseHP)
                {
                    signal = 2;
                }
                
                if (signal == 1) {
                    fEffect *= 4.0;
                }else if(signal == 2)
                {
                    fEffect *= 5.0;
                }else {
                    fEffect *= pAutoSkill1->fEffect;
                }

                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
            }
            else if (iSkillId1 == enAutoSkill_432)
            {
                int signal = 0;
                if(pAutoSkill1)
                {
                    for (int i = 0; i < kMaxCardCntInBattle; ++i)
                    {
                        if (m_card[i])
                        {
                            //武将ID=191、192、332、333、796、995
                            if (m_card[i]->getCardID() == 191 || m_card[i]->getCardID() == 192 || m_card[i]->getCardID() == 332
                                || m_card[i]->getCardID() == 333 || m_card[i]->getCardID() == 796 || m_card[i]->getCardID() == 995
                                ) {
                                signal = 1;
                            }
                        }
                    }
                }
                
                if (signal == 1) {
                    fEffect *= pAutoSkill1->fEffect;
                }
                
                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
            }
            else
            {
                fEffect *= pAutoSkill1->fEffect;
                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
            }
        }
        //同武将类型攻击加成
        else if(pAutoSkill1->cGenTp == m_card[i]->getCategory()
                || pAutoSkill1->cGenTp == m_card[i]->getSubCategory()
                )
        {
            fEffect *= pAutoSkill1->fEffect;
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
        /*else
         {
         if (iSkillId1 == enAutoSkill_AnAndSelfAtk25
         && i == 0)
         {
         fEffect *= pAutoSkill1->fEffect;
         bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
         }
         }*/
    }
    //双属性攻击提升
    else if(iType1 == enAutoSkillTp_atk2)
    {
        if(((pAutoSkill1->cElement & 0x0F) == m_card[i]->getSubElement())
           
           ||((pAutoSkill1->cElement & 0x0F) == m_card[i]->getElement())//shikun
           
           
           || (((pAutoSkill1->cElement >> 4) & 0x0F) == m_card[i]->getSubElement())
           
           
           || (((pAutoSkill1->cElement >> 4) & 0x0F) == m_card[i]->getElement())//shikun
           
           )
        {
            fEffect *= pAutoSkill1->fEffect;
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
    }
    else if (iType1 == enAutoSkillTp_atk_cmb)
    {
        //        // 根据combo数对攻击加成
        //        if (pAutoSkill1->cOdds <= m_stoneGrid->getComboNum()
        //            && (pAutoSkill1->cElement == enElementMax
        //                || pAutoSkill1->cElement == m_card[i]->getSubElement()))
        //        {
        //            fEffect *= pAutoSkill1->fEffect;
        //            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        //        }
        
        if ((enGeneralTp_Null==pAutoSkill1->cGenTp ||
             pAutoSkill1->cGenTp == m_card[i]->getCategory() ||
             pAutoSkill1->cGenTp == m_card[i]->getSubCategory())&&
            (enElementMax==pAutoSkill1->cElement ||
             pAutoSkill1->cElement == m_card[i]->getElement() ||
             pAutoSkill1->cElement == m_card[i]->getSubElement())
            )
        {
            if ((enAutoSkill_Water3Combo2_6Combo7_9Combo4==iSkillId1 ||
                 enAutoSkill_2Water3Combo2_6Combo7_9Combo4==iSkillId1))
            {
                if (9<=m_stoneGrid->getComboNum())
                {
                    fEffect *= 4.0f;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
                else if (6<=m_stoneGrid->getComboNum())
                {
                    fEffect *= 3.0f;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
                else if (3<=m_stoneGrid->getComboNum())
                {
                    fEffect *= 2.0f;
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            else if (enAutoSkill_391==iSkillId1) {
                if (m_stoneGrid->getComboNum() >= 3) {
                    if (m_stoneGrid->getComboNum() > 20) {
                        fEffect *= 0.5f * 20;
                    } else {
                        fEffect *= 0.5f * m_stoneGrid->getComboNum();
                    }
                }
            }
            else if (enAutoSkill_387 == iSkillId1)
            {
                if (iSkillCardId == i) {
                    // 达到4连击或以上时，自身攻击力提升3.5倍；
                    if (pAutoSkill1->cOdds <= m_stoneGrid->getComboNum()) {
                        fEffect *= pAutoSkill1->fEffect;
                    }
                    bool hasViceGeneral = false;
                    for (int i = 1; i < 5; i++) {
                        if (m_card[i]) {
                            hasViceGeneral = true;
                            break;
                        }
                    }
                    if (!hasViceGeneral) {
                        fEffect *= 3;
                    }
                    bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
                }
            }
            else if (enAutoSkill_430==iSkillId1) {
                if (m_stoneGrid->getComboNum() >= 4) {
                    if (m_stoneGrid->getComboNum() > 7) {
                        fEffect *= 2.5f+(7-4)*0.5f;
                    } else {
                        fEffect *= 2.5f*(m_stoneGrid->getComboNum()-4)*0.5f;
                    }
                }
            }
            else if (pAutoSkill1->cOdds <= m_stoneGrid->getComboNum())
            {
                fEffect *= pAutoSkill1->fEffect;
                bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
            }
        }
        
    }
    else if (iType1 == enAutoSkillTp_atk_elm_cnt)
    {   // 根据攻击属性个数对攻击加成
        if (pAutoSkill1->cOdds <= getAttackElemCnt())
        {
            if (iSkillId1==enAutoSkill_Elm5Atk4) {
                if (pAutoSkill1->cOdds+1<=getAttackElemCnt())
                    fEffect *= pAutoSkill1->fEffect+1;
                else
                    fEffect *= pAutoSkill1->fEffect;
            }else
                fEffect *= pAutoSkill1->fEffect;
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
    }
    else if (iType1 == enAutoSkillTp_atk_elm_type)
    {   // 根据攻击属性类型对攻击加成
        if (m_bElementAttck[element3Atk25[pAutoSkill1->cOdds][0]]
            && m_bElementAttck[element3Atk25[pAutoSkill1->cOdds][1]]
            && m_bElementAttck[element3Atk25[pAutoSkill1->cOdds][2]])
        {
            fEffect *= pAutoSkill1->fEffect;
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
    }
    else if (enAutoSkillTp_atk_elm_type4==iType1)
    {
        if (m_bElementAttck[elementAtk4[pAutoSkill1->cOdds][0]] &&
            m_bElementAttck[elementAtk4[pAutoSkill1->cOdds][1]] &&
            m_bElementAttck[elementAtk4[pAutoSkill1->cOdds][2]] &&
            m_bElementAttck[elementAtk4[pAutoSkill1->cOdds][3]])
        {
            fEffect *= pAutoSkill1->fEffect;
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
    }
    else if (enAutoSkillIp_del_half_time_atk2V2==iType1)
    {
        if (m_card[i]->getSubElement()==(pAutoSkill1->cElement & 0x0F) ||
            m_card[i]->getElement()==(pAutoSkill1->cElement & 0x0F))
        {
            fEffect *= pAutoSkill1->fEffect;
            bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
        }
    }

    
    // 副技能
    if (iViceType1 == enAutoSkillTp_atk
        && (m_card[i]->getSubElement() == pViceAutoSkill1->cElement || m_card[i]->getElement() == pViceAutoSkill1->cElement))//shikun
    {
        fEffect *= pViceAutoSkill1->fEffect;
        bCard0Jmp = cardHasAtkByIdx(iSkillCardId);
    }
    
}

//攻击前检测国家计
void Player::doCountrySkillBeforeAtk(int &iRecovToal)
{
    CUserInfo*  pUsrInf = CGameData::Inst()->getUsrInfo();
    bool isSame = true;
    short countryTag = pUsrInf->user_elem+1;
    if (countryTag != enCountryTp_shuguo) {
        return;
    }
    for (int i = 0; i < kMaxCardCntInBattle; i++) {
        if (m_card[i] == NULL) {
            continue;
        }
        if (countryTag != m_card[i]->getCountryPorperty()) {
            isSame = false;
            break;
        }
    }
    if (isSame) {
        iRecovToal += 1500;
        m_iHP += 1500;
        if (m_iHP > m_iBaseHP.get()) {
            m_iHP = m_iBaseHP;
        }
    }

}


//攻击前觉醒技能检测
void Player::doAwakeSkillBeforeAtk(int &iRecovTotal)
{
    int cout = getNumByAwakeId(enAwakeSkill_15);
    stAwakeSkill* pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(enAwakeSkill_15);
    int iRecov = (int)(pAwakeSkill->fEffect * cout);
    m_iHP += iRecov;
    iRecovTotal += iRecov;
    
    cout = getNumByAwakeId(enAwakeSkill_35);
    pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(enAwakeSkill_35);
    iRecov = (int)(pAwakeSkill->fEffect * cout);
    m_iHP += iRecov;
    iRecovTotal += iRecov;
    
    if(m_iHP > m_iBaseHP.get())
        m_iHP = m_iBaseHP;
}

//******************************************************************************
// doOneAutoRcvSkillBeforeAtk
//******************************************************************************
void Player::doOneAutoRcvSkillBeforeAtk(int iSkillCardId, bool &bRet,
                                        int &iRecovTotal, bool &bCard0Jmp)
{
    // 主技能
    int iSkillId1 = chkAutoSkillAvail(m_card[iSkillCardId]->getAutoSkId());
    stAutoSkill* pAutoSkill1 = CSkillLib::Ins()->getAutoSkill(iSkillId1);
    int iType1 = (pAutoSkill1)? pAutoSkill1->iType : enAutoSkillTp_null;
    
    if(iType1 == enAutoSkillTp_recover)
    {
        if(m_iHP < m_iBaseHP.get()){
            if(!bCard0Jmp)
                m_card[iSkillCardId]->useAutoSkill(-1.0);
            
            int iRecov = m_card[iSkillCardId]->getRecover() * pAutoSkill1->fEffect;
            m_iHP += iRecov;
            iRecovTotal += iRecov;
            if(m_iHP > m_iBaseHP.get())
                m_iHP = m_iBaseHP;
            
            bRet = true;
        }
    }
    else if(iSkillId1 == enAutoSkill_GodBless){ //神佑
        if(m_iHP < m_iBaseHP.get()){
            if(!bCard0Jmp)
                m_card[iSkillCardId]->useAutoSkill(-1.0);
            
            int iRecov = m_iBaseHP * 0.1;
            m_iHP += iRecov;
            iRecovTotal += iRecov;
            if(m_iHP > m_iBaseHP.get())
                m_iHP = m_iBaseHP;
            
            bRet = true;
        }
    }
    else if(iSkillId1 == enAutoSkill_GhostInvite){ //冥王的邀请
        if(m_iHP < m_iBaseHP.get()){
            if(!bCard0Jmp)
                m_card[iSkillCardId]->useAutoSkill(-1.0);
            
            int iRecov = m_card[iSkillCardId]->getBaseHp() * 0.5;
            m_iHP += iRecov;
            iRecovTotal += iRecov;
            if(m_iHP > m_iBaseHP.get())
                m_iHP = m_iBaseHP;
            
            bRet = true;
        }
    }
    else if(iSkillId1 == enAutoSkill_Xjtp
            ||iSkillId1 == enAutoSkill_Xjtp2
            ){ //限界突破
        if(m_iHP < m_iBaseHP.get()){
            if(!bCard0Jmp)
                m_card[iSkillCardId]->useAutoSkill(-1.0);
            
            int iRecov = m_card[iSkillCardId]->getAttack();
            m_iHP += iRecov;
            iRecovTotal += iRecov;
            if(m_iHP > m_iBaseHP.get())
                m_iHP = m_iBaseHP;
            
            bRet = true;
        }
    }
    else if(iSkillId1 == enAutoSkill_418) //418 消除宝珠后,血量超回复（5倍自身回复力,同ID33）
    {
        if(m_iHP < m_iBaseHP.get()){
            if(!bCard0Jmp)
                m_card[iSkillCardId]->useAutoSkill(-1.0);
            
            int iRecov = m_card[iSkillCardId]->getBaseRecover() * pAutoSkill1->fEffect;
            m_iHP += iRecov;
            iRecovTotal += iRecov;
            if(m_iHP > m_iBaseHP.get())
                m_iHP = m_iBaseHP;
            
            bRet = true;
        }
    }
    
    //条件
    if (pAutoSkill1 && beAllGoGenIDList(&pAutoSkill1->AppendInfo))
    {
        //回复HP
        if (pAutoSkill1->AppendInfo.isReplyHp())
        {
//            if(m_iHP < m_iBaseHP.get()){
                if(!bCard0Jmp)
                {
                    m_card[iSkillCardId]->useAutoSkill(-1.0);
                }
                int iRecov = pAutoSkill1->AppendInfo.getReplyHp();
                m_iHP = MIN(m_iHP+iRecov,m_iBaseHP.get());
                iRecovTotal += iRecov;
                bRet = true;
//            }
        }
    }

}

//******************************************************************************
// doAutoSkillBeforeAttacking
//******************************************************************************
bool Player::doAutoSkillBeforeAttacking()
{
    if(m_bErased == false)
        return false;
    
    bool bRet = false;
    
    int iSkillId1 = chkAutoSkillAvail(m_card[0]->getAutoSkId());
    int iSkillId2 = chkAutoSkillAvail(m_card[5]->getAutoSkId());
    float fEffect;
    
    stAutoSkill* pAutoSkill1 = CSkillLib::Ins()->getAutoSkill(iSkillId1);
    stAutoSkill* pAutoSkill2 = CSkillLib::Ins()->getAutoSkill(iSkillId2);
    
    // 副技能
    stAutoSkill* pViceAutoSkill1 = NULL;
    int iViceType1 = enAutoSkillTp_null;
    if (pAutoSkill1 && pAutoSkill1->sViceSkid != enAutoSkill_NULL)
    {
        pViceAutoSkill1 = CSkillLib::Ins()->getAutoSkill(pAutoSkill1->sViceSkid);
        iViceType1 = pViceAutoSkill1->iType;
    }

    stAutoSkill* pViceAutoSkill2 = NULL;
    int iViceType2 = enAutoSkillTp_null;
    if (pAutoSkill2 && pAutoSkill2->sViceSkid != enAutoSkill_NULL)
    {
        pViceAutoSkill2 = CSkillLib::Ins()->getAutoSkill(pAutoSkill2->sViceSkid);
        iViceType2 = pViceAutoSkill2->iType;
    }
    
    bool bCard0Jmp = false;
    bool bCard5Jmp = false;
    
        
    
    //攻击系
    //主属性攻击判断
    for(int i = 0; i < kMaxCardCntInBattle; i++)
    {
        if((m_card[i] == NULL) || (m_card[i]->getAttackInfo().iAttackPt == 0))
            continue;
        
        fEffect = 1.0;
        
        //觉醒技能加强
        fEffect += getAwakeValueById(m_card[i]->getElement());
        
        // 主将技能
        doOneAutoAtkSkillBeforeAtk(0, i, fEffect, bCard0Jmp , iSkillId1);
        // 友军技能
        doOneAutoAtkSkillBeforeAtk(5, i, fEffect, bCard5Jmp , iSkillId2);
        
        
        
        if (m_stEnWujiangBuffGrp.iType == m_card[i]->getCategory()||m_stEnWujiangBuffGrp.iType == m_card[i]->getSubCategory()) {
            fEffect *= m_stEnWujiangBuffGrp.fFactor;
        }
        if (m_stCountryWujiangBuffGrp.iType != enCountryTp_Null  &&  m_stCountryWujiangBuffGrp.iType ==  m_card[i]->getCountryPorperty() ) {
            fEffect *= m_stCountryWujiangBuffGrp.fFactor;
        }
        
        //x属性加强  //shikun
        if (m_stEnhanceBuffGrp.iElem == m_card[i]->getElement() || m_stEnhanceBuffGrp.iElem == m_card[i]->getSubElement() ) {
            fEffect *= m_stEnhanceBuffGrp.fFactor;
        }

        bool bb = false;
        
        
        if(fEffect > 1.05)
            bb = true;
        
        // enManSkill_257    2回合中的 首回合国士无双类武将变为虚弱状态
        if (m_stSickBuffGrp.bIsManSkill_257 && m_stRcvBuffGrp.iCD == 1 && (m_card[i]->getCategory() == enGeneralTp_GuoShi || m_card[i]->getSubCategory() == enGeneralTp_GuoShi))
        {
            
            fEffect *= 0.8;
        }
        
        if(bb){
            if ((i == 0)
                && (iSkillId1 >= enAutoSkill_FireAtk3)
                && (iSkillId1 <= enAutoSkill_DarkAtk4)){
                m_card[i]->useAutoSkill(fEffect, pAutoSkill1->fEffect);
            }
            else if((i == 5)
                    && (iSkillId2 >= enAutoSkill_FireAtk3)
                    && (iSkillId2 <= enAutoSkill_DarkAtk4)){
                m_card[i]->useAutoSkill(fEffect, pAutoSkill2->fEffect);
            }
            else
            {
                m_card[i]->useAutoSkill(fEffect);
            }
            bRet = true;
        }
        
        if(bCard0Jmp){
            if((iSkillId1 >= enAutoSkill_FireAtk3)
               && (iSkillId1 <= enAutoSkill_DarkAtk4)){
                m_card[0]->useAutoSkill(-3.0, pAutoSkill1->fEffect);
            }
            else{
                m_card[0]->useAutoSkill(-3.0);
            }
        }
        
        if(bCard5Jmp){
            if((iSkillId2 >= enAutoSkill_FireAtk3)
               && (iSkillId2 <= enAutoSkill_DarkAtk4)){
                m_card[5]->useAutoSkill(-3.0, pAutoSkill2->fEffect);
            }
            else{
                m_card[5]->useAutoSkill(-3.0);
            }
        }
    }
    
    //副属性攻击判断
    for(int i = 0; i < kMaxCardCntInBattle; i++)
    {
        if((m_card[i] == NULL) || (m_card[i]->getSubAttackInfo().iAttackPt == 0))
            continue;
        
        fEffect = 1.0;
        
        //觉醒技能加强
        fEffect += getAwakeValueById(m_card[i]->getSubElement());
        // 主将技能
        doOneSubAutoAtkSkillBeforeAtk(0, i, fEffect, bCard0Jmp , iSkillId1);
        // 友军技能
        doOneSubAutoAtkSkillBeforeAtk(5, i, fEffect, bCard5Jmp, iSkillId2);
        
        
        if (m_stEnWujiangBuffGrp.iType == m_card[i]->getCategory()||m_stEnWujiangBuffGrp.iType == m_card[i]->getSubCategory()) {
            fEffect *= m_stEnWujiangBuffGrp.fFactor;
        }
        if (m_stCountryWujiangBuffGrp.iType == m_card[i]->getCountryPorperty()) {
            fEffect *= m_stCountryWujiangBuffGrp.fFactor;
        }
        //x属性加强
        if (m_stEnhanceBuffGrp.iElem == m_card[i]->getSubElement() || m_stEnhanceBuffGrp.iElem == m_card[i]->getElement() )//shikun
        {
            fEffect *= m_stEnhanceBuffGrp.fFactor;
        }
        
        bool bb = false;
        

        if(fEffect > 1.05)
            bb = true;
        
        
        // enManSkill_257    2回合中的 首回合国士无双类武将变为虚弱状态
        if (m_stSickBuffGrp.bIsManSkill_257 && m_stRcvBuffGrp.iCD == 1 && (m_card[i]->getCategory() == enGeneralTp_GuoShi || m_card[i]->getSubCategory() == enGeneralTp_GuoShi))
            fEffect *= 0.8;
        
        if(fEffect > 1.05)
        {
            m_card[i]->subUseAutoSkill(fEffect);
            bRet = true;
        }
                
        if(bb){
            if ((i == 0)
                && (iSkillId1 >= enAutoSkill_FireAtk4)
                && (iSkillId1 <= enAutoSkill_DarkAtk4)){
                m_card[i]->drawSkillX3(fEffect, 4);
            }
            else if((i == 5)
                    && (iSkillId2 >= enAutoSkill_FireAtk4)
                    && (iSkillId2 <= enAutoSkill_DarkAtk4)){
                m_card[i]->drawSkillX3(fEffect, 4);
            }
            else
            {
                m_card[i]->drawSkillX3(fEffect);
            }
            bRet = true;
        }
        
        if(bCard0Jmp){
            if((iSkillId1 >= enAutoSkill_FireAtk4)
               && (iSkillId1 <= enAutoSkill_DarkAtk4)){
                m_card[0]->drawSkillX3(-4.0, 4);
            }
            else{
                m_card[0]->drawSkillX3(-4.0);
            }
        }
        
        if(bCard5Jmp){
            if((iSkillId2 >= enAutoSkill_FireAtk4)
               && (iSkillId2 <= enAutoSkill_DarkAtk4)){
                m_card[5]->drawSkillX3(-4.0, 4);
            }
            else{
                m_card[5]->drawSkillX3(-4.0);
            }
        }
        
        if(bb){
            if ((i == 0)
                && (iSkillId1 >= enAutoSkill_FireAtk3)
                && (iSkillId1 <= enAutoSkill_DarkAtk3)){
                m_card[i]->drawSkillX3(fEffect, 3);
            }
            else if((i == 5)
                    && (iSkillId2 >= enAutoSkill_FireAtk3)
                    && (iSkillId2 <= enAutoSkill_DarkAtk3)){
                m_card[i]->drawSkillX3(fEffect, 3);
            }
            else
            {
                m_card[i]->drawSkillX3(fEffect);
            }
            bRet = true;
        }
        
        if(bCard0Jmp){
            if((iSkillId1 >= enAutoSkill_FireAtk3)
               && (iSkillId1 <= enAutoSkill_DarkAtk3)){
                m_card[0]->drawSkillX3(-3.0, 3);
            }
            else{
                m_card[0]->drawSkillX3(-3.0);
            }
        }
        
        if(bCard5Jmp){
            if((iSkillId2 >= enAutoSkill_FireAtk3)
               && (iSkillId2 <= enAutoSkill_DarkAtk3)){
                m_card[5]->drawSkillX3(-3.0, 3);
            }
            else{
                m_card[5]->drawSkillX3(-3.0);
            }
        }
    }

    //加血系
    int iRecovTotal = 0;
    // 主将技能
    doOneAutoRcvSkillBeforeAtk(0, bRet, iRecovTotal, bCard0Jmp);
    // 友军技能
    doOneAutoRcvSkillBeforeAtk(5, bRet, iRecovTotal, bCard5Jmp);
    
    doAwakeSkillBeforeAtk(iRecovTotal);
    
    doCountrySkillBeforeAtk(iRecovTotal);

    if(iRecovTotal > 0)
    {
        m_iRecoverPt += iRecovTotal;
        bRet = true;
        updateBlood();
        m_bloodNum->setIsVisible(true);
        m_bloodNum->setNumber(m_iRecoverPt, 0.2, 0.0);
    }
    
    return bRet;
}

//******************************************************************************
// doAutoSkillAfterAttacking
//******************************************************************************
bool Player::doAutoSkillAfterAttacking()
{
    if(!m_bErased)
        return false;
    
    bool bRet = false;
    
    int iSkillId1 = m_card[0]->getAutoSkId();
    int iSkillId2 = m_card[5]->getAutoSkId();
    
    stAutoSkill* pAutoSkill1 = CSkillLib::Ins()->getAutoSkill(iSkillId1);
    stAutoSkill* pAutoSkill2 = CSkillLib::Ins()->getAutoSkill(iSkillId2);
    
    int iType1 = (pAutoSkill1)? pAutoSkill1->iType : enAutoSkillTp_null;
    int iType2 = (pAutoSkill2)? pAutoSkill2->iType : enAutoSkillTp_null;
    
    m_AtkInfo.clear();
    
//业炎，狱炎，无尽狱炎
    if(iType1 == enAutoSkillTp_burn){
        m_AtkInfo.iAutoSkillID = iSkillId1;
        m_AtkInfo.iManSkillID = kInvalidValue;
        m_AtkInfo.bAtkOvr = false;
        m_AtkInfo.iTargetCnt = 1;
        m_AtkInfo.iElement = pAutoSkill1->cElement; //避免属性相克
        if (m_AtkInfo.iElement == enElementGod) // 被动技能279，每回合1000的神圣伤害追打
            m_AtkInfo.iAttackPt = pAutoSkill1->fEffect;
        else
            m_AtkInfo.iAttackPt = m_card[0]->getAttack() * pAutoSkill1->fEffect;
        m_card[0]->useAutoSkill(-1.0);
        
        if (pAutoSkill1->cElement != pAutoSkill2->cElement) {
            (m_pListener->*m_pfnSelector)(&m_AtkInfo);
            
            m_AtkInfo.clear();
        }
        
        bRet = true;
    }
    
    if(iType2 == enAutoSkillTp_burn){
        m_AtkInfo.iAutoSkillID = iSkillId2;
        m_AtkInfo.iManSkillID = kInvalidValue;
        m_AtkInfo.bAtkOvr = false;
        m_AtkInfo.iTargetCnt = 1;
        m_AtkInfo.iElement = pAutoSkill2->cElement; //避免属性相克
        if (m_AtkInfo.iElement == enElementGod) // 被动技能279，每回合1000的神圣伤害追打
            m_AtkInfo.iAttackPt += pAutoSkill2->fEffect;
        else
            m_AtkInfo.iAttackPt += m_card[5]->getAttack() * pAutoSkill2->fEffect;
        m_card[5]->useAutoSkill(-1.0);
        
        bRet = true;
    }
    
    if(iType2 == enAutoSkillTp_burn
       || (iType1 == enAutoSkillTp_burn
           && pAutoSkill1->cElement == pAutoSkill2->cElement)){
        (m_pListener->*m_pfnSelector)(&m_AtkInfo);
    }
    
    
//弱点系列
    int elemOppositeTbl[enElementMax] = {
        enElementHeart,
        enElementFire,
        enElementWood,
        enElementWater,
        enElementDark,
        enElementLight
    };

    if((iType1 == enAutoSkillTp_elemburn) || (iType2 == enAutoSkillTp_elemburn)){
        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] == NULL)
                continue;
            
            m_AtkInfo.clear();
            
            if(iType1 == enAutoSkillTp_elemburn){
                FightMngr* fm = (FightMngr*) m_pListener;
                if(fm->haveEnemyWithElem(elemOppositeTbl[pAutoSkill1->cElement])){
                    if(m_card[i]->getElement() == pAutoSkill1->cElement){
                        m_AtkInfo.iAutoSkillID = iSkillId1;
                        m_AtkInfo.iManSkillID = kInvalidValue;
                        m_AtkInfo.bAtkOvr = false;
                        m_AtkInfo.iTargetCnt = 0;
                        m_AtkInfo.iElement = pAutoSkill1->cElement;
                        m_AtkInfo.iAttackPt += m_card[i]->getAttack() * pAutoSkill1->fEffect;
                        m_AtkInfo.srcPt = CCPointMake(55 + 106*i, 615);
                        m_card[i]->useAutoSkill(-1.0);
                        
                        bRet = true;
                    }
                }
            }
            
            if(iType2 == enAutoSkillTp_elemburn){
                FightMngr* fm = (FightMngr*) m_pListener;
                if(fm->haveEnemyWithElem(elemOppositeTbl[pAutoSkill2->cElement])){
                    if(m_card[i]->getElement() == pAutoSkill2->cElement){
                        m_AtkInfo.iAutoSkillID = iSkillId2;
                        m_AtkInfo.iManSkillID = kInvalidValue;
                        m_AtkInfo.bAtkOvr = false;
                        m_AtkInfo.iTargetCnt = 0;
                        m_AtkInfo.iElement = pAutoSkill2->cElement;
                        m_AtkInfo.iAttackPt += m_card[i]->getAttack() * pAutoSkill2->fEffect;
                        m_AtkInfo.srcPt = CCPointMake(55 + 106*i, 615);
                        m_card[i]->useAutoSkill(-1.0);
                        
                        bRet = true;
                    }
                }
            }
            
            if(m_AtkInfo.iAttackPt > 0)
                (m_pListener->*m_pfnSelector)(&m_AtkInfo);
        }
    }
    
//欺压
    bool bQiYa = false;
    m_AtkInfo.clear();
    if(iSkillId1 == enAutoSkill_QiYa
       ||iSkillId1 == enAutoSkill_QiYa2
       ){
        m_AtkInfo.iAutoSkillID = iSkillId1;
        m_AtkInfo.iManSkillID = kInvalidValue;
        m_AtkInfo.bAtkOvr = false;
        m_AtkInfo.iTargetCnt = 0;
        m_AtkInfo.iElement = pAutoSkill1->cElement; //避免属性相克
        m_AtkInfo.iAttackPt = m_card[0]->getAttack() * pAutoSkill1->fEffect;
        m_card[0]->useAutoSkill(-1.0);
        
        bQiYa = true;
    }
    
    if(iSkillId2 == enAutoSkill_QiYa
       ||iSkillId2 == enAutoSkill_QiYa2
       ){
        m_AtkInfo.iAutoSkillID = iSkillId2;
        m_AtkInfo.iManSkillID = kInvalidValue;
        m_AtkInfo.bAtkOvr = false;
        m_AtkInfo.iTargetCnt = 0;
        m_AtkInfo.iElement = pAutoSkill2->cElement; //避免属性相克
        m_AtkInfo.iAttackPt += m_card[5]->getAttack() * pAutoSkill2->fEffect;
        m_card[5]->useAutoSkill(-1.0);
        
        bQiYa = true;
    }
    
    if(bQiYa){
        (m_pListener->*m_pfnSelector)(&m_AtkInfo);
        bRet = true;
    }
    
    return bRet;
}

//******************************************************************************
// doAutoSkillBeforeBeingAttacked
//******************************************************************************
void Player::doAutoSkillBeforeBeingAttacked(CAttackInfo* atkinfo)
{
    float fHpPercent = (int)m_iHP * 1.0 / (int)m_iBaseHP;
    
    
    
//    CCLog("%d , %d , %f" , (int)m_iHP , (int)m_iBaseHP , fHpPercent );
    
    int iSkillId1 = m_card[0]->getAutoSkId();
    int iSkillId2 = m_card[5]->getAutoSkId();

    stAutoSkill* pAutoSkill1 = CSkillLib::Ins()->getAutoSkill(iSkillId1);
    stAutoSkill* pAutoSkill2 = CSkillLib::Ins()->getAutoSkill(iSkillId2);
    
    
//伤害减免
    if(pAutoSkill1){
        switch (pAutoSkill1->iType) {
            //单属性防
            case enAutoSkillTp_armor_attr:
                if(pAutoSkill1->cElement == atkinfo->iElement){
                    if (atkinfo->iManSkillID != enEnemySkill_528) {
                        atkinfo->iAttackPt *= pAutoSkill1->fEffect;
                        m_card[0]->useAutoSkill(-1.0);
                    }
//                    atkinfo->iAttackPt *= pAutoSkill1->fEffect;
//                    m_card[0]->useAutoSkill(-1.0);
                }
                break;
                
            //双属性防(相同效果)
            case enAutoSkillTp_armor_attr2:
                if(((pAutoSkill1->cElement & 0x0F) == atkinfo->iElement)
                   || (((pAutoSkill1->cElement >> 4) & 0x0F) == atkinfo->iElement)){
                    if (atkinfo->iManSkillID != enEnemySkill_528) {
                        atkinfo->iAttackPt *= pAutoSkill1->fEffect;
                        m_card[0]->useAutoSkill(-1.0);
                    }
//                    atkinfo->iAttackPt *= pAutoSkill1->fEffect;
//                    m_card[0]->useAutoSkill(-1.0);
                }
                break;
                
            //双属性防(不同效果)
            case enAutoSkillTp_armor_attr2V2:
                if ((pAutoSkill1->cElement & 0x0F) == atkinfo->iElement)
                {
                    if (atkinfo->iManSkillID != enEnemySkill_528){
                        atkinfo->iAttackPt *= pAutoSkill1->fEffect - int(pAutoSkill1->fEffect);
                        m_card[0]->useAutoSkill(-1.0);
                    }
//                    atkinfo->iAttackPt *= pAutoSkill1->fEffect - int(pAutoSkill1->fEffect);
//                    m_card[0]->useAutoSkill(-1.0);
                }
                else if (((pAutoSkill1->cElement >> 4) & 0x0F) == atkinfo->iElement)
                {
                    if (atkinfo->iManSkillID != enEnemySkill_528){
                        atkinfo->iAttackPt *= int(pAutoSkill1->fEffect) / 100.0;
                        m_card[0]->useAutoSkill(-1.0);
                    }
//                    atkinfo->iAttackPt *= int(pAutoSkill1->fEffect) / 100.0;
//                    m_card[0]->useAutoSkill(-1.0);
                }
                break;
                
            //3属性防
            case enAutoSkillTp_armor_attr3:
                if((atkinfo->iElement == enElementWater)
                   ||(atkinfo->iElement == enElementFire)
                   ||(atkinfo->iElement == enElementWood)){
                     if (atkinfo->iManSkillID != enEnemySkill_528)
                     {
                         atkinfo->iAttackPt *= pAutoSkill1->fEffect;
                         m_card[0]->useAutoSkill(-1.0);
                     }
//                    atkinfo->iAttackPt *= pAutoSkill1->fEffect;
//                    m_card[0]->useAutoSkill(-1.0);
                }
                break;
                
            //全属性防
            case enAutoSkillTp_armor_all:
                if (iSkillId1 == enAutoSkill_ManHpArmor)
                {
                    if(m_iHP == m_iBaseHP.get())
                    {
                        if (atkinfo->iManSkillID != enEnemySkill_528)
                        {
                            atkinfo->iAttackPt *= pAutoSkill1->fEffect;
                            m_card[0]->useAutoSkill(-1.0);
                        }
//                        atkinfo->iAttackPt *= pAutoSkill1->fEffect;
//                        m_card[0]->useAutoSkill(-1.0);
                    }
                }
                else if (iSkillId1 == enAutoSkill_453)
                {
                    if(m_iHP == m_iBaseHP.get())
                    {
                        if (atkinfo->iManSkillID != enEnemySkill_528)
                        {
                            atkinfo->iAttackPt *= pAutoSkill1->fEffect;
                            m_card[0]->useAutoSkill(-1.0);
                        }
                    }
                }
                else if (iSkillId1 == enAutoSkill_20HpArmor80)
                {
                    if (m_iHP < m_iBaseHP * 0.2)
                    {
                         if (atkinfo->iManSkillID != enEnemySkill_528)
                         {
                             atkinfo->iAttackPt *= pAutoSkill1->fEffect;
                             m_card[0]->useAutoSkill(-1.0);

                         }
//                        atkinfo->iAttackPt *= pAutoSkill1->fEffect;
//                        m_card[0]->useAutoSkill(-1.0);
                    }
                }
                //wuliang
                else if (iSkillId1 == enAutoSkill_380)
                {
                    if(m_iHP == m_iBaseHP.get())
                    {
                        if (atkinfo->iManSkillID != enEnemySkill_528)
                        {
                            atkinfo->iAttackPt *= pAutoSkill1->fEffect;
                            m_card[0]->useAutoSkill(-1.0);
                        }
//                        atkinfo->iAttackPt *= pAutoSkill1->fEffect;
//                        m_card[0]->useAutoSkill(-1.0);
                    }
                }
                else
                {
                     if (atkinfo->iManSkillID != enEnemySkill_528)
                     {
                         atkinfo->iAttackPt *= pAutoSkill1->fEffect;
                         m_card[0]->useAutoSkill(-1.0);
                     }
//                    atkinfo->iAttackPt *= pAutoSkill1->fEffect;
//                    m_card[0]->useAutoSkill(-1.0);
                }
                break;
                
            default:
                break;
        }
    }
    
    if(pAutoSkill2){
        switch (pAutoSkill2->iType) {
                //单属性防
            case enAutoSkillTp_armor_attr:
                if(pAutoSkill2->cElement == atkinfo->iElement){
                     if (atkinfo->iManSkillID != enEnemySkill_528)
                     {
                         atkinfo->iAttackPt *= pAutoSkill2->fEffect;
                         m_card[5]->useAutoSkill(-1.0);
                     }
//                    atkinfo->iAttackPt *= pAutoSkill2->fEffect;
//                    m_card[5]->useAutoSkill(-1.0);
                }
                break;
                
                //双属性防(相同效果)
            case enAutoSkillTp_armor_attr2:
                if(((pAutoSkill2->cElement & 0x0F) == atkinfo->iElement)
                   || (((pAutoSkill2->cElement >> 4) & 0x0F) == atkinfo->iElement)){
                    if (atkinfo->iManSkillID != enEnemySkill_528)
                    {
                        atkinfo->iAttackPt *= pAutoSkill2->fEffect;
                        m_card[5]->useAutoSkill(-1.0);
                    }
//                    atkinfo->iAttackPt *= pAutoSkill2->fEffect;
//                    m_card[5]->useAutoSkill(-1.0);
                }
                break;
                
                //双属性防(不同效果)
            case enAutoSkillTp_armor_attr2V2:
                if ((pAutoSkill2->cElement & 0x0F) == atkinfo->iElement)
                {
                    if (atkinfo->iManSkillID != enEnemySkill_528)
                    {
                        atkinfo->iAttackPt *= pAutoSkill2->fEffect - int(pAutoSkill2->fEffect);
                        m_card[5]->useAutoSkill(-1.0);
                    }
//                    atkinfo->iAttackPt *= pAutoSkill2->fEffect - int(pAutoSkill2->fEffect);
//                    m_card[5]->useAutoSkill(-1.0);
                }
                else if (((pAutoSkill2->cElement >> 4) & 0x0F) == atkinfo->iElement)
                {
                    if (atkinfo->iManSkillID != enEnemySkill_528)
                    {
                        atkinfo->iAttackPt *= int(pAutoSkill2->fEffect) / 100.0;
                        m_card[5]->useAutoSkill(-1.0);
                    }
//                    atkinfo->iAttackPt *= int(pAutoSkill2->fEffect) / 100.0;
//                    m_card[5]->useAutoSkill(-1.0);
                }
                break;
                
                //3属性防
            case enAutoSkillTp_armor_attr3:
                if((atkinfo->iElement == enElementWater)
                   ||(atkinfo->iElement == enElementFire)
                   ||(atkinfo->iElement == enElementWood)){
                    if (atkinfo->iManSkillID != enEnemySkill_528)
                    {
                        atkinfo->iAttackPt *= pAutoSkill2->fEffect;
                        m_card[5]->useAutoSkill(-1.0);
                    }
//                    atkinfo->iAttackPt *= pAutoSkill2->fEffect;
//                    m_card[5]->useAutoSkill(-1.0);
                }
                break;
                
                //全属性防
            case enAutoSkillTp_armor_all:
                if (iSkillId2 == enAutoSkill_ManHpArmor)
                {
                    if(m_iHP == m_iBaseHP.get())
                    {
                        if (atkinfo->iManSkillID != enEnemySkill_528)
                        {
                            atkinfo->iAttackPt *= pAutoSkill2->fEffect;
                            m_card[5]->useAutoSkill(-1.0);

                        }
//                        atkinfo->iAttackPt *= pAutoSkill2->fEffect;
//                        m_card[5]->useAutoSkill(-1.0);
                    }
                }
                else if (iSkillId2 == enAutoSkill_20HpArmor80)
                {
                    if (m_iHP < m_iBaseHP * 0.2)
                    {
                           if (atkinfo->iManSkillID != enEnemySkill_528)
                           {
                               atkinfo->iAttackPt *= pAutoSkill2->fEffect;
                               m_card[5]->useAutoSkill(-1.0);

                           }
//                        atkinfo->iAttackPt *= pAutoSkill2->fEffect;
//                        m_card[5]->useAutoSkill(-1.0);
                    }
                }
                else if (iSkillId2 == enAutoSkill_453)
                {
                    if(m_iHP == m_iBaseHP.get())
                    {
                        if (atkinfo->iManSkillID != enEnemySkill_528)
                        {
                            atkinfo->iAttackPt *= pAutoSkill2->fEffect;
                            m_card[5]->useAutoSkill(-1.0);
                        }
                    }
                }
                //wuliang
                else if (iSkillId2 == enAutoSkill_380)
                {
                    if(m_iHP == m_iBaseHP.get())
                    {
                        if (atkinfo->iManSkillID != enEnemySkill_528)
                        {
                            atkinfo->iAttackPt *= pAutoSkill2->fEffect;
                            m_card[0]->useAutoSkill(-1.0);
                        }
//                        atkinfo->iAttackPt *= pAutoSkill2->fEffect;
//                        m_card[0]->useAutoSkill(-1.0);
                    }
                }
                else
                {
                    if (atkinfo->iManSkillID != enEnemySkill_528)
                    {
                        atkinfo->iAttackPt *= pAutoSkill2->fEffect;
                        m_card[5]->useAutoSkill(-1.0);
                    }
//                    atkinfo->iAttackPt *= pAutoSkill2->fEffect;
//                    m_card[5]->useAutoSkill(-1.0);
                }

                break;
                
            default:
                break;
        }
    }
    
    if (atkinfo->iAttackPt == 0)
        atkinfo->iAttackPt = 1;
    
//免死
    m_iHP -= atkinfo->iAttackPt;
    
    if(m_iHP <= 0){
        if(m_stFanJiBuffGrp.iCD > 0 && m_stFanJiBuffGrp.bMianSiFanJi){  //绝地反击优先
            m_iHP = 1;
        }
        else{
            if((pAutoSkill1) && (pAutoSkill1->iType == enAutoSkillTp_revive)){
                
//                CCLog("%f , %f" , fHpPercent ,pAutoSkill1->fEffect );
                
                if(fHpPercent > pAutoSkill1->fEffect){
                    m_iHP = 1;
                    m_card[0]->useAutoSkill(-1.0);
                }
            }
            
            if((pAutoSkill2) && (pAutoSkill2->iType == enAutoSkillTp_revive)){
                
//                CCLog("%f , %f" , fHpPercent ,pAutoSkill1->fEffect );
                
                if(fHpPercent > pAutoSkill2->fEffect){
                    m_iHP = 1;
                    m_card[5]->useAutoSkill(-1.0);
                }
            }
        }
    }
}



//******************************************************************************
// doAwakeSkillBeforeBeingAttacked
//******************************************************************************
void Player::doAwakeSkillBeforeBeingAttacked(CAttackInfo* pAtkinfo)
{
    if (0<pAtkinfo->iAttackPt)
    {
        if (stAwakeSkill *pAwakeSkillList = CSkillLib2::Ins()->getAwakeSkill(0))
        {
            unsigned short ushSize = CSkillLib2::Ins()->getNowAwakeSkillSum();
            for (int i = 0; i < ushSize; ++i)
            {
                stAwakeSkill *pAwakeSkill = &pAwakeSkillList[i];
                if (!(100<=pAwakeSkill->cOdds || SaveData::Inst()->getrand()<=pAwakeSkill->cOdds))
                {
                    continue;
                }
                unsigned short ushSkillCount = getNumByAwakeId(i);
                if (1>ushSkillCount)
                {
                    continue;
                }
                
                switch (pAwakeSkill->iType)
                {
                    case enAwakeSkillTp_jianshang:
                    {
                        if (pAtkinfo->iElement==pAwakeSkill->cElement
                            ||enElementMax==pAwakeSkill->cElement)
                        {
//                            float fOddsSum = StoneGrid::getOddsAwake10(pAwakeSkill->fEffect,getNumByAwakeId(i));
                            float fOddsSum = pAwakeSkill->fEffect * ushSkillCount;
                            if (0.0f<fOddsSum)
                            {
                                pAtkinfo->iAttackPt *= (1.0f-fOddsSum);
//                                useAwakeSkill();
                            }
                        }
                        break;
                    }
                    default:
                    {
                        break;

                    }
                }
            }
        }
    }
}

//******************************************************************************
// doAutoSkillAfterEnemyAtkOvr
//******************************************************************************
void Player::doAutoSkillAfterEnemyAtkOvr()
{
    int iSkillId1 = m_card[0]->getAutoSkId();
    int iSkillId2 = m_card[5]->getAutoSkId();
    
    stAutoSkill* pAutoSkill1 = CSkillLib::Ins()->getAutoSkill(iSkillId1);
    stAutoSkill* pAutoSkill2 = CSkillLib::Ins()->getAutoSkill(iSkillId2);
    
//判断反击是否触发
    if((pAutoSkill1) && (pAutoSkill1->iType == enAutoSkillTp_fanji)){
        if(SaveData::Inst()->getrand() > pAutoSkill1->cOdds){
            iSkillId1 = 0;
            pAutoSkill1 = NULL;
        }
    }
    else{
        iSkillId1 = 0;
        pAutoSkill1 = NULL;
    }
    
    if((pAutoSkill2) && (pAutoSkill2->iType == enAutoSkillTp_fanji)){
        if(SaveData::Inst()->getrand() > pAutoSkill2->cOdds){
            iSkillId2 = 0;
            pAutoSkill2 = NULL;
        }
    }
    else{
        iSkillId2 = 0;
        pAutoSkill2 = NULL;
    }
    
    if ((iSkillId1 == 0) && (iSkillId2 == 0))
        return;
    
//触发反击的情况
    for (int i = 0; i < m_EnemyAtkArr->count(); i++) {
        CAttackInfo* pAtkInf = m_EnemyAtkArr->getObjectAtIndex(i);
        
        m_AtkInfo.bAtkOvr = false;
        m_AtkInfo.iTargetCnt = 0;
        m_AtkInfo.iManSkillID = kInvalidValue;
        m_AtkInfo.iEnemyIdx = pAtkInf->iEnemyIdx;

        if((pAutoSkill1 && pAutoSkill2)
           &&(pAutoSkill1->cElement == pAutoSkill2->cElement)){
            m_AtkInfo.iElement = pAutoSkill1->cElement;
            m_AtkInfo.iAutoSkillID = iSkillId1;
            m_AtkInfo.iAttackPt = pAtkInf->iAttackPt * pAutoSkill1->fEffect
                                + pAtkInf->iAttackPt * pAutoSkill2->fEffect;
            
            m_card[0]->useAutoSkill(-1.0);
            m_card[5]->useAutoSkill(-1.0);
            (m_pListener->*m_pfnSelector)(&m_AtkInfo);
        }
        else{
            if(pAutoSkill1){
                m_AtkInfo.iElement = pAutoSkill1->cElement;
                m_AtkInfo.iAutoSkillID = iSkillId1;
                m_AtkInfo.iAttackPt = pAtkInf->iAttackPt * pAutoSkill1->fEffect;
                
                m_card[0]->useAutoSkill(-1.0);
                (m_pListener->*m_pfnSelector)(&m_AtkInfo);
            }
            
            if(pAutoSkill2){
                m_AtkInfo.iElement = pAutoSkill2->cElement;
                m_AtkInfo.iAutoSkillID = iSkillId2;
                m_AtkInfo.iAttackPt = pAtkInf->iAttackPt * pAutoSkill2->fEffect;
                
                m_card[5]->useAutoSkill(-1.0);
                (m_pListener->*m_pfnSelector)(&m_AtkInfo);
            }
        }
    }
}

//******************************************************************************
// revive
//******************************************************************************
void Player::revive()
{
    RecordControl(false, 0);
    
//    m_iHP = m_iBaseHP;
//    updateBlood();
    
    for(int i = 0; i < kMaxCardCntInBattle; i++)
    {
        if(m_card[i] == NULL)
            continue;
     
        m_card[i]->setCardState(EN_CARD_STATE_NORMAL);
    }
    
    m_iHP = m_iBaseHP;
    updateBlood();
}

//******************************************************************************
// reviveByOption
//******************************************************************************
void Player::reviveByOption(int order)
{
    RecordControl(false, 0);

    for(int i = 0; i < kMaxCardCntInBattle; i++)
    {
        if(m_card[i] == NULL)
            continue;
        
        m_card[i]->setCardState(EN_CARD_STATE_NORMAL);
    }

    if (order == 0) {
        m_iHP = m_iBaseHP *1.0/100;
    }else if(order == 1)
    {
        m_iHP = m_iBaseHP;
    }else if(order == 2)
    {
        m_iHP = m_iBaseHP;
        for(int i = 0; i < kMaxCardCntInBattle; i++)
        {
            if(m_card[i] == NULL)
                continue;
            
            while(m_card[i]->getCD() > 0 && m_card[i]->getCD() != kInvalidValue)
            {
                m_card[i]->decCD();
                updateSkillHighlight();
            }
        }

    }
    
    updateBlood();
}

//******************************************************************************
// getPlayerTip
//******************************************************************************
const char* Player::getPlayerTip() {
    
    
    if(m_iNewbieStep == 5 && m_iNewbieMsgIdx == 3 && m_tempBoolNew)
    {
        return CGameData::Inst()->getLanguageValue("stoneGridTip1");
    }
    else
    return CGameData::Inst()->getLanguageValue(gStrMsg[m_iNewbieMsgIdx]);
}

//******************************************************************************
// getPlayerTip
//******************************************************************************
void Player::saveCurPlayerInfo() {
    SaveData::Inst()->setPlayerCurHp(m_iHP);
    SaveData::Inst()->setPlayerBaseHp(m_iBaseHP);
    SaveData::Inst()->setPlayerCanfei(m_bCanFei);
    
    SaveData::Inst()->setPlayerEnhanceCD(m_stEnhanceBuffGrp.iCD);
    SaveData::Inst()->setPlayerEnhanceElem(m_stEnhanceBuffGrp.iElem);
    SaveData::Inst()->setPlayerEnhanceFactor(m_stEnhanceBuffGrp.fFactor);
    SaveData::Inst()->setPlayerEnWujiangCD(m_stEnWujiangBuffGrp.iCD);
    SaveData::Inst()->setPlayerEnWujiangType(m_stEnWujiangBuffGrp.iType);
    SaveData::Inst()->setPlayerEnWujiangFactor(m_stEnWujiangBuffGrp.fFactor);
    SaveData::Inst()->setManSkill_129(m_bIsManSkill_129);   //
    
    SaveData::Inst()->setPlayerCountryWujiangCD(m_stCountryWujiangBuffGrp.iCD);
    SaveData::Inst()->setPlayerCountryWujiangType(m_stCountryWujiangBuffGrp.iType);
    SaveData::Inst()->setPlayerCountryWujiangFactor(m_stCountryWujiangBuffGrp.fFactor);
    
    for (int i = 0; i < 6; i++) {
        if (!m_card[i]) {
            continue;
        }
        SaveData::Inst()->setSkillCD(i, m_card[i]->getCD());
        SaveData::Inst()->setState(i, m_card[i]->getState());
        SaveData::Inst()->setFengYinCD(i, m_card[i]->getFengYinCD());
    }
}

//******************************************************************************
// decBuffCD
//******************************************************************************
void Player::decBuffCD()
{
    //加防冷卻遞减
    if(m_stArmorBuffGrp.iCD > 0) {
        if (!SaveData::Inst()->getIsUseSaveData()) {
            m_stArmorBuffGrp.iCD--;
            SaveData::Inst()->setSheildCd(m_stArmorBuffGrp.iCD);
            if (m_stArmorBuffGrp.iCD == 0) {
                SaveData::Inst()->setArmorEffect(1.0);
            }
        }
    }
    
    if(m_stArmorBuffGrp.iCD > 0){
        char buf[10];
        sprintf(buf, "%d", m_stArmorBuffGrp.iCD);
        m_stArmorBuffGrp.lb->setString(buf);
    }
    else{
        removeBuff(m_stArmorBuffGrp.sp);
        
        if(m_stArmorBuffGrp.sp){
            if(m_stArmorBuffGrp.lb){
                m_stArmorBuffGrp.sp->removeChild(m_stArmorBuffGrp.lb, true);
                m_stArmorBuffGrp.lb = NULL;
            }
            
            removeChild(m_stArmorBuffGrp.sp, true);
            m_stArmorBuffGrp.sp = NULL;
        }
    }
    
    //移除无敌buff
    if(m_stWudiBuffGrp.iCD > 0)
    {
        if (!SaveData::Inst()->getIsUseSaveData())
        {
            if (m_stWudiBuffGrp.enType != enWudiType_Hurt)
            {   // 如果不是按伤害无敌
                m_stWudiBuffGrp.iCD--;
                SaveData::Inst()->setWudiCd(m_stWudiBuffGrp.iCD);
            }
        }
    }
    
    if(m_stWudiBuffGrp.iCD > 0){
        if (m_stWudiBuffGrp.lb)
        {
            char buf[100];
            sprintf(buf, "%d", m_stWudiBuffGrp.iCD);
            m_stWudiBuffGrp.lb->setString(buf);
        }
    }
    else{
        removeBuff(m_stWudiBuffGrp.sp);
        
        if(m_stWudiBuffGrp.sp){
            if(m_stWudiBuffGrp.lb){
                m_stWudiBuffGrp.sp->removeChild(m_stWudiBuffGrp.lb, true);
                m_stWudiBuffGrp.lb = NULL;
            }
            
            removeChild(m_stWudiBuffGrp.sp, true);
            m_stWudiBuffGrp.sp = NULL;
        }
    }
    
    if(m_stRcvBuffGrp.iCD > 0){
        if (m_stSickBuffGrp.bIsManSkill_257) {  // 257 轮回·不灭          2回合内，血量完全回复；首回合国士无双类武将变为虚弱状态
            if (!SaveData::Inst()->getIsUseSaveData())
                m_stRcvBuffGrp.iCD--;
            if(m_stRcvBuffGrp.iCD == 0) {
                if (m_stSickBuffGrp.sp) {
                    removeBuff(m_stSickBuffGrp.sp);
                    
                    removeChild(m_stSickBuffGrp.sp, true);
                    m_stSickBuffGrp.sp = NULL;
                }
                m_stSickBuffGrp.bIsManSkill_257 = false;
            }
            
            SaveData::Inst()->setRecoverCd(m_stRcvBuffGrp.iCD);
            //-- updateBlood
            m_iHP = m_iBaseHP;
            updateRoundStartHp();
            updateBlood();
            //--
            
            if (m_stRcvBuffGrp.iCD == 0) {
                removeBuff(m_stRcvBuffGrp.sp);
                
                if(m_stRcvBuffGrp.sp){
                    if(m_stRcvBuffGrp.lb){
                        m_stRcvBuffGrp.sp->removeChild(m_stRcvBuffGrp.lb, true);
                        m_stRcvBuffGrp.lb = NULL;
                    }
                    
                    removeChild(m_stRcvBuffGrp.sp, true);
                    m_stRcvBuffGrp.sp = NULL;
                }
            }
            else if (m_stRcvBuffGrp.lb) {
                char buf[100];
                snprintf(buf, 99, "%d", m_stRcvBuffGrp.iCD);
                m_stRcvBuffGrp.lb->setString(buf);
            }
        } else {    //如沐春风
            if (!SaveData::Inst()->getIsUseSaveData())
                m_stRcvBuffGrp.iCD--;
            
            SaveData::Inst()->setRecoverCd(m_stRcvBuffGrp.iCD);
            
            if (m_stRcvBuffGrp.iCD == 0) {
                removeBuff(m_stRcvBuffGrp.sp);
                
                if(m_stRcvBuffGrp.sp){
                    if(m_stRcvBuffGrp.lb){
                        m_stRcvBuffGrp.sp->removeChild(m_stRcvBuffGrp.lb, true);
                        m_stRcvBuffGrp.lb = NULL;
                    }
                    
                    removeChild(m_stRcvBuffGrp.sp, true);
                    m_stRcvBuffGrp.sp = NULL;
                }
               SaveData::Inst()->setRecoverEffect(0);
            }
            else if (m_stRcvBuffGrp.lb) {
                char buf[100];
                snprintf(buf, 99, "%d", m_stRcvBuffGrp.iCD);
                m_stRcvBuffGrp.lb->setString(buf);
            }
          
        }
    }

    if (m_stRcvWuJiangBuffGrp.iCD > 0) {//武将回复力提升
        if (!SaveData::Inst()->getIsUseSaveData())
            m_stRcvWuJiangBuffGrp.iCD--;
        
        SaveData::Inst()->setRecoverWuJiangCd(m_stRcvWuJiangBuffGrp.iCD);
        
        if (m_stRcvWuJiangBuffGrp.iCD == 0) {
            removeBuff(m_stRcvWuJiangBuffGrp.sp);
            
            if(m_stRcvWuJiangBuffGrp.sp){
                if(m_stRcvWuJiangBuffGrp.lb){
                    m_stRcvWuJiangBuffGrp.sp->removeChild(m_stRcvWuJiangBuffGrp.lb, true);
                    m_stRcvWuJiangBuffGrp.lb = NULL;
                }
                
                removeChild(m_stRcvWuJiangBuffGrp.sp, true);
                m_stRcvWuJiangBuffGrp.sp = NULL;
            }
            m_iBaseRecoverPt /= m_stRcvWuJiangBuffGrp.fFactor;
            for (int i = 0; i < kMaxCardCntInBattle; i++) {
                if (m_card[i] == NULL) {
                    continue;
                }
                m_card[i]->setRecoverFactor(1/m_stRcvWuJiangBuffGrp.fFactor);
            }
        }
        else if (m_stRcvWuJiangBuffGrp.lb) {
            char buf[100];
            snprintf(buf, 99, "%d", m_stRcvWuJiangBuffGrp.iCD);
            m_stRcvWuJiangBuffGrp.lb->setString(buf);
        }
        

    }
    //绝地反击
    if (m_stFanJiBuffGrp.iCD > 0)
    {
        if (!SaveData::Inst()->getIsUseSaveData())
            m_stFanJiBuffGrp.iCD--;
        SaveData::Inst()->setFanjiCd(m_stFanJiBuffGrp.iCD);
        
        if(m_stFanJiBuffGrp.iCD == 0){
            removeBuff(m_stFanJiBuffGrp.sp);
            m_stFanJiBuffGrp.bMianSiFanJi = false;
            SaveData::Inst()->setIsFanjiMiansi(m_stFanJiBuffGrp.bMianSiFanJi);
            
            if(m_stFanJiBuffGrp.sp){
                if(m_stFanJiBuffGrp.lb){
                    m_stFanJiBuffGrp.sp->removeChild(m_stFanJiBuffGrp.lb, true);
                    m_stFanJiBuffGrp.lb = NULL;
                }
                
                removeChild(m_stFanJiBuffGrp.sp, true);
                m_stFanJiBuffGrp.sp = NULL;
            }
        }
        else
        {
            if (m_stFanJiBuffGrp.lb)
            {
                char buf[32];
                snprintf(buf, 31, "%d", m_stFanJiBuffGrp.iCD);
                m_stFanJiBuffGrp.lb->setString(buf);
            }
        }
    }
    
    //AOE
    if (m_stAOEBuffGrp.iCD > 0)
    {
        if (!SaveData::Inst()->getIsUseSaveData())
            m_stAOEBuffGrp.iCD--;
        
        SaveData::Inst()->setAOECd(m_stAOEBuffGrp.iCD);
        
        if (m_stAOEBuffGrp.iCD == 0)
        {
            removeBuff(m_stAOEBuffGrp.sp);
            
            if (m_stAOEBuffGrp.sp)
            {
                if (m_stAOEBuffGrp.lb)
                {
                    m_stAOEBuffGrp.sp->removeChild(m_stAOEBuffGrp.lb, true);
                    m_stAOEBuffGrp.lb = NULL;
                }
                
                removeChild(m_stAOEBuffGrp.sp, true);
                m_stAOEBuffGrp.sp = NULL;
            }
        }
        else
        {
            if (m_stAOEBuffGrp.lb)
            {
                char buf[32];
                snprintf(buf, 31, "%d", m_stAOEBuffGrp.iCD);
                m_stAOEBuffGrp.lb->setString(buf);
            }
        }
    }
    
    //x属性加强
    if(m_stEnhanceBuffGrp.iCD > 0){
        if (!SaveData::Inst()->getIsUseSaveData())
        {
            m_stEnhanceBuffGrp.iCD--;
        }

        if (m_stEnhanceBuffGrp.iCD == 0) {
            removeBuff(m_stEnhanceBuffGrp.sp);
            
            m_stEnhanceBuffGrp.iElem = enElementMax;
            
            if(m_stEnhanceBuffGrp.sp){
                if(m_stEnhanceBuffGrp.lb){
                    m_stEnhanceBuffGrp.sp->removeChild(m_stEnhanceBuffGrp.lb, true);
                    m_stEnhanceBuffGrp.lb = NULL;
                }
                
                removeChild(m_stEnhanceBuffGrp.sp, true);
                m_stEnhanceBuffGrp.sp = NULL;
            }
        }
        else if (m_stEnhanceBuffGrp.lb) {
            char buf[100];
            snprintf(buf, 99, "%d", m_stEnhanceBuffGrp.iCD);
            m_stEnhanceBuffGrp.lb->setString(buf);
        }
    }
    
    // 武将技能255
    if (m_stEnhanceH6BuffGrp.iCD > 0)
    {
        if (!SaveData::Inst()->getIsUseSaveData())
        {
            m_stEnhanceH6BuffGrp.iCD--;
        }
        
        if (m_stEnhanceH6BuffGrp.iCD == 0)
        {
            m_stEnhanceH6BuffGrp.iElem = enElementInvalid;
            m_stEnhanceH6BuffGrp.fFactor = 0.0;
            
            if (m_stEnhanceH6BuffGrp.sp) {
                removeBuff(m_stEnhanceH6BuffGrp.sp);
                
                if(m_stEnhanceH6BuffGrp.lb){
                    m_stEnhanceH6BuffGrp.sp->removeChild(m_stEnhanceH6BuffGrp.lb, true);
                    m_stEnhanceH6BuffGrp.lb = NULL;
                }
                
                removeChild(m_stEnhanceH6BuffGrp.sp, true);
                m_stEnhanceH6BuffGrp.sp = NULL;
            }
        }
    }
    
    //武将攻击增强
    if(m_stEnWujiangBuffGrp.iCD > 0){
        if (!SaveData::Inst()->getIsUseSaveData())
        {
            m_stEnWujiangBuffGrp.iCD--;
        }
        
        // enManSkill_SklCdDwn 129号武将技 比较特殊，此处特殊处理
        if (m_bIsManSkill_129 && SaveData::Inst()->getIsUseSaveData()) {
            m_stEnWujiangBuffGrp.iCD++;
        }
        
        if (m_bIsManSkill_129 && m_stEnWujiangBuffGrp.iCD == 1) {
            m_stEnWujiangBuffGrp.fFactor = 0.8;
            addSickBuff();
        }
        else if (m_bIsManSkill_129 && m_stEnWujiangBuffGrp.iCD == 0) {
            if (m_stSickBuffGrp.sp) {
                removeBuff(m_stSickBuffGrp.sp);
                
                removeChild(m_stSickBuffGrp.sp, true);
                m_stSickBuffGrp.sp = NULL;
            }
            m_bIsManSkill_129 = false;
        }
        
        
        
        if (m_stEnWujiangBuffGrp.iCD == 0) {
            removeBuff(m_stEnWujiangBuffGrp.sp);
            
            m_stEnWujiangBuffGrp.iType = enGeneralTp_Null;
            
            if(m_stEnWujiangBuffGrp.sp){
                if(m_stEnWujiangBuffGrp.lb){
                    m_stEnWujiangBuffGrp.sp->removeChild(m_stEnWujiangBuffGrp.lb, true);
                    m_stEnWujiangBuffGrp.lb = NULL;
                }
                
                removeChild(m_stEnWujiangBuffGrp.sp, true);
                m_stEnWujiangBuffGrp.sp = NULL;
            }
        }
        else if (m_stEnWujiangBuffGrp.lb) {
            char buf[100];
            snprintf(buf, 99, "%d", m_stEnWujiangBuffGrp.iCD);
            m_stEnWujiangBuffGrp.lb->setString(buf);
        }
    }
    
    if (m_stCountryWujiangBuffGrp.iCD > 0) {
        if (!SaveData::Inst()->getIsUseSaveData())
        {
            m_stCountryWujiangBuffGrp.iCD--;
        }
        
        if (m_stCountryWujiangBuffGrp.iCD == 0) {
            m_stCountryWujiangBuffGrp.iType = enCountryTp_Invalid;
            removeBuff(m_stCountryWujiangBuffGrp.sp);
            if (m_stCountryWujiangBuffGrp.sp) {
                if (m_stCountryWujiangBuffGrp.lb) {
                    removeChild(m_stCountryWujiangBuffGrp.lb, true);
                    m_stCountryWujiangBuffGrp.lb = NULL;
                }
                removeChild(m_stCountryWujiangBuffGrp.sp, true);
                m_stCountryWujiangBuffGrp.sp = NULL;
            }
            
        }
        else if (m_stCountryWujiangBuffGrp.lb)
        {
            char buf[100];
            snprintf(buf, 99, "%d",m_stCountryWujiangBuffGrp.iCD);
            m_stCountryWujiangBuffGrp.lb->setString(buf);
        }
    }
    
}

//******************************************************************************
// updateRoundStartHp
//******************************************************************************
void Player::updateRoundStartHp()
{
    m_iRoundStartHp = m_iHP;
}

//******************************************************************************
// addBlood
//******************************************************************************
void Player::addBlood(int iBlood)
{
    m_iHP += iBlood;
    
    updateRoundStartHp();
    
    updateBlood();
}

void Player::setBlood(int nHp,int BaseHp)
{
    m_iHP = nHp;
    m_iBaseHP = BaseHp;
    updateRoundStartHp();
    updateBlood();
}

//******************************************************************************
// yanChiSkillCD
//******************************************************************************
void Player::yanChiSkillCD(bool bYanChi)
{
    if (bYanChi == m_bSkillCDYanChi)
        return;
 
    m_bSkillCDYanChi = bYanChi;
 
    for (int i = 0; i < kMaxCardCntInBattle; i++) {
        if (!m_card[i])
            continue;
        
        if (SaveData::Inst()->getIsUseSaveData())
            m_card[i]->yanChiFromSaveData();
        else
            m_card[i]->yanChiSkillCD(bYanChi);
    }
    
    updateSkillHighlight();
}

//******************************************************************************
// enemyAtkOvr
//******************************************************************************
void Player::enemyAtkOvr()
{
    if(m_stFanJiBuffGrp.iCD > 0){
        //敌人没攻击的情况
        if(m_iHP >= m_iRoundEndHp)
            return;
        
        if (m_stFanJiBuffGrp.bMianSiFanJi)
        {   // 太史慈的绝地反击特殊处理
            m_AtkInfo.bAtkOvr = false;
            m_AtkInfo.iTargetCnt = 0;
            m_AtkInfo.iManSkillID = kInvalidValue;
            m_AtkInfo.iEnemyIdx = kInvalidValue;
            m_AtkInfo.iElement = m_stFanJiBuffGrp.iElem;
            m_AtkInfo.iAutoSkillID = kInvalidValue;
            m_AtkInfo.iAttackPt = (m_iRoundEndHp - m_iHP) * m_stFanJiBuffGrp.fFactor;
            m_AtkInfo.srcPt = CCPointMake(55 + 106*m_stFanJiBuffGrp.iJueDiCardIdx, 615);
            
            (m_pListener->*m_pfnSelector)(&m_AtkInfo);
        }
        else
        {
            for (int i = 0; i < m_EnemyAtkArr->count(); i++) {
                CAttackInfo* pAtkInf = m_EnemyAtkArr->getObjectAtIndex(i);
                
                m_AtkInfo.bAtkOvr = false;
                m_AtkInfo.iTargetCnt = 3; //保证反击的目标不被手动瞄准修改
                m_AtkInfo.iManSkillID = kInvalidValue;
                m_AtkInfo.iEnemyIdx = pAtkInf->iEnemyIdx;
                m_AtkInfo.iElement = m_stFanJiBuffGrp.iElem;
                m_AtkInfo.iAutoSkillID = kInvalidValue;
                m_AtkInfo.iAttackPt = pAtkInf->iAttackPt * m_stFanJiBuffGrp.fFactor;
                m_AtkInfo.srcPt = CCPointMake(55 + 106*m_stFanJiBuffGrp.iJueDiCardIdx, 615);
                
                (m_pListener->*m_pfnSelector)(&m_AtkInfo);
            }
        }
    }
    
    doAutoSkillAfterEnemyAtkOvr();
}

//******************************************************************************
// showHpDownNum
//******************************************************************************
void Player::showHpDownNum(int iAtkPt)
{
    char buf[100];
    sprintf(buf, "/%d", iAtkPt);

    CCSprite *sp = CCSprite::spriteWithFile("fire_number.png");
    CCLabelAtlas *lbb = CCLabelAtlas::labelWithString("0", "fire_number.png",sp->getContentSize().width / 11, sp->getContentSize().height, '/');
    lbb->setPosition(m_blood->getPosition());
    lbb->setAnchorPoint(CCPointMake(0.5, 0.5));
    lbb->setString(buf);
    addChild(lbb,11,kTagToBeDel);
    if(CGameData::Inst()->isNewbie())
        lbb->setScale(2.0);
    lbb->runAction(CCSpawn::actions(CCMoveBy::actionWithDuration(1.0, CCPointMake(0, 100)),
                                    CCFadeOut::actionWithDuration(1.0),
                                    NULL));

}

//******************************************************************************
// addArmorBuff
//******************************************************************************
void Player::addArmorBuff()
{
    char buf[100];
    
    if(!m_stArmorBuffGrp.sp)
        m_stArmorBuffGrp.sp = CCSprite::spriteWithSpriteFrameName("sheild.png");
    
    if(m_stArmorBuffGrp.sp){
        addChild(m_stArmorBuffGrp.sp);
        addBuff(m_stArmorBuffGrp.sp);
    }
    
    if(!m_stArmorBuffGrp.lb)
        m_stArmorBuffGrp.lb = TextNode::textWithString("0", 20);
    
    if((m_stArmorBuffGrp.lb) && (m_stArmorBuffGrp.sp)){
        m_stArmorBuffGrp.lb->setAnchorPoint(CCPointZero);
        m_stArmorBuffGrp.lb->setPosition(CCPointMake(m_stArmorBuffGrp.sp->getContentSize().width, 0));
        m_stArmorBuffGrp.sp->addChild(m_stArmorBuffGrp.lb);
        
        sprintf(buf, "%d", m_stArmorBuffGrp.iCD);
        m_stArmorBuffGrp.lb->setString(buf);
    }
}

//******************************************************************************
// addWudiBuff
//******************************************************************************
void Player::addWudiBuff()
{
    char buf[100];
    
    sprintf(buf, "invincible%d.png", m_stWudiBuffGrp.iElem == enElementMax ?m_stWudiBuffGrp.iElem-2:m_stWudiBuffGrp.iElem);
    if(!m_stWudiBuffGrp.sp)
        m_stWudiBuffGrp.sp = CCSprite::spriteWithSpriteFrameName(buf);
    
    if(m_stWudiBuffGrp.sp){
        addChild(m_stWudiBuffGrp.sp);
        addBuff(m_stWudiBuffGrp.sp);
    }
    
    if(!m_stWudiBuffGrp.lb)
        m_stWudiBuffGrp.lb = TextNode::textWithString("0", 20);
    
    if(m_stWudiBuffGrp.sp && m_stWudiBuffGrp.lb){
        m_stWudiBuffGrp.lb->setAnchorPoint(CCPointZero);
        m_stWudiBuffGrp.lb->setPosition(CCPointMake(m_stWudiBuffGrp.sp->getContentSize().width, 0));
        m_stWudiBuffGrp.sp->addChild(m_stWudiBuffGrp.lb);
        
        sprintf(buf, "%d", m_stWudiBuffGrp.iCD);
        m_stWudiBuffGrp.lb->setString(buf);
    }
}

//******************************************************************************
// addRcvBuff
//******************************************************************************
void Player::addRcvBuff()
{
    char buf[100];
    
    if(!m_stRcvBuffGrp.sp)
        m_stRcvBuffGrp.sp = CCSprite::spriteWithSpriteFrameName("icon_recover.png");
    
    if(m_stRcvBuffGrp.sp){
        addChild(m_stRcvBuffGrp.sp);
        addBuff(m_stRcvBuffGrp.sp);
    }
    
    if(!m_stRcvBuffGrp.lb)
        m_stRcvBuffGrp.lb = TextNode::textWithString("0", 20);
    
    if((m_stRcvBuffGrp.lb) && (m_stRcvBuffGrp.sp)){
        m_stRcvBuffGrp.lb->setAnchorPoint(CCPointZero);
        m_stRcvBuffGrp.lb->setPosition(CCPointMake(m_stRcvBuffGrp.sp->getContentSize().width, 0));
        m_stRcvBuffGrp.sp->addChild(m_stRcvBuffGrp.lb);
        
        sprintf(buf, "%d", m_stRcvBuffGrp.iCD);
        m_stRcvBuffGrp.lb->setString(buf);
    }
}

//******************************************************************************
// addRcvWuJiangBuff
//******************************************************************************
void Player::addRcvWuJiangBuff()
{
    char buf[100];
    
    if(!m_stRcvWuJiangBuffGrp.sp)
        m_stRcvWuJiangBuffGrp.sp = CCSprite::spriteWithSpriteFrameName("icon_recover.png");
    
    if(m_stRcvWuJiangBuffGrp.sp){
        addChild(m_stRcvWuJiangBuffGrp.sp);
        addBuff(m_stRcvWuJiangBuffGrp.sp);
    }
    
    if(!m_stRcvWuJiangBuffGrp.lb)
        m_stRcvWuJiangBuffGrp.lb = TextNode::textWithString("0", 20);
    
    if((m_stRcvWuJiangBuffGrp.lb) && (m_stRcvWuJiangBuffGrp.sp)){
        m_stRcvWuJiangBuffGrp.lb->setAnchorPoint(CCPointZero);
        m_stRcvWuJiangBuffGrp.lb->setPosition(CCPointMake(m_stRcvWuJiangBuffGrp.sp->getContentSize().width, 0));
        m_stRcvWuJiangBuffGrp.sp->addChild(m_stRcvWuJiangBuffGrp.lb);
        
        sprintf(buf, "%d", m_stRcvWuJiangBuffGrp.iCD);
        m_stRcvWuJiangBuffGrp.lb->setString(buf);
    }

}




//******************************************************************************
// addAOEBuff
//******************************************************************************
void Player::addAOEBuff()
{
    if(!m_stAOEBuffGrp.sp)
        m_stAOEBuffGrp.sp = CCSprite::spriteWithSpriteFrameName("icon_aoe.png");
    
    if(m_stAOEBuffGrp.sp){
        addChild(m_stAOEBuffGrp.sp);
        addBuff(m_stAOEBuffGrp.sp);
    }
    
    if(!m_stAOEBuffGrp.lb)
    {
        char buf[32];
        snprintf(buf, 31, "%d", m_stAOEBuffGrp.iCD);
        m_stAOEBuffGrp.lb = TextNode::textWithString(buf, 20);
    }
    
    if((m_stAOEBuffGrp.lb) && (m_stAOEBuffGrp.sp)){
        m_stAOEBuffGrp.lb->setAnchorPoint(CCPointZero);
        m_stAOEBuffGrp.lb->setPosition(CCPointMake(m_stAOEBuffGrp.sp->getContentSize().width, 0));
        m_stAOEBuffGrp.sp->addChild(m_stAOEBuffGrp.lb);
    }
}

//******************************************************************************
// addJdfjBuff
//******************************************************************************
void Player::addJdfjBuff()
{
    if(!m_stFanJiBuffGrp.sp)
        m_stFanJiBuffGrp.sp = CCSprite::spriteWithSpriteFrameName("icon_juedifanji.png");
    
    if(m_stFanJiBuffGrp.sp){
        addChild(m_stFanJiBuffGrp.sp);
        addBuff(m_stFanJiBuffGrp.sp);
    }
    
    if(!m_stFanJiBuffGrp.lb)
    {
        char buf[32];
        snprintf(buf, 31, "%d", m_stFanJiBuffGrp.iCD);
        m_stFanJiBuffGrp.lb = TextNode::textWithString(buf, 20);
    }
    
    if((m_stFanJiBuffGrp.lb) && (m_stFanJiBuffGrp.sp)){
        m_stFanJiBuffGrp.lb->setAnchorPoint(CCPointZero);
        m_stFanJiBuffGrp.lb->setPosition(CCPointMake(m_stFanJiBuffGrp.sp->getContentSize().width, 0));
        m_stFanJiBuffGrp.sp->addChild(m_stFanJiBuffGrp.lb);
    }
}

//******************************************************************************
// addEnhanceBuff
//******************************************************************************
void Player::addEnhanceBuff()
{
    char buf[20];
    
    if(m_stEnhanceBuffGrp.sp == NULL){
        sprintf(buf, "stone%ds.png", m_stEnhanceBuffGrp.iElem);
        m_stEnhanceBuffGrp.sp = CCSprite::spriteWithSpriteFrameName(buf);
    }
    
    if(m_stEnhanceBuffGrp.sp){
        addChild(m_stEnhanceBuffGrp.sp);
        addBuff(m_stEnhanceBuffGrp.sp);
    }
    
    if(m_stEnhanceBuffGrp.lb == NULL)
        m_stEnhanceBuffGrp.lb = TextNode::textWithString("0", 20);
    
    if((m_stEnhanceBuffGrp.lb) && (m_stEnhanceBuffGrp.sp)){
        m_stEnhanceBuffGrp.lb->setAnchorPoint(CCPointZero);
        m_stEnhanceBuffGrp.lb->setPosition(CCPointMake(m_stEnhanceBuffGrp.sp->getContentSize().width, 0));
        m_stEnhanceBuffGrp.sp->addChild(m_stEnhanceBuffGrp.lb);
        
        sprintf(buf, "%d", m_stEnhanceBuffGrp.iCD);
        m_stEnhanceBuffGrp.lb->setString(buf);
    }
}

//
void Player::addCountryWujiangBuff()
{
     char buf[50];
    if (m_stCountryWujiangBuffGrp.sp == NULL) {
        switch (m_stCountryWujiangBuffGrp.iType) {
            case enCountryTp_dahan:
                m_stCountryWujiangBuffGrp.sp = CCSprite::spriteWithFile("hanchao.png");
                break;
            case enCountryTp_weiguo:
                m_stCountryWujiangBuffGrp.sp = CCSprite::spriteWithFile("weiguo.png");
                break;
            case enCountryTp_shuguo:
                m_stCountryWujiangBuffGrp.sp = CCSprite::spriteWithFile("shuguo.png");
                break;
            case enCountryTp_wuguo:
                m_stCountryWujiangBuffGrp.sp = CCSprite::spriteWithFile("wuguo.png");
                break;
            case enCountryTp_qunying:
                m_stCountryWujiangBuffGrp.sp = CCSprite::spriteWithFile("qunying.png");
                break;
            case enCountryTp_jindai:
                m_stCountryWujiangBuffGrp.sp = CCSprite::spriteWithFile("jindai.png");
                break;
            default:
                break;
        }
    }
    if (m_stCountryWujiangBuffGrp.sp) {
        m_stCountryWujiangBuffGrp.sp->setScale(0.7);
        addChild(m_stCountryWujiangBuffGrp.sp);
        addBuff(m_stCountryWujiangBuffGrp.sp);
    }
    if(m_stCountryWujiangBuffGrp.lb == NULL)
        m_stCountryWujiangBuffGrp.lb = TextNode::textWithString("0", 30);
    
    if((m_stCountryWujiangBuffGrp.lb) && (m_stCountryWujiangBuffGrp.sp)){
        m_stCountryWujiangBuffGrp.lb->setAnchorPoint(CCPointZero);
        m_stCountryWujiangBuffGrp.lb->setPosition(CCPointMake(m_stCountryWujiangBuffGrp.sp->getContentSize().width-4, 4));
        m_stCountryWujiangBuffGrp.sp->addChild(m_stCountryWujiangBuffGrp.lb);
       
        sprintf(buf, "%d", m_stCountryWujiangBuffGrp.iCD);
        m_stCountryWujiangBuffGrp.lb->setString(buf);
    }

    
    
}



//******************************************************************************
// addEnWujiangBuff
//******************************************************************************
void Player::addEnWujiangBuff()
{
    char buf[50];
    
    if(m_stEnWujiangBuffGrp.sp == NULL){
        switch (m_stEnWujiangBuffGrp.iType) {
            case enGeneralTp_mojiang:
                sprintf(buf, "type_%d_mojiang.png", m_stEnWujiangBuffGrp.iType);
                break;
            case enGeneralTp_GuoShi:
                sprintf(buf, "type_%d_guoshiwushuang.png", m_stEnWujiangBuffGrp.iType);
                break;
            case enGeneralTp_GongShou:
                sprintf(buf, "type_%d_gongshoujianbei.png", m_stEnWujiangBuffGrp.iType);
                break;
            case enGeneralTp_ShengSheng:
                sprintf(buf, "type_%d_shengshengbuxi.png", m_stEnWujiangBuffGrp.iType);
                break;
            case enGeneralTp_XiaoYong:
                sprintf(buf, "type_%d_xiaoyongshanzhan.png", m_stEnWujiangBuffGrp.iType);
                break;
            case enGeneralTp_JinShi:
                sprintf(buf, "type_%d_jinshizhijian.png", m_stEnWujiangBuffGrp.iType);
                break;
            case enGeneralTp_God:
                sprintf(buf, "type_%d_shenjiang.png", m_stEnWujiangBuffGrp.iType);
                break;
            default:
                break;
        }
        m_stEnWujiangBuffGrp.sp = CCSprite::spriteWithSpriteFrameName(buf);
    }
    
    if(m_stEnWujiangBuffGrp.sp){
        addChild(m_stEnWujiangBuffGrp.sp);
        addBuff(m_stEnWujiangBuffGrp.sp);
    }
    
    if(m_stEnWujiangBuffGrp.lb == NULL)
        m_stEnWujiangBuffGrp.lb = TextNode::textWithString("0", 20);
    
    if((m_stEnWujiangBuffGrp.lb) && (m_stEnWujiangBuffGrp.sp)){
        m_stEnWujiangBuffGrp.lb->setAnchorPoint(CCPointZero);
        m_stEnWujiangBuffGrp.lb->setPosition(CCPointMake(m_stEnWujiangBuffGrp.sp->getContentSize().width, 0));
        m_stEnWujiangBuffGrp.sp->addChild(m_stEnWujiangBuffGrp.lb);
        
        sprintf(buf, "%d", m_stEnWujiangBuffGrp.iCD);
        m_stEnWujiangBuffGrp.lb->setString(buf);
    }
}

//******************************************************************************
// addSickBuff
//******************************************************************************
void Player::addSickBuff()
{
    if(m_stSickBuffGrp.sp == NULL)
        m_stSickBuffGrp.sp = CCSprite::spriteWithFile("playBuff_sick.png");
    
    if(m_stSickBuffGrp.sp){
        addChild(m_stSickBuffGrp.sp);
        addBuff(m_stSickBuffGrp.sp);
    }
}

//******************************************************************************
// addEnhanceBuffH6
//******************************************************************************
void Player::addEnhanceBuffH6()
{
    if(m_stEnhanceH6BuffGrp.sp == NULL)
        m_stEnhanceH6BuffGrp.sp = CCSprite::spriteWithFile("awakeskill_4.png");
    
    if(m_stEnhanceH6BuffGrp.sp){
        addChild(m_stEnhanceH6BuffGrp.sp);
        addBuff(m_stEnhanceH6BuffGrp.sp);
    }
    
    if(m_stEnhanceH6BuffGrp.lb == NULL)
        m_stEnhanceH6BuffGrp.lb = TextNode::textWithString("0", 16);
    
    char buf[50];
    if((m_stEnhanceH6BuffGrp.lb) && (m_stEnhanceH6BuffGrp.sp)){
        m_stEnhanceH6BuffGrp.lb->setAnchorPoint(CCPointZero);
        m_stEnhanceH6BuffGrp.lb->setPosition(CCPointMake(m_stEnhanceH6BuffGrp.sp->getContentSize().width, 0));
        m_stEnhanceH6BuffGrp.sp->addChild(m_stEnhanceH6BuffGrp.lb);
        
        sprintf(buf, "%.0f%%", m_stEnhanceH6BuffGrp.fFactor*100);
        m_stEnhanceH6BuffGrp.lb->setString(buf);
    }
}

//******************************************************************************
// removeBuff
//******************************************************************************
void Player::removeBuff(CCSprite* sp)
{
    CCSprite* buffArr[kPlayerBuffCnt];
    memset(buffArr, 0, sizeof(buffArr));
    
    int i = 0;
    int j = 0;
    for(i = 0; i < kPlayerBuffCnt; i++){
        if(m_buffArr[i] == sp){
            m_buffArr[i] = NULL;
        }
        else{
            if(m_buffArr[i]){
                buffArr[j] = m_buffArr[i];
                j++;
            }
        }
    }
    
    memcpy(m_buffArr, buffArr, sizeof(buffArr));
    
    redrawBuff();
}

//******************************************************************************
// addBuff
//******************************************************************************
void Player::addBuff(CCSprite* sp)
{
    int i = 0;
    for(i = 0; i < kPlayerBuffCnt; i++){
        if(m_buffArr[i] == sp)
            return;
        
        if(m_buffArr[i] == NULL){
            m_buffArr[i] = sp;
            break;
        }
    }
    
    redrawBuff();
}

//******************************************************************************
// redrawBuff
//******************************************************************************
void Player::redrawBuff()
{
    CCPoint pt(2, 839);
    
    for(int i = 0; i < kPlayerBuffCnt; i++){
        if(m_buffArr[i]){
            m_buffArr[i]->setAnchorPoint(ccp(0,0.5));
            m_buffArr[i]->setPosition(pt);
            pt.y -= m_buffArr[i]->getContentSize().height;
        }
        else{
            break;
        }
    }
}

//******************************************************************************
// canFei
//******************************************************************************
bool Player::canFei(bool bCanFei)
{
    if (bCanFei == m_bCanFei)
    {
        m_blood->setCanFeiColor(bCanFei);
        return false;
    }
    
    m_bCanFei = bCanFei;
    
    if (bCanFei)
    {
        int iCanFeiHP = m_iBaseHP - int(m_iBaseHP * 0.7);
        m_iBaseHP = m_iBaseHP * 0.7;
        if (m_iHP > m_iBaseHP.get())
        {
            m_iHP = m_iBaseHP;
            updateRoundStartHp();    // 被canfei后，无法触发神吕布被动技能
        }
        canFeiAnimate(iCanFeiHP);
    }
    else
    {
        m_iBaseHP = 0;
        for(int i = 0; i < kMaxCardCntInBattle; i++){
            if(m_card[i] == NULL)
                continue;
            
            m_iBaseHP += m_card[i]->getBaseHp();
        }
    }
    m_blood->canFei(bCanFei, m_iBaseHP);
    updateBlood(true);
    
    return bCanFei;
}

#pragma mark - Enemy Buff

void Player::addBuffEffect(int idx)
{
    switch (idx)
    {
        case enEnemyBuff_zhuanzhutime1:
            m_stoneGrid->changeMaxDurByTime(-1.0f);
            break;
            
        case enEnemyBuff_zhuanzhutime2:
            m_stoneGrid->changeMaxDurByTime(-2.0f);
            break;
            
        case enEnemyBuff_63:
            m_stoneGrid->changeMaxDurByTime(-3.0f);
            break;
            
        default:
            break;
    }
}

void Player::removeBuffEffect(int idx)
{
    switch (idx)
    {
        case enEnemyBuff_zhuanzhutime1:
            m_stoneGrid->changeMaxDurByTime(1.0f);
            break;
            
        case enEnemyBuff_zhuanzhutime2:
            m_stoneGrid->changeMaxDurByTime(2.0f);
            break;
            
        case enEnemyBuff_63:
            m_stoneGrid->changeMaxDurByTime(3.0f);
            break;
            
        case enEnemyBuff_wenyihuanjing:
        {
            m_iBaseHP = 0;
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if(m_card[i] == NULL)
                    continue;
                
                m_iBaseHP += m_card[i]->getBaseHp();
            }
            
            updateBlood(false);
        }
            break;
            
        default:
            break;
    }
}

void Player::beHitFromEnemyBuf(int bufIdx)
{
    switch (bufIdx)
    {
        case enEnemyBuff_wenyi:
            if (m_iHP > 1)
            {
                m_iHP -= (m_iHP * 0.3);
                m_iHP = m_iHP < 1 ? 1 : m_iHP.get();
                updateBlood();
            }
            break;
            
        case enEnemyBuff_wenyi2:
            m_iHP -= (m_iBaseHP * 0.3);
            updateBlood();
            break;
            
        case enEnemyBuff_wenyihuanjing:
            m_iBaseHP -= (m_iBaseHP * 0.1);
            m_iBaseHP = m_iBaseHP>0?m_iBaseHP.get():1;
            m_iHP = m_iHP<=m_iBaseHP.get()?m_iHP.get():m_iBaseHP.get();
            updateBlood(false);
            break;
            
        case enEnemyBuff_wenyihuanjing2:
            if (m_iHP > 1)
            {
                m_iHP -= (m_iHP * 0.1);
                m_iHP = m_iHP < 1 ? 1 : m_iHP.get();
                updateBlood();
            }
            break;
            
        case enEnemyBuff_wenyihuanjing3:
            m_iHP -= (m_iBaseHP * 0.1);
            updateBlood();
            break;
            
        default:
            break;
    }
}

//void Player::beWenYiHuanJing()
//{
//    int offHp = m_iBaseHP * 0.1;
//    offHp = (offHp == 0?1:offHp);
//    
//    m_iBaseHP -= offHp;
//    m_iBaseHP = m_iBaseHP>0?m_iBaseHP:1;
//    m_iHP = m_iHP<=m_iBaseHP?m_iHP:m_iBaseHP;
//
//    updateBlood(false);
//}
//******************************************************************************
// canFeiAnimate
//******************************************************************************
void Player::canFeiAnimate(int iCanFeiBlood)
{
    showHpDownNum(iCanFeiBlood);
    
    m_blood->setPosition(CCPointMake(320, 545));
    m_blood->runAction(CCShake::actionWithDuration(0.5, m_blood->getPosition()));
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(hpdown));

}

//******************************************************************************
// updateSkillHighlight
//******************************************************************************
void Player::updateSkillHighlight()
{
    if (m_pSkillHighlight == NULL) {
        m_pSkillHighlight = ShiningFrame::node();
        addChild(m_pSkillHighlight);
    }
    
    m_pSkillHighlight->clrCardList();
    
    for(int i = 0; i < kMaxCardCntInBattle; i++){
        if(m_card[i] == NULL)
            continue;
        
        if(m_card[i]->getCD() == 0){
            m_pSkillHighlight->addCard(m_card[i]);
        }
    }
}

//******************************************************************************
// getAttackElemCnt
//******************************************************************************
int Player::getAttackElemCnt()
{
    int elemCnt = 0;
    for (int i = 0; i < enElementMax; i++) {
        if (m_bElementAttck[i])
            elemCnt++;
    }
    
    return elemCnt;
}

//******************************************************************************
// OnExit
//******************************************************************************
void Player::onExit()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
    CCLayer::onExit();
}

//******************************************************************************
// record videl
//******************************************************************************
void Player::StartUpdate()
{
    if (RecordHelper::Inst()->CheckRecording())
    {
        m_record_wait_time = 2;
        scheduleUpdate();
    }
}

void Player::StopUpdate()
{
    unscheduleUpdate();
}

void Player::update(ccTime dt)
{
    m_record_wait_time -= dt;
    
    if (m_record_wait_time <= 0)
    {
        unscheduleUpdate();
        RecordHelper::Inst()->SetPauseRecord(true);
    }
}

void Player::RecordControl(bool pause, int time)
{
    if (!RecordHelper::Inst()->CheckRecording())
        return;
    
    RecordHelper::Inst()->SetPauseRecord(pause);
    
    if (pause)
    {
        if (m_record_wait_time > 0.0f)
        {
            if (time <= 0)
            {
                m_record_wait_time = 0.0f;
                unscheduleUpdate();
            }
            else
            {
                m_record_wait_time = (float)time;
            }
        }
        else
        {
            if (time > 0)
            {
                m_record_wait_time = time;
                scheduleUpdate();
            }
        }

    }
    else
    {
        if (m_record_wait_time > 0.0f)
        {
            if (time <= 0)
            {
                m_record_wait_time = 0.0f;
                unscheduleUpdate();
            }
            else
            {
                m_record_wait_time = (float)time;
            }
        }
        else
        {
            if (time > 0)
            {
                m_record_wait_time = time;
                scheduleUpdate();
            }
        }
    }
}

void Player::changeCaiDanStone()
{
    m_stoneGrid->changeCaiDanStone();
}

int Player::getCurHp()
{
    return m_iHP;
}

int Player::getBaseHP()
{
    return m_iBaseHP;
}

void Player::enemyDecBuffCD(){
    decBuffCD();
}


bool Player::beAllGoGenIDList(CAutoSkillAppend *pAutoSkillAppend)
{
    if (pAutoSkillAppend)
    {
        std::vector<unsigned short> vrData;
        for (int i = 0; i < kMaxCardCntInBattle; ++i)
        {
            if (m_card[i])
            {
                vrData.push_back(m_card[i]->getCardID());
            }
        }
        return pAutoSkillAppend->isGenIDList(vrData);
    }
    return true;
}




void Player::setAnimal_awakeskill_30()
{
    string ss("MISS");

    m_tlabel = TextNode::textWithString(ss.c_str(), m_blood->getContentSize(), CCTextAlignmentLeft, 35);
    m_tlabel->setColor(ccRED);
    m_tlabel->setShadowColor(ccBLACK);
   CCPoint pt = m_blood->getPosition();
    pt.x  += 200;
    m_tlabel->setPosition(pt);
    m_tlabel->setAnchorPoint(CCPointMake(1, 0));
    addChild(m_tlabel,11,kTagToBeDel);
    m_tlabel->runAction(CCSpawn::actions(CCMoveBy::actionWithDuration(1.0, CCPointMake(0, 100)),
                                         CCFadeOut::actionWithDuration(1.0),
                                         NULL));

}

bool Player::haveAwakeSkill_33()
{
    if (m_iAwakeSkill_33 == 1) {
        return true;
    } else if (m_iAwakeSkill_33 == 2) {
        return false;
    } else {
        int iOccurOdds = getNumByAwakeId(enAwakeSkill_33)*(CSkillLib2::Ins()->getAwakeSkill(enAwakeSkill_33)->cOdds);
        if (SaveData::Inst()->getrand()%100+1 <= iOccurOdds) {
            m_iAwakeSkill_33 = 1;
            return true;
        } else {
            m_iAwakeSkill_33 = 2;
            return false;
        }
    }
}

bool Player::haveAwakeSkill_32()
{
    int iOccurOdds = getNumByAwakeId(enAwakeSkill_32)*(CSkillLib2::Ins()->getAwakeSkill(enAwakeSkill_32)->cOdds);
    if (SaveData::Inst()->getrand()%100+1 <= iOccurOdds) {
        return true;
    } else {
        return false;
    }
}

void Player::delayPlaySkillCD(int d_cd)
{
    for(int i = 0; i < kMaxCardCntInBattle; i++){
        if(m_card[i] == NULL)
            continue;
        m_card[i]->updateCd(d_cd);
        if(m_card[i]->getCD() > 0)
        {
            m_card[i]->runAction(CCMoveTo::actionWithDuration(0.3, CCPointMake(0, 0)));
            m_card[i]->showSkillAnimation();
        }
    }

}

//bool Player::beAllGoGenIDList(int n,...)
//{
//    va_list ap;
//    va_start(ap, n);
//    int temp = n;
//    for(int i = 0; i < n; i++)
//    {
//        bool b = false;
//        for (int i = 0; i < kMaxCardCntInBattle; ++i)
//        {
//            if (temp==m_card[i]->getCardID())
//            {
//                b = true;
//            }
//        }
//        if (!b)
//        {
//            va_end(ap);
//            return false;
//        }
//        temp = va_arg(ap, int);
//    }
//    va_end(ap);
//    return true;
//}


//EnemyGrp * Player::getEnemyGrp()const
//{
//    if (FightMngr *pFightMngr = dynamic_cast<FightMngr*>(m_pListener))
//    {
//        return pFightMngr->getEnemyGrp();
//    }
//    return NULL;
//}




