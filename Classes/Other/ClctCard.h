//
//  ClctCard.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-12-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_ClctCard_h
#define AgainstWar_ClctCard_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class ClctCard : public CCLayer
{
    CCSprite*   m_spHead;
    
    int         m_iCid;
    int         m_iStatus;
    
public:
    ClctCard(int cid, int status);

    void    show();
    void    hide();
    int getStatus()const {return m_iStatus;}
};

#endif
