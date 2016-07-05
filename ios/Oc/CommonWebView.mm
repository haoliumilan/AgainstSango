//
//  CommonWebView.cpp
//  AgainstWar
//
//  Created by 王 海桅 on 12-12-3.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CommonWebView.h"
#include "sys/utsname.h"
#import "CWebView.h"
#include "OcProxy.h"
#import "AppController.h"


static  CommonWebView *cwInstance = NULL;

CWebView *m_Webview = NULL;

UIActivityIndicatorView *m_Indicator = NULL;

CommonWebView* CommonWebView::Inst()
{
    if(cwInstance==NULL)
    {
        cwInstance = new CommonWebView();
        cwInstance->m_isLoadSuccess = false;
    }
    return cwInstance;
}

//网页加载成功后的回调
void CommonWebView::webViewLoadSuccess()
{
    [m_Indicator stopAnimating];
     cwInstance->m_isLoadSuccess = true;
    
    if (cwInstance->isBottomAlign()) {
        CGRect frame = [m_Webview frame];
        CGFloat JSHeight = [[m_Webview stringByEvaluatingJavaScriptFromString:@"document.height"] floatValue];

        if (JSHeight > frame.size.height) {
            float preHeight = cwInstance->getCurContentHeight();
            
            if(preHeight <= frame.size.height){
                [m_Webview stringByEvaluatingJavaScriptFromString:
                 [NSString stringWithFormat:@"scrollTo(0,%f)", JSHeight - frame.size.height]];
            }
            else{
                if (preHeight > frame.size.height) {
                    [m_Webview stringByEvaluatingJavaScriptFromString:
                     [NSString stringWithFormat:@"scrollTo(0,%f)", preHeight - frame.size.height]];
                }
                
                [m_Webview stringByEvaluatingJavaScriptFromString:
                 [NSString stringWithFormat:@"scrollTo(0,%f)", JSHeight - frame.size.height]];
            }

            cwInstance->setCurContentHeight(JSHeight);
        }
    }
    
    [m_Webview setHidden: NO];
}

//显示webView
void CommonWebView::showWebViewWithFrameAndHtml(CCRect rect,const char *string,
                                                enWebviewLoadType type,
                                                bool bGoToBrowser)
{
    m_bBottomAlign = false;
    m_isLoadSuccess = false;
    CGRect preRect = CGRectMake(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
    if((m_Indicator == NULL)&&(m_Webview == NULL))
    {
        m_Indicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleGray];
        [m_Indicator setFrame:CGRectMake(135, 240+OcProxy::Inst()->deltaHeightOf568(), 50, 50)];
        
        m_Webview =  [[CWebView alloc] initWithFrame:preRect isGoToBrowser:bGoToBrowser];
        [m_Webview setBackgroundColor:[UIColor clearColor]];
        m_Webview.opaque = NO;
        [m_Webview setNoticeCallBack:webViewLoadSuccess];
    
        UIWindow* window = [UIApplication sharedApplication].keyWindow;
        if (!window) {
            window = [[UIApplication sharedApplication].windows objectAtIndex:0];
        }
        AppController *appCtr = (AppController *)[UIApplication sharedApplication].delegate;
        [((UIViewController *)appCtr.viewController).view  addSubview:m_Webview];
        
        [m_Webview release];
        [window addSubview:m_Indicator];
        [m_Indicator release];
    }
    
    [m_Webview setFrame:preRect];
    [m_Webview clearsContextBeforeDrawing];
    [m_Indicator startAnimating];
    if(type == enWebviewLoadTypeRequest)
    {
        NSString *htmlStr = [NSString stringWithUTF8String:string];
        [m_Webview loadRequest:[NSURLRequest requestWithURL:
                                [NSURL URLWithString:htmlStr]]];
    }else{
        NSString *htmlStr = [NSString stringWithUTF8String:string];
        [m_Webview loadHTMLString:htmlStr baseURL:nil];
    }
    
    m_ContentHeight = [[m_Webview stringByEvaluatingJavaScriptFromString:@"document.height"] floatValue];    
}
//是否加载完成
bool CommonWebView::getIsLoadSuccess()
{
    return m_isLoadSuccess;
}
//删掉
void CommonWebView::removeWebview()
{
    if(m_Indicator)
    {
        [m_Indicator stopAnimating]; 
        [m_Indicator removeFromSuperview];
         m_Indicator = NULL;
    }
    if(m_Webview)
    {
        [m_Webview removeFromSuperview];
         m_Webview = NULL;
    }
    m_isLoadSuccess = false;
}

void CommonWebView::showHtmlStr(const char *string, bool bBottomAlign)
{
    m_bBottomAlign = bBottomAlign;
    
    NSString *htmlStr = [NSString stringWithUTF8String:string];
    [m_Webview loadHTMLString:htmlStr baseURL:nil];
    
    [m_Webview setHidden: YES];
}

void CommonWebView::setFrame(CCRect rect)
{
    CGRect frm = CGRectMake(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
    [m_Webview setFrame:frm];
    
    CGFloat JSHeight = [[m_Webview stringByEvaluatingJavaScriptFromString:@"document.height"] floatValue];
    if (JSHeight > rect.size.height) {
        [m_Webview stringByEvaluatingJavaScriptFromString:
         [NSString stringWithFormat:@"scrollTo(0,%f)", JSHeight - rect.size.height]];
    }
}



