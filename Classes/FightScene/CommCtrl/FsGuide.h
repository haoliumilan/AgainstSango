//
//  FsGuide.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-4-8.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__FsGuide__
#define __AgainstWar__FsGuide__

#include "cocos2d.h"

using namespace cocos2d;

class TextNode;

class FsGuide : public cocos2d::CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;
    
    TextNode*       m_txtGuide;
    
    CCMutableArray<CCString*> * m_pTipsArr;
    int             m_idx;
    
public:
	// implement the "static node()" method manually
	LAYER_NODE_FUNC(FsGuide);
    
    void setGuideString(CCMutableArray<CCString*> * pTipsArr,
                        CCObject* target, SEL_CallFunc selector);
    
    virtual void onEnter();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
};


#endif /* defined(__AgainstWar__FsGuide__) */
