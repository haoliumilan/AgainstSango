//
//  EnemyGrp.cpp
//  CardDemo
//
//  Created by Hao Zhang on 12-4-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CommonDef.h"
#include "EnemyGrp.h"
#include "CGameState.h"
#include "CHurtNum.h"
#include "CCShake.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "SimpleAudioEngine.h"
#include "FightMngr.h"


using namespace CocosDenshion;

static CCPoint gEnemyPos[6];

static float gElemOppositeTbl[6][6] = {
    //              Heart,  Water,  Fire,   Wood,  Light,  Dark
    /* Heart */     {1.0,   1.0,    1.0,    1.0,    1.0,    1.0},
    /* Water */     {1.0,   1.0,    2.0,    0.5,    1.0,    1.0},
    /* Fire  */     {1.0,   0.5,    1.0,    2.0,    1.0,    1.0},
    /* Wood  */     {1.0,   2.0,    0.5,    1.0,    1.0,    1.0},
    /* Light */     {1.0,   1.0,    1.0,    1.0,    1.0,    2.0},
    /* Dark  */     {1.0,   1.0,    1.0,    1.0,    2.0,    1.0}
};

// 被克制的属性
static enElementDef gElemIntead[6] = {
    enElementHeart,
    enElementWood,
    enElementWater,
    enElementFire,
    enElementDark,
    enElementLight,
};

#define kMonsterYPos            686
#define kEnemySkillTag          0x1212
#define kEnemySkDetailTag       20130609
#define kEnemySkillAni          999

//******************************************************************************
// Constructor
//******************************************************************************
EnemyGrp::EnemyGrp(CDgStepInfo* pStepInfo, CCObject* target, SEL_CallFuncO selector)
{
    m_spBox = NULL;
    m_bSick = false;
    
    for (int i = 0; i < enEnemyBuff_max; i ++)
    {
        m_EnemyGrpBuff[i].spBuffIcon = NULL;
        m_EnemyGrpBuff[i].bBuffEffect = false;
    }
    
    m_pListener = target;
    m_pfnSelector = selector;

    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
    
    SaveData::Inst()->setPoisonCd(false);
    SaveData::Inst()->setPoisonPoint(0);
    
    initEnemyPos(pStepInfo);
    
    CCMutableArray<CDgMonsterInfo*> * pMonInfoArr = pStepInfo->m_pMonsterArr;
    CDgMonsterInfo* pMonInfo;
    
    m_iEnemyCnt = pMonInfoArr->count();
    m_iCurEnemy = 0;
    
    for (int i = 0; i < kMaxEnemyCnt; i++) {m_enemy[i] = NULL;}
    
    for(int i = 0; i < m_iEnemyCnt; i++){
        pMonInfo = pMonInfoArr->getObjectAtIndex(i);
        
        m_enemy[i] = new Enemy(pMonInfo,
                               gEnemyPos[i],
                               this,
                               callfuncO_selector(EnemyGrp::cbEnemy),
                               i);
        FightMngr* fm = (FightMngr*)m_pListener;
        for (int j = 0; j < ENEMYBUFFMAX; j++)
        {
            if (m_enemy[i]->getBuffId(j) == enEnemyBuff_82) {
                for (int k = 0; k < 6; k++) {
                    if (fm->getPlayer()->getCardByIdx(k)) {
                        fm->getPlayer()->getCardByIdx(k)->updateCd(5);
                    }
                }
                
            }
        }
        addChild(m_enemy[i]);
        m_enemy[i]->release();
    }
    
    updateEnemyBuff();
    
    
    
    
    
    schedule(schedule_selector(EnemyGrp::CheckFirstAtk), 0.2f);
    
    m_spAim = CCSprite::spriteWithSpriteFrameName("aim.png");
    m_spAim->setPosition(CCPointMake(-1000, -1000));
    addChild(m_spAim, 2);
    m_spAim->runAction(CCRepeatForever::actionWithAction(CCRotateBy::actionWithDuration(3.0, 360)));
    
    m_iGoldBox = pStepInfo->gold_box;
    if(m_iGoldBox > 0){
        m_iGoldBoxDropIdx = rand() % m_iEnemyCnt;
    }
    
    m_iSelected = kInvalidValue; // selected nothing
    m_iHitCnt = 0;    
}

EnemyGrp::EnemyGrp(SaveData_EnemyGrp* pStepInfo, CCObject* target, SEL_CallFuncO selector)
{
    m_spBox = NULL;
    for (int i = 0; i < kMaxEnemyCnt; i++) {m_enemy[i] = NULL;}

    for (int i = 0; i < enEnemyBuff_max; i ++)
    {
        m_EnemyGrpBuff[i].spBuffIcon = NULL;
        m_EnemyGrpBuff[i].bBuffEffect = false;
    }

    m_pListener = target;
    m_pfnSelector = selector;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
    
    if (!SaveData::Inst()->getIsUseSaveData())
    {
        SaveData::Inst()->setPoisonCd(false);
        SaveData::Inst()->setPoisonPoint(0);
    }
    
    initEnemyPos(pStepInfo);
        
    m_iEnemyCnt = pStepInfo->enemycnt;
    m_iCurEnemy = 0;
    
    for(int i = 0; i < m_iEnemyCnt; i++)
    {
        m_enemy[i] = new Enemy(&(pStepInfo->enemies[i]),
                               gEnemyPos[i],
                               this,
                               callfuncO_selector(EnemyGrp::cbEnemy),
                               i);
        addChild(m_enemy[i]);
        m_enemy[i]->release();
    }
    
    updateEnemyBuff();
    
    // s&l, 保存的这一局，如果是从天降宝珠开始游戏，那么就不去判断先制攻击；
    // 新的一局，不可能从天降宝珠开始，所以也不用判断了
    if (SaveData::Inst()->getIsUseSaveData())
    {
        if(SaveData::Inst()->getiMoment() == 0)
        {
            schedule(schedule_selector(EnemyGrp::CheckFirstAtk), 0.3f);
        }
    }
    else
    {
        schedule(schedule_selector(EnemyGrp::CheckFirstAtk), 0.3f);
    }
    
    m_iSelected = kInvalidValue; // selected nothing

    m_spAim = CCSprite::spriteWithSpriteFrameName("aim.png");
    m_spAim->setPosition(CCPointMake(-1000, -1000));
    addChild(m_spAim, 2);
    m_spAim->runAction(CCRepeatForever::actionWithAction(CCRotateBy::actionWithDuration(3.0, 360)));
    if (SaveData::Inst()->getIsUseSaveData())
    {
        int aimIndex = SaveData::Inst()->getAimingIndex();
        if (aimIndex != kInvalidValue) {
            m_iSelected = aimIndex;
            m_spAim->setPosition(m_enemy[aimIndex]->getEnemyPos());
        }
        
        if (SaveData::Inst()->getPoisonCd())
        {
            m_iPoisonPt = SaveData::Inst()->getPoisonPoint();

//            for(int i = 0; i < m_iEnemyCnt; i++)
//                m_enemy[i]->setPoison();
        }
        
        if(isSick())
        {
            m_iSickPt = SaveData::Inst()->getSickPoint();
        }
    }

    m_iGoldBox = pStepInfo->goldbox;
    if(m_iGoldBox > 0){
        m_iGoldBoxDropIdx = rand() % m_iEnemyCnt;
    }
    
    m_iHitCnt = 0;
}

//******************************************************************************
// ~EnemyGrp
//******************************************************************************
EnemyGrp::~EnemyGrp()
{

}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool EnemyGrp::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    int iCnt = 0;
    for (int i=0; i<m_iEnemyCnt; i++)
    {
        if (m_enemy[i]->isAlive())
            iCnt++;
    }
    
    if(iCnt < 2)
        return false;
    
    CCPoint touchLocation = touch->locationInView( touch->view() );	
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint nodePos = convertToNodeSpace( touchLocation );
    
    CCPoint posEnemy;
    CCSize  szEnemy;
    
    for(int i = 0; i < m_iEnemyCnt; i++){
        posEnemy = m_enemy[i]->getEnemyPos();
        szEnemy = m_enemy[i]->getEnemySize();

        if((m_enemy[i]->isAlive())
           && CCRect::CCRectContainsPoint(CCRectMake(posEnemy.x - szEnemy.width / 2,
                                                     posEnemy.y - szEnemy.height / 2,
                                                     szEnemy.width,
                                                     szEnemy.height),
                                          nodePos)){
            
            if(m_iSelected == i){
                m_iSelected = kInvalidValue;
                SaveData::Inst()->setAimingIndex(kInvalidValue);
                m_spAim->setPosition(CCPointMake(-1000, -1000));
            }
            else{
                m_iSelected = i;
                SaveData::Inst()->setAimingIndex(i);
                m_spAim->setPosition(m_enemy[i]->getEnemyPos());
            }
            break;
        }
    }
    
    return false;
}

//******************************************************************************
// queryAtkPos
//******************************************************************************
CCPoint* EnemyGrp::queryAtkPos(CAttackInfo* pAtkInfo)
{
    int j = 0;
    
    if(pAtkInfo->iTargetCnt == 1){
        for(int i = 0; i < m_iEnemyCnt; i++){
            if(m_enemy[i]->isAlive()){
                m_ptQueue[j] = m_enemy[i]->getEnemyPos();
                j++;
            }
        }
        
        m_ptQueue[j].x = 0;
        m_ptQueue[j].y = 0;
    }
    else {
        stAutoSkill* pAutoSkill = CSkillLib::Ins()->getAutoSkill(pAtkInfo->iAutoSkillID);
        int idx = kInvalidValue;
        
        if(m_iSelected != kInvalidValue){
            m_ptQueue[0] = m_enemy[m_iSelected]->getEnemyPos();
            m_ptQueue[1].x = 0;
            m_ptQueue[1].y = 0;
            
            idx = m_iSelected;
        }
        else{
            int k = lookupElemOpposite(pAtkInfo);
            
            if(k == kInvalidValue){
                m_ptQueue[0].x = 0;
                m_ptQueue[0].y = 0;
            }
            else{
                m_ptQueue[0] = m_enemy[k]->getEnemyPos();
                m_ptQueue[1].x = 0;
                m_ptQueue[1].y = 0;
                
                idx = k;
            }
        }

        if((pAutoSkill) && (pAutoSkill->iType == enAutoSkillTp_elemburn)){
            if(idx != kInvalidValue){
                float fFactor = gElemOppositeTbl[pAutoSkill->cElement][m_enemy[idx]->getElement()];
                if(fFactor < 1.1){
                    m_ptQueue[0].x = 0;
                    m_ptQueue[0].y = 0;
                }
            }
        }
    }

    
    return m_ptQueue;
}


void EnemyGrp::ResetEnemyLianFanState(){  //新增
    for(int i = 0; i < m_iEnemyCnt; i++){
        if(m_enemy[i]->isAlive()){
            m_enemy[i]->setLianFan(true);
        }
    }

}

//******************************************************************************
// beAttacked
//******************************************************************************
int EnemyGrp::beAttacked(CAttackInfo* pAtkInfo, ccTime dt)
{
    int iAtkPt = 0;
    
    if(chkBuff(pAtkInfo) == true)
        return iAtkPt;
    
    stAutoSkill* pAutoSkill = CSkillLib::Ins()->getAutoSkill(pAtkInfo->iAutoSkillID);
    
    if (pAtkInfo->iTargetCnt == 3)  // 反击、主动技能
    {
        iAtkPt = attackEnemy(pAtkInfo->iEnemyIdx, pAtkInfo, dt);
    }
    //反击、被动技能
    else if((pAutoSkill) && (pAutoSkill->iType == enAutoSkillTp_fanji)){
            iAtkPt = attackEnemy(pAtkInfo->iEnemyIdx, pAtkInfo, dt);
    }
    //弱点
    else if((pAutoSkill) && (pAutoSkill->iType == enAutoSkillTp_elemburn)){
        int idx = kInvalidValue;
        
        //判定选中的敌人是否为被克制属性
        if (m_iSelected != kInvalidValue) {
            float fFactor = gElemOppositeTbl[pAutoSkill->cElement][m_enemy[m_iSelected]->getElement()];
            if(fFactor > 1.1)
                idx = m_iSelected;
        }
        
        if(idx == kInvalidValue){
            idx = lookupElemOpposite(pAtkInfo);
            
            float fFactor = gElemOppositeTbl[pAutoSkill->cElement][m_enemy[idx]->getElement()];
            if(fFactor < 1.1)
                idx = kInvalidValue;
        }
        
        if(idx != kInvalidValue)
            iAtkPt = attackEnemy(idx, pAtkInfo, dt);
    }
    //欺压
    else if(pAtkInfo->iAutoSkillID == enAutoSkill_QiYa
            ||pAtkInfo->iAutoSkillID == enAutoSkill_QiYa2
            ){
        int idx = lookupWeakest();
        
        if(idx != kInvalidValue)
            iAtkPt = attackEnemy(idx, pAtkInfo, dt);
    }
    //普通攻击
    else{
        

        //群攻
        if(pAtkInfo->iTargetCnt == 1){
            for(int i = 0; i < m_iEnemyCnt; i++){
                if(m_enemy[i]->isAlive()){
                    iAtkPt = attackEnemy(i, pAtkInfo, dt);
                }
            }
        }
        //属性攻
        else if (pAtkInfo->iTargetCnt == 2) {
            for (int i = 0; i < m_iEnemyCnt; i++) {
                if (!m_enemy[i]->isAlive())
                    continue;
                
                float fFactor = gElemOppositeTbl[pAtkInfo->iElement][m_enemy[i]->getElement()];
                if (fFactor >= 2.0)
                    attackEnemy(i, pAtkInfo, dt);
            }
        }
        //觉醒技:二体攻击
        else if (4==pAtkInfo->iTargetCnt)
        {
            int nAtkCount = 2;      //攻击数量
            if(m_iSelected != kInvalidValue)
            {
                iAtkPt = attackEnemy(m_iSelected, pAtkInfo, dt);
                int nIndex2 = lookupElemOpposite(pAtkInfo,m_iSelected);
                if (kInvalidValue!=nIndex2)
                {
                    iAtkPt = attackEnemy(nIndex2, pAtkInfo, dt);
                }
                nAtkCount = 0;
            }
            else
            {
                std::set<int> setTemp;
                for (int i = 0; i < m_iEnemyCnt; i++)
                {
                    int nIndex = lookupElemOppositeBig(pAtkInfo,i);
                    if (kInvalidValue!=nIndex)
                    {
                        setTemp.insert(nIndex);
                    }
                }
                
                if (2>setTemp.size())
                {
                    for(int i = 0; i < m_iEnemyCnt; i++)
                    {
                        if(m_enemy[i]->isAlive())
                        {
                            setTemp.insert(i);
                        }
                    }
                }
                
                
                std::set<int>::iterator it = setTemp.begin(),end = setTemp.end();
                for (; it != end; ++it)
                {
                    if(1>nAtkCount)break;
                    nAtkCount--;
                    iAtkPt = attackEnemy(*it, pAtkInfo, dt);
                }
            }
            
            
        }
        //单打
        else
        {
            if(m_iSelected != kInvalidValue)
            {
                iAtkPt = attackEnemy(m_iSelected, pAtkInfo, dt);
            }
            else
            {
                int k = lookupElemOpposite(pAtkInfo);
                
                if(k != kInvalidValue)
                    iAtkPt = attackEnemy(k, pAtkInfo, dt);
            }
        }
    }

    chkAim();
    
    return iAtkPt;
}

float EnemyGrp::geRatioFromBuf(CAttackInfo atkinfo, int iEnemyIdx)
{
    if ((atkinfo.iManSkillID == 0 || atkinfo.iManSkillID == kInvalidValue)
        && (atkinfo.iAutoSkillID == 0 || atkinfo.iAutoSkillID == kInvalidValue))
    {
        if (m_EnemyGrpBuff[enEnemyBuff_combodun2].bBuffEffect || m_EnemyGrpBuff[enEnemyBuff_combodun22].bBuffEffect)
        {
            FightMngr* fm = (FightMngr*)m_pListener;
            if (abs(fm->getPlayer()->getCurCombo() - m_iComboDunNum) == 1)
                return 0.35f;
            
            if (abs(fm->getPlayer()->getCurCombo() - m_iComboDunNum) == 2)
                return 0.15f;
        }
    }
    
    if (m_EnemyGrpBuff[enEnemyBuff_64].bBuffEffect)
    {
        if (atkinfo.iAtkCategory == enGeneralTp_God
            || atkinfo.iAtkSubCategory == enGeneralTp_God
            )
        {
            return 0.50f;
        }
    }
    
    return 1.0f;
}

bool EnemyGrp::beDefenFromBuf(CAttackInfo atkinfo, int iEnemyIdx)
{
    if ((atkinfo.iManSkillID == 0 || atkinfo.iManSkillID == kInvalidValue)
        && (atkinfo.iAutoSkillID == 0 || atkinfo.iAutoSkillID == kInvalidValue))
    {
        if (m_EnemyGrpBuff[enEnemyBuff_combodun3].bBuffEffect || m_EnemyGrpBuff[enEnemyBuff_combodun32].bBuffEffect)
        {
            FightMngr* fm = (FightMngr*)m_pListener;
            if (fm->getPlayer()->getCurCombo() != m_iComboDunNum)
                return true;
        }
        
        if (m_EnemyGrpBuff[enEnemyBuff_combodun2].bBuffEffect || m_EnemyGrpBuff[enEnemyBuff_combodun22].bBuffEffect)
        {
            FightMngr* fm = (FightMngr*)m_pListener;
            if (abs(fm->getPlayer()->getCurCombo() - m_iComboDunNum) >= 3)
                return true;
        }
        
        if (m_EnemyGrpBuff[enEnemyBuff_1combodun].bBuffEffect || m_EnemyGrpBuff[enEnemyBuff_1combodun2].bBuffEffect)
        {
            FightMngr* fm = (FightMngr*)m_pListener;
            if (fm->getPlayer()->getCurCombo() != 1)
                return true;
        }
        
        if (m_EnemyGrpBuff[enEnemyBuff_2combodun].bBuffEffect || m_EnemyGrpBuff[enEnemyBuff_2combodun2].bBuffEffect)
        {
            FightMngr* fm = (FightMngr*)m_pListener;
            if (fm->getPlayer()->getCurCombo() != 2)
                return true;
        }
        
        if (m_EnemyGrpBuff[enEnemyBuff_3combodun].bBuffEffect || m_EnemyGrpBuff[enEnemyBuff_3combodun2].bBuffEffect)
        {
            FightMngr* fm = (FightMngr*)m_pListener;
            if (fm->getPlayer()->getCurCombo() != 3)
                return true;
        }
        
        if (m_EnemyGrpBuff[enEnemyBuff_4combodun].bBuffEffect || m_EnemyGrpBuff[enEnemyBuff_4combodun2].bBuffEffect)
        {
            FightMngr* fm = (FightMngr*)m_pListener;
            if (fm->getPlayer()->getCurCombo() != 4)
                return true;
        }
        
        if (m_EnemyGrpBuff[enEnemyBuff_5combodun].bBuffEffect || m_EnemyGrpBuff[enEnemyBuff_5combodun2].bBuffEffect)
        {
            FightMngr* fm = (FightMngr*)m_pListener;
            if (fm->getPlayer()->getCurCombo() != 5)
                return true;
        }
        
        if (m_EnemyGrpBuff[enEnemyBuff_6combodun].bBuffEffect || m_EnemyGrpBuff[enEnemyBuff_6combodun2].bBuffEffect)
        {
            FightMngr* fm = (FightMngr*)m_pListener;
            if (fm->getPlayer()->getCurCombo() != 6)
                return true;
        }
    }
    
    
    if ((m_EnemyGrpBuff[enEnemyBuff_aytaipingyaoshu].bBuffEffect)
        && atkinfo.iElement != enElementRecover)
        return true;
    
    if ((m_EnemyGrpBuff[enEnemyBuff_ysshenyuan].bBuffEffect)
        && atkinfo.iElement != enElementLight)
        return true;
    
    if (m_EnemyGrpBuff[enEnemyBuff_hunyuandun].bBuffEffect)
    {
        FightMngr* fm = (FightMngr*)m_pListener;
        if (fm->getPlayer()->getCurCombo() < 4)
        return true;
    }
    
    if (((m_EnemyGrpBuff[enEnemyBuff_ystaipingdao].bBuffEffect) || (m_EnemyGrpBuff[enEnemyBuff_77].bBuffEffect))
        && atkinfo.iTargetCnt == 0)
        return true;
    
    if ((m_enemy[iEnemyIdx]->isBuffByIdx(EN_ENEMY_MUTENGJIA)||m_EnemyGrpBuff[enEnemyBuff_mutengjia].bBuffEffect)
        && atkinfo.iElement != enElementFire)
        return true;
    
    if (m_EnemyGrpBuff[enEnemyBuff_shuxinghudun].bBuffEffect && atkinfo.iElement != gElemIntead[m_enemy[iEnemyIdx]->getElement()])
        return true;
    
    if (m_EnemyGrpBuff[enEnemyBuff_shuxinghudun2].bBuffEffect && atkinfo.iElement != gElemIntead[m_enemy[iEnemyIdx]->getElement()])
        return true;
    
    if (m_enemy[iEnemyIdx]->isBuffByIdx(EN_ENEMY_JINGANGHUTI))
        return true;
    
    if ((m_enemy[iEnemyIdx]->isBuffByIdx(EN_ENEMY_DEFEN_FIRE))
        && atkinfo.iElement == enElementFire)
        return true;
    
    if ((m_enemy[iEnemyIdx]->isBuffByIdx(EN_ENEMY_DEFEN_WATER))
        && atkinfo.iElement == enElementWater)
        return true;
    
    if ((m_enemy[iEnemyIdx]->isBuffByIdx(EN_ENEMY_DEFEN_WOOD))
        && atkinfo.iElement == enElementWood)
        return true;
    
    if ((m_enemy[iEnemyIdx]->isBuffByIdx(EN_ENEMY_DEFEN_LIGHT))
        && atkinfo.iElement == enElementLight)
        return true;
    
    if ((m_enemy[iEnemyIdx]->isBuffByIdx(EN_ENEMY_DEFEN_AN))
        && atkinfo.iElement == enElementDark)
        return true;
    
    return false;
}

int EnemyGrp::attackEnemy(int iEnemyIdx, CAttackInfo* attackinfo, ccTime dt)
{
    int iAttackPt1, iAttackPt2;
    float factor;
    
    CAttackInfo atkinfo = *attackinfo;
    
    if (atkinfo.iElement == enElementInstead)
    {
        if (m_enemy[iEnemyIdx]->getElement() < enElementMax)
        {
            atkinfo.iElement = gElemIntead[m_enemy[iEnemyIdx]->getElement()];
        }
    }
    
    stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(atkinfo.iManSkillID);
    stAutoSkill* pAutoSkill = CSkillLib::Ins()->getAutoSkill(attackinfo->iAutoSkillID);
        
    //重力攻擊
    if(((pManSkill) && (pManSkill->iType == enManSkillTp_gravity))
       || (atkinfo.iManSkillID == enManSkill_GuaGuLiaoShang))
    {
        
        iAttackPt1 = m_enemy[iEnemyIdx]->getCurHp() * pManSkill->fEffect;
        iAttackPt2 = iAttackPt1;
        
//        float fratio = geRatioFromBuf(atkinfo,iEnemyIdx);
//        iAttackPt1 *= fratio;
//        iAttackPt2 *= fratio;
//        
//         if (beDefenFromBuf(atkinfo,iEnemyIdx))
//         {
//             iAttackPt1 = 0;
//             iAttackPt2 = 0;
//         }
        
        this->runAction(CCShake::actionWithDuration(1.0, CCPointMake(0, 0)));
    }
    else if (atkinfo.iElement == enElementGod)
    {// 固定伤害，无视属性，防御
        iAttackPt1 = atkinfo.iAttackPt;
        iAttackPt2 = iAttackPt1;
    }
    else
    {
        iAttackPt1 = atkinfo.iAttackPt - m_enemy[iEnemyIdx]->getArmor();
        if(iAttackPt1 <= 0){
            if((pAutoSkill) && (pAutoSkill->iType == enAutoSkillTp_burn)){
                iAttackPt1 = 0;
            }
            else if((pAutoSkill) && (pAutoSkill->iType == enAutoSkillTp_elemburn)){
                iAttackPt1 = 0;
            }
            else if(attackinfo->iAutoSkillID == enAutoSkill_QiYa
                    ||attackinfo->iAutoSkillID == enAutoSkill_QiYa2
                    ){
                iAttackPt1 = 0;
            }
            else{
                iAttackPt1 = 1;
            }
        }
        if(atkinfo.iElement >= enElementMax)
            factor = 1.0;
        else
            factor = gElemOppositeTbl[atkinfo.iElement][m_enemy[iEnemyIdx]->getElement()];
        
        iAttackPt2 = atkinfo.iAttackPt * factor  - m_enemy[iEnemyIdx]->getArmor();
        if(iAttackPt2 <= 0){
            if((pAutoSkill) && (pAutoSkill->iType == enAutoSkillTp_burn)){
                iAttackPt2 = 0;
            }
            else if((pAutoSkill) && (pAutoSkill->iType == enAutoSkillTp_elemburn)){
                iAttackPt2 = 0;
            }
            else if(attackinfo->iAutoSkillID == enAutoSkill_QiYa
                    ||attackinfo->iAutoSkillID == enAutoSkill_QiYa2
                    ){
                iAttackPt2 = 0;
            }
            else{
                iAttackPt2 = 1;
            }
        }
    }
    
    // 敌将的无敌buff效果
    if (m_enemy[iEnemyIdx]->isWudi())
        iAttackPt2 = 0;
    
    float fratio = geRatioFromBuf(atkinfo,iEnemyIdx);
    iAttackPt1 *= fratio;
    iAttackPt2 *= fratio;
    
    if (beDefenFromBuf(atkinfo,iEnemyIdx))
    {
        iAttackPt1 = 0;
        iAttackPt2 = 0;
    }
    

    // enEnemyBuff_73/enEnemyBuff_76
    FightMngr* fm = (FightMngr*)m_pListener;
    if ((getIsEnemyGrpBuffByIdx(enEnemyBuff_73) || getIsEnemyGrpBuffByIdx(enEnemyBuff_76))
        && !fm->getPlayer()->getElementHasPlus(attackinfo->iElement)
        && fm->getPlayer()->getStoneGrid()->getCurEraseCnt()) {  // 表明不是放武将技
        iAttackPt1 = 0;
        iAttackPt2 = 0;
    }
    
   
    

    CHurtNum* hurtnum;
    switch (atkinfo.iElement) {
        case enElementHeart:
            hurtnum = CHurtNum::initNumber("pinknum.png");
            break;
            
        case enElementWood:
            hurtnum = CHurtNum::initNumber("greennum.png");
            break;
            
        case enElementWater:
            hurtnum = CHurtNum::initNumber("bluenum.png");
            break;
            
        case enElementLight:
            hurtnum = CHurtNum::initNumber("yellownum.png");
            break;
            
        case enElementDark:
            hurtnum = CHurtNum::initNumber("purplenum.png");
            break;
            
        case enElementFire:
        default:
            hurtnum = CHurtNum::initNumber("rednum.png");
            break;
    }
    hurtnum->setPosition(m_enemy[iEnemyIdx]->getEnemyPos());
    addChild(hurtnum, 1);
    hurtnum->setNumber(iAttackPt1, iAttackPt2, dt);
    
    if (m_enemy[iEnemyIdx]->isBuffByIdx(EN_ENEMY_500THOUNSAND_LIMIT) ) {
        
        int roundAtk = m_enemy[iEnemyIdx]->getcurrentRoundAtk();
        
       
        if (iAttackPt2+ roundAtk > 500000) {
            
            iAttackPt2 = 500000 - roundAtk;
             m_enemy[iEnemyIdx]->setcurrentRoundAtk( 500000);
        }else{
            m_enemy[iEnemyIdx]->setcurrentRoundAtk(iAttackPt2 + roundAtk);
        }
    }
    atkinfo.iAttackPt = iAttackPt2;
    m_enemy[iEnemyIdx]->beAttacked(&atkinfo, dt);
    
    if(m_enemy[iEnemyIdx]->isAlive() == false)
    {
        updateEnemyBuff();
        m_enemy[iEnemyIdx]->setcurrentRoundAtk(0);
        if((m_iGoldBox > 0) && (iEnemyIdx == m_iGoldBoxDropIdx))
            dropbox(m_enemy[iEnemyIdx]->getEnemyPos(), dt + 1.0);
    }
    
    return iAttackPt2;
}

bool EnemyGrp::hasDoctor(int curEnemyIdx)
{
    for(int i = 0; i < m_iEnemyCnt; i++)
    {
        if (i == curEnemyIdx)
        {
            continue;
        }
        
        if (m_enemy[i]->getSkill() == NULL)
        {
            continue;
        }
        
        for (int j=0; j<m_enemy[i]->getSkill()->count(); j++)
        {
            if (m_enemy[i]->getSkill()->getObjectAtIndex(j)->getSkillID() == enEnemySkillZhenFuHuo
                && m_enemy[i]->isAlive()
                )
            {
                return true;
            }
        }
    }
    
    return false;
}

//******************************************************************************
// isActionRunning
//******************************************************************************
bool EnemyGrp::isActionRunning()
{
    bool bRet = false;
    
    for(int i = 0; i < m_iEnemyCnt; i++){
        if(m_enemy[i]->isActionRunning()){
            bRet = true;
            break;
        }
    }
    
    if((m_spBox) && (m_spBox->numberOfRunningActions() > 0))
        bRet = true;
    
    return bRet;
}

//******************************************************************************
// attack
//******************************************************************************
void EnemyGrp::attack()
{
    bool hasSickOrPosion = false;
    for (int i=0; i<m_iEnemyCnt;i++)
    {
        if (!m_enemy[i]->isAlive())
            continue;
        
        if (m_enemy[i]->isBuffByIdx(EN_ENEMY_SICK) || m_enemy[i]->isBuffByIdx(EN_ENEMY_POISON) || m_enemy[i]->isBuffByIdx(EN_ENEMY_FREEZE))
        {
            hasSickOrPosion = true;
            break;
        }
    }
    if (hasSickOrPosion)
    {
        for(int i = 0; i < m_iEnemyCnt; i++)
        {
            if(m_enemy[i]->isAlive())
            {
                if(m_enemy[i]->isBuffByIdx(EN_ENEMY_POISON))
                    m_enemy[i]->bePoisoned(m_iPoisonPt);
                
                if(m_enemy[i]->isBuffByIdx(EN_ENEMY_FREEZE))
                    m_enemy[i]->beFreeze(m_iFreezePt);        //....
                
                if(isSick())
                    m_enemy[i]->sickHpDown(m_iSickPt);
            }
        }
        
        
        chkEnemyState();
        chkAim();
        schedule(schedule_selector(EnemyGrp::EnemyAttack), 1.5);
    }
    else
    {
        chkEnemyState();
        chkAim();
        schedule(schedule_selector(EnemyGrp::EnemyAttack), 0.5);
    }
    
}


void EnemyGrp::chkEnemyState()
{
     for(int i = 0; i < m_iEnemyCnt; i++)
     {
         if(m_enemy[i]->isAlive())
             m_enemy[i]->chkRoundSkill();
     }
}

int EnemyGrp::getPoisonPt()
{
    return m_iPoisonPt;
}

void EnemyGrp::setPoisonPt(int nPoisonPt)
{
    m_iPoisonPt = nPoisonPt;
}

void EnemyGrp::setFreezePt(int nFreezePt)
{
    m_iFreezePt = nFreezePt;
}

int EnemyGrp::getSickPt()
{
    return m_iSickPt;
}
//******************************************************************************
// attack
//******************************************************************************
bool EnemyGrp::isAllEnemyDead()
{
    bool bRet = true;
    
    for(int i = 0; i < m_iEnemyCnt; i++){
        if(m_enemy[i]->isAlive()){
            bRet = false;
            break;
        }
    }
    
    return bRet;
}


void EnemyGrp::skillStart()
{
    m_enemy[m_iCurEnemy]->skillStart();
}

void EnemyGrp::skillStart2(){
    FightMngr* tFightM = (FightMngr*)(m_enemy[m_iCurEnemy]->getParent()->getParent());
    Player* tPlayer = tFightM->getPlayer();
    tPlayer->enemyDecBuffCD();
    m_enemy[m_iCurEnemy]->skillStart();
}

//getSkillDetail
const char* EnemyGrp::getSkillDetail(int skillID)
{
    const char * skillDetail;
    
    switch (skillID)
    {
        case enEnemySkill_LianHuan:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_lianhuan_ep");
            break;
        case enEnemySkill_LianDa:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_lianda_ep");
            break;
            
        case enEnemySkill_LianJi82:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_lianji82_ep");
            break;
        case enEnemySkill_LianJi7_3:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_lianji7_3_ep");
            break;
        case enEnemySkill_LianJi45_4:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_lianji45_4_ep");
            break;
        case enEnemySkill_LianJi75_3:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_lianji75_3_ep");
            break;
        case enEnemySkill_LianJi83:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_lianji83_ep");
            break;
            
        case enEnemySkill_WuShuang:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_wushuang_ep");
            break;
        case enEnemySkill_ZhongJi:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_zhongji_ep");
            break;
        case enEnemySkill_QiangXi:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_qiangxi_ep");
            break;
        case enEnemySkill_SuiGu:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_suigu_ep");
            break;
        case enEnemySkillSuiGu2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_suigu_ep2");
            break;
        case enEnemySkillSuiGu3:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_suigu_ep3");
            break;
        case enEnemySkillHuoXiJin:
        case enEnemySkillHuoXiJin40:
        case enEnemySkillHuoXiJin10:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_huoxijin_ep");
            break;
        case enEnemySkillHuoXiJin1_3:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_huoxijin1_3_ep");
            break;
            
        case enEnemySkillShuiXiJin:
        case enEnemySkillShuiXiJin40:
        case enEnemySkillShuiXiJin10:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_shuixijin_ep");
            break;
        case enEnemySkillShuiXiJin1_3:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_shuixijin1_3_ep");
            break;
            
        case enEnemySkillMuXiJin:
        case enEnemySkillMuXiJin40:
        case enEnemySkillMuXiJin10:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_muxijin_ep");
            break;
        case enEnemySkillMuXiJin1_3:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_muxijin1_3_ep");
            break;
            
        case enEnemySkillGuangXiJin:
        case enEnemySkillGuangXiJin40:
        case enEnemySkillGuangXiJin10:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_guangxijin_ep");
            break;
        case enEnemySkillGuangXiJin1_3:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_guangxijin1_3_ep");
            break;
            
        case enEnemySkillAnXiJin:
        case enEnemySkillAnXiJin40:
        case enEnemySkillAnXiJin10:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_anxijin_ep");
            break;
        case enEnemySkillAnXiJin1_3:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_anxijin1_3_ep");
            break;
            
        case enEnemySkillShenJin:
        case enEnemySkillShenJin40:
        case enEnemySkillShenJin10:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_shenxijin_ep");
            break;
        case enEnemySkillShenJin1_3:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_shenxijin1_3_ep");
            break;
            
        case enEnemySkillLuanJin:
        case enEnemySkillLuanJin40:
        case enEnemySkillLuanJin10:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_luanshijin_ep");
            break;
            
        case enEnemySkillLuanJinRun2_3_23:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_luanshijin_run2_3_23_ep");
            break;
            
        case enEnemySkill_113:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_113");
            break;
            
        case enEnemySkill_114:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_114");
            break;
            
        case enEnemySkill_115:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_115");
            break;
            
        case enEnemySkill_116:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_116");
            break;
            
        case enEnemySkill_117:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_117");
            break;

        case enEnemySkill_118:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_118");
            break;
            
        case enEnemySkill_119:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_119");
            break;
            
        case enEnemySkill_120:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_120");
            break;
            
        case enEnemySkill_121:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_121");
            break;
            
        case enEnemySkill_122:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_122");
            break;
            
        case enEnemySkill_123:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_123");
            break;
            
        case enEnemySkill_124:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_124");
            break;
            
        case enEnemySkill_125:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_125");
            break;
            
        case enEnemySkill_126:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_126");
            break;
            
        case enEnemySkill_127:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_127");
            break;
            
        case enEnemySkill_128:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_128");
            break;
            
        case enEnemySkill_129:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_129");
            break;
            
        case enEnemySkill_130:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_130");
            break;
            
        case enEnemySkill_131:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_131");
            break;
        
        case enEnemySkill_132:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_132");
            break;
        
        case enEnemySkill_133:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_133");
            break;
        
        case enEnemySkill_134:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_134");
            break;
            
        case enEnemySkill_135:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_135");
            break;
            
        case enEnemySkill_136:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_136");
            break;
            
        case enEnemySkill_137:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_137");
            break;
            
        case enEnemySkill_138:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_138");
            break;
            
        case enEnemySkill_139:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_139");
            break;
            
        case enEnemySkill_140:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_140");
            break;
            
        case enEnemySkill_141:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_141");
            break;
            
        case enEnemySkill_142:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_142");
            break;
            
        case enEnemySkill_143:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_143");
            break;
            
        case enEnemySkill_144:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_144");
            break;
            
        case enEnemySkill_145:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_145");
            break;
            
        case enEnemySkill_146:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_146");
            break;
            
        case enEnemySkill_147:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_147");
            break;
            
        case enEnemySkill_148:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_148");
            break;
            
        case enEnemySkill_149:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_149");
            break;
            
        case enEnemySkill_150:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_150");
            break;
            
        case enEnemySkill_151:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_151");
            break;
            
        case enEnemySkill_152:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_152");
            break;
            
        case enEnemySkill_153:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_153");
            break;
            
        case enEnemySkill_154:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_154");
            break;
            
        case enEnemySkill_155:
            skillDetail = CGameData::Inst()->getLanguageValue("EnemySkillDsp_155");
            break;
            
        case enEnemySkillLuanJin3_23:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_luanshijin3_23_ep");
            break;
            
        case enEnemySkillZhiYu:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_zhiyu_ep");
            break;
            
        case enEnemySkillFuHuo:
        case enEnemySkillFuHuo2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_fuhuo_ep");
            break;
            
        case enEnemySkillkengdiezhuanshui:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanshui_ep");
            break;
            
        case enEnemySkillkengdiezhuanhuo:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanhuo_ep");
            break;
            
        case enEnemySkillkengdiezhuanmu:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanmu_ep");
            break;
            
        case enEnemySkillkengdiezhuanguang:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanguang_ep");
            break;
            
        case enEnemySkillkengdiezhuanan:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanan_ep");
            break;
            
        case enEnemySkillkengdiezhuanshui2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanshui2_ep");
            break;
            
        case enEnemySkillkengdiezhuanhuo2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanhuo2_ep");
            break;
            
        case enEnemySkillkengdiezhuanmu2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanmu2_ep");
            break;
            
        case enEnemySkillkengdiezhuanguang2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanguang2_ep");
            break;
            
        case enEnemySkillkengdiezhuanan2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanan2_ep");
            break;

            
        case enEnemySkillNiZhuanHuo:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_nizhuanhuo_ep");
            break;
            
        case enEnemySkillNiZhuanShui:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_nizhuanshui_ep");
            break;
            
        case enEnemySkillNiZhuanMu:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_nizhuanmu_ep");
            break;
            
        case enEnemySkillNiZhuanGuang:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_nizhuanguang_ep");
            break;
            
        case enEnemySkillNiZhuanAn:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_nizhuanan_ep");
            break;
            
        case enEnemySkillNiZhuanHuiFu:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_nizhuanhuifu_ep");
            break;
            
        case enEnemySkillBaoZhuYinCang:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_baozhuyincang_ep");
            break;
            
        case enEnemySkillShiguangnizhuan:
            skillDetail = CGameData::Inst()->getLanguageValue("enmeybuff_shiguangnizhuan_ep");
            break;
            
        case enEnemySkillNiZhuanTianXia:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_nizhuantianxia_ep");
            break;
            
        case enEnemySkillNiZhuanTianXia2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_nizhuantianxia2_ep");
            break;
            
        case enEnemySkillWuShengHuYou:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_wushenghuyou_ep");
            break;
            
        case enEnemySkillShenLinBiYou:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_shenlingbiyou_ep");
            break;
            
        case enEnemySkillXuShiDaiFa:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_xushidaifa_ep");
            break;
            
        case enEnemySkillXingYunYiJi:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_xingyunyiji_ep");
            break;
            
        case enEnemySkillJiNu:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_jinu_ep");
            break;
            
        case enEnemySkillJiNu2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_jinu2_ep");
            break;
            
        case enEnemySkillJiNu3:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_jinu3_ep");
            break;
            
        case enEnemySkillKuangBao:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_kuangbao_ep");
            break;
            
        case enEnemySkill_YiShan:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_yishan_ep");
            break;
            
        case enEnemySkillGongJiZiTai:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_gongjizitai_ep");
            break;
            
        case enEnemySkillBaoNu:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_baonu_ep");
            break;
            
        case enEnemySkillBaoZou:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_baozou_ep");
            break;
            
        case enEnemySkillShaShouJian:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_shashoujian_ep");
            break;
            
        case enEnemySkillNanManRuQin:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_nanmanruqin_ep");
            break;
            
        case enEnemySkillNanManRuQin2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_nanmanruqin2_ep");
            break;
            
        case enEnemySkill5lianji:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_lianji5_ep");
            break;
            
        case enEnemySkillEYunYiJi:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_eyunyiji_ep");
            break;
            
        case enEnemySkillluanshitianxia:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_luanshitianxia_ep");
            break;
            
        case enEnemySkillluanshitianxia2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_luanshitianxia2_ep");
            break;
            
        case enEnemySkillZhuQueQi:
        {
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_zhuqueqi_ep");
        }
            break;
            
        case enEnemySkillZhuQueQi2:
        {
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_zhuqueqi2_ep");
        }
            break;
            
        case enEnemySkillZhuQueQi3:
        {
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_zhuqueqi3_ep");
        }
            break;
            
        case enEnemySkillBingFengShiKe:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_bingfengshike_ep");
            break;
            
        case enEnemySkillBingFengShiKe2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_bingfengshike2_ep");
            break;
            
        case enEnemySkillWoHenYing:
        {
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_wohenying_ep");
        }
            break;
            
        case enEnemySkillTaGengYing:
        {
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_tagengying_ep");
        }
            break;
            
        case enEnemySkillshuohua20:
        {
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_shuohua20_ep");
        }
            break;
            
        case enEnemySkillHaoYin:
        case enEnemySkillHaoYin2:
        {
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_haoyin_ep");
        }
            break;
            
        case enEnemySkillShiJianNingJie:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_shijianningjie_ep");
            break;
            
        case enEnemySkillTu:
        {
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_tu_ep");
        }
            break;
            
        case enEnemySkillMengShowLuanWu:
        {
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_mengshouluanwu_ep");
        }
            break;
            
        case enEnemySkillMuTengJia:
        {
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_mutengjia_ep");
        }
            break;
            
        case enEnemySkillMuTengJia2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_mutengjia2_ep");
            break;
            
        case enEnemySkillMuTengJia3:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_jinshuhuo_ep");
            break;
            
        case enEnemySkillQuYan:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_quyan_ep");
            break;
            
        case enEnemySkillDuanLiu:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_duanliu_ep");
            break;
            
        case enEnemySkillPoMu:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_pomu_ep");
            break;
            
        case enEnemySkillZhanLei:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_zhanlei_ep");
            break;
            
        case enEnemySkillShiHun:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_shihun_ep");
            break;
            
        case enEnemySkillZhenFuHuo:
        {
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_zhenfuhuo_ep");
        }
            break;
            
        case enEnemySkillxinjinganghuti:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_newjinganghuti_ep");
            break;
            
        case enEnemySkillxinjinganghuti2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_newjinganghuti2_ep");
            break;
            
        case enEnemySkillTongGuiYuJin:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_tongguiyujin_ep");
            break;
            
        case enEnemySkillZiBao:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_zibao_ep");
            break;
            
        case enEnemySkillZiBao2:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_zibao2_ep");
            break;
            
        case enEnemySkillDianGuangHuoShi:
            skillDetail = CGameData::Inst()->getLanguageValue("enemyskill_dianguanghuoshi_ep");
            break;
            
        default:
        {
//            int nDialogue = -1;
//            if (kMaxEnemyCnt>m_iCurEnemy && m_enemy[m_iCurEnemy])
//            {
//                if (CCMutableArray<EnemySkill*> *pSkillList = m_enemy[m_iCurEnemy]->getSkill())
//                {
//                    unsigned int unCount = pSkillList->count();
//                    for (unsigned int i = 0; i < unCount; ++i)
//                    {
//                        if (EnemySkill *pSkill = pSkillList->getObjectAtIndex(i))
//                        {
//                            if (skillID==pSkill->getSkillID())
//                            {
//                                if (0<pSkill->getDialogueSum())
//                                {
//                                    nDialogue = pSkill->getCurDialogue();
//                                    break;
//                                }
//                            }
//                        }
//                    }
//                }
//            }
//            if (-1!=nDialogue)
//            {
//                char buf[100];
//                snprintf(buf, 99, "EnemySkillDsp_%d_%d",skillID,nDialogue);
//                skillDetail = CGameData::Inst()->getLanguageValue(buf);
//            }
//            else
//            {
                char buf[100];
                snprintf(buf, 99, "EnemySkillDsp_%d",skillID);
                skillDetail = CGameData::Inst()->getLanguageValue(buf);
//            }
        }
            break;
    }
    
    return skillDetail;
}

//getSkillName
const char* EnemyGrp::getSkillName(int skillID)
{
//    CCLog("EnemyGrp::getSkillName skillID = %d", skillID);
    const char * skillname;
    
    switch (skillID)
    {
        case enEnemySkill_LianHuan:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_lianhuan");
            break;
        case enEnemySkill_LianDa:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_lianda");
            break;
            
        case enEnemySkill_LianJi82:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_lianji82");
            break;
            
        case enEnemySkill_LianJi7_3:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_lianji7_3");
            break;
        case enEnemySkill_LianJi45_4:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_lianji45_4");
            break;
        case enEnemySkill_LianJi75_3:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_lianji75_3");
            break;
        case enEnemySkill_LianJi83:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_lianji83");
            break;
            
        case enEnemySkill_WuShuang:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_wushuang");
            break;
        case enEnemySkill_ZhongJi:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_zhongji");
            break;
        case enEnemySkill_QiangXi:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_qiangxi");
            break;
        case enEnemySkill_SuiGu:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_suigu");
            break;
        case enEnemySkillSuiGu2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_suigu2");
            break;
        case enEnemySkillSuiGu3:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_suigu3");
            break;
        case enEnemySkillHuoXiJin:
        case enEnemySkillHuoXiJin40:
        case enEnemySkillHuoXiJin10:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_huoxijin");
            break;
        case enEnemySkillHuoXiJin1_3:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_huoxijin1_3");
            break;
        case enEnemySkillShuiXiJin:
        case enEnemySkillShuiXiJin40:
        case enEnemySkillShuiXiJin10:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_shuixijin");
            break;
        case enEnemySkillShuiXiJin1_3:
            skillname = CGameData::Inst()->getLanguageValue("enEnemySkillShuiXiJin1_3");
            break;
        case enEnemySkillMuXiJin:
        case enEnemySkillMuXiJin40:
        case enEnemySkillMuXiJin10:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_muxijin");
            break;
        case enEnemySkillMuXiJin1_3:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_muxijin1_3");
            break;
            
        case enEnemySkillGuangXiJin:
        case enEnemySkillGuangXiJin40:
        case enEnemySkillGuangXiJin10:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_guangxijin");
            break;
        case enEnemySkillGuangXiJin1_3:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_guangxijin1_3");
            break;
        case enEnemySkillAnXiJin:
        case enEnemySkillAnXiJin40:
        case enEnemySkillAnXiJin10:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_anxijin");
            break;
        case enEnemySkillAnXiJin1_3:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_anxijin1_3");
            break;
            
        case enEnemySkillShenJin:
        case enEnemySkillShenJin40:
        case enEnemySkillShenJin10:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_shenxijin");
            break;
        case enEnemySkillShenJin1_3:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_shenxijin1_3");
            break;
        case enEnemySkillLuanJin:
        case enEnemySkillLuanJin40:
        case enEnemySkillLuanJin10:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_luanshijin");
            break;
            
        case enEnemySkillLuanJinRun2_3_23:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_luanshijin_run2_3_23");
            break;
            
        case enEnemySkill_113:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_113");
            break;
            
        case enEnemySkill_114:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_114");
            break;
            
        case enEnemySkill_115:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_115");
            break;
            
        case enEnemySkill_116:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_116");
            break;
            
        case enEnemySkill_117:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_117");
            break;
            
        case enEnemySkill_118:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_118");
            break;
            
        case enEnemySkill_119:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_119");
            break;
            
        case enEnemySkill_120:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_120");
            break;
            
        case enEnemySkill_121:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_121");
            break;
            
        case enEnemySkill_122:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_122");
            break;
            
        case enEnemySkill_123:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_123");
            break;
            
        case enEnemySkill_124:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_124");
            break;
            
        case enEnemySkill_125:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_125");
            break;
            
        case enEnemySkill_126:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_126");
            break;
            
        case enEnemySkill_127:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_127");
            break;
            
        case enEnemySkill_128:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_128");
            break;
            
        case enEnemySkill_129:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_129");
            break;
            
        case enEnemySkill_130:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_130");
            break;
            
        case enEnemySkill_131:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_131");
            break;
            
        case enEnemySkill_132:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_132");
            break;
            
        case enEnemySkill_133:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_133");
            break;
            
        case enEnemySkill_134:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_134");
            break;
            
        case enEnemySkill_135:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_135");
            break;
            
        case enEnemySkill_136:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_136");
            break;
            
        case enEnemySkill_137:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_137");
            break;
            
        case enEnemySkill_138:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_138");
            break;
            
        case enEnemySkill_139:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_139");
            break;
            
        case enEnemySkill_140:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_140");
            break;
            
        case enEnemySkill_141:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_141");
            break;
            
        case enEnemySkill_142:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_142");
            break;
            
        case enEnemySkill_143:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_143");
            break;
            
        case enEnemySkill_144:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_144");
            break;
            
        case enEnemySkill_145:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_145");
            break;
            
        case enEnemySkill_146:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_146");
            break;
            
        case enEnemySkill_147:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_147");
            break;
            
        case enEnemySkill_148:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_148");
            break;
            
        case enEnemySkill_149:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_149");
            break;
            
        case enEnemySkill_150:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_150");
            break;
            
        case enEnemySkill_151:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_151");
            break;
            
        case enEnemySkill_152:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_152");
            break;
            
        case enEnemySkill_153:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_153");
            break;
            
        case enEnemySkill_154:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_154");
            break;
            
        case enEnemySkill_155:
            skillname = CGameData::Inst()->getLanguageValue("EnemySkillTitle_155");
            break;
            
        case enEnemySkillLuanJin3_23:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_luanshijin3_23");
            break;
            
        case enEnemySkillZhiYu:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_zhiyu");
            break;
            
        case enEnemySkillFuHuo:
        case enEnemySkillFuHuo2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_fuhuo");
            break;
            
        case enEnemySkillkengdiezhuanshui:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanshui");
            break;
            
        case enEnemySkillkengdiezhuanhuo:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanhuo");
            break;
            
        case enEnemySkillkengdiezhuanmu:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanmu");
            break;
            
        case enEnemySkillkengdiezhuanguang:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanguang");
            break;
            
        case enEnemySkillkengdiezhuanan:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanan");
            break;
            
        case enEnemySkillkengdiezhuanshui2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanshui2");
            break;
            
        case enEnemySkillkengdiezhuanhuo2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanhuo2");
            break;
            
        case enEnemySkillkengdiezhuanmu2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanmu2");
            break;
            
        case enEnemySkillkengdiezhuanguang2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanguang2");
            break;
            
        case enEnemySkillkengdiezhuanan2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_kengdiezhuanan2");
            break;
            
        case enEnemySkillNiZhuanHuo:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_nizhuanhuo");
            break;
            
        case enEnemySkillNiZhuanShui:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_nizhuanshui");
            break;
            
        case enEnemySkillNiZhuanMu:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_nizhuanmu");
            break;
            
        case enEnemySkillNiZhuanGuang:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_nizhuanguang");
            break;
            
        case enEnemySkillNiZhuanAn:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_nizhuanan");
            break;
            
        case enEnemySkillNiZhuanHuiFu:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_nizhuanhuifu");
            break;
            
        case enEnemySkillBaoZhuYinCang:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_baozhuyincang");
            break;
            
        case enEnemySkillShiguangnizhuan:
            skillname = CGameData::Inst()->getLanguageValue("enmeybuff_shiguangnizhuan");
            break;
            
        case enEnemySkillNiZhuanTianXia:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_nizhuantianxia");
            break;
            
        case enEnemySkillNiZhuanTianXia2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_nizhuantianxia2");
            break;
            
        case enEnemySkillWuShengHuYou:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_wushenghuyou");
            break;
            
        case enEnemySkillShenLinBiYou:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_shenlingbiyou");
            break;
            
        case enEnemySkillXuShiDaiFa:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_xushidaifa");
            break;
            
        case enEnemySkillXingYunYiJi:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_xingyunyiji");
            break;
            
        case enEnemySkillJiNu:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_jinu");
            break;
            
        case enEnemySkillJiNu2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_jinu2");
            break;
            
        case enEnemySkillJiNu3:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_jinu3");
            break;
            
        case enEnemySkillKuangBao:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_kuangbao");
            break;
            
        case enEnemySkill_YiShan:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_yishan");
            break;
            
        case enEnemySkillGongJiZiTai:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_gongjizitai");
            break;
            
        case enEnemySkillBaoNu:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_baonu");
            break;
            
        case enEnemySkillBaoZou:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_baozou");
            break;
            
        case enEnemySkillShaShouJian:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_shashoujian");
            break;
            
        case enEnemySkillNanManRuQin:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_nanmanruqin");
            break;
            
        case enEnemySkillNanManRuQin2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_nanmanruqin2");
            break;
            
        case enEnemySkill5lianji:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_lianji5");
            break;
            
        case enEnemySkillEYunYiJi:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_eyunyiji");
            break;
            
        case enEnemySkillluanshitianxia:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_luanshitianxia");
            break;
            
        case enEnemySkillluanshitianxia2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_luanshitianxia2");
            break;
            
        case enEnemySkillZhuQueQi:
        {
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_zhuqueqi");
        }
            break;
            
        case enEnemySkillZhuQueQi2:
        {
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_zhuqueqi2");
        }
            break;
            
        case enEnemySkillZhuQueQi3:
        {
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_zhuqueqi3");
        }
            break;
            
        case enEnemySkillBingFengShiKe:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_bingfengshike");
            break;
            
        case enEnemySkillBingFengShiKe2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_bingfengshike2");
            break;
            
        case enEnemySkillWoHenYing:
        {
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_wohenying");
        }
            break;
            
        case enEnemySkillTaGengYing:
        {
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_tagengying");
        }
            break;
            
        case enEnemySkillshuohua20:
        {
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_shuohua20");
        }
            break;
            
        case enEnemySkillHaoYin:
        case enEnemySkillHaoYin2:
        {
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_haoyin");
        }
            break;
            
        case enEnemySkillShiJianNingJie:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_shijianningjie");
            break;
            
        case enEnemySkillTu:
        {
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_tu");
        }
            break;
            
        case enEnemySkillMengShowLuanWu:
        {
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_mengshouluanwu");
        }
            break;
            
        case enEnemySkillMuTengJia:
        {
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_mutengjia");
        }
            break;
            
        case enEnemySkillMuTengJia2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_mutengjia2");
            break;
            
        case enEnemySkillMuTengJia3:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_jinshuhuo");
            break;
            
        case enEnemySkillQuYan:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_quyan");
            break;
            
        case enEnemySkillDuanLiu:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_duanliu");
            break;
            
        case enEnemySkillPoMu:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_pomu");
            break;
            
        case enEnemySkillZhanLei:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_zhanlei");
            break;
            
        case enEnemySkillShiHun:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_shihun");
            break;
            
        case enEnemySkillZhenFuHuo:
        {
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_zhenfuhuo");
        }
            break;
            
        case enEnemySkillxinjinganghuti:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_newjinganghuti");
            break;
            
        case enEnemySkillxinjinganghuti2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_newjinganghuti2");
            break;
            
        case enEnemySkillTongGuiYuJin:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_tongguiyujin");
            break;
            
        case enEnemySkillZiBao:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_zibao");
            break;
            
        case enEnemySkillZiBao2:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_zibao2");
            break;
            
        case enEnemySkillDianGuangHuoShi:
            skillname = CGameData::Inst()->getLanguageValue("enemyskill_dianguanghuoshi");
            break;
            
        default:
        {
            char buf[100];
            snprintf(buf, 99, "EnemySkillTitle_%d",skillID);
            skillname = CGameData::Inst()->getLanguageValue(buf);
        }
            break;
    }

    return skillname;
}

const char* EnemyGrp::getSkillNameSon(int skillID)
{
    int nDialogue = -1;
    if (kMaxEnemyCnt>m_iCurEnemy && m_enemy[m_iCurEnemy])
    {
        if (CCMutableArray<EnemySkill*> *pSkillList = m_enemy[m_iCurEnemy]->getSkill())
        {
            unsigned int unCount = pSkillList->count();
            for (unsigned int i = 0; i < unCount; ++i)
            {
                if (EnemySkill *pSkill = pSkillList->getObjectAtIndex(i))
                {
                    if (skillID==pSkill->getSkillID())
                    {
                        if (0<pSkill->getDialogueSum())
                        {
                            nDialogue = pSkill->getCurDialogue();
                            break;
                        }
                    }
                }
            }
        }
    }
    
    if (-1!=nDialogue)
    {
        char buf[100];
        snprintf(buf, 99, "EnemySkillTitle_%d_%d",skillID,nDialogue);
        return CGameData::Inst()->getLanguageValue(buf);
    }
    return "";
}

void EnemyGrp::showAtkTip(const char* tip)
{
    CCNode* node = getChildByTag(kEnemySkillTag);
    if(node)
        removeChild(node, true);
    
    CCSprite* spEnemySkill = CCSprite::spriteWithFile("enemyskillbg.png");
    if(spEnemySkill)
    {
        addChild(spEnemySkill, 10, kEnemySkillTag);
        spEnemySkill->setPosition(ccp(320, 760 + 80));
        
        TextNode* txt = TextNode::textWithString(tip, CCSizeMake(210, 40), CCTextAlignmentCenter, 40);
        spEnemySkill->addChild(txt);
        txt->setAnchorPoint(ccp(0.5, 0.5));
        txt->setPosition(ccp(spEnemySkill->getContentSize().width * 0.5,
                             spEnemySkill->getContentSize().height * 0.5));
        
        spEnemySkill->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.6),
                                                    CCFadeOut::actionWithDuration(0.3),
                                                    NULL) );
        
        txt->runActionInside(CCSequence::actions(CCDelayTime::actionWithDuration(0.6),
                                                 CCFadeOut::actionWithDuration(0.3),
                                                 NULL) );
    }

}


void EnemyGrp::showSkillTip(Enemy* enemy, int idx)
{
    enemy->runAction(CCSequence::actions(CCBlink::actionWithDuration(0.3, 3),
                                                        CCShow::action(),
                                                        NULL));
    
    const char * skillDetail = getSkillDetail(idx);
    
    if (skillDetail != 0 && strlen(skillDetail)!=0)
    {
        CCNode* node1 = getChildByTag(kEnemySkDetailTag);
        if(node1)
            removeChild(node1, true);
        
        CCSprite* spEnemySkDetail = CCSprite::spriteWithFile("enskilldetail.png");
        if(spEnemySkDetail)
        {
            addChild(spEnemySkDetail, 11, kEnemySkDetailTag);
            spEnemySkDetail->setPosition(ccp(320, 760 + 80));
            
            TextNode* txt = TextNode::textWithString(skillDetail, CCSizeMake(210, 40), CCTextAlignmentCenter, 40);
            spEnemySkDetail->addChild(txt);
            txt->setAnchorPoint(ccp(0.5, 0.5));
            txt->setPosition(ccp(spEnemySkDetail->getContentSize().width * 0.5,
                                 spEnemySkDetail->getContentSize().height * 0.5));
            spEnemySkDetail->setOpacity(0);
            spEnemySkDetail->runAction(CCSequence::actions(
                                                           CCDelayTime::actionWithDuration(0.8),
                                                           CCFadeIn::actionWithDuration(0.1),
                                                           CCDelayTime::actionWithDuration(0.8),
                                                           CCFadeOut::actionWithDuration(0.3),
                                                           NULL) );
            txt->setOpacity(0);
            txt->runActionInside(CCSequence::actions(
                                                     CCDelayTime::actionWithDuration(0.8),
                                                     CCFadeIn::actionWithDuration(0.1),
                                                     CCDelayTime::actionWithDuration(0.8),
                                                     CCFadeOut::actionWithDuration(0.3),
                                                     NULL) );
        }
        
    }

    
    
    //技能提示
    CCNode* node = getChildByTag(kEnemySkillTag);
    if(node)
        removeChild(node, true);
    
    
    CCSprite* spEnemySkill = CCSprite::spriteWithFile("enemyskillbg.png");
    if(spEnemySkill)
    {
        addChild(spEnemySkill, 10, kEnemySkillTag);
        spEnemySkill->setPosition(ccp(320, 760 + 80));
        
        const char * skillname = getSkillName(idx);
        TextNode* txt = TextNode::textWithString(skillname, CCSizeMake(210, 40), CCTextAlignmentCenter, 40);
        spEnemySkill->addChild(txt);
        txt->setAnchorPoint(ccp(0.5, 0.5));
        txt->setPosition(ccp(spEnemySkill->getContentSize().width * 0.5,
                             spEnemySkill->getContentSize().height * 0.5));
        
        spEnemySkill->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.6),
                                                    CCFadeOut::actionWithDuration(0.3),
                                                    NULL) );
        
        txt->runActionInside(CCSequence::actions(CCDelayTime::actionWithDuration(0.6),
                                                 CCFadeOut::actionWithDuration(0.3),
                                                 NULL) );
    }

}

void EnemyGrp::AtkDirectFromEnemy(Enemy* pEnemy ,CAttackInfo atkInfo)
{
    CAttackInfo AtkInfo = atkInfo;
    
    AtkInfo.srcPt = pEnemy->getEnemyPos();
    AtkInfo.iEnemyIdx = pEnemy->getEnemyIndex();
    
    (((FightMngr*)m_pListener)->AtkPlayerDirect(AtkInfo));
}
//******************************************************************************
// cbEnemy
//******************************************************************************
void EnemyGrp::cbEnemy(CCObject* obj)
{
    CAttackInfo* pAtkInfo = (CAttackInfo*)obj;
    
    if (pAtkInfo->bShowSkillEffect)
    {
        const char * skillDetail = getSkillDetail(pAtkInfo->iManSkillID);
        ccTime delayTime = 0.0f;
        if (skillDetail != 0 && strlen(skillDetail)!=0)
        {
            delayTime += 0.3f;
            
            CCNode* node1 = getChildByTag(kEnemySkDetailTag);
            if(node1)
                removeChild(node1, true);
            
            CCSprite* spEnemySkDetail = CCSprite::spriteWithFile("enskilldetail.png");
            if(spEnemySkDetail)
            {
                addChild(spEnemySkDetail, 11, kEnemySkDetailTag);
                spEnemySkDetail->setPosition(ccp(320, 760 + 80));
                
                TextNode* txt = TextNode::textWithString(skillDetail, CCSizeMake(210, 40), CCTextAlignmentCenter, 40);
                spEnemySkDetail->addChild(txt);
                txt->setAnchorPoint(ccp(0.5, 0.5));
                txt->setPosition(ccp(spEnemySkDetail->getContentSize().width * 0.5,
                                     spEnemySkDetail->getContentSize().height * 0.5));
                spEnemySkDetail->setOpacity(0);
                spEnemySkDetail->runAction(CCSequence::actions(
                                                               CCDelayTime::actionWithDuration(0.8),
                                                               CCFadeIn::actionWithDuration(0.1),
                                                               CCDelayTime::actionWithDuration(0.8),
                                                            CCFadeOut::actionWithDuration(0.3),
                                                            NULL) );
                txt->setOpacity(0);
                txt->runActionInside(CCSequence::actions(
                                                         CCDelayTime::actionWithDuration(0.8),
                                                         CCFadeIn::actionWithDuration(0.1),
                                                         CCDelayTime::actionWithDuration(0.8),
                                                         CCFadeOut::actionWithDuration(0.3),
                                                         NULL) );
            }
            
        }
        
        pAtkInfo->bShowSkillEffect = false;
        
        if (pAtkInfo->iManSkillID == enEnemySkill_124||pAtkInfo->iManSkillID == enEnemySkill_123 || pAtkInfo->iManSkillID==enEnemySkill_244) {
            m_enemy[m_iCurEnemy]->runAction(CCSequence::actions(CCBlink::actionWithDuration(0.2, 2),
                                                            CCShow::action(),
                                                            CCDelayTime::actionWithDuration(delayTime),
                                                            NULL));
        }else{
            m_enemy[m_iCurEnemy]->runAction(CCSequence::actions(CCBlink::actionWithDuration(0.2, 2),
                                                                CCShow::action(),
                                                                CCDelayTime::actionWithDuration(delayTime),
                                                                CCCallFunc::actionWithTarget(this, callfunc_selector(EnemyGrp::skillStart)),
                                                                NULL));
        }
        
        //技能提示
        CCNode* node = getChildByTag(kEnemySkillTag);
        if(node)
            removeChild(node, true);
        
        
        CCSprite* spEnemySkill = CCSprite::spriteWithFile("enemyskillbg.png");
        if(spEnemySkill){
            addChild(spEnemySkill, 10, kEnemySkillTag);
            spEnemySkill->setPosition(ccp(320, 760 + 80));
            
            const char * skillname = "";
            skillname = getSkillNameSon(pAtkInfo->iManSkillID);
            if (std::string(skillname).empty())
            {
                skillname = getSkillName(pAtkInfo->iManSkillID);
            }
            
            TextNode* txt = TextNode::textWithString(skillname, CCSizeMake(210, 40), CCTextAlignmentCenter, 40);
            spEnemySkill->addChild(txt);
            txt->setAnchorPoint(ccp(0.5, 0.5));
            txt->setPosition(ccp(spEnemySkill->getContentSize().width * 0.5,
                                 spEnemySkill->getContentSize().height * 0.5));
            
            spEnemySkill->setOpacity(0);
            
            spEnemySkill->runAction(CCSequence::actions(
                                                         CCDelayTime::actionWithDuration(0.2),
                                                        CCFadeIn::actionWithDuration(0.1),
                                                        
                                                        
                                                        CCDelayTime::actionWithDuration(0.6),
                                                        CCFadeOut::actionWithDuration(0.3),
                                                        NULL) );
            txt->setOpacity(0);
            txt->runActionInside(CCSequence::actions(
                                                     CCDelayTime::actionWithDuration(0.2),
                                                     CCFadeIn::actionWithDuration(0.1),
                                                     CCDelayTime::actionWithDuration(0.6),
                                                     CCFadeOut::actionWithDuration(0.3),
                                                     NULL) );
        }
        
        if (pAtkInfo->iManSkillID == enEnemySkill_124||pAtkInfo->iManSkillID == enEnemySkill_123 || pAtkInfo->iManSkillID==enEnemySkill_244) {
            CCNode* node = getChildByTag(kEnemySkillAni);
            if(node)
                removeChild(node, true);
            
            CCNode* node1 = getChildByTag(kEnemySkillAni+1);
            if(node1)
                removeChild(node1, true);
            
            CCSprite* quan = CCSprite::spriteWithFile("qujxiaoquan.png");
            CCSprite* xing = CCSprite::spriteWithFile("qujxiaoxing.png");
            addChild(quan, 5,kEnemySkillAni);
            addChild(xing,5,kEnemySkillAni+1);
            quan->setPosition(m_enemy[m_iCurEnemy]->getEnemyPos());
            xing->setPosition(m_enemy[m_iCurEnemy]->getEnemyPos());
            quan->setOpacity(0);
            xing->setOpacity(0);
            quan->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.2),
                                                CCFadeIn::actionWithDuration(0.1),
                                                CCScaleTo::actionWithDuration(0.3, 0.2),
                                                CCFadeOut::actionWithDuration(0.1),
                                                NULL));
            xing->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.6),
                                                CCFadeIn::actionWithDuration(0.1),
                                                CCSpawn::actionOneTwo(CCScaleBy::actionWithDuration(0.2, 1.5), CCRotateBy::actionWithDuration(0.2, -60)),
                                                CCScaleBy::actionWithDuration(0.1, 100),
                                                CCFadeOut::actionWithDuration(0.2),
                                                CCCallFunc::actionWithTarget(this, callfunc_selector(EnemyGrp::skillStart2)),
                                                NULL));
            
            
        }
        
        return;
    }
    
    
    pAtkInfo->srcPt = m_enemy[m_iCurEnemy]->getEnemyPos();
    pAtkInfo->iElement = m_enemy[m_iCurEnemy]->getElement();
    pAtkInfo->iEnemyIdx = m_iCurEnemy;
    
    m_atkInfo = *pAtkInfo;
    
    if (!pAtkInfo->bAtkOvr)
    {
        
    }
    else if ( m_enemy[m_iCurEnemy]->getEnemyActTimes()>0 )
    {
        pAtkInfo->bAtkOvr =  false;
        schedule(schedule_selector(EnemyGrp::AttackAgain), 0.3);
    }
    else 
    {
        if (getIsEnemyGrpBuffByIdx(enEnemyBuff_shuxinghudun))
        {
            if(m_enemy[m_iCurEnemy]->isAlive() == true)
            m_enemy[m_iCurEnemy]->elementChange();
        }
        
        if (getIsEnemyGrpBuffByIdx(enEnemyBuff_shuxinghudun2))
        {
            if(m_enemy[m_iCurEnemy]->isAlive() == true)
                m_enemy[m_iCurEnemy]->elementChange2();
        }
        
        if(m_iCurEnemy < m_iEnemyCnt)
        {
            pAtkInfo->bAtkOvr =  false;
            m_iCurEnemy++;
            
            if (pAtkInfo->iAttackPt > 0)
            {
                float temptime = 0.3f;
                
                if(pAtkInfo->changeStone)
                    temptime = 1.0f;
                
                schedule(schedule_selector(EnemyGrp::EnemyAttack), temptime);
            }
            else
            {
                EnemyAttack(0);
            }
        }
    }
    
    (m_pListener->*m_pfnSelector)(pAtkInfo);
}

void EnemyGrp::ChangeShuxinghudun()
{
    showAtkTip(CGameData::Inst()->getLanguageValue("enmeybuff_shuxingzhuanhuan"));
    
    CCPoint posBuf = m_EnemyGrpBuff[enEnemyBuff_shuxinghudun].spBuffIcon->getPosition();
    removeChild(m_EnemyGrpBuff[enEnemyBuff_shuxinghudun].spBuffIcon, true);
    m_EnemyGrpBuff[enEnemyBuff_shuxinghudun].spBuffIcon = CCSprite::spriteWithSpriteFrameName(getBufIcon(enEnemyBuff_shuxinghudun));
    addChild(m_EnemyGrpBuff[enEnemyBuff_shuxinghudun].spBuffIcon);
    m_EnemyGrpBuff[enEnemyBuff_shuxinghudun].spBuffIcon->setPosition(posBuf);
}

void EnemyGrp::ChangeShuxinghudun2()
{
    showAtkTip(CGameData::Inst()->getLanguageValue("enmeybuff_name_62"));
    
    CCPoint posBuf = m_EnemyGrpBuff[enEnemyBuff_shuxinghudun2].spBuffIcon->getPosition();
    removeChild(m_EnemyGrpBuff[enEnemyBuff_shuxinghudun2].spBuffIcon, true);
    m_EnemyGrpBuff[enEnemyBuff_shuxinghudun2].spBuffIcon = CCSprite::spriteWithSpriteFrameName(getBufIcon(enEnemyBuff_shuxinghudun2));
    addChild(m_EnemyGrpBuff[enEnemyBuff_shuxinghudun2].spBuffIcon);
    m_EnemyGrpBuff[enEnemyBuff_shuxinghudun2].spBuffIcon->setPosition(posBuf);
}

void EnemyGrp::AttackAgain(ccTime dt)
{
    unschedule(schedule_selector(EnemyGrp::AttackAgain));
    m_enemy[m_iCurEnemy]->AttackAgain();
}

void EnemyGrp::EnemyAttack(ccTime dt)
{

    unschedule(schedule_selector(EnemyGrp::EnemyAttack));
    
    if (m_iCurEnemy<m_iEnemyCnt)
    {
        m_enemy[m_iCurEnemy]->startAttack();
        chkAim();
    }
    else
    {
        m_iCurEnemy = 0;
        m_atkInfo.bAtkOvr = true;
        
        for (int i=0; i<m_iEnemyCnt; i++)
        {
            m_enemy[i]->buffCheckAfterGrp();
        }
        
        (m_pListener->*m_pfnSelector)(&m_atkInfo);
    }
}


//******************************************************************************
// shoot
//******************************************************************************
void EnemyGrp::shoot(const CCPoint& src, const CCPoint& dst, float dt)
{
    CCSprite* bullet = CCSprite::spriteWithSpriteFrameName("bullet.png");
    bullet->setPosition(src);
    bullet->setOpacity(0);
    addChild(bullet);

    CCPoint bezierPos1 = dst;
    CCPoint bezierPos2 = dst;
    bezierPos1.x = (src.x + dst.x) * 0.5;
    bezierPos2.x = (bezierPos1.x + dst.x) * 0.5;
    ccBezierConfig bezier;
    bezier.controlPoint_1 = bezierPos1;
    bezier.controlPoint_2 = bezierPos2;
    bezier.endPosition = dst;
    bullet->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                          CCFadeTo::actionWithDuration(0.1, 255),
                                          CCBezierTo::actionWithDuration(0.2, bezier),
                                          CCFadeOut::actionWithDuration(0.1),
                                          NULL) );

}

//******************************************************************************
// lookupElemOpposite
//******************************************************************************
int EnemyGrp::lookupElemOpposite(CAttackInfo* attackinfo)
{
    int iRet = kInvalidValue;
    int iElem;
    float fFactor = 0.0;
    float fCurFactor = 0.0;
    
    for(int i = 0; i < m_iEnemyCnt; i++){
        if(m_enemy[i]->isAlive() == false)
            continue;
        
        iElem = m_enemy[i]->getElement();
        
        if(attackinfo->iElement >= enElementMax)
            fCurFactor = 1.0;
        else
            fCurFactor = gElemOppositeTbl[attackinfo->iElement][iElem];
        
        if(fCurFactor > fFactor){
            iRet = i;
            fFactor = fCurFactor;
        }
    }
    
    return iRet;
}

int  EnemyGrp::lookupElemOpposite(CAttackInfo*  attackinfo,int nNotIndex)
{
    int iRet = kInvalidValue;
    int iElem;
    float fFactor = 0.0;
    float fCurFactor = 0.0;
    
    for(int i = 0; i < m_iEnemyCnt; i++)
    {
        if (nNotIndex==i)continue;
        if(m_enemy[i]->isAlive() == false)
            continue;
        
        iElem = m_enemy[i]->getElement();
        
        if(attackinfo->iElement >= enElementMax)
            fCurFactor = 1.0;
        else
            fCurFactor = gElemOppositeTbl[attackinfo->iElement][iElem];
        
        if(fCurFactor > fFactor){
            iRet = i;
            fFactor = fCurFactor;
        }
    }
    
    return iRet;
}

int  EnemyGrp::lookupElemOppositeBig(CAttackInfo*  attackinfo,int nBigIndex)
{
    int iRet = kInvalidValue;
    int iElem;
    float fFactor = 0.0;
    float fCurFactor = 0.0;
    
    for(int i = nBigIndex; i < m_iEnemyCnt; i++)
    {
//        if (nBigIndex>i)continue;
        if(m_enemy[i]->isAlive() == false)
            continue;
        
        iElem = m_enemy[i]->getElement();
        
        if(attackinfo->iElement >= enElementMax)
            fCurFactor = 1.0;
        else
            fCurFactor = gElemOppositeTbl[attackinfo->iElement][iElem];
        
        if(fCurFactor > fFactor){
            iRet = i;
            fFactor = fCurFactor;
        }
    }
    
    return iRet;
    
    
    
    
    
    
    
}

//******************************************************************************
// lookupWeakest
//******************************************************************************
int EnemyGrp::lookupWeakest()
{
    int iRet = kInvalidValue;
    float fPercentMin = 2.0;
    
    for(int i = 0; i < m_iEnemyCnt; i++){
        if(m_enemy[i]->isAlive() == false)
            continue;
        
        float fPercent = m_enemy[i]->getCurHp() * 1.0 / m_enemy[i]->getBaseHp();
        
        if(fPercent < fPercentMin){
            fPercentMin = fPercent;
            iRet = i;
        }
    }
    
    return iRet;
}

//******************************************************************************
// dropbox
//******************************************************************************
void EnemyGrp::dropbox(const CCPoint& pt,float dt)
{
    if(m_spBox != NULL){
        removeChild(m_spBox, true);
    }
    
    CCAnimation* animation;
	char frameName[100] = {0};
	CCActionInterval*  action;
    
    animation = CCAnimation::animation();
    for( int i=1;i<5;i++){
        sprintf(frameName, "boxdrop%d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( frameName );
        animation->addFrame(frame);
    }
    action = CCAnimate::actionWithDuration(0.5, animation, false);
    
    m_spBox = CCSprite::spriteWithSpriteFrameName("blank.png");
    addChild(m_spBox);
    m_spBox->setAnchorPoint(CCPointMake(0.5, 0));
    m_spBox->setPosition(CCPointMake(pt.x, kMonsterYPos));
    m_spBox->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                          action,
                                          NULL));
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(boxdrop));
}

//******************************************************************************
// openBox
//******************************************************************************
void EnemyGrp::openBox()
{
    if(m_spBox == NULL)
        return;
    
    //show open box animation
    CCAnimation* animation;
	char frameName[100] = {0};
	CCActionInterval*  action;
    
    animation = CCAnimation::animation();
    for( int i=1;i<24;i++){
        sprintf(frameName, "openbox00%02d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( frameName );
        animation->addFrame(frame);
    }
    action = CCAnimate::actionWithDuration(1.0, animation, false);
    
    m_spBox->runAction(action);
    
    //show money
    char buf[100];
    snprintf(buf, 99, "%d", m_iGoldBox);
    
    CCSprite* spTemp = CCSprite::spriteWithFile("yellownum.png");
    
    CCLabelAtlas* lbGold = CCLabelAtlas::labelWithString(buf, "yellownum.png", 
                                             spTemp->getContentSize().width/10, 
                                             spTemp->getContentSize().height, 
                                             '0');
    
//    lbGold->setAnchorPoint(CCPointMake(0.5, -0.5));
    lbGold->setPosition(m_spBox->getPosition());
    addChild(lbGold);
    lbGold->runAction(CCSpawn::actions(CCMoveBy::actionWithDuration(1.0, CCPointMake(0, 100)),
                                       CCFadeOut::actionWithDuration(1.0),
                                       NULL));
    if(CGameData::Inst()->isSoundOn)
    SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(openbox));
}

//******************************************************************************
// chkBuff      如果爲毒，延時，破甲， 返回true
//******************************************************************************
bool EnemyGrp::chkBuff(CAttackInfo*  attackinfo)
{
    bool bRet = false;
    stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(attackinfo->iManSkillID);
    
    //毒系攻擊
    if((pManSkill) && (pManSkill->iType == enManSkillTp_poison))
    {
        m_iPoisonPt = attackinfo->iAttackPt;
        
        SaveData::Inst()->setPoisonCd(true);
        SaveData::Inst()->setPoisonPoint(m_iPoisonPt);
        
        for(int i = 0; i < m_iEnemyCnt; i++)
            m_enemy[i]->setPoison();
        
        bRet = true;
    }
    else if (pManSkill && (pManSkill->iType == enManSkillTp_EnWuJiangType) && attackinfo->iManSkillID == enManSkill_284){//武将技能284
        m_iPoisonPt =attackinfo->iAttackPt;
        
        SaveData::Inst()->setPoisonCd(true);
        SaveData::Inst()->setPoisonPoint(m_iPoisonPt);
        
        for(int i = 0; i < m_iEnemyCnt; i++)
            m_enemy[i]->setPoison();
        
        bRet = true;

    }
    //延時
    else if((pManSkill) && (pManSkill->iType == enManSkillTp_delay))
    {
        for(int i = 0; i < m_iEnemyCnt; i++)
            m_enemy[i]->setBuffByIdx(EN_ENEMY_DELAY, pManSkill->cContinue, -1);
        bRet = true;
    }
    //破甲
    else if((pManSkill) && (pManSkill->iType == enManSkillTp_break))
    {
        
        for(int i = 0; i < m_iEnemyCnt; i++)
        {
            int iArmor = (1 - pManSkill->fEffect) * m_enemy[i]->getArmor();
            m_enemy[i]->breakArmor(iArmor, pManSkill->cContinue,-1);
        }
                
        bRet = true;
    }
    //麻痹
    else if((pManSkill) && (pManSkill->iType == enManSkillTp_mabi))
    {
        for(int i = 0; i < m_iEnemyCnt; i++)
        {
            if((m_enemy[i]->getElement() == pManSkill->cElement)
               || (pManSkill->cElement == enElementMax))
            {
                m_enemy[i]->setBuffByIdx(EN_ENEMY_MABI, pManSkill->cContinue, -1);
            }
        }
        
        bRet = true;
    }
    //瘟疫
    else if((pManSkill) && (pManSkill->iType == enManSkillTp_sick))
    {
        m_bSick = true;
        m_iSickPt = attackinfo->iAttackPt;
        
        SaveData::Inst()->setSickPoint(m_iSickPt);
        
        for(int i = 0; i < m_iEnemyCnt; i++)
            m_enemy[i]->setBuffByIdx(EN_ENEMY_SICK, pManSkill->cContinue, -1);
        
        bRet = true;
    }
    //改变属性
    else if ((pManSkill) && (pManSkill->iType == enManSkillTp_chgElm))
    {
        for(int i = 0; i < m_iEnemyCnt; i++)
        {
            if (m_enemy[i]->getElement() == pManSkill->cElement)
                continue;
            m_enemy[i]->changeElement(pManSkill->cElement, pManSkill->cContinue, -1);
        }
            
        bRet = true;
    }
    
    return bRet;
}


//******************************************************************************
// isSbDelayed
//******************************************************************************
bool EnemyGrp::isSbDelayed()
{    
    for(int i = 0; i < m_iEnemyCnt; i++)
    {
        if((m_enemy[i]->isAlive()) && (m_enemy[i]->isBuffByIdx(EN_ENEMY_DELAY)))
        {
            return true;
        }
    }
    
    return false;
}


//******************************************************************************
// isSbDelayed
//******************************************************************************
bool EnemyGrp::isArmorBroken()
{
    bool bRet = false;
    
    for(int i = 0; i < m_iEnemyCnt; i++){
        if((m_enemy[i]->isAlive()) && (m_enemy[i]->isBuffByIdx(EN_ENEMY_POJIA)))
        {
            bRet = true;
            break;
        }
    }
    
    return bRet;
}

//******************************************************************************
// isPoison
//******************************************************************************
bool EnemyGrp::isPoison()
{
    for(int i = 0; i < m_iEnemyCnt; i++)
    {
        if((m_enemy[i]->isAlive()) && (m_enemy[i]->isBuffByIdx(EN_ENEMY_POISON)))
        {
            return true;
        }
    }
    
    return false;
}

//******************************************************************************
// isSick
//******************************************************************************
bool EnemyGrp::isSick()
{
    m_bSick = false;
    
    for(int i = 0; i < m_iEnemyCnt; i++)
    {
        if((m_enemy[i]->isAlive()) && (m_enemy[i]->isBuffByIdx(EN_ENEMY_SICK)))
        {
            m_bSick = true;
            break;
        }
    }
    
    return m_bSick;
}

//******************************************************************************
// isChangeElement
//******************************************************************************
bool EnemyGrp::isChangeElement()
{
    bool bRet = false;
    
    for (int i = 0; i < m_iEnemyCnt; i++) {
        if ((m_enemy[i]->isAlive())
            && (m_enemy[i]->isBuffByIdx(EN_ENEMY_CHGELM)))
        {
            bRet = true;
        }
    }
    
    return bRet;
}

//******************************************************************************
// isPoison
//******************************************************************************
bool EnemyGrp::isMabi()
{
    bool bRet = false;
    
    for(int i = 0; i < m_iEnemyCnt; i++){
        if((m_enemy[i]->isAlive()) && (m_enemy[i]->isBuffByIdx(EN_ENEMY_MABI))){
            bRet = true;
            break;
        }
    }
    
    return bRet;
}


//******************************************************************************
// isBuffEffect
//******************************************************************************
bool EnemyGrp::isBuffEffect(enEnemyBuff buffType)
{
    return m_EnemyGrpBuff[buffType].bBuffEffect;
}

//******************************************************************************
// initEnemyPos
//******************************************************************************
void EnemyGrp::initEnemyPos(CDgStepInfo* pStepInfo)
{
    CCMutableArray<CDgMonsterInfo*> * pMonInfoArr = pStepInfo->m_pMonsterArr;
    CDgMonsterInfo* pMonInfo;
    
    CCSprite* sp[6];
    int iTotalWidth = 0;
    int iGap;
    
    for(int i = 0; i < pMonInfoArr->count(); i++){
        pMonInfo = pMonInfoArr->getObjectAtIndex(i);
        
        sp[i] = CGameData::Inst()->getBodySprite(pMonInfo->cid);
        
        iTotalWidth += sp[i]->getContentSize().width;
    }

    iGap = (640 - iTotalWidth) / ((int)pMonInfoArr->count() + 1);

    int iCurX = 0;
    for(int i = 0; i < pMonInfoArr->count(); i++){
        gEnemyPos[i].x = iCurX + iGap + sp[i]->getContentSize().width * 0.5;
        iCurX = gEnemyPos[i].x + sp[i]->getContentSize().width * 0.5;
        gEnemyPos[i].y = kMonsterYPos;
    }
}

void EnemyGrp::initEnemyPos(SaveData_EnemyGrp* pStepInfo)
{
    SaveData_Enemy* pMonInfo;
    
    CCSprite* sp[6];
    int iTotalWidth = 0;
    int iGap;
    
    for(int i = 0; i < pStepInfo->enemycnt; i++){
        pMonInfo = &(pStepInfo->enemies[i]);
        sp[i] = CGameData::Inst()->getBodySprite(pMonInfo->cid);
        
        iTotalWidth += sp[i]->getContentSize().width;
    }
    
    iGap = (640 - iTotalWidth) / (pStepInfo->enemycnt + 1);
    
    int iCurX = 0;
    for(int i = 0; i < pStepInfo->enemycnt; i++){
        gEnemyPos[i].x = iCurX + iGap + sp[i]->getContentSize().width * 0.5;
        iCurX = gEnemyPos[i].x + sp[i]->getContentSize().width * 0.5;
        gEnemyPos[i].y = kMonsterYPos;
    }
}

//******************************************************************************
// collectCard
//******************************************************************************
int EnemyGrp::collectCard()
{
    int iRet = 0;
    
    for(int i = 0; i < m_iEnemyCnt; i++){
        iRet += m_enemy[i]->collectDropItem();
    }
    
    return iRet;
}

//******************************************************************************
// collectGold
//******************************************************************************
int EnemyGrp::collectGold()
{
    int iGold = 0;
    
    for(int i = 0; i < m_iEnemyCnt; i++){
        if(m_enemy[i]->isAlive() == false)
            iGold += m_enemy[i]->getDropGold();
    }
    
    if(isAllEnemyDead() && (m_iGoldBox > 0)){
        iGold += m_iGoldBox;
        openBox();
    }
    
    return iGold;
}


//******************************************************************************
// collectExp
//******************************************************************************
int EnemyGrp::collectExp()
{
    int iExp = 0;
    
    for(int i = 0; i < m_iEnemyCnt; i++){
        if(m_enemy[i]->isAlive() == false)
            iExp += m_enemy[i]->getExp();
    }
    
    return iExp;
}


//******************************************************************************
// haveGoldBox
//******************************************************************************
bool EnemyGrp::haveGoldBox()
{
    if(m_iGoldBox > 0)
        return true;
    else
        return false;
}

//******************************************************************************
// haveEnemyWithElemInstead
//******************************************************************************
bool EnemyGrp::haveEnemyWithElemInstead(int iElem)
{
    bool bRet = false;
    
    for (int i = 0; i < m_iEnemyCnt; i++) {
        if (m_enemy[i]->isAlive()
            && gElemOppositeTbl[iElem][m_enemy[i]->getElement()] >= 2.0)
        {
            bRet = true;
            break;
        }
    }
    
    return bRet;
}

//******************************************************************************
// haveEnemyWithNotElem
//******************************************************************************
bool EnemyGrp::haveEnemyWithNotElem(int iElem)
{
    bool bRet = false;
    
    for (int i = 0; i < m_iEnemyCnt; i++) {
        if ((m_enemy[i]->isAlive())
            && (m_enemy[i]->getElement() != iElem))
        {
            bRet = true;
            break;
        }
    }
    
    return bRet;
}

//******************************************************************************
// haveEnemyWithElem
//******************************************************************************
bool EnemyGrp::haveEnemyWithElem(int iElem)
{
    bool bRet = false;
    
    for(int i = 0; i < m_iEnemyCnt; i++){
        if((m_enemy[i]->isAlive())
           && ((m_enemy[i]->getElement() == iElem)
               || (iElem == enElementMax))){
            bRet = true;
            break;
        }
    }
    
    return bRet;
}

Enemy* EnemyGrp::getEnemyByIdx(int idx)
{
    return m_enemy[idx];
}


//******************************************************************************
// getEnemySkill
//******************************************************************************
//int EnemyGrp::getEnemySkillId(int idx)
//{
//    return m_enemy[idx]->getSkid();
//}

//******************************************************************************
// getEnemyCurHp
//******************************************************************************
int EnemyGrp::getEnemyCurHp(int idx)
{
    return m_enemy[idx]->getCurHp();
}

//******************************************************************************
// getEnemyCurState
//******************************************************************************
int EnemyGrp::getEnemyCurState(int idx)
{
    return m_enemy[idx]->getCurState();
}

//******************************************************************************
// getEnemyCurRound
//******************************************************************************
int EnemyGrp::getEnemyCurRound(int idx)
{
    return m_enemy[idx]->getCurRound();
}

//******************************************************************************
// getEnemyCount
//******************************************************************************
int EnemyGrp::getEnemyCount()
{
    return m_iEnemyCnt;
}

//******************************************************************************
// chkAim
//******************************************************************************
void EnemyGrp::chkAim()
{
    int iCnt = 0;
    for (int i=0; i<m_iEnemyCnt; i++)
    {
        if (m_enemy[i]->isAlive())
            iCnt++;
    }
    
    if (m_iSelected != kInvalidValue)
    {
        if ((m_enemy[m_iSelected]->isAlive() == false)
            || (m_enemy[m_iSelected]->isAlive() == true && iCnt == 1))
        {
            // 当目标敌人死亡, 或着只剩下目标敌人时候, 去掉瞄准光标
            m_iSelected = kInvalidValue;
            SaveData::Inst()->setAimingIndex(kInvalidValue);
            m_spAim->setPosition(CCPointMake(-1000, -1000));
        }
    }
}

//******************************************************************************
// useCanFei
//******************************************************************************
const CCPoint& EnemyGrp::useCanFei()
{
    int enemyIndex = 0;
    for (int i = 0; i < m_iEnemyCnt; i++)
    {
        if (!m_enemy[i]->isAlive())
            continue;
        
        
        for (int j=0; j<ENEMYBUFFMAX; j++)
        {
            if (m_enemy[i]->getBuffId(j) == enEnemyBuff_canfei)
            {
                enemyIndex = i;
                break;
            }
        }
        
        if (enemyIndex == i)
        {
            break;
        }
    }
    
    return  m_enemy[enemyIndex]->getEnemyPos();
}

void EnemyGrp::chageStoneAndAtk(CAttackInfo* pAtkInfo)
{
    
    pAtkInfo->srcPt = m_enemy[m_iCurEnemy]->getEnemyPos();
    pAtkInfo->iElement = m_enemy[m_iCurEnemy]->getElement();
    pAtkInfo->iEnemyIdx = m_iCurEnemy;
    
    m_atkInfo = *pAtkInfo;
    
    
    (((FightMngr*)m_pListener)->changeStoneAndAtk(pAtkInfo));
}


void EnemyGrp::CheckFirstAtk(ccTime dt)
{
    unschedule(schedule_selector(EnemyGrp::CheckFirstAtk));
    
    if (m_iCurEnemy < m_iEnemyCnt)
    {
        
    }
    else
    {
        m_iCurEnemy = 0;
        if (CGameData::Inst()->isNewbie()) {
            
        }else{
            m_atkInfo.clear();
            m_atkInfo.bAtkOvr = true;
            m_atkInfo.fristAtkOver = true;
           (m_pListener->*m_pfnSelector)(&m_atkInfo);
        }
        if (CGameState::Instance()->getState() == enStateGameOver) {
            return;
        }
        
        if(CGameState::Instance()->getState() != enStateTouching)
        CGameState::Instance()->setState(enStateWaitTouch);

        
       
        return;
    }
    
    float durTime = 0.1f;
    
    if (m_enemy[m_iCurEnemy]->isAlive() && m_enemy[m_iCurEnemy]->getSkill() != NULL)
    {
        for (int i=0; i<m_enemy[m_iCurEnemy]->getSkill()->count(); i++)
        {
            enEnemySkill enemySkillId = (enEnemySkill)m_enemy[m_iCurEnemy]->getSkill()->getObjectAtIndex(i)->getSkillID();
            if (enemySkillId == enEnemySkillDianGuangHuoShi // 73  //yes
                || enemySkillId == enEnemySkillQuYan    // 77
                || enemySkillId == enEnemySkillDuanLiu  // 78
                || enemySkillId == enEnemySkillPoMu     // 79
                || enemySkillId == enEnemySkillZhanLei  // 80
                || enemySkillId == enEnemySkillShiHun   // 81
                || enemySkillId == enEnemySkill_128//yes
                || enemySkillId == enEnemySkill_129//yes
                || enemySkillId == enEnemySkill_130//yes
                || enemySkillId == enEnemySkill_156
                || enemySkillId == enEnemySkill_195
                || enemySkillId == enEnemySkill_200
                || enemySkillId == enEnemySkill_205
                || enemySkillId == enEnemySkill_214//yes
                || enemySkillId == enEnemySkill_217
                || enemySkillId == enEnemySkill_221//yes
                || enemySkillId == enEnemySkill_226
                || enemySkillId == enEnemySkill_261//yes
                || enemySkillId == enEnemySkill_272
                || enemySkillId == enEnemySkill_296//yes
                || enemySkillId == enEnemySkill_299//yes
                || enemySkillId == enEnemySkill_308
                || enemySkillId == enEnemySkill_311
                || enemySkillId == enEnemySkill_315//yes
                || enemySkillId == enEnemySkill_318//yes
                || enemySkillId == enEnemySkill_325
                || enemySkillId == enEnemySkill_330//yes
                || enemySkillId == enEnemySkill_333
                || enemySkillId == enEnemySkill_336//yes
                || enemySkillId == enEnemySkill_337
                || enemySkillId == enEnemySkill_340
                || enemySkillId == enEnemySkill_346//yes
                || enemySkillId == enEnemySkill_357//yes
                || enemySkillId == enEnemySkill_361//yes
                || enemySkillId == enEnemySkill_373
                || enemySkillId == enEnemySkill_378
                || enemySkillId == enEnemySkill_390
                || enemySkillId == enEnemySkill_398//yes
                || enemySkillId == enEnemySkill_400
                || enemySkillId == enEnemySkill_421
                || enemySkillId == enEnemySkill_432//yes
                || enemySkillId == enEnemySkill_435
                || enemySkillId == enEnemySkill_444//yes
                || enemySkillId == enEnemySkill_445
                || enemySkillId == enEnemySkill_450//yes
                || enemySkillId == enEnemySkill_455//yes
                || enemySkillId == enEnemySkill_459
                || enemySkillId == enEnemySkill_463
                || enemySkillId == enEnemySkill_500
                || enemySkillId == enEnemySkill_506
                || enemySkillId == enEnemySkill_512
                || enemySkillId == enEnemySkill_528
                )//lvgenquan
            {
                
                // 32	西凉铁壁	有几率使敌将的先制攻击类技能无效	25%(觉醒技)
                FightMngr* fm = (FightMngr*)m_pListener;
                if ((enEnemySkillDianGuangHuoShi == enemySkillId
                    || enEnemySkill_128 == enemySkillId
                    || enEnemySkill_129 == enemySkillId
                    || enEnemySkill_130 == enemySkillId
                    || enEnemySkill_214 == enemySkillId
                    || enEnemySkill_221 == enemySkillId
                    || enEnemySkill_261 == enemySkillId
                    || enEnemySkill_296 == enemySkillId
                    || enEnemySkill_299 == enemySkillId
                    || enEnemySkill_315 == enemySkillId
                    || enEnemySkill_318 == enemySkillId
                    || enEnemySkill_330 == enemySkillId
                    || enEnemySkill_336 == enemySkillId
                    || enEnemySkill_346 == enemySkillId
                    || enEnemySkill_357 == enemySkillId
                    || enEnemySkill_361 == enemySkillId
                    || enEnemySkill_398 == enemySkillId
                    || enEnemySkill_432 == enemySkillId
                    || enEnemySkill_444 == enemySkillId
                    || enEnemySkill_450 == enemySkillId
                    || enEnemySkill_455 == enemySkillId
                    || enemySkillId == enEnemySkill_528)
                    && fm->getPlayer()->haveAwakeSkill_32())
                {
                    m_enemy[m_iCurEnemy]->getSkill()->getObjectAtIndex(i)->set_occurTimes(0);
                }
                
                
                m_enemy[m_iCurEnemy]->getSkill()->getObjectAtIndex(i)->action();
                
                durTime = 0.3f;

                if(m_enemy[m_iCurEnemy]->getSkill()->getObjectAtIndex(i)->getChangeStone())
                durTime = 1.0f;
                
                FightMngr* tFightM = (FightMngr*)m_pListener;
                tFightM->initPlayerRoundVars();
                break;
            }
        }
    }
    m_iCurEnemy++;
    if (m_iCurEnemy <= m_iEnemyCnt)
        schedule(schedule_selector(EnemyGrp::CheckFirstAtk), durTime);
//    else
//        m_iCurEnemy = 0;
}

//******************************************************************************
// updateEnemyBuff
//******************************************************************************
void EnemyGrp::updateEnemyBuff()
{    
    bool bBuffEffect[enEnemyBuff_max];
    for (int i = 0; i < enEnemyBuff_max; i++)
        bBuffEffect[i] = false;
    
    for (int i = 0; i < m_iEnemyCnt; i++) {
        if (!m_enemy[i]->isAlive())
            continue;
        
        for (int j=0; j<ENEMYBUFFMAX; j++)
        {
            enEnemyBuff buffType = (enEnemyBuff)(m_enemy[i]->getBuffId(j));
            if (buffType > enEnemyBuff_null && buffType < enEnemyBuff_max)
            {
                bBuffEffect[buffType] = true;
            }
        }
    }

    // 判断buff是否变化
    bool bBuffChange = false;

    // 无敌buff是否有变化
    bool isWudi = bBuffEffect[enEnemyBuff_wudi];
    if (isWudi != m_EnemyGrpBuff[enEnemyBuff_wudi].bBuffEffect)
    {
        bBuffChange = true;
        m_EnemyGrpBuff[enEnemyBuff_wudi].bBuffEffect = isWudi;

        for (int i = 0; i < m_iEnemyCnt; i++) {
            if (!m_enemy[i]->isAlive())
                continue;

            m_enemy[i]->setWudi(isWudi);
        }
    }
    
    // enEnemyBuff_83
    if (bBuffEffect[enEnemyBuff_83] && m_EnemyGrpBuff[enEnemyBuff_83].spBuffIcon) {
        CCLabelAtlas *lbBuffRound = (CCLabelAtlas *)m_EnemyGrpBuff[enEnemyBuff_83].spBuffIcon->getChildren()->objectAtIndex(0);
        if (lbBuffRound) {
            char buf[50];
            snprintf(buf, 49, "%lu", 10 - SaveData::Inst()->getCurTouchCnt());
            lbBuffRound->setString(buf);

        }
    }
    
    // 其他buff是否有变化
    for (int i = enEnemyBuff_chenmo; i < enEnemyBuff_max; i ++)
    {
        if (bBuffEffect[i] != m_EnemyGrpBuff[i].bBuffEffect)
        {
            bBuffChange = true;
            m_EnemyGrpBuff[i].bBuffEffect = bBuffEffect[i];
            
            if (!m_EnemyGrpBuff[i].bBuffEffect)
            {
                FightMngr* tFightM = (FightMngr*)m_pListener;
                tFightM->removeBuffEffect(i);
            }
            
            if (m_EnemyGrpBuff[i].bBuffEffect)
            {
                FightMngr* tFightM = (FightMngr*)m_pListener;
                tFightM->addBuffEffect(i);
            }
        }
    }

    // 如果buff有变化
    if (bBuffChange)
    {
        for (int i = 0; i < enEnemyBuff_max; i++)
        {
            if (m_EnemyGrpBuff[i].spBuffIcon != NULL)
            {
                m_EnemyGrpBuff[i].spBuffIcon->removeFromParentAndCleanup(true);
                m_EnemyGrpBuff[i].spBuffIcon = NULL;
            }
        }
        
        int buffIndex = 1;
        for (int i = 1; i < enEnemyBuff_max; i++)
        {
            if (m_EnemyGrpBuff[i].bBuffEffect)
            {
                showBuffTip((enEnemyBuff)i, buffIndex);
                showBuffAni((enEnemyBuff)i, buffIndex);
                showBuffIcon((enEnemyBuff)i, buffIndex);
                buffIndex++;
            }
        }
    }
    
}


bool EnemyGrp::getIsEnemyGrpBuffByIdx(int idx)
{
    return m_EnemyGrpBuff[idx].bBuffEffect;
}


const char* EnemyGrp::getBufName(int buffType)
{
    const char * buffName;
    
    switch (buffType)
    {
        case enEnemyBuff_wudi:
            buffName = CGameData::Inst()->getLanguageValue("enmeyBuffId_1");
            break;
            
        case enEnemyBuff_chenmo:
            buffName = CGameData::Inst()->getLanguageValue("enmeyBuffId_2");
            break;
            
        case enEnemyBuff_yanchi:
            buffName = CGameData::Inst()->getLanguageValue("enmeyBuffId_3");
            break;
            
        case enEnemyBuff_zhuanzhutime1:
            buffName = CGameData::Inst()->getLanguageValue("enmeyBuffzhuanzhutime1");
            break;
            
        case enEnemyBuff_zhuanzhutime2:
            buffName = CGameData::Inst()->getLanguageValue("enmeyBuffzhuanzhutime2");
            break;
            
        case enEnemyBuff_63:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_63");
            break;
            
        case enEnemyBuff_canfei:
            buffName = CGameData::Inst()->getLanguageValue("enmeyBuffId_4");
            break;
            
        case enEnemyBuff_mutengjia:
            buffName = CGameData::Inst()->getLanguageValue("enemyskill_mutengjia");
            break;
            
        case enEnemyBuff_64:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_64");
            break;
            
        case enEnemyBuff_65:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_65");
            break;
            
        case enEnemyBuff_ysshenyuan:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_shenyuan");
            break;
            
        case enEnemyBuff_hunyuandun:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_hunyuandun");
            break;
            
        case enEnemyBuff_ystaipingdao:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_taipingdao");
            break;
            
        case enEnemyBuff_aytaipingyaoshu:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_taipingyaoshu");
            break;
            
        case enEnemyBuff_wenyihuanjing:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_wenyihuanjing");
            break;
            
        case enEnemyBuff_wenyihuanjing2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_wenyihuanjing2");
            break;
            
        case enEnemyBuff_wenyihuanjing3:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_wenyihuanjing3");
            break;
            
        case enEnemyBuff_wenyi:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_wenyi");
            break;
            
        case enEnemyBuff_wenyi2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_wenyi2");
            break;
            
        case enEnemyBuff_shenfa:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_shenfa");
            break;
            
        case enEnemyBuff_1combodun:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_1combodun");
            break;
            
        case enEnemyBuff_2combodun:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_2combodun");
            break;
            
        case enEnemyBuff_3combodun:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_3combodun");
            break;
            
        case enEnemyBuff_4combodun:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_4combodun");
            break;
            
        case enEnemyBuff_5combodun:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_5combodun");
            break;
            
        case enEnemyBuff_6combodun:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_6combodun");
            break;
            
        case enEnemyBuff_1combodun2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_1combodun2");
            break;
            
        case enEnemyBuff_2combodun2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_2combodun2");
            break;
            
        case enEnemyBuff_3combodun2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_3combodun2");
            break;
            
        case enEnemyBuff_4combodun2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_4combodun2");
            break;
            
        case enEnemyBuff_5combodun2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_5combodun2");
            break;
            
        case enEnemyBuff_6combodun2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_6combodun2");
            break;
            
        case enEnemyBuff_combodun2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_combodun2");
            break;
            
        case enEnemyBuff_combodun22:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_combodun22");
            break;
            
        case enEnemyBuff_combodun3:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_combodun3");
            break;
            
        case enEnemyBuff_combodun32:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_combodun32");
            break;
            
        case enEnemyBuff_1lianbufan:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_1beilianfan");
            break;
            
        case enEnemyBuff_2lianbufan:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_2beilianfan");
            break;
            
        case enEnemyBuff_3lianbufan:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_3beilianfan");
            break;
            
        case enEnemyBuff_4lianbufan:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_4beilianfan");
            break;
            
        case enEnemyBuff_5lianbufan:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_5beilianfan");
            break;
            
        case enEnemyBuff_6lianbufan:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_6beilianfan");
            break;
            
        case enEnemyBuff_7lianbufan:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_7beilianfan");
            break;
            
        case enEnemyBuff_8lianbufan:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_8beilianfan");
            break;
            
        case enEnemyBuff_9lianbufan:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_9beilianfan");
            break;
            
        case enEnemyBuff_10lianbufan:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_10beilianfan");
            break;
            
        case enEnemyBuff_1lianbufan2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_1beilianfan2");
            break;
            
        case enEnemyBuff_2lianbufan2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_2beilianfan2");
            break;
            
        case enEnemyBuff_3lianbufan2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_3beilianfan2");
            break;
            
        case enEnemyBuff_4lianbufan2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_4beilianfan2");
            break;
            
        case enEnemyBuff_5lianbufan2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_5beilianfan2");
            break;
            
        case enEnemyBuff_6lianbufan2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_6beilianfan2");
            break;
            
        case enEnemyBuff_7lianbufan2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_7beilianfan2");
            break;
            
        case enEnemyBuff_8lianbufan2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_8beilianfan2");
            break;
            
        case enEnemyBuff_9lianbufan2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_9beilianfan2");
            break;
            
        case enEnemyBuff_10lianbufan2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_10beilianfan2");
            break;
            
        case enEnemyBuff_liushuifanshi:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_liushuifanshi");
            break;
            
        case enEnemyBuff_shihunhuo:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_shihunhuo");
            break;
            
        case enEnemyBuff_shihunshui:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_shihunshui");
            break;
            
        case enEnemyBuff_shihunmu:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_shihunmu");
            break;
            
        case enEnemyBuff_shihunguang:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_shihunguang");
            break;
            
        case enEnemyBuff_shihunan:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_shihunan");
            break;
            
        case enEnemyBuff_shengguangdun:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_shengguangdun");
            break;
            
        case enEnemyBuff_shuxinghudun:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_shuxinghudun");
            break;
            
        case enEnemyBuff_shuxinghudun2:
            buffName = CGameData::Inst()->getLanguageValue("enmeybuff_name_62");
            break;
        
        case enEnemyBuff_66:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_66");
            break;
        
        case enEnemyBuff_67:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_67");
            break;
        
        case enEnemyBuff_68:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_68");
            break;
        
        case enEnemyBuff_69:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_69");
            break;
        
        case enEnemyBuff_70:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_70");
            break;
        
        case enEnemyBuff_71:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_71");
            break;
            
        case enEnemyBuff_72:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_72");
            break;
        
        case enEnemyBuff_73:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_73");
            break;
        
        case enEnemyBuff_74:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_74");
            break;
        
        case enEnemyBuff_75:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_75");
            break;
        
        case enEnemyBuff_76:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_76");
            break;
            
        case enEnemyBuff_77:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_77");
            break;
            
        case enEnemyBuff_78:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_78");
            break;
            
        case enEnemyBuff_79:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_79");
            break;
            
        case enEnemyBuff_80:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_80");
            break;
            
        case enEnemyBuff_81:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_81");
            break;
            
        case enEnemyBuff_82:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_82");
            break;
            
        case enEnemyBuff_83:
            buffName = CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_83");
            break;
            
        default:
        {
            char buf[100];
            snprintf(buf, 99, "enmeybuff_name_%d",buffType);
            buffName = CGameData::Inst()->getLanguageValue(buf);
        }
            break;
    }
    
    return buffName;
}


//******************************************************************************
// showBuffTip
//******************************************************************************
void EnemyGrp::showBuffTip(enEnemyBuff buffType, int index)
{
    //技能提示
    CCNode* node = getChildByTag(kEnemySkillTag);
    if(node)
    {
        removeChild(node, true);
    }
    
    CCSprite* spEnemySkill = CCSprite::spriteWithFile("enemyskillbg.png");
    if(spEnemySkill){
        addChild(spEnemySkill, 10, kEnemySkillTag+index);
        spEnemySkill->setPosition(ccp(320, 760));
        spEnemySkill->setIsVisible(false);
        TextNode* txt = TextNode::textWithString(getBufName(buffType),
                                                 CCSizeMake(210, 40),
                                                 CCTextAlignmentCenter,
                                                 40);
        spEnemySkill->addChild(txt);
        txt->setAnchorPoint(ccp(0.5, 0.5));
        txt->setPosition(ccp(spEnemySkill->getContentSize().width * 0.5,
                             spEnemySkill->getContentSize().height * 0.5));
        
        spEnemySkill->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.8 + 2.0 * (index-1)),
                                                    CCShow::action(),
                                                    CCFadeOut::actionWithDuration(1.2),
                                                    CCHide::action(),
                                                    CCCallFuncND::actionWithTarget(this, callfuncND_selector(EnemyGrp::rmBuffTip),(void*)(kEnemySkillTag+index)),
                                                    NULL) );
        
        txt->runActionInside(CCSequence::actions(CCDelayTime::actionWithDuration(0.8 + 2.0 * (index-1)),
                                                 CCFadeOut::actionWithDuration(1.2),
                                                 NULL) );
    }
}

//******************************************************************************
// rmBuffTip
//******************************************************************************
void EnemyGrp::rmBuffTip(CCObject* obj,int tag)
{
    if (getChildByTag(tag))
    removeChildByTag(tag, true);
}

//******************************************************************************
// showBuffAni
//******************************************************************************
void EnemyGrp::showBuffAni(enEnemyBuff buffType, int index)
{
    CGameData::Inst()->addBigImageByRGBA4444("superII.plist");
    
    CCAnimation* animation;
	char frameName[100] = {0};
	CCActionInterval*  action;
    CCPoint posEnemy;
    
    for (int ii = 0; ii < m_iEnemyCnt; ii++) {
        if (!m_enemy[ii]->isAlive())
            continue;
        
        animation = CCAnimation::animation();
        for(int jj = 1; jj <= 8; jj++){
            sprintf(frameName, "superII00%02d.png", jj);
            
            CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName );
            animation->addFrame(frame);
        }
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
        animation->addFrame(frame);
        action = CCAnimate::actionWithDuration(0.5, animation, false);
        
        CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("blank.png");
        addChild(spAnim);
        spAnim->setPosition(m_enemy[ii]->getEnemyPos());
        spAnim->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.1 + 2.0 * (index-1)),
                                              action,
                                              NULL));
    }
}


const char* EnemyGrp::getBufIcon(int buffType)
{
    const char * buffIcon;

    switch (buffType)
    {
        case enEnemyBuff_wudi:
            buffIcon = "icon_wudi.png";
            break;
            
        case enEnemyBuff_chenmo:
            buffIcon = "icon_chenmo.png";
            break;
            
        case enEnemyBuff_yanchi:
            buffIcon = "icon_yanchi.png";
            break;
            
        case enEnemyBuff_zhuanzhutime1:
        case enEnemyBuff_zhuanzhutime2:
        case enEnemyBuff_63:
            buffIcon = "xianzhizhuantime.png";
            break;
            
        case enEnemyBuff_canfei:
            buffIcon = "icon_canfei.png";
            break;
            
        case enEnemyBuff_mutengjia:
            buffIcon = "mutengjia.png";
            break;
            
        case enEnemyBuff_64:
            buffIcon = "shentypebuff2.png";
            break;
            
        case enEnemyBuff_65:
            buffIcon = "mutengjia.png";
            break;
            
        case enEnemyBuff_66:
            buffIcon = "mutengjia.png";
            break;
            
        case enEnemyBuff_ysshenyuan:
            buffIcon = "yaoshushenyuan.png";
            break;
            
        case enEnemyBuff_hunyuandun:
            buffIcon = "hunyuandun.png";
            break;
            
        case enEnemyBuff_ystaipingdao:
            buffIcon = "yaoshutaipingdao.png";
            break;
            
        case enEnemyBuff_aytaipingyaoshu:
            buffIcon = "aytaipingyaoshu.png";
            break;
            
        case enEnemyBuff_wenyihuanjing:
        case enEnemyBuff_wenyihuanjing2:
        case enEnemyBuff_wenyihuanjing3:
            buffIcon = "wenyihuanjing.png";
            break;
            
        case enEnemyBuff_wenyi:
        case enEnemyBuff_wenyi2:
            buffIcon = "wenyihuanjing.png";
            break;
            
        case enEnemyBuff_shenfa:
            buffIcon = "shenfa.png";
            break;
            
        case enEnemyBuff_1combodun:
        case enEnemyBuff_2combodun:
        case enEnemyBuff_3combodun:
        case enEnemyBuff_4combodun:
        case enEnemyBuff_5combodun:
        case enEnemyBuff_6combodun:
            buffIcon = "combodun.png";
            break;
            
        case enEnemyBuff_1combodun2:
        case enEnemyBuff_2combodun2:
        case enEnemyBuff_3combodun2:
        case enEnemyBuff_4combodun2:
        case enEnemyBuff_5combodun2:
        case enEnemyBuff_6combodun2:
        case enEnemyBuff_69:
            buffIcon = "combodun.png";
            break;
            
        case enEnemyBuff_combodun2:
            buffIcon = "combodun.png";
            break;
            
        case enEnemyBuff_combodun22:
            buffIcon = "combodun.png";
            break;
            
        case enEnemyBuff_combodun3:
            buffIcon = "combodun.png";
            break;
            
        case enEnemyBuff_combodun32:
            buffIcon = "combodun.png";
            break;
            
        case enEnemyBuff_1lianbufan:
        case enEnemyBuff_2lianbufan:
        case enEnemyBuff_3lianbufan:
        case enEnemyBuff_4lianbufan:
        case enEnemyBuff_5lianbufan:
        case enEnemyBuff_6lianbufan:
        case enEnemyBuff_7lianbufan:
        case enEnemyBuff_8lianbufan:
        case enEnemyBuff_9lianbufan:
        case enEnemyBuff_10lianbufan:
            buffIcon = "combobeifan.png";
            break;
            
        case enEnemyBuff_1lianbufan2:
        case enEnemyBuff_2lianbufan2:
        case enEnemyBuff_3lianbufan2:
        case enEnemyBuff_4lianbufan2:
        case enEnemyBuff_5lianbufan2:
        case enEnemyBuff_6lianbufan2:
        case enEnemyBuff_7lianbufan2:
        case enEnemyBuff_8lianbufan2:
        case enEnemyBuff_9lianbufan2:
        case enEnemyBuff_10lianbufan2:
        case enEnemyBuff_68:
            buffIcon = "combobeifan.png";
            break;
            
        case enEnemyBuff_liushuifanshi:
            buffIcon = "liushuifanshi.png";
            break;
            
        case enEnemyBuff_shihunhuo:
            buffIcon = "shihunhuo.png";
            break;
            
        case enEnemyBuff_shihunshui:
            buffIcon = "shihunshui.png";
            break;
            
        case enEnemyBuff_shihunmu:
            buffIcon = "shihunmu.png";
            break;
            
        case enEnemyBuff_shihunguang:
            buffIcon = "shihunguang.png";
            break;
            
        case enEnemyBuff_shihunan:
            buffIcon = "shihunan.png";
            break;
            
        case enEnemyBuff_shengguangdun:
            buffIcon = "shengguangdun.png";
            break;
            
        case enEnemyBuff_shuxinghudun:
            buffIcon = "yaoshumu.png";
            for (int i=0; i<m_iEnemyCnt; i++)
            {
                if (m_enemy[i]->isAlive())
                {
                    char buf[30];
                    snprintf(buf, 29, "stone%ds.png", m_enemy[i]->getElement());
                    buffIcon = buf;
                    break;
                }
            }
            break;
            
        case enEnemyBuff_shuxinghudun2:
            buffIcon = "yaoshumu.png";
            for (int i=0; i<m_iEnemyCnt; i++)
            {
                if (m_enemy[i]->isAlive())
                {
                    char buf[30];
                    snprintf(buf, 29, "stone%ds.png", m_enemy[i]->getElement());
                    buffIcon = buf;
                    break;
                }
            }
            break;
            
        case enEnemyBuff_70:
            buffIcon = "shengguangdun.png";
            break;
            
        case enEnemyBuff_71:
        case enEnemyBuff_75:
            buffIcon = "buffIcon_changeZhuJiang.png";
            break;
            
        case enEnemyBuff_72:
        case enEnemyBuff_81:
            buffIcon = "buffIcon_SuiJiFengYin.png";
            break;
            
        case enEnemyBuff_73:
        case enEnemyBuff_76:
            buffIcon = "buffIcon_PlusAttack.png";
            break;
            
        case enEnemyBuff_74:
        case enEnemyBuff_79:
            buffIcon = "buffIcon_toElementInvalid.png";
            break;
            
        default:
            buffIcon = "buffIcon_PlusAttack.png";
            break;
    }
    
    return buffIcon;
}


int EnemyGrp::getComboDunNum()
{
    return m_iComboDunNum;
}

void EnemyGrp::setComboDunNum(int num)
{
    m_iComboDunNum = num;
    
    if (m_EnemyGrpBuff[enEnemyBuff_combodun2].bBuffEffect)
    {
        CCLabelAtlas* lbBuffRound = (CCLabelAtlas*)(m_EnemyGrpBuff[enEnemyBuff_combodun2].spBuffIcon->getChildByTag(130905));
        
        char buf[50];
        snprintf(buf, 49, "%d",num);
        lbBuffRound->setString(buf);
    }
    
    if (m_EnemyGrpBuff[enEnemyBuff_combodun22].bBuffEffect)
    {
        CCLabelAtlas* lbBuffRound = (CCLabelAtlas*)(m_EnemyGrpBuff[enEnemyBuff_combodun22].spBuffIcon->getChildByTag(130905));
        
        char buf[50];
        snprintf(buf, 49, "%d",num);
        lbBuffRound->setString(buf);
    }
    
    if (m_EnemyGrpBuff[enEnemyBuff_combodun3].bBuffEffect)
    {
        CCLabelAtlas* lbBuffRound = (CCLabelAtlas*)(m_EnemyGrpBuff[enEnemyBuff_combodun3].spBuffIcon->getChildByTag(130905));
        
        char buf[50];
        snprintf(buf, 49, "%d",num);
        lbBuffRound->setString(buf);
    }
    
    if (m_EnemyGrpBuff[enEnemyBuff_combodun32].bBuffEffect)
    {
        CCLabelAtlas* lbBuffRound = (CCLabelAtlas*)(m_EnemyGrpBuff[enEnemyBuff_combodun32].spBuffIcon->getChildByTag(130905));
        
        char buf[50];
        snprintf(buf, 49, "%d",num);
        lbBuffRound->setString(buf);
    }
}

int EnemyGrp::getAliveCount()
{
    int nSum = 0;
    for (int i = 0; i < m_iEnemyCnt; ++i)
    {
        if (m_enemy[i] && m_enemy[i]->isAlive())
        {
            ++nSum;
        }
    }
    return nSum;
}

//******************************************************************************
// showBuffIcon
//******************************************************************************
void EnemyGrp::showBuffIcon(enEnemyBuff buffType, int index)
{
    if (m_EnemyGrpBuff[buffType].spBuffIcon != NULL)
        return;
    if (buffType >= enEnemyBuff_71 && buffType <= enEnemyBuff_83) {  // 新给了小图片，没有整合到大图中去，所以直接 spriteWithFile
        m_EnemyGrpBuff[buffType].spBuffIcon = CCSprite::spriteWithFile(getBufIcon(buffType));
       
    } else {
        m_EnemyGrpBuff[buffType].spBuffIcon = CCSprite::spriteWithSpriteFrameName(getBufIcon(buffType));
      
    }
    int num = -1;
    
    if (buffType >=  enEnemyBuff_1lianbufan && buffType <= enEnemyBuff_10lianbufan2)
    {
        num = (int)(buffType - enEnemyBuff_1lianbufan +1);
        num = (num%10);
        num = num == 0 ? 10: num;
    }
    
    if (buffType >= enEnemyBuff_1combodun && buffType <= enEnemyBuff_6combodun)
        num = (int)(buffType - enEnemyBuff_1combodun +1);
    
    if (buffType >= enEnemyBuff_1combodun2 && buffType <= enEnemyBuff_6combodun2)
        num = (int)(buffType - enEnemyBuff_1combodun2 +1);
    
    if (buffType == enEnemyBuff_combodun2 || buffType ==enEnemyBuff_combodun22
        || buffType == enEnemyBuff_combodun3 || buffType ==enEnemyBuff_combodun32
        )
    {
        num = (SaveData::Inst()->getrand()%4)+3;
        m_iComboDunNum = num;
    }
    
    if (buffType == enEnemyBuff_83) {
        num = 10 - SaveData::Inst()->getCurTouchCnt();
    }
    
    if (num > 0)
    {
        char buf[50];
        snprintf(buf, 49, "%d", num);
        CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
        CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
        lbBuffRound->setPosition(ccp(-4,0));
        m_EnemyGrpBuff[buffType].spBuffIcon->addChild(lbBuffRound,0,130905);
        lbBuffRound->setAnchorPoint(CCPointMake(1, 0));
    }
    
    addChild(m_EnemyGrpBuff[buffType].spBuffIcon);
    m_EnemyGrpBuff[buffType].spBuffIcon->setPosition(CCPointMake(620, 700 + 40 * (index-1)));
    m_EnemyGrpBuff[buffType].spBuffIcon->setIsVisible(false);
    
    m_EnemyGrpBuff[buffType].spBuffIcon->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.6 + 2.0 * (index-1)),
                                                                       CCShow::action(),
                                                                       CCScaleTo::actionWithDuration(0.2, 1.2),
                                                                       CCScaleTo::actionWithDuration(0.2, 1.0),
                                                                       NULL));
}


void EnemyGrp::recoverEnemyBlood(Enemy* pEnemy)
{
    for (int i = 0; i < kMaxEnemyCnt; i++) {
        if (m_enemy[i] == NULL || m_enemy[i] == pEnemy) {
            continue;
        }
        if (m_enemy[i]->isAlive() == true) {
            m_enemy[i]->changeCurHPAction(m_enemy[i]->getBaseHp(), 0.2);
        }
    }
}


//******************************************************************************
// onExit
//******************************************************************************
void EnemyGrp::onExit()
{
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("superII.plist");
    CCLayer::onExit();
}

void EnemyGrp::changeEnemy_huihe(int num)
{
    for (int i = 0 ; i < m_iEnemyCnt; i++) {
        m_enemy[i]->changeEnemyCount(num);
    }
}




