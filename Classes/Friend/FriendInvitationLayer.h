//
//  FriendInvitationLayer.h
//  AgainstWar
//
//  Created by Hao Liu on 12-7-10.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_FriendInvitationLayer_h
#define AgainstWar_FriendInvitationLayer_h

#include "cocos2d.h"

using namespace cocos2d;

class FriendInvitationLayer : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;
    
public:
    FriendInvitationLayer(CCObject* target, SEL_CallFunc selector);
    ~FriendInvitationLayer();
    virtual void onEnter();
    virtual void onExit();
    
    void showInvitationCode(const char* invitationCode);
    void inviteFriendFromWeibo();
    void enableShareBtn(bool isok);
    bool isEnableShareBtn;
};

#endif
