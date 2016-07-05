//
//  ScrollHorizontal.cpp
//  AgainstWar
//
//  Created by caojiye on 13-8-8.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//
//////////////////////////////////////////////////////
#include "ScrollHorizontal.h"
#include "CommonDef.h"

#include "MainScene.h"
//////////////////////////////////////////////////////
#define kSelectedTag   456
#define kArrowTag      12

//////////////////////////////////////////////////////
ScrollHorizontal* ScrollHorizontal::CreateHorizontalScroll(CCObject* target, SEL_CallFuncO selector, int _width, int _height)
{
    ScrollHorizontal* rt = new ScrollHorizontal(target , selector, _width, _height, 9, true);
    rt -> autorelease();
    return rt;
}
ScrollHorizontal::ScrollHorizontal(CCObject* target, SEL_CallFuncO selector, int _width, int _height, int _gap, bool select)
{
    m_pListener = target;
    m_pfnSelector = selector;
    m_width = _width;
    m_height = _height;
    m_position_gap = _gap;
    m_positionX = 0;
    m_move_enable = false;
    m_has_selected_state = select;
    m_left_arrow = NULL;
    m_right_arrow = NULL;
    
}

void ScrollHorizontal::SetDefaultSelected(int _tag)
{
    SetSelected(_tag);
}

void ScrollHorizontal::PushBackItem(CCSprite* item)
{
    CCAssert(item->getTag()>0, "please set the tag greater than 0");
    CCSprite* selected_sprite = CCSprite::spriteWithFile("rwdbtnhadxuan.png");
    PushBackItem(item, 0, selected_sprite);
}

void ScrollHorizontal::PushBackItem(CCSprite* item, int z_order, CCSprite* selected_sprite)
{
    if (item == NULL)
        return;
    
    m_positionX += m_position_gap;
    addChild(item, z_order);
    item->setAnchorPoint(CCPointZero);
    item->setPosition(CCPointMake(m_positionX, m_height/2-item->getContentSize().height/2));
    
    if (m_has_selected_state)
    {
        item->addChild(selected_sprite);
        selected_sprite->setTag(kSelectedTag);
        selected_sprite->setAnchorPoint(CCPointZero);
        selected_sprite->setScale(0);
    }
    
    m_positionX += item->getContentSize().width;
    m_width = m_positionX;
    
    if (m_positionX - m_position_gap > 640)
    {
        // 2个箭头要显示粗来
        m_move_enable = true;
        Set2Arrow();
    }
}

void ScrollHorizontal::Set2Arrow()
{
    if (!m_move_enable)
        return;
    
    if (m_left_arrow == NULL)
    {
        m_left_arrow = CCNode::node();
        CCSprite* sp = CCSprite::spriteWithFile("rwdhuadongarrow.png");
        sp->setFlipX(true);
        sp->setPosition(CCPointMake(0, 0));
        m_left_arrow->addChild(sp, 1, kArrowTag);
        addChild(m_left_arrow, 15);
        m_left_arrow->setPosition(CCPointMake(0, m_height/2));
        sp->setPosition(CCPointMake(0, 0));
        
        CCActionInterval* move1 = CCMoveTo::actionWithDuration(0, CCPointMake(50, 0));
        CCActionInterval* move2 = CCMoveTo::actionWithDuration(1, CCPointMake(sp->getContentSize().width/2, 0));
        CCAction* action = CCRepeatForever::actionWithAction((CCActionInterval*)CCSequence::actions(move1, move2, NULL));
        sp->runAction(action);

    }
    
    if (m_right_arrow == NULL)
    {
        m_right_arrow = CCNode::node();
        CCSprite* sp = CCSprite::spriteWithFile("rwdhuadongarrow.png");
        sp->setPosition(CCPointMake(0, 0));
        m_right_arrow->addChild(sp, 1, kArrowTag);
        addChild(m_right_arrow, 15);
        m_right_arrow->setPosition(CCPointMake(0, m_height/2));
        
        CCActionInterval* move1 = CCMoveTo::actionWithDuration(0, CCPointMake(-50, 0));
        CCActionInterval* move2 = CCMoveTo::actionWithDuration(1, CCPointMake(-sp->getContentSize().width/2, 0));
        CCAction* action = CCRepeatForever::actionWithAction((CCActionInterval*)CCSequence::actions(move1, move2, NULL));
        sp->runAction(action);
    }
    
    // 左边的
    if (fabs(getPositionX()) < 5)
    {
        m_left_arrow -> setPosition(CCPointMake(1000, 1000));
        m_left_arrow->getChildByTag(kArrowTag)->pauseSchedulerAndActions();
    }
    else
    {
        m_left_arrow -> setPosition(CCPointMake(-getPositionX(), m_height/2));
        m_left_arrow->getChildByTag(kArrowTag)->resumeSchedulerAndActions();
    }
    
    // 右边的
    if (fabs(640-getPositionX()-m_width) < 5)
    {
        m_right_arrow->setPosition(CCPointMake(1000, 1000));
        m_right_arrow->getChildByTag(kArrowTag)->pauseSchedulerAndActions();
    }
    else
    {
        m_right_arrow->setPosition(CCPointMake(-getPositionX()+640-m_right_arrow->getContentSize().width/2, m_height/2));
        m_right_arrow->getChildByTag(kArrowTag)->resumeSchedulerAndActions();
    }
    
}

// touch delegate
void ScrollHorizontal::onEnter()
{
    CCLayer::onEnter();
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -228, true);
}

void ScrollHorizontal::onExit()
{
    setIsTouchEnabled(false);
    CCLayer::onExit();
}

bool ScrollHorizontal::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    

    
    
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    touchLocation = convertToNodeSpace(touchLocation);
    
    bool rt = CCRect::CCRectContainsPoint(CCRectMake(0, 0, m_width, m_height), touchLocation);
    if (rt)
    {
        m_touch_begin = touchLocation;
        m_pos_begin = getPosition();
    }
    
    return rt;
}

void ScrollHorizontal::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    if (m_move_enable == false)
        return;
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    touchLocation = convertToNodeSpace(touchLocation);
    float delta_x = touchLocation.x - m_touch_begin.x;
    
    CCPoint pos = CCPointMake(m_pos_begin.x+delta_x, getPosition().y);
    if (pos.x > 0)
        pos.x = 0;
    if (pos.x + m_width <640)
        pos.x = 640-m_width;
    this->setPosition(pos);
    Set2Arrow();
}

void ScrollHorizontal::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    

    
    
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    touchLocation = convertToNodeSpace(touchLocation);
    
    float distance = ccpDistance(touchLocation, m_touch_begin);
    if (distance < kListBoxMinMovY)
    {
        CCArray* arr_children = getChildren();
        for (int i=0; i<arr_children->count(); i++)
        {
            CCNode* child = (CCNode*)arr_children->objectAtIndex(i);
            int tag = child->getTag();
            if (tag>0)
            {
                if (CCRect::CCRectContainsPoint(CCRectMake(child->getPosition().x, child->getPosition().y, child->getContentSize().width, child->getContentSize().height), touchLocation))
                {
                    m_ret.node_tag = tag;
                    
                    //引导
                    if(MainScene::Inst()->new_MyNew )
                    {
                        
                        
                        if(MainScene::Inst()->getChildByTag(99997) && tag == 1001)
                        {
                            
                            SetSelected(tag);
                        }
                    }
                    else
                    SetSelected(tag);
                    
                    (m_pListener->*m_pfnSelector)(&m_ret);
                    break;
                }
            }
        }
    }
}

void ScrollHorizontal::SetSelected(int _tag)
{
    if (m_has_selected_state)
    {
        CCArray* arr_children = getChildren();
        for (int i=0; i<arr_children->count(); i++)
        {
            CCNode* child = (CCNode*)arr_children->objectAtIndex(i);
            int tag = child->getTag();
            if (tag>0)
            {
                CCNode* selected_sprite = child->getChildByTag(kSelectedTag);
                if (selected_sprite)
                {
                    if (tag == _tag)
                    {
                        selected_sprite->setScale(1);
                    }
                    else
                    {
                        selected_sprite->setScale(0);
                    }
                }
            }
        }
    }
}







