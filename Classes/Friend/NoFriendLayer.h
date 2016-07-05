//
//  NoFriendLayer.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-6-20.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_NoFriendLayer_h
#define AgainstWar_NoFriendLayer_h

#include "cocos2d.h"

using namespace cocos2d;

class TextNode;

class NoFriendLayerRet :public CCObject
{
  public:
    int iBSel;
};


class NoFriendLayer :public CCLayer {
    
    CCObject *m_listener;
    SEL_CallFuncO m_selector;
    NoFriendLayerRet m_ret;
    CCSprite *m_background;
    TextNode *infoLabel;
public:
    NoFriendLayer(CCObject *listener,SEL_CallFuncO selector);
    void menuCallback();
    void setInfoLabel(const char *labelStr);
};

#endif
