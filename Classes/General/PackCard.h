//
//  PackCard.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-25.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_PackCard_h
#define AgainstWar_PackCard_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

class CUserCardInfo;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class PackCard : public CCLayer
{    
    bool        m_bInCurTeam;
    bool        m_bGray;
    bool        m_bJustGray;
    bool        m_bSel;
    bool        m_bUsed;
    bool        m_bPowUp;
    bool        m_bCostEnough;
    bool        m_bLock;
    bool        m_bIsEluTgt;
    bool        m_bIsWkTgt;
    bool        m_bGrayByNiePanFood;    // 因为有相同的cid卡片被选为涅槃素材而变Gray
    
    bool        m_bShow;
    
    CCSize      m_size;
    
    CCSprite*   m_spGray;
    CCSprite*   m_spSel;
    CCSprite*   m_spLock;
    CCSprite*   m_spHead;
    CCSprite*   m_spIndex;
    
    char        m_ucid[100];
    int         m_eatExp;
    int         m_cType;
    int         m_iSellGold;
    int         m_attack;
    int         m_hp;
    int         m_recover;
    int         m_plus;
    int         m_star;
    int         m_category;
    int         m_iCid;
    int         m_lv;
    int         m_lvMax;
    int         m_iCost;
    int         m_iMaxAwake;
    int         m_countryperperty;
    
    unsigned long           m_updTime;
    unsigned int            m_unSortType;
    
    int         m_awakeIndex;   //觉醒个数
    
    int         m_iIndex; // -1:无效 0:主将  1:第一个副将  2:第二个副将。。。
    
public:
    PackCard(CUserCardInfo* , bool bUsing, bool bUsingGray,bool bchkTeam);
    
    int     getMaxAwake();
    bool    isGray();
    void    setGray(bool bGray);
    void    justSetGray();
    
    void    setIsGrayByNiePanFood(bool bGray);
    bool    getIsGrayByNiePanFood();
    
    void    setCostEnough(bool bEnough);
    
    void    setIsLockCard(bool bLock);
    void    lockCard();
    void    unlockCard();
    
    int     getIndex(){return m_iIndex;};
    bool    isUsed();
    
    void    setPowUp(bool bPowUp);
    bool    isPowUp();
    
    void    setSel(bool bSel);
    bool    isSelected();
    
    int     getEatExp();
    char*   getUcid();
    int     getcType();
    int     getSellGold();
    int     getPlus();
    int     getAttack();
    int     getHp();
    int     getRecover();
    int     getStar();
    int     getCategory();
    int     getCost();
    int     getCountryPoerperty();
    
    unsigned long getUpdTime();
    CUserCardInfo *getUserCardInfo();
    virtual void onExit();
    short   getCardId();
    
    void    show(unsigned int unSortType = -1);
    void    hide();
    bool updateText(int nIndex);
    void hintText(int nSortType = -1);
    bool beUseNow();
    bool beCostEnough()const {return m_bCostEnough;}
    
    void    awakeIndexShow();
    void setNumber(int index, bool isGray);
};

#endif
