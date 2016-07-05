//
//  TipsPopInfo.cpp
//  AgainstWar
//
//  Created by user on 13-9-2.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "TipsPopInfo.h"


TipsPopInfo::TipsPopInfo()
{
    
}

TipsPopInfo::~TipsPopInfo()
{
    
}

void TipsPopInfo::addTipString(const char* Content)
{
    TextNode* tContent = TextNode::textWithString(Content, CCSizeMake(getContentSize().width - 30, getContentSize().height - 10) , CCTextAlignmentLeft, 23);
    addChild(tContent, 10);
    tContent->setPosition(ccp((getContentSize().width - 30)/2 + 15, (getContentSize().height - 10)/2 + 6));
}


TipsPopInfo* TipsPopInfo::tipWithContent(const char* Content)
{
    TipsPopInfo* ret = new TipsPopInfo();
    if (ret && ret->initWithSpriteFrameName("propertyTip_dlg.png"))
    {
        ret->autorelease();
        
        ret->addTipString(Content);
        
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return NULL;
}





