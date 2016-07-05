//
//  PublishWeiboView.m
//  AgainstWar
//
//  Created by 海桅 王 on 12-7-10.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#import "PublishWeiboView.h"

@implementation PublishWeiboView

- (id)initWithFrame:(CGRect)frame withText:(NSString *)weiboStr withImage:(UIImage *)weiboImage
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        scrollView = [[[UIScrollView alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height)] autorelease];
        [self addSubview:scrollView];
        
        CGSize size = CGSizeMake(frame.size.width, 250*frame.size.width/220);
        
        //        NSString* commentStr = [[NSString alloc] initWithFormat:@"测试我已经点亮这么多武将啦！更多更强的武将尽在《逆转三国》！http://t.cn/zWcOcPO"];
        NSString *commentStr = [NSString stringWithString:weiboStr];
        CGSize labelSize = [commentStr sizeWithFont:[UIFont fontWithName:
                                                     @"Arial" size:13*frame.size.width/220]
                                  constrainedToSize:size
                                      lineBreakMode:UILineBreakModeCharacterWrap];
        weiboLabel = [[[UILabel alloc] initWithFrame:CGRectMake(0, 0,
                                                               labelSize.width, labelSize.height)] autorelease];
        weiboLabel.text = commentStr;
        weiboLabel.font = [UIFont fontWithName:@"Arial" size:13*frame.size.width/220];
        weiboLabel.backgroundColor = [UIColor clearColor];
        weiboLabel.numberOfLines = 0;
        [weiboLabel setLineBreakMode:UILineBreakModeCharacterWrap];
        [scrollView addSubview:weiboLabel];
        
        
        NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,
                                                             NSUserDomainMask, YES);
        NSString *documentsDirectory = [paths objectAtIndex:0];
        NSString* path = [documentsDirectory stringByAppendingPathComponent:
                          [NSString stringWithUTF8String:"captureImage.png"]];
        UIImage* smallImage = [UIImage imageWithContentsOfFile:path];
        
        imageView = [[[UIImageView alloc] initWithImage:smallImage] autorelease];
        [imageView setFrame:CGRectMake(0, weiboLabel.frame.size.height+weiboLabel.frame.origin.y+10*frame.size.width/220, frame.size.width/2, smallImage.size.height/smallImage.size.width*frame.size.width/2)];
        [scrollView addSubview:imageView];
        [scrollView setContentSize:CGSizeMake(frame.size.width, imageView.frame.origin.y+imageView.frame.size.height)];
        
    }
    return self;
}

- (void)dealloc
{
    [super dealloc];
}

@end
