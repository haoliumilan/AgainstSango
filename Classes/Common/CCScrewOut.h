//
//  CCScrewOut.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-11-8.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CCScrewOut_h
#define AgainstWar_CCScrewOut_h

#include "cocos2d.h"

using namespace cocos2d;


class CCScrewOut : public CCActionInterval
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
    CCScrewOut();
    
	/** creates the action */
	static CCScrewOut* actionWithDuration(ccTime d);
};

#endif
