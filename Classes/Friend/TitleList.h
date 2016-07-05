//
//  TitleList.h
//  test
//
//  Created by Zhang Hao on 13-5-20.
//
//

#ifndef __test__TitleList__
#define __test__TitleList__

#include "cocos2d.h"

USING_NS_CC;

class TitleItem;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class TitleList : public CCLayer
{
    bool    m_bMvFlg;
    CCRect  m_displayRct;
    
    int     m_iItmCnt;
    float   m_fCurOffsetY;
    
    float       m_fThisY;
    float       m_fLimitY;
    
    CCPoint     m_ptStartTouch;
    CCPoint     m_ptStartGl;
    float       m_lastYm;
    CCPoint     last_moveTouch;
public:
    TitleList(CCRect rct);
    
    //virtual void visit();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    void push_back(CCNode* pTtlItm);
    
    TitleItem* getEquipedItem();
    
    void equipById(string title_id);
    
    void clearAll();
    
private:
    void redraw();
    
    void inertiaMove(float dt);
    void moveY(float dt);
};

#endif /* defined(__test__TitleList__) */
