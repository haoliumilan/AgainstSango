//
//  CPlayerBlood.h
//  CardDemo
//
//  Created by Hao Zhang on 12-5-3.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_CPlayerBlood_h
#define CardDemo_CPlayerBlood_h

#include "cocos2d.h"
#include "CommonDef.h"


using namespace cocos2d;

class CPlayerBlood : public CCNode
{
    CCSprite*   m_spBloodBase;
    CCSprite*   m_spBlood;
    
    CCSprite*   m_spAnim;
    
    CCLabelAtlas*   m_lbHp;
    CCLabelAtlas*   m_lbHurt;
    
    

    int             m_iStartNum;
    float           m_fElapse;
    float           m_fNumChgDur;
    
    int         m_iCurBlood;
    int         m_iBloodBase;
public:
    CPlayerBlood(int iBloodBase, int iBloodCur);
    virtual ~CPlayerBlood();
    
    void update(ccTime dt);
    
    void updateState();

    virtual void onExit();
    
    void setBlood(int iBlood, ccTime dt,bool updateByBaseHp);
    void setBaseBlood(int iBloodBase);
    int getBaseBlood(){return m_iBloodBase;};
    
    void setPosition(const CCPoint& newPosition);
    
    const CCPoint& getPosition();
    
    void canFei(bool bCanFei, int iBaseBlood);
    void setCanFeiColor(bool canfei);
    
   
};

#endif
