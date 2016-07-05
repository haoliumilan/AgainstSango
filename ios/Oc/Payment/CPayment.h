//
//  CPayment.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-01-05.
//  Copyright 2011 zhanghao. All rights reserved.
//

#ifndef _CPAYMENT_H_
#define _CPAYMENT_H_

#import <StoreKit/StoreKit.h>

@interface ProductInf : NSObject
{
    NSString* strTitle;
    NSString* strIapId;
    NSString* strPrice;
    int       iPrice;
}

@property (nonatomic, readwrite, retain) NSString* strTitle;
@property (nonatomic, readwrite, retain) NSString* strIapId;
@property (nonatomic, readwrite, retain) NSString* strPrice;
@property (nonatomic, readwrite) int iPrice;

@end

@interface IapManager : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
    SKProductsRequest *productsRequest;
    
    NSString* strData;
    NSString* strTransId;

    SKPaymentTransaction *payTransaction;
    
    NSMutableArray* arrProduct;
    NSArray*        arrSpecial;

    int storeStatus;
    int purchaseStatus;
}

- (void) addProduct: (const char*) szIapId  isAlipay: (bool) isAlipay;
- (void) rmvProduct: (const char*) szIapId;
- (int) getProductCnt;
- (const char*) getIapIdAtIdx: (int) idx;
- (const char*) getPriceAtIdx: (int) idx;
- (const char*) getTitleAtIdx: (int) idx;
- (int) getIntPriceAtIdx: (int) idx;
- (void)finishPayment;
- (void)requestProductInf;
- (void)loadStore;
- (BOOL)canMakePurchases;
- (void)purchase :(int )index;
- (void)resetPurchaseStatus;
- (void)resetStoreStatus;

@property (nonatomic, readonly)NSString* strData;
@property (nonatomic, readonly)NSString* strTransId;
@property (nonatomic, readonly)int storeStatus;
@property (nonatomic, readonly)int purchaseStatus;

@end

#endif
