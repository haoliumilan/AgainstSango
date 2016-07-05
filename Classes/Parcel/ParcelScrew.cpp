//
//  ParcelScrew.cpp
//  AgainstWar
//
//  Created by AllenSK on 14-2-18.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#include "ParcelScrew.h"


ParcelScrew::ParcelScrew(CCSprite * sp ,float ang , float vec)
{
//    m_angle = 0;
//    m_vecFactor = 250;
    
    m_angle = ang;
    m_vecFactor = vec;
    
    m_sp = sp;

    m_vecFactor2 = 0.01;
    
    m_iStep = 0;
}

ParcelScrew* ParcelScrew::actionWithDuration(CCSprite * sp , ccTime d , float ang , float vec)
{
	ParcelScrew* pAction = new ParcelScrew(sp , ang , vec);
    
	pAction->initWithDuration(d);
 	pAction->autorelease();
    
	return pAction;
}

CCObject* ParcelScrew::copyWithZone(CCZone *pZone)
{
	CCZone* pNewZone = NULL;
	ParcelScrew* pCopy = NULL;
	if(pZone && pZone->m_pCopyObject)
	{
		//in case of being called at sub class
		pCopy = (ParcelScrew*)(pZone->m_pCopyObject);
	}
	else
	{
//		pCopy = new ParcelScrew();
//		pZone = pNewZone = new CCZone(pCopy);
	}
	
	CCActionInterval::copyWithZone(pZone);
    
	CC_SAFE_DELETE(pNewZone);
    
	return pCopy;
}

void ParcelScrew::update(ccTime time)
{
    if(m_pTarget){
        int iStep = 180 * time;
        
//        CCPoint pt = m_pTarget->getPositionInPixels();
        

        
        
        for(; m_iStep < iStep; m_iStep++){
            CCPoint pt = CCPointMake(320, 480);
            
            
            
            m_angle += 0.06f;
//            m_vecFactor += (0.1 + m_vecFactor2);
//            m_vecFactor2 += 0.0005;
            
            m_vecFactor -= 1.1f;
            
            if(m_vecFactor <= 0)
                m_vecFactor = 0;
            
            m_vec.x = m_vecFactor * cos(m_angle);
            m_vec.y = m_vecFactor * sin(m_angle);
            
            pt = ccpAdd(pt, m_vec);
            
            m_pTarget->setPositionInPixels(pt);
            
            
//            float tt = m_sp->getRotation();
//            
//            m_sp -> setRotation(tt + 3.0);
        }
        
        
    }
}

CCActionInterval* ParcelScrew::reverse(void)
{
	return NULL;
}