//
//  ParcelScrewSelf.cpp
//  AgainstWar
//
//  Created by AllenSK on 14-2-18.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#include "ParcelScrewSelf.h"

ParcelScrewSelf::ParcelScrewSelf()
{
    m_angle = 3;
    m_vecFactor = 10;
    m_vecFactor2 = 0.01;
    
    m_iStep = 0;
}

ParcelScrewSelf* ParcelScrewSelf::actionWithDuration(ccTime d)
{
	ParcelScrewSelf* pAction = new ParcelScrewSelf();
    
	pAction->initWithDuration(d);
 	pAction->autorelease();
    
	return pAction;
}

CCObject* ParcelScrewSelf::copyWithZone(CCZone *pZone)
{
	CCZone* pNewZone = NULL;
	ParcelScrewSelf* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject)
	{
		//in case of being called at sub class
		pCopy = (ParcelScrewSelf*)(pZone->m_pCopyObject);
	}
	else
	{
		pCopy = new ParcelScrewSelf();
		pZone = pNewZone = new CCZone(pCopy);
	}
	
	CCActionInterval::copyWithZone(pZone);
    
	CC_SAFE_DELETE(pNewZone);
    
	return pCopy;
}

void ParcelScrewSelf::update(ccTime time)
{
    if(m_pTarget){
        int iStep = 180 * time;
        
        CCPoint pt = m_pTarget->getPositionInPixels();
        
        for(; m_iStep < iStep; m_iStep++){
            m_angle += 0.06;
//            m_vecFactor += (0.1 + m_vecFactor2);
//            m_vecFactor2 += 0.0005;
            
            m_vec.x = m_vecFactor * cos(m_angle);
            m_vec.y = m_vecFactor * sin(m_angle);
            
            pt = ccpAdd(pt, m_vec);
        }
        
        m_pTarget->setPositionInPixels(pt);
    }
}

CCActionInterval* ParcelScrewSelf::reverse(void)
{
	return NULL;
}
