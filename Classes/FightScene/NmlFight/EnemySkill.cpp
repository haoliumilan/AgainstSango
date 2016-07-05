//
//  EnemySkill.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 13-3-15.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "Enemy.h"
#include "EnemySkill.h"
#include "CSkillLib.h"
#include "SaveData.h"
#include "Player.h"
#include "FightMngr.h"

EnemySkillPool* EnemySkillPool::s_pcIf = NULL;


#pragma mark -
#pragma mark 同归于尽
EsTongGuiYuJin::EsTongGuiYuJin(int iSkId, Enemy* pEnemy)
{
    m_nOccurOdds = -1;
    m_BaseAtkPt = 0;
    m_iSkId = iSkId;
    m_pEnemy = pEnemy;
    switch (iSkId)
    {
        case enEnemySkillTongGuiYuJin:
            m_BaseAtkPt = 999.0f;
            break;
        case enEnemySkill_509:
        case enEnemySkill_519:
        case enEnemySkill_524:
            m_BaseAtkPt = 9999.0f;
            break;
        case enEnemySkill_520:
            m_BaseAtkPt = 6666.0f;
            break;
        case enEnemySkill_521:
            m_BaseAtkPt = 7777.0f;
            break;
         
        case enEnemySkillDianGuangHuoShi:
            m_BaseAtkPt = 1200.0f;
            m_occurTimes = 1;
            break;
            
        case enEnemySkill_128:
        {
            if (pEnemy)
            {
                m_BaseAtkPt = pEnemy->getAtkPt();
                m_occurTimes = 1;
            }
            break;
        }
        case enEnemySkill_129:
        {
            if (pEnemy)
            {
                m_BaseAtkPt = pEnemy->getAtkPt()*0.5f;
                m_occurTimes = 1;
            }
            break;
        }
        case enEnemySkill_130:
        {
            if (pEnemy)
            {
                m_BaseAtkPt = pEnemy->getAtkPt()*0.75f;
                m_occurTimes = 1;
            }
            break;
        }
        case enEnemySkill_214:
        case enEnemySkill_221:
        {
            if (pEnemy)
            {
                m_BaseAtkPt = pEnemy->getAtkPt();
                m_occurTimes = 1;
            }
            break;
        }
        case enEnemySkill_261:
        {
            if (pEnemy)
            {

                m_BaseAtkPt = pEnemy->getAtkPt()*0.5f;
                m_occurTimes = 1;
            }
            break;
        }
        case enEnemySkill_195:
        case enEnemySkill_200:
        case enEnemySkill_205:
        case enEnemySkill_337://lvgenquan
        case enEnemySkill_445:
        case enEnemySkill_421:
            case enEnemySkill_506:
        {
            m_BaseAtkPt = 0;
            m_occurTimes = 1;
            m_nOccurOdds = 100;
            break;
        }
        case enEnemySkillZiBao:
        case enEnemySkillZiBao2:
 
            break;
        case enEnemySkill_330:{//lvgenquan
            if (pEnemy) {
                
                m_BaseAtkPt = pEnemy->getAtkPt()*0.3f;
                m_occurTimes = 1;
               
            }
            break;
        }
        case enEnemySkill_336:
        {
            if (pEnemy)
            {
                
                m_BaseAtkPt = pEnemy->getAtkPt()*0.5f;
                m_occurTimes = 1;
            }
            break;
        }
        case enEnemySkill_333:
        case enEnemySkill_500:
               m_occurTimes = 1;
            
                break;
        case enEnemySkill_346:{
            if (pEnemy) {
                m_BaseAtkPt = pEnemy->getAtkPt()*0.1f;
                m_occurTimes = 1;
            }
            break;
        }
        case enEnemySkill_315:
        {
            if (pEnemy)
            {
                m_BaseAtkPt = pEnemy->getAtkPt()*0.2f;
                m_occurTimes = 1;
            }
            break;
        }
        
        case enEnemySkill_308:
        case  enEnemySkill_296:
        case enEnemySkill_299:
        case enEnemySkill_318:
            case enEnemySkill_378:
            case enEnemySkill_400:
            case enEnemySkill_459:
        case enEnemySkill_512:
            m_occurTimes = 1;
            break;
        case enEnemySkill_357:{
            if (pEnemy)
            {
                m_BaseAtkPt = pEnemy->getAtkPt()*0.60f;
                m_occurTimes = 1;
            }

        }
            break;
            case enEnemySkill_361:
        {
            if (pEnemy)
            {
                m_BaseAtkPt = pEnemy->getAtkPt()*1.50f;
                m_occurTimes = 1;
            }
        }
            break;
            case enEnemySkill_398:
            case enEnemySkill_432:
        {
            if (pEnemy)
            {
                m_BaseAtkPt = pEnemy->getAtkPt()*0.80f;
                m_occurTimes = 1;
            }

            break;
        }
            case enEnemySkill_444:
            case enEnemySkill_450:
            case enEnemySkill_455:
        {
            if (pEnemy)
            {
                m_BaseAtkPt = pEnemy->getAtkPt()*0.50f;
                m_occurTimes = 1;
            }
            break;
        }
       case enEnemySkill_373:
        {
             m_nOccurOdds = 100;
            m_ChangeStone = true;
        }
            break;
            case enEnemySkill_528:
        {
            m_BaseAtkPt = 10000;
            m_occurTimes = 1;
        }
            break;
            default:
            break;
    }
    
}

bool EsTongGuiYuJin::canExecute()
{
    ++m_runCounts;
    if (enEnemySkill_373 == m_iSkId) {
        m_isAtkNum--;
        return 0 == m_runCounts % 2;
        
    }
    
    if (EnemySkill::canExecute())
    {
       
        if (0<m_occurTimes)
        {
            return true;
        }
    }
    return false;
}

void EsTongGuiYuJin::action()
{
    switch (m_iSkId)
    {
        case enEnemySkillDianGuangHuoShi:
        case enEnemySkill_128:
        case enEnemySkill_129:
        case enEnemySkill_130:
        case enEnemySkill_214:
        case enEnemySkill_221:
        case enEnemySkill_261:
        case enEnemySkill_195:
        case enEnemySkill_200:
        case enEnemySkill_205:
        case enEnemySkill_330: //lvgenquan
        case enEnemySkill_336:
        case enEnemySkill_337:
        case enEnemySkill_333:
        case enEnemySkill_346:
        case enEnemySkill_308:
        case enEnemySkill_315:
        case enEnemySkill_296://lvgenquan
        case enEnemySkill_299:
        case enEnemySkill_318:
        case enEnemySkill_357:
        case enEnemySkill_361:
        case enEnemySkill_378:
        case enEnemySkill_400:
        case enEnemySkill_459:
        case enEnemySkill_445:
        case enEnemySkill_421:
        case enEnemySkill_398:
        case enEnemySkill_432:
        case enEnemySkill_444:
        case enEnemySkill_450:
        case enEnemySkill_455:
        case enEnemySkill_500:
            case enEnemySkill_506:
            case enEnemySkill_512:
            if (m_occurTimes <= 0)
            {
                return;
            }
            m_occurTimes--;
            break;
        case enEnemySkillZiBao:
        case enEnemySkillZiBao2:
            m_BaseAtkPt = m_pEnemy->getCurHp();
            if (m_BaseAtkPt < 1.0f)
                m_BaseAtkPt = 1.0f;
            break;
        default:
            break;
    }
    if (!(enEnemySkill_195 == m_iSkId
          || enEnemySkill_200 == m_iSkId
          || enEnemySkill_205 == m_iSkId
          || enEnemySkill_337 == m_iSkId
          || enEnemySkill_445 == m_iSkId
          || enEnemySkill_421 == m_iSkId))//lvgenquan
    {
        EnemyGrp* pEnemyGrp = (EnemyGrp*)(m_pEnemy->getParent());
        pEnemyGrp->showSkillTip(m_pEnemy, m_iSkId);
    }

    if (enEnemySkill_296 == m_iSkId) {
        FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
        Player* tPlayer = tFightM->getPlayer();
        int  iAtk = tPlayer->getCurHp()*0.50f;
        if (iAtk >= tPlayer->getCurHp())
            iAtk = tPlayer->getCurHp()-1;
        m_BaseAtkPt = iAtk;
    }
    
    if (enEnemySkill_299 == m_iSkId
        || enEnemySkill_318 == m_iSkId) {
        FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
        Player* tPlayer = tFightM->getPlayer();
        int  iAtk = tPlayer->getCurHp()*0.99f;
        if (iAtk >= tPlayer->getCurHp())
            iAtk = tPlayer->getCurHp()-1;
        m_BaseAtkPt = iAtk;
    }
    
    if (enEnemySkill_506 == m_iSkId) {
        FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
        Player* tPlayer = tFightM->getPlayer();
        int curhp = tPlayer->getCurHp();
        int basehp = tPlayer->getBaseHP();
        tPlayer->setBlood(curhp*0.5, basehp*0.5);
    }
    if (enEnemySkill_512 == m_iSkId) {

        FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
        Player* tPlayer = tFightM->getPlayer();
        tPlayer->delayPlaySkillCD(5);
        
    }
    
    shootAtk(NULL);
}

void EsTongGuiYuJin::shootAtk(CCObject* pSender)
{
    if (0<m_BaseAtkPt)
    {
        CAttackInfo atkInfo;
        atkInfo.clear();
        atkInfo.iAttackPt = m_BaseAtkPt;
        atkInfo.iElement = m_pEnemy->getElement();
        atkInfo.iManSkillID =  m_iSkId;
     
        EnemyGrp* pEnemyGrp = (EnemyGrp*)(m_pEnemy->getParent());
     //   pEnemyGrp->AtkDirectFromEnemy(m_pEnemy,atkInfo);
        m_pEnemy->setAtkInfo(atkInfo);
        pEnemyGrp->cbEnemy(&atkInfo);
        
        if (enEnemySkill_519 == m_iSkId) {
            pEnemyGrp->recoverEnemyBlood(m_pEnemy);
        }
    }
    if (enEnemySkill_333 == m_iSkId
        || enEnemySkill_346 == m_iSkId
        || enEnemySkill_308 == m_iSkId
        ||enEnemySkill_378 == m_iSkId
        || enEnemySkill_400 == m_iSkId
        || enEnemySkill_459 == m_iSkId
        ||  enEnemySkill_500 == m_iSkId) {
        if ( EnemyGrp* pEnemyGrp = (EnemyGrp*)(m_pEnemy->getParent()))
        {
            CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
            atkInfo.iManSkillID = m_iSkId;
            atkInfo.iAttackPt = m_BaseAtkPt > 0 ? m_BaseAtkPt : m_pEnemy->getAtkPt()*1.0f;
            m_pEnemy->setAtkInfo(atkInfo);
            pEnemyGrp->cbEnemy(&atkInfo);
        }
    } 
    if (enEnemySkill_373 == m_iSkId) {
        if ( EnemyGrp* pEnemyGrp = (EnemyGrp*)(m_pEnemy->getParent()))
        {
            CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
            atkInfo.iManSkillID = m_iSkId;
            atkInfo.iAttackPt = m_BaseAtkPt > 0 ? m_BaseAtkPt : m_pEnemy->getAtkPt()*0.80f;
           
            if (m_isAtkNum == 1) {
                atkInfo.bAtkOvr = true;
                m_pEnemy->setAtkInfo(atkInfo);
                pEnemyGrp->chageStoneAndAtk(&atkInfo);
              //  pEnemyGrp->cbEnemy(&atkInfo);
                
            }else{
                atkInfo.bAtkOvr = true;
                m_pEnemy->setAtkInfo(atkInfo);
                m_pEnemy->sentAtkInfo();
            }
            
            
        }
        
    }
    if (enEnemySkill_195==m_iSkId
        || enEnemySkill_200==m_iSkId
        || enEnemySkill_205==m_iSkId
        || enEnemySkill_337 == m_iSkId
        || enEnemySkill_445 == m_iSkId
        || enEnemySkill_421 == m_iSkId) //lvgenquan
    {
        if (FightMngr *pFightMngr = dynamic_cast<FightMngr*>(m_pEnemy->getParent()->getParent()))
        {
            if (Player *pPlayer = pFightMngr->getPlayer())
            {
                //pPlayer->revive();
                int nAddBlood = pPlayer->getBaseHP()-pPlayer->getCurHp();
                if (0<nAddBlood)
                {
                    EnemyGrp* pEnemyGrp = (EnemyGrp*)(m_pEnemy->getParent());
                    pEnemyGrp->showSkillTip(m_pEnemy, m_iSkId);
                    pPlayer->addBlood(nAddBlood);
                }
            }
        }
    }
    
    
    
}

#pragma mark -
#pragma mark 连环
EsLianHuan::EsLianHuan(int iSkId, Enemy* pEnemy)
{
    m_BaseAtkPt = 0.0f;
   // m_bIsRunning = false;
//    m_bIsExclusive = false;
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    m_iHitCnt = LIANHUAN_HITCNT;
    m_fAtkFactor = 1.0;
    m_fSkillDur = 0.2f;
}

int EsLianHuan::getOccurOdds()
{
    //血量大于50%
    if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.5) {
        m_nOccurOdds = 20;
    }
    //血量 30%～50%
    else if(m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.3) {
        m_nOccurOdds = 40;
    }
    //血量30%以下
    else{
        m_nOccurOdds = 80;
    }
    
    return m_nOccurOdds;
}

void EsLianHuan::action()
{
  //  m_bIsRunning = true;
    ++m_runCounts;
    unschedule(schedule_selector(EsLianHuan::action));
    schedule(schedule_selector(EsLianHuan::action), m_fSkillDur);
    
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    if (m_iHitCnt == LIANHUAN_HITCNT)
    {
        m_BaseAtkPt = atkInfo.iAttackPt;
    }
    atkInfo.iAttackPt = m_BaseAtkPt*m_fAtkFactor;
    atkInfo.iManSkillID = m_iSkId;
    
    m_iHitCnt--;
    
    if (m_iHitCnt <= 0)
    {
        m_iHitCnt = LIANHUAN_HITCNT;
        unschedule(schedule_selector(EsLianHuan::action));
        atkInfo.bAtkOvr = true;
    }
    
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}


#pragma mark -
#pragma mark 连打
EsLianDa::EsLianDa(int iSkId, Enemy* pEnemy)
{
    m_BaseAtkPt = 0.0f;
  //  m_bIsRunning = false;
 //   m_bIsExclusive = false;
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    m_fSkillDur = 0.2f;
    
    if (enEnemySkill_115 == iSkId)
    {
        m_iHitCnt = LIANJI5_HITCNT;
        m_fAtkFactor = 0.4f;
        m_fSkillDur = 0.15f;
    }
    else
    if (iSkId == enEnemySkillEYunYiJi)
    {
        m_fAtkFactor = 0.3f;
        m_fSkillDur = 0.15f;
    }
    else if (iSkId == enEnemySkill5lianji)
    {
        m_iHitCnt = LIANJI5_HITCNT;
        m_fAtkFactor = 0.36f;
        m_fSkillDur = 0.15f;
    }
    else if (iSkId == enEnemySkillNanManRuQin
             || iSkId == enEnemySkillNanManRuQin2
             )
    {
        m_iHitCnt = NANMANRUQIN_HITCNT;
        m_fAtkFactor = 0.43f;
        m_fSkillDur = 0.15f;
    }
    else if (enEnemySkillluanshitianxia == iSkId || enEnemySkillluanshitianxia2 == iSkId)
    {
        m_iHitCnt = LUANSHITIANXIA_HITCNT;
        m_fAtkFactor = 0.25f;
        m_fSkillDur = 0.15f;
    }
    else if (iSkId == enEnemySkill_LianJi82)
    {
        m_iHitCnt = LIANJI82_HITCNT;
        m_fAtkFactor = 0.8f;
        m_fSkillDur = 0.20f;
    }
    else if (iSkId == enEnemySkill_LianJi83)
    {
        m_iHitCnt = LIANJI83_HITCNT;
        m_fAtkFactor = 1.0f;
        m_fSkillDur = 0.15f;
    }
    else if (iSkId == enEnemySkill_LianJi7_3)
    {
        m_iHitCnt = LIANJI73_HITCNT;
        m_fAtkFactor = 0.7f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 75;
    }
    else if (iSkId == enEnemySkill_LianJi45_4)
    {
        m_iHitCnt = LIANJI454_HITCNT;
        m_fAtkFactor = 0.45f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 75;
    }
    else if (iSkId == enEnemySkill_LianJi75_3)
    {
        m_iHitCnt = LIANJI753_HITCNT;
        m_fAtkFactor = 0.75f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
    }
    else if (enEnemySkill_119==iSkId)
    {
        m_iHitCnt = LIANJI119_HITCNT;
        m_fAtkFactor = 0.3f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_125==iSkId)
    {
        m_iHitCnt = LIANJI125_HITCNT;
        m_fAtkFactor = 1.8f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_132==iSkId)
    {
        m_iHitCnt = LIANJI132_HITCNT;
        m_fAtkFactor = 0.3f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_141==iSkId)
    {
        m_iHitCnt = LIANJI141_HITCNT;
        m_fAtkFactor = 0.2f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
        
    }
    else if (enEnemySkill_143==iSkId)
    {
        m_iHitCnt = LIANJI143_HITCNT;
        m_fAtkFactor = 0.25f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
        
    }
    else if (enEnemySkill_147==iSkId)
    {
        m_fAtkFactor = 0.3f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
    }
    else if (enEnemySkill_151==iSkId)
    {
        m_fAtkFactor = 0.25f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_154==iSkId)
    {
        m_fAtkFactor = 0.4f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
    }
    else if (enEnemySkill_158==iSkId)
    {
        m_fAtkFactor = 0.5f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 60;
    }
    else if (enEnemySkill_163==iSkId)
    {
        m_fAtkFactor = 0.4f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
    }
    else if (enEnemySkill_164==iSkId)
    {
        m_iHitCnt = LIANJI164_HITCNT;
        m_fAtkFactor = 1.f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_166==iSkId)
    {
        m_iHitCnt = LIANJI166_HITCNT;
        m_fAtkFactor = 0.6f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 50;
    }
    else if (enEnemySkill_170==iSkId)
    {
        m_fAtkFactor = 0.3f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 75;
    }
    else if (enEnemySkill_194==iSkId)
    {
        m_iHitCnt = LIANJI194_HITCNT;
        m_fAtkFactor = 1.0f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_unDialogueSum = 3;
    }
    else if (enEnemySkill_197==iSkId)
    {
        m_fAtkFactor = 0.25f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_208==iSkId)
    {
        m_iHitCnt = LIANJI208_HITCNT;
        m_fAtkFactor = 0.2f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
    }
    else if (enEnemySkill_210==iSkId)
    {
        m_iHitCnt = LIANJI210_HITCNT;
        m_fAtkFactor = 1.0f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        
    }
    else if (enEnemySkill_215==iSkId)
    {
        m_iHitCnt = LIANJI215_HITCNT;
        m_fAtkFactor = 0.4f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
    }
    else if (enEnemySkill_218==iSkId)
    {
        m_iHitCnt = LIANJI218_HITCNT;
        m_fAtkFactor = 0.5f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_220==iSkId)
    {
        m_iHitCnt = LIANJI220_HITCNT;
        m_fAtkFactor = 0.75f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_occurTimes = 1;
    }
    else if (enEnemySkill_222==iSkId)
    {
        m_iHitCnt = LIANJI222_HITCNT;
        m_fAtkFactor = 0.5f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_228==iSkId)
    {
        m_iHitCnt = LIANJI228_HITCNT;
        m_fAtkFactor = 0.45f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
    }
    else if (enEnemySkill_225==iSkId)
    {
        m_fAtkFactor = 0.3f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_239==iSkId)
    {
        m_iHitCnt = LIANJI239_HIYCNT;
        m_fAtkFactor = 0.2f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_246==iSkId)
    {
        m_iHitCnt = LIANJI246_HIYCNT;
        m_fAtkFactor = 0.6f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_248==iSkId)
    {
        m_iHitCnt = LIANJI248_HIYCNT;
        m_fAtkFactor = 0.85f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        
    }
    else if (enEnemySkill_253==iSkId)
    {
        m_fAtkFactor = 0.3f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
    }
    else if (enEnemySkill_267==iSkId)
    {
        m_iHitCnt = LIANJI267_HIYCNT;
        m_fAtkFactor = 2.0f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_268==iSkId)
    {
        m_iHitCnt = LIANJI268_HIYCNT;
        m_fAtkFactor = 0.5f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_269==iSkId)
    {
        m_iHitCnt = LIANJI269_HIYCNT;
        m_fAtkFactor = 0.7f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
    }
    else if (enEnemySkill_271==iSkId)
    {
        m_fAtkFactor = 0.3f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
    }
    else if (enEnemySkill_275==iSkId)
    {
        m_iHitCnt = LIANJI275_HIYCNT;
        m_fAtkFactor = 1.0f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_276==iSkId)
    {
        m_iHitCnt = LIANJI276_HIYCNT;
        m_fAtkFactor = 0.75f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 50;
    }
    else if (enEnemySkill_277==iSkId)
    {
        m_fAtkFactor = 0.3f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 50;
    }
    else if (enEnemySkill_285==iSkId)
    {
        m_iHitCnt = LIANJI285_HIYCNT;
        m_fAtkFactor = 0.7f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_292 == iSkId)//lvgenquan
    {
        m_iHitCnt = LIANJI292_HIYCNT;
        m_fAtkFactor = 0.7f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 40;
    }
    else if (enEnemySkill_300 == iSkId)//lvgenquan
    {
       
        m_fAtkFactor = 0.5f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_345 == iSkId)
    {
        m_iHitCnt = LIANJI345_HIYCNT;
        m_fAtkFactor = 1.0f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_350 == iSkId)
    {
        m_iHitCnt = LIANJI350_HIYCNT;
        m_fAtkFactor = 0.7f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_351 == iSkId)
    {
        m_iHitCnt = LIANJI351_HIYCNT;
        m_fAtkFactor = 1.0f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_302 == iSkId)
    {
        m_fAtkFactor = 0.30f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_307 == iSkId)
    {
        m_fAtkFactor = 0.25f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
        m_iHitCnt =LIANJI307_HIYCNT;
    }
    else if (enEnemySkill_309 == iSkId)
    {
        m_fAtkFactor = 0.65f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
        m_iHitCnt =LIANJI309_HIYCNT;
    }
    else if (enEnemySkill_312 == iSkId)
    {
        m_fAtkFactor = 2.0f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI312_HIYCNT;
         m_unDialogueSum = 3;
    }
    
    else if (enEnemySkill_320 == iSkId)
    {
        m_fAtkFactor = 0.20f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    
    }
    else if (enEnemySkill_329 == iSkId)
    {
        m_fAtkFactor = 0.15f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI329_HIYCNT;
    }
    else if (enEnemySkill_354 == iSkId)
    {
        m_fAtkFactor = 0.70f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 10;
        m_iHitCnt =LIANJI354_HIYCNT;
    }
    else if (enEnemySkill_358 == iSkId)
    {
        m_fAtkFactor = 0.35f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
       
    }
    else if (enEnemySkill_364 == iSkId)
    {
        m_fAtkFactor = 0.50f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI364_HIYCNT;
    }
    else if (enEnemySkill_370 == iSkId)
    {
        m_fAtkFactor = 0.70f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 10;
        m_iHitCnt =LIANJI370_HIYCNT;
    }
    else if (enEnemySkill_380 == iSkId)
    {
        m_fAtkFactor = 0.60f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
        m_iHitCnt =LIANJI380_HIYCNT;
    }
    else if (enEnemySkill_382 == iSkId)
    {
        m_fAtkFactor = 0.30f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 50;
        m_iHitCnt =LIANJI382_HIYCNT;
    }
    else if (enEnemySkill_388 == iSkId)
    {
        m_fAtkFactor = 0.80f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
        m_iHitCnt =LIANJI388_HIYCNT;
    }
    else if (enEnemySkill_389 == iSkId)
    {
        m_fAtkFactor = 0.30f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
        m_iHitCnt =LIANJI389_HIYCNT;
    }
    else if (enEnemySkill_392 == iSkId)
    {
        m_fAtkFactor = 0.70f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
        m_iHitCnt =LIANJI392_HIYCNT;
    }

    else if (enEnemySkill_399 == iSkId)
    {
        m_fAtkFactor = 0.40f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 70;
        m_iHitCnt =LIANJI399_HIYCNT;
    }
    else if (enEnemySkill_403 == iSkId)
    {
        m_fAtkFactor = 0.55f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI403_HIYCNT;
    }
    else if (enEnemySkill_407 == iSkId)
    {
        m_fAtkFactor = 0.65f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI407_HIYCNT;
    }
    else if (enEnemySkill_418 == iSkId)
    {
        m_fAtkFactor = 2.0f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI418_HIYCNT;
    }
    else if (enEnemySkill_423 == iSkId)
    {
        m_fAtkFactor = 0.40f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI423_HIYCNT;
    }
    else if (enEnemySkill_429 == iSkId)
    {
        m_fAtkFactor = 0.30f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI429_HIYCNT;
    }
    else if (enEnemySkill_431 == iSkId)
    {
        m_fAtkFactor = 0.50f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 50;
        m_iHitCnt =LIANJI431_HIYCNT;
    }
    else if (enEnemySkill_433 == iSkId)
    {
        m_fAtkFactor = 0.40f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 50;
        m_iHitCnt =LIANJI433_HIYCNT;
    }
    else if (enEnemySkill_448 == iSkId)
    {
        m_fAtkFactor = 1.0f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 40;
        m_iHitCnt =LIANJI448_HIYCNT;
    }
    else if (enEnemySkill_451 == iSkId)
    {
        m_fAtkFactor = 0.50f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI451_HIYCNT;
    }
    else if (enEnemySkill_454 == iSkId)
    {
        m_fAtkFactor = 0.75f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI454_HIYCNT;
    }
    else if (enEnemySkill_466 == iSkId)
    {
        m_fAtkFactor = 0.20f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI466_HIYCNT;
    }
    else if (enEnemySkill_467 == iSkId)
    {
        m_fAtkFactor = 3.25f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI467_HIYCNT;
    }
    else if (enEnemySkill_474 == iSkId)
    {
        m_fAtkFactor = 0.70f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 10;
        m_iHitCnt =LIANJI474_HIYCNT;
    }
    else if (enEnemySkill_481 == iSkId)
    {
        m_fAtkFactor = 0.50f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI481_HIYCNT;
    }
    else if (enEnemySkill_483 == iSkId)
    {
        m_fAtkFactor = 1.40f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI483_HIYCNT;
    }
    else if (enEnemySkill_485 == iSkId)
    {
        m_fAtkFactor = 0.25f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI485_HIYCNT;
    }
    else if (enEnemySkill_487 == iSkId)
    {
        m_fAtkFactor = 0.70f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        
    }
    else if (enEnemySkill_489 == iSkId)
    {
        m_fAtkFactor = 0.20f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI489_HIYCNT;
    }
    else if (enEnemySkill_491 == iSkId)
    {
        m_fAtkFactor = 0.50f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI491_HIYCNT;
    }
    else if (enEnemySkill_495 == iSkId)
    {
        m_fAtkFactor = 0.70f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI495_HIYCNT;
    }
    else if (enEnemySkill_497 == iSkId)
    {
        m_fAtkFactor = 0.25f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 50;
        m_iHitCnt =LIANJI497_HIYCNT;
    }
    else if (enEnemySkill_499 == iSkId)
    {
        m_fAtkFactor = 0.90f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI499_HIYCNT;
    }
    else if (enEnemySkill_513 == iSkId)
    {
       m_fAtkFactor = 1.0f;
        m_fSkillDur = 0.2;
        m_nOccurOdds = 100;
        m_iHitCnt =LIANJI513_HIYCNT;
    }
    else if (enEnemySkill_523 == iSkId
             || enEnemySkill_527 == iSkId)
    {
        m_fAtkFactor = 1.0f;
        m_fSkillDur = 0.2f;
        m_nOccurOdds = 50;
    }
    else if (enEnemySkill_529 == iSkId)
    {
        m_fAtkFactor = 1.0f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_530 == iSkId)
    {
        m_fAtkFactor = 1.0f;
        m_fSkillDur = 0.20f;
        m_nOccurOdds = 100;
        m_iHitCnt = LIANJI530_HIYCNT;
    }
       else
    {
        m_iHitCnt = LIANDA_HITCNT;
        m_fAtkFactor = 0.7f;
    }
}

int EsLianDa::getOccurOdds()
{
    switch (m_iSkId)
    {
        case enEnemySkill5lianji:
            //血量大于50%
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.5)
            {
                m_nOccurOdds = 20;
            }
            else
            {
                m_nOccurOdds = 40;
            }
            break;
            
        case enEnemySkillNanManRuQin2:
            m_nOccurOdds = 80;
            break;
        case enEnemySkill_LianJi7_3:
        case enEnemySkill_LianJi45_4:
        case enEnemySkill_LianJi75_3:
        case enEnemySkill_119:
        case enEnemySkill_125:
        case enEnemySkill_132:
        case enEnemySkill_141:
        case enEnemySkill_143:
        case enEnemySkill_147:
        case enEnemySkill_151:
        case enEnemySkill_154:
        case enEnemySkill_158:
        case enEnemySkill_163:
        case enEnemySkill_164:
        case enEnemySkill_166:
        case enEnemySkill_170:
        case enEnemySkill_194:
        case enEnemySkill_197:
        case enEnemySkill_208:
        case enEnemySkill_210:
        case enEnemySkill_215:
        case enEnemySkill_218:
        case enEnemySkill_220:
        case enEnemySkill_222:
        case enEnemySkill_228:
        case enEnemySkill_225:
        case enEnemySkill_239:
        case enEnemySkill_246:
        case enEnemySkill_248:
        case enEnemySkill_253:
        case enEnemySkill_267:
        case enEnemySkill_268:
        case enEnemySkill_269:
        case enEnemySkill_271:
        case enEnemySkill_275:
        case enEnemySkill_276:
        case enEnemySkill_277:
        case enEnemySkill_285:
        case enEnemySkill_292: //lvgenquan
        case enEnemySkill_300: //lvgenquan
        case enEnemySkill_345:
        case enEnemySkill_354:
        case enEnemySkill_350:
        case enEnemySkill_351:
        case enEnemySkill_358:
        case enEnemySkill_302:
        case enEnemySkill_307:
        case enEnemySkill_309:
        case enEnemySkill_312:
        case enEnemySkill_320:
        case enEnemySkill_329:
        case enEnemySkill_364:
        case enEnemySkill_370:
        case enEnemySkill_380:
        case enEnemySkill_382:
        case enEnemySkill_388:
        case enEnemySkill_389:
        case enEnemySkill_392:
        case enEnemySkill_399:
        case enEnemySkill_403:
        case enEnemySkill_407:
        case enEnemySkill_418:
        case enEnemySkill_423:
        case enEnemySkill_429:
        case enEnemySkill_431:
        case enEnemySkill_433:
        case enEnemySkill_448:
        case enEnemySkill_451:
        case enEnemySkill_454:
        case enEnemySkill_466:
        case enEnemySkill_467:
        case enEnemySkill_474:
        case enEnemySkill_481:
        case enEnemySkill_483:
        case enEnemySkill_485:
        case enEnemySkill_487:
        case enEnemySkill_489:
        case enEnemySkill_491:
        case enEnemySkill_495:
        case enEnemySkill_497:
        case enEnemySkill_499:
            case enEnemySkill_513:
            case enEnemySkill_523:
            case enEnemySkill_527:
            case enEnemySkill_529:
            case enEnemySkill_530:
        {
            break;
        }
        default:
            //血量大于50%
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.5)
            {
                m_nOccurOdds = 20;
            }
            //血量 30%～50%
            else if(m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.3)
            {
                m_nOccurOdds = 40;
            }
            //血量30%以下
            else{
                m_nOccurOdds = 80;
            }
            
            break;
    }
    
    return m_nOccurOdds;
}

bool EsLianDa::canExecute()
{
    ++m_runCounts;
    bool ret = SaveData::Inst()->getrand() < getOccurOdds();
    if (m_iSkId == enEnemySkillEYunYiJi)
    {
        m_iMaxHitCnt = SaveData::Inst()->getrand()%5 + 2;
        m_iHitCnt = m_iMaxHitCnt;
    }
    else if (enEnemySkill_LianJi7_3 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f);
    }
    else if (enEnemySkill_LianJi45_4 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75f);
    }
    else if (enEnemySkill_LianJi75_3 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75f);
    }
    else if (enEnemySkill_115 == m_iSkId)
    {
        if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.49)
        {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_116)
                return true;
            else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_115)
                return false;
            else
                return true;
        }
        else
            return false;
    }
    else if (enEnemySkill_119==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.3f);
    }
    else if (enEnemySkill_125==m_iSkId)
    {
        return ret && 0<m_runCounts && 0==m_runCounts%3;
    }
    else if (enEnemySkill_132==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.3f);
    }
    else if (enEnemySkill_141==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75f);
    }
    else if (enEnemySkill_143==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75f &&
                       m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.3f);
    }
    else if (enEnemySkill_147==m_iSkId)
    {
        m_iMaxHitCnt = SaveData::Inst()->getrand()%4 + 3;
        m_iHitCnt = m_iMaxHitCnt;
    }
    else if (enEnemySkill_151==m_iSkId)
    {
        m_iMaxHitCnt = SaveData::Inst()->getrand()%3 + 3;
        m_iHitCnt = m_iMaxHitCnt;
    }
    else if (enEnemySkill_154==m_iSkId)
    {
        if (ret && m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f) {
            m_iMaxHitCnt = SaveData::Inst()->getrand()%4 + 3;
            m_iHitCnt = m_iMaxHitCnt;
            return true;
        }else
            return false;
    }
    else if (enEnemySkill_158==m_iSkId)
    {
        if (ret && m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.3f) {
            m_iMaxHitCnt = SaveData::Inst()->getrand()%4 + 2;
            m_iHitCnt = m_iMaxHitCnt;
            return true;
        }else
            return false;
    }
    else if (enEnemySkill_163==m_iSkId)
    {
        if (ret && m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75f) {
            m_iMaxHitCnt = SaveData::Inst()->getrand()%4 + 3;
            m_iHitCnt = m_iMaxHitCnt;
            return true;
        }else
            return false;
    }
    else if (enEnemySkill_164==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.3f);
    }
    else if (enEnemySkill_166==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.6f &&
                       m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.25f);
    }
    else if (enEnemySkill_170==m_iSkId)
    {
        if (ret && m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.7f) {
            m_iMaxHitCnt = SaveData::Inst()->getrand()%3 + 4;
            m_iHitCnt = m_iMaxHitCnt;
            return true;
        }else
            return false;
    }
    else if (enEnemySkill_194==m_iSkId)
    {
        return true;
    }
    else if (enEnemySkill_197==m_iSkId)
    {
        if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() >= 0.5f)
        {
            m_iMaxHitCnt = SaveData::Inst()->getrand()%3 + 4;
            m_iHitCnt = m_iMaxHitCnt;
            return true;
        }
        return false;
    }
    else if (enEnemySkill_225==m_iSkId)
    {
        if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f)
        {
            m_iMaxHitCnt = SaveData::Inst()->getrand()%3 + 4;
            m_iHitCnt = m_iMaxHitCnt;
            return true;
        }
        return false;
    }
    else if (enEnemySkill_208==m_iSkId)
    {
        if (ret)
        {
            float fHPR = m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp();
            return 0.15f<=fHPR && 0.5f>fHPR;
        }
        else
        {
            return false;
        }
    }
    else if (enEnemySkill_210==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()<=0.14f);
    }
    else if (enEnemySkill_215==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()>=0.5f);
    }
    else if (enEnemySkill_218==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()>=0.5f);
    }
    else if (enEnemySkill_220==m_iSkId)
    {
        return ret && 0<m_occurTimes && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()<0.5f);
    }
    else if (enEnemySkill_222==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()>=0.5f);
    }
    else if (enEnemySkill_228==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()>=0.5f);
    }
    else if (enEnemySkill_239==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()<0.5f);
    }
    else if (enEnemySkill_246==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()>=0.5f);
    }
    else if (enEnemySkill_248==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()<=0.25f);
    }
    else if (enEnemySkill_253==m_iSkId)
    {
        if (ret)
        {
            m_iMaxHitCnt = SaveData::Inst()->getrand()%4 + 3;
            m_iHitCnt = m_iMaxHitCnt;
            return true;
        }
        else
        {
            return false;
        }
    }
    else if (enEnemySkill_267==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()<0.4f);
    }
    else if (enEnemySkill_269==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()<=0.75f);
    }
    else if (enEnemySkill_271==m_iSkId)
    {
        if (ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()<=0.75f))
        {
            m_iMaxHitCnt = SaveData::Inst()->getrand()%3 + 3;
            m_iHitCnt = m_iMaxHitCnt;
            return true;
        }
        return false;
    }
    else if (enEnemySkill_276==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()<=0.75f);
    }
    else if (enEnemySkill_277==m_iSkId)
    {
         if (ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()<=0.75f))
         {
             m_iMaxHitCnt = SaveData::Inst()->getrand()%3 + 3;
             m_iHitCnt = m_iMaxHitCnt;
             return true;
         }
        return false;
    }
    else if (enEnemySkill_285==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()<0.75f);
    }
    else if (enEnemySkill_292 == m_iSkId)//lvgenquan
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75f);
    }
    else if (enEnemySkill_300 == m_iSkId)//lvgenquan
    {
        if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.3f && ret)
        {
            m_iMaxHitCnt = SaveData::Inst()->getrand()%2 + 4;
            m_iHitCnt = m_iMaxHitCnt;
            return true;
        }
        return false;
    }
    else if (enEnemySkill_345 == m_iSkId)
    {
        if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.50f && ret) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_344) {
                return true;
            }
            else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_345)
                return false;
            else
                return false;
        }
        return false;
    }
    else if (enEnemySkill_350 == m_iSkId)
    {
        if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() >= 0.30f && m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.49f && ret) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_349) {
                return true;
            }else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_350)
                return false;
            else
            return true;
        }
        return false;
    }
    else if (enEnemySkill_351 == m_iSkId){
        return  ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.29f);
    }
    else if (enEnemySkill_302 == m_iSkId){
        if (ret) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_301){
                m_iMaxHitCnt = SaveData::Inst()->getrand()%2 + 4;
                m_iHitCnt = m_iMaxHitCnt;
                return true;
            }
            else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_302)
                return false;
            else
                return false;
        }
        return false;
    }
    else if (enEnemySkill_307 == m_iSkId || enEnemySkill_309 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.70f);
    }
   else if (enEnemySkill_320 == m_iSkId)
   {
       if (ret && m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.29f) {
           m_iMaxHitCnt = SaveData::Inst()->getrand()%3 + 5;
           m_iHitCnt = m_iMaxHitCnt;

           return true;
       }else
           return false;

   }
   else if (enEnemySkill_329 == m_iSkId )
   {
       return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.30f);
   }
    else if (enEnemySkill_358 == m_iSkId)
    {
        if (ret && m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() > 0.30f) {
            m_iMaxHitCnt = SaveData::Inst()->getrand()%2 + 3;
            m_iHitCnt = m_iMaxHitCnt;
            
            return true;
        }else
            return false;

    }
    else if (enEnemySkill_364 == m_iSkId)
    {
    
        if (ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() > 0.30f)) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_363) {
                return true;
            }
        }
        return false;
    }
    else if (enEnemySkill_380 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() > 0.40f);
    }
    else if (enEnemySkill_382 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.75f);
    }
    else if (enEnemySkill_388 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.30f);
    }
    else if (enEnemySkill_389 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.75f);
    }
    else if (enEnemySkill_392 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.75f);
    }

    else if (enEnemySkill_399 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.50f);
    }
    else if (enEnemySkill_403 == m_iSkId)
    {
        if (ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() > 0.40f)) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_402) {
                return true;
            }
        }
        return false;
    }
    else if (enEnemySkill_407 == m_iSkId)
    {
        if (ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.39f)) {
                return true;
        }
        return false;
    }
    else if (enEnemySkill_423 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.30f);
    }
    else if (enEnemySkill_429 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.30f);
    }
    else if (enEnemySkill_431 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.50f);
    }
    else if (enEnemySkill_433 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.50f);
    }
    else if (enEnemySkill_448 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.99f);
    }
    else if (enEnemySkill_451 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() > 0.50f);
    }
    else if (enEnemySkill_454 == m_iSkId)
    {
        if (ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.49f)){
                return true;

        }
        return false;
    }
    else if (enEnemySkill_466 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.50f);
    }
    else if (enEnemySkill_467 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.24f);
    }
    else if (enEnemySkill_481 == m_iSkId)
    {
      
        if (ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() > 0.25f)) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_480) {
                return true;
            }
        }
        return false;
    }
    else if (enEnemySkill_483 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.24f);
    }
    else if (enEnemySkill_485 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() > 0.25f);
    }
    else if (enEnemySkill_487 == m_iSkId)
    {
        if (ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.24f)) {
            m_iMaxHitCnt = SaveData::Inst()->getrand()%4 + 2;
            m_iHitCnt = m_iMaxHitCnt;
            
            return true;
        }else
            return false;
    }
    else if (enEnemySkill_489 == m_iSkId)
    {
        if (ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() > 0.25f)) {
          
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_488) {
                return true;
            }
            
        }
            return false;
    }
    else if (enEnemySkill_491 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.24f);
    }
    else if (enEnemySkill_495 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.24f);
    }
    else if (enEnemySkill_497 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() > 0.25f);
    }
    else if (enEnemySkill_499 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.24f);
    }
    else if (enEnemySkill_513 == m_iSkId || enEnemySkill_530 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.50f);
    }
    else if (enEnemySkill_523 == m_iSkId || enEnemySkill_527 == m_iSkId)
    {
        if (ret) {
            m_iMaxHitCnt = SaveData::Inst()->getrand()%3 + 1;
            m_iHitCnt = m_iMaxHitCnt;
            return true;
        }
        else
            return false;
    }
    else if (enEnemySkill_529 == m_iSkId)
    {
        if (ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.30f)) {
            m_iMaxHitCnt = SaveData::Inst()->getrand()%3 + 1;
            m_iHitCnt = m_iMaxHitCnt;
            return true;
        }else
            return false;
    }
    else if (enEnemySkill_530 == m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp() < 0.50f) ;
    }
    return ret;
}

void EsLianDa::action()
{
    if (0<m_unDialogueSum)
    {
        m_unCurDialogue = (m_runCounts-1)%(m_unDialogueSum+1);
        if (m_unCurDialogue!=m_unDialogueSum &&
            enEnemySkill_194==m_iSkId && LIANJI194_HITCNT==m_iHitCnt)
        {
            unschedule(schedule_selector(EsLianDa::action));
            --m_occurTimes;
            CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
            atkInfo.iManSkillID = m_iSkId;
            atkInfo.bAtkOvr = true;
            atkInfo.iAttackPt = 0;
            m_pEnemy->setAtkInfo(atkInfo);
            m_pEnemy->sentAtkInfo();
            //m_unCurDialogue = ++m_unCurDialogue%(m_unDialogueSum+1);
            return;
        }
        else if (m_unCurDialogue!=m_unDialogueSum &&
                 enEnemySkill_312==m_iSkId && LIANJI312_HIYCNT==m_iHitCnt)
        {
            unschedule(schedule_selector(EsLianDa::action));
            --m_occurTimes;
            CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
            atkInfo.iManSkillID = m_iSkId;
            atkInfo.bAtkOvr = true;
            atkInfo.iAttackPt = 0;
            m_pEnemy->setAtkInfo(atkInfo);
            m_pEnemy->sentAtkInfo();
            //m_unCurDialogue = ++m_unCurDialogue%(m_unDialogueSum+1);
            return;
        }
        else
        {
        }
    }
  //  m_bIsRunning = true;
    unschedule(schedule_selector(EsLianDa::action));
    schedule(schedule_selector(EsLianDa::action), m_fSkillDur);
    m_occurTimes--;
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    
    if (m_iSkId == enEnemySkillEYunYiJi && m_iHitCnt == m_iMaxHitCnt)
    {
        m_BaseAtkPt = atkInfo.iAttackPt;
    }
    else if (m_iSkId == enEnemySkill5lianji && m_iHitCnt == LIANJI5_HITCNT)
    {
        m_BaseAtkPt = atkInfo.iAttackPt;
    }
    else if ( (m_iSkId == enEnemySkillNanManRuQin || m_iSkId == enEnemySkillNanManRuQin2)
             && (m_iHitCnt == NANMANRUQIN_HITCNT)
             )
    {
        m_BaseAtkPt = atkInfo.iAttackPt;
    }
    else if (m_iSkId == enEnemySkill_LianDa && m_iHitCnt == LIANDA_HITCNT)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (m_iSkId == enEnemySkill_LianJi82 && m_iHitCnt == LIANJI82_HITCNT)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (m_iSkId == enEnemySkill_LianJi83 && m_iHitCnt == LIANJI83_HITCNT)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_115 == m_iSkId && LIANJI5_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if ((enEnemySkillluanshitianxia == m_iSkId || enEnemySkillluanshitianxia2 == m_iSkId)
             && m_iHitCnt == LUANSHITIANXIA_HITCNT)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_LianJi7_3==m_iSkId && LIANJI73_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_LianJi45_4==m_iSkId && LIANJI454_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_LianJi75_3==m_iSkId && LIANJI753_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_119==m_iSkId && LIANJI119_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_125==m_iSkId && LIANJI125_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_132==m_iSkId && LIANJI132_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_141==m_iSkId && LIANJI141_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_143==m_iSkId && LIANJI143_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_147==m_iSkId && m_iMaxHitCnt==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_151==m_iSkId && m_iMaxHitCnt==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_154==m_iSkId && m_iMaxHitCnt==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_158==m_iSkId && m_iMaxHitCnt==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_163==m_iSkId && m_iMaxHitCnt==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_164==m_iSkId && LIANJI164_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_166==m_iSkId && LIANJI166_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_170==m_iSkId && m_iMaxHitCnt==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_194==m_iSkId && LIANJI194_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_197==m_iSkId && m_iMaxHitCnt==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_208==m_iSkId && LIANJI208_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_210==m_iSkId && LIANJI210_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_215==m_iSkId && LIANJI215_HITCNT==m_iHitCnt)
         m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_218==m_iSkId && LIANJI218_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_220==m_iSkId && LIANJI220_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_222==m_iSkId && LIANJI222_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_228==m_iSkId && LIANJI228_HITCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_225==m_iSkId && m_iMaxHitCnt==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_239==m_iSkId && LIANJI239_HIYCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_246==m_iSkId && LIANJI246_HIYCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_248==m_iSkId && LIANJI248_HIYCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_253==m_iSkId && m_iMaxHitCnt==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_267==m_iSkId && LIANJI267_HIYCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_268==m_iSkId && LIANJI268_HIYCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_269==m_iSkId && LIANJI269_HIYCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_271==m_iSkId && m_iMaxHitCnt==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_275==m_iSkId && LIANJI275_HIYCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_276==m_iSkId && LIANJI276_HIYCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_277==m_iSkId && m_iMaxHitCnt==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_285==m_iSkId && LIANJI285_HIYCNT==m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_292 == m_iSkId && LIANJI292_HIYCNT == m_iHitCnt)//lvgenquan
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_300 == m_iSkId && m_iMaxHitCnt==m_iHitCnt)//lvgenquan
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_350 == m_iSkId && LIANJI350_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_351 == m_iSkId && LIANJI351_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_302 == m_iSkId && m_iMaxHitCnt == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_307 == m_iSkId && LIANJI307_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_309 == m_iSkId && LIANJI309_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_312 == m_iSkId && LIANJI312_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_320 == m_iSkId && m_iMaxHitCnt == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_329 == m_iSkId && LIANJI329_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_345 == m_iSkId && LIANJI345_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_354 == m_iSkId && LIANJI354_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_358 == m_iSkId && m_iMaxHitCnt == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_364 == m_iSkId && LIANJI364_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_370 == m_iSkId && LIANJI370_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_380 == m_iSkId && LIANJI380_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_382 == m_iSkId && LIANJI382_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_388 == m_iSkId && LIANJI388_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_389 == m_iSkId && LIANJI389_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_392 == m_iSkId && LIANJI392_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_399 == m_iSkId && LIANJI399_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_403 == m_iSkId && LIANJI403_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_407 == m_iSkId && LIANJI407_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_418 == m_iSkId && LIANJI418_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_423 == m_iSkId && LIANJI423_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_429 == m_iSkId && LIANJI429_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_431 == m_iSkId && LIANJI431_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_433 == m_iSkId && LIANJI433_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_448 == m_iSkId && LIANJI448_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_451 == m_iSkId && LIANJI451_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_454 == m_iSkId && LIANJI454_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_466 == m_iSkId && LIANJI466_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_467 == m_iSkId && LIANJI467_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_474 == m_iSkId && LIANJI474_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_481 == m_iSkId && LIANJI481_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_483 == m_iSkId && LIANJI483_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_485 == m_iSkId && LIANJI485_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_487 == m_iSkId && m_iMaxHitCnt == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_489 == m_iSkId && LIANJI489_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_491 == m_iSkId && LIANJI491_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_495 == m_iSkId && LIANJI495_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_497 == m_iSkId && LIANJI497_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_499 == m_iSkId && LIANJI499_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = atkInfo.iAttackPt;
    else if (enEnemySkill_513 == m_iSkId && LIANJI513_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = 10000;
    else if (enEnemySkill_523 == m_iSkId && m_iMaxHitCnt == m_iHitCnt)
        m_BaseAtkPt = 8888;
    else if (enEnemySkill_527 == m_iSkId && m_iMaxHitCnt == m_iHitCnt)
        m_BaseAtkPt = 9999;
    else if (enEnemySkill_529 == m_iSkId && m_iMaxHitCnt == m_iHitCnt)
        m_BaseAtkPt = 10000;
    else if (enEnemySkill_530 == m_iSkId && LIANJI530_HIYCNT == m_iHitCnt)
        m_BaseAtkPt = 66666;
        

    atkInfo.iAttackPt = m_BaseAtkPt*m_fAtkFactor;
    atkInfo.iManSkillID = m_iSkId;
    
    m_iHitCnt--;
    
    if (m_iHitCnt <= 0)
    {
        if (enEnemySkillEYunYiJi == m_iSkId ||
            enEnemySkill_147 == m_iSkId ||
            enEnemySkill_151 == m_iSkId ||
            enEnemySkill_154 == m_iSkId ||
            enEnemySkill_158 == m_iSkId ||
            enEnemySkill_163 == m_iSkId ||
            enEnemySkill_170 == m_iSkId ||
            enEnemySkill_197 == m_iSkId ||
            enEnemySkill_225 == m_iSkId ||
            enEnemySkill_253 == m_iSkId ||
            enEnemySkill_271 == m_iSkId ||
            enEnemySkill_277 == m_iSkId ||
            enEnemySkill_300 == m_iSkId ||
            enEnemySkill_302 == m_iSkId ||
            enEnemySkill_320 == m_iSkId ||
            enEnemySkill_358 == m_iSkId ||
            enEnemySkill_487 == m_iSkId ||
            enEnemySkill_523 == m_iSkId ||
            enEnemySkill_527 == m_iSkId ||
            enEnemySkill_529 == m_iSkId
            )
        {
            m_iHitCnt = m_iMaxHitCnt;
        }
        else if (m_iSkId == enEnemySkill5lianji)
        {
            m_iHitCnt = LIANJI5_HITCNT;
        }
        else if (m_iSkId == enEnemySkillNanManRuQin || m_iSkId == enEnemySkillNanManRuQin2)
        {
            m_iHitCnt = NANMANRUQIN_HITCNT;
        }
        else if (enEnemySkillluanshitianxia == m_iSkId || enEnemySkillluanshitianxia2 == m_iSkId)
        {
            m_iHitCnt = LUANSHITIANXIA_HITCNT;
        }
        else if (enEnemySkill_LianJi7_3==m_iSkId)
        {
            m_iHitCnt = LIANJI73_HITCNT;
        }
        else if (enEnemySkill_LianJi45_4==m_iSkId)
        {
            m_iHitCnt = LIANJI454_HITCNT;
        }
        else if (enEnemySkill_LianJi75_3==m_iSkId)
        {
            m_iHitCnt = LIANJI753_HITCNT;
        }
        else if (enEnemySkill_115 == m_iSkId)
        {
            m_iHitCnt = LIANJI5_HITCNT;
        }
        else if (enEnemySkill_119==m_iSkId)
        {
            m_iHitCnt = LIANJI119_HITCNT;
        }
        else if (enEnemySkill_125==m_iSkId)
        {
            m_iHitCnt = LIANJI125_HITCNT;
        }
        else if (enEnemySkill_132==m_iSkId)
        {
            m_iHitCnt = LIANJI132_HITCNT;
        }
        else if (enEnemySkill_141==m_iSkId)
        {
            m_iHitCnt = LIANJI141_HITCNT;
        }
        else if (enEnemySkill_143==m_iSkId)
        {
            m_iHitCnt = LIANJI143_HITCNT;
        }
        else if (enEnemySkill_164==m_iSkId)
        {
            m_iHitCnt = LIANJI164_HITCNT;
        }
        else if (enEnemySkill_166==m_iSkId)
        {
            m_iHitCnt = LIANJI166_HITCNT;
        }
        else if (enEnemySkill_LianJi83 == m_iSkId)
        {
            m_iHitCnt = LIANJI83_HITCNT;
        }
        else if (enEnemySkill_194==m_iSkId)
        {
            m_iHitCnt = LIANJI194_HITCNT;
        }
        else if (enEnemySkill_208==m_iSkId)
        {
            m_iHitCnt = LIANJI208_HITCNT;
        }
        else if (enEnemySkill_210==m_iSkId)
        {
            m_iHitCnt = LIANJI210_HITCNT;
        }
        else if (enEnemySkill_215==m_iSkId)
        {
            m_iHitCnt = LIANJI215_HITCNT;
        }
        else if (enEnemySkill_218==m_iSkId)
        {
            m_iHitCnt = LIANJI218_HITCNT;
        }
        else if (enEnemySkill_220==m_iSkId)
        {
            m_iHitCnt = LIANJI220_HITCNT;
        }
        else if (enEnemySkill_222==m_iSkId)
        {
             m_iHitCnt = LIANJI222_HITCNT;
        }
        else if (enEnemySkill_228==m_iSkId)
        {
            m_iHitCnt = LIANJI228_HITCNT;
        }
        else if (enEnemySkill_239==m_iSkId)
        {
            m_iHitCnt = LIANJI239_HIYCNT;
        }
        else if (enEnemySkill_246==m_iSkId)
        {
            m_iHitCnt = LIANJI246_HIYCNT;
        }
        else if (enEnemySkill_248==m_iSkId)
        {
            m_iHitCnt = LIANJI248_HIYCNT;
        }
        else if (enEnemySkill_267==m_iSkId)
        {
            m_iHitCnt = LIANJI267_HIYCNT;
        }
        else if (enEnemySkill_268==m_iSkId)
        {
            m_iHitCnt = LIANJI268_HIYCNT;
        }
        else if (enEnemySkill_269==m_iSkId)
        {
            m_iHitCnt = LIANJI269_HIYCNT;
        }
        else if (enEnemySkill_275==m_iSkId)
        {
            m_iHitCnt = LIANJI275_HIYCNT;
        }
        else if (enEnemySkill_276==m_iSkId)
        {
            m_iHitCnt = LIANJI276_HIYCNT;
        }
        else if (enEnemySkill_285==m_iSkId)
        {
            m_iHitCnt = LIANJI285_HIYCNT;
        }
        else if (enEnemySkill_292 == m_iSkId)//lvgenquan
        {
            m_iHitCnt = LIANJI292_HIYCNT;
        }
        else if (enEnemySkill_350 == m_iSkId)
        {
            m_iHitCnt = LIANJI350_HIYCNT;
        }
        else if (enEnemySkill_351 == m_iSkId)
        {
            m_iHitCnt = LIANJI351_HIYCNT;
        }
        else if (enEnemySkill_307 == m_iSkId){
            m_iHitCnt = LIANJI307_HIYCNT;
        }
        else if (enEnemySkill_309 == m_iSkId){
            m_iHitCnt = LIANJI309_HIYCNT;
        }
        else if (enEnemySkill_312 == m_iSkId){
            m_iHitCnt = LIANJI312_HIYCNT;
        }
        else if (enEnemySkill_329 == m_iSkId){
            m_iHitCnt = LIANJI329_HIYCNT;
        }
        else if (enEnemySkill_345 == m_iSkId){
            m_iHitCnt = LIANJI345_HIYCNT;
        }
        else if (enEnemySkill_354 == m_iSkId)
        {
        
            m_iHitCnt =LIANJI354_HIYCNT;
        }
     
        else if (enEnemySkill_364 == m_iSkId)
        {
          
            m_iHitCnt =LIANJI364_HIYCNT;
        }
        else if (enEnemySkill_370 == m_iSkId)
        {
            m_iHitCnt =LIANJI370_HIYCNT;
        }
        else if (enEnemySkill_380 == m_iSkId)
        {
            m_iHitCnt =LIANJI380_HIYCNT;
        }
        else if (enEnemySkill_382 == m_iSkId)
        {
            m_iHitCnt =LIANJI382_HIYCNT;
        }
        else if (enEnemySkill_388 == m_iSkId)
        {
            m_iHitCnt =LIANJI388_HIYCNT;
        }
        else if (enEnemySkill_389 == m_iSkId)
        {
            m_iHitCnt =LIANJI389_HIYCNT;
        }
        else if (enEnemySkill_392 == m_iSkId)
        {
            m_iHitCnt =LIANJI392_HIYCNT;
        }
        else if (enEnemySkill_399 == m_iSkId)
        {
            m_iHitCnt =LIANJI399_HIYCNT;
        }
        else if (enEnemySkill_403 == m_iSkId)
        {
            m_iHitCnt =LIANJI403_HIYCNT;
        }
        else if (enEnemySkill_407 == m_iSkId)
        {
            m_iHitCnt =LIANJI407_HIYCNT;
        }
        else if (enEnemySkill_418 == m_iSkId)
        {
            m_iHitCnt =LIANJI418_HIYCNT;
        }
        else if (enEnemySkill_423 == m_iSkId)
        {
            m_iHitCnt =LIANJI423_HIYCNT;
        }
        else if (enEnemySkill_429 == m_iSkId)
        {
            m_iHitCnt =LIANJI429_HIYCNT;
        }
        else if (enEnemySkill_431 == m_iSkId)
        {
            m_iHitCnt =LIANJI431_HIYCNT;
        }
        else if (enEnemySkill_433 == m_iSkId)
        {
            m_iHitCnt =LIANJI433_HIYCNT;
        }
        else if (enEnemySkill_448 == m_iSkId)
        {
            m_iHitCnt =LIANJI448_HIYCNT;
        }
        else if (enEnemySkill_451 == m_iSkId)
        {
            m_iHitCnt =LIANJI451_HIYCNT;
        }
        else if (enEnemySkill_454 == m_iSkId)
        {
            m_iHitCnt =LIANJI454_HIYCNT;
        }
        else if (enEnemySkill_466 == m_iSkId)
        {
            m_iHitCnt =LIANJI466_HIYCNT;
        }
        else if (enEnemySkill_467 == m_iSkId)
        {
            m_iHitCnt =LIANJI467_HIYCNT;
        }
        else if (enEnemySkill_474 == m_iSkId)
        {
            m_iHitCnt =LIANJI474_HIYCNT;
        }
        else if (enEnemySkill_481 == m_iSkId)
        {
            m_iHitCnt =LIANJI481_HIYCNT;
        }
        else if (enEnemySkill_483 == m_iSkId)
        {
            m_iHitCnt =LIANJI483_HIYCNT;
        }
        else if (enEnemySkill_485 == m_iSkId)
        {
            m_iHitCnt =LIANJI485_HIYCNT;
        }
        else if (enEnemySkill_489 == m_iSkId)
        {
            m_iHitCnt =LIANJI489_HIYCNT;
        }
        else if (enEnemySkill_491 == m_iSkId)
        {
            m_iHitCnt =LIANJI491_HIYCNT;
        }
        else if (enEnemySkill_495 == m_iSkId)
        {
            m_iHitCnt =LIANJI495_HIYCNT;
        }
        else if (enEnemySkill_497 == m_iSkId)
        {
            m_iHitCnt =LIANJI497_HIYCNT;
        }
        else if (enEnemySkill_499 == m_iSkId)
        {
            m_iHitCnt =LIANJI499_HIYCNT;
        }
        else if (enEnemySkill_513 == m_iSkId)
        {
            m_iHitCnt = LIANJI513_HIYCNT;
        }
        else if (enEnemySkill_530 == m_iSkId)
        {
            m_iHitCnt = LIANJI530_HIYCNT;
        }
        else
        {
            m_iHitCnt = LIANDA_HITCNT;
        }
        
        unschedule(schedule_selector(EsLianDa::action));
        atkInfo.bAtkOvr = true;
    }
    //printf("-----------iAttackPt [%d]\n",atkInfo.iAttackPt);
    
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
    
}

#pragma mark -
#pragma mark 无双
EsWuShuang::EsWuShuang(int iSkId, Enemy* pEnemy)
{
    m_BaseAtkPt = 0.0f;
  //  m_bIsRunning = false;
 //   m_bIsExclusive = false;
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    m_iHitCnt = WUSHUANG_HITCNT;
    m_fAtkFactor = 0.45;
    m_fSkillDur = 0.15f;
}

int EsWuShuang::getOccurOdds()
{
    //血量大于50%
    if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.5) {
        m_nOccurOdds = 20;
    }
    //血量 30%～50%
    else if(m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.3) {
        m_nOccurOdds = 40;
    }
    //血量30%以下
    else{
        m_nOccurOdds = 80;
    }
    
    return m_nOccurOdds;
}

void EsWuShuang::action()
{
   // m_bIsRunning = true;
    ++m_runCounts;
    unschedule(schedule_selector(EsWuShuang::action));
    schedule(schedule_selector(EsWuShuang::action), m_fSkillDur);
    
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    if (m_iHitCnt == WUSHUANG_HITCNT)
    {
        m_BaseAtkPt = atkInfo.iAttackPt;
    }
    atkInfo.iAttackPt = m_BaseAtkPt*m_fAtkFactor;
    atkInfo.iManSkillID = m_iSkId;
    
    m_iHitCnt--;
    
    if (m_iHitCnt <= 0)
    {
        m_iHitCnt = WUSHUANG_HITCNT;
        unschedule(schedule_selector(EsWuShuang::action));
        atkInfo.bAtkOvr = true;
    }
    
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

#pragma mark -
#pragma mark 比例扣血
EsScaleAtk::EsScaleAtk(int iSkId, Enemy* pEnemy)
{
  //  m_bIsRunning = false;
 //   m_bIsExclusive = false;
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    m_nOccurOdds = 100;
    switch (iSkId) {
            
        case enEnemySkill_327:
            m_nOccurOdds = 40;
            break;
            case enEnemySkill_371:
            case enEnemySkill_375:
            m_nOccurOdds = 50;
            break;
        default:
            break;
    }
}

void EsScaleAtk::action()
{
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    atkInfo.iManSkillID = m_iSkId;
    atkInfo.bAtkOvr = true;
    
    switch (m_iSkId)
    {
        case enEnemySkill_116:
        {
            FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
            Player* tPlayer = tFightM->getPlayer();
            
            int iAtk = tPlayer->getCurHp()*0.99f;
            if (iAtk >= tPlayer->getCurHp())
                iAtk = tPlayer->getCurHp()-1;
            
            atkInfo.iAttackPt = iAtk;
        }
            break;
        case enEnemySkill_198:
        {
            FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
            Player* tPlayer = tFightM->getPlayer();
            int iAtk = tPlayer->getCurHp()*0.99f;
            if (iAtk >= tPlayer->getCurHp())
                iAtk = tPlayer->getCurHp()-1;
            atkInfo.iAttackPt = iAtk;
            break;
        }
         case enEnemySkill_331://lvgenquan
        {
            FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
            Player* tPlayer = tFightM->getPlayer();
           int  iAtk = tPlayer->getCurHp()*0.99f;
           if (iAtk >= tPlayer->getCurHp())
                iAtk = tPlayer->getCurHp()-1;
            atkInfo.iAttackPt = iAtk;
            break;
        }
      
            case enEnemySkill_327:
        {
            FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
            Player* tPlayer = tFightM->getPlayer();
            int  iAtk = tPlayer->getCurHp()*0.50f;
            if (iAtk >= tPlayer->getCurHp())
                iAtk = tPlayer->getCurHp()-1;
            atkInfo.iAttackPt = iAtk;

            break;
        }
       
            case enEnemySkill_319:
            case enEnemySkill_366:
            case enEnemySkill_457:
        {
            FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
            Player* tPlayer = tFightM->getPlayer();
            int  iAtk = tPlayer->getCurHp()*0.99f;
            if (iAtk >= tPlayer->getCurHp())
                iAtk = tPlayer->getCurHp()-1;
            atkInfo.iAttackPt = iAtk;
            break;
        }
        case enEnemySkill_371:
        case enEnemySkill_375:
        {
            FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
            Player* tPlayer = tFightM->getPlayer();
            int  iAtk = tPlayer->getCurHp()*0.75f;
            if (iAtk >= tPlayer->getCurHp())
                iAtk = tPlayer->getCurHp()-1;
            atkInfo.iAttackPt = iAtk;
            
            break;
        }


        default:
            break;
    }
    
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

bool EsScaleAtk::canExecute()
{
    ++m_runCounts;
    bool ret = SaveData::Inst()->getrand() < getOccurOdds() < m_nOccurOdds ;
    switch (m_iSkId)
    {
        case enEnemySkill_116:
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.49f  && ret)
            {
                if (m_pEnemy->getSkillUseLastID() == enEnemySkill_115)
                    return true;
                else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_116)
                    return false;
                else
                    return true;
            }
            else
                return false;
            break;
            
        case enEnemySkill_198:
        {
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f && ret)
            {
                if (m_pEnemy->getSkillUseLastID() == enEnemySkill_199)
                    return true;
                else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_198)
                    return false;
                else return true;
            }
            return false;
            break;
        }
        case enEnemySkill_331:{//lvgenquan
            if (ret) {
                if (m_pEnemy->getSkillUseLastID() == enEnemySkill_332)
                    return true;
                else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_331)
                    return false;
                else
                    return true;
            }
            return false;
            break;
        }
        case enEnemySkill_319:
            return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() >= 0.30f) && 0==m_runCounts%3 ;
            break;
        case enEnemySkill_327:
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.30f) && ret ;
            break;
        case enEnemySkill_366:{
            if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.29f) && ret ) {
                if (m_pEnemy->getSkillUseLastID() == enEnemySkill_366) {
                    return false;
                }
                return true;
            }
            return false;
        }
            break;
        case enEnemySkill_457:{
            if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 1.0f) && ret ) {
                if (m_pEnemy->getSkillUseLastID() == enEnemySkill_457) {
                    return false;
                }
                return true;
            }
            return false;

        }
            break;
            
        case enEnemySkill_375:
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.5f) && ret ;
            break;
        case enEnemySkill_371:
            return ret;
            break;
        default:
            break;
    }
    
    return false;
}


#pragma mark -
#pragma mark 重击
EsZhongJi::EsZhongJi(int iSkId, Enemy* pEnemy)
{
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    
    
    switch (iSkId)
    {
        case enEnemySkill_131:
        case enEnemySkill_257:
            m_nOccurOdds = 100;
            m_fAtkFactor = 10.0f;
            break;
        case enEnemySkill_138:
        case enEnemySkill_212:
        case enEnemySkill_251:
        case enEnemySkill_385:
        case enEnemySkill_397:
        case enEnemySkill_461:
            m_nOccurOdds = 100;
            m_fAtkFactor = 1.5f;
            break;
        case enEnemySkill_168:
        case enEnemySkill_231:
            m_nOccurOdds = 100;
            m_fAtkFactor = 6.f;
            break;
        case enEnemySkill_169:
            m_nOccurOdds = 75;
            m_fAtkFactor = 2.f;
            break;
        case enEnemySkill_199:
        case enEnemySkill_262:
        {
            m_nOccurOdds = 100;
            m_fAtkFactor = 1.25f;
            break;
        }
        case enEnemySkill_204:
        {
            m_nOccurOdds = 100;
            m_fAtkFactor = 0.88f;
            break;
        }
        case enEnemySkill_229:
        {
            m_nOccurOdds = 70;
            m_fAtkFactor = 1.7f;
            break;
        }
        case enEnemySkill_237:
        {
            m_nOccurOdds = 100;
            m_fAtkFactor = 4.0f;
            break;
        }
        case enEnemySkill_241:
        case enEnemySkill_273:
        case enEnemySkill_439:
        {
            m_nOccurOdds = 100;
            m_fAtkFactor = 3.0f;
            break;
        }
        case enEnemySkill_264:
        {
            m_nOccurOdds = 60;
            m_fAtkFactor = 1.5f;
            break;
        }
        case enEnemySkill_161:
        case enEnemySkill_216:
        case enEnemySkill_280:
        case enEnemySkill_283:
        case enEnemySkill_338:
        case enEnemySkill_360:
        case enEnemySkill_367:
        case enEnemySkill_377:
        case enEnemySkill_426:
        case enEnemySkill_458:
        case enEnemySkill_462:
        {
            m_nOccurOdds = 100;
            m_fAtkFactor = 2.0f;
            break;
        }
        case enEnemySkill_286:{
        case enEnemySkill_334:
            m_nOccurOdds = 100;
            m_fAtkFactor = 1.0f;
            break;
        }
        case enEnemySkill_332:
        case enEnemySkill_335:
        {
            m_nOccurOdds = 100;
            m_fAtkFactor = 1.80f;
            break;
        }
            case enEnemySkill_298:
        {
            m_nOccurOdds = 100;
            m_fAtkFactor = 5.0f;
            break;
        }
        case enEnemySkill_305:
        {
            m_nOccurOdds = 75;
            m_fAtkFactor = 1.25f;
            break;
        }
        case enEnemySkill_328:
        {
            m_nOccurOdds = 30;
            m_fAtkFactor = 1.25f;
            break;
        }
        case enEnemySkill_359:
        {
            m_nOccurOdds = 100;
            m_fAtkFactor = 0.1f;
            break;
        }
        case enEnemySkill_372:
        case enEnemySkill_394:
        case enEnemySkill_420:
        {
            m_nOccurOdds = 70;
            m_fAtkFactor = 1.5f;
            break;
        }
        case enEnemySkill_383:
        {
            m_occurTimes = 1;
            m_nOccurOdds = 70;
            m_fAtkFactor = 1.6f;
            break;
        }
        case enEnemySkill_384:
        case enEnemySkill_396:
        {
            m_nOccurOdds = 100;
            m_fAtkFactor = 0.3f;
            break;
        }
        case enEnemySkill_386:
        case enEnemySkill_410:
        case enEnemySkill_413:
        {
            m_nOccurOdds = 40;
            m_fAtkFactor = 1.2f;
            break;
        }
        case enEnemySkill_415:
        {
            m_nOccurOdds = 70;
            m_fAtkFactor = 1.3f;
            break;
        }
        case enEnemySkill_493:
        {
            m_nOccurOdds = 100;
            m_fAtkFactor = 1.1f;
            break;
        }
        default:
            m_nOccurOdds = 30;
            m_fAtkFactor = 1.25f;
            break;
    }
}

void EsZhongJi::action()
{
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    atkInfo.iAttackPt *= m_fAtkFactor;
    atkInfo.iManSkillID = m_iSkId;
    m_occurTimes--;
    atkInfo.bAtkOvr = true;
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

bool EsZhongJi::canExecute()
{
    ++m_runCounts;
    if (m_occurTimes <= 0)
    {
        return false;
    }
    if (SaveData::Inst()->getrand() < getOccurOdds())
    {
        if (m_iSkId==enEnemySkill_138)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.49f);
        }
        if (m_iSkId==enEnemySkill_161)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.25f);
        }
        if (m_iSkId==enEnemySkill_168)
        {
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.24f)
            {
                if (m_pEnemy->getSkillUseLastID() == enEnemySkill_165)
                    return true;
                else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_168)
                    return false;
                else
                    return false;
            }
            return false;
        }
        if (m_iSkId==enEnemySkill_169)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.3f);
        }
        
        if (enEnemySkill_199==m_iSkId)
        {
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f)
            {
                if (m_pEnemy->getSkillUseLastID() == enEnemySkill_198)
                    return true;
                else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_199)
                    return false;
                else
                    return true;
            }
            else
                return false;
        }
        if (enEnemySkill_204==m_iSkId)
        {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f;
        }
        if (enEnemySkill_212==m_iSkId)
        {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f;
        }
        if (enEnemySkill_216==m_iSkId)
        {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f;
        }
        if (enEnemySkill_229==m_iSkId)
        {
            float fHpR = m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp();
            return 0.25f<= fHpR && 0.5f>fHpR;
        }
        if (enEnemySkill_231==m_iSkId)
        {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.24f;
        }
        if (enEnemySkill_241==m_iSkId)
        {
            return m_pEnemy->getCurHp()<m_pEnemy->getBaseHp();
        }
        if (enEnemySkill_251==m_iSkId || enEnemySkill_394==m_iSkId)
        {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f;
        }
        if (enEnemySkill_262==m_iSkId)
        {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.25f;
        }
        if (enEnemySkill_264==m_iSkId)
        {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() >= 0.4f;
        }
        if (enEnemySkill_280==m_iSkId)
        {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.25f;
        }
        if (enEnemySkill_332 == m_iSkId) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_331) {
                return true;
            }else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_332)
                return false;
            return false;
        }
        if (enEnemySkill_334 == m_iSkId){
            
        //lvgenquan
           if (m_pEnemy->getSkillUseLastID() == enEnemySkill_335)
            return true;
           else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_334)
            return false;
           else
            return true;
        }
        if (enEnemySkill_335 == m_iSkId){//lvgenquan
            
           if (m_pEnemy->getSkillUseLastID() == enEnemySkill_334)
                return true;
           else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_335)
                return false;
           else
               return false;
       }
        
        if (enEnemySkill_298 == m_iSkId) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_297)
                return true;
            else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_298)
                return false;
            else
                return false;
        }
        if (enEnemySkill_328 == m_iSkId) {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.30f;
        }
        if (enEnemySkill_359 == m_iSkId) {
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.29f) {
                if (m_pEnemy->getSkillUseLastID() == enEnemySkill_360)
                    return true;
                else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_359)
                    return false;
                else
                    return true;
            }
            return false;
        }
        if (enEnemySkill_360 == m_iSkId) {
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.29f) {
                if (m_pEnemy->getSkillUseLastID() == enEnemySkill_359)
                    return true;
                else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_360)
                    return false;
                else
                    return false;
            }
            return false;
        }
        if (enEnemySkill_367 == m_iSkId) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_366)
                return true;
            else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_367)
                return false;
            else
                return false;
        }
        if (enEnemySkill_372==m_iSkId)
        {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.29f;
        }
        if (enEnemySkill_377 == m_iSkId) {
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f && m_pEnemy->getSkillUseLastID() == enEnemySkill_376)
                return true;
            else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_377)
                return false;
            else
                return false;
        }
        if (enEnemySkill_383==m_iSkId || enEnemySkill_420==m_iSkId)
        {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.3f;
        }
        if (enEnemySkill_384==m_iSkId || enEnemySkill_385==m_iSkId  )
        {
            return m_runCounts%2;
        }
        if (enEnemySkill_397==m_iSkId) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_396) {
                return true;
            }
            return false;
        }
        if (enEnemySkill_396==m_iSkId) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_396) {
                return false;
            }
            return true;
        }
        if (enEnemySkill_386==m_iSkId)
        {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75f;
        }
        if (enEnemySkill_410==m_iSkId || enEnemySkill_413==m_iSkId || enEnemySkill_415==m_iSkId)
        {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.7f;
        }
        if (enEnemySkill_439==m_iSkId)
        {
            return m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.24f;
        }
        if (enEnemySkill_458 == m_iSkId) {
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 1.0f && m_pEnemy->getSkillUseLastID() == enEnemySkill_457)
                return true;
            else
                return false;
        }
        if (enEnemySkill_461 == m_iSkId) {
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 1.0f && m_pEnemy->getSkillUseLastID() != enEnemySkill_461) {
                return true;
            } else
                return false;
        }
        if (enEnemySkill_462 == m_iSkId) {
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 1.0f && m_pEnemy->getSkillUseLastID() == enEnemySkill_461)
                return true;
            else
                return false;
        }
        if (enEnemySkill_493 == m_iSkId) {
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.25f && m_pEnemy->getSkillUseLastID() == enEnemySkill_492)
                return true;
            else
                return false;
        }
        return true;
    }
    return false;
}

#pragma mark -
#pragma mark 强袭
EsQiangXi::EsQiangXi(int iSkId, Enemy* pEnemy)
{
  //  m_bIsRunning = false;
 //   m_bIsExclusive = false;
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    m_fAtkFactor = 1.5;
    m_nOccurOdds = 30;
}

void EsQiangXi::action()
{
    ++m_runCounts;
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    atkInfo.iAttackPt *= m_fAtkFactor;
    atkInfo.iManSkillID = m_iSkId;
    
    atkInfo.bAtkOvr = true;
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

#pragma mark -
#pragma mark 碎骨
EsSuiGu::EsSuiGu(int iSkId, Enemy* pEnemy)
{
 //   m_bIsRunning = false;
 //   m_bIsExclusive = false;
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    
    if (m_iSkId == enEnemySkillMengShowLuanWu)
    {
        m_fAtkFactor = 10.0;
        m_nOccurOdds = 0;
    }
    else if (enEnemySkillSuiGu2 == m_iSkId)
    {
        m_fAtkFactor = 2.0;
        m_nOccurOdds = 100;
        m_occurTimes = 1;
    }
    else if (enEnemySkillSuiGu3==m_iSkId)
    {
        m_fAtkFactor = 4.0;
        m_nOccurOdds = 100;
        m_occurTimes = 1;
    }
    else
    {
        m_fAtkFactor = 2.0;
        m_nOccurOdds = 30;
    }
}

void EsSuiGu::action()
{
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    atkInfo.iAttackPt *= m_fAtkFactor;
    atkInfo.iManSkillID = m_iSkId;
    m_occurTimes--;
    atkInfo.bAtkOvr = true;
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

bool EsSuiGu::canExecute()
{
    ++m_runCounts;
    if (!EnemySkill::canExecute())return false;
    if (1>m_occurTimes)return false;
    return true;
}


#pragma mark -
#pragma mark 封印
EsFengYin::EsFengYin(int iSkId, Enemy* pEnemy)
{
    m_occurTimes = 3;
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    m_fAtkFactor = 2.0;
    
    switch (m_iSkId)
    {
        case enEnemySkillZhuQueQi:
        case enEnemySkillHuoXiJin40:
        case enEnemySkillShuiXiJin40:
        case enEnemySkillMuXiJin40:
        case enEnemySkillGuangXiJin40:
        case enEnemySkillAnXiJin40:
        case enEnemySkillShenJin40:
        case enEnemySkillLuanJin40:
        case enEnemySkillZhuQueQi2:
        case enEnemySkill_114:
            case enEnemySkill_505:
            m_nOccurOdds = 20;
            break;
            
        case enEnemySkillHuoXiJin10:
        case enEnemySkillShuiXiJin10:
        case enEnemySkillMuXiJin10:
        case enEnemySkillGuangXiJin10:
        case enEnemySkillAnXiJin10:
        case enEnemySkillShenJin10:
        case enEnemySkillLuanJin10:
            m_nOccurOdds = 10;
            break;
        case enEnemySkillBingFengShiKe:
            
            m_nOccurOdds = 40;
            m_occurTimes = 1;
            break;
            
        case enEnemySkillBingFengShiKe2:
        case enEnemySkillZhuQueQi3:
        case enEnemySkillShuiXiJin1_3:
        case enEnemySkillHuoXiJin1_3:
        case enEnemySkillMuXiJin1_3:
        case enEnemySkillGuangXiJin1_3:
        case enEnemySkillAnXiJin1_3:
        case enEnemySkillShenJin1_3:
        case enEnemySkillLuanJin3_23:
        case enEnemySkillLuanJinRun2_3_23:
        case enEnemySkill_113:
        case enEnemySkill_120:
        case enEnemySkill_121:
        case enEnemySkill_126:
        case enEnemySkill_127:
        case enEnemySkill_152:
        case enEnemySkill_153:
        case enEnemySkill_155:
        case enEnemySkill_160:
        case enEnemySkill_191:
        case enEnemySkill_192:
        case enEnemySkill_193:
        case enEnemySkill_223:
        case enEnemySkill_227:
        case enEnemySkill_242:
        case enEnemySkill_243:
        case enEnemySkill_245:
        case enEnemySkill_254:
        case enEnemySkill_258:
        case enEnemySkill_263:
        case enEnemySkill_219:
        case enEnemySkill_233:
        case enEnemySkill_260:
        case enEnemySkill_270:
        case enEnemySkill_284:
        case enEnemySkill_291: //lvgenquan
        case enEnemySkill_339:
        case enEnemySkill_310:
        case enEnemySkill_323:
        case enEnemySkill_324:
        case enEnemySkill_387:
        case enEnemySkill_395:
        case enEnemySkill_436:
        case enEnemySkill_440:
        case enEnemySkill_441:
        case enEnemySkill_442:
        case enEnemySkill_452:
            case enEnemySkill_504:
            case enEnemySkill_526:
        {
            m_nOccurOdds = 100;
            m_occurTimes = 1;
            break;
        }
        case enEnemySkill_148:
            m_nOccurOdds = 40;
            m_occurTimes = 1;
            break;
        case enEnemySkill_211:
        case enEnemySkill_213:
        {
            m_nOccurOdds = 90;
            m_occurTimes = 1;
            break;
        }
        case enEnemySkill_341:
        case enEnemySkill_347:
        {
            m_nOccurOdds = 100;
            m_occurTimes = 1;
            m_fAtkFactor = 1.0f;
            break;
        }
        case enEnemySkill_317:
        case enEnemySkill_363:
        case enEnemySkill_374:
        case enEnemySkill_379:
        case enEnemySkill_401:
        {
            m_nOccurOdds = 100;
            m_occurTimes = 999;
            break;
        }
        case enEnemySkill_353:
        case enEnemySkill_369:
        case enEnemySkill_469:
        case enEnemySkill_470:
        case enEnemySkill_471:
        case enEnemySkill_478:
        {
            m_nOccurOdds = 10;
            m_occurTimes = 999;
            break;
        }
        case enEnemySkill_393:
        {
            m_nOccurOdds = 70;
            m_occurTimes = 1;
            break;
        }
        case enEnemySkill_406:
        case enEnemySkill_472:
        case enEnemySkill_473:
        {
            m_nOccurOdds = 25;
            m_occurTimes = 999;
            break;
        }
        case enEnemySkill_501:
            m_nOccurOdds = 30;
            break;
        case enEnemySkill_516:
            m_nOccurOdds = 60;
            break;
        default:
            m_nOccurOdds = 100;
            break;
    }
}

void EsFengYin::action()
{
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    atkInfo.iAttackPt *= m_fAtkFactor;
    atkInfo.iManSkillID = m_iSkId;
    m_occurTimes--;
    
    atkInfo.bAtkOvr = true;
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

void EsFengYin::set_occurTimes(short occurtimes)
{
    m_occurTimes = occurtimes;
}

bool EsFengYin::canExecute()
{
    ++m_runCounts;
    if (m_occurTimes <= 0)
    {
        return false;
    }
    
    bool hasCorPropty = false;
    
    switch (m_iSkId)
    {
        case enEnemySkillHuoXiJin:
        case enEnemySkillHuoXiJin40:
        case enEnemySkillHuoXiJin10:
        case enEnemySkillHuoXiJin1_3:
        case enEnemySkill_242:
        case enEnemySkill_245:
        case enEnemySkill_254:
        case enEnemySkill_353:
        case enEnemySkill_393:
        case enEnemySkill_471:
        {
            FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
            Player* tPlayer = tFightM->getPlayer();
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (tPlayer->getCardByIdx(i) != NULL
                    && (tPlayer->getCardByIdx(i)->getElement()== enElementFire || tPlayer->getCardByIdx(i)->getSubElement()== enElementFire))
                {
                    hasCorPropty = true;
                    break;
                }
            }
        }
            break;
            
        case enEnemySkillShuiXiJin:
        case enEnemySkillShuiXiJin40:
        case enEnemySkillShuiXiJin10:
        case enEnemySkillShuiXiJin1_3:
        case enEnemySkill_213:
        case enEnemySkill_219:
        case enEnemySkill_227:
        case enEnemySkill_469:
        {
            FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
            Player* tPlayer = tFightM->getPlayer();
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (tPlayer->getCardByIdx(i) != NULL
                    && (tPlayer->getCardByIdx(i)->getElement()== enElementWater || tPlayer->getCardByIdx(i)->getSubElement()== enElementWater))
                {
                    hasCorPropty = true;
                    break;
                }
            }

        }
            break;
            
        case enEnemySkillMuXiJin:
        case enEnemySkillMuXiJin40:
        case enEnemySkillMuXiJin10:
        case enEnemySkillMuXiJin1_3:
        case enEnemySkill_284:
        case enEnemySkill_470:
        {
            FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
            Player* tPlayer = tFightM->getPlayer();
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (tPlayer->getCardByIdx(i) != NULL
                    && (tPlayer->getCardByIdx(i)->getElement()== enElementWood || tPlayer->getCardByIdx(i)->getSubElement()== enElementWood))
                {
                    hasCorPropty = true;
                    break;
                }
            }
        }
            break;
            
        case enEnemySkillGuangXiJin:
        case enEnemySkillGuangXiJin40:
        case enEnemySkillGuangXiJin10:
        case enEnemySkillGuangXiJin1_3:
        case enEnemySkill_153:
        case enEnemySkill_258:
        case enEnemySkill_347:
        case enEnemySkill_473:
        case enEnemySkill_501:
        {
            FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
            Player* tPlayer = tFightM->getPlayer();
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (tPlayer->getCardByIdx(i) != NULL
                    && (tPlayer->getCardByIdx(i)->getElement()== enElementLight || tPlayer->getCardByIdx(i)->getSubElement()== enElementLight))
                {
                    hasCorPropty = true;
                    break;
                }
            }
        }
            break;
            
        case enEnemySkillAnXiJin:
        case enEnemySkillAnXiJin40:
        case enEnemySkillAnXiJin10:
        case enEnemySkillAnXiJin1_3:
        case enEnemySkill_193:
        case enEnemySkill_223:
        case enEnemySkill_341: //lvgenquan
        case enEnemySkill_369:
        case enEnemySkill_472:
            case enEnemySkill_516:
        {
            FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
            Player* tPlayer = tFightM->getPlayer();
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (tPlayer->getCardByIdx(i) != NULL
                    && (tPlayer->getCardByIdx(i)->getElement()== enElementDark || tPlayer->getCardByIdx(i)->getSubElement()== enElementDark))
                {
                    hasCorPropty = true;
                    break;
                }
            }
        }
            break;
            
        case enEnemySkillShenJin:
        case enEnemySkillShenJin40:
        case enEnemySkillShenJin10:
        case enEnemySkillShenJin1_3:
        case enEnemySkill_270:
        {
            FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
            Player* tPlayer = tFightM->getPlayer();
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (tPlayer->getCardByIdx(i) != NULL
                    && (tPlayer->getCardByIdx(i)->getCategory()== enGeneralTp_God || tPlayer->getCardByIdx(i)->getSubCategory()== enGeneralTp_God )
                    )
                {
                    hasCorPropty = true;
                    break;
                }
            }
        }
            break;
            
        case enEnemySkill_155:
        {
            FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
            Player* tPlayer = tFightM->getPlayer();
            
            for(int i = 0; i < kMaxCardCntInBattle; i++)
            {
                if (tPlayer->getCardByIdx(i) != NULL
                    && (tPlayer->getCardByIdx(i)->getCategory()== enGeneralTp_GuoShi || tPlayer->getCardByIdx(i)->getSubCategory()== enGeneralTp_GuoShi )
                    )
                {
                    hasCorPropty = true;
                    break;
                }
            }
        }
            break;
            
            default:
        {
            hasCorPropty = true;
        }
            break;
    }
    
    if (!hasCorPropty)
    {
        return false;
    }
    
    if (SaveData::Inst()->getrand() < getOccurOdds())
    {
        if (enEnemySkillZhuQueQi3 == m_iSkId)
        {
            return  (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.25);
        }
        else if (enEnemySkillLuanJin3_23 == m_iSkId)
        {
            return  (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.39);
        }
        else if (enEnemySkillLuanJinRun2_3_23==m_iSkId)
        {
            return 2==getRunCount();
        }
        else if (enEnemySkill_120==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75);
        }
        else if (enEnemySkill_121==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5);
        }
        else if (enEnemySkill_126==m_iSkId ||
                 enEnemySkill_127==m_iSkId ||
                 enEnemySkill_191==m_iSkId ||
                 enEnemySkill_192==m_iSkId ||
                 enEnemySkill_193==m_iSkId)
        {
            return 1==m_runCounts;
        }
        else if (enEnemySkill_152==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.19);
        }
        else if (enEnemySkill_153==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75);
        }
        else if (enEnemySkill_155==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75);
        }
        else if (enEnemySkill_160==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5);
        }
        else if (enEnemySkill_211==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() >= 0.5f);
        }
        else if (enEnemySkill_213==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() >= 0.5f);
        }
        else if (enEnemySkill_219==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f);
        }
        else if (enEnemySkill_223==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f);
        }
        else if (enEnemySkill_227==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() >= 0.5f);
        }
        else if (enEnemySkill_233==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f);
        }
        else if (enEnemySkill_242==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f);
        }
        else if (enEnemySkill_243==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f);
        }
        else if (enEnemySkill_245==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() >= 0.5f);
        }
        else if (enEnemySkill_260==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.25f);
        }
        else if (enEnemySkill_263==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() >= 0.4f);
        }
        else if (enEnemySkill_270==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f);
        }
        else if (enEnemySkill_291 == m_iSkId){//lvgenquan
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f);
        }
        else if (enEnemySkill_339 == m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f);
        }
        else if (enEnemySkill_310 == m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.30f);
        }
        else if (enEnemySkill_317 == m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.09f);
        }
        else if (enEnemySkill_323 == m_iSkId
                 || enEnemySkill_324 == m_iSkId
                 || enEnemySkill_504 == m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.30f);
        }
        else if (enEnemySkill_363 == m_iSkId)
        {
            return (m_pEnemy->getSkillUseLastID() == enEnemySkill_362 &&(m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.30f));
        }
        else if (enEnemySkill_374 == m_iSkId || enEnemySkill_440 == m_iSkId || enEnemySkill_441 == m_iSkId || enEnemySkill_442 == m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.25f);
        }
        else if (enEnemySkill_379 == m_iSkId)
        {
            return m_runCounts%2;
        }
        else if (enEnemySkill_387==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5);
        }
        else if (enEnemySkill_393==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75);
        }
        else if (enEnemySkill_401==m_iSkId)
        {
            return ((m_pEnemy->getSkillUseLastID() == enEnemySkill_404 || (m_pEnemy->getSkillUseLastID() == 0))&& (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.4));
        }
        else if (enEnemySkill_406==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.39);
        }
        else if (enEnemySkill_436==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5);
        }
        else if (enEnemySkill_452==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.49);
        }
        else if (enEnemySkill_526 == m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0f/m_pEnemy->getBaseHp()< 0.50f);
        }
        return true;
    }
    return false;
}


#pragma mark -
#pragma mark 治愈
EsZhiYu::EsZhiYu(int iSkId, Enemy* pEnemy)
{
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    switch (iSkId)
    {
        case enEnemySkillZhiYu:
        case enEnemySkill_279:
        case enEnemySkill_282:
           
        {
            m_nOccurOdds = 100;
            m_occurTimes = 1;
            break;
        }
        case enEnemySkill_475:
        {
            m_nOccurOdds = 50;
            m_occurTimes = 1;
            break;
        }
        case enEnemySkill_476:
        {
            m_nOccurOdds = 70;
            m_occurTimes = 1;
            break;
        }
        default:
        {
            break;
        }
    }
    
}

void EsZhiYu::action()
{
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    atkInfo.iManSkillID = m_iSkId;
    m_occurTimes--;
    
    atkInfo.bAtkOvr = true;
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

void EsZhiYu::set_occurTimes(short occurtimes)
{
    m_occurTimes = occurtimes;
}

bool EsZhiYu::canExecute()
{
    ++m_runCounts;
    if (0<m_occurTimes && SaveData::Inst()->getrand() < getOccurOdds())
    {
        switch (m_iSkId)
        {
            case enEnemySkillZhiYu:
            case enEnemySkill_279:
            {
                return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.3f);
            }
            case enEnemySkill_282:
            {
                return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.2f);
            }
            case enEnemySkill_475:
            {
                return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.3f);
            }
            case enEnemySkill_476:
            {
                return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.5f);
            }
            default:
            {
                break;
            }
        }
        return true;
    }
    return false;
}


#pragma mark -
#pragma mark 复活
EsFuHuo::EsFuHuo(int iSkId, Enemy* pEnemy)
{
 //   m_bIsRunning = false;
  //  m_bIsExclusive = false;
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    m_nOccurOdds = 100;
    if (m_iSkId == enEnemySkillFuHuo2)
    {
        m_occurTimes = 3;
    }
    else
    {
        m_occurTimes = 1;
    }
}

void EsFuHuo::action()
{
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    atkInfo.iManSkillID = m_iSkId;
    m_occurTimes--;
    
    atkInfo.bAtkOvr = true;
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

void EsFuHuo::set_occurTimes(short occurtimes)
{
    m_occurTimes = occurtimes;
}

bool EsFuHuo::canExecute()
{
    ++m_runCounts;
    if (m_iSkId == enEnemySkillZhenFuHuo)
    {
        return false;
    }
    
    return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.3) && (m_occurTimes>0);
}


#pragma mark -
#pragma mark 逆转





EsNiZhuan::EsNiZhuan(int iSkId, Enemy* pEnemy)
{
    m_ChangeStone = true;
    
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    m_fAtkFactor = 1.0;
    if (m_iSkId == enEnemySkillBaoZhuYinCang)
        m_nOccurOdds = 10;
    else if (m_iSkId == enEnemySkillShiguangnizhuan
             || m_iSkId == enEnemySkill_162
             || m_iSkId == enEnemySkill_167)
    {
        m_occurTimes = 1;
        m_nOccurOdds =100;
    }
    else if (m_iSkId >= enEnemySkillkengdiezhuanshui && m_iSkId <= enEnemySkillkengdiezhuanan2)
    {
        m_nOccurOdds = 25;
    }
    else if (enEnemySkillNiZhuanTianXia2==m_iSkId)
    {
        m_occurTimes = 1;
        m_nOccurOdds = 20;
    }
    else if (enEnemySkill_117 == m_iSkId
             || enEnemySkill_511 == m_iSkId)
        m_nOccurOdds = 20;
    else if (m_iSkId>= enEnemySkill_134 && m_iSkId <= enEnemySkill_136)
    {
        m_nOccurOdds = 50;
    }
    else if (m_iSkId == enEnemySkill_139)
    {
        m_fAtkFactor = 0.7;
        m_nOccurOdds = 100;
    }
    else if (m_iSkId == enEnemySkill_140)
    {
        m_fAtkFactor = 0.7;
        m_nOccurOdds = 100;
        m_lastCD = 0;
    }
    else if (m_iSkId == enEnemySkill_150
             || m_iSkId == enEnemySkill_507)
    {
        m_fAtkFactor = 0.7;
        m_nOccurOdds = 40;
    }
    else if (m_iSkId==enEnemySkill_196
             || m_iSkId == enEnemySkill_503)
    {
        m_fAtkFactor = 0.7;
        m_nOccurOdds = 30;
    }
    else if (enEnemySkill_203==m_iSkId)
    {
        m_fAtkFactor = 0.7f;
        m_occurTimes = 9999;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_207==m_iSkId)
    {
        m_fAtkFactor = 0.8f;
        m_occurTimes = 9999;
        m_nOccurOdds = 60;
    }
    else if (enEnemySkill_234==m_iSkId)
    {
        m_nOccurOdds = 25;
    }
    else if (enEnemySkill_255==m_iSkId
             || enEnemySkill_259==m_iSkId)
    {
        m_nOccurOdds = 50;
    }
    else if (enEnemySkill_265==m_iSkId)
    {
        m_fAtkFactor = 0.7;
        m_nOccurOdds = 24;
    }
    else if (enEnemySkill_278==m_iSkId )
    {
        m_nOccurOdds = 70;
    }
    else if (enEnemySkill_281==m_iSkId
             || enEnemySkill_288 == m_iSkId
             || enEnemySkill_289 == m_iSkId
             || enEnemySkill_290 == m_iSkId)
    {
        m_fAtkFactor = 0.7;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_293 == m_iSkId)
    {
        m_occurTimes = 2;
        m_nOccurOdds = 10;
    }
    else if (enEnemySkill_295 == m_iSkId)
    {
        m_fAtkFactor = 0.8;
        m_nOccurOdds = 70;
    }
   
    else if (enEnemySkill_342 == m_iSkId
             || enEnemySkill_343 == m_iSkId
             || enEnemySkill_502 == m_iSkId)
    {
        m_fAtkFactor = 0.80f;
        m_nOccurOdds = 50;
    }
    else if (enEnemySkill_348 == m_iSkId
             || enEnemySkill_525 == m_iSkId)
    {
        m_fAtkFactor = 0.50f;
        m_nOccurOdds = 50;
    }
    else if (enEnemySkill_349 == m_iSkId)
    {
        m_fAtkFactor = 1.0f;
        m_nOccurOdds = 100;
    }
   else if (enEnemySkill_301 == m_iSkId)
   {
       m_fAtkFactor = 0.80f;
       m_nOccurOdds = 100;
   }
    else if (enEnemySkill_304 == m_iSkId)
    {
        m_nOccurOdds = 25;
    }
    else if (enEnemySkill_313 == m_iSkId)
    {
        m_nOccurOdds = 100;
        m_occurTimes = 1;
        m_fAtkFactor = 0.50f;
    }
    else if (enEnemySkill_322 == m_iSkId)
    {
        m_nOccurOdds = 25;
    }
    else if (enEnemySkill_326 == m_iSkId)
    {
        m_nOccurOdds = 25;
       
        m_fAtkFactor = 0.80f;
    }
   else if (enEnemySkill_356 == m_iSkId)
   {
       m_nOccurOdds = 70;
       m_occurTimes = 1;
       m_fAtkFactor = 0.80f;
    }
   else if (enEnemySkill_362 == m_iSkId)
   {
       m_nOccurOdds = 100;
       m_fAtkFactor = 0.80f;
   }
   else if (enEnemySkill_381 == m_iSkId)
   {
       m_nOccurOdds = 100;
       m_fAtkFactor = 1.50f;
   }
   else if (enEnemySkill_391 == m_iSkId)
   {
       m_nOccurOdds = 70;
       m_fAtkFactor = 1.50f;
   }
   else if (enEnemySkill_402 == m_iSkId)
   {
       m_nOccurOdds = 100;
       m_fAtkFactor = 1.0f;
   }
   else if (enEnemySkill_404 == m_iSkId)
   {
       m_nOccurOdds = 100;
       m_fAtkFactor = 0.70f;
   }
   else if (enEnemySkill_405 == m_iSkId)
   {
       m_nOccurOdds = 50;
       m_fAtkFactor = 1.35f;
   }
   else if (enEnemySkill_411 == m_iSkId)
   {
       m_nOccurOdds = 40;
      
   }
   else if (enEnemySkill_414 == m_iSkId)
   {
       m_nOccurOdds = 50;
       m_fAtkFactor = 1.20f;
       m_lastCD = 0;
   }
   else if (enEnemySkill_416 == m_iSkId)
   {
       m_nOccurOdds = 70;
       m_fAtkFactor = 0.80f;
       m_lastCD = 0;
   }
   else if (enEnemySkill_417 == m_iSkId)
   {
       m_nOccurOdds = 40;
      
   }
   else if (enEnemySkill_419 == m_iSkId)
   {
       m_nOccurOdds = 50;
       m_fAtkFactor = 1.20f;
       m_lastCD = 0;
   }
   else if (enEnemySkill_422 == m_iSkId)
   {
       m_nOccurOdds = 100;
       m_fAtkFactor = 1.20f;
   }
   else if (enEnemySkill_434 == m_iSkId
            || enEnemySkill_484 == m_iSkId)
   {
       m_nOccurOdds = 100;
       m_fAtkFactor = 0.80f;
       m_lastCD = 0;
   }
   else if (enEnemySkill_437 == m_iSkId)
   {
       m_nOccurOdds = 70;
       m_fAtkFactor = 0.80f;
   }
   else if (enEnemySkill_443 == m_iSkId)
   {
       m_nOccurOdds = 25;
       
   }
   else if (enEnemySkill_449 == m_iSkId)
   {
       m_nOccurOdds = 100;
       m_fAtkFactor = 1.75f;
      
   }
   else if (enEnemySkill_453 == m_iSkId)
   {
       m_nOccurOdds = 40;
       m_fAtkFactor = 0.80f;
   }
   else if (enEnemySkill_465 == m_iSkId)
   {
       m_nOccurOdds = 100;
       m_fAtkFactor = 1.30f;
       m_lastCD = 0;
   }
   else if (enEnemySkill_480 == m_iSkId
            || enEnemySkill_488 == m_iSkId
            || enEnemySkill_492 == m_iSkId)
   {
       m_nOccurOdds = 100;
       m_fAtkFactor = 0.80f;
   }
   else if (enEnemySkill_496 == m_iSkId){
       m_nOccurOdds = 50;
       m_fAtkFactor = 0.80f;
   }
   else if (enEnemySkill_482 == m_iSkId
            || enEnemySkill_486 == m_iSkId
            || enEnemySkill_490 == m_iSkId
            || enEnemySkill_494 == m_iSkId
            || enEnemySkill_498 == m_iSkId)
   {
       m_nOccurOdds = 100;
       m_fAtkFactor = 1.60f;
   }
    else if (enEnemySkill_517 == m_iSkId)
    {
        m_nOccurOdds = 60;
    }
    else if (enEnemySkill_522 == m_iSkId)
    {
        m_nOccurOdds = 50;
        m_fAtkFactor = 0.6f;
    }
   else
   {
       m_nOccurOdds = 30;
   }
}

void EsNiZhuan::action()
{
   
    if (m_occurTimes <= 0) {
        return;
    }
     m_occurTimes --;
        CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    
        atkInfo.iManSkillID = m_iSkId;
        atkInfo.iAttackPt *= m_fAtkFactor;
        atkInfo.bAtkOvr = true;
  
        m_pEnemy->setAtkInfo(atkInfo);
        m_pEnemy->sentAtkInfo();

    
}

bool EsNiZhuan::canExecute()
{
//    if (m_occurTimes < 1)
//        return false;
//    
//    if (m_iSkId >= enEnemySkillkengdiezhuanshui && m_iSkId <= enEnemySkillkengdiezhuanan2)
//    {
//        return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75) && SaveData::Inst()->getrand() < getOccurOdds();
//    }
//    
//    return SaveData::Inst()->getrand() < getOccurOdds();
    
    
    
//    switch (m_iSkId)
//    {
//            //暗暗耐性
//        case enEnemySkillBaoZhuYinCang:
//        {
//            if (FightMngr *pFightMngr = dynamic_cast<FightMngr*>(m_pEnemy->getParent()->getParent()))
//            {
//                if (Player *pPlayer = pFightMngr->getPlayer())
//                {
//                    unsigned short ushSkillCount = pPlayer->getNumByAwakeId(enAwakeSkill_24);
//                    if (0<ushSkillCount)
//                    {
//                        if (stAwakeSkill *pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(enAwakeSkill_24))
//                        {
//                            //                        float fOdds = StoneGrid::getOddsAwake10(pAwakeSkill->fEffect,ushSkillCount);
//                            int nOdds = ushSkillCount;
//                            nOdds *= pAwakeSkill->cOdds;
//                            if (SaveData::Inst()->getrand()<nOdds)
//                            {
//                                return false;
//                            }
//                        }
//                    }
//                }
//            }
//            break;
//        }
//            //无效块耐性
//        case enEnemySkillNiZhuanHuo:
//        case enEnemySkillNiZhuanShui:
//        case enEnemySkillNiZhuanMu:
//        case enEnemySkillNiZhuanGuang:
//        case enEnemySkillNiZhuanAn:
//        case enEnemySkillNiZhuanHuiFu:
//        case enEnemySkillNiZhuanTianXia:
//        case enEnemySkillNiZhuanTianXia2:
//        case enEnemySkill_150:
//        case enEnemySkill_167:
//        case enEnemySkill_278:
//        case enEnemySkill_281:
//        case enEnemySkill_196:
//        case enEnemySkill_265:
//        {
//            if (FightMngr *pFightMngr = dynamic_cast<FightMngr*>(m_pEnemy->getParent()->getParent()))
//            {
//                if (Player *pPlayer = pFightMngr->getPlayer())
//                {
//                    unsigned short ushSkillCount = pPlayer->getNumByAwakeId(enAwakeSkill_25);
//                    if (0<ushSkillCount)
//                    {
//                        if (stAwakeSkill *pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(enAwakeSkill_25))
//                        {
//                            int nOdds = ushSkillCount;
//                            nOdds *= pAwakeSkill->cOdds;
//                            if (SaveData::Inst()->getrand()<nOdds)
//                            {
//                                return false;
//                            }
//                        }
//                    }
//                }
//                break;
//            }
//        default:
//            {
//                break;
//            }
//        }
//    }
    
    ++m_runCounts;
    
    if (0<m_occurTimes && SaveData::Inst()->getrand() < getOccurOdds())
    {
        if (m_iSkId==enEnemySkillNiZhuanTianXia2)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.55f);
        }
        else if (m_iSkId >= enEnemySkillkengdiezhuanshui && m_iSkId <= enEnemySkillkengdiezhuanan2)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75);
        }
        else if (m_iSkId==enEnemySkill_139)
        {
            return 1==m_runCounts%3;
        }
        else if (m_iSkId == enEnemySkill_167)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.25);
        }
        else if (m_iSkId==enEnemySkill_140)
        {
            bool canDo = false;
            if (m_lastCD==0) {
                canDo = true;
                m_lastCD = SaveData::Inst()->getrand()%2;
            }else{
                m_lastCD--;
            }
            return canDo;
            
        }
        else if (m_iSkId==enEnemySkill_196
                 || m_iSkId == enEnemySkill_503)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() >= 0.5f);
        }
        else if (enEnemySkill_203==m_iSkId)
        {
            float fHpR = m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp();
            return 0.5f<=fHpR && 1.0f>fHpR;
        }
        else if (enEnemySkill_207==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp())>=0.5f;
        }
        else if (enEnemySkill_265==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp())>=0.4f;
        }
        else if (enEnemySkill_281==m_iSkId ||enEnemySkill_288 == m_iSkId || enEnemySkill_289 == m_iSkId || enEnemySkill_290 == m_iSkId) //lvgenquan
        {
            return 1==m_runCounts%2;
        }
        else if (enEnemySkill_234==m_iSkId)
        {
            if (FightMngr *pFightMngr = dynamic_cast<FightMngr*>(m_pEnemy->getParent()->getParent()))
            {
                if (Player *pPlayer = pFightMngr->getPlayer())
                {
                    if (StoneGrid* pStoneGrid = pPlayer->getStoneGrid())
                    {
                        //if(m_stoneNodeArray[j][i].iElement == src)
                        return pStoneGrid->haveStone(enElementDark,enElementDark);
                    }
                }
            }
            
        }
        else if (enEnemySkill_278==m_iSkId)
        {
            if (FightMngr *pFightMngr = dynamic_cast<FightMngr*>(m_pEnemy->getParent()->getParent()))
            {
                if (Player *pPlayer = pFightMngr->getPlayer())
                {
                    if (StoneGrid* pStoneGrid = pPlayer->getStoneGrid())
                    {
                        return pStoneGrid->haveStone(enElementFire,enElementFire);
                    }
                }
            }
        }
        else if (enEnemySkill_342 == m_iSkId || enEnemySkill_343 == m_iSkId || enEnemySkill_348 == m_iSkId || enEnemySkill_502 == m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.50f);
        }
        else if (enEnemySkill_349 == m_iSkId){ //lvgenquan
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() >= 0.3f && m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.49f) {
                if (m_pEnemy->getSkillUseLastID() == enEnemySkill_350 ) {
                    return true;
                }
                else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_349)
                    return false;
                else
                    return true;
            }
            return false;
        }
        else if (enEnemySkill_301 == m_iSkId)
        {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_302)
                return true;
            else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_301)
                return false;
            else
                return true;
        }
        else if (m_iSkId==enEnemySkill_326)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.30f);
        }
        else if (m_iSkId==enEnemySkill_362)
        {
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.30f)
            {
                if (m_pEnemy->getSkillUseLastID() == enEnemySkill_363 || m_pEnemy->getSkillUseLastID() == enEnemySkill_364 || m_pEnemy->getSkillUseLastID() == enEnemySkill_362) {
                    return false;
                }
                return true;
            }
            return false;
        }
        else if (m_iSkId==enEnemySkill_381)
        {
            if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.39f)
            {
                return true;
            }
            return false;
        }
        else if (m_iSkId==enEnemySkill_391)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.30f);
        }
        else if (m_iSkId==enEnemySkill_402)
        {
            if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.40f)) {
                if (m_pEnemy->getSkillUseLastID() == enEnemySkill_401) {
                    return true;
                }
                return false;
            }
            return false;
            
        }
        else if (m_iSkId==enEnemySkill_404)
        {
            if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.40f)) {
                if (m_pEnemy->getSkillUseLastID() == enEnemySkill_403) {
                    return true;
                }
                return false;
            }
            return false;
            
        }
        else if (enEnemySkill_405==m_iSkId)
        {
            return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()) < 0.39f;
        }

         else if (m_iSkId==enEnemySkill_414)
         {
             if (m_runCounts - m_lastCD == 1 && m_pEnemy->getSkillUseLastID() == enEnemySkill_414) {
                 return false;
             }else{
                 m_lastCD = m_runCounts;
                 return true;
             }
         }
         else if (m_iSkId==enEnemySkill_416)
         {
             if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.70f)) {
                 
                 if (m_runCounts - m_lastCD == 1 && m_pEnemy->getSkillUseLastID() == enEnemySkill_416) {
                     return false;
                 }else{
                     m_lastCD = m_runCounts;
                     return true;
                 }
             }
             return false;
             
         }
         else if (m_iSkId==enEnemySkill_419)
         {
             if (m_runCounts - m_lastCD ==1 && m_pEnemy->getSkillUseLastID() == enEnemySkill_419) {
                 return false;
             }else{
                 m_lastCD = m_runCounts;
                 return true;
             }
         }
         else if (m_iSkId==enEnemySkill_422)
         {
             if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.70f)) {
                 return 1 == m_runCounts %2;
             }
             return false;
             
         }
         else if (m_iSkId==enEnemySkill_434)
         {
             if (m_runCounts - m_lastCD == 1 && m_pEnemy->getSkillUseLastID() == enEnemySkill_434) {
                 return false;
             }else{
                 m_lastCD = m_runCounts;
                 return true;
             }
    
         }
         else if (m_iSkId==enEnemySkill_449)
         {
             return true;
         }
         else if (m_iSkId==enEnemySkill_453)
         {
             if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.49f)) {
                 return true;
             }
             return false;
             
         }
         else if (m_iSkId==enEnemySkill_465)
         {
             if (m_runCounts - m_lastCD == 1 && m_pEnemy->getSkillUseLastID() == enEnemySkill_465) {
                 return false;
             }
             else{
                 m_lastCD = m_runCounts;
                 return true;
             }
             
         }
         else if (m_iSkId==enEnemySkill_480)
         {
             if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.25f)) {
                 if (m_pEnemy->getSkillUseLastID() !=enEnemySkill_480) {
                     return true;
                 }
                 return false;
             }
             return false;
             
         }
         else if (m_iSkId==enEnemySkill_482)
         {
             if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.24f)) {
                 
                 return true;
             }
             return false;
             
         }
         else if (m_iSkId==enEnemySkill_484)
         {
             if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.25f)) {
                 
                 if (m_runCounts - m_lastCD == 1 && m_pEnemy->getSkillUseLastID() == enEnemySkill_484) {
                     return false;
                 }else{
                     m_lastCD = m_runCounts;
                     return true;
                 }
                 
                 
             }
             return false;
             
         }
         else if (m_iSkId==enEnemySkill_486)
         {
             if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.24f)) {
                return true;
             }
             return false;
             
         }
         else if (m_iSkId==enEnemySkill_488)
         {
             if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.25f)) {
                 if (m_pEnemy->getSkillUseLastID() !=enEnemySkill_488) {
                     return true;
                 }
                 return false;
             }
             return false;
             
         }
         else if (m_iSkId==enEnemySkill_490)
         {
             if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.24f)) {
                 
                 return true;
             }
             return false;
             
         }
         else if (m_iSkId==enEnemySkill_492)
         {
             if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.25f)) {
                 if (m_pEnemy->getSkillUseLastID() !=enEnemySkill_492) {
                     return true;
                 }
                 return false;
             }
             return false;
             
         }
         else if (m_iSkId==enEnemySkill_494)
         {
             if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.24f)) {
                return true;
             }
             return false;
             
         }
         else if (m_iSkId==enEnemySkill_496)
         {
             if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.25f)) {                     return true;
                 
             }
             return false;
             
         }
         else if (m_iSkId==enEnemySkill_498)
         {
             if ((m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.24f)) {
                return true;
             }
             return false;
             
         }
     

         return true;
        
    }
    return false;
    
}

#pragma mark -
#pragma mark 护甲
EsHuJia::EsHuJia(int iSkId, Enemy* pEnemy)
{
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    m_nOccurOdds = 100;
    
    switch (iSkId)
    {
        case enEnemySkillMuTengJia2:
            m_nOccurOdds = 40;
            break;
            
        case enEnemySkillMuTengJia3:
        case enEnemySkillQuYan:
        case enEnemySkillDuanLiu:
        case enEnemySkillPoMu:
        case enEnemySkillZhanLei:
        case enEnemySkillShiHun:
        case enEnemySkill_156:
        case enEnemySkill_272:
        case enEnemySkill_226:
        case enEnemySkill_217:
        case enEnemySkill_340:
        case enEnemySkill_311:
        case enEnemySkill_325:
        case enEnemySkill_390:
        case enEnemySkill_435:
        case enEnemySkill_463:
        case enEnemySkill_206:
            case enEnemySkill_510:
            m_occurTimes = 1;
            break;
            
        default:
            break;
    }
}

void EsHuJia::action()
{
    switch (m_iSkId)
    {
        case enEnemySkillQuYan:
        case enEnemySkillDuanLiu:
        case enEnemySkillPoMu:
        case enEnemySkillZhanLei:
        case enEnemySkillShiHun:
        case enEnemySkill_156:
        case enEnemySkill_272:
        case enEnemySkill_226:
        case enEnemySkill_217:
        case enEnemySkill_340:
        case enEnemySkill_311:
        case enEnemySkill_325:
        case enEnemySkill_390:
        case enEnemySkill_435:
        case enEnemySkill_463:
            
        {
            if (m_occurTimes <= 0)
            {
                return;
            }
            EnemyGrp* pEnemyGrp = (EnemyGrp*)(m_pEnemy->getParent());
            pEnemyGrp->showSkillTip(m_pEnemy, m_iSkId);
            
            int iName = 0;
            int iTimes = 3;
            switch (m_iSkId)
            {
                case enEnemySkill_156:
                    iName = EN_ENEMY_HUJIA;
                    iTimes = 10;
                    break;
                    
                case enEnemySkillQuYan:
                    iName = EN_ENEMY_DEFEN_FIRE;
                    break;
                    
                case enEnemySkillDuanLiu:
                    iName = EN_ENEMY_DEFEN_WATER;
                    break;
                    
                case enEnemySkillPoMu:
                    iName = EN_ENEMY_DEFEN_WOOD;
                    break;
                    
                case enEnemySkillZhanLei:
                    iName = EN_ENEMY_DEFEN_LIGHT;
                    break;
                    
                case enEnemySkillShiHun:
                    iName = EN_ENEMY_DEFEN_AN;
                    break;
                    
                case enEnemySkill_272:
                case enEnemySkill_226:
                case enEnemySkill_217:
                case enEnemySkill_311:
                case enEnemySkill_325:
                case enEnemySkill_435:
                case enEnemySkill_463:
                case enEnemySkill_427:
                case enEnemySkill_430:
                    iName = EN_ENEMY_HUJIA;
                    iTimes = 999;
                  //  iTimes = 3;
                    break;
                case enEnemySkill_340:
                    iName = EN_ENEMY_500THOUNSAND_LIMIT;
                    m_pEnemy->setcurrentRoundAtk(0);
                    break;
                case enEnemySkill_390:
                    iName = EN_ENEMY_HUJIA;
                    iTimes = 20;
                    break;
                default:
                    iName = EN_ENEMY_DEFEN_FIRE;
                    break;
            }
            
            m_pEnemy->setBuffByIdx(iName, iTimes, -1);
            m_occurTimes--;
        }
            break;
                    
        default:
            m_occurTimes--;
            CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
            atkInfo.iManSkillID = m_iSkId;
            atkInfo.bAtkOvr = true;
            m_pEnemy->setAtkInfo(atkInfo);
            m_pEnemy->sentAtkInfo();
            break;
    }
}

bool EsHuJia::canExecute()
{
    ++m_runCounts;
    switch (m_iSkId)
    {
        case enEnemySkillMuTengJia3:
             return m_occurTimes > 0;
            break;
            
        case enEnemySkillQuYan:
        case enEnemySkillDuanLiu:
        case enEnemySkillPoMu:
        case enEnemySkillZhanLei:
        case enEnemySkillShiHun:
            return false;
            break;
            
        case enEnemySkillMuTengJia2:
            return SaveData::Inst()->getrand() < getOccurOdds()
            && (!m_pEnemy->isBuffByIdx(EN_ENEMY_MUTENGJIA));
            break;
            
        case enEnemySkillMuTengJia:
            return !m_pEnemy->isBuffByIdx(EN_ENEMY_MUTENGJIA);
            break;
         case enEnemySkill_340: //lvgenquan
            case enEnemySkill_272:
            case enEnemySkill_311:
            case enEnemySkill_390:
            case enEnemySkill_427:
            case enEnemySkill_226:
            case enEnemySkill_217:
            case enEnemySkill_206:
            case enEnemySkill_510:
            return m_occurTimes > 0;
            break;
        default:
            return !m_pEnemy->isBuffByIdx(EN_ENEMY_HUJIA);
            break;
    }
}


#pragma mark -
#pragma mark 单体buff
EsBuffOnly::EsBuffOnly(int iSkId, Enemy* pEnemy)
{
 //   m_bIsRunning = false;
 //   m_bIsExclusive = false;
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    m_nOccurOdds = 100;
}

void EsBuffOnly::action()
{
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    atkInfo.iManSkillID = m_iSkId;
    atkInfo.bAtkOvr = true;
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

bool EsBuffOnly::canExecute()
{
    ++m_runCounts;
    if (m_iSkId == enEnemySkillxinjinganghuti)
    {
        if (m_pEnemy->isBuffByIdx(EN_ENEMY_JINGANGHUTI))
            return false;
        
        EnemyGrp* pEnemyGrp = (EnemyGrp*)(m_pEnemy->getParent());
        for (int j=0; j < pEnemyGrp->getEnemyCount(); j++)
        {
            if (pEnemyGrp->getEnemyByIdx(j)->isAlive() && j != m_pEnemy->getEnemyIndex())
            {
                return true;
            }
        }
    }
    
    return false;
}

#pragma mark -
#pragma mark 蓄势待发
EsXuShiDaiFa::EsXuShiDaiFa(int iSkId, Enemy* pEnemy)
{
 //   m_bIsRunning = false;
  //  m_bIsExclusive = false;
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    
    if (m_iSkId == enEnemySkillHaoYin)
    {
        m_nOccurOdds = 30;
    }
    else if (enEnemySkill_238==m_iSkId
             || enEnemySkill_250==m_iSkId)
    {
        m_nOccurOdds = 50;
    }
    else if (enEnemySkill_294 == m_iSkId
             || enEnemySkill_316 == m_iSkId)
    {
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_508 == m_iSkId)
    {
        m_nOccurOdds = 100;
        m_occurTimes = 1;
    }
    else
    {
        m_nOccurOdds = 30;
    }
}

void EsXuShiDaiFa::action()
{
    m_occurTimes--;
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    atkInfo.iManSkillID = m_iSkId;
    atkInfo.bAtkOvr = true;
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

bool EsXuShiDaiFa::canExecute()
{
    ++m_runCounts;
    if (m_occurTimes<=0)
        return false;
    switch (m_iSkId)
    {
        case enEnemySkillShiJianNingJie:
            return ((!m_pEnemy->isBuffByIdx(EN_ENEMY_SHIJIANNINGJIE))
                    && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5)
                    );
            break;
            
        case enEnemySkillHaoYin:
            return (!m_pEnemy->isBuffByIdx(EN_ENEMY_HAOYIN) && SaveData::Inst()->getrand() < getOccurOdds());
            break;
            
        case enEnemySkillHaoYin2:
            return (!m_pEnemy->isBuffByIdx(EN_ENEMY_HAOYIN2));
            break;
        
        case enEnemySkillXingYunYiJi:
            return (!m_pEnemy->isBuffByIdx(EN_ENEMY_XINGYUNYIJI))&&SaveData::Inst()->getrand() < getOccurOdds();
            break;
            
        case enEnemySkill_238:
        {
            return (!m_pEnemy->isBuffByIdx(EN_ENEMY_XUSHIDAIFA238))&&SaveData::Inst()->getrand() < getOccurOdds();
            break;
        }
        case enEnemySkill_250:
        {
            return (!m_pEnemy->isBuffByIdx(EN_ENEMY_XUSHIDAIFA250))&&
            SaveData::Inst()->getrand() < getOccurOdds() &&
            (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp()>=0.5f);
            
            break;
        }
            case enEnemySkill_294://lvgenquan
        {
            return ((!m_pEnemy->isBuffByIdx(EN_ENEMY_XUSHIDAIFA294)) && SaveData::Inst()->getrand() < getOccurOdds() && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.3f));
            break;
        }
        case enEnemySkill_316://lvgenquan
        {
            return ((!m_pEnemy->isBuffByIdx(EN_ENEMY_XUSHIDAIFA316)) && SaveData::Inst()->getrand() < getOccurOdds() && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.40f));
            break;
        }
        case enEnemySkill_508:
        {
            return (!m_pEnemy->isBuffByIdx(EN_ENEMY_XUSHIDAIFA508))&&SaveData::Inst()->getrand() < getOccurOdds();
            break;
        }
        default:
            return (!m_pEnemy->isBuffByIdx(EN_ENEMY_XUSHIDAIFA))&&SaveData::Inst()->getrand() < getOccurOdds();
            break;
    }
}

#pragma mark -
#pragma mark 杀手锏
EsShaShowJian::EsShaShowJian(int iSkId, Enemy* pEnemy)
{
//    m_bIsRunning = false;
//    m_bIsExclusive = false;
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    
    m_iHitCnt = SHASHOUJIAN_HITCNT;
    m_fAtkFactor = 2.0;
    m_fSkillDur = 0.3f;
}

void EsShaShowJian::action()
{
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    atkInfo.iManSkillID = m_iSkId;
    atkInfo.bAtkOvr = true;
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

void EsShaShowJian::attack()
{
    unschedule(schedule_selector(EsShaShowJian::attack));
    schedule(schedule_selector(EsShaShowJian::attack), m_fSkillDur);
    
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    if (m_iHitCnt == SHASHOUJIAN_HITCNT)
    {
        m_BaseAtkPt = atkInfo.iAttackPt;
    }
    atkInfo.iAttackPt = m_BaseAtkPt*m_fAtkFactor;
    atkInfo.iManSkillID = 0;
    
    m_iHitCnt--;
    
    if (m_iHitCnt <= 0)
    {
        m_iHitCnt = SHASHOUJIAN_HITCNT;
        unschedule(schedule_selector(EsShaShowJian::attack));
        atkInfo.bAtkOvr = true;
    }
    
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

bool EsShaShowJian::canExecute()
{
    ++m_runCounts;
    return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5)
    && (!m_pEnemy->isBuffByIdx(EN_ENEMY_SHASHOUJIAN));
}

#pragma mark -
#pragma mark 激怒狂暴
EsJiNuKuangBao::EsJiNuKuangBao(int iSkId, Enemy* pEnemy)
{
 //   m_bIsRunning = false;
  //  m_bIsExclusive = false;
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    
    if (m_iSkId == enEnemySkillGongJiZiTai
        || m_iSkId == enEnemySkillBaoNu
        || enEnemySkillBaoZou == m_iSkId
        )
    {
        m_nOccurOdds = 80;
    }
    else if (enEnemySkillJiNu3==m_iSkId ||
             enEnemySkill_118==m_iSkId ||
             enEnemySkill_122==m_iSkId ||
             enEnemySkill_142==m_iSkId ||
             enEnemySkill_146==m_iSkId ||
             enEnemySkill_149==m_iSkId ||
             enEnemySkill_145==m_iSkId ||
             enEnemySkill_159==m_iSkId ||
             enEnemySkill_306==m_iSkId ||
             enEnemySkill_365==m_iSkId ||
             enEnemySkill_456==m_iSkId ||
             enEnemySkill_460==m_iSkId ||
             enEnemySkill_464==m_iSkId ||
             enEnemySkill_479==m_iSkId ||
             enEnemySkill_202==m_iSkId ||
             enEnemySkill_232==m_iSkId ||
             enEnemySkill_355==m_iSkId ||
             enEnemySkill_247==m_iSkId ||
             enEnemySkill_447==m_iSkId ||
             enEnemySkill_428==m_iSkId ||
             enEnemySkill_249==m_iSkId ||
             enEnemySkill_252==m_iSkId ||
             enEnemySkill_287 == m_iSkId)
    {
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_133==m_iSkId)
    {
        m_nOccurOdds = 100;
        m_occurTimes = 1;
    }
    else if (enEnemySkill_157==m_iSkId)
    {
        m_nOccurOdds = 75;
    }
    else if (enEnemySkill_303 == m_iSkId ||
             enEnemySkill_314 == m_iSkId ||
             enEnemySkill_352 == m_iSkId ||
             enEnemySkill_368 == m_iSkId)
    {
        m_nOccurOdds = 40;
    }
    else
    {
        m_nOccurOdds = 50;
    }
}

void EsJiNuKuangBao::action()
{
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    m_occurTimes--;
    atkInfo.iManSkillID = m_iSkId;
    atkInfo.bAtkOvr = true;
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

bool EsJiNuKuangBao::canExecute()
{
    ++m_runCounts;
    if (m_occurTimes < 1)
        return false;
    if (enEnemySkillJiNu2 == m_iSkId)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_JINU2))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_JINU2)
            && SaveData::Inst()->getrand() < getOccurOdds())
        {
            return true;
        }
    }
    else
    if (m_iSkId == enEnemySkillJiNu)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_JINU))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_JINU)
            && SaveData::Inst()->getrand() < getOccurOdds())
        {
            return true;
        }
    }
    else
    if (m_iSkId == enEnemySkillKuangBao)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_KUANGBAO))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_KUANGBAO)
            && SaveData::Inst()->getrand() < getOccurOdds())
        {
            return true;
        }
    }
    else if (m_iSkId == enEnemySkill_142)
    {
            if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF142))
                && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF142)
                && SaveData::Inst()->getrand() < getOccurOdds()
                && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.39f))
            {
                return true;
            }
    }
    else
    if (m_iSkId == enEnemySkillGongJiZiTai)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_GONGJIZITAI))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_GONGJIZITAI)
            && SaveData::Inst()->getrand() < getOccurOdds())
        {
            return true;
        }
    }
    else if (m_iSkId == enEnemySkillBaoNu)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BAONU))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BAONU)
            && SaveData::Inst()->getrand() < getOccurOdds())
        {
            return true;
        }
    }
    else if (enEnemySkillBaoZou == m_iSkId)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BAOZOU))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BAOZOU)
            && SaveData::Inst()->getrand() < getOccurOdds())
        {
            return true;
        }
    }
    else if (enEnemySkillJiNu3 == m_iSkId)
    {
        if (!m_pEnemy->isBuffByIdx(EN_ENEMY_JINU3) &&
            (m_pEnemy->getCurAtkType() != EN_ENEMY_JINU3) &&
            (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.25f))
        {
            return true;
        }
    }
    else if (enEnemySkill_118==m_iSkId)
    {
        if (!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF118) &&
            (m_pEnemy->getCurAtkType()!=EN_ENEMY_BUFF118) &&
            (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.7f) &&
            m_pEnemy->isAliveOnlyMe())
        {
            return true;
        }
    }
    else if (enEnemySkill_157==m_iSkId)
    {
        if (!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF157) &&
            (m_pEnemy->getCurAtkType()!=EN_ENEMY_BUFF157) &&
            SaveData::Inst()->getrand() < getOccurOdds() &&
            (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75f))
        {
            return true;
        }
    }
    else if (enEnemySkill_159==m_iSkId)
    {
        if (!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF159) &&
            (m_pEnemy->getCurAtkType()!=EN_ENEMY_BUFF159) &&
            m_pEnemy->isAliveOnlyMe())
        {
            return true;
        }
    }
    else if (enEnemySkill_122==m_iSkId)
    {
        if (!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF122) &&
            (m_pEnemy->getCurAtkType()!=EN_ENEMY_BUFF122) &&
            m_pEnemy->isAliveOnlyMe())
        {
            return true;
        }
    }
    else if (enEnemySkill_146==m_iSkId)
    {
        if (!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF146) &&
            (m_pEnemy->getCurAtkType()!=EN_ENEMY_BUFF146) &&
            m_pEnemy->isAliveOnlyMe())
        {
            return true;
        }
    }
    else if (m_iSkId == enEnemySkill_133)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF133))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF133)
            && SaveData::Inst()->getrand() < getOccurOdds()
            && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.3f))
        {
            return true;
        }
    }
    else if (m_iSkId == enEnemySkill_149)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF149))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF149)
            && SaveData::Inst()->getrand() < getOccurOdds()
            && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.29f))
        {
            return true;
        }
    }
    else if (m_iSkId == enEnemySkill_137)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF137))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF137)
            && SaveData::Inst()->getrand() < getOccurOdds()
            && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.5f))
        {
            return true;
        }
    }
    else if (m_iSkId == enEnemySkill_145)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF145))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF145)
            && SaveData::Inst()->getrand() < getOccurOdds()
            && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.29f))
        {
            return true;
        }
    }
    else if (enEnemySkill_202==m_iSkId)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF202))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF202)
            && SaveData::Inst()->getrand() < getOccurOdds()
            &&(m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 1.0f))
        {
            return true;
        }
    }
    else if (enEnemySkill_232==m_iSkId || enEnemySkill_355==m_iSkId)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF232))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF232)
            && SaveData::Inst()->getrand() < getOccurOdds()
            && m_pEnemy->isAliveOnlyMe())
        {
            return true;
        }
    }
    else if (enEnemySkill_247==m_iSkId)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF247))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF247)
            && SaveData::Inst()->getrand() < getOccurOdds()
            &&(m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f))
        {
            return true;
        }
    }
    else if (enEnemySkill_249==m_iSkId)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF249))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF249)
            && SaveData::Inst()->getrand() < getOccurOdds()
            &&(m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.25f))
        {
            return true;
        }
    }
    else if (enEnemySkill_252==m_iSkId)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF252))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF252)
            && SaveData::Inst()->getrand() < getOccurOdds()
            && m_pEnemy->isAliveOnlyMe())
        {
            return true;
        }
    }
    else if (enEnemySkill_287 == m_iSkId){//lvgenquan
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF287)) && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF287) && SaveData::Inst()->getrand() < getOccurOdds() && m_pEnemy -> isAliveOnlyMe()) {
            return true;
        }
    }
    else if (enEnemySkill_303 == m_iSkId || enEnemySkill_352==m_iSkId || enEnemySkill_368==m_iSkId)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF303)) && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF303) && SaveData::Inst()->getrand() < getOccurOdds() ) {
            return true;
        }
    }
    else if (enEnemySkill_306 == m_iSkId || enEnemySkill_456 == m_iSkId || enEnemySkill_460 == m_iSkId)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF306)) && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF306) && SaveData::Inst()->getrand() < getOccurOdds() && m_pEnemy -> isAliveOnlyMe()) {
            return true;
        }
    }
    else if (enEnemySkill_314==m_iSkId)
    {
        if (!m_pEnemy->isBuffByIdx(EN_ENEMY_XUSHIDAIFA314) &&
            (m_pEnemy->getCurAtkType()!=EN_ENEMY_XUSHIDAIFA314) &&
            SaveData::Inst()->getrand() < getOccurOdds() &&
            (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.50f))
        {
            return true;
        }
    }
    else if (m_iSkId == enEnemySkill_365)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF142))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF142)
            && SaveData::Inst()->getrand() < getOccurOdds()
            && m_pEnemy->getSkillUseLastID() == enEnemySkill_364
            && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.3f))
        {
            return true;
        }
    }
    else if (enEnemySkill_428==m_iSkId)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF247))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF247)
            && SaveData::Inst()->getrand() < getOccurOdds()
            &&(m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.7f))
        {
            return true;
        }
    }
    else if (enEnemySkill_447==m_iSkId)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF447))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF447)
            && SaveData::Inst()->getrand() < getOccurOdds()
            &&(m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 1.0f))
        {
            return true;
        }
    }
    else if (m_iSkId == enEnemySkill_464)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF142))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF142)
            && SaveData::Inst()->getrand() < getOccurOdds()
            && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75f))
        {
            return true;
        }
    }
    else if (m_iSkId == enEnemySkill_468)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF137))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF137)
            && SaveData::Inst()->getrand() < getOccurOdds())
        {
            return true;
        }
    }
    else if (m_iSkId == enEnemySkill_479)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF142))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF142)
            && SaveData::Inst()->getrand() < getOccurOdds()
            && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f))
        {
            return true;
        }
    }
    else if (m_iSkId == enEnemySkill_477)
    {
        if ((!m_pEnemy->isBuffByIdx(EN_ENEMY_BUFF157))
            && (m_pEnemy->getCurAtkType() != EN_ENEMY_BUFF157)
            && SaveData::Inst()->getrand() < getOccurOdds()
            && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() > 0.25f)
            && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.75f))
        {
            return true;
        }
    }
    return false;
}

#pragma mark -
#pragma mark 对话
EsDuiHua::EsDuiHua(int iSkId, Enemy* pEnemy)
{
    m_pEnemy = pEnemy;
    m_iSkId = iSkId;
    
    if (m_iSkId == enEnemySkillTu)
    {
        m_nOccurOdds = 0;
    }
    else if (enEnemySkillshuohua20 == m_iSkId)
    {
        m_nOccurOdds = 20;
    }
    else if (enEnemySkill_123 == m_iSkId)
    {
        m_nOccurOdds = 50;
    }
    else if (enEnemySkill_124 == m_iSkId
             || enEnemySkill_376 == m_iSkId
             || enEnemySkill_201==m_iSkId
             || enEnemySkill_240==m_iSkId
             || enEnemySkill_266==m_iSkId
             || enEnemySkill_244==m_iSkId
             || enEnemySkill_165 == m_iSkId
             || enEnemySkill_344 == m_iSkId
             || enEnemySkill_297 == m_iSkId
             || enEnemySkill_446 == m_iSkId)
    {
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_171 <= m_iSkId && enEnemySkill_190 >= m_iSkId)
    {
        m_nOccurOdds = 20;
    }
    else if (enEnemySkill_209==m_iSkId ||
             enEnemySkill_224==m_iSkId ||
             enEnemySkill_230==m_iSkId ||
             enEnemySkill_235==m_iSkId ||
             enEnemySkill_236==m_iSkId ||
             enEnemySkill_256==m_iSkId ||
             enEnemySkill_274==m_iSkId ||
             enEnemySkill_438==m_iSkId ||
             enEnemySkill_514==m_iSkId ||
             enEnemySkill_515==m_iSkId ||
             enEnemySkill_532==m_iSkId ||
             enEnemySkill_533==m_iSkId ||
             enEnemySkill_534==m_iSkId )
        
    {
        m_occurTimes = 1;
        m_nOccurOdds = 100;
    }
    else if (enEnemySkill_321 == m_iSkId)
    {
        m_nOccurOdds = 15;
    }
    else if (enEnemySkill_408 == m_iSkId || enEnemySkill_412 == m_iSkId)
    {
        m_nOccurOdds = 30;
    }
    else if (enEnemySkill_409 == m_iSkId)
    {
        m_nOccurOdds = 10;
    }
    else if (enEnemySkill_424 == m_iSkId || enEnemySkill_425 == m_iSkId)
    {
        m_nOccurOdds = 25;
    }
    else
    {
        m_nOccurOdds = 40;
    }
}

void EsDuiHua::action()
{
    --m_occurTimes;
    CAttackInfo atkInfo = m_pEnemy->getAtkInfo();
    atkInfo.iManSkillID = m_iSkId;
    atkInfo.bAtkOvr = true;
    m_pEnemy->setAtkInfo(atkInfo);
    m_pEnemy->sentAtkInfo();
}

bool EsDuiHua::canExecute()
{
    ++m_runCounts;
    if (m_occurTimes < 1) return false;
    bool ret = SaveData::Inst()->getrand() < getOccurOdds();
    if (enEnemySkill_123 == m_iSkId || enEnemySkill_124 == m_iSkId || enEnemySkill_244==m_iSkId)
    {
        FightMngr* tFightM = (FightMngr*)(m_pEnemy->getParent()->getParent());
        Player* tPlayer = tFightM->getPlayer();
        if (ret && (tPlayer->getArmorCD() >1 || tPlayer->getEnhanceCD() >1 || tPlayer->getEnWujiangCD() >1)) {
            tPlayer->setArmorCD(1);
            tPlayer->setEnhanceCD(1);
            tPlayer->setEnWujiangCD(1);
            //tFightM->getEnemyGrp()->showAtkTip();
            return ret;
        }else
            return false;
    }
    if (enEnemySkill_201==m_iSkId)
    {
        return m_pEnemy->getBaseHp()<=m_pEnemy->getCurHp();
    }
    if (enEnemySkill_209==m_iSkId)
    {
        return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp())<=0.14f;
    }
    if (enEnemySkill_224==m_iSkId)
    {
        return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp())<0.5f;
    }
    if (enEnemySkill_230==m_iSkId)
    {
        return (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp())<=0.24f;
    }
    if (enEnemySkill_240==m_iSkId)
    {
        return m_pEnemy->getBaseHp()<=m_pEnemy->getCurHp();
    }
    if (enEnemySkill_266==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp())<0.4f;
    }
    if (enEnemySkill_438==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.24f);
    }
    if (enEnemySkill_446==m_iSkId)
    {
        return ret && (m_pEnemy->getCurHp() == m_pEnemy->getBaseHp());
    }
    
    if (enEnemySkill_165 == m_iSkId)
    {
        if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() <= 0.24 && ret)
        {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_168)
                return true;
            else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_165)
                return false;
            else
                return true;
        }
        else
            return false;
    }
    if (enEnemySkill_344 == m_iSkId) {
        if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.50f && ret) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_345) {
                return true;
            }else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_344)
                return false;
            else
                return true;
        }
        return false;
    }
    if (enEnemySkill_297 == m_iSkId) {
        if (ret) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_298)
                return true;
            else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_297)
                return false;
            else
                return true;
        }
        return false;
    }
    if (enEnemySkill_376 == m_iSkId) {
        if (m_pEnemy->getCurHp()*1.0/m_pEnemy->getBaseHp() < 0.5f && ret) {
            if (m_pEnemy->getSkillUseLastID() == enEnemySkill_377)
                return true;
            else if (m_pEnemy->getSkillUseLastID() == enEnemySkill_376)
                return false;
            else
                return true;
        }
        return false;
    }
    
    return ret;
}


#pragma mark -
#pragma mark EnemySkillPool
EnemySkill* EnemySkillPool::createEnemySkill(int skid, Enemy* pEnemy)
{
    EnemySkill* pEnemySkill = NULL;
    
    switch (skid)
    {
        case enEnemySkill_LianHuan:     //1 连环 x%几率2次100%攻击
            pEnemySkill = new EsLianHuan(skid, pEnemy);
            break;
            
        case enEnemySkill_LianDa:       //2 连打 x%几率2次70%攻击
        case enEnemySkillNanManRuQin:
        case enEnemySkillluanshitianxia:
        case enEnemySkillluanshitianxia2:
        case enEnemySkill5lianji:
        case enEnemySkillNanManRuQin2:
        case enEnemySkillEYunYiJi:
        case enEnemySkill_LianJi82:
        case enEnemySkill_LianJi83:
        case enEnemySkill_LianJi7_3:
        case enEnemySkill_LianJi45_4:
        case enEnemySkill_LianJi75_3:
        case enEnemySkill_115:
        case enEnemySkill_119:
        case enEnemySkill_125:
        case enEnemySkill_132:
        case enEnemySkill_141:
        case enEnemySkill_143:
        case enEnemySkill_147:
        case enEnemySkill_151:
        case enEnemySkill_154:
        case enEnemySkill_158:
        case enEnemySkill_163:
        case enEnemySkill_164:
        case enEnemySkill_166:
        case enEnemySkill_170:
        case enEnemySkill_194:
        case enEnemySkill_197:
        case enEnemySkill_208:
        case enEnemySkill_210:
        case enEnemySkill_215:
        case enEnemySkill_218:
        case enEnemySkill_220:
        case enEnemySkill_222:
        case enEnemySkill_228:
        case enEnemySkill_225:
        case enEnemySkill_239:
        case enEnemySkill_246:
        case enEnemySkill_248:
        case enEnemySkill_253:
        case enEnemySkill_267:
        case enEnemySkill_268:
        case enEnemySkill_269:
        case enEnemySkill_271:
        case enEnemySkill_275:
        case enEnemySkill_276:
        case enEnemySkill_277:
        case enEnemySkill_285:
        case enEnemySkill_292: //lvgenquan
        case enEnemySkill_300: //lvgenquan
        case enEnemySkill_350:
        case enEnemySkill_351:
        case enEnemySkill_302:
        case enEnemySkill_307:
        case enEnemySkill_309:
        case enEnemySkill_312:
        case enEnemySkill_320:
        case enEnemySkill_329:
        case enEnemySkill_345:
        case enEnemySkill_354:
        case enEnemySkill_358:
        case enEnemySkill_364:
        case enEnemySkill_370:
        case enEnemySkill_380:
        case enEnemySkill_382:
        case enEnemySkill_388:
        case enEnemySkill_389:
        case enEnemySkill_392:
        case enEnemySkill_399:
        case enEnemySkill_403:
        case enEnemySkill_407:
        case enEnemySkill_418:
        case enEnemySkill_423:
        case enEnemySkill_429:
        case enEnemySkill_431:
        case enEnemySkill_433:
        case enEnemySkill_448:
        case enEnemySkill_451:
        case enEnemySkill_454:
        case enEnemySkill_466:
        case enEnemySkill_467:
        case enEnemySkill_474:
        case enEnemySkill_481:
        case enEnemySkill_483:
        case enEnemySkill_485:
        case enEnemySkill_487:
        case enEnemySkill_489:
        case enEnemySkill_491:
        case enEnemySkill_495:
        case enEnemySkill_497:
        case enEnemySkill_499:
            case enEnemySkill_513:
            case enEnemySkill_523:
            case enEnemySkill_527:
            case enEnemySkill_529:
            case enEnemySkill_530:
            pEnemySkill = new EsLianDa(skid, pEnemy);
            break;
            
        case enEnemySkill_116:
        case enEnemySkill_198:
        case enEnemySkill_331://lvgenquan
        case enEnemySkill_319:
        case enEnemySkill_327:
        case enEnemySkill_366:
        case enEnemySkill_371:
        case enEnemySkill_375:
        case enEnemySkill_457:
            pEnemySkill = new EsScaleAtk(skid, pEnemy);
            break;
            
        case enEnemySkill_ZhongJi:      //3 重击 30%几率1次125%攻击
        case enEnemySkill_131:
        case enEnemySkill_138:
        case enEnemySkill_161:
        case enEnemySkill_168:
        case enEnemySkill_169:
        case enEnemySkill_199:
        case enEnemySkill_204:
        case enEnemySkill_212:
        case enEnemySkill_216:
        case enEnemySkill_229:
        case enEnemySkill_231:
        case enEnemySkill_237:
        case enEnemySkill_241:
        case enEnemySkill_251:
        case enEnemySkill_257:
        case enEnemySkill_262:
        case enEnemySkill_264:
        case enEnemySkill_273:
        case enEnemySkill_280:
        case enEnemySkill_283:
        case enEnemySkill_286:   //lvgenquan
        case enEnemySkill_332://lvgenquan
        case enEnemySkill_334://lvgenquan
        case enEnemySkill_335://lvgenquan
        case enEnemySkill_338:
        case enEnemySkill_305:
        case enEnemySkill_328:
        case enEnemySkill_298:
        case enEnemySkill_359:
        case enEnemySkill_360:
        case enEnemySkill_367:
        case enEnemySkill_372:
        case enEnemySkill_377:
        case enEnemySkill_383:
        case enEnemySkill_384:
        case enEnemySkill_385:
        case enEnemySkill_386:
        case enEnemySkill_394:
        case enEnemySkill_396:
        case enEnemySkill_397:
        case enEnemySkill_410:
        case enEnemySkill_413:
        case enEnemySkill_415:
        case enEnemySkill_420:
        case enEnemySkill_426:
        case enEnemySkill_439:
        case enEnemySkill_458:
        case enEnemySkill_461:
        case enEnemySkill_462:
        case enEnemySkill_493:
            pEnemySkill = new EsZhongJi(skid, pEnemy);
            break;
            
        case enEnemySkill_QiangXi:      //4 强袭 30%几率1次150%攻击
            case enEnemySkill_518:
            pEnemySkill = new EsQiangXi(skid, pEnemy);
            break;
            
        case enEnemySkill_SuiGu:        //5 碎骨 30%几率1次200%攻击
        case enEnemySkillMengShowLuanWu:
        case enEnemySkillSuiGu2:
        case enEnemySkillSuiGu3:
            pEnemySkill = new EsSuiGu(skid, pEnemy);
            break;

        case enEnemySkill_WuShuang:     //7 无双 x%几率5次45%攻击
            pEnemySkill = new EsWuShuang(skid, pEnemy);
            break;
            
        case enEnemySkillHuoXiJin:       //8 封印火系武将,随机2-4回合
        case enEnemySkillShuiXiJin:      //9 封印水系武将,随机2-4回合
        case enEnemySkillMuXiJin:        //10 封印木系武将,随机2-4回合
        case enEnemySkillGuangXiJin:     //11 封印光系武将,随机2-4回合
        case enEnemySkillAnXiJin:        //12 封印暗系武将,随机2-4回合
        case enEnemySkillShenJin:        //13 封印神系武将,随机2-4回合
        case enEnemySkillLuanJin:        //14 随机封印2-4个武将,随机2-4回合
        case enEnemySkillZhuQueQi:
        case enEnemySkillHuoXiJin40:       //41 封印火系武将,随机2-4回合
        case enEnemySkillShuiXiJin40:      //42 封印水系武将,随机2-4回合
        case enEnemySkillMuXiJin40:        //43 封印木系武将,随机2-4回合
        case enEnemySkillGuangXiJin40:     //44 封印光系武将,随机2-4回合
        case enEnemySkillAnXiJin40:        //45 封印暗系武将,随机2-4回合
        case enEnemySkillShenJin40:        //46 封印神系武将,随机2-4回合
        case enEnemySkillLuanJin40:        //47 随机封印2-4个武将,随机2-4回合
        case enEnemySkillHuoXiJin10:
        case enEnemySkillShuiXiJin10:
        case enEnemySkillMuXiJin10:
        case enEnemySkillGuangXiJin10:
        case enEnemySkillAnXiJin10:
        case enEnemySkillShenJin10:
        case enEnemySkillLuanJin10:
        case enEnemySkillZhuQueQi2:
        case enEnemySkillBingFengShiKe:
        case enEnemySkillBingFengShiKe2:
        case enEnemySkillZhuQueQi3:
        case enEnemySkillShuiXiJin1_3:
        case enEnemySkillHuoXiJin1_3:
        case enEnemySkillMuXiJin1_3:
        case enEnemySkillGuangXiJin1_3:
        case enEnemySkillAnXiJin1_3:
        case enEnemySkillShenJin1_3:
        case enEnemySkillLuanJin3_23:
        case enEnemySkillLuanJinRun2_3_23:
        case enEnemySkill_113:
        case enEnemySkill_114:
        case enEnemySkill_120:
        case enEnemySkill_121:
        case enEnemySkill_126:
        case enEnemySkill_127:
        case enEnemySkill_148:
        case enEnemySkill_152:
        case enEnemySkill_153:
        case enEnemySkill_155:
        case enEnemySkill_160:
        case enEnemySkill_191:
        case enEnemySkill_192:
        case enEnemySkill_193:
        case enEnemySkill_211:
        case enEnemySkill_213:
        case enEnemySkill_219:
        case enEnemySkill_223:
        case enEnemySkill_227:
        case enEnemySkill_233:
        case enEnemySkill_242:
        case enEnemySkill_243:
        case enEnemySkill_245:
        case enEnemySkill_254:
        case enEnemySkill_258:
        case enEnemySkill_260:
        case enEnemySkill_263:
        case enEnemySkill_270:
        case enEnemySkill_284:
        case enEnemySkill_291://lvgenquan
        case enEnemySkill_339:
        case enEnemySkill_341:
        case enEnemySkill_347:
        case enEnemySkill_310:
        case enEnemySkill_317:
        case enEnemySkill_323:
        case enEnemySkill_324:
        case enEnemySkill_353:
        case enEnemySkill_363:
        case enEnemySkill_369:
        case enEnemySkill_374:
        case enEnemySkill_379:
        case enEnemySkill_387:
        case enEnemySkill_393:
        case enEnemySkill_395:
        case enEnemySkill_401:
        case enEnemySkill_406:
        case enEnemySkill_440:
        case enEnemySkill_436:
        case enEnemySkill_441:
        case enEnemySkill_442:
        case enEnemySkill_452:
        case enEnemySkill_469:
        case enEnemySkill_470:
        case enEnemySkill_471:
        case enEnemySkill_472:
        case enEnemySkill_473:
        case enEnemySkill_478:
        case enEnemySkill_501:
            case enEnemySkill_504:
            case enEnemySkill_505:
            case enEnemySkill_516:
            case enEnemySkill_526:
        {
            pEnemySkill = new EsFengYin(skid, pEnemy);
        }
            break;
            
        case enEnemySkillZhiYu:          //15 当血量低于30%,100%回复自己50%的血量
        case enEnemySkill_279:
        case enEnemySkill_282:
        case enEnemySkill_475:
        case enEnemySkill_476:
        {
            pEnemySkill = new EsZhiYu(skid,pEnemy);
        }
            break;
            
        case enEnemySkillFuHuo:          //15 当血量低于30%,100%回复自己100%的血量
        case enEnemySkillZhenFuHuo:
        case enEnemySkillFuHuo2:
        {
            pEnemySkill = new EsFuHuo(skid,pEnemy);
        }
            break;
            
        case enEnemySkillNiZhuanHuo:     //17 将火宝珠转换成无效块
        case enEnemySkillNiZhuanShui:    //18 将水宝珠转换成无效块
        case enEnemySkillNiZhuanMu:      //19 将木宝珠转换成无效块
        case enEnemySkillNiZhuanGuang:   //20 将光宝珠转换成无效块
        case enEnemySkillNiZhuanAn:      //21 将暗宝珠转换成无效块
        case enEnemySkillNiZhuanHuiFu:   //22 将回复宝珠转换成无效块
        case enEnemySkillNiZhuanTianXia: //23 随机将一种宝珠转化成无效块
        case enEnemySkillBaoZhuYinCang:
        case enEnemySkillShiguangnizhuan:
        case enEnemySkillkengdiezhuanshui: 
        case enEnemySkillkengdiezhuanhuo:  
        case enEnemySkillkengdiezhuanmu:   
        case enEnemySkillkengdiezhuanguang:
        case enEnemySkillkengdiezhuanan:
        case enEnemySkillkengdiezhuanshui2:
        case enEnemySkillkengdiezhuanhuo2:
        case enEnemySkillkengdiezhuanmu2:
        case enEnemySkillkengdiezhuanguang2:
        case enEnemySkillkengdiezhuanan2:
        case enEnemySkillNiZhuanTianXia2:
        case enEnemySkill_117:
        case enEnemySkill_134:
        case enEnemySkill_135:
        case enEnemySkill_136:
        case enEnemySkill_139:
        case enEnemySkill_140:
        case enEnemySkill_150:
        case enEnemySkill_162:
        case enEnemySkill_167:
        case enEnemySkill_196:
        case enEnemySkill_203:
        case enEnemySkill_207:
        case enEnemySkill_234:
        case enEnemySkill_255:
        case enEnemySkill_259:
        case enEnemySkill_265:
        case enEnemySkill_278:
        case enEnemySkill_281:
        case enEnemySkill_288: //lvgenquan
        case enEnemySkill_289: //lvgenquan
        case enEnemySkill_290: //lvgenquan
        case enEnemySkill_293: //lvgenquan
        case enEnemySkill_295: //lvgenquan
        case enEnemySkill_342:
        case enEnemySkill_343:
        case enEnemySkill_348:
        case enEnemySkill_349:
        case enEnemySkill_301:
        case enEnemySkill_304:
        case enEnemySkill_313:
        case enEnemySkill_322:
        case enEnemySkill_326:
        case enEnemySkill_356:
        case enEnemySkill_362:
        case enEnemySkill_381:
        case enEnemySkill_391:
        case enEnemySkill_402:
        case enEnemySkill_404:
        case enEnemySkill_405:
        case enEnemySkill_414:
        case enEnemySkill_411:
        case enEnemySkill_416:
        case enEnemySkill_419:
        case enEnemySkill_417:
        case enEnemySkill_422:
        case enEnemySkill_434:
        case enEnemySkill_437:
        case enEnemySkill_443:
        case enEnemySkill_449:
        case enEnemySkill_453:
        case enEnemySkill_465:
        case enEnemySkill_480:
        case enEnemySkill_482:
        case enEnemySkill_484:
        case enEnemySkill_486:
        case enEnemySkill_488:
        case enEnemySkill_490:
        case enEnemySkill_492:
        case enEnemySkill_494:
        case enEnemySkill_496:
        case enEnemySkill_498:
        case enEnemySkill_502:
            case enEnemySkill_503:
            case enEnemySkill_507:
            case enEnemySkill_511:
            case enEnemySkill_517:
            case enEnemySkill_522:
            case enEnemySkill_525:
            case enEnemySkill_531:
            pEnemySkill = new EsNiZhuan(skid,pEnemy);
            break;
            
        case enEnemySkillWuShengHuYou:   //24 给自己添加一个buff，持续10回合免疫一切异常状态，包括：威吓、破甲、毒
        case enEnemySkillShenLinBiYou:   //25 给自己添加一个buff，持续999回合免疫一切异常状态，包括：威吓、破甲、毒
        case enEnemySkillMuTengJia:
        case enEnemySkillMuTengJia2:
        case enEnemySkillMuTengJia3:
        case enEnemySkillQuYan:
        case enEnemySkillDuanLiu:
        case enEnemySkillPoMu:
        case enEnemySkillZhanLei:
        case enEnemySkillShiHun:
        case enEnemySkill_156:
        case enEnemySkill_206:
        case enEnemySkill_217:
        case enEnemySkill_226:
        case enEnemySkill_272:
        case enEnemySkill_340://lvgenquan
        case enEnemySkill_311:
        case enEnemySkill_325:
        case enEnemySkill_390:
        case enEnemySkill_427:
        case enEnemySkill_430:
        case enEnemySkill_435:
        case enEnemySkill_463:
            case enEnemySkill_510:
            pEnemySkill = new EsHuJia(skid,pEnemy);
            break;
            
        case enEnemySkillXuShiDaiFa:
        case enEnemySkillHaoYin:
        case enEnemySkillHaoYin2:
        case enEnemySkillXingYunYiJi:
        case enEnemySkillShiJianNingJie:
        case enEnemySkill_238:
        case enEnemySkill_250:
        case enEnemySkill_294://lvgenquan
        case enEnemySkill_316:
            case enEnemySkill_508:
         
        {
            pEnemySkill = new EsXuShiDaiFa(skid,pEnemy);
        }
            break;
            
        case enEnemySkillShaShouJian:
        {
            pEnemySkill = new EsShaShowJian(skid,pEnemy);
        }
            break;
            
        case enEnemySkillJiNu:
        case enEnemySkillKuangBao:
        case enEnemySkillGongJiZiTai:
        case enEnemySkillBaoNu:
        case enEnemySkillJiNu2:
        case enEnemySkillBaoZou:
        case enEnemySkillJiNu3:
        case enEnemySkill_118:
        case enEnemySkill_122:
        case enEnemySkill_142:
        case enEnemySkill_133:
        case enEnemySkill_137:
        case enEnemySkill_146:
        case enEnemySkill_149:
        case enEnemySkill_145:
        case enEnemySkill_157:
        case enEnemySkill_159:
        case enEnemySkill_202:
        case enEnemySkill_232:
        case enEnemySkill_247:
        case enEnemySkill_249:
        case enEnemySkill_252:
        case enEnemySkill_287: //lvgenquan
        case enEnemySkill_303:
        case enEnemySkill_306:
        case enEnemySkill_314:
        case enEnemySkill_352:
        case enEnemySkill_355:
        case enEnemySkill_365:
        case enEnemySkill_368:
        case enEnemySkill_428:
        case enEnemySkill_447:
        case enEnemySkill_456:
        case enEnemySkill_460:
        case enEnemySkill_464:
        case enEnemySkill_479:
        case enEnemySkill_468:
        case enEnemySkill_477:
        {
            pEnemySkill = new EsJiNuKuangBao(skid,pEnemy);
        }
            break;
            
        case enEnemySkillWoHenYing:
        case enEnemySkillTaGengYing:
        case enEnemySkillTu:
        case enEnemySkillshuohua20:
        case enEnemySkill_123:
        case enEnemySkill_124:
        case enEnemySkill_171:
        case enEnemySkill_172:
        case enEnemySkill_173:
        case enEnemySkill_174:
        case enEnemySkill_175:
        case enEnemySkill_176:
        case enEnemySkill_177:
        case enEnemySkill_178:
        case enEnemySkill_179:
        case enEnemySkill_180:
        case enEnemySkill_181:
        case enEnemySkill_182:
        case enEnemySkill_183:
        case enEnemySkill_184:
        case enEnemySkill_185:
        case enEnemySkill_186:
        case enEnemySkill_187:
        case enEnemySkill_188:
        case enEnemySkill_189:
        case enEnemySkill_190:
        case enEnemySkill_201:
        case enEnemySkill_209:
        case enEnemySkill_224:
        case enEnemySkill_230:
        case enEnemySkill_235:
        case enEnemySkill_236:
        case enEnemySkill_240:
        case enEnemySkill_256:
        case enEnemySkill_266:
        case enEnemySkill_274:
        case enEnemySkill_244:
        case enEnemySkill_165:
        case enEnemySkill_344:
        case enEnemySkill_297:
        case enEnemySkill_321:
        case enEnemySkill_376:
        case enEnemySkill_408:
        case enEnemySkill_409:
        case enEnemySkill_412:
        case enEnemySkill_424:
        case enEnemySkill_425:
        case enEnemySkill_438:
        case enEnemySkill_446:
            case enEnemySkill_514:
            case enEnemySkill_515:
            case enEnemySkill_532:
            case enEnemySkill_533:
            case enEnemySkill_534:
        {
            pEnemySkill = new EsDuiHua(skid,pEnemy);
        }
            break;
            
        case enEnemySkillxinjinganghuti:
        case enEnemySkillxinjinganghuti2:
        {
            pEnemySkill = new EsBuffOnly(skid,pEnemy);
        }
            break;
            
        case enEnemySkillTongGuiYuJin:
        case enEnemySkillDianGuangHuoShi:
        case enEnemySkillZiBao:
        case enEnemySkillZiBao2:
        case enEnemySkill_128:
        case enEnemySkill_129:
        case enEnemySkill_130:
        case enEnemySkill_214:
        case enEnemySkill_221:
        case enEnemySkill_261:
        case enEnemySkill_195:
        case enEnemySkill_200:
        case enEnemySkill_205:
            
        case enEnemySkill_330://lvgenquan
        case enEnemySkill_336:
        case enEnemySkill_337:
        case enEnemySkill_333: //lvgenquan
        case enEnemySkill_346:
        case enEnemySkill_308:
        case enEnemySkill_315:
        case enEnemySkill_296://lvgenquan
        case enEnemySkill_299:
        case enEnemySkill_318:
        case enEnemySkill_357:
        case enEnemySkill_361:
        case enEnemySkill_373:
        case enEnemySkill_398:
        case enEnemySkill_432:
        case enEnemySkill_444:
        case enEnemySkill_450:
        case enEnemySkill_455:
        case enEnemySkill_378:
        case enEnemySkill_400:
        case enEnemySkill_459:
        case enEnemySkill_445:
        case enEnemySkill_421:
             case enEnemySkill_500:
            case enEnemySkill_506:
            case enEnemySkill_512:
            case enEnemySkill_509:
            case enEnemySkill_519:
            case enEnemySkill_520:
            case enEnemySkill_521:
            case enEnemySkill_524:
            case enEnemySkill_528:
            pEnemySkill = new EsTongGuiYuJin(skid,pEnemy);
            break;
            
        case enEnemySkill_YiShan:        //6 一闪 30%几率刷新宝石方阵(遗弃)
        default:
            break;
    }
    
    return pEnemySkill;
}







