//
//  CWebView.m
//  AgainstWar
//
//  Created by 海桅 王 on 12-8-10.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#import "CWebView.h"

@implementation CWebView

- (id)initWithFrame:(CGRect)frame isGoToBrowser:(BOOL)bGoTo
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        isSetCb = false;
        self.delegate = self;
        bGoToBrowser = bGoTo;
    }
    return self;
}


- (void)setNoticeCallBack:(void (*)())cb
{
    isSetCb = true;
    webViewLoadSuccess = cb;
}


- (void)webViewDidFinishLoad:(UIWebView *)webView
{
    if(isSetCb)
    {
        webViewLoadSuccess();
    }
}

-(BOOL)webView:(UIWebView *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(UIWebViewNavigationType)navigationType;
{
    NSURL *requestURL =[ [ request URL ] retain ];
    if (bGoToBrowser)
    {
        if ([ [ requestURL absoluteString] rangeOfString: @"#" ].location !=NSNotFound) {
            [ requestURL release ];
            return YES;
        }
        if ( ( [ [ requestURL scheme ] isEqualToString: @"http" ]
              || [ [ requestURL scheme ] isEqualToString: @"https" ]
              || [ [ requestURL scheme ] isEqualToString: @"mailto" ])
            && ( navigationType == UIWebViewNavigationTypeLinkClicked ) ) {
            return ![ [ UIApplication sharedApplication ] openURL: [ requestURL autorelease ] ];
        }
    }
    [ requestURL release ];
    return YES;
}

@end
