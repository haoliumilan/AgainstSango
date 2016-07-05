//
//  CNotice.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-15.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CNotice_h
#define AgainstWar_CNotice_h
#include "cocos2d.h"


using namespace cocos2d;

enum enNoticeType {
    enNoticeTypeSystem = 1,
    enNoticeTypeGift,
    enNoticeTypeLendInfo,
    enNoticeTypeInviteAward,
    enNoticeTypeClaim,
};

class TextNode;

class CNoticeRet :public CCObject{
    
    
public:
    int iBtnSel;
};


class CC_DLL CNotice :public CCLayer {
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    CNoticeRet *    m_ret;
    TextNode*       titleLabel;
    int             touchTime;
    enNoticeType   cureNoticeType;
    CCLayerColor *  coverLayer;
    CCMenuItem *sureItem;
    CCMenu*         m_fr_button;
    
    CCMenuItem*     checkItem;
    CCMenu*         checkMenu;
    TextNode*       isRead;
    
    CCSprite*       noticeBack;
public:
    virtual void enterWithType(enNoticeType type);
    virtual void exit();
    virtual void onEnter();
    virtual void onExit();
    CNotice(CCObject *listener,SEL_CallFuncO selector);
    void callBackMenu(CCObject *pSender);
    void changeNoticeWithType(enNoticeType type);
    void update(ccTime dt);
    
    void cbCheckMenu(CCObject *pSender);
};


#endif
