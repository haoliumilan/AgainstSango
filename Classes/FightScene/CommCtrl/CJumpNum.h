//
//  CJumpNum.h
//
//  Created by Hao Zhang on 11-11-10.
//  Copyright (c) 2011 zhanghao. All rights reserved.
//

#ifndef _CJUMPNUM_H_
#define _CJUMPNUM_H_

#include "cocos2d.h"

using namespace cocos2d;

class CJumpNum :public CCLabelAtlas
{
    CCPoint         m_position;

    int             m_iStartNum;
    int             m_iCurNum;
    int             m_iDstNum;
    
    float           m_fDur;
    float           m_fStartTime;
    float           m_fElapse;
    
    bool            m_bActionFlg;
    
    bool            m_bJump;

    bool            m_bIsMain;
    bool            m_bScale;
    CJumpNum(bool isMain);
public:
    static CJumpNum * initNumber(const char* filename, bool isMain = true);
    
    void setPosition(const CCPoint& newPosition);
    
    void setNumber(int num, float fDur, ccTime dt);
    
    void update(ccTime dt);
    
    virtual void onExit();
    
    void setJump(bool bJump);
    
    void setIsScale(bool bScale);
};

#endif
