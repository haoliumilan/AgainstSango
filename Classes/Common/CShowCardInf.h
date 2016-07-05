//
//  CShowCardInf.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CShowCardInf_h
#define AgainstWar_CShowCardInf_h

#include "cocos2d.h"
#include "CommonDef.h"
using namespace cocos2d;


class CUserCardInfo;
class CCardBaseInfo;
class TitleBar;

# define kLaiYuan    1001
# define kXiangQing  1002
# define kSource     1003

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CShowCardInf : public CCLayer
{
    bool       m_bisDropImage;
    bool       b_showAwakeInfo;
    bool       m_bCanBeTouched;
    bool       m_bShowBtmAfterHide;
    bool       m_bForbShowRangeBtn;
    TitleBar*  m_source;
    bool       m_bUseScrollView;
    
    CUserCardInfo* m_pUsrCardInfo;
    void drawAwakeSkillTip(CCPoint pos, int awakeSkillId, int idx);
    
public:
    
    CShowCardInf();
    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

//    void showScroll(CUserCardInfo*);
//    void showScroll(CCardBaseInfo*, int plHp = 0, int plAttack = 0, int plRecover = 0);
    void show(CUserCardInfo* , bool bSelf = true);
    void show(CCardBaseInfo* , int plHp = 0, int plAttack = 0, int plRecover = 0);
    void hide();
    
    void setUseScrollView(bool b);
    CCNode * getChildBody();
    CCNode * getChildInfoBox();
    void loadBody(unsigned short ushCID);
    
    void ccCanBeTouched();
    CCObject*       m_pListener;
    SEL_CallFunc   m_pfnSelector;
    void setOnExitCB(CCObject* listener,SEL_CallFunc   m_pSelector);
    void showTorchParticle();
    
    void setShowBtmAfterHide(bool bShow);
    void setForbShowRangeBtn(bool isSet);
    
    void setIsDropImage(bool bl);
private:
    void ShowGeneralSource();
};

#endif
