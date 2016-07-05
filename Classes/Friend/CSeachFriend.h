//
//  CSeachFriend.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-4.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CSeachFriend_h
#define AgainstWar_CSeachFriend_h

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"

using namespace cocos2d;

class TextNode;

class CSeachFriend : public CCLayer,CCTextFieldDelegate
{
    CCObject*       m_pListener;
    SEL_CallFunc   m_pfnSelector;

private:
    CCSprite *      m_background;
    CCMenuItem *    searchItem;
    TextNode *      myIdLabel;
    bool            isAttach;
  
public:
    CCTextFieldTTF *textField;
    CSeachFriend(CCObject* target, SEL_CallFunc selector);
    ~CSeachFriend();
    virtual void enter();
    virtual void exit();
    virtual void onExit();
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void removeDispacher();
    void menuCallback(CCObject* pSender);
    bool requestSearchFriend();
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
};
#endif
