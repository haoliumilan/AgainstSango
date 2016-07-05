//
//  MailViewController.h
//  AgainstWar
//
//  Created by Liu Hao on 13-1-5.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface MailViewController : UIViewController <UITextViewDelegate>
{
    UITextView *mailTextView;
    NSString *strMail;          // 邮件内容
    BOOL bHasSent;              // 是否已经发送
    NSString *strUserName;      // 邮件目标玩家的名字
}

@property (nonatomic, retain) NSString* strMail;
@property (nonatomic) BOOL bHasSent;

- (id)initWithUserName:(NSString*) userName;

@end
