//
//  BulletLight.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-7-30.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __test__BulletLight__
#define __test__BulletLight__

#include "cocos2d.h"
#include <list>

USING_NS_CC;

#define BltLgtPtNum 16

class BulletLight : public CCLayer
{
    ccTime          m_fElapse;
    ccTime          m_fDelay;
    ccTime          m_fDur;
    ccBezierConfig  m_sConfig;
    ccColor4B       m_color4;
    CCPoint         m_startPt;
    
    CCPoint         m_pointPath[BltLgtPtNum] ;
    unsigned long   m_Index ;
    
public:
    BulletLight();
    void shoot(const CCPoint& srcPt, const CCPoint& destPt, const ccColor4B& color4, ccTime fDelay, ccTime fDur);

private:
    void line();
    void eraseTail();
    void triangle(CCPoint* vertex, CCPoint p1, CCPoint p2, GLfloat w);

    void update(ccTime dt);
    void draw();
};

#endif /* defined(__test__BulletLight__) */
