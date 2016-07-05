//
//  CPayment.m
//  AgainstWar
//
//  Created by Hao Zhang on 12-01-05.
//  Copyright 2011 zhanghao. All rights reserved.
//

#import "CPayment.h"
#import "CommonDef.h"

@implementation ProductInf

@synthesize strTitle;
@synthesize strIapId;
@synthesize strPrice;
@synthesize iPrice;

- (void)dealloc
{
    [strTitle release];
    [strIapId release];
    [strPrice release];
    [super dealloc];
}

@end





@implementation IapManager

@synthesize strData;
@synthesize strTransId;
@synthesize storeStatus;
@synthesize purchaseStatus;

//******************************************************************************
// init
//******************************************************************************
- (id)init
{
    if(self = [super init]){    
        storeStatus = 0;
        purchaseStatus = 0;
        strData = nil;
        strTransId = nil;
        
        arrProduct = [[NSMutableArray alloc] init];
        [arrProduct removeAllObjects];
        
        arrSpecial = [[NSArray alloc] initWithObjects:[NSString stringWithUTF8String:kProductId00],
                      [NSString stringWithUTF8String:kProductId12],
                      [NSString stringWithUTF8String:kProductId13],
                      [NSString stringWithUTF8String:kProductId09],
                      [NSString stringWithUTF8String:kProductId11],
                      nil];
    }
    
    return self;
}

//******************************************************************************
// dealloc
//******************************************************************************
- (void)dealloc
{
    [arrSpecial release];
    [arrProduct release];
    
    [super dealloc];
}

//******************************************************************************
// encode
//******************************************************************************
- (NSString *)encode:(const uint8_t *)input length:(NSInteger)length {
    static char table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
    
    NSMutableData *data = [NSMutableData dataWithLength:((length + 2) / 3) * 4];
    uint8_t *output = (uint8_t *)data.mutableBytes;
    
    for (NSInteger i = 0; i < length; i += 3) {
        NSInteger value = 0;
        for (NSInteger j = i; j < (i + 3); j++) {
            value <<= 8;
            
            if (j < length) {
                value |= (0xFF & input[j]);
            }
        }
        
        NSInteger index = (i / 3) * 4;
        output[index + 0] = table[(value >> 18) & 0x3F];
        output[index + 1] = table[(value >> 12) & 0x3F];
        output[index + 2] = (i + 1) < length ? table[(value >> 6) & 0x3F] : '=';
        output[index + 3] = (i + 2) < length ? table[(value >> 0) & 0x3F] : '=';
    }
    
    return [[[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding] autorelease];
}

//******************************************************************************
// addProduct
//******************************************************************************
- (void) addProduct: (const char*) szIapId isAlipay:(bool)isAlipay
{
    for(int i = 0; i < arrProduct.count; i++){
        ProductInf * prodInf = [arrProduct objectAtIndex:i];
        
        if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:szIapId ]]){
            return;
        }
    }
    
    ProductInf * prodInf = [[ProductInf alloc] init];
    [prodInf setStrIapId:[NSString stringWithUTF8String:szIapId ]];
    
    if (isAlipay) {
        if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:kProductId00]]){
            [prodInf setStrTitle:@"30个元宝"];
            [prodInf setStrPrice:@"￥6"];
            [prodInf setIPrice:6];
        }else if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:kProductId01]]){
            [prodInf setStrTitle:@"6个元宝"];
            [prodInf setStrPrice:@"￥6"];
            [prodInf setIPrice:6];
        }else if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:kProductId02]]){
            [prodInf setStrTitle:@"32个元宝"];
            [prodInf setStrPrice:@"￥30"];
            [prodInf setIPrice:30];
        }else if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:kProductId03]]){
            [prodInf setStrTitle:@"75个元宝"];
            [prodInf setStrPrice:@"￥68"];
            [prodInf setIPrice:68];
        }else if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:kProductId04]]){
            [prodInf setStrTitle:@"230个元宝"];
            [prodInf setStrPrice:@"￥198"];
            [prodInf setIPrice:198];
        }else if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:kProductId05]]){
            [prodInf setStrTitle:@"395个元宝"];
            [prodInf setStrPrice:@"￥328"];
            [prodInf setIPrice:328];
        }else if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:kProductId06]]){
            [prodInf setStrTitle:@"810个元宝"];
            [prodInf setStrPrice:@"￥648"];
            [prodInf setIPrice:648];
        }else if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:kProductId08]]){
            [prodInf setStrTitle:@"300个元宝"];
            [prodInf setStrPrice:@"￥248"];
            [prodInf setIPrice:248];
        }else if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:kProductId09]]){
            [prodInf setStrTitle:@"财源滚滚大礼包"];
            [prodInf setStrPrice:@"￥198"];
            [prodInf setIPrice:198];
        }else if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:kProductId11]]){
            [prodInf setStrTitle:@"招财进宝大礼包"];
            [prodInf setStrPrice:@"￥30"];
            [prodInf setIPrice:30];
        }else if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:kProductId12]]){
            [prodInf setStrTitle:@"猛将无双大礼包"];
            [prodInf setStrPrice:@"￥30"];
            [prodInf setIPrice:30];
        }else if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:kProductId13]]){
            [prodInf setStrTitle:@"乱世神将大礼包"];
            [prodInf setStrPrice:@"￥198"];
            [prodInf setIPrice:198];
        }
    }
    
    int iStartPos = 0;
    BOOL bSpecialIap = false;
    
    NSString* pStrIap = NULL;
    for (int i = 0; i < arrSpecial.count; i++) {
        if (iStartPos >= arrProduct.count)
        {
            break;
        }
        
        pStrIap = [arrSpecial objectAtIndex:i];
        if ([pStrIap isEqualToString:[prodInf strIapId]])
        {
            [arrProduct insertObject:prodInf atIndex:iStartPos];
            bSpecialIap = true;
        }
        else
        {
            if ([pStrIap isEqualToString:[[arrProduct objectAtIndex:iStartPos] strIapId]])
            {
                iStartPos++;
            }
        }
    }
    
    if (bSpecialIap == false)
    {
        [arrProduct addObject:prodInf];
    }
}

//******************************************************************************
// rmvProduct
//******************************************************************************
- (void) rmvProduct: (const char*) szIapId
{
    for(int i = 0; i < arrProduct.count; i++){
        ProductInf * prodInf = [arrProduct objectAtIndex:i];
        
        if([[prodInf strIapId] isEqualToString:[NSString stringWithUTF8String:szIapId ]]){
            [arrProduct removeObjectAtIndex:i];
            break;
        }
    }
}

//******************************************************************************
// getProductCnt
//******************************************************************************
- (int) getProductCnt
{
    return arrProduct.count;
}

//******************************************************************************
// getIapIdAtIdx
//******************************************************************************
- (const char*) getIapIdAtIdx: (int) idx
{
    const char* pIapid = NULL;
    
    if((idx >= 0) && (idx < arrProduct.count)){
        pIapid = [[[arrProduct objectAtIndex:idx] strIapId] UTF8String];
    }
    
    return pIapid;
}

//******************************************************************************
// getPriceAtIdx
//******************************************************************************
- (const char*) getPriceAtIdx: (int) idx
{
    const char* pPrice = NULL;
    
    if((idx >= 0) && (idx < arrProduct.count)){
        pPrice = [[[arrProduct objectAtIndex:idx] strPrice] UTF8String];
    }
    
    return pPrice;
}

//******************************************************************************
// getTitleAtIdx
//******************************************************************************
- (const char*) getTitleAtIdx: (int) idx
{
    const char* pTitle = NULL;
    
    if((idx >= 0) && (idx < arrProduct.count)){
        pTitle = [[[arrProduct objectAtIndex:idx] strTitle] UTF8String];
    }
    
    return pTitle;
}

- (int) getIntPriceAtIdx: (int) idx
{
    int pPrice = 0;
    
    if((idx >= 0) && (idx < arrProduct.count)){
        pPrice = [[arrProduct objectAtIndex:idx] iPrice];
    }
    
    return pPrice;
}

//******************************************************************************
// resetPurchaseStatus
//******************************************************************************
- (void)resetPurchaseStatus
{
    purchaseStatus = 0;
}

//******************************************************************************
// resetStoreStatus
//******************************************************************************
- (void)resetStoreStatus
{
    storeStatus = 0;
}

//******************************************************************************
// request product description
//******************************************************************************
- (void)requestProductInf 
{
   NSMutableSet *productIdentifiers=[[NSMutableSet alloc]init];

    if (arrProduct.count > 0) {
        for (int i=0; i < arrProduct.count; i++) {
            [productIdentifiers addObject: [[arrProduct objectAtIndex:i] strIapId]];
        }
        
        productsRequest = [[SKProductsRequest alloc] initWithProductIdentifiers:productIdentifiers];
        productsRequest.delegate = self;
        [productsRequest start];
        [productIdentifiers release];
    }
    else{
        storeStatus = -1;
    }
}

//******************************************************************************
// sortByPriceDown
//******************************************************************************
- (void) sortByPriceDown
{
    if(arrProduct.count == 0)
        return;
    
    int iStartSortIdx = 0;
    // 对大礼包、月卡顺序特殊处理
    NSString* pSpecialStr = NULL;
    NSString* pIapStr = NULL;
    for (int j = 0; j < arrSpecial.count; j++) {
        pSpecialStr = [arrSpecial objectAtIndex:j];
        
        for (int k = iStartSortIdx; k < arrProduct.count; k++) {
            pIapStr = [[arrProduct objectAtIndex:k] strIapId];
            if ([pIapStr isEqualToString:pSpecialStr])
            {
                [arrProduct exchangeObjectAtIndex:iStartSortIdx withObjectAtIndex:k];
                iStartSortIdx++;
                break;
            }
        }
    }
    
    // 普通的iap降序排列
    for (int i = iStartSortIdx; i < arrProduct.count-1; i++) {
        for(int j = i+1; j < arrProduct.count; j++){
            int priceA = [[arrProduct objectAtIndex:i] iPrice];
            int priceB = [[arrProduct objectAtIndex:j] iPrice];
            
            if(priceB > priceA)
                [arrProduct exchangeObjectAtIndex:i withObjectAtIndex:j];
        }
    }
}


//******************************************************************************
// receive product description
//******************************************************************************
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
    NSArray *products = response.products;

    int total = [products count];
    
    if(total == 0){
        storeStatus = -1;  //-1: fail
//        NSLog(@"load store failed");
    }
    else {
        storeStatus = 2;  //2: success
    }
    
    [arrProduct removeAllObjects];

    for (unsigned int i = 0; i < total; i++) {
        SKProduct *product = [products objectAtIndex:i];

//        NSLog(@"Product title: %@" , product.localizedTitle);
//        NSLog(@"Product description: %@" , product.localizedDescription);
//        NSLog(@"Product price: %@" , product.price);
//        NSLog(@"Product id: %@" , product.productIdentifier);

        NSNumberFormatter *numberFormatter = [[NSNumberFormatter alloc] init];
        [numberFormatter setFormatterBehavior:NSNumberFormatterBehavior10_4];
        [numberFormatter setNumberStyle:NSNumberFormatterCurrencyStyle];
        [numberFormatter setLocale:product.priceLocale];
        NSString *formattedPrice = [numberFormatter stringFromNumber:product.price];
//        NSLog(@"Product formattedPrice: %@" , formattedPrice);

        NSRange rg = [formattedPrice rangeOfString:@".00"];
        if (rg.length > 0) {
            formattedPrice = [formattedPrice substringToIndex:rg.location];
        }
        
        ProductInf * prodInf = [[ProductInf alloc] init];
        [prodInf setStrIapId:product.productIdentifier];
        [prodInf setStrPrice:formattedPrice];
        [prodInf setStrTitle:product.localizedTitle];
        [prodInf setIPrice:product.price.intValue];
        
        [arrProduct addObject:prodInf];
        
        [numberFormatter release];
    }

    // finally release the reqest we alloc/init’ed in requestProUpgradeProductData
    [productsRequest release];
    
    [self sortByPriceDown];
}

//******************************************************************************
// init (add observer, request product description)
//******************************************************************************
- (void)loadStore
{
    if(storeStatus == 1)
        return;
    
//    NSLog(@"loadStore...");
          
    storeStatus = 1;  //1: loading
    
    strData = nil;
    strTransId = nil;
    payTransaction=nil;
    
    // restarts any purchases if they were interrupted last time the app was open
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];

    // get the product description (defined in early sections)
    [self requestProductInf];
}


//******************************************************************************
// canMakePurchases
//******************************************************************************
- (BOOL)canMakePurchases
{
    return [SKPaymentQueue canMakePayments];
}


//******************************************************************************
// purchase
//******************************************************************************
- (void)purchase :(int )index
{
    if(![self canMakePurchases]){
        purchaseStatus = -1;
        return;
    }
    
    if((index >= 0) && (index < arrProduct.count)){
        SKPayment *payment = [SKPayment paymentWithProductIdentifier:[[arrProduct objectAtIndex:index] strIapId]];
        
        [[SKPaymentQueue defaultQueue] addPayment:payment];
        
        purchaseStatus = 1;
    }
    else{
        purchaseStatus = -1;
    }
}


//******************************************************************************
// provideContent
//******************************************************************************
- (void)provideContent:(NSString *)productId
{

}


//******************************************************************************
// finishTransaction
//******************************************************************************
- (void)finishTransaction:(SKPaymentTransaction *)transaction wasSuccessful:(BOOL)wasSuccessful
{
    // remove the transaction from the payment queue.
    if (wasSuccessful) {
        payTransaction=transaction;
        
        //通常付费的情况
        if(purchaseStatus == 1){
            purchaseStatus = 2;
        }
        //启动时有未完成的付费的情况
        else{
            purchaseStatus = 10;
        }
    }
    else
    {
        [[SKPaymentQueue defaultQueue] finishTransaction:transaction];
        purchaseStatus = -1;
    }
}


//******************************************************************************
// completeTransaction
//******************************************************************************
- (void)completeTransaction:(SKPaymentTransaction *)transaction
{
    NSData* data =  [transaction transactionReceipt];
//    NSLog(@"%@", [transaction transactionIdentifier]);
    if(strData){
        [strData release];
        strData = nil;
    }
    
    if(strTransId){
        [strTransId release];
        strTransId = nil;
    }
    
    strData = [self encode:(uint8_t *)data.bytes length:data.length];
    if(strData)
        [strData retain];
    
    strTransId = [transaction transactionIdentifier];
    if(strTransId)
        [strTransId retain];
    
    [self provideContent:transaction.payment.productIdentifier];

    [self finishTransaction:transaction wasSuccessful:YES];
}


//******************************************************************************
// restoreTransaction
//******************************************************************************
- (void)restoreTransaction:(SKPaymentTransaction *)transaction
{
    [self provideContent:transaction.originalTransaction.payment.productIdentifier];
    [self finishTransaction:transaction wasSuccessful:YES];
}


//******************************************************************************
// failedTransaction
//******************************************************************************
- (void)failedTransaction:(SKPaymentTransaction *)transaction
{
    [self finishTransaction:transaction wasSuccessful:NO];
}


//******************************************************************************
// failedTransaction
//******************************************************************************
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
//                NSLog(@"oooooo Purchased");
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
//                NSLog(@"oooooo PurchaseFailed %d", transaction.error.code);
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
//                NSLog(@"oooooo PurchaseRestored");
                [self restoreTransaction:transaction];
                break;
            default:
//                NSLog(@"oooooo transactionState: %d" , transaction.transactionState);
                break;
        }
    }
}

- (void)finishPayment
{
    if(payTransaction!=nil)
    {
        [[SKPaymentQueue defaultQueue] finishTransaction:payTransaction];
        payTransaction=nil;
        strData = nil;
    }
}





@end