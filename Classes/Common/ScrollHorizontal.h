//
//  ScrollHorizontal.h
//  AgainstWar
//
//  Created by caojiye on 13-8-8.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__ScrollHorizontal__
#define __AgainstWar__ScrollHorizontal__

#include "cocos2d.h"

using namespace cocos2d;



//////////////////////////////////////////////////////
class ScrollHorizontalRet : public CCObject
{
public:
    int node_tag;
    
};
//////////////////////////////////////////////////////
class ScrollHorizontal : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    int             m_width;
    int             m_height;
    bool            m_has_selected_state;
    float           m_positionX;
    float           m_position_gap;
    CCNode*         m_left_arrow;
    CCNode*         m_right_arrow;
    CCPoint         m_touch_begin;
    CCPoint         m_pos_begin;
    bool            m_move_enable;
    
    
    ScrollHorizontalRet m_ret;
    
public:
    static ScrollHorizontal* CreateHorizontalScroll(CCObject* target, SEL_CallFuncO selector, int _width, int _height);
    
    void SetDefaultSelected(int _tag);
    
    void PushBackItem(CCSprite* item);   // 增加一个item
    void PushBackItem(CCSprite* item, int z_order, CCSprite* selected_sprite);
    
private:
    
    ScrollHorizontal(CCObject* target, SEL_CallFuncO selector, int _width, int _height, int _gap, bool select);
    
    void Set2Arrow();
    void SetSelected(int tag);
    
    virtual void onEnter();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
};




#endif /* defined(__AgainstWar__ScrollHorizontal__) */
