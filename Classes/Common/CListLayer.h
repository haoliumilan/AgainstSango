//
//  CListLayer.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CListLayer_h
#define AgainstWar_CListLayer_h

#include "cocos2d.h"
#include "CommonDef.h"

#define deltaTag  999

using namespace cocos2d;

class CScrollBar;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CListLayerRet : public CCObject
{
public:
    
    int     iBtnSel;
    CCPoint pos;
    CCPoint screenPos;
};

#define SCOROLLBARSTARTX 438

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CListLayer : public CCLayer
{
    int             m_iItemCnt;
    
    CScrollBar*     m_pScrollBar;
    
    bool            m_bTouchMvFlg;
    CCPoint         m_ptStartTouch;
    CCPoint         m_ptStartGl;
    
    float           m_fUpLimitY;
    
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    CListLayerRet   m_ret;
    
    SEL_CallFuncO   m_pfnSelectorBegin;
    SEL_CallFuncO   m_pfnSelectorEnd;
    SEL_CallFuncO   m_pfnSelectorRemove;
    
    SEL_CallFuncO   m_pfnReload;
    CListLayerRet   m_rdRet;
    
    float           m_lastYm;
    CCPoint         last_moveTouch;
    
    float           m_startHeight;  // list的起始点高度
    float           m_cellHeight;   // 每个cell的高
    float           m_cellShowCount;  // 显示出来的cell个数
    
    int topShow;
    int bottomShow;
    
    int startTopShow;
    int startBottomShow;

    void ChkObjectShow();
    
    void NtyGoTop(CCObject* pSender);
    void NtyGoBottom(CCObject* pSender);
    
    void chkTouchEnd(CCPoint point);

public:
    
    void addTBobserver();
    void removeTBobserver();
    
    void setPosition(const CCPoint& newPosition);
    void updatePosition();
    
    int getPreTopShow();
    int getPreBottomShow();
    void setPreTopShow(int topshow);
    void setPreBottomShow(int bottonshow);
    
    int getMenuBtnIndex(int nTab2);    //通过Tab2取当前CMenuBtn的index
    
    
    CListLayer(CCObject* target, SEL_CallFuncO selector,
               float cellShowCount = 5, float cellHeight = 116.0, float startHeight = 666.0);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    void push_back(CCNode* node);
    void push_back(CCNode* node, int z_order);
    void increaseUpLimitY(float f_offy);
    void clearAll();
    
    int  getItemCount() {return m_iItemCnt;};
    void setNumber(int index, int number, bool isGray);
    void setCellShowCount(int cellShowCount) {m_cellShowCount = cellShowCount;}
    
    void list_reset();
    
    virtual void onExit();
    
    void inertiaMove(float dt);
    void moveY(float dt);
    
    void setReloadFunc(SEL_CallFuncO selector);
    void reloadItemByIdx(CCNode* node, int idx);
    
    void setTouchBeginFunc(SEL_CallFuncO selector);
    void setTouchEndFunc(SEL_CallFuncO selector);
    void setTouchRemoveFunc(SEL_CallFuncO selector);
};

class CListLayerModel
{
private:
    int m_topShow;
    int m_bottomShow;
    float m_y;
    
public:
    CListLayerModel( int topShow,int bottomShow,float y);
    int getTopShow();
    void setTopShow(int topShow);
    int getBottonShow();
    void setBottonShow(int bottomShow);
    float getY();
    void  setY(float y);
    
};

#endif
