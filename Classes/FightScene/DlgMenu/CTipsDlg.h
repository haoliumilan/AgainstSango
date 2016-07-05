//
//  CTipsDlg.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CTipsDlg_h
#define AgainstWar_CTipsDlg_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

class TextNode;
//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CTipsDlg : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;
    
    TextNode*       m_lbTip;
    TextNode*       m_lbTipBk;
    
public:
	CTipsDlg(CCObject* target, SEL_CallFunc selector);
    virtual ~CTipsDlg();
    
    virtual void onExit();
    
    void show();
    
    
    
private:
    const char* get1tip();
    
    void menuCallback(CCObject* sender);
};

#endif
