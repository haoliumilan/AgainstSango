//
//  CMonsterBlood.h
//  CardDemo
//
//  Created by Hao Zhang on 12-5-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_CMonsterBlood_h
#define CardDemo_CMonsterBlood_h

#include "cocos2d.h"
#include "Enemy.h"

using namespace cocos2d;

class Enemy;

class CMonsterBlood : public CCNode
{
    CCSprite*   m_spBloodBase;
    CCSprite*   m_spElem;
    CCSprite*   m_spBlood;
    CCObject    *m_pEndBlood;
    SEL_CallFuncND fcEndBlood;
    int         m_nDelayMoveHP;
    float       m_fDelayMoveTime;
    
    
    int         m_iCurBlood;
    int         m_iBloodBase;
    
    float       m_fScaleMax;
public:
    CMonsterBlood(CCPoint& pos, CCSize szEnemy, int iElem);
    static bool beElem(int nElem);
    void endBloodMove();
    void loginBloodMove(CCObject *pObj, SEL_CallFuncND func);
    
    virtual void onExit();

    void setBlood(int iBlood, int iBloodBase, ccTime dt, int nDelayMoveHP = 0,float fDelayMoveTime = 0.0f);
    
    void setElem(int iElem);
    
    void fadeout(ccTime dt);
    
    void hide();
    
    void fadein(ccTime dt);
    
    float getLeftPosX();
    float getRightPosX();
    int getDelayMoveHP()const {return m_nDelayMoveHP;}
    float getDelayMoveTime()const {return m_fDelayMoveTime;}

};

#endif
