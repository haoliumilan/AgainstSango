//
//  OrgStoreEnergy.h
//  AgainstWar
//
//  Created by Zhang Hao on 13-1-22.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__OrgStoreEnergy__
#define __AgainstWar__OrgStoreEnergy__

#include "cocos2d.h"

using namespace cocos2d;

class OrgStoreEnergy : public CCLayer
{
    
    CCProgressTimer*    m_engyBar;
//    
//    float       m_fWidth;
//    float       m_fHeight;

    float       m_fElapse;
    float       m_fStartPerct;
    float       m_fCurPerct;
    float       m_fDestPerct;
public:
    OrgStoreEnergy();
    
    virtual void onEnter();
    
    void setStep(int iStep, float dt);
    
    void useEnergy(float dt);
    
private:

    
    
    void update(ccTime dt);
    
    void cbUseEnergy(ccTime dt);
};

#endif /* defined(__AgainstWar__OrgStoreEnergy__) */
