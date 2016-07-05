//
//  CAlipay.m
//  AgainstWar
//
//  Created by hejian on 13-10-16.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#import "CAlipay.h"

#import "AlixPayOrder.h"
#import "AlixPayResult.h"
#import "AlixPay.h"
#import "DataSigner.h"
#import "PartnerConfig.h"

#import <sys/utsname.h>
#import "DataVerifier.h"
//#import "CPayMM.h"

@implementation CAlipay

- (id)init
{
    if(self == [super init])
    {
        
    }
    return self;
}

- (void)purAlipay:(const char*) tradeNo withTitle:(const char*) title withPrice:(int) price withIsTruePrice:(BOOL) isTruePrice
{
    NSString *partner = PartnerID;
    NSString *seller = SellerID;
	
	//partner和seller获取失败,提示
	if ([partner length] == 0 || [seller length] == 0)
	{
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"提示"
														message:@"缺少partner或者seller。"
													   delegate:self
											  cancelButtonTitle:@"确定"
											  otherButtonTitles:nil];
		[alert show];
		[alert release];
		return;
	}
	
	/*
	 *生成订单信息及签名
	 *由于demo的局限性，本demo中的公私钥存放在AlixPayDemo-Info.plist中,外部商户可以存放在服务端或本地其他地方。
	 */
	//将商品信息赋予AlixPayOrder的成员变量
	AlixPayOrder *order = [[AlixPayOrder alloc] init];
	order.partner = partner;
	order.seller = seller;
	order.tradeNO = [[NSString alloc] initWithCString:tradeNo encoding:NSUTF8StringEncoding]; //订单ID（由商家自行制定）
	order.productName = [[NSString alloc] initWithCString:title encoding:NSUTF8StringEncoding]; //商品标题
	order.productDescription = @"神一样的1分钱"; //商品描述
    if (isTruePrice) {
        order.amount = [NSString stringWithFormat:@"%d",price]; //商品真实价格
        order.notifyURL =  @"http://ocpay.negaplay.com/charge_result/"; //回调URL
    }else {
        order.amount = [NSString stringWithFormat:@"%.2f",0.01]; //商品假价格
        order.notifyURL =  @"http://42.121.15.153:9139/charge_result/"; //回调URL
    }
	
	//应用注册scheme,在AlixPayDemo-Info.plist定义URL types,用于快捷支付成功后重新唤起商户应用
	NSString *appScheme = @"alipay5e2fd0219cc58026";
	
	//将商品信息拼接成字符串
	NSString *orderSpec = [order description];
	NSLog(@"orderSpec = %@",orderSpec);
	
	//获取私钥并将商户信息签名,外部商户可以根据情况存放私钥和签名,只需要遵循RSA签名规范,并将签名字符串base64编码和UrlEncode
	id<DataSigner> signer = CreateRSADataSigner(PartnerPrivKey);
	NSString *signedString = [signer signString:orderSpec];
	
	//将签名成功字符串格式化为订单字符串,请严格按照该格式
	NSString *orderString = nil;
	if (signedString != nil) {
		orderString = [NSString stringWithFormat:@"%@&sign=\"%@\"&sign_type=\"%@\"",
                       orderSpec, signedString, @"RSA"];
        
        //获取快捷支付单例并调用快捷支付接口
        AlixPay * alixpay = [AlixPay shared];
        int ret = [alixpay pay:orderString applicationScheme:appScheme];
        
        if (ret == kSPErrorAlipayClientNotInstalled) {
            UIAlertView * alertView = [[UIAlertView alloc] initWithTitle:@"提示"
                                                                 message:@"您还没有安装支付宝快捷支付，请先安装。"
                                                                delegate:self
                                                       cancelButtonTitle:@"确定"
                                                       otherButtonTitles:nil];
            [alertView setTag:123];
            [alertView show];
            [alertView release];
        }
        else if (ret == kSPErrorSignError) {
            NSLog(@"签名错误！");
        }
        
	}
    
}

- (int)parseURL:(NSURL *)url application:(UIApplication *)application {
    alipayStatus = -1;
	AlixPay *alixpay = [AlixPay shared];
	AlixPayResult *result = [alixpay handleOpenURL:url];
	if (result) {
		//是否支付成功
		if (9000 == result.statusCode) {
			/*
			 *用公钥验证签名
			 */
			id<DataVerifier> verifier = CreateRSADataVerifier(AlipayPubKey);
			if ([verifier verifyString:result.resultString withSign:result.signString]) {
                //CPayMM::Inst()->alipayBack(0);
                alipayStatus=0;
			}//验签错误
			else {
				alipayStatus=-1;
			}
		}
		//如果支付失败,可以通过result.statusCode查询错误码
		else {
            alipayStatus=-1;
		}
		
	}
    return alipayStatus;
}


- (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex {
	if (alertView.tag == 123) {
		NSString * URLString = @"http://itunes.apple.com/cn/app/id535715926?mt=8";
		[[UIApplication sharedApplication] openURL:[NSURL URLWithString:URLString]];
	}
}

- (void)endAlipay
{
    
}
@end
