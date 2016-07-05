//
//  dialog0.h
//  AgainstWar
//
//  Created by caojiye on 13-2-4.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__dialog0__
#define __AgainstWar__dialog0__

#include "cocos2d.h"
using namespace cocos2d;

class Dialog0 : public  CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;
    const char*     m_content;
public:
    Dialog0(CCObject* target, SEL_CallFunc selector, const char* dsp);
private:
    void SetBackground();
    void cbSure();
    
};





#endif /* defined(__AgainstWar__dialog0__) */
