//
//  CountryCreate.h
//  AgainstWar
//
//  Created by Liu Hao on 12-12-26.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__CountryCreate__
#define __AgainstWar__CountryCreate__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

#define   NAME_FIELD_GAP  95
#define   MANI_FIELD_GAP  170


enum enKeyboardStatusDef {
    enKeyboard_Hide = 0,
    enKeyboard_NameShow,
    enKeyboard_ManifestoShow,
};

class Dialog2;
class Dialog0;

class CountryCreate : public CCLayer, public CCTextFieldDelegate
{
public:
    CountryCreate(CCObject* target, SEL_CallFunc selector);
    ~CountryCreate();
    
    virtual void onEnter();
    virtual void onExit();
    
    void showCntryCreatDialog();
    bool creatCountry();
private:
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;

    CCTextFieldTTF* m_tfCntryName;
    CCTextFieldTTF* m_tfCntryManifesto;
    Dialog2*        m_dlCoinAlert;          // 元宝不足的警告
    Dialog0*        m_FbdWords;             // 屏蔽字 警告
    
    CCMenuItem* m_mItemOK ;   //为了方便启用disable 状态，放到头文件
    
    int             m_keyboardStatus;      // 记录当前键盘的状态
    
    bool          m_bShowName;
    bool          m_bShowMani;
    
private:
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    void menuCallBack(CCObject* pObj);
    
    void showKeyboard(CCTextFieldTTF * &sender);
    void hidekeyboard();
    void showCoinAlert();
    void cbCoinAlert();
    void cbFbdWords();
    bool CheckAllNumber(string str0);
    void createCountryError();
    void createCountryNumber();
};


#endif /* defined(__AgainstWar__CountryCreate__) */
