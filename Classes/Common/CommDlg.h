//
//  CommDlg.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-6-8.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CommDlg_h
#define AgainstWar_CommDlg_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

enum CommDlgType{
    enCommDlgTp_connecting,
    enCommDlgTp_retry
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CommDlgRet : public CCObject
{
public:
    bool bOk;
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CommDlg : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    
    CommDlgRet      m_ret;
    int             m_iRc;
public:
    CommDlg(CCObject* target, SEL_CallFuncO selector, CommDlgType);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    virtual void onExit();
    
    void setNoCancel();
private:
    void initConnecting();
    void initRetry();
    
    void initBtn();
};

#endif
