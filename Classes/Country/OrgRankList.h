//
//  OrgRankList.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-3-20.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __test__OrgRankList__
#define __test__OrgRankList__

#include "cocos2d.h"

using namespace cocos2d;


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class OrgRankList : public CCLayer
{
public:
    string      m_dgtype;
    
    CCPoint     m_ptStart;
    float       m_fThisY;
    float       m_fLimitY;
    
    CCSprite*   m_spScrollBg;
    CCSprite*   m_spScrollBar;
    float       m_fBarPos0;
    float       m_fBarPos100;
    float       m_fBarBgPosY;
    
    CCPoint     last_moveTouch;
    CCPoint     m_ptStartNode;
    CCPoint     m_ptStartGl;
    float       m_lastYm;
    
public:
    OrgRankList(string dg_type);
    ~OrgRankList();

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    virtual void onEnter();
    virtual void visit();
    
    void setPosition(const CCPoint& pos);
    
    void showSpecialDgRank(string dgType);

    string getDgType();
private:
    void showCntryDgRank();
    
    void inertiaMove(float dt);
    
    void moveY(float dt);
};

#endif /* defined(__test__OrgRankLayer__) */
