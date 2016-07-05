//
//  AgainstWarAppController.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-13.
//  Copyright __MyCompanyName__ 2012年. All rights reserved.
//

#import "RootViewController.h"
#import <PPAppPlatformKit/PPAppPlatformKit.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/types.h>
#include <arpa/inet.h>


@implementation RootViewController

#define GAMESERVER_PORT_TEST                 @"8000"
#define GAMESERVER_IP_TEST                   "58.218.248.218"

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
 - (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
 if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
 // Custom initialization
 }
 return self;
 }
 */

/*
 // Implement loadView to create a view hierarchy programmatically, without using a nib.
 - (void)loadView {
 }
 */


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad
{
    
    [super viewDidLoad];
}


#pragma mark ---------------------------------------------------- 登 录 -------------------------------------------------------

- (void)loginPressDown{
    [[PPAppPlatformKit sharedInstance] showLogin];
}



- (void)ppCenterPressDown{
    [[PPAppPlatformKit sharedInstance] showCenter];
}


-(void) setPrice: (int)price setBillNo: (NSString *)billNo setTitle: (NSString*)title setRoleid:(NSString*)roleid{
    [[PPAppPlatformKit sharedInstance] exchangeGoods:price BillNo:billNo BillTitle:title RoleId:roleid ZoneId:0];
}


#pragma mark    ---------------SDK CALLBACK---------------
//字符串登录成功回调【实现其中一个就可以】
- (void)ppLoginStrCallBack:(NSString *)paramStrToKenKey{
    
    //字符串token验证方式
    MSG_GAME_SERVER_STR mgs_s = {};
    mgs_s.len_str =  41;
    mgs_s.commmand_str = 0xAA000022;
    memcpy(mgs_s.token_key_str, [paramStrToKenKey UTF8String], 33);
    _token = [NSString stringWithFormat:@"%s",mgs_s.token_key_str];
    [_token retain];
    _iLoginstatus = 1;
    _iPurchaseStatus = 1;
    _iLogoutStatus = 1;
    [[PPAppPlatformKit sharedInstance] getUserInfoSecurity];
}

-(void)resetLogoutStatus
{
    _iLogoutStatus = 1;
}

-(int)getLogoutStatus
{
    return  _iLogoutStatus;
}

-(void)showPPCenter{
    [[PPAppPlatformKit sharedInstance] showCenter];
}

- (void)PPlogout{
    [[PPAppPlatformKit sharedInstance] PPlogout];
}


-(int)getPpLoginStatus{
    return _iLoginstatus;
}

-(void)resetPpLoginStatus{
    _iLoginstatus = 0;
}

-(void)resetPpPurchaseStatus{
    _iPurchaseStatus = 1;
}

-(int)getPpPurchaseStatus{
    return _iPurchaseStatus;
}

-(NSString*)getPptoken{
    return _token;
}

-(int)getPpCurrentUserId{
    return [[PPAppPlatformKit sharedInstance] currentUserId];
}



//关闭客户端页面回调方法
-(void)ppClosePageViewCallBack:(PPPageCode)paramPPPageCode{
    //可根据关闭的VIEW页面做你需要的业务处理
    NSLog(@"当前关闭的VIEW页面回调是1%d", paramPPPageCode);
    switch((int)paramPPPageCode)
    {
        case PPLoginViewPageCode:
        case PPRegisterViewPageCode:
            _iLoginstatus = -1;
            break;
    }
}



//关闭WEB页面回调方法
- (void)ppCloseWebViewCallBack:(PPWebViewCode)paramPPWebViewCode{
    //可根据关闭的WEB页面做你需要的业务处理
    NSLog(@"当前关闭的WEB页面回调是2%d", paramPPWebViewCode);
}

//注销回调方法
- (void)ppLogOffCallBack{
    //[[PPAppPlatformKit sharedInstance] showLogin];
    _iLogoutStatus = 2;
}

//兑换回调接口【只有兑换会执行此回调】
- (void)ppPayResultCallBack:(PPPayResultCode)paramPPPayResultCode{
    NSLog(@"兑换回调返回编码%d",paramPPPayResultCode);
    //回调购买成功。其余都是失败
    if(paramPPPayResultCode == PPPayResultCodeSucceed){
        //购买成功发放道具
        _iPurchaseStatus = 2;
    }else{
        _iPurchaseStatus = -1;
    }
}

-(void)ppVerifyingUpdatePassCallBack{
    NSLog(@"ppVerifyingUpdatePassCallBack");
}



// Override to allow orientations other than the default landscape orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
	return FALSE;
    
    // switch to this line if you want to set portrait view
    // return UIInterfaceOrientationIsPortrait( interfaceOrientation );
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}


@end
