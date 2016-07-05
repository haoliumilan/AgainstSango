//
//  CPayMM.h
//  AgainstWar
//
//  Created by Zhang Hao on 12-12-7.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CPayMM_h
#define AgainstWar_CPayMM_h

class CPayMM {
    static  CPayMM*  s_pcIf;

public:
	//Get the instance pointer
	static CPayMM* Inst(){
        if(s_pcIf == 0){
            s_pcIf = new CPayMM();
        }
        
        return s_pcIf;
    }
    
    void loadStore();
    void purchase(int idx);
    int  getStoreStatus();
    void resetStoreStatus();
    int  getPurchaseStatus();
    void resetPurchaseStatus();
    void finishPayment();
    const char* getReceipt();
    const char* getTransId();
    void addProduct(const char* szIapId,bool isAlipay);
    void rmvProduct(const char* szIapId);
    int  getProductCnt();
    const char* getIapIdAtIdx(int idx);
    const char* getPriceAtIdx(int idx);
    const char* getTitleAtIdx(int idx);
    
    void purAlipay(const char* tradeNo,const char* title,int price,bool isTruePrice);
    int getIntPriceAtIdx(int idx);
    
private:
    //forbid constructor being called outside
	CPayMM();
    
	//forbid copy constructor being called outside
	CPayMM(const CPayMM& );
	
	//forbid assignment
	CPayMM& operator=(const CPayMM& );

};

#endif
