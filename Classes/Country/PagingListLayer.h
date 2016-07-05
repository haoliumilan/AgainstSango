//
//  PagingListLayer.h
//  AgainstWar
//
//  Created by Liu Hao on 12-12-29.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__PagingListLayer__
#define __AgainstWar__PagingListLayer__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class CListLayer;
class CScrollBar;

enum enMemberListType
{
    enMemberListType_normal = 0,    //  成员列表
    enMemberListType_assign,        //  战利品分配列表
    enMemberListType_reward,        //  悬赏参加成员列表
};

class PagingListRet : public CCObject
{
public:
    int iBtnSel;
    int pLocal;
};

class PagingListLayer : public CCLayer
{
public:
    PagingListLayer(CCObject* pListener, SEL_CallFuncO pfnSelector);
    ~PagingListLayer();
    
    virtual void onEnter();
    virtual void onExit();
    
    void showList(enMemberListType memListType,int positionY = 0,int mListSel = 0);
    void changeListRange();
private:
    void cbList(CCObject* pObj);
    void btnCallback(CCObject* pObj);
    void newBtn(CCMenuItem *&menuItem, const char *label, const CCPoint& position, int tag);
    
    void updatePaingList();
    void pagingToPrev();
    void pagingToNext();

    void showMemberList();
    void showAssignSupplyList();
    void showRewardMemberList();
    
    void showCountryDetailInfo(CCObject* pObj);
    
    void showMemberItem(int index);
    void showAssignItem(int index);
    void showAverageItem();
    void showContributionItem();
    void showWeekItem();
    
    
public:
    
    
private:
    enMemberListType    m_memListType;
    PagingListRet       m_ret;          // 选中的项序数
    CCObject*           m_pListener;
    SEL_CallFuncO       m_pfnSelector;

    int                 m_curPage;      // 当前页数，默认0
    int                 m_maxLines;     // 最大行数

    CListLayer*         m_list;
    CCMenu*             m_pagingMenu;   // 翻页按钮
    
    int                 positionY;      // 记录Y轴坐标
    
};


#endif /* defined(__AgainstWar__PagingListLayer__) */
