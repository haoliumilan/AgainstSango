//
//  PackCard.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-25.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "PackCard.h"
#include "CGamedata.h"
#include "TextNode.h"
#include "Package.h"
#include "CSkillLib.h"
#include "OcProxy.h"

#define  kPackCardTxtTag1  0x987
#define  kPackCardTxtTag2  0x988
#define  kPackCardTxtTag3  0x989


#define UI_TAB_HINT_TEXT0   100
#define UI_TAB_HINT_TEXT1   101
#define UI_TAB_HINT_TEXT2   102
#define UI_TAB_HINT_TEXT3   103
#define UI_TAB_HINT_TEXT4   104
#define UI_TAB_HINT_TEXT5   105
#define UI_TAB_HINT_COUNT   6


//******************************************************************************
// Constructor
//******************************************************************************
PackCard::PackCard(CUserCardInfo* pUsrCard, bool bUsing, bool bUsingGray,bool bchkTeam)
{
    m_bInCurTeam = false;
    m_bGray = false;
    m_spGray = NULL;
    m_spSel = NULL;
    m_spLock = NULL;
    m_spHead = NULL;
    m_bSel = false;
    m_bUsed = false;
    m_bPowUp = false;
    m_bShow = false;
    m_bJustGray = false;
    m_bCostEnough = true;
    m_bGrayByNiePanFood = false;
    m_updTime = pUsrCard->upd_time;
    //m_cardInfo = pUsrCard;
    m_eatExp = pUsrCard->base_exp * pUsrCard->lv;
   // m_ucid = pUsrCard->ucid;
    memcpy(m_ucid, pUsrCard->ucid, 99);
    m_cType = pUsrCard->ctype;
    m_iSellGold = pUsrCard->sell_gold;
    m_attack = pUsrCard->attack;
    m_hp = pUsrCard->hp;
    m_recover = pUsrCard->recover;
    m_plus = pUsrCard->plus;
    m_star = pUsrCard->star;
    m_category = pUsrCard->category;
    m_countryperperty = pUsrCard->cou_name;
    m_iCid = pUsrCard->cid;
    m_lv = pUsrCard->lv;
    m_lvMax = pUsrCard->max_lv;
    m_bIsEluTgt = pUsrCard->bIsEluTgt;
    m_bIsWkTgt = pUsrCard->bIsWkTgt;
    m_iCost = pUsrCard->cost;
    m_unSortType = -1;
    m_iMaxAwake = 0;
    m_iIndex = -1;
    for (int i=0; i<MaxAwakeSkillCnt; i++)
    {
        m_iMaxAwake = i;
        if (pUsrCard->AwakeSkillArr[i] == 0)
        {
            break;
        }
        m_iMaxAwake++;
    }
    
    if(bUsing && !bchkTeam)
    {
        if (CGameData::Inst()->isOneOfTeam(pUsrCard->ucid)) {
            m_bUsed = true;
        }
    }
    
    // lock
    m_bLock = CGameData::Inst()->getIsLock(m_ucid);
    
    m_awakeIndex = pUsrCard->i_AwakeNum;
    
    if(m_bUsed)
    {
        if(bUsingGray){
            m_bGray = true;
        }
    }
    
    if (bchkTeam)
    {
        m_bInCurTeam = CGameData::Inst()->isInTeamForIdx(pUsrCard->ucid, CGameData::Inst()->getCurTeamIdx());
        
        if (m_bInCurTeam)
        {
            if(bUsingGray)
            {
                m_bGray = true;
            }
        }
    }
    
}

int PackCard::getMaxAwake()
{
    return m_iMaxAwake;
}


//******************************************************************************
// isGray
//******************************************************************************
bool PackCard::isGray()
{
    return m_bGray;
}

//******************************************************************************
// setIsGrayByNiePanFood
//******************************************************************************
void PackCard::setIsGrayByNiePanFood(bool bGray)
{
    m_bGrayByNiePanFood = bGray;
    setGray(bGray);
}

//******************************************************************************
// getIsGrayByNiePanFood
//******************************************************************************
bool PackCard::getIsGrayByNiePanFood()
{
    return m_bGrayByNiePanFood;
}

//******************************************************************************
// setGray
//******************************************************************************
void PackCard::setGray(bool bGray)
{
    m_bGray = bGray;
    
    if(m_bGray){
        if(m_bShow && (m_spGray == NULL)){
            m_spGray = CCSprite::spriteWithSpriteFrameName("fr_black.png");
            if(m_spGray != NULL){
                m_spHead->addChild(m_spGray);
                m_spGray->setAnchorPoint(CCPointMake(0, 0));
                m_spGray->setPosition(CCPointMake(0, -1));
            }
        }
    }
    else{
        if(m_spGray && m_spHead){
            m_spHead->removeChild(m_spGray, true);
            m_spGray = NULL;
        }
    }
} 

//******************************************************************************
// setCostEnough
//******************************************************************************
void PackCard::setCostEnough(bool bEnough)
{
    m_bCostEnough = bEnough;
    if (m_spHead) {
        char chBuf[128];
        snprintf(chBuf, 99, "%s", CGameData::Inst()->getLanguageValue("cost_not_enough"));
        if (!m_bCostEnough)
        {
            for (int i = 0; i < m_spHead->getChildren()->count(); i++) {
                CCNode *temp = (CCNode *)(m_spHead->getChildren()->objectAtIndex(i));
                if (temp->getTag() == UI_TAB_HINT_TEXT1) {
                    m_spHead->removeChild(temp, true);
                    temp = NULL;
                    break;
                }
            }
            TextNode* lbText = TextNode::textWithString(chBuf, CCSizeMake(100, 20), CCTextAlignmentCenter, 20);
            lbText->setPosition(CCPointMake(m_size.width * 0.5, 10));
            lbText->setColor(ccORANGE);
            m_spHead->addChild(lbText, 2,UI_TAB_HINT_TEXT1);
        } else {
            for (int i = 0; i < m_spHead->getChildren()->count(); i++) {
                CCNode *temp = (CCNode *)(m_spHead->getChildren()->objectAtIndex(i));
                if (temp->getTag() == UI_TAB_HINT_TEXT1) {
                    m_spHead->removeChild(temp, true);
                    temp = NULL;
                    break;
                }
            }
            if (CCSprite* spTmp = CCSprite::spriteWithFile("lvnum.png"))
            {
                if(m_lv == m_lvMax){
                    snprintf(chBuf, sizeof(chBuf), "()*+");
                }
                else{
                    snprintf(chBuf, sizeof(chBuf), ",-%d", m_lv);
                }
                CCSize szSize = spTmp->getContentSize();
                CCLabelAtlas *pLabelAtlas1 = CCLabelAtlas::labelWithString(chBuf, "lvnum.png", szSize.width / 18, szSize.height, '(');
                pLabelAtlas1->setAnchorPoint(CCPointMake(0.5, 0));
                pLabelAtlas1->setPosition(CCPointMake(m_size.width * 0.5, 0));
                m_spHead->addChild(pLabelAtlas1, 1,UI_TAB_HINT_TEXT1);
            }
        }
    }
}

//******************************************************************************
// justSetGray
//******************************************************************************
void PackCard::justSetGray()
{
    m_bJustGray = true;
    
    if(m_bShow && (m_spGray == NULL)){
        m_spGray = CCSprite::spriteWithSpriteFrameName("fr_black.png");
        if(m_spGray != NULL){
            m_spHead->addChild(m_spGray);
            m_spGray->setAnchorPoint(CCPointMake(0, 0));
            m_spGray->setPosition(CCPointMake(0, -1));
        }
    }
}

//******************************************************************************
// setPowUp
//******************************************************************************
void PackCard::setPowUp(bool bPowUp) {
    m_bPowUp = bPowUp;
}

//******************************************************************************
// isPowUp
//******************************************************************************
bool PackCard::isPowUp() {
    return m_bPowUp;
}

//******************************************************************************
// isUsed
//******************************************************************************
bool PackCard::isUsed() {
    return m_bUsed;
}


//******************************************************************************
// setSel
//******************************************************************************
void PackCard::setSel(bool bSel)
{
    m_bSel = bSel;

    if(m_bSel && m_bShow){
        if(m_spGray == NULL){
            m_spGray = CCSprite::spriteWithSpriteFrameName("fr_black.png");
            if(m_spGray != NULL){
                m_spHead->addChild(m_spGray);
                m_spGray->setAnchorPoint(CCPointMake(0, 0));
                m_spGray->setPosition(CCPointMake(0, -1));
            }
        }
        
        if(m_spSel == NULL){
            m_spSel = CCSprite::spriteWithSpriteFrameName("fr_selected.png");
            if(m_spSel != NULL){
                m_spHead->addChild(m_spSel);
                m_spSel->setAnchorPoint(CCPointMake(1, 1));
                m_spSel->setPosition(CCPointMake(m_size.width, m_size.height));
            }
        }
    }
    else{
        if(m_spGray && m_spHead){
            m_spHead->removeChild(m_spGray, true);
            m_spGray = NULL;
        }
        
        if(m_spSel && m_spHead){
            m_spHead->removeChild(m_spSel, true);
            m_spSel = NULL;
        }
    }
}

//******************************************************************************
// isGray
//******************************************************************************
bool PackCard::isSelected()
{
    return m_bSel;
}

//******************************************************************************
// getEatExp
//******************************************************************************
int PackCard::getEatExp()
{
    return m_eatExp;
}

//******************************************************************************
// getUcid
//******************************************************************************
char* PackCard::getUcid()
{
    return m_ucid;
}

//******************************************************************************
// lockCard
//******************************************************************************
void PackCard::lockCard()
{
    m_bLock = true;
    if(m_bShow && (!m_spLock)) {
        m_spLock = CCSprite::spriteWithFile("awakeshow_qa.png");
        m_spLock->setAnchorPoint(CCPointMake(0.5, 1));
        m_spLock->setPosition(CCPointMake(m_size.width+4, -12));
        addChild(m_spLock, 3);
    }
}

void PackCard::awakeIndexShow()
{
    CCSprite* awakeBg = CCSprite::spriteWithFile("awakeshow_bg.png");
    awakeBg->setAnchorPoint(CCPointMake(1, 1));
    awakeBg->setPosition(CCPointMake(m_size.width-5, 0));
    addChild(awakeBg, 3);
    char buf[20];
    if (m_iMaxAwake == m_awakeIndex)
        snprintf(buf, 19, "awakeshow_max.png");
    else
        snprintf(buf, 19, "awakeshow_%d.png", m_awakeIndex);
    CCSprite* awakeNum = CCSprite::spriteWithFile(buf);
    awakeNum->setAnchorPoint(CCPointMake(0.5, 0.5));
    awakeNum->setPosition(CCPointMake(m_size.width-20, -15));
    addChild(awakeNum, 4);
}

//******************************************************************************
// unlockCard
//******************************************************************************
void PackCard::unlockCard()
{
    m_bLock = false;
    if (m_spLock) {
        removeChild(m_spLock, true);
        m_spLock = NULL;
        
        m_bLock = false;
    }
}

//******************************************************************************
// setIsLockCard
//******************************************************************************
void PackCard::setIsLockCard(bool bLock)
{
    m_bLock = bLock;
    
    if (bLock) {
        CGameData::Inst()->setIsLock(m_ucid, true);
        lockCard();
    }
    else {
        CGameData::Inst()->setIsLock(m_ucid, false);
        unlockCard();
    }
}

//******************************************************************************
// getcType
//******************************************************************************
int PackCard::getcType()
{
    return m_cType;
}

//******************************************************************************
// getSellGold
//******************************************************************************
int PackCard::getSellGold()
{
    return m_iSellGold + m_plus * 10000;
}

int PackCard::getPlus()
{
    return m_plus;
}

int PackCard::getAttack()
{
    return m_attack;
}

int PackCard::getHp()
{
    return m_hp;
}

int PackCard::getCost()
{
    return m_iCost;
}

int PackCard::getRecover()
{
    return m_recover;
}

unsigned long PackCard::getUpdTime()
{
    return m_updTime; 
}


int PackCard::getStar()
{
    return m_star;
}
CUserCardInfo* PackCard::getUserCardInfo()
{
    CUserCardInfo* pInf = NULL;
    
    CCMutableArray<CUserCardInfo*> *  pCardsArr = CGameData::Inst()->getUserCards();
    
    for (int i = 0; i < pCardsArr->count(); i++) {
        pInf = pCardsArr->getObjectAtIndex(i);
        
        if (strncmp(m_ucid, pInf->ucid, 99) == 0) {
            break;
        }
    }
    
    return pInf;
}

int PackCard::getCategory()
{
    return  m_category;
}


int PackCard::getCountryPoerperty()
{
    return m_countryperperty;
}

//******************************************************************************
// show
//******************************************************************************
void PackCard::show(unsigned int unSortType)
{
    char buf[100];
    
    m_bShow = true;
    
    if(m_spHead == NULL){
        m_spHead = CGameData::Inst()->getHeadSprite(m_iCid);
        
        if(m_spHead){
            addChild(m_spHead);
            m_spHead->setAnchorPoint(CCPointMake(0, 1));
            
            m_size = m_spHead->getContentSize();

            //究极金边
            if (m_bIsEluTgt) {
                CCSprite* sp = CCSprite::spriteWithSpriteFrameName("GoldFrm.png");
                if (sp) {
                    m_spHead->addChild(sp);
                    sp->setAnchorPoint(CCPointMake(0, 1));
                    sp->setPosition(ccp(4, m_size.height-4));
                }
            }
            else if(m_bIsWkTgt){
                CCSprite* sp = CCSprite::spriteWithSpriteFrameName("wakeFrame.png");
                if (sp) {
                    m_spHead->addChild(sp);
                    sp->setAnchorPoint(CCPointMake(0, 1));
                    sp->setPosition(ccp(4, m_size.height-4));
                }
            }
            
            //星级背景
            if(m_star>=3){
                if (m_star == 3) {
                    snprintf(buf, 99, "fr_yellow.png");
                }
                else if (m_star == 4) {
                    snprintf(buf, 99, "fr_yellow2.png");
                }
                else {
                    snprintf(buf, 99, "fr_yellow3.png");
                }
                CCSprite *starCardBack = CCSprite::spriteWithSpriteFrameName(buf);
                if(starCardBack)
                {
                    m_spHead->addChild(starCardBack,-9);
                    starCardBack->setAnchorPoint(CCPointZero);
                    starCardBack->setPosition(CCPoint(6, 6));
                }
            }
            else{
                CCSprite* sp = CCSprite::spriteWithSpriteFrameName("fr_black.png");
                if(sp){
                    m_spHead->addChild(sp, -10);
                    sp->setAnchorPoint(CCPointZero);
                }
            }
            
            //灰掉
            if(m_bGray || m_bJustGray){
                if(m_spGray == NULL){
                    m_spGray = CCSprite::spriteWithSpriteFrameName("fr_black.png");
                    if(m_spGray != NULL){
                        m_spHead->addChild(m_spGray);
                        m_spGray->setAnchorPoint(CCPointMake(0, 0));
                        m_spGray->setPosition(CCPointMake(0, -1));
                    }
                }
            }
            
            //选择
            if(m_bSel){
                if(m_spGray == NULL){
                    m_spGray = CCSprite::spriteWithSpriteFrameName("fr_black.png");
                    if(m_spGray != NULL){
                        m_spHead->addChild(m_spGray);
                        m_spGray->setAnchorPoint(CCPointMake(0, 0));
                        m_spGray->setPosition(CCPointMake(0, -1));
                    }
                }
                
                if(m_spSel == NULL){
                    m_spSel = CCSprite::spriteWithSpriteFrameName("fr_selected.png");
                    if(m_spSel != NULL){
                        m_spHead->addChild(m_spSel);
                        m_spSel->setAnchorPoint(CCPointMake(1, 1));
                        m_spSel->setPosition(CCPointMake(m_size.width, m_size.height));
                    }
                }
            }
            
            if (m_iIndex < 0) {
                if(m_spIndex){
                    m_spHead->removeChild(m_spIndex, true);
                    m_spIndex = NULL;
                }
            } else if (index == 0) {
                if(m_spIndex){
                    m_spHead->removeChild(m_spIndex, true);
                    m_spIndex = NULL;
                }
                m_spIndex = OcProxy::Inst()->showTipsAndNum(0);
                if(m_spIndex != NULL){
                    m_spHead->addChild(m_spIndex);
                    m_spIndex->setPosition(CCPointMake(55, 55));
                }
            } else {
                if(m_spIndex){
                    m_spHead->removeChild(m_spIndex, true);
                    m_spIndex = NULL;
                }
                m_spIndex = OcProxy::Inst()->showTipsAndNum(m_iIndex);
                if(m_spIndex != NULL){
                    m_spHead->addChild(m_spIndex);
                    m_spIndex->setPosition(CCPointMake(55, 55));
                }
            }
            
//            //使用中
//            if((m_bUsed && !m_bInCurTeam) || m_bInCurTeam)
//            {
//                snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("package_using"));
//                TextNode* lbText = TextNode::textWithString(buf, CCSizeMake(100, 20), CCTextAlignmentCenter, 20);
//                lbText->setPosition(CCPointMake(m_size.width * 0.5, 10));
//                lbText->setColor(ccORANGE);
//                m_spHead->addChild(lbText, 2,UI_TAB_HINT_TEXT0);
//            }
//            else
//            {
//                if(m_bCostEnough)
//                {
//                    hintText(unSortType);
//                }
//                else
//                {
//                    CCSprite* spCost = CCSprite::spriteWithSpriteFrameName("notenough.png");
//                    if(spCost)
//                    {
//                        spCost->setPosition(CCPointMake(m_size.width * 0.5, 10));
//                        m_spHead->addChild(spCost, 1,kPackCardTxtTag1);
//                    }
//                }
//            }
            
//            if(m_bCostEnough || beUseNow())
//            {
//                hintText(unSortType);
//            }
//            else
//            {
//                CCSprite* spCost = CCSprite::spriteWithSpriteFrameName("notenough.png");
//                if(spCost)
//                {
//                    spCost->setPosition(CCPointMake(m_size.width * 0.5, 10));
//                    m_spHead->addChild(spCost, 1,kPackCardTxtTag1);
//                }
//            }
            hintText(unSortType);
        }
        
        
        //lock
        if(m_bLock)
            lockCard();
        if (m_awakeIndex > 0) {
            awakeIndexShow();
        }
    }
}

//******************************************************************************
// hide
//******************************************************************************
void PackCard::hide()
{
    m_bShow = false;
    m_unSortType = -1;
    
    if(m_spHead){
        if(m_spGray){
            m_spHead->removeChild(m_spGray, true);
            m_spGray = NULL;
        }
        
        if(m_spSel){
            m_spHead->removeChild(m_spSel, true);
            m_spSel = NULL;
        }
        
        if(m_spIndex){
            m_spHead->removeChild(m_spIndex, true);
            m_spIndex = NULL;
        }
        
        removeChild(m_spHead, true);
        m_spHead = NULL;
    }

    if(m_spLock){
        removeChild(m_spLock, true);
        m_spLock = NULL;
    }
}

//******************************************************************************
// getCardId
//******************************************************************************
short PackCard::getCardId()
{
    return m_iCid;
}

//******************************************************************************
// onExit
//******************************************************************************
void PackCard::onExit()
{
    CCLayer::onExit();
}

//******************************************************************************
// setSortType
//******************************************************************************

////******************************************************************************
//// updateText
////******************************************************************************
bool PackCard::updateText(int nIndex)
{
    if (!m_spHead || !m_bShow)
    {
        return false;
    }
    
    std::vector<CCNode*> vrNode;
    for (int i = 0; i < UI_TAB_HINT_COUNT; ++i)
    {
        if (CCNode *pNode = m_spHead->getChildByTag(UI_TAB_HINT_TEXT0+i))
        {
            vrNode.push_back(pNode);
        }
    }

    int nSize = vrNode.size();
    if (1>nSize)return false;
    int nNowIndex = nIndex%nSize;
    for (int i = 0; i < nSize; ++i)
    {
        vrNode[i]->setIsVisible(i==nNowIndex);
    }
    
    if (1==nSize)
    {
        vrNode[0]->setIsVisible(true);
    }
    
    return true;
}

void PackCard::hintText(int nSortType /*=-1*/)
{
    if (!m_spHead || !m_bShow || -1==nSortType || m_unSortType==nSortType)return;
    m_unSortType = nSortType;
    CCLabelAtlas *pLabelAtlas1 = NULL;
    CCLabelAtlas *pLabelAtlas2 = NULL;
    CCLabelAtlas *pLabelAtlas3 = NULL;
    char chBuf[128];
    
    if (!m_spHead->getChildByTag(UI_TAB_HINT_TEXT1))
    {
        //使用中
        if(beUseNow())
        {
            snprintf(chBuf, 99, "%s", CGameData::Inst()->getLanguageValue("package_using"));
            TextNode* lbText = TextNode::textWithString(chBuf, CCSizeMake(100, 20), CCTextAlignmentCenter, 20);
            lbText->setPosition(CCPointMake(m_size.width * 0.5, 10));
            lbText->setColor(ccORANGE);
            m_spHead->addChild(lbText, 2,UI_TAB_HINT_TEXT1);
        }
        //统御力不足
        else if (!beCostEnough())
        {
            snprintf(chBuf, 99, "%s", CGameData::Inst()->getLanguageValue("cost_not_enough"));
            TextNode* lbText = TextNode::textWithString(chBuf, CCSizeMake(100, 20), CCTextAlignmentCenter, 20);
            lbText->setPosition(CCPointMake(m_size.width * 0.5, 10));
            lbText->setColor(ccORANGE);
            m_spHead->addChild(lbText, 2,UI_TAB_HINT_TEXT1);
        }
    }
    
    //lv
    if (!m_spHead->getChildByTag(UI_TAB_HINT_TEXT1))
    {
        if (CCSprite* spTmp = CCSprite::spriteWithFile("lvnum.png"))
        {
            if(m_lv == m_lvMax){
                snprintf(chBuf, sizeof(chBuf), "()*+");
            }
            else{
                snprintf(chBuf, sizeof(chBuf), ",-%d", m_lv);
            }
            CCSize szSize = spTmp->getContentSize();
            pLabelAtlas1 = CCLabelAtlas::labelWithString(chBuf, "lvnum.png", szSize.width / 18, szSize.height, '(');
            pLabelAtlas1->setAnchorPoint(CCPointMake(0.5, 0));
            pLabelAtlas1->setPosition(CCPointMake(m_size.width * 0.5, 0));
            m_spHead->addChild(pLabelAtlas1, 1,UI_TAB_HINT_TEXT1);
        }
    }

    //+值
    if (!m_spHead->getChildByTag(UI_TAB_HINT_TEXT2))
    {
        if (0<m_plus)
        {
            if (CCSprite* spTmp = CCSprite::spriteWithFile("plusnum.png"))
            {
                snprintf(chBuf, sizeof(chBuf), "/%d", m_plus);
                CCSize sz = spTmp->getContentSize();
                pLabelAtlas2 = CCLabelAtlas::labelWithString(chBuf, "plusnum.png", sz.width / 11, sz.height, '/');
                pLabelAtlas2->setAnchorPoint(CCPointMake(0.5, 0));
                pLabelAtlas2->setPosition(CCPointMake(m_size.width * 0.5, 0));
                m_spHead->addChild(pLabelAtlas2, 1,UI_TAB_HINT_TEXT2);
            }
        }
    }

    m_spHead->removeChildByTag(UI_TAB_HINT_TEXT3,true);
    m_spHead->removeChildByTag(UI_TAB_HINT_TEXT4,true);
    m_spHead->removeChildByTag(UI_TAB_HINT_TEXT5, true);
    enCardRangeType emRangeType = (enCardRangeType)nSortType;
    if (enCardRangeTypeProp==emRangeType)           // 默认属性
    {
    }
    else if (enCardRangeTypeAtta==emRangeType)      //按照攻击力排列
    {
        if (CCSprite* spTmp = CCSprite::spriteWithFile("rewardnum.png"))
        {
            snprintf(chBuf, sizeof(chBuf), "%d", m_attack);
            CCSize sz = spTmp->getContentSize();
            pLabelAtlas3 = CCLabelAtlas::labelWithString(chBuf, "rewardnum.png", sz.width / 11, sz.height, '/');
            pLabelAtlas3->setAnchorPoint(CCPointMake(0.5, 0));
            pLabelAtlas3->setPosition(CCPointMake(m_size.width * 0.5, 0));
            m_spHead->addChild(pLabelAtlas3, 1,UI_TAB_HINT_TEXT3);
        }
    }
    else if (enCardRangeTypeHp==emRangeType)        //按照血量排列
    {
        if (CCSprite* spTmp = CCSprite::spriteWithFile("rewardnum.png"))
        {
            snprintf(chBuf, sizeof(chBuf), "%d", m_hp);
            CCSize sz = spTmp->getContentSize();
            pLabelAtlas3 = CCLabelAtlas::labelWithString(chBuf, "rewardnum.png", sz.width / 11, sz.height, '/');
            pLabelAtlas3->setAnchorPoint(CCPointMake(0.5, 0));
            pLabelAtlas3->setPosition(CCPointMake(m_size.width * 0.5, 0));
            m_spHead->addChild(pLabelAtlas3, 1,UI_TAB_HINT_TEXT3);
        }
    }
    else if (enCardRangeTypeRec==emRangeType)       //按照恢复力
    {
        // if (CCSprite* spTmp = CCSprite::spriteWithFile("rewardnum.png"))
        {
            snprintf(chBuf, sizeof(chBuf), "%d", m_recover);
            TextNode *pText = TextNode::textWithString(chBuf, 21);
            pText->setAnchorPoint(ccp(0.5,0.5));
            pText->setColor(ccc3(0, 254, 137));
            pText->setShadowColor(ccBLACK);
            pText->setPosition(ccp(52, 10));
            m_spHead->addChild(pText, 1,UI_TAB_HINT_TEXT3);
            
            
            //            snprintf(chBuf, sizeof(chBuf), "%d", m_recover);
            //            CCSize sz = spTmp->getContentSize();
            //            pLabelAtlas3 = CCLabelAtlas::labelWithString(chBuf, "rewardnum.png", sz.width / 11, sz.height, '/');
            //            pLabelAtlas3->setAnchorPoint(CCPointMake(0.5, 0));
            //            pLabelAtlas3->setPosition(CCPointMake(m_size.width * 0.5, 0));
            //            m_spHead->addChild(pLabelAtlas3, 1,UI_TAB_HINT_TEXT3);
            //            m_spHead->addChild(pText, 1,UI_TAB_HINT_TEXT3);
            
        }
    }
    else if (enCardRangeTypeGet==emRangeType)       //按照入手排列
    {
    }
    else if (enCardRangeTypeLock==emRangeType)      //按照加锁
    {
    }
    else if (enCardRangeTypePlus==emRangeType)      //+值排序
    {
    }
    else if (enCardRangeTypeCost==emRangeType)      //统御力
    {
        if (CCSprite* spTmp = CCSprite::spriteWithFile("rewardnum.png"))
        {
            snprintf(chBuf, sizeof(chBuf), "%d", m_iCost);
            CCSize sz = spTmp->getContentSize();
            pLabelAtlas3 = CCLabelAtlas::labelWithString(chBuf, "rewardnum.png", sz.width / 11, sz.height, '/');
            pLabelAtlas3->setAnchorPoint(CCPointMake(0.5, 0));
            pLabelAtlas3->setPosition(CCPointMake(m_size.width * 0.5, 0));
            m_spHead->addChild(pLabelAtlas3, 1,UI_TAB_HINT_TEXT3);
        }
    }
    else if (enCardRangeTypeCategory==emRangeType)
    {
        if (!m_spHead->getChildByTag(UI_TAB_HINT_TEXT4))
        {
            std::string szCategoryFrameName;
            if (enGeneralTp_GuoShi==m_category) szCategoryFrameName = "type_1_guoshiwushuang.png";
            else if (enGeneralTp_GongShou==m_category) szCategoryFrameName = "type_2_gongshoujianbei.png";
            else if (enGeneralTp_ShengSheng==m_category) szCategoryFrameName = "type_3_shengshengbuxi.png";
            else if (enGeneralTp_XiaoYong==m_category) szCategoryFrameName = "type_4_xiaoyongshanzhan.png";
            else if (enGeneralTp_JinShi==m_category) szCategoryFrameName = "type_5_jinshizhijian.png";
            else if (enGeneralTp_God==m_category) szCategoryFrameName = "type_6_shenjiang.png";
            else if (enGeneralTp_ZhuanSheng==m_category) szCategoryFrameName = "type_7_zhuanshengdaoju.png";
            else if (enGeneralTp_QiangHua==m_category) szCategoryFrameName = "type_8_qianghuahecheng.png";
            else if (enGeneralTp_mojiang==m_category) szCategoryFrameName = "type_9_mojiang.png";
            if (CCSprite *pSprite = CCSprite::spriteWithSpriteFrameName(szCategoryFrameName.c_str()))
            {
               
                pSprite->setAnchorPoint(CCPointMake(0.5, 0));
                pSprite->setPosition(CCPointMake(m_size.width * 0.5, 2));
                m_spHead->addChild(pSprite,0,UI_TAB_HINT_TEXT4);
            }
        }
    }
    else if (enCardRangeTypeCountryPorterty == emRangeType)
    {
        if (!m_spHead->getChildByTag(UI_TAB_HINT_TEXT5))
        {
            std::string szCategoryFrameName;
            if (enCountryTp_Null==m_countryperperty) szCategoryFrameName = "wu.png";
            else if (enCountryTp_dahan==m_countryperperty) szCategoryFrameName = "hanchao.png";
            else if (enCountryTp_weiguo==m_countryperperty) szCategoryFrameName = "weiguo.png";
            else if (enCountryTp_shuguo==m_countryperperty) szCategoryFrameName = "shuguo.png";
            else if (enCountryTp_wuguo==m_countryperperty) szCategoryFrameName = "wuguo.png";
            else if (enCountryTp_qunying==m_countryperperty) szCategoryFrameName = "qunying.png";
            else if (enCountryTp_jindai==m_countryperperty) szCategoryFrameName = "jindai.png";
            if (CCSprite *pSprite = CCSprite::spriteWithFile(szCategoryFrameName.c_str()))
            {
                pSprite->setScale(0.7);
                pSprite->setAnchorPoint(CCPointMake(0.5, 0));
                pSprite->setPosition(CCPointMake(m_size.width * 0.5, 2));
                m_spHead->addChild(pSprite,0,UI_TAB_HINT_TEXT5);
            }
        }

    }
    updateText(Package::getTickCount()+1);
}


bool PackCard::beUseNow()
{
    return m_bInCurTeam || (m_bUsed && !m_bInCurTeam);
}


void PackCard::setNumber(int index, bool isGray){
    m_iIndex = index;
    setGray(isGray);
    if (index < 0) {
        if(m_spIndex && m_spHead){
            m_spHead->removeChild(m_spIndex, true);
            m_spIndex = NULL;
        }
    } else if (index == 0) {
        if(m_spIndex && m_spHead){
            m_spHead->removeChild(m_spIndex, true);
            m_spIndex = NULL;
        }
        m_spIndex = OcProxy::Inst()->showTipsAndNum(0);
        if(m_spIndex != NULL && m_spHead){
            m_spHead->addChild(m_spIndex);
            m_spIndex->setPosition(CCPointMake(55, 55));
        }
    } else {
        if(m_spIndex && m_spHead){
            m_spHead->removeChild(m_spIndex, true);
            m_spIndex = NULL;
        }
        m_spIndex = OcProxy::Inst()->showTipsAndNum(index);
        if(m_spIndex != NULL && m_spHead){
            m_spHead->addChild(m_spIndex);
            m_spIndex->setPosition(CCPointMake(55, 55));
        }
    }
}


