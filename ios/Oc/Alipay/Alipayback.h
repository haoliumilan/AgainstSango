#ifndef __Alipayback_SCENE_H__
#define __Alipayback_SCENE_H__


#include "cocos2d.h"

using namespace cocos2d;

class TextNode;

class Alipayback : public cocos2d::CCLayer
{
private:
    TextNode    *tipLabel;
    CCSprite    *sureItem;
public:
    
    virtual bool init();
    
    static cocos2d::CCScene* scene();
    
	LAYER_NODE_FUNC(Alipayback);
    
    void alipaySuccess(int i);
    
    void menuCloseCallback(CCObject* pSender);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
};




#endif
