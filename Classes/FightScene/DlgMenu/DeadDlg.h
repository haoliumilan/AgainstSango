//
//  DeadDlg.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-16.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_DeadDlg_h
#define AgainstWar_DeadDlg_h
#include "cocos2d.h"

using namespace cocos2d;

enum {
    enDeadType_Null=0,
    enDeadType_normal,
    enDeadType_free,
    };

class TextNode;

class DeadDlgRet :public CCObject
{
  public:
    int iBtnsel;
};


class DeadDlg :public CCLayer{
    CCObject *m_listener;
    SEL_CallFuncO m_selector;
    DeadDlgRet m_ret;
    
    CCSprite *m_backGround;
    CCMenuItem *sureItem;
    CCMenuItem *cancelItem;
    void menuCallback(CCObject *pSender);
    TextNode *  infoLabel;
    TextNode *  myYuanbaoLabel;
public:
    DeadDlg(CCObject *listener,SEL_CallFuncO selector,int type);
};

#endif
