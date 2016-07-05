//
//  CAlipay.h
//  AgainstWar
//
//  Created by hejian on 13-10-16.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface CAlipay : NSObject
{
    int alipayStatus;
}

- (void)purAlipay:(const char*) mTradeNo withTitle:(const char*) title withPrice:(int) price withIsTruePrice:(BOOL) isTruePrice;
- (void)endAlipay;
- (int)parseURL:(NSURL *)url application:(UIApplication *)application;

@end
