//
//  ParcelScrewSelf.h
//  AgainstWar
//
//  Created by AllenSK on 14-2-18.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__ParcelScrewSelf__
#define __AgainstWar__ParcelScrewSelf__

#include "cocos2d.h"

using namespace cocos2d;


class ParcelScrewSelf : public CCActionInterval
{
    CCPoint     m_vec;
    float       m_angle;
    float       m_vecFactor;
    float       m_vecFactor2;
    
    int         m_iStep;
    
public:
    virtual void update(ccTime time);
	virtual CCActionInterval* reverse(void);
	virtual CCObject* copyWithZone(CCZone* pZone);
    
public:
    ParcelScrewSelf();
    
	/** creates the action */
	static ParcelScrewSelf* actionWithDuration(ccTime d);
};

#endif /* defined(__AgainstWar__ParcelScrewSelf__) */
