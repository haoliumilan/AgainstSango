//
//  BonusList.h
//  test
//
//  Created by mac on 13-6-5.
//
//

#ifndef __test__BonusList__
#define __test__BonusList__

#include "cocos2d.h"

using namespace cocos2d;

class BonusItem;
class JiFengRecordInf;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class BonusList : public CCLayer
{
    int         m_iItemCnt;
    CCPoint     m_ptLeftTop;
    float       m_fUpLimitY;
    float       m_fHeight;
    
    CCPoint     last_moveTouch;
    CCPoint     m_ptStartNode;
    CCPoint     m_ptStartGl;
    float       m_lastYm;
    
    int         m_iSortType;
    
    CCMutableArray<JiFengRecordInf* > * m_pJiFengRecordArr;
    
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;

public:
    BonusList(CCObject* target, SEL_CallFuncO selector);
    
    void setPosition(const CCPoint& pos);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    virtual void visit();
    
    void toggleSort();

private:
    void inertiaMove(float dt);
    
    void moveY(float dt);
    
    void cbBonusItem(CCObject* pObj);

};

#endif /* defined(__test__BonusList__) */
