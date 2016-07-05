//
//  ShopMain.h
//  AgainstWar
//
//  Created by XIII on 12-6-21.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_TimerLayer_h
#define AgainstWar_TimerLayer_h

#include "cocos2d.h"
#include "CGamedata.h"


using namespace cocos2d;

class CommDlg;



//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CTimerLayer : public CUiLayerBase
{
    int     m_iReqType;
    
public:
    CTimerLayer();
    CommDlg*        m_commDlg;
    virtual void enter();
    virtual void exit();
    virtual void onExit();
    
    void innerExit();
    void update(ccTime dt);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    void cbCommDlg(CCObject* pObj);
    void setTouchEnable();
};

#endif
