//
//  Enemy.cpp
//  CardDemo
//
//  Created by Hao Zhang on 12-4-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "Enemy.h"
#include "CCShake.h"
#include "CHurtNum.h"
#include "CSkillLib.h"
#include "SimpleAudioEngine.h"
#include "CGamedata.h"
#include "FightMngr.h"

using namespace CocosDenshion;

#define KBUFFTAGNUMBER    9

//#define kBuffTag_Mabi   0xabcd
//#define kBuffTag_Delay  0x1234
//#define kBuffTag_Sick   0xcdef

//******************************************************************************
// Construct
//******************************************************************************
Enemy::Enemy(CDgMonsterInfo* pMonInfo, CCPoint& pos, CCObject* target, SEL_CallFuncO selector, int index)
{
    m_currentAtk = 0;
    
    m_iAtkRatio = 1.0f;
    m_iState = EN_ENEMY_STATE_NORMAL;
    m_iActTimes = 1;
    m_actSkillID = -1;
    m_iDoSpecilAtk = EN_ENEMY_NONE;
    m_iCurAtkType = EN_ENEMY_NONE;
    for(int i = 0; i < MAX_BUFF_ITEMS; i++)
    {
        m_buffArr[i].cd = 0;
        m_buffArr[i].buffID = EN_ENEMY_NONE;
        m_buffArr[i].spBuff = NULL;
        m_buffArr[i].showCd = false;
    }
    
    m_skillArr = NULL;
    m_index = index;
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_lbPoison = NULL;
    m_lbSick = NULL;
    m_pSecondBlood = NULL;
    m_iSecondElement = -1;
    
    m_scid = pMonInfo->cid;
    m_iBaseHP = pMonInfo->hp;
    m_iAttackPt = pMonInfo->attack;
    
    m_iRoundMax = pMonInfo->turn;
    m_iArmor = pMonInfo->defense;
    m_iArmorMax = pMonInfo->defense;
    m_iDrop_Gold = pMonInfo->drop_gold;
    m_iElement = pMonInfo->ctype;
    m_iSecondElement = pMonInfo->mtype;
    m_iDefaultElm = m_iElement;
    m_iExp = pMonInfo->drop_exp;
    
    for (int i=0; i<ENEMYBUFFMAX; i++)
    {
        m_iBuffIdArr[i] = pMonInfo->buffArr[i];
    }
    
    m_iCurRound = pMonInfo->init_attack_turn;
    m_iHP = m_iBaseHP;
    m_bWudi = false;
    m_bDrop_plus = pMonInfo->drop_plus;
    
    CCardBaseInfo* pCardInf = CGameData::Inst()->getCardBaseInfByCid(pMonInfo->drop_cid);
    if(pCardInf){
        m_iDropStar = pCardInf->star;
    }
    else {
        m_iDropStar = 0;
    }
    
    
    init(pos);
    
    m_iSkillUseLastID = 0;
    int skillNum = 0;
    
    // m_iAttackPt = 599999;//shikun
    //  m_iBaseHP = 150;
//       if (index == 0) {
//            m_iBuffIdArr[0] = 73;
//           m_iBuffIdArr[1] = 74;
//       }
//    
//       if (index == 0) {
//          pMonInfo->skidArr[0] = 346;
//          pMonInfo->skidArr[1] = 347;
//          pMonInfo->skidArr[2] = 351;
//           pMonInfo->skidArr[3] = 349;
//            pMonInfo->skidArr[4] = 350;
//          pMonInfo->skidArr[5] = 348;
//        }
  // pMonInfo->skidArr[0] = enEnemySkill_509;

    for (int i=0; i<ENEMYSKILLMAX; i++)
    {
       if (pMonInfo->skidArr[i] == 0)
       {
           break;
       }
        
       skillNum++;
    }
    
    m_skillArr = new CCMutableArray<EnemySkill*>();
    
    if (skillNum>0)
    {
        for (int i=0; i<skillNum; i++)
        {
            EnemySkill* pSkill = EnemySkillPool::Inst()->createEnemySkill(pMonInfo->skidArr[i], this);
            if (pSkill != NULL)
            {
                addChild(pSkill);
                pSkill->release();
            }
            
            m_skillArr->addObject(pSkill);
        }
    }
    
    m_isLianFan2 = true; //新增
    
    buffSelfInit();
}

Enemy::Enemy(SaveData_Enemy* pMonInfo, CCPoint& pos, CCObject* target, SEL_CallFuncO selector, int index)
{
   // m_currentAtk = 0;
   
    
    m_iAtkRatio = 1.0f;
    m_iActTimes = 1;
    m_actSkillID = -1;
    m_iDoSpecilAtk = EN_ENEMY_NONE;
    m_iCurAtkType = EN_ENEMY_NONE;
    for(int i = 0; i < MAX_BUFF_ITEMS; i++)
    {
        m_buffArr[i].cd = 0;
        m_buffArr[i].buffID = EN_ENEMY_NONE;
        m_buffArr[i].spBuff = NULL;
        m_buffArr[i].showCd = false;
    }
    
    m_skillArr = NULL;
    m_index = index;
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_lbPoison = NULL;
    m_lbSick = NULL;
    m_pSecondBlood = NULL;
    m_iSecondElement = -1;
    
    m_scid = pMonInfo->cid;
    m_iBaseHP = pMonInfo->base_hp;
    m_iAttackPt = pMonInfo->attack;
    m_iRoundMax = pMonInfo->round_max;
    m_iArmor = pMonInfo->cur_armor;
    m_iArmorMax = pMonInfo->base_armor;
    m_iDrop_Gold = pMonInfo->drop_gold;
    m_iElement = pMonInfo->element;
    m_iSecondElement = pMonInfo->shSecondElement;
    m_iDefaultElm = pMonInfo->element_default;
    m_iExp = pMonInfo->drop_exp;
    
    for (int i=0; i<ENEMYBUFFMAX; i++)
    {
        m_iBuffIdArr[i] = pMonInfo->buffArr[i];
    }
    
    m_iState = pMonInfo->state;
    m_iCurRound = pMonInfo->cur_round;
    m_iDropStar = pMonInfo->drop_star;
    m_bDrop_plus = pMonInfo->drop_plus;
    m_iHP = pMonInfo->cur_hp;
    
    m_bWudi = false;
    
    m_isLianFan2 = true; //新增
    
    init(pos);
    
    for (int i=0; i<MAX_BUFF_ITEMS; i++)
    {
        m_buffArr[i].buffID = pMonInfo->buffId[i];
        m_buffArr[i].cd = pMonInfo->buffCd[i];
        m_buffArr[i].showCd = pMonInfo->buffShow[i] == 0 ? false:true;
       
    }
    if (isAlive())
    {
        AddBuffs();
    }
    
    m_iSkillUseLastID = pMonInfo->iSkillUseLastID;
    
    m_currentAtk = pMonInfo->enemyHurt;
    
  //  pMonInfo->skidArr[0] = enEnemySkill_512;
    int skillNum = 0;
    for (int i=0; i<ENEMYSKILLMAX; i++)
    {
        if (pMonInfo->skidArr[i] == 0)
        {
            break;
        }
        skillNum++;
    }
    
    m_skillArr = new CCMutableArray<EnemySkill*>();
    
    if (skillNum>0)
    {
        for (int i=0; i<skillNum; i++)
        {
            EnemySkill* pSkill = EnemySkillPool::Inst()->createEnemySkill(pMonInfo->skidArr[i], this);
            if (pSkill != NULL)
            {
                addChild(pSkill);
                pSkill->release();
            }

            if (pMonInfo->skill_occurTimes[i] != -1)
            {
               pSkill->set_occurTimes(pMonInfo->skill_occurTimes[i]);
            }
            if (pMonInfo->runCount[i] != 0)
            {
                pSkill->set_runCounts(pMonInfo->runCount[i]);
            }
            
            m_skillArr->addObject(pSkill);
        }
    }
}



//******************************************************************************
// ~Enemy
//******************************************************************************
Enemy::~Enemy()
{
    if (m_skillArr != NULL)
    {
        delete m_skillArr;
    }
    
    CCLog("Enemy::~Enemy");
}




void Enemy::buffCheckAfterGrp()
{
    if (!isAlive())
        return;
    
    for (int i=0; i<m_skillArr->count(); i++)
    {
        //金刚护体
        if (m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillxinjinganghuti2
            && !isBuffByIdx(EN_ENEMY_JINGANGHUTI)
            )
        {
            EnemyGrp* pEnemyGrp = (EnemyGrp*)m_pListener;
            for (int j=0; j < pEnemyGrp->getEnemyCount(); j++)
            {
                if (pEnemyGrp->getEnemyByIdx(j)->isAlive() && j!=m_index)
                {
                    setBuffByIdx(EN_ENEMY_JINGANGHUTI, 9999, -1);
                    pEnemyGrp->showSkillTip(this, enEnemySkillxinjinganghuti2);
                    break;
                }
            }
            
            if (isBuffByIdx(EN_ENEMY_JINGANGHUTI))
                break;
        }
    }
}

void Enemy::buffSelfInit()
{
    for (int i=0; i<m_skillArr->count(); i++)
    {
        //金刚护体
        if ((m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillxinjinganghuti
             ||m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillxinjinganghuti2)
            && !isBuffByIdx(EN_ENEMY_JINGANGHUTI)
            )
        {
            int hasJingangHuti = false;
            EnemyGrp* pEnemyGrp = (EnemyGrp*)m_pListener;
            for (int j=0; j < m_index; j++)
            {
                if (pEnemyGrp->getEnemyByIdx(j)->isAlive() && pEnemyGrp->getEnemyByIdx(j)->isBuffByIdx(EN_ENEMY_JINGANGHUTI))
                    {
                        hasJingangHuti = true;
                        break;
                    }
            }
            
            if (!hasJingangHuti)
            {
                setBuffByIdx(EN_ENEMY_JINGANGHUTI, 9999, -1);
                if (m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillxinjinganghuti)
                {
                    pEnemyGrp->showSkillTip(this, enEnemySkillxinjinganghuti);
                }
                else if (m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillxinjinganghuti2)
                {
                    pEnemyGrp->showSkillTip(this, enEnemySkillxinjinganghuti2);
                }
            }
        }
    }
}

//******************************************************************************
// init
//******************************************************************************
void Enemy::init(CCPoint& pos)
{    
    m_spDropItem = NULL;
    
    //enemy sprite
    
    char buf[100];
    m_spEnemy = CGameData::Inst()->getBodySprite(m_scid);
    CCPoint pt = pos;
    pt.y += m_spEnemy->getContentSize().height * 0.5;
    m_spEnemy->setPosition(pt);
    m_pos =pt;
    addChild(m_spEnemy);
    m_spEnemy->setOpacity(0);
    
    if (m_iHP > 0)
    {
        m_spEnemy->runAction(CCFadeIn::actionWithDuration(0.5));
    }
    
    if (m_iState == EN_ENEMY_STATE_NEAR_DIE)
        m_spEnemy->runAction(CCFadeTo::actionWithDuration(0.5,50));
    
    CCPoint ptEnemy = m_spEnemy->getPosition();
//    ptEnemy.y += 34.0f;
    //blood && second blood
    if (-1!=m_iSecondElement && CMonsterBlood::beElem(m_iSecondElement))
    {
        //CCPoint ptEnemy2 = CCPoint(ptEnemy.x,ptEnemy.y-34.0f);
        CCPoint ptEnemy2 = CCPoint(ptEnemy.x,ptEnemy.y);
        m_pSecondBlood = new CMonsterBlood(ptEnemy2, m_spEnemy->getContentSize(), m_iSecondElement);
        m_pSecondBlood->setBlood(getCurSecondHP(), getBaseSecondHP(), 0);
        m_pSecondBlood->loginBloodMove(this,callfuncND_selector(Enemy::endBloodMove));
        addChild(m_pSecondBlood);
        m_pSecondBlood->release();
        
        if (1>getCurSecondHP())
        {
            m_pSecondBlood->hide();
        }
        
    }
//    else
//    {
//        ptEnemy.y -= 34.0f;
//    }
    //1
    //blood
    m_blood = new CMonsterBlood(ptEnemy, m_spEnemy->getContentSize(), m_iDefaultElm);
    //m_blood->setBlood(m_iHP, m_iBaseHP, 0);
    m_blood->setBlood(getCurFirstHP(), getBaseFirstHP(), 0);
    m_blood->loginBloodMove(this,callfuncND_selector(Enemy::endBloodMove));
    addChild(m_blood);
    m_blood->release();
    if (1>getCurFirstHP())// && !m_pSecondBlood
    {
        m_blood->hide();
    }
    
//    m_blood->hide();
//    if (m_iHP > 0)
//    {
//        m_blood->fadein(0);
//    }
    
    //回合
    pt.x = m_blood->getLeftPosX();
    pt.y = 916;
    m_spRound = CCSprite::spriteWithSpriteFrameName("huihe.png");
    m_spRound->setAnchorPoint(CCPointMake(0, 1));
    m_spRound->setPosition(pt);
    addChild(m_spRound);
    m_spRound->setOpacity(0);
    if (m_iHP > 0)
    {
        m_spRound->runAction(CCFadeIn::actionWithDuration(0.5));
    }
    
    pt.x += m_spRound->getContentSize().width;
    snprintf(buf, 99, "%d", m_iCurRound.get());
    CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
    m_lbRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
    m_lbRound->setAnchorPoint(CCPointMake(0, 1));
    m_lbRound->setPosition(pt);
    addChild(m_lbRound);
    m_lbRound->setOpacity(0);
    if (m_iHP > 0)
    {
        m_lbRound->runAction(CCFadeIn::actionWithDuration(0.5));
    }
    
    m_spBullet = CCSprite::spriteWithSpriteFrameName("bullet.png");
    m_spBullet->setPosition(pt);
    m_spBullet->setOpacity(0);
    addChild(m_spBullet);
    
    memset(m_buffArr, 0, sizeof(m_buffArr));
    
    //drop item
    if(m_iDropStar > 0){
        if(m_iDropStar > 5)
            m_iDropStar = 5;
        snprintf(buf, 99, "droplv%d.png", m_iDropStar);
        m_spDropItem = CCSprite::spriteWithFile(buf);
        m_spDropItem->setAnchorPoint(CCPointMake(0.5, 0));
        m_spDropItem->setPosition(pos);
        addChild(m_spDropItem);
        
        if (m_bDrop_plus) {
            CCSprite* spDropPlus = CCSprite::spriteWithSpriteFrameName("icon_plus.png");
            spDropPlus->setPosition(CCPointMake(60, 20));
            m_spDropItem->addChild(spDropPlus);
        }
        
        if (m_iHP > 0 || m_iState == EN_ENEMY_STATE_NEAR_DIE)
        {
            m_spDropItem->setPosition(ccpAdd(pos, CCPointMake(0, 50)) );
            m_spDropItem->setIsVisible(false);
        }
    }
    
    m_spDropMoney = CCSprite::spriteWithSpriteFrameName("blank.png");
    m_spDropMoney->setAnchorPoint(CCPointMake(0.5, 0));
    m_spDropMoney->setScale(2.0);
    m_spDropMoney->setPosition(ccpAdd(pos, CCPointMake(0, 50)) );
    addChild(m_spDropMoney);
}


//addbuff from savedata
void Enemy::AddBuffs()
{
    for (int i=0; i<MAX_BUFF_ITEMS; i++)
    {
        switch (m_buffArr[i].buffID)
        {
            case EN_ENEMY_MABI:
            case EN_ENEMY_SICK:
            case EN_ENEMY_HUJIA:
            case EN_ENEMY_XUSHIDAIFA:
            case EN_ENEMY_SHASHOUJIAN:
            case EN_ENEMY_JINU:
            case EN_ENEMY_KUANGBAO:
            case EN_ENEMY_GONGJIZITAI:
            case EN_ENEMY_BAONU:
            case EN_ENEMY_HAOYIN:
            case EN_ENEMY_HAOYIN2:
            case EN_ENEMY_MUTENGJIA:
            case EN_ENEMY_JINGANGHUTI:
            case EN_ENEMY_JINU2:
            case EN_ENEMY_BAOZOU:
            case EN_ENEMY_XINGYUNYIJI:
            case EN_ENEMY_SHIJIANNINGJIE:
            case EN_ENEMY_DEFEN_FIRE:
            case EN_ENEMY_DEFEN_WATER:
            case EN_ENEMY_DEFEN_WOOD:
            case EN_ENEMY_DEFEN_LIGHT:
            case EN_ENEMY_DEFEN_AN:
            case EN_ENEMY_JINU3:
            case EN_ENEMY_FREEZE:
            case EN_ENEMY_BUFF118:
            case EN_ENEMY_BUFF122:
            case EN_ENEMY_BUFF133:
            case EN_ENEMY_BUFF137:
            case EN_ENEMY_BUFF142:
            case EN_ENEMY_BUFF146:
            case EN_ENEMY_BUFF149:
            case EN_ENEMY_BUFF145:
            case EN_ENEMY_BUFF157:
            case EN_ENEMY_BUFF159:
            case EN_ENEMY_BUFF202:
            case EN_ENEMY_BUFF232:
            case EN_ENEMY_XUSHIDAIFA238:
            case EN_ENEMY_BUFF247:
            case EN_ENEMY_XUSHIDAIFA250:
            case EN_ENEMY_BUFF249:
            case EN_ENEMY_BUFF252:
            case EN_ENEMY_BUFF287: //lvgenquan
            case EN_ENEMY_XUSHIDAIFA294://lvgenquan
            case EN_ENEMY_BUFF303:
            case EN_ENEMY_BUFF306:
            case EN_ENEMY_XUSHIDAIFA314:
            case EN_ENEMY_XUSHIDAIFA316:
            case EN_ENEMY_BUFF447:
            case EN_ENEMY_500THOUNSAND_LIMIT:
            case EN_ENEMY_XUSHIDAIFA508:
            {
                setBuffByIdx(m_buffArr[i].buffID, m_buffArr[i].cd,i);
            }
                break;
                
            case EN_ENEMY_CHGELM:
            {
                changeElement(m_iElement, m_buffArr[i].cd, i);
            }
                break;
                
            case EN_ENEMY_POISON:
                setPoison();
                break;
                
            case EN_ENEMY_POJIA:
            {
                breakArmor(m_iArmor, m_buffArr[i].cd,i);
            }
                break;
                
            case EN_ENEMY_DELAY:
            {
                setBuffByIdx(m_buffArr[i].buffID,m_buffArr[i].cd,i);
            }
                break;
                
                
            case EN_ENEMY_NONE:
            default:
                break;
        }
    }
}

//******************************************************************************
// isAlive
//******************************************************************************
bool Enemy::isAlive()
{
//    if(m_iHP > 0)
//        return true;
//    else
//        return false;
    return 0<m_iHP;
    
}

void Enemy::setEnemyState(int state)
{
    m_iState = state;
}

//******************************************************************************
// getElement
//******************************************************************************
int  Enemy::getElement()
{
    if (0<getCurFirstHP())
    {
        return m_iElement;
    }
    return m_iSecondElement;
}

//******************************************************************************
// getWidth
//******************************************************************************
int Enemy::getWidth()
{
    return m_spEnemy->getContentSize().width;
}

//******************************************************************************
// getHeight
//******************************************************************************
int Enemy::getHeight()
{
    return m_spEnemy->getContentSize().height;
}

//******************************************************************************
// getArmor
//******************************************************************************
int Enemy::getArmor()
{
    return m_iArmor;
}

//******************************************************************************
// getCurHp
//******************************************************************************
int Enemy::getCurHp()
{
    return m_iHP;
}

//******************************************************************************
// getCurState
//******************************************************************************
int Enemy::getCurState()
{
    return m_iState;
}

//******************************************************************************
// getBaseHp
//******************************************************************************
int Enemy::getBaseHp()
{
    return m_iBaseHP;
}

//******************************************************************************
// getCurRound
//******************************************************************************
int Enemy::getCurRound()
{
    return m_iCurRound;
}

//******************************************************************************
// getDropGold
//******************************************************************************
int Enemy::getDropGold()
{
    return m_iDrop_Gold;
}

//******************************************************************************
// getExp
//******************************************************************************
int Enemy::getExp()
{
    return m_iExp;
}

//******************************************************************************
// getAtkPt
//******************************************************************************
int Enemy::getAtkPt()
{
    return m_iAttackPt;
}

//******************************************************************************
// getSkid
//******************************************************************************
//int Enemy::getSkid()
//{
//    return m_iSkid;
//}

//******************************************************************************
// getBuffId
//******************************************************************************
int Enemy::getBuffId(int idx)
{
    //return m_iBuffId;
    return m_iBuffIdArr[idx];
}


bool Enemy::isDoctor()
{
    for (int i=0; i<m_skillArr->count(); i++)
    {
        if (m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillZhenFuHuo)
        {
            return true;
        }
    }
    
    return false;
}


void Enemy::ChkGrpBuffEffect(CAttackInfo* attackinfo)
{
    if (CheckGrpBuff(enEnemyBuff_shengguangdun))
    {
        EnemyGrp* pEnemyGrp = (EnemyGrp*)(m_pListener);
        pEnemyGrp->showAtkTip(CGameData::Inst()->getLanguageValue("enmeybuff_shengguangdun"));
        
        CAttackInfo atkInfo;
        atkInfo.clear();
//        atkInfo.iAttackPt = (int)((attackinfo->iAttackPt < m_iHP ? attackinfo->iAttackPt : m_iHP)*0.15f);
        atkInfo.iAttackPt = (int)((attackinfo->iAttackPt < m_iHP.get() ? attackinfo->iAttackPt.get() : m_iHP.get())*0.15f);
        atkInfo.iElement = enElementZero;
        pEnemyGrp->AtkDirectFromEnemy(this,atkInfo);
    }
    
    if ((CheckGrpBuff(enEnemyBuff_shihunhuo) && attackinfo->iElement == enElementFire)
        ||(CheckGrpBuff(enEnemyBuff_shihunshui) && attackinfo->iElement == enElementWater)
        ||(CheckGrpBuff(enEnemyBuff_shihunmu) && attackinfo->iElement == enElementWood)
        ||(CheckGrpBuff(enEnemyBuff_shihunguang) && attackinfo->iElement == enElementLight)
        ||(CheckGrpBuff(enEnemyBuff_shihunan) && attackinfo->iElement == enElementDark)
        )
    {
        attackinfo->iAttackPt *= -1;
        m_spEnemy->stopAllActions();
        m_spEnemy->runAction(CCSequence::actions(CCScaleTo::actionWithDuration(0.3f,1.3f),
                                                 CCScaleTo::actionWithDuration(0.3,1.0f),
                                                     NULL));
    }
}

/*
 *判断是否有连返2的敌将buf
 *
 */
bool Enemy::b_UseBuffLianfan2()
{
    FightMngr* fm = (FightMngr*)getParent()->getParent();
    
    if (CheckGrpBuff(enEnemyBuff_1lianbufan2) && (fm->getPlayer()->getCurCombo() == 1))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_2lianbufan2) && (fm->getPlayer()->getCurCombo() == 2))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_3lianbufan2) && (fm->getPlayer()->getCurCombo() == 3))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_4lianbufan2) && (fm->getPlayer()->getCurCombo() == 4))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_5lianbufan2) && (fm->getPlayer()->getCurCombo() == 5))
        return true;
    
    if ( CheckGrpBuff(enEnemyBuff_6lianbufan2) && (fm->getPlayer()->getCurCombo() == 6))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_7lianbufan2) && (fm->getPlayer()->getCurCombo() == 7))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_8lianbufan2) && (fm->getPlayer()->getCurCombo() == 8))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_9lianbufan2) && (fm->getPlayer()->getCurCombo() == 9))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_10lianbufan2) && (fm->getPlayer()->getCurCombo() == 10))
        return true;
    
    return false;
}

void Enemy::setLianFan(bool st)
{
    m_isLianFan2 = st;
}

//******************************************************************************
// beAttacked
//******************************************************************************
void Enemy::beAttacked(CAttackInfo* attackinfo, float dt)
{
    SaveData::Inst()->IncCurDamage(attackinfo->iAttackPt);
    
    ChkGrpBuffEffect(attackinfo);
    
    m_spEnemy->setPosition(m_pos);
    
    if ((attackinfo->iManSkillID == 0 || attackinfo->iManSkillID == 65535)
        && (attackinfo->iAutoSkillID == 0 || attackinfo->iAutoSkillID == 65535)
        && attackinfo->iAttackPt > 0
        )
    {
        if (bUseBuffLianfan())//如果满足连返的条件（一个敌将攻击力可以翻倍多次）
        {
            m_iCurRound = 1;
            m_iAtkRatio *= 2.0;
        }
        if (m_isLianFan2 && b_UseBuffLianfan2()) //如果满足连返2的条件（但一个敌将攻击力最多攻击翻倍一次）
        {
            m_isLianFan2 = false;
            m_iCurRound = 1;
            m_iAtkRatio *= 2.0;
        }
      
    }
    
//    m_iHP = m_iHP - attackinfo->iAttackPt;
//    
//    m_iHP = m_iHP > m_iBaseHP ? m_iBaseHP : m_iHP;
//    
//
//    //2
//    m_blood->setBlood(m_iHP, m_iBaseHP, dt);
    
    if (0<attackinfo->iAttackPt &&
        0<attackinfo->nEffectAttack)
    {
        if (!isBuffByIdx(attackinfo->nEffectType))
        {
            if (EnemyGrp* pEnemyGrp = dynamic_cast<EnemyGrp*>(m_pListener))
            {
                if (EN_ENEMY_POISON==attackinfo->nEffectType)
                {
                    pEnemyGrp->setPoisonPt(attackinfo->nEffectAttack);
                }
                else if (EN_ENEMY_FREEZE==attackinfo->nEffectType)
                {
                    pEnemyGrp->setFreezePt(attackinfo->nEffectAttack);
                }
                setBuffByIdx(attackinfo->nEffectType, attackinfo->chRoundCount, -1);
            }
        }
    }
   
  
    
    
    changeCurHPAction(-attackinfo->iAttackPt,dt,(enElementDef)attackinfo->iElement);
    
    
    stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(attackinfo->iManSkillID);
    bool bSkillGravity = false;
    if (pManSkill)
    {
        if (pManSkill->iType == enManSkillTp_gravity)
            bSkillGravity = true;
    }
    if(bSkillGravity || (attackinfo->iManSkillID == enManSkill_GuaGuLiaoShang))
    {
    }
    else
    {
        runAnim(attackinfo->iElement, dt);
    }
    
    chkActBeAttack();
    
    if(m_iHP <= 0)
    {
        m_isLianFan2 = true;
        checkOtherJinGangHuti();
        checkDie(dt);
    }
    else
    {
        m_spEnemy->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                                 CCShake::actionWithDuration(1.0, m_spEnemy->getPosition()),
                                                 NULL));
        
        if (attackinfo->iElement == enElementFire
            && CheckGrpBuff(enEnemyBuff_liushuifanshi))
        {
            EnemyGrp* pEnemyGrp = (EnemyGrp*)(m_pListener);
            pEnemyGrp->showAtkTip(CGameData::Inst()->getLanguageValue("enmeybuff_liushuifanshi"));
            
            CAttackInfo atkInfo;
            atkInfo.clear();
            atkInfo.iAttackPt = attackinfo->iAttackPt;
            atkInfo.iElement = enElementWater;
            pEnemyGrp->AtkDirectFromEnemy(this,atkInfo);
        } else if (CheckGrpBuff(enEnemyBuff_70)) {
            EnemyGrp* pEnemyGrp = (EnemyGrp*)(m_pListener);
            pEnemyGrp->showAtkTip(CGameData::Inst()->getLanguageValue("enEnemyBuffTitle_70"));
            
            CAttackInfo atkInfo;
            atkInfo.clear();
            atkInfo.iAttackPt = m_iAttackPt * 1.5;
            atkInfo.iElement = enElementFire;
            pEnemyGrp->AtkDirectFromEnemy(this,atkInfo);
        }
    }
}




int Enemy::getcurrentRoundAtk()
{
    return m_currentAtk;
}

void Enemy::setcurrentRoundAtk(int cAtx)
{
    m_currentAtk = cAtx;
    SaveData::Inst()->setEnemyBeAttrack(cAtx, SaveData::Inst()->getCurStep(), m_index); //新增
}


void Enemy::checkOtherJinGangHuti()
{
    int idx = getIndexByBufID(EN_ENEMY_JINGANGHUTI);
    if (idx < 0)
        return;
    
    bool allDieButHe = true;
    EnemyGrp* pEnemyGrp = (EnemyGrp*)m_pListener;
    for (int i=0; i < pEnemyGrp->getEnemyCount(); i++)
    {
        if (pEnemyGrp->getEnemyByIdx(i)->isAlive() && i != idx)
        {
            allDieButHe = false;
            break;
        }
    }
    
    if (allDieButHe)
    {
        pEnemyGrp->getEnemyByIdx(idx)->clearBuffByIdx(EN_ENEMY_JINGANGHUTI);
    }
}

int Enemy::getIndexByBufID(int bufId)
{
    EnemyGrp* pEnemyG = (EnemyGrp*)m_pListener;
    
    for(int i = 0; i < pEnemyG->getEnemyCount(); i++)
    {
        if (!pEnemyG->getEnemyByIdx(i)->isAlive())
            continue;
        
        if (pEnemyG->getEnemyByIdx(i)->isBuffByIdx(bufId))
            return i;
    }
    
    return -1;
}

void Enemy::elementChange()
{
    m_iElement++;
    if (m_iElement > enElementDark)
    {
        m_iElement = enElementWater;
    }
    m_blood->setElem(m_iElement);
}

void Enemy::elementChange2()
{
    int element = SaveData::Inst()->getrand()%5+1;
    if (m_iElement == element) {
        elementChange();
    }else{
        m_iElement = element;
    }
    //3
    m_blood->setElem(m_iElement);
}



bool Enemy::hasDoctorNotMe()
{
    EnemyGrp* pEnemyG = (EnemyGrp*)m_pListener;
    
    for(int i = 0; i < pEnemyG->getEnemyCount(); i++)
    {
        if (i == m_index)
        {
            continue;
        }
        
        if (pEnemyG->getEnemyByIdx(i)->getSkill() == NULL)
        {
            continue;
        }
        
        for (int j=0; j<pEnemyG->getEnemyByIdx(i)->getSkill()->count(); j++)
        {
            if (pEnemyG->getEnemyByIdx(i)->getSkill()->getObjectAtIndex(j)->getSkillID() == enEnemySkillZhenFuHuo
                && pEnemyG->getEnemyByIdx(i)->isAlive()
                )
            {
                return true;
            }
        }
    }
    
    return false;
}

void Enemy::checkDie(float dt)
{
    bool hasDoctor = hasDoctorNotMe();
    
    if (hasDoctor)
    {
        setEnemyState(EN_ENEMY_STATE_NEAR_DIE);
        
        m_spEnemy->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                                 CCShake::actionWithDuration(0.5, m_spEnemy->getPosition()),
                                                 CCFadeTo::actionWithDuration(0.5,50),
                                                 NULL));
    }
    else
    {
        if (isDoctor())
        {
            EnemyGrp* t_enemyGrp = (EnemyGrp*)(this->getParent());
            for (int j=0; j < t_enemyGrp->getEnemyCount(); j++)
            {
                if ((!t_enemyGrp->getEnemyByIdx(j)->isAlive())
                    &&(j != m_index)
                    )
                {
                    
                    t_enemyGrp->getEnemyByIdx(j)->realDie();
                    
                }
            }
        }
        
        m_spEnemy->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                                 CCShake::actionWithDuration(0.5, m_spEnemy->getPosition()),
                                                 CCFadeOut::actionWithDuration(0.5),
                                                 NULL));
    }
    m_lbRound->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                             CCFadeOut::actionWithDuration(1.0),
                                             NULL));
    m_spRound->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                             CCFadeOut::actionWithDuration(1.0),
                                             NULL));
    if (!hasDoctor)
    {
        setEnemyState(EN_ENEMY_STATE_DEAD);
        chkActionFromDie();
        
        moneyDrop(dt);
        
        //4
//        m_blood->fadeout(dt+0.5);
        fadeoutHP(dt+0.5f);
        
        
        if(m_spDropItem != NULL)
        {
            m_spDropItem->setIsVisible(false);
            m_spDropItem->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt + 1.0),
                                                        CCShow::action(),
                                                        CCMoveBy::actionWithDuration(0.1, CCPointMake(0, -50)),
                                                        CCMoveBy::actionWithDuration(0.1, CCPointMake(0, 20)),
                                                        CCMoveBy::actionWithDuration(0.1, CCPointMake(0, -20)),
                                                        CCMoveBy::actionWithDuration(0.1, CCPointMake(0, 5)),
                                                        CCMoveBy::actionWithDuration(0.1, CCPointMake(0, -5)),
                                                        NULL));
        }
    }
    
    clearBuffWithEnemyDie(dt);
}

void Enemy::realDie()
{
    setEnemyState(EN_ENEMY_STATE_DEAD);
    chkActionFromDie();
    m_spEnemy->stopAllActions();
    m_spEnemy->runAction(CCFadeOut::actionWithDuration(0.3));
    moneyDrop(0.2);
    //5
//    m_blood->fadeout(0.5);
    fadeoutHP(0.5f);
    if(m_spDropItem != NULL)
    {
        m_spDropItem->setIsVisible(false);
        m_spDropItem->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.2),
                                                    CCShow::action(),
                                                    CCMoveBy::actionWithDuration(0.1, CCPointMake(0, -50)),
                                                    CCMoveBy::actionWithDuration(0.1, CCPointMake(0, 20)),
                                                    CCMoveBy::actionWithDuration(0.1, CCPointMake(0, -20)),
                                                    CCMoveBy::actionWithDuration(0.1, CCPointMake(0, 5)),
                                                    CCMoveBy::actionWithDuration(0.1, CCPointMake(0, -5)),
                                                    NULL));
    }
    
    clearBuffWithEnemyDie(0.2f);
}

void Enemy::chkActBeAttack()
{
    if (m_skillArr == NULL)
    {
        return;
    }
    
    if (m_iHP<m_iBaseHP*0.1f && m_iHP>0)
    {
        for (int i=0; i<m_skillArr->count(); i++)
        {
            if (m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillZiBao)
            {
                m_skillArr->getObjectAtIndex(i)->action();
                m_iHP = 0;
                break;
            }
        }
    }
}

void Enemy::chkRoundSkill()
{
    if (m_skillArr == NULL)
    {
        return;
    }
    
//    if (m_iHP<m_iBaseHP*0.1f && m_iHP>0)
//    {
//        for (int i=0; i<m_skillArr->count(); i++)
//        {
//            if (m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillZiBao2)
//            {
//                m_skillArr->getObjectAtIndex(i)->action();
//                m_iHP = 0;
//                //6
//                m_blood->setBlood(m_iHP, m_iBaseHP, 0.2f);
//                break;
//            }
//        }
//    }
    
    if (0<m_iHP && m_iHP<m_iBaseHP*0.1f)
    {
        for (int i=0; i<m_skillArr->count(); i++)
        {
            if (m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillZiBao2)
            {
                m_skillArr->getObjectAtIndex(i)->action();
                changeCurHPAction(-m_iHP,0.2f);
                break;
            }
        }
    }
    
}

void Enemy::chkActionFromDie()
{
    if (m_skillArr == NULL)
    {
        return;
    }
    
    for (int i=0; i<m_skillArr->count(); i++)
    {
        if (m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillTongGuiYuJin
            || m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkill_509
            || m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkill_519
            || m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkill_520
            || m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkill_521
            || m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkill_524)
        {
            m_skillArr->getObjectAtIndex(i)->action();
            break;
        }
    }
}

void Enemy::reLive()
{
    setEnemyState(EN_ENEMY_STATE_NORMAL);
//    m_iHP = m_iBaseHP;
//    //7
//    m_blood->setBlood(m_iHP, m_iBaseHP, 0.2);
    changeCurHPAction(m_iBaseHP,0.2f);
    

        
    m_spEnemy->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.2),
                                               //  CCShake::actionWithDuration(0.5, m_spEnemy->getPosition()),
                                                 CCFadeIn::actionWithDuration(1.0),
                                                 NULL));

    m_lbRound->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.2),
                                             CCFadeIn::actionWithDuration(1.0),
                                             NULL));
    m_spRound->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.2),
                                             CCFadeIn::actionWithDuration(1.0),
                                             NULL));
//    //8
//    m_blood->fadein(0.2+0.5);
    fadeinHP(0.7f);
    
    CGameData::Inst()->addBigImageByRGBA4444("fuhuoanim.plist");

    CCAnimation* animation = CCAnimation::animation();
    
    for (int i=1; i<=19; i++)
    {
        char buf[50];
        sprintf(buf, "fuhuo00%2d.png",i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( buf );
        animation->addFrame(frame);
    }
    
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    animation->addFrame(frame);
    
    CCActionInterval* action = CCAnimate::actionWithDuration(19 / 20.0f, animation, false);
    
    
    CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("blank.png");
    addChild(spAnim);
    spAnim->setAnchorPoint(CCPointMake(0.5, 0));
    CCPoint pt = m_spEnemy->getPosition();
    pt.y -= (m_spEnemy->getContentSize().height * 0.5);
    spAnim->setPosition(pt);
    spAnim->setScale(2.0);
    spAnim->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.2),
                                          action,
                                          NULL));
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("fuhuoanim.plist");
    clearBuffWithEnemyRelive();
}

void Enemy::clearBuffWithEnemyRelive()
{
    for(int i = 0; i < MAX_BUFF_ITEMS; i++)
    {
        if(m_buffArr[i].spBuff)
        {
            m_buffArr[i].cd = 0;
            m_buffArr[i].buffID = EN_ENEMY_NONE;
            removeChild(m_buffArr[i].spBuff, true);
            m_buffArr[i].spBuff = NULL;
            m_buffArr[i].showCd = false;
        }
    }
    
    
    for (int i=0; i<MAX_BUFF_ITEMS; i++)
    {
        SaveData::Inst()->setEnemyBuffbuffId(i, m_buffArr[i].buffID, SaveData::Inst()->getCurStep(), m_index);
        SaveData::Inst()->setEnemyBuffbuffCd(i, m_buffArr[i].cd, SaveData::Inst()->getCurStep(), m_index);
        short s_show = m_buffArr[i].showCd ? 1:0;
        SaveData::Inst()->setEnemyBuffbuffShow(i, s_show, SaveData::Inst()->getCurStep(), m_index);
    }
}

void Enemy::saveEnemyBuff()
{
    for (int i=0; i<MAX_BUFF_ITEMS; i++)
    {
        SaveData::Inst()->setEnemyBuffbuffId(i, m_buffArr[i].buffID, SaveData::Inst()->getCurStep(), m_index);
        SaveData::Inst()->setEnemyBuffbuffCd(i, m_buffArr[i].cd, SaveData::Inst()->getCurStep(), m_index);
        short s_show = m_buffArr[i].showCd ? 1:0;
        SaveData::Inst()->setEnemyBuffbuffShow(i, s_show, SaveData::Inst()->getCurStep(), m_index);
    }
}

//******************************************************************************
// isActionRunning
//******************************************************************************
bool Enemy::isActionRunning()
{
    bool bRet = false;
    
    if(m_spEnemy->numberOfRunningActions() > 0)
        bRet = true;
    
    if((m_spDropItem != NULL) && ((m_spDropItem->numberOfRunningActions() > 0)))
        bRet = true;
    
    return bRet;
}

//******************************************************************************
// getEnemyPos
//******************************************************************************
const CCPoint& Enemy::getEnemyPos()
{
    return m_spEnemy->getPosition();
}

//******************************************************************************
// getEnemySize
//******************************************************************************
const CCSize& Enemy::getEnemySize()
{
    return m_spEnemy->getContentSize();
}

void Enemy::skillStart()
{
   if (m_skillArr != NULL && m_skillArr->getObjectAtIndex(m_actSkillID) != NULL)
   {
       m_skillArr->getObjectAtIndex(m_actSkillID)->action();
   }
}

int Enemy::getSkillUseLastID()
{
    return m_iSkillUseLastID;
}

//判断是否有技能释放，并给出技能下标
int Enemy::getCanExecuteSkillIdx()
{
    if (m_skillArr == NULL)
        return -1;
    
    for (int i=0; i<m_skillArr->count(); i++)
    {
        if (m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillZhenFuHuo)
        {
            EnemyGrp* t_enemyGrp = (EnemyGrp*)(this->getParent());
            for (int j=0; j < t_enemyGrp->getEnemyCount(); j++)
            {
                if (!t_enemyGrp->getEnemyByIdx(j)->isAlive())
                {
                     return i;
                }
            }
        }
    }
    
//    for (int i=0; i<m_skillArr->count(); i++)
//    {
//        if (m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillZhiYu
//            ||m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillFuHuo
//            ||m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillFuHuo2
//            )
//        {
//            if (m_skillArr->getObjectAtIndex(i)->canExecute())
//            {
//                return i;
//            }
//        }
//    }
    
//    for (int i=0; i<m_skillArr->count(); i++)
//    {
//        m_skillArr->getObjectAtIndex(i)->recordRunCount();
//        if (m_skillArr->getObjectAtIndex(i)->canExecute())
//        {
//            return i;
//        }
//    }
    unsigned int unCount = m_skillArr->count();
    for (unsigned int i = 0; i < unCount; ++i)
    {
        if (EnemySkill *pSkill = m_skillArr->getObjectAtIndex(i))
        {
            //pSkill->recordRunCount();
            if (m_skillArr->getObjectAtIndex(i)->canExecute())
            {
                return i;
            }
        }
    }
    return -1;
}

void Enemy::cbStartAttack(ccTime time)
{
    m_iActTimes--;
    char buf[20];
    sprintf(buf, "%d", m_iCurRound.get());
    m_lbRound->setString(buf);
    m_lbRound->setScale(1.5);
    m_lbRound->runAction(CCScaleTo::actionWithDuration(0.5, 1.0));
    
    if (m_iDoSpecilAtk > EN_ENEMY_NONE)
    {
        switch (m_iDoSpecilAtk)
        {
            case EN_ENEMY_SHASHOUJIAN:
            {
                for (int i=0; i<m_skillArr->count(); i++)
                {
                    if (m_skillArr->getObjectAtIndex(i)->getSkillID() == enEnemySkillShaShouJian)
                    {
                        EsShaShowJian* pShaShouJian = (EsShaShowJian*)(m_skillArr->getObjectAtIndex(i));
                        pShaShouJian->attack();
                    }
                }
            }
                break;
                
            case EN_ENEMY_XUSHIDAIFA:
            case EN_ENEMY_XINGYUNYIJI:
            case EN_ENEMY_XUSHIDAIFA238:
            case EN_ENEMY_XUSHIDAIFA250:
                case EN_ENEMY_XUSHIDAIFA294://lvgenquan
                case EN_ENEMY_XUSHIDAIFA316:
                case EN_ENEMY_XUSHIDAIFA508:
            {
                m_atkInfo.bAtkOvr = true;
                (m_pListener->*m_pfnSelector)(&m_atkInfo);
            }
                break;
                
            case EN_ENEMY_HAOYIN:
            {
                m_actSkillID = -1;
                
                int skillID = 0;
                
                if (SaveData::Inst()->getrand()<70)
                {
                    skillID = enEnemySkillMengShowLuanWu;
                }
                else
                {
                    skillID = enEnemySkillTu;
                }
                
                for (int i=0; i<m_skillArr->count(); i++)
                {
                    if (m_skillArr->getObjectAtIndex(i)->getSkillID() == skillID)
                    {
                        m_actSkillID = i;
                        break;
                    }
                }
                
                if (m_actSkillID>0)
                {
                    m_atkInfo.bShowSkillEffect = true;
                    m_atkInfo.iManSkillID = m_skillArr->getObjectAtIndex(m_actSkillID)->getSkillID();
                    
                    m_atkInfo.changeStone = m_skillArr->getObjectAtIndex(m_actSkillID)->getChangeStone();
                    
                    (m_pListener->*m_pfnSelector)(&m_atkInfo);
                }
                else
                {
                    m_atkInfo.bAtkOvr = true;
                    (m_pListener->*m_pfnSelector)(&m_atkInfo);
                }
            }
                
            default:
                break;
        }
        
        m_iDoSpecilAtk = EN_ENEMY_NONE;
    }
    else if (m_atkInfo.iAttackPt > 0)
    {
        m_actSkillID = getCanExecuteSkillIdx();
        
        if (m_skillArr != NULL && m_actSkillID != -1)
        {
            m_atkInfo.bShowSkillEffect = true;
            m_atkInfo.iManSkillID = m_skillArr->getObjectAtIndex(m_actSkillID)->getSkillID();
            m_iSkillUseLastID = m_atkInfo.iManSkillID;
            
            m_atkInfo.changeStone = m_skillArr->getObjectAtIndex(m_actSkillID)->getChangeStone();
            
            (m_pListener->*m_pfnSelector)(&m_atkInfo);
        }
        else
        {
            m_atkInfo.bAtkOvr = true;
            (m_pListener->*m_pfnSelector)(&m_atkInfo);
        }
    }
    else
    {
        m_atkInfo.bAtkOvr = true;
        (m_pListener->*m_pfnSelector)(&m_atkInfo);
    }
    
    m_iCurAtkType = EN_ENEMY_NONE;
}

void Enemy::enemyBuff_78()
{
    m_iEnemyBuf_78--;
    if (m_iEnemyBuf_78 >= 0) {
        m_atkInfo.clear();
        m_atkInfo.bAtkOvr = false;
        m_atkInfo.iAttackPt = 6666;
        (m_pListener->*m_pfnSelector)(&m_atkInfo);
    } else {
        unschedule(schedule_selector(Enemy::enemyBuff_78));
        m_atkInfo.clear();
        m_atkInfo.iAttackPt = attack();
        cbStartAttack(0);
    }
}

void Enemy::enemyBuff_80()
{
    m_iEnemyBuf_80--;
    if (m_iEnemyBuf_80 >= 0) {
        m_atkInfo.clear();
        m_atkInfo.bAtkOvr = false;
        m_atkInfo.iAttackPt = 7777;
        (m_pListener->*m_pfnSelector)(&m_atkInfo);
    } else {
        unschedule(schedule_selector(Enemy::enemyBuff_80));
        m_atkInfo.clear();
        m_atkInfo.iAttackPt = attack();
        cbStartAttack(0);
    }
}

void Enemy::enemyBuff_83()
{
    m_atkInfo.clear();
    m_atkInfo.bAtkOvr = true;
    m_atkInfo.iAttackPt = 99999;
    (m_pListener->*m_pfnSelector)(&m_atkInfo);
}

//fullAttack
void Enemy::startAttack()
{
    m_iActTimes = 1;
    
    // enEnemyBuff_78/enEnemyBuff_80
    if (m_iCurRound.get() == 1 && CheckGrpBuff(enEnemyBuff_78) && isAlive()) {
        changeCurHPAction(MIN(m_iBaseHP*0.25f,m_iBaseHP.get() - m_iHP),0.5f);
        m_iEnemyBuf_78 = SaveData::Inst()->getrand()%3 + 1;
        schedule(schedule_selector(Enemy::enemyBuff_78), 0.2);
    } else if (m_iCurRound.get() == 1 && CheckGrpBuff(enEnemyBuff_80) && isAlive()) {
        changeCurHPAction(MIN(m_iBaseHP*0.25f,m_iBaseHP.get() - m_iHP),0.5f);
        m_iEnemyBuf_80 = SaveData::Inst()->getrand()%3 + 1;
        schedule(schedule_selector(Enemy::enemyBuff_80), 0.2);
    } else {
        m_atkInfo.clear();
        m_atkInfo.iAttackPt = attack();
        cbStartAttack(0);
    }
}

void Enemy::AttackAgain()
{
    m_atkInfo.clear();
    m_atkInfo.iAttackPt = m_iAttackPt*m_iAtkRatio;
    cbStartAttack(0);
}

void Enemy::sentAtkInfo()
{
    switch (m_atkInfo.iManSkillID)
    {
        case enEnemySkillZhiYu:
        case enEnemySkill_279:
        {
            changeCurHPAction(MIN(m_iHP+m_iBaseHP*0.5f,m_iBaseHP.get()),0.5f);
            break;
        }
        case enEnemySkill_282:
        {
            if (m_iHP.get()<m_iBaseHP)
            {
                changeCurHPAction(m_iBaseHP,0.5f);
            }
            break;
        }
        case enEnemySkill_475:
        {
            changeCurHPAction(MIN(m_iHP+m_iBaseHP*0.5f,m_iBaseHP.get()),0.5f);
            break;
        }
        case enEnemySkill_476:
        {
            changeCurHPAction(MIN(m_iHP+m_iBaseHP*0.4f,m_iBaseHP.get()),0.5f);
            break;
        }
        case enEnemySkillFuHuo:
        case enEnemySkillFuHuo2:
            changeCurHPAction(m_iBaseHP,0.5f);
            break;
            
        case enEnemySkillWuShengHuYou:
            setBuffByIdx(EN_ENEMY_HUJIA,10,-1);
            break;
            
        case enEnemySkillShenLinBiYou:
        case enEnemySkill_206:
        case enEnemySkill_427:
        case enEnemySkill_430:
            case enEnemySkill_510:
        {
            setBuffByIdx(EN_ENEMY_HUJIA,999,-1);
        }
            break;
            
        case enEnemySkillMuTengJia:
        {
            setBuffByIdx(EN_ENEMY_MUTENGJIA, 9999, -1);
        }
            break;
            
        case enEnemySkillMuTengJia2:
        {
            setBuffByIdx(EN_ENEMY_MUTENGJIA, 5, -1);
        }
            break;
            
        case enEnemySkillMuTengJia3:
            setBuffByIdx(EN_ENEMY_DEFEN_FIRE, 3, -1);
            break;

        case enEnemySkillQuYan:
            setBuffByIdx(EN_ENEMY_DEFEN_FIRE, 3, -1);
            break;
            
        case enEnemySkillDuanLiu:
            setBuffByIdx(EN_ENEMY_DEFEN_WATER, 3, -1);
            break;
            
        case enEnemySkillPoMu:
            setBuffByIdx(EN_ENEMY_DEFEN_WOOD, 3, -1);
            break;
            
        case enEnemySkillZhanLei:
            setBuffByIdx(EN_ENEMY_DEFEN_LIGHT, 3, -1);
            break;
            
        case enEnemySkillShiHun:
            setBuffByIdx(EN_ENEMY_DEFEN_AN, 3, -1);
            break;
            
        case enEnemySkillxinjinganghuti:
        case enEnemySkillxinjinganghuti2:
            setBuffByIdx(EN_ENEMY_JINGANGHUTI, 9999, -1);
            break;
            
        case enEnemySkillXuShiDaiFa:
        {
            setBuffByIdx(EN_ENEMY_XUSHIDAIFA,1,-1);
        }
            break;
        case enEnemySkill_238:
         
        {
            setBuffByIdx(EN_ENEMY_XUSHIDAIFA238,1,-1);
            break;
        }
        case enEnemySkill_250:
        {
            setBuffByIdx(EN_ENEMY_XUSHIDAIFA250,1,-1);
            break;
        }
        case enEnemySkillXingYunYiJi:
        {
            setBuffByIdx(EN_ENEMY_XINGYUNYIJI,1,-1);
        }
            break;
            
        case enEnemySkill_133:
        {
            setBuffByIdx(EN_ENEMY_BUFF133,1,-1);
        }
            break;
            
        case enEnemySkillHaoYin:
        {
            setBuffByIdx(EN_ENEMY_HAOYIN, 1, -1);
        }
            break;
            
        case enEnemySkillHaoYin2:
        {
            setBuffByIdx(EN_ENEMY_HAOYIN2, 9999, -1);
        }
            break;
            
        case enEnemySkillShiJianNingJie:
            setBuffByIdx(EN_ENEMY_SHIJIANNINGJIE, 9999, -1);
            break;
            
        case enEnemySkillShaShouJian:
        {
            setBuffByIdx(EN_ENEMY_SHASHOUJIAN, 3,-1);
        }
            break;
            
        case enEnemySkillJiNu:
        {
            setBuffByIdx(EN_ENEMY_JINU, 5, -1);
        }
            break;
            
        case enEnemySkillJiNu2:
        {
            setBuffByIdx(EN_ENEMY_JINU2, 3, -1);
        }
            break;
        case enEnemySkillJiNu3:
        {
            setBuffByIdx(EN_ENEMY_JINU3, 5, -1);
        }
            break;
        case enEnemySkillKuangBao:
        {
            setBuffByIdx(EN_ENEMY_KUANGBAO, 10, -1);
        }
            break;
            
        case enEnemySkill_142:
        case enEnemySkill_464:
        {
            setBuffByIdx(EN_ENEMY_BUFF142, 99, -1);
        }
            break;
            
        case enEnemySkillGongJiZiTai:
        {
            setBuffByIdx(EN_ENEMY_GONGJIZITAI, 5, -1);
        }
            break;
            
        case enEnemySkillBaoNu:
        {
            setBuffByIdx(EN_ENEMY_BAONU, 5, -1);
        }
            break;
            
        case enEnemySkillBaoZou:
        {
            setBuffByIdx(EN_ENEMY_BAOZOU, 9999, -1);
        }
            break;
        case enEnemySkill_118:
        {
            setBuffByIdx(EN_ENEMY_BUFF118, 9999, -1);
        }
            break;
            
        case enEnemySkill_122:
        {
            setBuffByIdx(EN_ENEMY_BUFF122, 9999, -1);
        }
            break;
        case enEnemySkill_146:
        {
            setBuffByIdx(EN_ENEMY_BUFF146, 99, -1);
        }
            break;
        case enEnemySkill_137:
        case enEnemySkill_468:
        {
            setBuffByIdx(EN_ENEMY_BUFF137, 1, -1);
        }
            break;
        case enEnemySkill_149:
        {
            setBuffByIdx(EN_ENEMY_BUFF149, 1, -1);
        }
            break;
        case enEnemySkill_145:
        {
            setBuffByIdx(EN_ENEMY_BUFF145, 1, -1);
        }
            break;
        case enEnemySkill_157:
        case enEnemySkill_477:
        {
            setBuffByIdx(EN_ENEMY_BUFF157, 1, -1);
        }
            break;
        case enEnemySkill_159:
        {
            setBuffByIdx(EN_ENEMY_BUFF159, 999, -1);
        }
            break;
        case enEnemySkill_202:
        {
            setBuffByIdx(EN_ENEMY_BUFF202, 9999, -1);
            break;
        }
        case enEnemySkill_232:
        {
            setBuffByIdx(EN_ENEMY_BUFF232, 9999, -1);
            break;
        }
        case enEnemySkill_247:
        {
            setBuffByIdx(EN_ENEMY_BUFF247, 9999, -1);
            break;
        }
        case enEnemySkill_249:
        {
            setBuffByIdx(EN_ENEMY_BUFF249, 5, -1);
            break;
        }
        case enEnemySkill_252:
        {
            setBuffByIdx(EN_ENEMY_BUFF252, 99, -1);
            break;
        }
        case enEnemySkill_287:{//lvgenquan
            setBuffByIdx(EN_ENEMY_BUFF287, 999, -1);
            break;
        }
        case enEnemySkill_294:{//lvgenquan
            setBuffByIdx(EN_ENEMY_XUSHIDAIFA294, 1, -1);
            break;
        }
        case enEnemySkill_340:{
           setBuffByIdx(EN_ENEMY_500THOUNSAND_LIMIT, 3, -1);

            break;
        }
        case enEnemySkill_303:{
            setBuffByIdx(EN_ENEMY_BUFF303, 3, -1);
            break;
        }
        case enEnemySkill_306:{
            setBuffByIdx(EN_ENEMY_BUFF306, 10, -1);
            break;
        }
            case enEnemySkill_314:
        {
            setBuffByIdx(EN_ENEMY_XUSHIDAIFA314, 1, -1);
            break;

        }
        case enEnemySkill_316:{//lvgenquan
            setBuffByIdx(EN_ENEMY_XUSHIDAIFA316, 1, -1);
            break;
        }
        case enEnemySkill_352:
        {
            setBuffByIdx(EN_ENEMY_BUFF303, 3, -1);
            break;
            
        }
        case enEnemySkill_355:
        {
            setBuffByIdx(EN_ENEMY_BUFF232, 9999, -1);
            break;
        }
        case enEnemySkill_365:
        {
            setBuffByIdx(EN_ENEMY_BUFF142, 3, -1);
            break;
        }
        case enEnemySkill_368:
        {
            setBuffByIdx(EN_ENEMY_BUFF303, 3, -1);
            break;
            
        }
        case enEnemySkill_428:
        {
            setBuffByIdx(EN_ENEMY_BUFF247, 9999, -1);
            break;
        }
        case enEnemySkill_447:
        {
            setBuffByIdx(EN_ENEMY_BUFF447, 9999, -1);
            break;
        }
        case enEnemySkill_456:
        case enEnemySkill_460:
        {
            setBuffByIdx(EN_ENEMY_BUFF306, 999, -1);
            break;
        }
        case enEnemySkill_479:
        {
            setBuffByIdx(EN_ENEMY_BUFF142, 5, -1);
        }
            break;
            case enEnemySkill_508:
        {
            setBuffByIdx(EN_ENEMY_XUSHIDAIFA508, 1, -1);
        }
            break;
        default:
            break;
    }
   
    (m_pListener->*m_pfnSelector)(&m_atkInfo);
}

//******************************************************************************
// attack
//******************************************************************************
int  Enemy::attack()
{
    //m_iAtkRatio = 1.0f;
    int iRet;

    if(m_iHP <= 0){
        iRet = -1;
        return iRet;
    }
    
    if(isBuffByIdx(EN_ENEMY_MABI))
    {
        decBuffCD(true);
        
        iRet = 0;
        return iRet;
    }
    else
    {
        decBuffCD(true);
    }
    
    if(m_iCurRound > 1)
    {
        m_iCurRound--;
        iRet = 0;
    }
    else
    {
        decBuffCD(false);
        m_iCurRound = m_iRoundMax;
        iRet = m_iAttackPt*m_iAtkRatio;
        m_iAtkRatio = 1.0f;
        
        int chkcomb = CheckComboAtk();
        if (chkcomb >= 0)
        {
            if (chkcomb == 0)
            {
                iRet = 0;
                m_iDoSpecilAtk = EN_ENEMY_NONE;
            }
            else
            {
                iRet *= chkcomb;
            }
        }
        
        EnemyGrp* pEGrp = (EnemyGrp*)m_pListener;
        pEGrp->setComboDunNum((SaveData::Inst()->getrand()%4) + 3);
    }
    
    if (!isAlive())
    {
        iRet = 0;
    }
    
    if (m_iDoSpecilAtk == EN_ENEMY_FREEZE)
    {
        m_iDoSpecilAtk = EN_ENEMY_NONE;
        iRet = 0;
    }
    
    return iRet;
}

int Enemy::CheckComboAtk()
{
    FightMngr* fm = (FightMngr*)getParent()->getParent();
    if (CheckGrpBuff(enEnemyBuff_combodun2) || CheckGrpBuff(enEnemyBuff_combodun22))
    {
        return abs((fm->getPlayer()->getCurCombo() - fm->getEnemyGrp()->getComboDunNum()));
    }
    
    return -1;
}

bool Enemy::bUseBuffLianfan()
{
    FightMngr* fm = (FightMngr*)getParent()->getParent();
    

    
    if (CheckGrpBuff(enEnemyBuff_1lianbufan) && (fm->getPlayer()->getCurCombo() == 1))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_2lianbufan) && (fm->getPlayer()->getCurCombo() == 2))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_3lianbufan) && (fm->getPlayer()->getCurCombo() == 3))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_4lianbufan) && (fm->getPlayer()->getCurCombo() == 4))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_5lianbufan) && (fm->getPlayer()->getCurCombo() == 5))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_6lianbufan) && (fm->getPlayer()->getCurCombo() == 6))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_7lianbufan) && (fm->getPlayer()->getCurCombo() == 7))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_8lianbufan) && (fm->getPlayer()->getCurCombo() == 8))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_9lianbufan) && (fm->getPlayer()->getCurCombo() == 9))
        return true;
    
    if (CheckGrpBuff(enEnemyBuff_10lianbufan) && (fm->getPlayer()->getCurCombo() == 10))
        return true;
  
    return false;
}

//******************************************************************************
// runAnim
//******************************************************************************
void Enemy::runAnim(int iElem, float dt)
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
    spAnim->setPosition(pt);
    spAnim->setScale(2.0);
    spAnim->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
                                            action,
                                            NULL));
}

//******************************************************************************
// setWudi
//******************************************************************************
void Enemy::setWudi(bool bWudi)
{
    for (int i=0; i<ENEMYBUFFMAX; i++)
    {
        if (m_iBuffIdArr[i] == enEnemyBuff_wudi)
            return;
    }
    
    m_bWudi = bWudi;
}


//******************************************************************************
// sickHpDown
//******************************************************************************
void Enemy::sickHpDown(int sickPt)
{
    if(isAlive() == false)
        return;
    
    if (m_bWudi)
        sickPt = 0;
    
//    m_iHP = m_iHP - sickPt;
//    
//    //11
//    m_blood->setBlood(m_iHP, m_iBaseHP, 0.5);
    
    changeCurHPAction(-sickPt,0.5f);
    
    
    char buf[10];
    sprintf(buf, "%d", sickPt);
    if(m_lbSick){
        m_lbSick->setString(buf);
        m_lbSick->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                  CCFadeIn::actionWithDuration(0.1),
                                                  CCFadeOut::actionWithDuration(0.9),
                                                  NULL));
    }
    
    
    chkActBeAttack();
    
    
    if(m_iHP <= 0)
    {
        checkDie(0.5);
    }
    else
    {
        m_spEnemy->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.5),
                                                 CCShake::actionWithDuration(1.0, m_spEnemy->getPosition()),
                                                 NULL));
    }
}

bool Enemy::isDecEachRound(int idx)
{
    bool ret = false;
    
    switch (idx)
    {
        case EN_ENEMY_CHGELM:
        case EN_ENEMY_POJIA:
        case EN_ENEMY_MABI:
        case EN_ENEMY_DELAY:
        case EN_ENEMY_SICK:
        case EN_ENEMY_POISON:
        case EN_ENEMY_HUJIA:
        case EN_ENEMY_MUTENGJIA:
        case EN_ENEMY_JINGANGHUTI:
        case EN_ENEMY_DEFEN_FIRE:
        case EN_ENEMY_DEFEN_WATER:
        case EN_ENEMY_DEFEN_WOOD:
        case EN_ENEMY_DEFEN_LIGHT:
        case EN_ENEMY_DEFEN_AN:
        case EN_ENEMY_FREEZE:
           case EN_ENEMY_500THOUNSAND_LIMIT://lvgenquan
            ret = true;
            break;
            
        case EN_ENEMY_XUSHIDAIFA:
        case EN_ENEMY_JINU:
        case EN_ENEMY_KUANGBAO:
        case EN_ENEMY_GONGJIZITAI:
        case EN_ENEMY_BAONU:
        case EN_ENEMY_SHASHOUJIAN:
        case EN_ENEMY_HAOYIN:
        case EN_ENEMY_HAOYIN2:
        case EN_ENEMY_JINU2:
        case EN_ENEMY_BAOZOU:
        case EN_ENEMY_XINGYUNYIJI:
        case EN_ENEMY_SHIJIANNINGJIE:
        case EN_ENEMY_JINU3:
        case EN_ENEMY_BUFF118:
        case EN_ENEMY_BUFF122:
        case EN_ENEMY_BUFF137:
        case EN_ENEMY_BUFF133:
        case EN_ENEMY_BUFF142:
        case EN_ENEMY_BUFF146:
        case EN_ENEMY_BUFF149:
        case EN_ENEMY_BUFF145:
        case EN_ENEMY_BUFF157:
        case EN_ENEMY_BUFF159:
        case EN_ENEMY_BUFF202:
        case EN_ENEMY_BUFF232:
        case EN_ENEMY_XUSHIDAIFA238:
        case EN_ENEMY_BUFF247:
        case EN_ENEMY_XUSHIDAIFA250:
        case EN_ENEMY_BUFF249:
        case EN_ENEMY_BUFF252:
            case EN_ENEMY_BUFF287://lvgenquan
            case EN_ENEMY_XUSHIDAIFA294://lvgenquan
            case EN_ENEMY_BUFF303:
            case EN_ENEMY_BUFF306:
            case EN_ENEMY_XUSHIDAIFA314:
            case EN_ENEMY_XUSHIDAIFA316:
            case EN_ENEMY_BUFF447:
            case EN_ENEMY_XUSHIDAIFA508:
            ret = false;
            break;
            
        default:
            break;
    }
    
    return ret;
}

bool Enemy::isBuffByIdx(int idx)
{
    for (int i=0; i<MAX_BUFF_ITEMS; i++)
    {
        if (m_buffArr[i].buffID == idx)
        {
            return true;
        }
    }
    
    return false;
}



void Enemy::clearBuffByIdx(int idx)
{
    for (int i=0; i<MAX_BUFF_ITEMS; i++)
    {
        if (m_buffArr[i].buffID == idx)
        {
            m_buffArr[i].cd = 0;
            removeChild(m_buffArr[i].spBuff, true);
            m_buffArr[i].spBuff = NULL;
            m_buffArr[i].buffID = EN_ENEMY_NONE;
            m_buffArr[i].showCd = false;
            
            updateBuffPos();
            
            break;
        }
    }
}

bool Enemy::isAliveOnlyMe()
{
    if (isAlive())
    {
        if (EnemyGrp *pEnemyGrp = dynamic_cast<EnemyGrp*>(m_pListener))
        {
            return 1==pEnemyGrp->getAliveCount();
        }
    }
    return false;
}

int Enemy::getCurFirstHP()
{
    if (!m_pSecondBlood)
    {
        return m_iHP;
    }
    return MAX(0,m_iHP-getBaseSecondHP());
}

int Enemy::getCurSecondHP()
{
    if (!m_pSecondBlood)
    {
        return 0;
    }
    return 0<getCurFirstHP()? getBaseSecondHP() : m_iHP.get();
}

int Enemy::getBaseFirstHP()
{
    if (!m_pSecondBlood)
    {
        return m_iBaseHP;
    }
    return m_iBaseHP/2;
}

int Enemy::getBaseSecondHP()
{
    if (!m_pSecondBlood)
    {
        return 0;
    }
    return m_iBaseHP/2;
}

//int Enemy::changeCurHPAction(int nHP,float fTime,enElementDef emElement /*= enElementInvalid*/)
//{
//    //计算
//    //2增
//    if (0<nHP && m_pSecondBlood)
//    {
//        if (getBaseSecondHP()>getCurSecondHP())
//        {
//            if (getBaseSecondHP()<(getCurSecondHP()+nHP))
//            {
//                nHP = getCurSecondHP()+nHP-getBaseSecondHP();
//                m_iHP = getBaseSecondHP();
//            }
//            else
//            {
//                m_iHP += nHP;
//                nHP = 0;
//            }
//        }
//        m_pSecondBlood->setBlood(getCurSecondHP(), getBaseSecondHP(), fTime);
//    }
//    //1增
//    if (0<nHP && m_blood)
//    {
//        if (getBaseFirstHP()>getCurFirstHP())
//        {
//            if (getBaseFirstHP()<(getCurFirstHP()+nHP))
//            {
//                nHP = getCurFirstHP()+nHP-getBaseFirstHP();
//                m_iHP = getBaseHp();
//            }
//            else
//            {
//                m_iHP += nHP;
//                nHP = 0;
//            }
//        }
//        m_blood->setBlood(getCurFirstHP(), getBaseFirstHP(), fTime);
//    }
//    //1减
//    if (0>nHP && m_blood)
//    {
//        if (0<getCurFirstHP())
//        {
//            if (0>=(getCurFirstHP()+nHP))
//            {
//                nHP += getCurFirstHP();
//                m_iHP -= getCurFirstHP();
//            }
//            else
//            {
//                m_iHP += nHP;
//                nHP = 0;
//            }
//        }
//        m_blood->setBlood(getCurFirstHP(), getBaseFirstHP(), fTime);
//    }
//    //2减
//    if (0>nHP && m_pSecondBlood)
//    {
//        if (0<getCurSecondHP())
//        {
//            if (0>=(getCurSecondHP()+nHP))
//            {
//                nHP += getCurSecondHP();
//                m_iHP -= getCurSecondHP();
//            }
//            else
//            {
//                m_iHP += nHP;
//                nHP = 0;
//            }
//        }
//        m_pSecondBlood->setBlood(getCurSecondHP(), getBaseSecondHP(), fTime);
//    }
//    return nHP;
//}

int Enemy::changeCurHPAction(int nHP,float fTime,enElementDef emElement /*= enElementInvalid*/)
{
    //计算
    //2增
    if (0<nHP && m_pSecondBlood && m_pSecondBlood->getIsVisible())
    {
        //当前可增加血
        int nSumAddHP = nHP>(m_iBaseHP-m_iHP.get())? (m_iBaseHP-m_iHP.get()): nHP;
        float fDelayMoveTime = fTime;
        
        if (getBaseSecondHP()>getCurSecondHP())
        {
            if (getBaseSecondHP()<(getCurSecondHP()+nHP))
            {
                //time
                fDelayMoveTime *= (float)(getBaseSecondHP()-getCurSecondHP())/(float)nSumAddHP;
                if(0==fDelayMoveTime) fDelayMoveTime = fTime;
                
                //hp
                nHP = getCurSecondHP()+nHP-getBaseSecondHP();
                m_iHP = getBaseSecondHP();
            }
            else
            {
                m_iHP += nHP;
                nHP = 0;
            }
        }
        m_pSecondBlood->setBlood(getCurSecondHP(), getBaseSecondHP(), fDelayMoveTime, nHP, (fTime-fDelayMoveTime)*0.3f);
        
        //存在双血
        if (0<nHP && m_blood)
        {
            if (getBaseFirstHP()>getCurFirstHP())
            {
                if (getBaseFirstHP()<(getCurFirstHP()+nHP))
                {
                    nHP = getCurFirstHP()+nHP-getBaseFirstHP();
                    m_iHP = getBaseHp();
                }
                else
                {
                    m_iHP += nHP;
                    nHP = 0;
                }
            }
            //延时移动血条
            //...
        }
        
    }
    //1增
    else if (0<nHP && m_blood && m_blood->getIsVisible())
    {
        if (getBaseFirstHP()>getCurFirstHP())
        {
            if (getBaseFirstHP()<(getCurFirstHP()+nHP))
            {
                nHP = getCurFirstHP()+nHP-getBaseFirstHP();
                m_iHP = getBaseHp();
            }
            else
            {
                m_iHP += nHP;
                nHP = 0;
            }
        }
        m_blood->setBlood(getCurFirstHP(), getBaseFirstHP(), fTime);
    }
    //1减
    else if (0>nHP && m_blood && m_blood->getIsVisible())
    {
        //当前可减少血
        int nSumDelHP = abs(abs(nHP)>m_iHP.get()? m_iHP.get() : nHP);
        
        float fDelayMoveTime = fTime;
        if (0<getCurFirstHP())
        {
            if (0>=(getCurFirstHP()+nHP))
            {
                //time
                fDelayMoveTime *= (float)getCurFirstHP()/(float)nSumDelHP;
                if(0==fDelayMoveTime) fDelayMoveTime = fTime;
                
                //hp
                nHP += getCurFirstHP();
                m_iHP -= getCurFirstHP();
            }
            else
            {
                m_iHP += nHP;
                nHP = 0;
            }
        }
        m_blood->setBlood(getCurFirstHP(), getBaseFirstHP(), fDelayMoveTime, nHP,fTime-fDelayMoveTime);
        
        //存在双血
        if (0>nHP && m_pSecondBlood)
        {
            if (0<getCurSecondHP())
            {
                if (0>=(getCurSecondHP()+nHP))
                {
                    nHP += getCurSecondHP();
                    m_iHP -= getCurSecondHP();
                }
                else
                {
                    m_iHP += nHP;
                    nHP = 0;
                }
            }
            //延时移动血条
            //...
        }
    }
    //2减
    else if (0>nHP && m_pSecondBlood && m_pSecondBlood->getIsVisible())
    {
        if (0<getCurSecondHP())
        {
            if (0>=(getCurSecondHP()+nHP))
            {
                nHP += getCurSecondHP();
                m_iHP -= getCurSecondHP();
            }
            else
            {
                m_iHP += nHP;
                nHP = 0;
            }
        }
        m_pSecondBlood->setBlood(getCurSecondHP(), getBaseSecondHP(), fTime);
    }
    return nHP;
    
}

void Enemy::endBloodMove(CCObject *pObj,void *pVoid)
{
    if (pObj==m_blood)
    {
//        std::string szNameType;
//        if (1==m_iSecondElement) szNameType = "xin.plist";
//        else if (2==m_iSecondElement) szNameType = "shui.plist";
//        else if (3==m_iSecondElement) szNameType = "huo.plist";
//        else if (4==m_iSecondElement) szNameType = "mu.plist";
//        else if (5==m_iSecondElement) szNameType = "guang.plist";
//        else szNameType = "an.plist";
//        if (CCParticleSystemQuad * emitter = CCParticleSystemQuad::particleWithFile(szNameType.c_str()))
//        {
//            emitter->setScaleX(2.0f);
//            addChild(emitter);
//            CCPoint pt = m_spEnemy->getPosition();
//            pt.y -= 100;
//            emitter->setPosition(pt);
//            emitter->setIsAutoRemoveOnFinish(true);
//        }
        
        int nDelayMoveHP = m_blood->getDelayMoveHP();
        if (0>nDelayMoveHP &&m_pSecondBlood)
        {
            std::string szNameType;
            if (1==m_iSecondElement) szNameType = "xin.plist";
            else if (2==m_iSecondElement) szNameType = "shui.plist";
            else if (3==m_iSecondElement) szNameType = "huo.plist";
            else if (4==m_iSecondElement) szNameType = "mu.plist";
            else if (5==m_iSecondElement) szNameType = "guang.plist";
            else szNameType = "an.plist";
            if (CCParticleSystemQuad * emitter = CCParticleSystemQuad::particleWithFile(szNameType.c_str()))
            {
                emitter->setScaleX(2.0f);
                addChild(emitter);
                CCPoint pt = m_spEnemy->getPosition();
                pt.y -= 100;
                emitter->setPosition(pt);
                emitter->setIsAutoRemoveOnFinish(true);
            }
            
            m_blood->setIsVisible(false);
            m_pSecondBlood->setIsVisible(true);
            m_pSecondBlood->setBlood(getCurSecondHP(), getBaseSecondHP(), m_blood->getDelayMoveTime());
        }
    }
    else if (pObj==m_pSecondBlood)
    {
        int nDelayMoveHp = m_pSecondBlood->getDelayMoveHP();
        if (0<nDelayMoveHp && m_blood)
        {
            std::string szNameType;
            if (1==m_iElement) szNameType = "xin.plist";
            else if (2==m_iElement) szNameType = "shui.plist";
            else if (3==m_iElement) szNameType = "huo.plist";
            else if (4==m_iElement) szNameType = "mu.plist";
            else if (5==m_iElement) szNameType = "guang.plist";
            else szNameType = "an.plist";
            if (CCParticleSystemQuad * emitter = CCParticleSystemQuad::particleWithFile(szNameType.c_str()))
            {
                emitter->setScaleX(2.0f);
                addChild(emitter);
                CCPoint pt = m_spEnemy->getPosition();
                pt.y -= 100;
                emitter->setPosition(pt);
                emitter->setIsAutoRemoveOnFinish(true);
            }
            
            m_pSecondBlood->setIsVisible(false);
            m_blood->setIsVisible(true);
            m_blood->setBlood(getCurFirstHP(), getBaseFirstHP(),m_pSecondBlood->getDelayMoveTime());
        }
        
    }
}

void Enemy::fadeoutHP(float fTime)
{
    if (m_blood)
    {
        m_blood->fadeout(fTime);
    }
    
    if (m_pSecondBlood)
    {
        m_pSecondBlood->fadeout(fTime);
    }
}

void Enemy::fadeinHP(float fTime)
{
    if (m_blood)
    {
        m_blood->fadein(fTime);
    }
    
    if (m_pSecondBlood)
    {
        m_pSecondBlood->fadein(fTime);
    }
}

void Enemy::showDefenFromBuff(int idx)
{
    switch (idx)
    {
        case EN_ENEMY_HUJIA:
        {
            CCSprite* Spr = CCSprite::spriteWithSpriteFrameName("wsyouhu.png");
            addChild(Spr,10,201356);
            Spr->setPosition(m_spEnemy->getPosition());
            Spr->setScale((m_spEnemy->getContentSize().width*0.8)/Spr->getContentSize().width);
            Spr->setOpacity(160);
            
            
            Spr->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.2),
                                               CCCallFunc::actionWithTarget(this, callfunc_selector(Enemy::removeObj)),
                                               
                                               NULL));
        }
            break;
            
        case EN_ENEMY_MUTENGJIA:
        {
            CCSprite* Spr = CCSprite::spriteWithSpriteFrameName("mutengjia.png");
            addChild(Spr,10,201356);
            Spr->setPosition(m_spEnemy->getPosition());
            Spr->setScale((m_spEnemy->getContentSize().width*0.8)/Spr->getContentSize().width);
            Spr->setOpacity(160);
            
            
            Spr->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.2),
                                               CCCallFunc::actionWithTarget(this, callfunc_selector(Enemy::removeObj)),
                                               
                                               NULL));
        }
            
        case EN_ENEMY_JINGANGHUTI:
        {
            CCSprite* Spr = CCSprite::spriteWithSpriteFrameName("icon_wudi.png");
            addChild(Spr,10,201356);
            Spr->setPosition(m_spEnemy->getPosition());
            Spr->setScale((m_spEnemy->getContentSize().width*0.8)/Spr->getContentSize().width);
            Spr->setOpacity(160);
            
            
            Spr->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.2),
                                               CCCallFunc::actionWithTarget(this, callfunc_selector(Enemy::removeObj)),
                                               
                                               NULL));
        }
            break;
            
        default:
            break;
    }
}

//******************************************************************************
// setPoison
//******************************************************************************
void Enemy::setPoison()
{
    if(isAlive() == false)
        return;
    
    if (isBuffByIdx(EN_ENEMY_HUJIA))
    {
        showDefenFromBuff(EN_ENEMY_HUJIA);
        return;
    }
    
    if (isBuffByIdx(EN_ENEMY_MUTENGJIA)
        || CheckGrpBuff(enEnemyBuff_mutengjia))
    {
        showDefenFromBuff(EN_ENEMY_MUTENGJIA);
        return;
    }
    
    if (isBuffByIdx(EN_ENEMY_JINGANGHUTI))
    {
        showDefenFromBuff(EN_ENEMY_JINGANGHUTI);
        return;
    }
    
    if (CheckGrpBuff(enEnemyBuff_shuxinghudun)
        ||CheckGrpBuff(enEnemyBuff_ysshenyuan)
        ||CheckGrpBuff(enEnemyBuff_ystaipingdao)
        ||CheckGrpBuff(enEnemyBuff_hunyuandun)
        ||CheckGrpBuff(enEnemyBuff_shuxinghudun2)
        ||CheckGrpBuff(enEnemyBuff_77)
        )
    {
        return;
    }
    
    for(int i = 0; i < MAX_BUFF_ITEMS; i++)
    {
        if(m_buffArr[i].spBuff == NULL)
        {
            m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("poison.png");
            m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
            addChild(m_buffArr[i].spBuff);
            
            m_buffArr[i].cd = 9999;
            m_buffArr[i].buffID = EN_ENEMY_POISON;
            
            //poison damage
            CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
            m_lbPoison = CCLabelAtlas::labelWithString("0", "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
            m_lbPoison->setPosition(m_spEnemy->getPosition());
            addChild(m_lbPoison);
            m_lbPoison->setAnchorPoint(CCPointMake(0.5, 0.5));
            m_lbPoison->setOpacity(0);
            break;
        }
    }
    
    updateBuffPos();
}

bool Enemy::CheckGrpBuff(int idx)
{
    EnemyGrp* pEnemyG = (EnemyGrp*)m_pListener;
    return pEnemyG->getIsEnemyGrpBuffByIdx(idx);
}


void Enemy::removeObj()
{
    while (getChildByTag(201356))
    {
        removeChildByTag(201356, true);
    }
}

//******************************************************************************
// isWudi
//******************************************************************************
bool Enemy::isWudi()
{
    return m_bWudi;
}

void Enemy::setBuffByIdx(int iName, int cd, int idx)
{
    switch (iName)
    {
        case EN_ENEMY_JINGANGHUTI:
        {
            for(int i = 0; i < MAX_BUFF_ITEMS; i++)
            {
                if((m_buffArr[i].buffID == EN_ENEMY_POISON
                    ||m_buffArr[i].buffID == EN_ENEMY_SICK
                    ||m_buffArr[i].buffID == EN_ENEMY_FREEZE
                    )
                   &&m_buffArr[i].spBuff != NULL)
                {
                    m_buffArr[i].cd = 0;
                    m_buffArr[i].buffID = EN_ENEMY_NONE;
                    removeChild(m_buffArr[i].spBuff, true);
                    m_buffArr[i].spBuff = NULL;
                    m_buffArr[i].showCd = false;
                }
            }
            
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("icon_wudi.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
            }
            else
            {
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if (m_buffArr[i].buffID == EN_ENEMY_JINGANGHUTI)
                    {
                        m_buffArr[i].cd = cd;
                        break;
                    }
                    else if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].buffID = EN_ENEMY_JINGANGHUTI;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("icon_wudi.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        break;
                    }
                }
            }
        }
            break;
            
        case EN_ENEMY_MUTENGJIA:
        {
            for(int i = 0; i < MAX_BUFF_ITEMS; i++)
            {
                if((m_buffArr[i].buffID == EN_ENEMY_SICK
                    ||m_buffArr[i].buffID == EN_ENEMY_POISON
                    ||m_buffArr[i].buffID == EN_ENEMY_FREEZE)
                   &&m_buffArr[i].spBuff != NULL)
                {
                    m_buffArr[i].cd = 0;
                    m_buffArr[i].buffID = EN_ENEMY_NONE;
                    removeChild(m_buffArr[i].spBuff, true);
                    m_buffArr[i].spBuff = NULL;
                    m_buffArr[i].showCd = false;
                }
            }
            
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("mutengjia.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
                
                if (m_buffArr[idx].showCd)
                {
                  if (m_buffArr[idx].cd < 100 && m_buffArr[idx].cd > 1){
                    char buf[100];
                    snprintf(buf, 99, "%d", m_buffArr[idx].cd.get());
                    CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                    CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                    lbBuffRound->setPosition(ccp(m_buffArr[idx].spBuff->getContentSize().width,m_buffArr[idx].spBuff->getContentSize().height/2));
                    m_buffArr[idx].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                    lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                    }
                }
            }
            else
            {
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if (m_buffArr[i].buffID == EN_ENEMY_MUTENGJIA)
                    {
                        m_buffArr[i].cd += cd;
                        break;
                    }
                    else if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].buffID = EN_ENEMY_MUTENGJIA;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("mutengjia.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        
                        if (m_buffArr[i].cd < 100 && m_buffArr[i].cd > 1)
                        {
                            m_buffArr[i].showCd = true;
                            char buf[100];
                            snprintf(buf, 99, "%d", m_buffArr[i].cd.get());
                            CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                            CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                            lbBuffRound->setPosition(ccp(m_buffArr[i].spBuff->getContentSize().width,m_buffArr[i].spBuff->getContentSize().height/2));
                            m_buffArr[i].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                            lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                      }
                        break;
                    }
                }
            }
        }
            break;
            
        case EN_ENEMY_DEFEN_FIRE:
        case EN_ENEMY_DEFEN_WATER:
        case EN_ENEMY_DEFEN_WOOD:
        case EN_ENEMY_DEFEN_LIGHT:
        case EN_ENEMY_DEFEN_AN:
        {
            char cBuff[50];
            switch (iName)
            {
                case EN_ENEMY_DEFEN_FIRE:
                    snprintf(cBuff, 49, "invincible2.png");
                    break;
                    
                case EN_ENEMY_DEFEN_WATER:
                    snprintf(cBuff, 49, "invincible1.png");
                    break;
                    
                case EN_ENEMY_DEFEN_WOOD:
                    snprintf(cBuff, 49, "invincible3.png");
                    break;
                    
                case EN_ENEMY_DEFEN_LIGHT:
                    snprintf(cBuff, 49, "invincible4.png");
                    break;
                    
                case EN_ENEMY_DEFEN_AN:
                    snprintf(cBuff, 49, "invincible5.png");
                    break;
                    
                default:
                    snprintf(cBuff, 49, "invincible2.png");
                    break;
            }
            
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName(cBuff);
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
                
                if (m_buffArr[idx].showCd)
                {
                    char buf[100];
                    snprintf(buf, 99, "%d", m_buffArr[idx].cd.get());
                    CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                    CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                    lbBuffRound->setPosition(ccp(m_buffArr[idx].spBuff->getContentSize().width,m_buffArr[idx].spBuff->getContentSize().height/2));
                    m_buffArr[idx].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                    lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                }
            }
            else
            {
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if (m_buffArr[i].buffID == iName)
                    {
                        m_buffArr[i].cd += cd;
                        break;
                    }
                    else if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].buffID = iName;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName(cBuff);
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
  
                        m_buffArr[i].showCd = true;
                        char buf[100];
                        snprintf(buf, 99, "%d", m_buffArr[i].cd.get());
                        CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                        CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                        lbBuffRound->setPosition(ccp(m_buffArr[i].spBuff->getContentSize().width,m_buffArr[i].spBuff->getContentSize().height/2));
                        m_buffArr[i].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                        lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                        
                        break;
                    }
                }
            }
        }
            break;

            
        case EN_ENEMY_MABI:
        {
            if (idx >= 0)
            {
//                char buf[100];
//                snprintf(buf, 99, "vertigo%d.png", m_iElement);
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("vertigo6.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
            }
            else
            {
                if(isAlive() == false)
                    return;
                
                if (isBuffByIdx(EN_ENEMY_HUJIA))
                {
                    showDefenFromBuff(EN_ENEMY_HUJIA);
                    return;
                }
                
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if (m_buffArr[i].buffID == EN_ENEMY_MABI)
                    {
                        m_buffArr[i].cd += cd;
                        break;
                    }
                    else if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].buffID = EN_ENEMY_MABI;
                        
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("vertigo6.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        break;
                    }
                }

            }
        }
            break;
                        
        case EN_ENEMY_DELAY:
        {
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("sandglass.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
            }
            else
            {
                if(isAlive() == false)
                    return;
                
                if (isBuffByIdx(EN_ENEMY_HUJIA))
                {
                    showDefenFromBuff(EN_ENEMY_HUJIA);
                    return;
                }
                
                m_iCurRound += cd;
                
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if (m_buffArr[i].buffID == EN_ENEMY_DELAY)
                    {
//                        m_buffArr[i].cd += cd;
                        break;
                    }
                    else if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].cd = m_iCurRound;
                        m_buffArr[i].buffID = EN_ENEMY_DELAY;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("sandglass.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        break;
                    }
                }
                
                char buf[20];
                sprintf(buf, "%d", m_iCurRound.get());
                m_lbRound->setString(buf);
                m_lbRound->setScale(1.5);
                m_lbRound->runAction(CCScaleTo::actionWithDuration(0.5, 1.0));
            }
        }
            break;
            
        case EN_ENEMY_HAOYIN:
        case EN_ENEMY_HAOYIN2:
        {
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("haoyin.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
            }
            else
            {
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].buffID = iName;
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("haoyin.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        break;
                    }
                }
            }
        }
            break;
            
        case EN_ENEMY_SHIJIANNINGJIE:
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("shijianningjie.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
            }
            else
            {
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].buffID = iName;
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("shijianningjie.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        break;
                    }
                }
            }
            break;
            
        case EN_ENEMY_XUSHIDAIFA:
        case EN_ENEMY_XINGYUNYIJI:
        case EN_ENEMY_XUSHIDAIFA238:
        case EN_ENEMY_XUSHIDAIFA250:
            case EN_ENEMY_XUSHIDAIFA294://lvgenquan
            case EN_ENEMY_XUSHIDAIFA316:
            case EN_ENEMY_XUSHIDAIFA508:
        {
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("jinu.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
            }
            else
            {
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].buffID = iName;
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("jinu.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        
                        break;
                    }
                }
            }
        }
            break;
            
        case EN_ENEMY_HUJIA:
        {
            for(int i = 0; i < MAX_BUFF_ITEMS; i++)
            {
                if((m_buffArr[i].buffID == EN_ENEMY_POJIA
                    ||m_buffArr[i].buffID == EN_ENEMY_CHGELM
                    ||m_buffArr[i].buffID == EN_ENEMY_MABI
                    ||m_buffArr[i].buffID == EN_ENEMY_DELAY
                    ||m_buffArr[i].buffID == EN_ENEMY_SICK
                    ||m_buffArr[i].buffID == EN_ENEMY_POISON
                    ||m_buffArr[i].buffID == EN_ENEMY_FREEZE)
                   &&m_buffArr[i].spBuff != NULL)
                {
                    if (m_buffArr[i].buffID == EN_ENEMY_POJIA)
                    {
                        m_iArmor = m_iArmorMax;
                        SaveData::Inst()->setEnemyCurArmor(m_iArmor,SaveData::Inst()->getCurStep(),m_index);
                    }
                    
                    m_buffArr[i].cd = 0;
                    m_buffArr[i].buffID = EN_ENEMY_NONE;
                    removeChild(m_buffArr[i].spBuff, true);
                    m_buffArr[i].spBuff = NULL;
                    m_buffArr[i].showCd = false;
                }
            }
            
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("wsyouhu.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
                
                if (m_buffArr[idx].showCd)
                {
                    if (m_buffArr[idx].cd < 100 && m_buffArr[idx].cd > 0) {
                        char buf[100];
                        snprintf(buf, 99, "%d", m_buffArr[idx].cd.get());
                        CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                        CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                        lbBuffRound->setPosition(ccp(m_buffArr[idx].spBuff->getContentSize().width,m_buffArr[idx].spBuff->getContentSize().height/2));
                        m_buffArr[idx].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                        lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));

                    }
                }

            }
            else
            {
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if (m_buffArr[i].buffID == EN_ENEMY_HUJIA)
                    {
                        m_buffArr[i].cd += cd;
                        break;
                    }
                    else if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].buffID = EN_ENEMY_HUJIA;
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("wsyouhu.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        
                        if (cd < 100 && cd >0)
                        {
                            m_buffArr[i].showCd = true;
                            char buf[100];
                            snprintf(buf, 99, "%d", m_buffArr[i].cd.get());
                            CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                            CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                            lbBuffRound->setPosition(ccp(m_buffArr[i].spBuff->getContentSize().width,m_buffArr[i].spBuff->getContentSize().height/2));
                            m_buffArr[i].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                            lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                       }
                        break;
                    }
                }
            }
            
        }
            break;
        case   EN_ENEMY_500THOUNSAND_LIMIT:{
            for(int i = 0; i < MAX_BUFF_ITEMS; i++)
            {
                if(m_buffArr[i].spBuff != NULL)
                {
                    m_buffArr[i].cd = 0;
                    m_buffArr[i].buffID = EN_ENEMY_NONE;
                    removeChild(m_buffArr[i].spBuff, true);
                    m_buffArr[i].spBuff = NULL;
                    m_buffArr[i].showCd = false;
                }
            }

                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    
                     if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].buffID = EN_ENEMY_500THOUNSAND_LIMIT;
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].spBuff = CCSprite::spriteWithFile("limitAtk_50wan.png");


                         m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                       if (cd < 100 && cd > 0) {
                            m_buffArr[i].showCd = true;
                            char buf[100];
                            snprintf(buf, 99, "%d", m_buffArr[i].cd.get());
                            CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                            CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                            lbBuffRound->setPosition(ccp(m_buffArr[i].spBuff->getContentSize().width,m_buffArr[i].spBuff->getContentSize().height/2));
                            m_buffArr[i].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                            lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                           

                     }
                        break;
                    }
                }
            

        }
            break;
        case EN_ENEMY_SHASHOUJIAN:
        {
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("jinu.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
                
                char buf[100];
                snprintf(buf, 99, "%d", m_buffArr[idx].cd.get());
                CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                lbBuffRound->setPosition(ccp(m_buffArr[idx].spBuff->getContentSize().width,m_buffArr[idx].spBuff->getContentSize().height/2));
                m_buffArr[idx].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
            }
            else
            {
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].buffID = EN_ENEMY_SHASHOUJIAN;
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("jinu.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        
                        m_buffArr[i].showCd = true;
                        char buf[100];
                        snprintf(buf, 99, "%d", m_buffArr[i].cd.get());
                        CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                        CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                        lbBuffRound->setPosition(ccp(m_buffArr[i].spBuff->getContentSize().width,m_buffArr[i].spBuff->getContentSize().height/2));
                        m_buffArr[i].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                        lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                        break;
                    }
                }

            }
        }
            break;
            
            
        case EN_ENEMY_JINU:
        case EN_ENEMY_JINU2:
        case EN_ENEMY_JINU3:
        case EN_ENEMY_BUFF249:
            case EN_ENEMY_BUFF303:
            case EN_ENEMY_BUFF306:
        {
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("jinu.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
                
                m_buffArr[idx].showCd = true;
                char buf[100];
                snprintf(buf, 99, "%d", m_buffArr[idx].cd.get());
                CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                lbBuffRound->setPosition(ccp(m_buffArr[idx].spBuff->getContentSize().width,m_buffArr[idx].spBuff->getContentSize().height/2));
                m_buffArr[idx].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));

            }
            else
            {
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if (m_buffArr[i].buffID == iName)
                    {
                        m_buffArr[i].cd += cd;
                        break;
                    }
                    else if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].buffID = iName;
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("jinu.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        
                        m_buffArr[i].showCd = true;
                        char buf[100];
                        snprintf(buf, 99, "%d", m_buffArr[i].cd.get());
                        CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                        CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                        lbBuffRound->setPosition(ccp(m_buffArr[i].spBuff->getContentSize().width,m_buffArr[i].spBuff->getContentSize().height/2));
                        m_buffArr[i].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                        lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                        break;
                    }
                }
            }
        }
            break;
            
        case EN_ENEMY_KUANGBAO:
        {
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("jinu.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
                
                m_buffArr[idx].showCd = true;
                char buf[100];
                snprintf(buf, 99, "%d", m_buffArr[idx].cd.get());
                CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                lbBuffRound->setPosition(ccp(m_buffArr[idx].spBuff->getContentSize().width,m_buffArr[idx].spBuff->getContentSize().height/2));
                m_buffArr[idx].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));

            }
            else
            {
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if (m_buffArr[i].buffID == EN_ENEMY_KUANGBAO)
                    {
                        m_buffArr[i].cd += cd;
                        break;
                    }
                    else if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].buffID = EN_ENEMY_KUANGBAO;
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("jinu.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        
                        m_buffArr[i].showCd = true;
                        char buf[100];
                        snprintf(buf, 99, "%d", m_buffArr[i].cd.get());
                        CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                        CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                        lbBuffRound->setPosition(ccp(m_buffArr[i].spBuff->getContentSize().width,m_buffArr[i].spBuff->getContentSize().height/2));
                        m_buffArr[i].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                        lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                        break;
                    }
                }
            }
        }
            break;
            
        case EN_ENEMY_GONGJIZITAI:
        {
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("gongjizitai.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
                
                m_buffArr[idx].showCd = true;
                char buf[100];
                snprintf(buf, 99, "%d", m_buffArr[idx].cd.get());
                CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                lbBuffRound->setPosition(ccp(m_buffArr[idx].spBuff->getContentSize().width,m_buffArr[idx].spBuff->getContentSize().height/2));
                m_buffArr[idx].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
            }
            else
            {
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if (m_buffArr[i].buffID == EN_ENEMY_GONGJIZITAI)
                    {
                        m_buffArr[i].cd += cd;
                        break;
                    }
                    else if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].buffID = EN_ENEMY_GONGJIZITAI;
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("gongjizitai.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        
                        m_buffArr[i].showCd = true;
                        char buf[100];
                        snprintf(buf, 99, "%d", m_buffArr[i].cd.get());
                        CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                        CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                        lbBuffRound->setPosition(ccp(m_buffArr[i].spBuff->getContentSize().width,m_buffArr[i].spBuff->getContentSize().height/2));
                        m_buffArr[i].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                        lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                        break;
                    }
                }

            }
        }
            break;
            
        case EN_ENEMY_BAONU:
        {
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("jinu.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
                
                m_buffArr[idx].showCd = true;
                char buf[100];
                snprintf(buf, 99, "%d", m_buffArr[idx].cd.get());
                CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                lbBuffRound->setPosition(ccp(m_buffArr[idx].spBuff->getContentSize().width,m_buffArr[idx].spBuff->getContentSize().height/2));
                m_buffArr[idx].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
            }
            else
            {
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if (m_buffArr[i].buffID == EN_ENEMY_BAONU)
                    {
                        m_buffArr[i].cd += cd;
                        break;
                    }
                    else if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].buffID = EN_ENEMY_BAONU;
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("jinu.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        
                        m_buffArr[i].showCd = true;
                        char buf[100];
                        snprintf(buf, 99, "%d", m_buffArr[i].cd.get());
                        CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                        CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                        lbBuffRound->setPosition(ccp(m_buffArr[i].spBuff->getContentSize().width,m_buffArr[i].spBuff->getContentSize().height/2));
                        m_buffArr[i].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                        lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                        break;
                    }
                }
            }
        }
            break;
            
            
        case EN_ENEMY_BAOZOU:
        case EN_ENEMY_BUFF118:
        case EN_ENEMY_BUFF122:
        case EN_ENEMY_BUFF133:
        case EN_ENEMY_BUFF137:
        case EN_ENEMY_BUFF142:
        case EN_ENEMY_BUFF146:
        case EN_ENEMY_BUFF149:
        case EN_ENEMY_BUFF145:
        case EN_ENEMY_BUFF157:
        case EN_ENEMY_BUFF159:
        case EN_ENEMY_BUFF202:
        case EN_ENEMY_BUFF232:
        case EN_ENEMY_BUFF247:
        case EN_ENEMY_BUFF252:
        case EN_ENEMY_BUFF447:
            case EN_ENEMY_BUFF287: //lvgenquan
            case EN_ENEMY_XUSHIDAIFA314:
        {
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("jinu.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
                if (m_buffArr[idx].showCd) {
                    char buf[100];
                    snprintf(buf, 99, "%d", m_buffArr[idx].cd.get());
                    CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                    CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                    lbBuffRound->setPosition(ccp(m_buffArr[idx].spBuff->getContentSize().width,m_buffArr[idx].spBuff->getContentSize().height/2));
                    m_buffArr[idx].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                    lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));

                }
               
            }
            else
            {
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if (m_buffArr[i].buffID == iName)
                    {
                        //m_buffArr[i].cd += cd;
                        break;
                    }
                    else if(m_buffArr[i].spBuff == NULL)
                    {
                        m_buffArr[i].buffID = iName;
                        m_buffArr[i].cd = cd;
                        m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("jinu.png");
                        m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                        addChild(m_buffArr[i].spBuff);
                        
                        m_buffArr[i].showCd = true;
                        char buf[100];
                        snprintf(buf, 99, "%d", m_buffArr[i].cd.get());
                        CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                        CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                        lbBuffRound->setPosition(ccp(m_buffArr[i].spBuff->getContentSize().width,m_buffArr[i].spBuff->getContentSize().height/2));
                        m_buffArr[i].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                        lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                        break;
                    }
                }
            }
        }
            break;
            
        case EN_ENEMY_SICK:
        {
            if (idx >= 0)
            {
                m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("sick.png");
                m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[idx].spBuff);
            }
            else
            {
                if(isAlive() == false)
                    return;
                
                if (isBuffByIdx(EN_ENEMY_HUJIA))
                {
                    showDefenFromBuff(EN_ENEMY_HUJIA);
                    return;
                }
                
                if (isBuffByIdx(EN_ENEMY_MUTENGJIA)
                    || CheckGrpBuff(enEnemyBuff_mutengjia)
                    )
                {
                    showDefenFromBuff(EN_ENEMY_MUTENGJIA);
                    return;
                }
                
                if (CheckGrpBuff(enEnemyBuff_shuxinghudun)
                    ||CheckGrpBuff(enEnemyBuff_ysshenyuan)
                    ||CheckGrpBuff(enEnemyBuff_ystaipingdao)
                    ||CheckGrpBuff(enEnemyBuff_hunyuandun)
                    ||CheckGrpBuff(enEnemyBuff_shuxinghudun2)
                    ||CheckGrpBuff(enEnemyBuff_77)
                    )
                {
                    return;
                }
                
                if (isBuffByIdx(EN_ENEMY_JINGANGHUTI))
                {
                    showDefenFromBuff(EN_ENEMY_JINGANGHUTI);
                    return;
                }
                
                for(int i = 0; i < MAX_BUFF_ITEMS; i++)
                {
                    if (m_buffArr[i].buffID == EN_ENEMY_SICK)
                    {
                        m_buffArr[i].cd += cd;
                        break;
                    }
                    else
                        if(m_buffArr[i].spBuff == NULL)
                        {
                            m_buffArr[i].cd = cd;
                            m_buffArr[i].buffID = EN_ENEMY_SICK;
                            
                            m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("sick.png");
                            m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                            addChild(m_buffArr[i].spBuff);
 
                            //sick damage
                            CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                            m_lbSick = CCLabelAtlas::labelWithString("0", "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                            m_lbSick->setPosition(ccp(m_spEnemy->getPosition().x, m_spEnemy->getPosition().y + spTemp->getContentSize().height));
                            addChild(m_lbSick);
                            m_lbSick->setAnchorPoint(CCPointMake(0.5, 0.5));
                            m_lbSick->setOpacity(0);
                            break;
                        }
                }

            }
        }
            break;
        case EN_ENEMY_POISON:
        {
            if(isAlive() == false)
                return;
            
            if (isBuffByIdx(EN_ENEMY_HUJIA))
            {
                showDefenFromBuff(EN_ENEMY_HUJIA);
                return;
            }
            
            if (isBuffByIdx(EN_ENEMY_MUTENGJIA)
                || CheckGrpBuff(enEnemyBuff_mutengjia))
            {
                showDefenFromBuff(EN_ENEMY_MUTENGJIA);
                return;
            }
            
            if (isBuffByIdx(EN_ENEMY_JINGANGHUTI))
            {
                showDefenFromBuff(EN_ENEMY_JINGANGHUTI);
                return;
            }
            
            if (CheckGrpBuff(enEnemyBuff_shuxinghudun)
                ||CheckGrpBuff(enEnemyBuff_ysshenyuan)
                ||CheckGrpBuff(enEnemyBuff_ystaipingdao)
                ||CheckGrpBuff(enEnemyBuff_hunyuandun)
                ||CheckGrpBuff(enEnemyBuff_shuxinghudun2)
                ||CheckGrpBuff(enEnemyBuff_77)
                )
            {
                return;
            }
            
            for(int i = 0; i < MAX_BUFF_ITEMS; i++)
            {
                if(m_buffArr[i].spBuff == NULL)
                {
                    m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("poison.png");
                    m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                    addChild(m_buffArr[i].spBuff);
                    
                    m_buffArr[i].cd = cd;
                    m_buffArr[i].buffID = iName;
                    m_buffArr[i].showCd = true;
                    
                    char buf[100];
                    snprintf(buf, 99, "%d", m_buffArr[i].cd.get());
                    CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                    CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                    lbBuffRound->setPosition(ccp(m_buffArr[i].spBuff->getContentSize().width,m_buffArr[i].spBuff->getContentSize().height/2));
                    m_buffArr[i].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                    lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                    
                    break;
                }
            }
            break;
        }
        case EN_ENEMY_FREEZE:
        {
            if(isAlive() == false)
                return;
            
            if (isBuffByIdx(EN_ENEMY_HUJIA))
            {
                showDefenFromBuff(EN_ENEMY_HUJIA);
                return;
            }
            
            if (isBuffByIdx(EN_ENEMY_MUTENGJIA)
                || CheckGrpBuff(enEnemyBuff_mutengjia))
            {
                showDefenFromBuff(EN_ENEMY_MUTENGJIA);
                return;
            }
            
            if (isBuffByIdx(EN_ENEMY_JINGANGHUTI))
            {
                showDefenFromBuff(EN_ENEMY_JINGANGHUTI);
                return;
            }
            
            if (CheckGrpBuff(enEnemyBuff_shuxinghudun)
                ||CheckGrpBuff(enEnemyBuff_ysshenyuan)
                ||CheckGrpBuff(enEnemyBuff_ystaipingdao)
                ||CheckGrpBuff(enEnemyBuff_hunyuandun)
                ||CheckGrpBuff(enEnemyBuff_shuxinghudun2)
                ||CheckGrpBuff(enEnemyBuff_77)
                )
            {
                return;
            }
            
            for(int i = 0; i < MAX_BUFF_ITEMS; i++)
            {
                if(m_buffArr[i].spBuff == NULL)
                {
                    m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("bingdongbuff.png");
                    m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                    addChild(m_buffArr[i].spBuff);
                    
                    m_buffArr[i].cd = cd;
                    m_buffArr[i].buffID = iName;
                    m_buffArr[i].showCd = true;
                    
                    char buf[100];
                    snprintf(buf, 99, "%d", m_buffArr[i].cd.get());
                    CCSprite* spTemp = CCSprite::spriteWithFile("rednum.png");
                    CCLabelAtlas* lbBuffRound = CCLabelAtlas::labelWithString(buf, "rednum.png", spTemp->getContentSize().width/10, spTemp->getContentSize().height, '0');
                    lbBuffRound->setPosition(ccp(m_buffArr[i].spBuff->getContentSize().width,m_buffArr[i].spBuff->getContentSize().height/2));
                    m_buffArr[i].spBuff->addChild(lbBuffRound,0,KBUFFTAGNUMBER);
                    lbBuffRound->setAnchorPoint(CCPointMake(0, 0.5));
                    
                    break;
                }
            }
            break;
        }
            
            
        default:
            break;
    }
    
    updateBuffPos();
    
    for (int i=0; i<MAX_BUFF_ITEMS; i++)
    {
        SaveData::Inst()->setEnemyBuffbuffId(i, m_buffArr[i].buffID, SaveData::Inst()->getCurStep(), m_index);
        SaveData::Inst()->setEnemyBuffbuffCd(i, m_buffArr[i].cd, SaveData::Inst()->getCurStep(), m_index);
        short s_show = m_buffArr[i].showCd ? 1:0;
        SaveData::Inst()->setEnemyBuffbuffShow(i, s_show, SaveData::Inst()->getCurStep(), m_index);
    }
}

//******************************************************************************
// changeElement
//******************************************************************************
void Enemy::changeElement(int element, int iRound, int idx)
{
    if(isAlive() == false)
        return;
    
    if (isBuffByIdx(EN_ENEMY_HUJIA))
    {
        showDefenFromBuff(EN_ENEMY_HUJIA);
        return;
    }
    
    m_iElement = element;
    
    if (idx >= 0)
    {
        char buf[100];
        snprintf(buf, 99, "stone%ds.png", m_iElement);
        m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName(buf);
        m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
        addChild(m_buffArr[idx].spBuff);
    }
    else {
        if(isAlive() == false)
            return;
        
        if (isBuffByIdx(EN_ENEMY_HUJIA))
        {
            showDefenFromBuff(EN_ENEMY_HUJIA);
            return;
        }
        
        char buf[100];
        snprintf(buf, 99, "stone%ds.png", m_iElement);
        
        for(int i = 0; i < MAX_BUFF_ITEMS; i++)
        {
            if (m_buffArr[i].buffID == EN_ENEMY_CHGELM)
            {
                m_buffArr[i].cd += iRound;
                break;
            }
            else if(m_buffArr[i].spBuff == NULL)
            {
                m_buffArr[i].cd = iRound;
                m_buffArr[i].buffID = EN_ENEMY_CHGELM;
                
                m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName(buf);
                m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[i].spBuff);
                break;
            }
        }
    }

    updateBuffPos();
}

//******************************************************************************
// breakArmor
//******************************************************************************
void Enemy::breakArmor(int iArmor, int iRound, int idx)
{
    if(isAlive() == false)
        return;
    
    if (isBuffByIdx(EN_ENEMY_HUJIA))
    {
        showDefenFromBuff(EN_ENEMY_HUJIA);
        return;
    }
    
    m_iArmor = iArmor;
    
    SaveData::Inst()->setEnemyCurArmor(m_iArmor,
                                       SaveData::Inst()->getCurStep(),
                                       m_index);
    
    if (idx >= 0)
    {
        m_buffArr[idx].spBuff = CCSprite::spriteWithSpriteFrameName("brokensheild.png");
        m_buffArr[idx].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
        addChild(m_buffArr[idx].spBuff);
    }
    else
    {
        for(int i = 0; i < MAX_BUFF_ITEMS; i++)
        {
            if (m_buffArr[i].buffID == EN_ENEMY_POJIA)
            {
                m_buffArr[i].cd += iRound;
                
                SaveData::Inst()->setEnemyBuffbuffCd(i, m_buffArr[i].cd, SaveData::Inst()->getCurStep(), m_index);
                
                break;
            }
            else if(m_buffArr[i].spBuff == NULL)
            {
                m_buffArr[i].spBuff = CCSprite::spriteWithSpriteFrameName("brokensheild.png");
                m_buffArr[i].spBuff->setAnchorPoint(CCPointMake(0.5, 1));
                addChild(m_buffArr[i].spBuff);
                
                m_buffArr[i].cd = iRound;
                m_buffArr[i].buffID = EN_ENEMY_POJIA;
                m_buffArr[i].showCd = false;
                
                SaveData::Inst()->setEnemyBuffbuffId(i, m_buffArr[i].buffID, SaveData::Inst()->getCurStep(), m_index);
                SaveData::Inst()->setEnemyBuffbuffCd(i, m_buffArr[i].cd, SaveData::Inst()->getCurStep(), m_index);
                short s_show = m_buffArr[i].showCd ? 1:0;
                SaveData::Inst()->setEnemyBuffbuffShow(i, s_show, SaveData::Inst()->getCurStep(), m_index);
                
                break;
            }
        }
    }
    
    updateBuffPos();
}

//******************************************************************************
// bePoisoned
//******************************************************************************
void Enemy::bePoisoned(int poisonPt)
{    
//    m_iHP = m_iHP - poisonPt;
//    
//    //12
//    m_blood->setBlood(m_iHP, m_iBaseHP, 0.5);
    
    changeCurHPAction(-poisonPt,0.5f);
    
    char buf[10];
    sprintf(buf, "%d", poisonPt);
    if(m_lbPoison){
        m_lbPoison->setString(buf);
        m_lbPoison->runAction(CCSequence::actions(//CCDelayTime::actionWithDuration(0.5),
                                                  CCFadeIn::actionWithDuration(0.1),
                                                  CCFadeOut::actionWithDuration(1.9),
                                                  NULL));
    }
    
    chkActBeAttack();
    
    if(m_iHP <= 0)
    {
        checkDie(0.5);
    }
    else
    {
        m_spEnemy->runAction(CCSequence::actions(//CCDelayTime::actionWithDuration(0.5),
                                                 CCShake::actionWithDuration(1.0, m_spEnemy->getPosition()),
                                                 NULL));
    }
}

void Enemy::beFreeze(int FreezePt)
{
    changeCurHPAction(-FreezePt,0.5f);
    
    char buf[10];
    sprintf(buf, "%d", FreezePt);
    if(m_lbPoison){
        m_lbPoison->setString(buf);
        m_lbPoison->runAction(CCSequence::actions(//CCDelayTime::actionWithDuration(0.5),
                                                  CCFadeIn::actionWithDuration(0.1),
                                                  CCFadeOut::actionWithDuration(0.9),
                                                  NULL));
    }
    
    chkActBeAttack();
    
    if(m_iHP <= 0)
    {
        checkDie(0.5);
    }
    else
    {
        m_spEnemy->runAction(CCSequence::actions(//CCDelayTime::actionWithDuration(0.5),
                                                 CCShake::actionWithDuration(1.0, m_spEnemy->getPosition()),
                                                 NULL));
    }
}

//******************************************************************************
// collectDropItem
//******************************************************************************
int Enemy::collectDropItem()
{
    if(m_iDropStar <= 0)
        return 0;
    
    if(m_spDropItem != NULL){
        m_spDropItem->runAction(CCSpawn::actions(CCMoveTo::actionWithDuration(0.3, CCPointMake(475, 945)),
                                                 CCRotateBy::actionWithDuration(0.3, 720),
                                                 CCScaleTo::actionWithDuration(0.3, 0.0),
                                                 NULL));
    }
    
    return 1;
}

//******************************************************************************
// onExit
//******************************************************************************
void Enemy::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCLayer::onExit();
}

//******************************************************************************
// moneyDrop
//******************************************************************************
void Enemy::moneyDrop(int dt)
{
    CCAnimation* animation;
    CCActionInterval*  action;
    animation = CCAnimation::animation();

    char frameName[50];
    for(int i=1;i<10;i++)
    {
        sprintf(frameName, "golddrop00%02d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName( frameName );
        animation->addFrame(frame);
    }
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    animation->addFrame(frame);
    action = CCAnimate::actionWithDuration(0.5, animation, false);
    m_spDropMoney->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt+0.8),
                                                 action,NULL));
}

//******************************************************************************
// updateBuffPos
//******************************************************************************
void Enemy::updateBuffPos()
{
    //buff往一起凑
    for(int i = 0; i < MAX_BUFF_ITEMS; i++)
    {
        if(m_buffArr[i].spBuff == NULL)
        {
            for(int j = i; j < MAX_BUFF_ITEMS; j++)
            {
                if(m_buffArr[j].spBuff)
                {
                    m_buffArr[i].spBuff = m_buffArr[j].spBuff;
                    m_buffArr[i].cd = m_buffArr[j].cd;
                    m_buffArr[i].buffID = m_buffArr[j].buffID;
                    m_buffArr[i].showCd = m_buffArr[j].showCd;
                    
                    
                    m_buffArr[j].spBuff = NULL;
                    m_buffArr[j].cd = 0;
                    m_buffArr[j].buffID = EN_ENEMY_NONE;
                    m_buffArr[j].showCd = false;
                    break;
                }
            }
        }
    }
   
    
    //刷新位置
    CCPoint pt = m_spRound->getPosition();
    CCSize sz = m_spRound->getContentSize();
    pt.y -= sz.height;
    float fOffsetX = 0.0;
    float fOffsetY = 0.0;
    for(int i = 0; i < MAX_BUFF_ITEMS; i++)
    {
        if(m_buffArr[i].spBuff)
        {
            m_buffArr[i].spBuff->setPosition(CCPointMake(pt.x-6 + fOffsetX, pt.y - fOffsetY));
            
            float offx = m_buffArr[i].spBuff->getContentSize().width;
            if (m_buffArr[i].showCd)
            {
                if (m_buffArr[i].cd>9)
                {
                    offx += 46;
                }
                else
                {
                    offx += 26;
                }
            }
            
            fOffsetX += offx;
            
            if (fOffsetX > m_buffArr[i].spBuff->getContentSize().width*4)
            {
                fOffsetX = 0;
                fOffsetY += m_buffArr[i].spBuff->getContentSize().height;
            }
        }
    }
}

//******************************************************************************
// decBuffCD
//******************************************************************************
void Enemy::decBuffCD(bool isEachRound)
{
    bool bNeedToUpdatePos = false;
        
    for(int i = 0; i < MAX_BUFF_ITEMS; i++)
    {
        bool condition1 = m_buffArr[i].cd > 0 && m_buffArr[i].buffID > 0;
        bool condition2 = (isEachRound && isDecEachRound(m_buffArr[i].buffID))||((!isEachRound) && (!isDecEachRound(m_buffArr[i].buffID)));
        
        if(condition1 && condition2)
        {
            m_buffArr[i].cd--;
            
            if (m_buffArr[i].showCd && m_buffArr[i].cd>0)
            {
                char buf[100];
                snprintf(buf, 99, "%d", m_buffArr[i].cd.get());
                CCLabelAtlas* tempText = (CCLabelAtlas*)m_buffArr[i].spBuff->getChildByTag(KBUFFTAGNUMBER);
                tempText->setString(buf);
                if (m_buffArr[i].cd == 9)
                {
                    bNeedToUpdatePos = true;
                }
            }
            
            switch (m_buffArr[i].buffID)
            {
                case EN_ENEMY_FREEZE:
                {
                    m_iDoSpecilAtk = EN_ENEMY_FREEZE;
                }
                    break;
                    
                case EN_ENEMY_JINGANGHUTI:
                {
                    bool onlySelflive = true;
                    EnemyGrp* pEnemyGrp = (EnemyGrp*)m_pListener;
                    for (int j=0; j < pEnemyGrp->getEnemyCount(); j++)
                    {
                        if (pEnemyGrp->getEnemyByIdx(j)->isAlive() && j != m_index)
                        {
                            onlySelflive = false;
                            break;
                        }
                    }
                    
                    if (onlySelflive)
                    {
                        m_buffArr[i].cd = 0;
                    }
                }
                    break;
                
                case EN_ENEMY_CHGELM:
                {
                    if (m_buffArr[i].cd == 0)
                    {
                        m_iElement = m_iDefaultElm;
                    }
                }
                    break;

                case EN_ENEMY_POISON:
                case EN_ENEMY_MABI:
                case EN_ENEMY_DELAY:
                    break;
                    
                case EN_ENEMY_SICK:
                    if((m_buffArr[i].cd == 0) && (m_lbSick))
                    {
                        removeChild(m_lbSick, true);
                        m_lbSick = NULL;
                    }
                    break;
                    
                case EN_ENEMY_POJIA:
                {
                    if(m_buffArr[i].cd == 0)
                    {
                        m_iArmor = m_iArmorMax;
                        SaveData::Inst()->setEnemyCurArmor(m_iArmor,SaveData::Inst()->getCurStep(),m_index);
                    }
                }
                    break;
                    
                case EN_ENEMY_XUSHIDAIFA:
                {
                    if (m_buffArr[i].cd == 0)
                    {
                        m_iAtkRatio *= 3.0f;
                        m_iDoSpecilAtk = EN_ENEMY_XUSHIDAIFA;
                    }
                }
                    break;
                case EN_ENEMY_XUSHIDAIFA238:
                {
                    if (m_buffArr[i].cd == 0)
                    {
                        m_iAtkRatio *= 2.0f;
                        m_iDoSpecilAtk = EN_ENEMY_XUSHIDAIFA238;
                    }
                    break;
                }
                case EN_ENEMY_XUSHIDAIFA250:
                {
                    if (m_buffArr[i].cd == 0)
                    {
                        m_iAtkRatio *= 2.0f;
                        m_iDoSpecilAtk = EN_ENEMY_XUSHIDAIFA250;
                    }
                    break;
                }
                    
                case EN_ENEMY_XINGYUNYIJI:
                    if (m_buffArr[i].cd == 0)
                    {
                        int abc = (SaveData::Inst()->getrand())%3;
                        switch (abc)
                        {
                            case 0:
                                m_iAtkRatio *= 1.0f;
                                break;
                               
                            case 1:
                                m_iAtkRatio *= 5.0f;
                                break;
                                
                            case 2:
                                m_iAtkRatio *= 15.0f;
                                break;
                                
                            default:
                                m_iAtkRatio *= 1.0f;
                                break;
                        }
                        m_iDoSpecilAtk = m_buffArr[i].buffID;
                    }
                    break;
                    
                case EN_ENEMY_SHASHOUJIAN:
                {
                    if (m_buffArr[i].cd == 0)
                    {
                        m_iDoSpecilAtk = EN_ENEMY_SHASHOUJIAN;
                    }
                }
                    break;
                    
                case EN_ENEMY_JINU:
                {
                    m_iAtkRatio *= 2.0f;
                    m_iCurAtkType = EN_ENEMY_JINU;
                }
                    break;
                    
                case EN_ENEMY_JINU2:
                {
                    m_iAtkRatio *= 1.3f;
                    m_iCurAtkType = EN_ENEMY_JINU2;
                }
                    break;
                case EN_ENEMY_JINU3:
                {
                    m_iAtkRatio *= 3.0f;
                    m_iCurAtkType = EN_ENEMY_JINU3;
                }
                    break;
                case EN_ENEMY_KUANGBAO:
                {
                    m_iAtkRatio *= 1.5f;
                    m_iCurAtkType = EN_ENEMY_KUANGBAO;
                }
                    break;
                    
                case EN_ENEMY_GONGJIZITAI:
                {
                    m_iAtkRatio *= 1.25f;
                    m_iCurAtkType = EN_ENEMY_GONGJIZITAI;
                }
                    break;
                    
                case EN_ENEMY_BAONU:
                {
                    m_iAtkRatio *= 2.0f;
                    m_iCurAtkType = EN_ENEMY_BAONU;
                }
                    break;
                    
                case EN_ENEMY_BAOZOU:
                    m_iAtkRatio *= 5.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                    
                case EN_ENEMY_BUFF118:
                {
                    m_iAtkRatio *= 2.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF122:
                {
                    m_iAtkRatio *= 3.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF157:
                {
                    m_iAtkRatio *= 3.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF159:
                {
                    m_iAtkRatio *= 3.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF149:
                {
                    m_iAtkRatio *= 5.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF145:
                {
                    m_iAtkRatio *= 3.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF146:
                {
                    m_iAtkRatio *= 2.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF133:
                {
                    m_iAtkRatio *= 2.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF137:
                {
                    m_iAtkRatio *= 2.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF142:
                {
                    m_iAtkRatio *= 2.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_HAOYIN:
                {
                    if (m_buffArr[i].cd == 0)
                    {
                        m_iDoSpecilAtk = EN_ENEMY_HAOYIN;
                    }
                }
                    break;
                    
                case EN_ENEMY_HAOYIN2:
                {
                   m_iDoSpecilAtk = EN_ENEMY_HAOYIN;
                }
                    break;
                    
                case EN_ENEMY_SHIJIANNINGJIE:
                    m_iActTimes = 2;
                    break;
                    
                case EN_ENEMY_BUFF202:
                {
                    m_iAtkRatio *= 3.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF232:
                {
                    m_iAtkRatio *= 3.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF247:
                {
                    m_iAtkRatio *= 2.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF249:
                {
                    m_iAtkRatio *= 3.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF252:
                {
                    m_iAtkRatio *= 2.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_BUFF287:{ //lvgenquan
                    m_iAtkRatio *= 2.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    
                    break;
                }
                case EN_ENEMY_XUSHIDAIFA294:{//lvgenquan
                    
                    if (m_buffArr[i].cd == 0)
                    {
                       m_iAtkRatio *= 11.0f;
                        m_iDoSpecilAtk = EN_ENEMY_XUSHIDAIFA294;
                    }
                    break;
                }
                    case EN_ENEMY_BUFF303:
                {
                    m_iAtkRatio *= 1.30f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                    case EN_ENEMY_BUFF306:
                {
                    m_iAtkRatio *= 3.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_XUSHIDAIFA314:
                {
                    m_iAtkRatio *= 1.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_XUSHIDAIFA316:{//lvgenquan
                    
                    if (m_buffArr[i].cd == 0)
                    {
                        m_iAtkRatio *= 1.50f;
                        m_iDoSpecilAtk = EN_ENEMY_XUSHIDAIFA316;
                    }
                    break;
                }
                case EN_ENEMY_500THOUNSAND_LIMIT:
                    setcurrentRoundAtk(0);
                    break;
                case EN_ENEMY_BUFF447:
                {
                    m_iAtkRatio *= 10.0f;
                    m_iCurAtkType = m_buffArr[i].buffID;
                    break;
                }
                case EN_ENEMY_XUSHIDAIFA508:
                {
                    if (m_buffArr[i].cd == 0) {
                        m_iAttackPt = 99999;
                        m_iDoSpecilAtk = EN_ENEMY_XUSHIDAIFA508;
                    }
                }
                    break;
                default:
                    break;
            }
            
            if(m_buffArr[i].cd == 0)
            {
                removeChild(m_buffArr[i].spBuff, true);
                m_buffArr[i].spBuff = NULL;
                m_buffArr[i].buffID = EN_ENEMY_NONE;
                m_buffArr[i].showCd = false;
                
                m_currentAtk = 0;
                bNeedToUpdatePos = true;
            }
        }
    }
    
    if(bNeedToUpdatePos)
    {
        updateBuffPos();
    }
    
    for (int i=0; i<MAX_BUFF_ITEMS; i++)
    {
        SaveData::Inst()->setEnemyBuffbuffId(i, m_buffArr[i].buffID, SaveData::Inst()->getCurStep(), m_index);
        SaveData::Inst()->setEnemyBuffbuffCd(i, m_buffArr[i].cd, SaveData::Inst()->getCurStep(), m_index);
        short s_show = m_buffArr[i].showCd ? 1:0;
        SaveData::Inst()->setEnemyBuffbuffShow(i, s_show, SaveData::Inst()->getCurStep(), m_index);
    }
}

//******************************************************************************
// clearBuffWithEnemyDie
//******************************************************************************
void Enemy::clearBuffWithEnemyDie(float dt)
{
    for(int i = 0; i < MAX_BUFF_ITEMS; i++)
    {
        if(m_buffArr[i].spBuff != NULL)
        {
//            m_buffArr[i].spBuff->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
//                                                               CCFadeOut::actionWithDuration(1.0),
//                                                               NULL));
//            
//            if (m_buffArr[i].spBuff->getChildByTag(KBUFFTAGNUMBER))
//            {
//                CCLabelAtlas* tempText = (CCLabelAtlas*)m_buffArr[i].spBuff->getChildByTag(KBUFFTAGNUMBER);
//                tempText->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(dt),
//                                                        CCFadeOut::actionWithDuration(1.0),
//                                                        NULL));
//            }
            m_buffArr[i].cd = 0;
            removeChild(m_buffArr[i].spBuff, true);
            m_buffArr[i].spBuff = NULL;
            m_buffArr[i].buffID = EN_ENEMY_NONE;
            m_buffArr[i].showCd = false;
            
        }
    }
}

void Enemy::changeEnemyCount(int num)
{
    
    m_iCurRound += num;
    char buf[20];
    sprintf(buf, "%d", m_iCurRound.get());
    m_lbRound->setString(buf);
    m_lbRound->setScale(1.5);
    m_lbRound->runAction(CCScaleTo::actionWithDuration(0.5, 1.0));

}



//void Enemy::recordSkillRunCount()
//{
//    int nCount = m_skillArr->count();
//    for (int i=0; i<nCount; i++)
//    {
//        m_skillArr->getObjectAtIndex(i)->recordRunCount();
//    }
//}

















