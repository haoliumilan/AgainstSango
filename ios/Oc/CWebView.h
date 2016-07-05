//
//  CWebView.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-8-10.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface CWebView : UIWebView<UIWebViewDelegate>
{
    void (*webViewLoadSuccess)();
    bool isSetCb;
    BOOL bGoToBrowser;  // 是否跳转到浏览器
}
- (id)initWithFrame:(CGRect)frame isGoToBrowser:(BOOL)bGoTo;
- (void)setNoticeCallBack:(void (*)())cb;

@end
