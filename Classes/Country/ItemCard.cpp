//
//  ItemCard.cpp
//  AgainstWar
//
//  Created by user on 13-4-12.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "ItemCard.h"


ItemCard::ItemCard(string id, short cardNum)
{
    m_CardId = id;
    m_CardNumber = cardNum;
}


ItemCard::~ItemCard()
{
    
}

ItemCard* ItemCard::spriteWithFile(const char *normalImage, string id, short cardNum)
{
    ItemCard* pRet = new ItemCard(id, cardNum);
    
    if ((normalImage == NULL) || (*normalImage == 0)) {
        normalImage = "box_none.png";
    }
    
    if (pRet && pRet->initWithFile(normalImage))
    {
        pRet->autorelease();
        
        char buf[30];
        sprintf(buf, "/%d",cardNum);
        
        CCSprite* spTmp = CCSprite::spriteWithFile("rewardnum.png");
        if(spTmp){
            CCSize sz = spTmp->getContentSize();
            CCLabelAtlas* textNum = CCLabelAtlas::labelWithString(buf,
                                                                      "rewardnum.png",
                                                                      sz.width / 11,
                                                                      sz.height,
                                                                      '/');
            textNum->setAnchorPoint(ccp(0.5, 0.5));
            textNum->setPosition(ccp(54,10));
            pRet->addChild(textNum,0,9);
        }
        
        return pRet;
    }
    
    CC_SAFE_DELETE(pRet);
    return NULL;
}