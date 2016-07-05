//
//  CDKeyExchange.h
//  AgainstWar
//
//  Created by Liu Hao on 13-4-18.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__CDKeyExchange__
#define __AgainstWar__CDKeyExchange__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class CDKeyExchange : public CCLayer, CCTextFieldDelegate
{
public:
    CDKeyExchange(CCObject* target, SEL_MenuHandler selector);
    ~CDKeyExchange();
    
    virtual void onEnter();
    virtual void onExit();
    
    void showCDKeyExchangeDlg();
    void showExchangeSuccessDlg();
    
    bool exchangeCDKey();
private:
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    void showKeyboard();
    void hideKeyboard();
    
    const char* getGiftContent();
    
public:
    
private:
    CCTextFieldTTF *m_textField;
    bool            m_bAttach;
    CCMenuItem     *m_mItemExchange;
    
    CCObject*       m_pListener;
    SEL_MenuHandler    m_pfnSelector;

};

#endif /* defined(__AgainstWar__CDKeyExchange__) */
