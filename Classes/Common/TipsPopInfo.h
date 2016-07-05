//
//  TipsPopInfo.h
//  AgainstWar
//
//  Created by user on 13-9-2.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__TipsPopInfo__
#define __AgainstWar__TipsPopInfo__

#include "cocos2d.h"
#include "TextNode.h"

USING_NS_CC;

class TipsPopInfo : public CCSprite
{
    void addTipString(const char* content);
    
public:
    
    TipsPopInfo();
    ~TipsPopInfo();
    
    static TipsPopInfo* tipWithContent(const char* Content);
};


#endif /* defined(__AgainstWar__TipsPopInfo__) */
