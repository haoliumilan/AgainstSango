//
//  OrgFightCfm.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-3-21.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__OrgFightCfm__
#define __AgainstWar__OrgFightCfm__

#include "cocos2d.h"

using namespace cocos2d;

class TextNode;
class Dialog2;
class Dialog1;
class CommDlg;
class OrgRankList;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class OrgFightCfmRet : public CCObject
{
public:
    bool    bOk;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class OrgFightCfm : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    OrgFightCfmRet  m_ret;
    
    float           m_fElapse;
    float           m_fCntDwnTtl;
    int             m_iCntDwn;
    
    TextNode*       m_txtCntDwn;
    
    Dialog2*        m_dlgTip;
    Dialog1*        m_dlgRecover;
    
    int             m_iCurReqType;
    CommDlg*        m_commDlg;
    
    CCSprite*       m_spBtnJiFeng;
    CCSprite*       m_spBtnLuanWu;
    CCSprite*       m_spBtnBg;
    
    OrgRankList*    m_rankLy;
    
    TextNode*       m_txtJiFengTime;
    
public:
    OrgFightCfm(CCObject* target, SEL_CallFuncO selector, string dg_id);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
private:
    void update(ccTime dt);
    
    void showRecoverAcPointTip();   // 提示回复行动点数需要的元宝
    void cbRecoverAcPointTip(CCObject* pObj);
    void showNoCoinForRecover();    // 提示回复行动点数元宝不够
    void cbNoCoinForRecover(CCObject* pObj);
    void showAcPointFullTip();      // 提示行动力点数已满
    void cbAcPointFullTip();
    void showAcPointNotEnoughTip();
    void cbAcPointNotEnoughTip(cocos2d::CCObject *pObj);
    
    void recoverActionPoint();
    
    void cbBack(CCObject* sender);
    void cbAcRcv(CCObject* sender);
    
    void cbCommDlg(CCObject* );
    
    void showJiFengLuanWuBtn(string dgType);
};

#endif /* defined(__AgainstWar__OrgFightCfm__) */
