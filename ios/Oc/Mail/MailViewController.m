//
//  MailViewController.m
//  AgainstWar
//
//  Created by Liu Hao on 13-1-5.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#import "MailViewController.h"

@interface MailViewController (Private)

- (void)quitMail;
- (void)sendMail;

@end

@implementation MailViewController
@synthesize strMail;
@synthesize bHasSent;

- (id)initWithUserName:(NSString *)userName
{
    self = [super initWithNibName:nil bundle:nil];
    if (self) {
        // Custom initialization
        strUserName = [[NSString alloc] initWithString:userName];
    }
    return self;
}

- (void)viewDidLoad
{
    /*
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    bHasSent = NO;
    
    [self.view setBackgroundColor:[UIColor whiteColor]];
    
    [self.navigationItem setTitle:@"发邮件"];
    
    UIBarButtonItem* leftButton = [[UIBarButtonItem alloc] initWithTitle:@"返回"
                                                                   style:UIBarButtonItemStylePlain
                                                                  target:self
                                                                  action:@selector(quitMail)];

    UIBarButtonItem* rightButton = [[UIBarButtonItem alloc] initWithTitle:@"发送"
                                                                    style:UIBarButtonItemStylePlain
                                                                   target:self
                                                                   action:@selector(sendMail)];
    
    self.navigationItem.leftBarButtonItem = leftButton;
    self.navigationItem.rightBarButtonItem = rightButton;
    
    [leftButton release];
    [rightButton release];
    
    UILabel* lbToUser = [[UILabel alloc] initWithFrame:CGRectMake(10, 0, 320, 30)];
    [lbToUser setTextAlignment:UITextAlignmentLeft];
    [lbToUser setText:[NSString stringWithFormat:@"To:  %@", strUserName]];
    [self.view addSubview:lbToUser];
    [lbToUser release];
    
    UILabel* lbLimit = [[UILabel alloc] initWithFrame:CGRectMake(10, 30, 320, 30)];
    [lbLimit setTextAlignment:UITextAlignmentLeft];
    [lbLimit setTextColor:[UIColor lightGrayColor]];
    [lbLimit setText:@"要求不得多余N个字符"];
    [self.view addSubview:lbLimit];
    [lbLimit release];
    
    CGSize size_screen = [[UIScreen mainScreen] bounds].size;

    mailTextView = [[UITextView alloc] initWithFrame:CGRectMake(10,
                                                                60,
                                                                size_screen.width - 20,
                                                                size_screen.height * 0.5)];
    mailTextView.delegate =self;
    [mailTextView setBackgroundColor:[UIColor clearColor]];
    mailTextView.font = [UIFont systemFontOfSize:15.0];
    mailTextView.returnKeyType = UIReturnKeyDone;
    [mailTextView becomeFirstResponder];
    mailTextView.text = @"";
    mailTextView.returnKeyType = UIReturnKeyDone;
    [self.view addSubview:mailTextView];
     */
    
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    bHasSent = NO;
    
    [self.view setBackgroundColor:[UIColor whiteColor]];
    
    [self.navigationItem setTitle:@"发邮件"];
    
    UIBarButtonItem* leftButton = [[UIBarButtonItem alloc] initWithTitle:@"返回"
                                                                   style:UIBarButtonItemStylePlain
                                                                  target:self
                                                                  action:@selector(quitMail)];
    
    UIBarButtonItem* rightButton = [[UIBarButtonItem alloc] initWithTitle:@"发送"
                                                                    style:UIBarButtonItemStylePlain
                                                                   target:self
                                                                   action:@selector(sendMail)];
    
    self.navigationItem.leftBarButtonItem = leftButton;
    self.navigationItem.rightBarButtonItem = rightButton;
    
    [leftButton release];
    [rightButton release];
    UILabel* lbToUser = nil;
   
    if ([[UIDevice currentDevice].systemVersion floatValue] < 7.0 ) {
         lbToUser = [[UILabel alloc] initWithFrame:CGRectMake(10, 0, 320, 30)];
    }else{
         lbToUser = [[UILabel alloc] initWithFrame:CGRectMake(10, ([@"iPad" isEqualToString:[[UIDevice currentDevice].model substringWithRange:NSMakeRange(0, 4)]] ? 64 : 44), 320, 30)];
    }
   
    [lbToUser setTextAlignment:UITextAlignmentLeft];
    [lbToUser setText:[NSString stringWithFormat:@"To:  %@", strUserName]];
    [self.view addSubview:lbToUser];
    [lbToUser release];
    

    
    CGSize size_screen = [[UIScreen mainScreen] bounds].size;
    if ([[UIDevice currentDevice].systemVersion floatValue] < 7.0) {
        mailTextView = [[UITextView alloc] initWithFrame:CGRectMake(10,
                                                                    25,
                                                                    size_screen.width - 20,
                                                                    size_screen.height * 0.5)];
    }else{
        mailTextView = [[UITextView alloc] initWithFrame:CGRectMake(10,
                                                                    ([@"iPad" isEqualToString:[[UIDevice currentDevice].model substringWithRange:NSMakeRange(0, 4)]] ? 89 : 69),
                                                                    size_screen.width - 20,
                                                                    size_screen.height * 0.5)];
    }
    
    mailTextView.delegate =self;
    [mailTextView setBackgroundColor:[UIColor clearColor]];
    mailTextView.font = [UIFont systemFontOfSize:15.0];
    mailTextView.returnKeyType = UIReturnKeyDone;
    [mailTextView becomeFirstResponder];
    mailTextView.text = @"";
    mailTextView.returnKeyType = UIReturnKeyDone;
    [self.view addSubview:mailTextView];
        
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)dealloc
{
    [mailTextView release];
    [strMail release];
    [strUserName release];
    [super dealloc];
}

- (void)quitMail
{
    [self setStrMail:@""];
    bHasSent = YES;
}

- (void)sendMail
{
    NSString *realText = [mailTextView text];
    realText = [realText stringByReplacingOccurrencesOfString:@"&" withString:@"%26"];
    realText = [realText stringByReplacingOccurrencesOfString:@";" withString:@"%3B"];
    [self setStrMail:realText];
    bHasSent = YES;
    // NSLog(@"send content-------------->%@", realText);
}

#pragma mark -
#pragma mark UITextViewDelegate
- (void)textViewDidChange:(UITextView *)textView {

}

-(BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    //实现 回车就 消失键盘的效果，tangaowen , 2012.2.5
    if ([text isEqualToString:@"\n"])
    {
        [textView resignFirstResponder];
        return NO;
    }
    return YES;
}

@end
