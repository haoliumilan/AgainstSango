//
//  TitleItem.h
//  test
//
//  Created by Zhang Hao on 13-5-20.
//
//

#ifndef __test__TitleItem__
#define __test__TitleItem__

#include "cocos2d.h"

USING_NS_CC;

class TextNode;
class TitleInf;

enum enTtlEquipSt{
    enTtlEquipStNull,
    enTtlNotEquiped,
    enTtlEquiped,
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class TitleItem : public CCLayer
{
    TextNode*       m_txtTitleName;
    enTtlEquipSt    m_enEquipSt;
    
    CCMenuItem*     m_mnUse;
    CCSprite*       m_spMnCvr;
    
    CCSprite*       m_spTtlNameBg;
    CCSprite*       m_spTtlDspBg1;
    CCSprite*       m_spTtlDspBg2;
    CCSprite*       m_spTtlDspBg3;
    CCSize          m_szTtlDspBg;
    CCLayer*        m_lyTxt;
    
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    string          m_strTitleId;
    
    CCPoint         m_TxtStartPos;
public:
    TitleItem(TitleInf* pTtlInf);

    //展开、关闭状态有变化返回true
    bool procTouch(CCPoint glPoint);
    
    void setEquipSt(enTtlEquipSt eqpst);
    enTtlEquipSt getEquipSt();
    
    void setUseCb(CCObject* target, SEL_CallFuncO selector);
    
    string getTitleId(){return m_strTitleId;};

private:
    void drawTitleDspTxt(TitleInf* pTtlInf);
    
    void updateMenuBtn();
    void cbUse(CCObject* sender);
};

#endif /* defined(__test__TitleItem__) */



