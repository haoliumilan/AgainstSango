//
//  PublishWeiboView.h
//  AgainstWar
//
//  Created by 海桅 王 on 12-7-10.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface PublishWeiboView : UIView
{
    UIScrollView *scrollView;
    UILabel *weiboLabel;
    UIImageView *imageView;
}

- (id)initWithFrame:(CGRect)frame withText:(NSString *)weiboStr withImage:(UIImage *)weiboImage;
@end
