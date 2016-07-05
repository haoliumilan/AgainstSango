/****************************************************************************
 Copyright (c) 2010-2011 cocos2d-x.org
 Copyright (c) 2010      Ricardo Quesada
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import <PPAppPlatformKit/PPAppPlatformKit.h>


@interface RootViewController : UIViewController<PPAppPlatformKitDelegate> {
    
    int _iLoginstatus;  //登陆状态 0:初始值 1:登陆成功 －1:登陆失败
    int _iPurchaseStatus;  //购买状态
    int _iLogoutStatus;  // 1没有注销，2注销
    NSString *_token;
    
}

-(void)resetLogoutStatus;
-(int)getLogoutStatus;
-(void)showPPCenter;
-(int)getPpLoginStatus;
-(int)getPpPurchaseStatus; //1:正在购买中 -1:购买失败 2:购买成功
-(void)resetPpLoginStatus; //重置登陆状态
-(void)resetPpPurchaseStatus; //重置支付状态
-(int)getPpCurrentUserId;
-(NSString*)getPptoken;
- (void)loginPressDown;
- (void)PPlogout; //注销
-(void) setPrice: (int)price setBillNo: (NSString *)billNo setTitle: (NSString*)title setRoleid:(NSString*)roleid;


@end
