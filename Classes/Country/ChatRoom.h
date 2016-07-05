//
//  ChatRoom.h
//  AgainstWar
//
//  Created by liang jun on 13-4-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_ChatRoom_h
#define AgainstWar_ChatRoom_h

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGameData.h"

using namespace cocos2d;

class CommDlg;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class ChatRoom : public CCLayer,CCTextFieldDelegate
{
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;
    
    CommDlg*        m_commDlg;
    
    int             m_iReqType;
    bool            m_bImeShowing;
    
    void update(ccTime dt);
    
    void cbCommDlg(CCObject* );
public:
    ChatRoom(CCObject* target, SEL_CallFunc selector);
    
    ~ChatRoom();

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);
	virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
    
    virtual void onEnter();
    virtual void onExit();
    
private:
    
    CCTextFieldTTF *m_textField;
    CCLayer*        m_textlayer;
    void addAllchild();
    void textFieldInit();
    void btnCallBack(CCObject* pSender);
    void GoManagePage(CCObject* pSender);
    
    void showIMEKeyboard();
    void hideIMEKeyboard();
    
    
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    virtual void afterUpdate();
    
};

#endif
