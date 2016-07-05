//
//  ios_sheet.h
//  Cocos2d_ios
//
//  Created by user on 12-11-17.
//
//


#import <UIKit/UIKit.h>
#import "WXApi.h"



@interface ios_sheet :  UIViewController<UIApplicationDelegate,UIActionSheetDelegate,WXApiDelegate>
{
    UIActionSheet *mySheet;
    int wxtype;
    bool sinaft;
    bool qqft;
    bool weixinft;
    bool invitation_code;
   
}


-(void) CopyCode;
- (void) boxSheet:(NSInteger) type;
-(NSString*)RerutnNSstringutf8:(const char*)  string;
-(void) setWxTpe:(NSInteger)type;

@end












