//
//  MailViewTexture.m
//  AgainstWar
//
//  Created by caojiye on 13-1-10.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#import "MailViewTexture.h"

@implementation MailViewTexture
- (id)initWithFrame:(CGRect)frame withText:(NSString *)mailstr
{
    self = [super initWithFrame:frame];
    if (self)
        // Initialization code
    scrollView = [[[UIScrollView alloc] initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height)] autorelease];
    [self addSubview:scrollView];
    
    CGSize size = CGSizeMake(frame.size.width, CGFLOAT_MAX);

    NSString *commentStr = [NSString stringWithString:mailstr];
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

    [scrollView setContentSize:CGSizeMake(frame.size.width, weiboLabel.frame.size.height)];
    //*/
return self;

}

- (void)dealloc
{
    [super dealloc];
}





@end
