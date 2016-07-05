//
//  CHurtNum.h
//  CardDemo
//
//  Created by Hao Zhang on 12-5-5.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_CHurtNum_h
#define CardDemo_CHurtNum_h

#include "cocos2d.h"

using namespace cocos2d;

#define kHurtNumDur1    0.6
#define kHurtNumDur2    0.5
#define kHurtArea       30

class CHurtNum :public CCLabelAtlas
{
    CCPoint         m_position;
    
    int             m_iStartNum;
    int             m_iEndNum;
    
    float           m_fElapse;
    float           m_fNumChgDur;
    
    CHurtNum();
public:
    static CHurtNum * initNumber(const char* filename);
    
    void setPosition(const CCPoint& newPosition);
    
    void setNumber(int startnum, int endnum, ccTime dt);
    
    void update(ccTime dt);
};

#endif
