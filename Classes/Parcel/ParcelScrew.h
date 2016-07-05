//
//  ParcelScrew.h
//  AgainstWar
//
//  Created by AllenSK on 14-2-18.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__ParcelScrew__
#define __AgainstWar__ParcelScrew__

#include "cocos2d.h"

using namespace cocos2d;


class ParcelScrew : public CCActionInterval
{
    CCPoint     m_vec;
    float       m_angle;
    float       m_vecFactor;
    float       m_vecFactor2;
    
    CCSprite * m_sp;
    
    int         m_iStep;
    
public:
    virtual void update(ccTime time);
	virtual CCActionInterval* reverse(void);
	virtual CCObject* copyWithZone(CCZone* pZone);
    
public:
    ParcelScrew(CCSprite * sp , float ang , float vec);
    
	/** creates the action */
	static ParcelScrew* actionWithDuration(CCSprite * sp , ccTime d,float ang , float vec);
};




#endif /* defined(__AgainstWar__ParcelScrew__) */
