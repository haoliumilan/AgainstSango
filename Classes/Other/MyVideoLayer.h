//
//  MyVideoLayer.h
//  AgainstWar
//
//  Created by caojiye on 13-7-22.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__MyVideoLayer__
#define __AgainstWar__MyVideoLayer__
#include "cocos2d.h"

class CListLayer;
class Dialog2;
class CommDlg;

using namespace cocos2d;

class MyVideoLayer : public CCLayer
{
    CCObject*         m_pListener;
    SEL_CallFuncO     m_pfnSelector;
    
    CCObject*         m_item_pListener;
    SEL_CallFuncO     m_item_pfnSelector;
    
    Dialog2*          m_dialog2;
    CommDlg*          m_commDlg;

    
    CListLayer*       m_list_layer;
    
public:

    static MyVideoLayer* Create(CCObject* target, SEL_CallFuncO selector);
    
    void Refresh();
    void RefreshStrategy();
    
    
private:
    
    MyVideoLayer(CCObject* target, SEL_CallFuncO selector);
    
    void SetList();
    
    void cbListItem(CCObject* obj);
    void cbList(CCObject* obj);
    void cbNoLocalRecord();
    void cbRemoveDialog2();
    void cbRemoveDialog2AndRefresh();
    void cbCommDlg(CCObject* pObj);


};




#endif /* defined(__AgainstWar__MyVideoLayer__) */
