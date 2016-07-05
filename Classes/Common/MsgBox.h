//
//  MsgBox.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-15.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_MsgBox_h
#define AgainstWar_MsgBox_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class MsgBox : public CCLayer
{

public:
    MsgBox(const char* imgName, const char* dsp, ccColor3B txtColor);
    
    virtual void onExit();
    
private:
    
};

#endif
