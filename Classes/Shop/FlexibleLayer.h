//
//  FlexibleLayer.h
//  AgainstWar
//
//  Created by user on 13-6-4.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_FlexibleLayer_h
#define AgainstWar_FlexibleLayer_h

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"
#include "MainScene.h"
#include "TextNode.h"
#include "Dialog1.h"

USING_NS_CC;

enum EN_FLEX_REQ_TYPE
{
    enFlexReqType_Refresh = 0,
};


class FlexibleLayer : public CCLayer
{
    void showAllChildren();
    
    void showAllChildren2();
    
    void drawOrderSort();
    
    void showScrollBar();
    
    void huodongRefresh(CCObject* pSender);
    void checkHuodong(CCObject* pSender);
    
    CCLayer* m_MoveInfoLayer;
    
    float m_fTotalStrWidth;
    
    CCSprite* m_moveBar;
    CCSprite* bgBar;
    
    int   m_FlexType;
    
    int m_iReqType;
    void update(ccTime dt);
    CommDlg*   m_commDlg;
    void cbCommDlg(CCObject* pObj);
    
    void reFreshOrder();
    
    TextNode *itemLabel3;
    
    void reFreshSumCharge();
    
public:
    
    FlexibleLayer();
    ~FlexibleLayer();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
};


#endif
