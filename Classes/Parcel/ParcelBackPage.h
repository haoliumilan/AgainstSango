//
//  ParcelBackPage.h
//  AgainstWar
//
//  Created by AllenSK on 14-2-11.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__ParcelBackPage__
#define __AgainstWar__ParcelBackPage__

#include "cocos2d.h"
#include "CGamedata.h"
#include "ItemCardLayer.h"
#include "Package.h"

using namespace cocos2d;




class TextNode;

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class ParcelBackPage : public CUiLayerBase
{
   
    
public:
    
    CCMenuItem *m_child_on;
    CCMenuItem *m_item_on;
    
    
    CCMenuItem *m_child_off;
    CCMenuItem *m_item_off;
    
    
    
    CCSprite * m_pParcalBack;
    
    bool m_choiceChild;
    
    void cbCheckItemBtn();
    void cbCheckChildBtn();
    
    
    
    ParcelBackPage();
    
    
    void NtyShow();
    void NtyHide();
    
    virtual void enter();
    virtual void exit();
    
    
};

#endif /* defined(__AgainstWar__ParcelBackPage__) */
