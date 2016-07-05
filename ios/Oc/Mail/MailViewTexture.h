//
//  MailViewTexture.h
//  AgainstWar
//
//  Created by caojiye on 13-1-10.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface MailViewTexture : UIView
{
    UIScrollView *scrollView;
    UILabel *weiboLabel;
}
- (id)initWithFrame:(CGRect)frame withText:(NSString *)mailstr;
@end
