//
//  CCShake.h
//  CardDemo
//
//  Created by Hao Zhang on 12-4-19.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef CardDemo_CCShake_h
#define CardDemo_CCShake_h

#include "cocos2d.h"

using namespace cocos2d;


class CCShake : public CCActionInterval
{
    CCPoint m_ptOrig;
public:
    virtual void update(ccTime time);
	virtual CCActionInterval* reverse(void);
	virtual CCObject* copyWithZone(CCZone* pZone);
    
public:
    CCShake(CCPoint pt);
    
	/** creates the action */
	static CCShake* actionWithDuration(ccTime d, CCPoint pt);
};

#endif
