//
//  CCShake.cpp
//  CardDemo
//
//  Created by Hao Zhang on 12-4-19.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CCShake.h"


#define	RAND_LIMIT	32767
/// Random floating point number in range [lo, hi]
float RandomFloat(float lo, float hi)
{
	float r = (float)(std::rand() & (RAND_LIMIT));
	r /= RAND_LIMIT;
	r = (hi - lo) * r + lo;
	return r;
}

CCShake::CCShake(CCPoint pt)
{
    m_ptOrig = pt;
}

CCShake* CCShake::actionWithDuration(ccTime d, CCPoint pt)
{
	CCShake* pAction = new CCShake(pt);
    
	pAction->initWithDuration(d);
 	pAction->autorelease();
    
	return pAction;
}

CCObject* CCShake::copyWithZone(CCZone *pZone)
{
	CCZone* pNewZone = NULL;
	CCShake* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject)
	{
		//in case of being called at sub class
		pCopy = (CCShake*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCShake(m_ptOrig);
		pZone = pNewZone = new CCZone(pCopy);
	}
	
	CCActionInterval::copyWithZone(pZone);
    
	CC_SAFE_DELETE(pNewZone);
    
	return pCopy;
}

void CCShake::update(ccTime time)
{
	CCPoint pt = m_ptOrig;
    
    float randx = RandomFloat(pt.x - 3, pt.x + 3);
    float randy = RandomFloat(pt.y - 3, pt.y + 3);
    
    if(time > 0.95)
        m_pTarget->setPosition(m_ptOrig);
    else
        m_pTarget->setPosition(CCPointMake(randx, randy));
}

CCActionInterval* CCShake::reverse(void)
{
	return NULL;
}