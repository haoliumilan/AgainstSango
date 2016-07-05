//
//  ActNumTTF.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-7-6.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef _ActNumTTF_H
#define _ActNumTTF_H

#include "cocos2d.h"

using namespace cocos2d;

class TextNode;

class ActNumTTF :public CCNode
{
    unsigned long long             m_iPrevNum;
    unsigned long long             m_iCurNum;
    unsigned long long             m_iEndNum;
    unsigned long long             m_iMaxNum;
    
    float           m_fElapse;
    float           m_fDur;
    
    TextNode*       m_lbNum;
    
    ActNumTTF(const CCSize& dimensions, 
              CCTextAlignment alignment, 
              float fontSize, 
              const ccColor3B& color3);
    
public:
    static ActNumTTF * initNumber(const CCSize& dimensions, 
                                  CCTextAlignment alignment, 
                                  float fontSize, 
                                  const ccColor3B& color3);
    
    virtual void onExit();

    void setNumber(unsigned long long endnum, ccTime dt);
    
    void reachNumInstantly();
    
    void setMaxNum(unsigned long long maxNum);
    
    void update(ccTime dt);
};

#endif
