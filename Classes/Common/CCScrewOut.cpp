//
//  CCScrewOut.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 12-11-8.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CCScrewOut.h"

CCScrewOut::CCScrewOut()
{
    m_angle = 3;
    m_vecFactor = 1;
    m_vecFactor2 = 0.01;
    
    m_iStep = 0;
}

CCScrewOut* CCScrewOut::actionWithDuration(ccTime d)
{
	CCScrewOut* pAction = new CCScrewOut();
    
	pAction->initWithDuration(d);
 	pAction->autorelease();
    
	return pAction;
}

CCObject* CCScrewOut::copyWithZone(CCZone *pZone)
{
	CCZone* pNewZone = NULL;
	CCScrewOut* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject)
	{
		//in case of being called at sub class
		pCopy = (CCScrewOut*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new CCScrewOut();
		pZone = pNewZone = new CCZone(pCopy);
	}
	
	CCActionInterval::copyWithZone(pZone);
    
	CC_SAFE_DELETE(pNewZone);
    
	return pCopy;
}

void CCScrewOut::update(ccTime time)
{
    if(m_pTarget){
        int iStep = 180 * time;
        
        CCPoint pt = m_pTarget->getPositionInPixels();
        
        for(; m_iStep < iStep; m_iStep++){
            m_angle += 0.06;
            m_vecFactor += (0.1 + m_vecFactor2);
            m_vecFactor2 += 0.0005;
            
            m_vec.x = m_vecFactor * cos(m_angle);
            m_vec.y = m_vecFactor * sin(m_angle);

            pt = ccpAdd(pt, m_vec);
        }
        
        m_pTarget->setPositionInPixels(pt);
    }
}

CCActionInterval* CCScrewOut::reverse(void)
{
	return NULL;
}