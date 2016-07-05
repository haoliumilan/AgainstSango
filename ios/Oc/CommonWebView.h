//
//  CommonWebView.h
//  AgainstWar
//
//  Created by 王 海桅 on 12-12-3.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__CommonWebView__
#define __AgainstWar__CommonWebView__

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;

class CommonWebView :public CCObject
{
    bool m_isLoadSuccess;
    bool m_bBottomAlign;
    float m_ContentHeight;
    
public:
    static CommonWebView* Inst();
    static void   webViewLoadSuccess();
    void showWebViewWithFrameAndHtml(CCRect rect,const char *string,
                                     enWebviewLoadType type,
                                     bool bGoToBrowser = false);
    void removeWebview();
    bool getIsLoadSuccess();
    
    void showHtmlStr(const char *string, bool bBottomAlign);
    
    bool isBottomAlign(){return m_bBottomAlign;}
    float getCurContentHeight(){return m_ContentHeight;}
    void setCurContentHeight(float fHeight){m_ContentHeight = fHeight;}

    void setFrame(CCRect rect);
};


#endif /* defined(__AgainstWar__CommonWebView__) */
