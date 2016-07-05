//
//  CPayMM.cpp
//  AgainstWar
//
//  Created by Zhang Hao on 12-12-7.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CPayMM.h"
#include "CPayment.h"

#include "CAlipay.h"
static CAlipay *s_alipay = NULL;

CPayMM* CPayMM::s_pcIf = NULL;

static IapManager* s_pIapManager = nil;


CPayMM::CPayMM()
{
    s_pIapManager = [[IapManager alloc] init];
}

void CPayMM::purAlipay(const char* tradeNo,const char* title,int price,bool isTruePrice)
{
    if(s_alipay == NULL)
    {
        s_alipay =  [[CAlipay alloc] init];
    }
    [s_alipay purAlipay:tradeNo withTitle:title withPrice:price withIsTruePrice:isTruePrice];
}

int CPayMM::getIntPriceAtIdx(int idx)
{
    int szTitle = [s_pIapManager getIntPriceAtIdx:idx];
    return szTitle;
}

void CPayMM::loadStore()
{
    [s_pIapManager loadStore];
}

void CPayMM::purchase(int idx)
{
    [s_pIapManager purchase:idx];
}

int  CPayMM::getStoreStatus()
{
    int iStoreStatus = [s_pIapManager storeStatus];
    return iStoreStatus;

}

void CPayMM::resetStoreStatus()
{
    [s_pIapManager resetStoreStatus];
}

int  CPayMM::getPurchaseStatus()
{
    int iPurchaseStatus = [s_pIapManager purchaseStatus];
    return iPurchaseStatus;
}

void CPayMM::resetPurchaseStatus()
{
    [s_pIapManager resetPurchaseStatus];
}

void CPayMM::finishPayment()
{
    [s_pIapManager finishPayment];
}

const char* CPayMM::getReceipt()
{
    const char* szReceipt = [[s_pIapManager strData] UTF8String];
    return szReceipt;
}

const char* CPayMM::getTransId()
{
    const char* szTransId = [[s_pIapManager strTransId] UTF8String];
    return szTransId;
}

void CPayMM::addProduct(const char* szIapId,bool isAlipay)
{
    [s_pIapManager addProduct:szIapId isAlipay:isAlipay];
}

void CPayMM::rmvProduct(const char* szIapId)
{
    [s_pIapManager rmvProduct:szIapId];
}

int  CPayMM::getProductCnt()
{
    int iCnt = [s_pIapManager getProductCnt];
    return iCnt;
}

const char* CPayMM::getIapIdAtIdx(int idx)
{
    const char* szIapid = [s_pIapManager getIapIdAtIdx:idx];
    return szIapid;
}

const char* CPayMM::getPriceAtIdx(int idx)
{
    const char* szPrice = [s_pIapManager getPriceAtIdx:idx];
    return szPrice;
}

const char* CPayMM::getTitleAtIdx(int idx)
{
    const char* szTitle = [s_pIapManager getTitleAtIdx:idx];
    return szTitle;
}
