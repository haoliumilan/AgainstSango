//
//  CScrollBar.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-18.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CScrollBar_h
#define AgainstWar_CScrollBar_h


#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CScrollBar : public CCLayer
{
    CCSprite*   m_spScrollBar;
    CCSprite*   m_spScrollBarBk;
    
    float       m_fBarSize;
public:
    CScrollBar();

    void setBarSize(float fsize);
    void setPercent(float fPercent);
};


#endif
