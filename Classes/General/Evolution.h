//
//  Evolution.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-1.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_Evolution_h
#define AgainstWar_Evolution_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

class EvoluTypeRet : public CCObject
{
public:
    int iType;
};



class CUserCardInfo;
class CShowCardInf;
class Dialog2;
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class Evolution : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO    m_pfnSelector;

    CCSprite*       m_spOk;
    bool            m_bReady;
    bool            m_bFoodInTeam;  // 进化素材是否在编队中
    bool            m_bFoodLocked;  // 进化素材是否被加锁
    CShowCardInf*   m_showCard;
    int             m_plus;         // 记录转生目标的plus
    
    bool            m_bEluEvo;      // 究极进化标记
    bool            m_bWake;        // 觉醒标记
    
    int             m_type;         //0 转生 1 觉醒 2超转生(究极进化）
    
public:
    Evolution(CCObject* target, SEL_CallFuncO selector, int type); //type 0 转生 1 觉醒
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

    virtual void onExit();
    
private:
    void initBaseCard(CUserCardInfo*);
    void initFoodCard(CUserCardInfo*);
    void initTargetCard(CUserCardInfo*);
    //长按显示详细信息
    void waitShowInfo();
    CCPoint beganPoint;
    bool isLongPress;
    bool m_bTouchMvFlg;
    Dialog2 *m_dlgAlertLock;
    
    void showAlertLock();
    void cbAlertLock(CCObject* pObj);
    
    void showSevoRule();
    void cbSevoRule(CCObject *pSender);
    
    void showWakeRule();
    void cbWakeRule(CCObject *pSender);
};

#endif
