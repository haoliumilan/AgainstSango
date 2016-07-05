//
//  MailView.h
//  AgainstWar
//
//  Created by caojiye on 13-1-10.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__MailView__
#define __AgainstWar__MailView__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class MailView :public CCObject
{
private:
    const char* ch_sender;
    const char* ch_mail_texture;
public:
    MailView();
    ~MailView();
    static MailView* Inst();
    void RemoveView();
    void ShowMailSender(const char* ch);
    void ShowMailTexture(const char* ch);
    void SetVisibleOrNot(bool is_visible);
   
};








#endif /* defined(__AgainstWar__MailView__) */
