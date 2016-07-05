//
//  MsgBox.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-15.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "MsgBox.h"
#include "TextNode.h"

//******************************************************************************
// Constructor
//******************************************************************************
MsgBox::MsgBox(const char* imgName, const char* dsp, ccColor3B txtColor)
{
    CCSprite* sp = CCSprite::spriteWithFile(imgName);
    if(sp == NULL)
        sp = CCSprite::spriteWithSpriteFrameName(imgName);
    
    CCSize sz;
    if(sp){
        addChild(sp);
        sz = sp->getContentSize();
    }
    
    //dsp
    sz.width -= 30;
    sz.height -= 30;
    TextNode* lbText = TextNode::textWithString(dsp, sz, CCTextAlignmentCenter,  30);
    lbText->setColor(txtColor);
    addChild(lbText, 1);
}

//******************************************************************************
// onExit
//******************************************************************************
void MsgBox::onExit()
{
    removeAllChildrenWithCleanup(true);
    
    CCLayer::onExit();
}