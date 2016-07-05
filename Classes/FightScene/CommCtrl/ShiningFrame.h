//
//  ShiningFrame.h
//  test
//
//  Created by Zhang Hao on 13-4-2.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __test__ShiningFrame__
#define __test__ShiningFrame__

#include "cocos2d.h"
#include <list>

using namespace cocos2d;

class TextNode;
class Card;

class ShiningFrame : public cocos2d::CCLayer
{
    int m_idx;
    int m_idx2;
    int m_iPtCnt;

    list<Card*> m_lstCard;
public:
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
    
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(ShiningFrame);
    
    void draw();
    
    void update(ccTime dt);
    
    void addCard(Card* pCard);
    void rmvCard(Card* pCard);
    void clrCardList();
    
private:
    void drawFrameBg(CCPoint& pt);
    void drawMovHead(CCPoint& pt);
    void drawMovTail(CCPoint& pt);
};

#endif /* defined(__test__ShiningFrame__) */
