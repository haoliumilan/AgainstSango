//
//  TipsAwakeSkill.h
//  AgainstWar
//
//  Created by user on 13-10-29.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__TipsAwakeSkill__
#define __AgainstWar__TipsAwakeSkill__

#include "cocos2d.h"
#include "TextNode.h"

USING_NS_CC;

class TipsAwakeSkill : public CCSprite
{
    void addTipString(char* icon, const char* strName, const char* Content, const char* idxTitle);
    
public:
    
    TipsAwakeSkill();
    ~TipsAwakeSkill();
    
    static TipsAwakeSkill* tipWithContent(char* icon, const char* strName, const char* Content, const char* idxTitle);
};

#endif /* defined(__AgainstWar__TipsAwakeSkill__) */
