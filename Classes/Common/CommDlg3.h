//
//  CommDlg3.h
//  AgainstWar
//
//  Created by caojiye on 13-7-2.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__CommDlg3__
#define __AgainstWar__CommDlg3__

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

class TextNode;

///////////////////////////////////////////////////
class Dialog3Ret : public CCObject
{
public:
    int  bSel;
};
///////////////////////////////////////////////////
class Dialog3 : public CCLayer
{
    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;

    
    CCSprite*       m_spOk;
    CCSprite*       m_spCancel;
    
    Dialog3Ret      m_ret;
public:
    
    static Dialog3* CreateBind(CCObject* target, SEL_CallFuncO selector);
    static Dialog3* CreateUpload(CCObject* target, SEL_CallFuncO selector);

    TextNode* GetTextTitle(const char* ch, float width);
    TextNode* GetTextContent(const char* ch, float font_size, CCSize text_size);
    TextNode* GetTextNode(const char* ch, CCSize text_size ,CCTextAlignment text_alignment, float font_size, ccColor3B text_color, bool shadow_black);
    
    CCMenuItem* GetMenuItem(const char* title);
    CCMenuItem* GetMenuItemWithImage(const char* img);
    
    void CallBack(CCNode* _node);
    
private:
    Dialog3(CCObject* target, SEL_CallFuncO selector, const char* title,const char* dsp, const char* bkg, const char* btn_text1, const char* btn_text2, const char* btn_text3, const char* btn_img1, const char* btn_img2, const char* btn_img3);
    
};
///////////////////////////////////////////////////


#endif /* defined(__AgainstWar__CommDlg3__) */
