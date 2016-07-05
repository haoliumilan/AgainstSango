//
//  EvoOverShow.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-1.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_EvoOverShow_h
#define AgainstWar_EvoOverShow_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

class CUserCardInfo;
class CShowCardInf;

enum enEvoType{
    enEvoType_Nml,
    enEvoType_Sevo,
    enEvoType_Wake
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class EvoOverShow : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;
    
    CShowCardInf*   m_showCard;
public:
    EvoOverShow(CCObject* target, SEL_CallFunc selector, int iEvoType);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    virtual void onExit();
    
private:
    void nmlEvoAnim(int src_cid, int tgt_cid);
    void superEvoAnim(int src_cid, int tgt_cid);
    void wakeEvoAnim(int src_cid, int tgt_cid);
    
    void showCardInfo();
    void cbShowCardInfo();
    void showTorchParticle();
    
    CCActionInterval* getActionFromFile(const char* filename);
    CCActionInterval* getActionFromPlist(const char* filename);
};

#endif
