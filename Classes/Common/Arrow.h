//
//  Arrow.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-15.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_Arrow_h
#define AgainstWar_Arrow_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class Arrow : public CCLayer
{
    
public:
    Arrow(int length, GLubyte Opacity);
    
    virtual void onExit();
    
private:
    
};

#endif
