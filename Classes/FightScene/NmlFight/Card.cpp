//
//  Card.cpp
//  CardDemo
//
//  Created by Hao Zhang on 12-4-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "Card.h"
#include "CSkillLib.h"
#include "CGameState.h"
#include "SimpleAudioEngine.h"
#include "CGamedata.h"
#include "SaveData.h"
#include "Player.h"

using namespace CocosDenshion;

#define kToBeDeleteTag          444
#define kSkillElementTagBase    500

Card::Card(int idx):m_numAtkVal(NULL),m_mininumAtkVal(NULL)
{
    m_FengYinCD = 0;
    m_state = EN_CARD_STATE_NORMAL;
    m_idx = idx;
    m_iSklCdDwn = 0;
    
    m_isTouchOffAwakeSkill_28 = true;
    m_countryPorperty = 0;
    
    initCardFromSaveData();
    
    //card back
    m_lcCardBk = CCLayerColor::layerWithColorWidthHeight( ccc4(255, 255, 255, 0), 
                                                         m_spCard->getContentSize().width -12, 
                                                         m_spCard->getContentSize().height -12);
	addChild(m_lcCardBk, -1);
    CCPoint pt = m_spCard->getPosition();
    pt.x -= m_spCard->getContentSize().width * 0.5;
    pt.y -= m_spCard->getContentSize().height * 0.5;
    pt.x += 6;
    pt.y += 6;
    m_lcCardBk->setPositionInPixels(pt);
    
    reset();

    if (m_iCurSkillCD == 0) {
        this->runAction(CCMoveTo::actionWithDuration(0.2, CCPointMake(0, 8)));
    }
    
    showSkillElementList();
}

int Card::getCanUseNumByAwakeId(int idx)
{
    int cout = 0;
    if (EN_CARD_STATE_FNEGYIN!=m_state)     //不在FNEGYIN
    {
        for (int i=0; i<m_iAwakeNum; i++)
        {
            if (m_iAwakeSkillArr[i] == enAwakeSkill_NULL)
                break;
            if (m_iAwakeSkillArr[i] == idx)
                cout++;
        }
    }
    return cout;
}

int Card::getAwakeSkillNum()
{
    return m_iAwakeNum;
}

int Card::getAwakeSkillArr(int idx)
{
    return m_iAwakeSkillArr[idx];
}


void Card::fengYinUpdate(int cd)
{
    m_FengYinCD -= cd;
    if (m_FengYinCD <= 0)
    {
        m_FengYinCD = 0;
        setCardState(EN_CARD_STATE_NORMAL);
    }
    else
    {
        char buf[10];
        sprintf(buf, "%d",m_FengYinCD);
        TextNode* tempText =  (TextNode*)getChildByTag(FENGYINPNGTAG+1);
        tempText->setString(buf);
    }
}

//setFengYinCD
void Card::setFengYinCD(short cd)
{
    m_FengYinCD = cd;
}

void Card::setCardIndex(int cardIndex){
    m_idx = cardIndex;
}

void Card::resetCardPos(){
    m_spCard->setPositionInPixels(CCPointMake(55 + 106*m_idx, 615));
    
    CCPoint pt = m_spCard->getPosition();
    pt.x -= m_spCard->getContentSize().width * 0.5;
    pt.y -= m_spCard->getContentSize().height * 0.5;
    pt.x += 6;
    pt.y += 6;
    m_lcCardBk->setPositionInPixels(pt);
    
    if (m_state == EN_CARD_STATE_FNEGYIN) {
        getChildByTag(FENGYINPNGTAG)->setPosition(getCardPos());
        getChildByTag(FENGYINPNGTAG+1)->setPosition(getCardPos());
    }
    
    if (m_numAtkVal!=NULL) {
        m_numAtkVal->setPosition(m_spCard->getPosition());
    }
    if (m_mininumAtkVal!=NULL) {
        m_mininumAtkVal->setPosition(ccp(m_spCard->getPosition().x,m_spCard->getPosition().y - 42));
    }
    
    
    
}

//setstate
void Card::setCardState(int state, int param1)
{
    switch (state)
    {
        case EN_CARD_STATE_FNEGYIN:
        {
            if (param1 > 0)
            {
                Player* pPlayer = (Player*)getParent();
                int iOccurOdds = getCanUseNumByAwakeId(enAwakeSkill_16) * 50;
                if (SaveData::Inst()->getrand()%100+1 <= iOccurOdds || pPlayer->haveAwakeSkill_33()){   //新增
                    char fileName[100];
                    CCSprite* spAnim = CCSprite::spriteWithSpriteFrameName("blank.png");
                    addChild(spAnim,100,FENGYINDEFENSETAG);
                    spAnim->setPosition(getCardPos());
                    
                    CCAnimation *animation1 = CCAnimation::animation();
                    for(int i=1;i<=9;i++)
                    {
                        sprintf(fileName, "dikang00%02d.png",i);
                        animation1->addFrameWithFileName(fileName);
                    }
                    animation1->setDelay(1.0/20.0);
                    CCAnimate* animate1 = CCAnimate::actionWithAnimation(animation1);
                    spAnim->runAction(animate1);
                    return;
                }
                
            }
            
            m_FengYinCD += param1;

            if (m_state == state)
            {
                char buf[10];
                sprintf(buf, "%d",m_FengYinCD);
                TextNode* tempText =  (TextNode*)getChildByTag(FENGYINPNGTAG+1);
                tempText->setString(buf);
            }
            else
            {
                if (param1 > 0)
                {
                    if (m_idx == 0 || m_idx == 5)
                    {
                        Player* pPlayer = (Player*)getParent();
                        pPlayer->fengYinAutoSkill(m_idx);
                    }
                }
                
                CCSprite* cardLock = CCSprite::spriteWithSpriteFrameName("cardlock.png");
                addChild(cardLock,100,FENGYINPNGTAG);
                cardLock->setPosition(getCardPos());
                
                cardLock->runAction(CCRepeatForever::actionWithAction((CCActionInterval *)CCSequence::actions(CCFadeIn::actionWithDuration(1.0),CCFadeOut::actionWithDuration(1.0),NULL)));
                
                char buf[10];
                sprintf(buf, "%d",m_FengYinCD);
                TextNode* textCD = TextNode::textWithString(buf,55);
                addChild(textCD,101,FENGYINPNGTAG+1);
                textCD->setPosition(getCardPos());
                textCD->setShadowColor(ccBLACK);
                textCD->setColor(ccWHITE);
                m_spCard->setColor(ccc3(60, 60, 60));
            }
        }
            break;
            
        case EN_CARD_STATE_NORMAL:
            if (m_state == EN_CARD_STATE_FNEGYIN)
            {
                m_FengYinCD = 0;
                removeChildByTag(FENGYINPNGTAG, true);
                removeChildByTag(FENGYINPNGTAG+1, true);
                removeChildByTag(FENGYINDEFENSETAG, true);
                
                if (m_idx == 0 || m_idx == 5)
                {
                    Player* pPlayer = (Player*)getParent();
                    pPlayer->unFengYinAutoSkill(m_idx);
                }
            }
            m_spCard->setColor(ccWHITE);
            break;
            
        default:
            break;
    }
    
    if (state!=m_state)
    {
        m_state = state;
        if (Player *pPlayer = dynamic_cast<Player*>(getParent()))
        {
            pPlayer->updateNumByAwakeId();
        }
        
        if (EN_CARD_STATE_NORMAL==state)
        {
            awakeUpdate(true,false);
        }
        else if (EN_CARD_STATE_FNEGYIN==state)
        {
            awakeUpdate(false,false);
        }
    }
}

//******************************************************************************
// initCardFromSaveData
//******************************************************************************
void Card::initCardFromSaveData()
{
    SaveData_Card* cardInfo = SaveData::Inst()->getCardInfo(m_idx);
    
    snprintf(m_iName, 99, "%s", cardInfo->card_name);
    m_cid = cardInfo->cid;
    m_plus = cardInfo->plus;
    m_iHP = cardInfo->hp;
    m_iRecoverPt = cardInfo->recover;
    m_iAttackPt = cardInfo->attack;
    m_iElement = cardInfo->element;
    m_iSubElement = cardInfo->sub_element;
    m_iManSkillID = cardInfo->man_skill_id;
    m_iAutoSkillID = cardInfo->auto_skill_id;
    m_iCategory = cardInfo->category;
    m_iSubCategory = cardInfo->sub_category;
    m_iSkillCDMax = cardInfo->skill_cd_max;
    m_iCurSkillCD = cardInfo->cur_skill_cd;
    m_iAwakeNum = cardInfo->AwakeNum;
    m_countryPorperty = cardInfo->m_couPorperty;
    
//    m_iHP = 10000;
//    m_iAttackPt = 10000;
  //  m_iManSkillID = enManSkill_285;
   
   
    for (int i=0; i<MaxAwakeSkillCnt; i++)
    {
        m_iAwakeSkillArr[i] = cardInfo->AwakeSkillArr[i];
    }

    //card sprite
    m_spCard = CGameData::Inst()->getHeadSprite(cardInfo->cid);
    if(m_spCard){
        m_spCard->setPositionInPixels(CCPointMake(55 + 106*m_idx, 615));
        addChild(m_spCard);
        
        //究极金边
        CCSprite* sp1 = NULL;
        if (cardInfo->isEluTgt) {
            sp1 = CCSprite::spriteWithSpriteFrameName("GoldFrm.png");
        }
        else if (cardInfo->isWkTgt) {
            sp1 = CCSprite::spriteWithSpriteFrameName("wakeFrame.png");
        }
        
        if (sp1) {
            m_spCard->addChild(sp1);
            sp1->setAnchorPoint(CCPointMake(0, 1));
            sp1->setPosition(ccp(4, m_spCard->getContentSize().height-4));
        }
        
        m_FengYinCD = cardInfo->FengYinCD;
        setCardState(cardInfo->state);
    }
}

//******************************************************************************
// setNoCD
//******************************************************************************
void Card::setNoCD()
{
    if (m_iCurSkillCD == kInvalidValue)
        return;
    
    m_iCurSkillCD = 0;
    
    this->runAction(CCMoveTo::actionWithDuration(0.2, CCPointMake(0, 8)));
}

//******************************************************************************
// setJumpNumber
//******************************************************************************
void Card::setJumpNumber(CJumpNum* jumpNum, bool isMainElem)
{
    if (isMainElem)
    {
        //jump number
        m_numAtkVal = jumpNum;
        m_numAtkVal->setPosition(m_spCard->getPosition());
        m_numAtkVal->setNumber(0, 0, 0);
        m_numAtkVal->setIsVisible(false);
    }
    else
    {
        //MiniJump number
        m_mininumAtkVal = jumpNum;
        m_mininumAtkVal->setPosition(ccp(m_spCard->getPosition().x,m_spCard->getPosition().y - 42));
        m_mininumAtkVal->setNumber(0, 0, 0);
        m_mininumAtkVal->setIsVisible(false);
        m_mininumAtkVal->setScale(0.7);
    }
}

//******************************************************************************
// reset
//******************************************************************************
void Card::reset()
{
    if(m_numAtkVal != NULL){
        m_numAtkVal->setIsVisible(false);
        m_numAtkVal->setNumber(0, 0, 0);
    }
    
    if(m_mininumAtkVal != NULL){
        m_mininumAtkVal->setIsVisible(false);
        m_mininumAtkVal->setNumber(0, 0, 0);
    }
    
    m_AtkInfo.clear();
    m_SubAtkInfo.clear();
}


//******************************************************************************
// getName
//******************************************************************************
const char *Card::getName()
{
    return m_iName;
}


//******************************************************************************
// getElement
//******************************************************************************
int Card::getElement()
{
    return m_iElement;
}

int Card::getSubElement()
{
    return m_iSubElement;
}

//******************************************************************************
// getElement
//******************************************************************************
int Card::getRecover()
{
    return  m_iRecoverPt;
}

//******************************************************************************
// getElement
//******************************************************************************
int Card::getBaseHp()
{
    return m_iHP;
}


//******************************************************************************
// getAttack
//******************************************************************************
int Card::getAttack()
{
    return m_iAttackPt;
} 

//******************************************************************************
// getAttack
//******************************************************************************
int Card::getManSkId()
{
    return m_iManSkillID;
}

//******************************************************************************
// getAttack
//******************************************************************************
int Card::getAutoSkId()
{
    if (m_state == EN_CARD_STATE_FNEGYIN)
    {
        return enAutoSkill_NULL;
    }
    
    return m_iAutoSkillID;
}

int Card::carryAutoSkId()
{
    return m_iAutoSkillID;
}

//******************************************************************************
// getCategory
//******************************************************************************
int Card::getCategory()
{
    return m_iCategory;
}

int Card::getSubCategory()
{
    return m_iSubCategory;
}

//******************************************************************************
// setHpFactor
//******************************************************************************
void Card::setHpFactor(float fFactor)
{
    m_iHP *= fFactor;
}


void Card::decHpFactor(float fFactor)
{
    float f = (float)m_iHP;
    
    f /= fFactor;
    int i = (int) f;
    float tf = (float)(f-i);
    if (tf > 0.001f)
        i++;
    
    m_iHP = i;
    
    //m_iHP /= fFactor;
}

//******************************************************************************
// setRecoverFactor
//******************************************************************************
void Card::setRecoverFactor(float fFactor)
{
    m_iRecoverPt *= fFactor;
}

void Card::decRecoverFactor(float fFactor)
{
    m_iRecoverPt /= fFactor;
}

//******************************************************************************
// stoneEraseNotify
//******************************************************************************
void Card::stoneEraseNotify(int iCnt, float fStonePlus, bool bAOE, bool isMain)
{
    //Player *pPlayer = dynamic_cast<Player*>(getParent());
    
    if (isMain)
    {
        m_AtkInfo.iElement = m_iElement;
        m_AtkInfo.iAtkCategory = m_iCategory;
        m_AtkInfo.iAtkSubCategory = m_iSubCategory;
        
        m_AtkInfo.iAttackPt += (m_iAttackPt * ((iCnt - 3) * 0.25 + 1) * fStonePlus);// + (delata > 0.05f ? (m_iAttackPt*delata) : 0);
        
        if(iCnt > 4 || bAOE)
        {
            m_AtkInfo.iTargetCnt = 1;
        }
        else if(iCnt > 3 || bAOE)
        {
            unsigned short nAwakeCount = getCanUseNumByAwakeId(enAwakeSkill_27);
            if (0<nAwakeCount)
            {
                m_AtkInfo.iTargetCnt = 4;
            }
        }
        
        m_numAtkVal->setIsVisible(true);
        m_numAtkVal->setNumber(m_AtkInfo.iAttackPt, 0.3, 0.2);
        
        m_iComboBase = m_AtkInfo.iAttackPt;
    }
    else
    {
        m_SubAtkInfo.iElement = m_iSubElement;
        m_AtkInfo.iAtkCategory = m_iCategory;
        m_AtkInfo.iAtkSubCategory = m_iSubCategory;
        
        float fscale = m_iSubElement == m_iElement ? 0.1f : 0.333f;
        m_SubAtkInfo.iAttackPt += (m_iAttackPt * fscale * ((iCnt - 3) * 0.25 + 1) * fStonePlus);// + (delata > 0.05f ? (m_iAttackPt*fscale*delata) : 0);
        if(iCnt > 4 || bAOE)
        {
            m_SubAtkInfo.iTargetCnt = 1;
        }
        else if(iCnt > 3 || bAOE)
        {
            unsigned short nAwakeCount = getCanUseNumByAwakeId(enAwakeSkill_27);
            if (0<nAwakeCount)
            {
                m_SubAtkInfo.iTargetCnt = 4;
            }
        }
        
        m_mininumAtkVal->setIsVisible(true);
        m_mininumAtkVal->setNumber(m_SubAtkInfo.iAttackPt, 0.3, 0.2);
        m_iSubComoBase = m_SubAtkInfo.iAttackPt;
    }
}

//******************************************************************************
// comboNotify
//******************************************************************************
void Card::comboNotify()
{
    if(m_AtkInfo.iAttackPt != 0)
    {
        int nAutoSkillID1 = enAutoSkill_NULL,nAutoSkillID2 = enAutoSkill_NULL;
        if (Player *pPlayer = dynamic_cast<Player*>(getParent()))
        {
            if (Card *pCard = pPlayer->getCardByIdx(0))
            {
                nAutoSkillID1 = pCard->getAutoSkId();
            }
            if (Card *pCard = pPlayer->getCardByIdx(5))
            {
                nAutoSkillID2 = pCard->getAutoSkId();
            }
        }
        
        if (enAutoSkill_1Combo5_2Combo7_5==nAutoSkillID1 &&
            enAutoSkill_1Combo5_2Combo7_5==nAutoSkillID2)
        {
            m_AtkInfo.iAttackPt += m_iComboBase * 0.75f;
        }
        else if (enAutoSkill_1Combo5_2Combo7_5==nAutoSkillID1 ||
                 enAutoSkill_1Combo5_2Combo7_5==nAutoSkillID2)
        {
            m_AtkInfo.iAttackPt += m_iComboBase * 0.5f;
        }
        else
        {
            m_AtkInfo.iAttackPt += m_iComboBase * 0.25f;
        }
        m_numAtkVal->setNumber(m_AtkInfo.iAttackPt, 0.6, 0.0);
    }
    
    if(m_SubAtkInfo.iAttackPt != 0)
    {
        int nAutoSkillID1 = enAutoSkill_NULL,nAutoSkillID2 = enAutoSkill_NULL;
        if (Player *pPlayer = dynamic_cast<Player*>(getParent()))
        {
            if (Card *pCard = pPlayer->getCardByIdx(0))
            {
                nAutoSkillID1 = pCard->getAutoSkId();
            }
            if (Card *pCard = pPlayer->getCardByIdx(5))
            {
                nAutoSkillID2 = pCard->getAutoSkId();
            }
        }
        
        if (enAutoSkill_1Combo5_2Combo7_5==nAutoSkillID1 &&
            enAutoSkill_1Combo5_2Combo7_5==nAutoSkillID2)
        {
            m_SubAtkInfo.iAttackPt += m_iSubComoBase * 0.75f;
        }
        else if (enAutoSkill_1Combo5_2Combo7_5==nAutoSkillID1 ||
                 enAutoSkill_1Combo5_2Combo7_5==nAutoSkillID2)
        {
            m_SubAtkInfo.iAttackPt += m_iSubComoBase * 0.5f;
        }
        else
        {
            m_SubAtkInfo.iAttackPt += m_iSubComoBase * 0.25f;
        }
        m_mininumAtkVal->setNumber(m_SubAtkInfo.iAttackPt, 0.6, 0.0);
    }
}

//******************************************************************************
// getPosition
//******************************************************************************
const CCPoint& Card::getCardPos()
{
    return m_spCard->getPosition();
}

//******************************************************************************
// getContentSize
//******************************************************************************
const CCSize& Card::getContentSize()
{
    return m_spCard->getContentSize();
}

//******************************************************************************
// getAttackPt
//******************************************************************************
CAttackInfo& Card::getAttackInfo()
{
    //標記爲非技能攻擊
    m_AtkInfo.iManSkillID = kInvalidValue;
    m_AtkInfo.iAutoSkillID = kInvalidValue;
    
    m_AtkInfo.srcPt = m_spCard->getPosition();
    return m_AtkInfo;
}

void Card::setSpecialAtk(int type , int cout,int nAttack)
{
    m_AtkInfo.nEffectType = type;
    m_AtkInfo.chRoundCount = cout;
    m_AtkInfo.nEffectAttack = nAttack;
}

void Card::setSubSpecialAtk(int type , int cout,int nAttack)
{
    m_SubAtkInfo.nEffectType = type;
    m_SubAtkInfo.chRoundCount = cout;
    m_SubAtkInfo.nEffectAttack = nAttack;
}

//******************************************************************************
// getAttackPt
//******************************************************************************
CAttackInfo& Card::getSubAttackInfo()
{
    //標記爲非技能攻擊
    m_SubAtkInfo.iManSkillID = kInvalidValue;
    m_SubAtkInfo.iAutoSkillID = kInvalidValue;
    
    m_SubAtkInfo.srcPt = ccp(m_spCard->getPosition().x, m_spCard->getPosition().y - 30);
    return m_SubAtkInfo;
}

//******************************************************************************
// getCD
//******************************************************************************
int Card::getCD()
{
    if(m_iCurSkillCD < 0)
        return 0;
    else
        return m_iCurSkillCD;
}

void Card::updateCd(int iCd)
{
    m_iCurSkillCD += iCd;
    
    if (m_iCurSkillCD <= 0) {
        m_iCurSkillCD = 0;
        this->setPosition(CCPointMake(0, 8));
    }
}


//******************************************************************************
// useManSkill
//******************************************************************************
CAttackInfo& Card::useManSkill()
{
    this->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(0.2, CCPointMake(0, 0)),
                                        CCCallFunc::actionWithTarget(this, callfunc_selector(Card::showSkillAnimation)),
                                        NULL));
    m_numAtkVal->setPosition(m_spCard->getPosition());
    m_iCurSkillCD = (m_iSkillCDMax - m_iSklCdDwn);
    
    stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(m_iManSkillID);
    if (pManSkill->iType == enManSkillTp_atk
        && pManSkill->iEffectBase == 2)
    {
        int iEffect1 = (int)pManSkill->fEffect;
        int iEffect2 = (pManSkill->fEffect - (int)pManSkill->fEffect) * 1000;
        int iEffect3 = (iEffect2 - iEffect1 + 1) * (SaveData::Inst()->getrand()/100.0) + iEffect1;
        m_skillAtkInfo.iAttackPt = m_iAttackPt * iEffect3;
        if (m_iManSkillID == enManSkill_272) {
            m_skillAtkInfo.iTargetCnt = 1;
        }
    }
    
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(useskill));
    
    return m_skillAtkInfo;
}

//******************************************************************************
// showSkillAnimtion
//******************************************************************************
void Card::showSkillAnimation() {
    CCAnimation *skillAnim = CCAnimation::animation();
    char skillAnimName[100] = {0};
    for (int i = 2; i <= 13 ; i++) {
        sprintf(skillAnimName, "skill00%02d.png", i);
        CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(skillAnimName);
        skillAnim->addFrame(frame);
    }
    CCSpriteFrame* frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("blank.png");
    skillAnim->addFrame(frame);

    skillAnim->setDelay(0.5f / 14);
    CCAnimate* skillAction = CCAnimate::actionWithAnimation(skillAnim, true);
    
    CCSprite *spSkillAnim = CCSprite::spriteWithSpriteFrameName("skill0001.png");
    spSkillAnim->setPositionInPixels(CCPointMake(55 + 106 * m_idx, 615));
    spSkillAnim->setScale(2.0f);
    addChild(spSkillAnim, 1, kToBeDeleteTag);
    
    spSkillAnim->runAction(CCSequence::actions(skillAction,
                                               CCCallFunc::actionWithTarget(this, callfunc_selector(Card::delUnusedObj)),
                                               NULL));
}

//******************************************************************************
// delUnusedObj
//******************************************************************************
void Card::delUnusedObj()
{
    CCNode* node = this->getChildByTag(kToBeDeleteTag);
    
    while (node != NULL) {
        removeChild(node, true);
        node = this->getChildByTag(kToBeDeleteTag);
    }
}

//******************************************************************************
// getCDMax
//******************************************************************************
int Card::getCDMax()
{
    return (m_iSkillCDMax - m_iSklCdDwn);
}


void Card::resetSkillCD()
{
    if (m_iCurSkillCD == 0)
    {
        runAction(CCMoveTo::actionWithDuration(0.2, CCPointMake(0, 0)));
        m_numAtkVal->setPosition(m_spCard->getPosition());
    }
    
    m_iCurSkillCD = (m_iSkillCDMax - m_iSklCdDwn);
}

//******************************************************************************
// decCD
//******************************************************************************
void Card::decCD()
{
    if(m_iSkillCDMax == kInvalidValue)
        return;
    
    
//    m_iCurSkillCD = 1; //shikun
    
    if(m_iCurSkillCD == 1){
        this->runAction(CCMoveTo::actionWithDuration(0.2, CCPointMake(0, 8)));
        m_numAtkVal->setPosition(CCPointMake(m_spCard->getPosition().x, m_spCard->getPosition().y+8));
        if(CGameData::Inst()->isSoundOn)
        {
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(skillavail));
        }
    }
    --m_iCurSkillCD;
}

//******************************************************************************
// attack
//******************************************************************************
void Card::attack()
{
    m_lcCardBk->runAction(CCSequence::actions(CCFadeIn::actionWithDuration(0.1),
                                              CCFadeOut::actionWithDuration(0.1),
                                              NULL));
}


//******************************************************************************
// initSkillAtk
//******************************************************************************
void Card::initSkillAtk()
{
    stManualSkill* pManSkill = CSkillLib::Ins()->getManSkill(m_iManSkillID);
    
    if(pManSkill == NULL)
        return;
    
    m_skillAtkInfo.bAtkOvr = false;
    if (pManSkill->cElement == enElementMax)
        m_skillAtkInfo.iElement = m_iElement;
    else
        m_skillAtkInfo.iElement = pManSkill->cElement;
    m_skillAtkInfo.iTargetCnt = pManSkill->cTargetCnt;
    m_skillAtkInfo.iManSkillID = m_iManSkillID;
    m_skillAtkInfo.iAutoSkillID = kInvalidValue;
    m_skillAtkInfo.iAtkCategory = m_iCategory;
    m_skillAtkInfo.iAtkSubCategory = m_iSubCategory;
    m_skillAtkInfo.srcPt = m_spCard->getPosition();
    
    switch (pManSkill->iType) {
        case enManSkillTp_null:
            break;
            
        case enManSkillTp_atk:
            //卡牌攻擊力作爲基數
            if(pManSkill->iEffectBase == 1){
                m_skillAtkInfo.iAttackPt = m_iAttackPt * pManSkill->fEffect;
            }
            //卡牌攻擊力作爲基數,一定概率翻倍
            else if (pManSkill->iEffectBase == 2)
            {
            }
            //固定數值作爲基數
            else{
                m_skillAtkInfo.iAttackPt = pManSkill->iEffectBase * pManSkill->fEffect;
            }
            break;
        
        //重力，延時，破甲 由敵人自己判斷
        case enManSkillTp_gravity:
        case enManSkillTp_delay:
        case enManSkillTp_break:
            break;
            
        case enManSkillTp_poison:
        case enManSkillTp_sick:
            m_skillAtkInfo.iAttackPt = m_iAttackPt * pManSkill->fEffect;
            break;
            
        case enManSkillTp_recover:
            //寵物回覆力作爲基數
            if(pManSkill->iEffectBase == 6)
                m_skillAtkInfo.iAttackPt = m_iRecoverPt * pManSkill->fEffect;
            else
                m_skillAtkInfo.iAttackPt = pManSkill->iEffectBase;
            break;
            
        //加防,寶石變化系 由Player處理
        case enManSkillTp_armor:
        case enManSkillTp_stonetrans:
        case enManSkillTp_stonerand:
        case enManSkillTp_luanshi:
            break;
        case enManSkillTp_stoneAtk:
            m_skillAtkInfo.iAttackPt = m_iAttackPt * pManSkill->fEffect;
            break;
        case enManSkillTp_EnWuJiangType:
        {
            if (m_skillAtkInfo.iManSkillID == enManSkill_284) {
                m_skillAtkInfo.iAttackPt = m_iAttackPt * 10;
            }
        }
            break;
        default:
            break;
    }
}

void Card::subUseAutoSkill(float fEffect)
{
    m_SubAtkInfo.iAttackPt *= fEffect;
    m_mininumAtkVal->setNumber(m_SubAtkInfo.iAttackPt, 0.6, 0.0);
}



void Card::drawSkillX3(float fEffect, int iBaoJi )
{
    float fDur = 0.0;
    
    if(fEffect < -2.0){
        fDur = 0.3;
    }
    else if(fEffect < 0.0){
        fDur = 0.1;
    }
    else{

        fDur = 0.3;
    }
    
    CCPoint ptSrc = CCPointMake(0, 0);
    if (getCD() == 0)
        ptSrc = CCPointMake(0, 8);
    CCPoint ptDst = CCPointMake(ptSrc.x, ptSrc.y + 8);
    
    this->stopAllActions();
    this->setPositionInPixels(ptSrc);
    this->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(fDur, ptDst),
                                        CCMoveTo::actionWithDuration(fDur, ptSrc),
                                        NULL));
    
    if (iBaoJi > 0) {
        CCSprite* spBaojiBg = CCSprite::spriteWithSpriteFrameName("baojibg.png");
        if (spBaojiBg) {
            spBaojiBg->setAnchorPoint(CCPointMake(0.5, 0));
            spBaojiBg->setPositionInPixels(CCPointMake(55 + 106*m_idx, 615 + m_spCard->getContentSize().height * 0.5));
            addChild(spBaojiBg,0,kToBeDeleteTag);
        }
        
        CCSprite *sp = CCSprite::spriteWithFile("baojinum.png");
        if (sp) {
            char buf[10];
            snprintf(buf, 9, "/%d", iBaoJi);
            CCLabelAtlas *lbb = CCLabelAtlas::labelWithString(buf,
                                                              "baojinum.png",
                                                              sp->getContentSize().width / 11,
                                                              sp->getContentSize().height, '/');
            lbb->setPositionInPixels(CCPointMake(55 + 106*m_idx, 615 + m_spCard->getContentSize().height * 0.5));
            lbb->setAnchorPoint(CCPointMake(0.5, 0));
            addChild(lbb,0,kToBeDeleteTag);
            
            lbb->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(fDur*2),
                                               CCCallFunc::actionWithTarget(this, callfunc_selector(Card::delUnusedObj)),
                                               NULL));
        }
    }
}


//******************************************************************************
// useAutoSkill
//******************************************************************************
void Card::useAutoSkill(float fEffect, int iBaoJi)
{
    float fDur = 0.0;
    
    if(fEffect < -2.0){
        fDur = 0.3;
    }
    else if(fEffect < 0.0){
        fDur = 0.1;
    }
    else{
        m_AtkInfo.iAttackPt *= fEffect;
        m_numAtkVal->setNumber(m_AtkInfo.iAttackPt, 0.6, 0.0);
        
        fDur = 0.3;
    }
    
    CCPoint ptSrc = CCPointMake(0, 0);
    if (getCD() == 0)
        ptSrc = CCPointMake(0, 8);
    CCPoint ptDst = CCPointMake(ptSrc.x, ptSrc.y + 8);
    
    this->stopAllActions();
    this->setPositionInPixels(ptSrc);
    this->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(fDur, ptDst),
                                        CCMoveTo::actionWithDuration(fDur, ptSrc),
                                        NULL));
    
    if (iBaoJi > 0) {
        CCSprite* spBaojiBg = CCSprite::spriteWithSpriteFrameName("baojibg.png");
        if (spBaojiBg) {
            spBaojiBg->setAnchorPoint(CCPointMake(0.5, 0));
            spBaojiBg->setPositionInPixels(CCPointMake(55 + 106*m_idx, 615 + m_spCard->getContentSize().height * 0.5));
            addChild(spBaojiBg,0,kToBeDeleteTag);
        }
        
        CCSprite *sp = CCSprite::spriteWithFile("baojinum.png");
        if (sp) {
            char buf[10];
            snprintf(buf, 9, "/%d", iBaoJi);
            CCLabelAtlas *lbb = CCLabelAtlas::labelWithString(buf,
                                                              "baojinum.png",
                                                              sp->getContentSize().width / 11,
                                                              sp->getContentSize().height, '/');
            lbb->setPositionInPixels(CCPointMake(55 + 106*m_idx, 615 + m_spCard->getContentSize().height * 0.5));
            lbb->setAnchorPoint(CCPointMake(0.5, 0));
            addChild(lbb,0,kToBeDeleteTag);
            
            lbb->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(fDur*2),
                                               CCCallFunc::actionWithTarget(this, callfunc_selector(Card::delUnusedObj)),
                                               NULL));
        }
    }
}

//******************************************************************************
// yanChiSkillCD
//******************************************************************************
void Card::yanChiSkillCD(bool bYanChi)
{
    if (m_iCurSkillCD != kInvalidValue)
    {
        if (bYanChi)
        {
            if (m_iCurSkillCD <= 0)
            {
                this->setPosition(CCPointMake(0, 0));
                m_numAtkVal->setPosition(m_spCard->getPosition());
                m_iCurSkillCD = 0;
            }
            
            m_iCurSkillCD = m_iCurSkillCD + 3;
            m_iSkillCDMax = m_iSkillCDMax * 2;
        }
        else
        {
            m_iSkillCDMax = m_iSkillCDMax / 2;
             m_iCurSkillCD = MIN(m_iCurSkillCD.get(), m_iSkillCDMax);
        }
    }
}

//******************************************************************************
// yanChiFromSaveData
//******************************************************************************
void Card::yanChiFromSaveData()
{
    if (m_iCurSkillCD != kInvalidValue)
    {
        m_iSkillCDMax = m_iSkillCDMax * 2;
    }
}

//******************************************************************************
// skillDecCD
//******************************************************************************
void Card::skillDecCD(int cd)
{
    CCSprite* spWhite = CCSprite::spriteWithFile("headw.png");
    if (spWhite) {
        addChild(spWhite, 2, kToBeDeleteTag);
        spWhite->setPositionInPixels(CCPointMake(55 + 106*m_idx, 615));
        spWhite->setIsVisible(false);
        spWhite->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.7),
                                               CCShow::action(),
                                               CCFadeIn::actionWithDuration(0.15),
                                               CCFadeOut::actionWithDuration(0.15),
                                               CCCallFunc::actionWithTarget(this, callfunc_selector(Card::delUnusedObj)),
                                               NULL));
        
        CCProgressTimer* tmOrange = CCProgressTimer::progressWithFile("heado.png");
        if (tmOrange) {
            addChild(tmOrange, 2, kToBeDeleteTag);
            tmOrange->setPositionInPixels(CCPointMake(55 + 106*m_idx, 615));
            tmOrange->setType(kCCProgressTimerTypeRadialCCW);
            tmOrange->setPercentage(99);
            tmOrange->runAction(CCProgressTo::actionWithDuration(0.7, 0));
        }
    }
    
    m_iSklCdDwn = cd;
    m_iCurSkillCD = MIN(m_iCurSkillCD.get(), (m_iSkillCDMax - m_iSklCdDwn));
}

//******************************************************************************
// resetSklCdDwn
//******************************************************************************
void Card::resetSklCdDwn()
{
    m_iSklCdDwn = 0;
    
    delUnusedObj();
}

//******************************************************************************
// showSkillElementList
//******************************************************************************
void Card::showSkillElementList()
{
    if (m_iAutoSkillID != enAutoSkill_NULL
        && (m_idx == 0 || m_idx == 5))
    {
        stAutoSkill* pAutoSkill = CSkillLib::Ins()->getAutoSkill(m_iAutoSkillID);
        if (pAutoSkill->iType == enAutoSkillTp_atk_elm_cnt)
        {
            for (int i = enElementHeart; i <= enElementDark; i++) {
                m_listSkillElement.push_back((enElementDef)i);
            }
        }
        else if (pAutoSkill->iType == enAutoSkillTp_atk_elm_type)
        {
            for (int i = 0; i < 3; i++)
            {
                m_listSkillElement.push_back(element3Atk25[pAutoSkill->cOdds][i]);
            }
        }
        else if (pAutoSkill->iType == enAutoSkillTp_atk_elm_type4)
        {
            for (int i = 0; i < 4; ++i)
            {
                m_listSkillElement.push_back(elementAtk4[pAutoSkill->cOdds][i]);
            }
        }
    }

    if (m_listSkillElement.size() == 0)
        return;
    
    char buf[100];    
    int i = 0;
    for (list<enElementDef>::iterator elem = m_listSkillElement.begin();
         elem != m_listSkillElement.end();
         ++elem) {
        enElementDef element = *elem;
        snprintf(buf, 99, "stone%ds.png", element);
        CCSprite* spElement = CCSprite::spriteWithSpriteFrameName(buf);
        m_spCard->addChild(spElement, 1, kSkillElementTagBase + element);
        spElement->setScale(0.5);
        spElement->setOpacity(128);
        float middlePosX = m_spCard->getContentSize().width * 0.5;
        float sizeWidth = spElement->getContentSize().width * 0.5;
        float starBeginPosX = middlePosX - sizeWidth * (m_listSkillElement.size() - 1) / 2;
        spElement->setPosition(CCPointMake(starBeginPosX + sizeWidth * i,
                                           10));
        
        i++;
    }
}

//******************************************************************************
// resetSkillElementList
//******************************************************************************
void Card::resetSkillElementList()
{
    if (m_listSkillElement.size() == 0)
        return;

    for (list<enElementDef>::iterator elem = m_listSkillElement.begin();
         elem != m_listSkillElement.end();
         ++elem) {
        enElementDef element = *elem;
        CCSprite* spElement = (CCSprite*)m_spCard->getChildByTag(kSkillElementTagBase + element);
        if (spElement)
            spElement->setOpacity(128);
    }
}

//******************************************************************************
// showSkillElement
//******************************************************************************
void Card::showSkillElement(enElementDef elem)
{
    if (m_state == EN_CARD_STATE_FNEGYIN)
    {
        return;
    }

    if (m_listSkillElement.size() == 0)
        return;

    CCSprite* spElement = (CCSprite*)m_spCard->getChildByTag(kSkillElementTagBase + elem);
    if (spElement)
        spElement->setOpacity(255);
}

//******************************************************************************
// getBaseRecover
//******************************************************************************
int Card::getBaseRecover()
{
    SaveData_Card* cardInfo = SaveData::Inst()->getCardInfo(m_idx);
    return cardInfo->recover;
}

//******************************************************************************
// onExit
//******************************************************************************
void Card::onExit()
{
    removeAllChildrenWithCleanup(true);

    CCLayer::onExit();
}

unsigned short Card::getCardID()
{
    return m_cid;
}

unsigned short Card::getCardPlus()
{
    return m_plus;
}


void Card::awakeUpdate(bool bAdd,bool bInit /*= true*/)
{
    for (int i = 0; i < MaxAwakeSkillCnt; ++i)
    {
        if (i<m_iAwakeNum)
        {
            stAwakeSkill* pAwakeSkill = CSkillLib2::Ins()->getAwakeSkill(m_iAwakeSkillArr[i]);
            if (!pAwakeSkill)continue;
            switch (pAwakeSkill->iType)
            {
                case enAwakeSkillTp_valChg_hp:
                {
                    int nValue = pAwakeSkill->fEffect*(bAdd? 1.0f:-1.0f);
                    m_iHP += nValue;
                    if (!bInit)
                    {
                        if (Player *pPlayer = dynamic_cast<Player*>(getParent()))
                        {
                            pPlayer->setBlood(pPlayer->getCurHp()+nValue,pPlayer->getBaseHP()+nValue);
                        }
                    }
                    break;
                }
                case enAwakeSkillTp_valChg_atk:
                {
                    int nValue = pAwakeSkill->fEffect*(bAdd? 1.0f:-1.0f);
                    m_iAttackPt += nValue;
                    break;
                }
                case enAwakeSkillTp_valChg_rcv:
                {
                    int nValue = pAwakeSkill->fEffect*(bAdd? 1.0f:-1.0f);
                    m_iRecoverPt += nValue;
                    if (!bInit)
                    {
                        if (Player *pPlayer = dynamic_cast<Player*>(getParent()))
                        {
                            pPlayer->setBaseRecoverPt(pPlayer->getBaseRecoverPt()+nValue);
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

short Card::getCountryPorperty()
{
    return m_countryPorperty;
}

bool Card::bisTouchOffAwakeSkill_28(){
    return m_isTouchOffAwakeSkill_28;
}

void Card::setisTouchOffAwakeSkill_28(bool bl)
{
    m_isTouchOffAwakeSkill_28 = bl;
}
