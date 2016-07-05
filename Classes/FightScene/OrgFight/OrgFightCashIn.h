//
//  OrgFightCashIn.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-2-6.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__OrgFightCashIn__
#define __AgainstWar__OrgFightCashIn__

#include "cocos2d.h"

using namespace cocos2d;

class TextNode;
class ActNumTTF;


class OrgFightCashIn : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;
    
    TextNode*       m_lbFightName;
    CCSprite*       m_spLastAtk;
    TextNode*       m_lbMyAtk;
    TextNode*       m_lbGold;
    TextNode*       m_lbWealth;
    TextNode*       m_lbCntrbt;
    ActNumTTF*      m_numMyAtk;
    ActNumTTF*      m_numGold;
    ActNumTTF*      m_numWealth;
    ActNumTTF*      m_numCntrbt;
    
    CCSprite*       m_showDetailLayer;//同盟战场掉落的物品显示层
    
    bool            m_bTxtAnimOver;
    bool            m_bItmAnimOver;
    
public:
    OrgFightCashIn(CCObject* target, SEL_CallFunc selector);
    
    void show(const char* szFightName);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    virtual void ccTouchEnded(CCTouch* touch,CCEvent* event);
private:

    void onTimer(ccTime dt);
    
    void cbLbMyAtk();
    void cbLbWealth();
    void cbLbGold();
    void cbLbCntrbt();
    
    bool showItemCard();
    void cbShowItemCard(ccTime dt);
    
    void crtShowDetailLayer();
};

#endif /* defined(__AgainstWar__OrgFightCashIn__) */
