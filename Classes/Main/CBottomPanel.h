//
//  CBottomPanel.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-17.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CBottomPanel_h
#define AgainstWar_CBottomPanel_h

#include "cocos2d.h"
#include "CommonDef.h"


using namespace cocos2d;

class TextNode;

typedef enum 
{
    id_first = 0,
    id_generals,
    id_gacha,
    id_shop,
    id_social,
    id_country,
}ButtonType;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CBottomPanelRet : public CCObject
{
public:
    int     iBtnSel;
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CBottomPanel : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    CBottomPanelRet m_ret;

    CCSprite        *numberTips;
    CCSprite        *gachaTips;
    CCSprite        *mailTips;
    CCSprite        *cntryTips;
        
    bool            m_bEnable;
    bool            m_bMvFlg;
    

    
    
public:
    CBottomPanel(CCObject* target, SEL_CallFuncO selector);

    void setEnable(bool bEnable);
    bool getIsEnable(){return m_bEnable;}
    
    void showTips();
    void upDateIcon();
    
    void showCntryTips(bool clearStatus);
    
    void clearTips();
        
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
private:
    int chkTouchedBtn(CCPoint& pt);

};

#endif
