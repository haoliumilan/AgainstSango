//
//  CSkillDlg.h
//  CardDemo
//
//  Created by Hao Zhang on 12-5-8.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_CSkillDlg_h
#define CardDemo_CSkillDlg_h

#include "cocos2d.h"

using namespace cocos2d;

class TextNode;

class CSkillDlgRet: public CCObject
{
public:
    bool bUse;
};


class CSkillDlg : public CCLayer
{
    bool        m_bShow;
    
    CCSprite*   m_spDlg;
    
    TextNode*   m_lbName;
    TextNode*   m_lbDsp;
    TextNode*   m_lbCD;
    TextNode*   m_lbUseCfm;
    
    CCMenuItem* m_miOk;
    CCMenuItem* m_miCancel;
    
    CSkillDlgRet    m_pSkillDlgRet;
    
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    CSkillDlg(CCObject* target, SEL_CallFuncO selector);
public:
    static CSkillDlg* initDlg(CCObject* target, SEL_CallFuncO selector){
        CSkillDlg* pDlg = new CSkillDlg(target, selector);
        
        pDlg->autorelease();
        
        return pDlg;
    }
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    void setSkillID(int iID);
    void setCD(int iCD);
    
    void show(bool bShow);
    
    virtual void onExit();
    
    void menuCallback(CCObject *pSender);
    
private:
    int containsTouchLocation(CCTouch *pTouch);
};

#endif
