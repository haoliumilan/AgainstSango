//
//  FeatureAnim.h
//  AgainstWar
//
//  Created by user on 13-7-29.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__FeatureAnim__
#define __AgainstWar__FeatureAnim__

#include "cocos2d.h"

USING_NS_CC;

class FeatureAnim : public CCLayer
{
  
    void CreateAnim();
    
    CCSprite* m_roler;
    
    void showRoler();
    void HideRoler();
    
    const char* m_imgName;
    bool        m_bTemp;
    
public:
    FeatureAnim(const char* imgName,bool bTemp = false);
    ~FeatureAnim();
};

#endif /* defined(__AgainstWar__FeatureAnim__) */
