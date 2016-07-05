//
//  ItemCard.h
//  AgainstWar
//
//  Created by user on 13-4-12.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__ItemCard__
#define __AgainstWar__ItemCard__

#include "cocos2d.h"
#include "CommonDef.h"
#include "CGamedata.h"
#include "CScrollBar.h"

using namespace cocos2d;


#define IMCARDSTRTYPE_COIN          "coin"
#define IMCARDSTRTYPE_GOLD          "gold"
#define IMCARDSTRTYPE_EXP           "exp"
#define IMCARDSTRTYPE_WEALTH        "wealth"
#define IMCARDSTRTYPE_STAMINA       "stamina"
#define IMCARDSTRTYPE_CARD          "card"
#define IMCARDSTRTYPE_CD            "cd"
#define IMCARDSTRTYPE_RENAME        "rename"
#define IMCARDSTRTYPE_GACHA         "gacha"
#define IMCARDSTRTYPE_GACHA_MULTI   "gacha_multi"
#define IMCARDSTRTYPE_STONE         "stone"
#define IMCARDSTRTYPE_COST_UP       "cost_up"

#define IMCARDSTRTYPE_COMPOUND       "compound"



class ItemCard : public CCSprite
{
      public:
    
    string m_CardId;
    short m_CardNumber;
    

    
    static ItemCard* spriteWithFile(const char *normalImage, string id, short cardNum);
    
    ItemCard(string id, short cardNum);
    
    ~ItemCard();
    
};


#endif /* defined(__AgainstWar__ItemCard__) */
