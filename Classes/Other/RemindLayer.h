//
//  RemindLayer.h
//  AgainstWar
//
//  Created by hejian on 13-10-15.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__RemindLayer__
#define __AgainstWar__RemindLayer__

#include "cocos2d.h"
#include "CListLayer.h"
using namespace cocos2d;

class CommDlg;

enum EN_REMIND_TYPE
{
    EN_REMIND_ACTIVITY = 0,
    EN_REMIND_TWOUP,
    EN_REMIND_CNTRYCD,
    EN_REMIND_POWER,
    EN_RWD_MAX,
};


class RemindLayer : public CCLayer,CCTextFieldDelegate {
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    CListLayer *    m_remindList;
    CommDlg*        m_commDlg;
    
    CCTextFieldTTF*    powerNumber;
    
    
    bool            moveUp;
    
    // 纪录 修改成功与否  以便维护显示设置的状态
    int     changeNumber;  // 0,1,2,3   指示对应的提醒的编号
    int     changeSum;     // 0,1   false,true    ／     0,1,2....n   体力值
    
    void cbList(CCObject* pObj);
    void insertItemByIdx(CCObject* pObj);
    void showRemindList();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);

    void MoveUpKeyboard();
    
    CCLayer* createRLayer(const char* tStr,int remindType);
    
private:
public:
    RemindLayer(CCObject* target, SEL_CallFuncO selector);
    ~RemindLayer();
    void update(ccTime ct);
    void cbCommDlg(CCObject* );
    void switchOnOff(CCObject *pSender);
    void refreshRemind();
    void layerMoveUpOrDown(bool up);
};


#endif /* defined(__AgainstWar__RemindLayer__) */
