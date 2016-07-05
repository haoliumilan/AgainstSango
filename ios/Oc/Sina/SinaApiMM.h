//
//  SinaApiMM.h
//  AgainstWar
//
//  Created by Zhang Hao on 12-12-7.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__SinaApiMM__
#define __AgainstWar__SinaApiMM__

class SinaApiMM {
    static  SinaApiMM*  s_pcIf;
    
public:
	//Get the instance pointer
	static SinaApiMM* Inst(){
        if(s_pcIf == 0){
            s_pcIf = new SinaApiMM();
        }
        
        return s_pcIf;
    }
    
    void Login();
    int getLoginStatus();
    const char* getOpenId();
    const char* getAccessToken();
    void sendWeibo(const char *str ,bool isImageExist);
    void setIsGachaWeibo(bool isOk);
    void setLoginStatus(int status);
private:
    //forbid constructor being called outside
	SinaApiMM();
    
	//forbid copy constructor being called outside
	SinaApiMM(const SinaApiMM& );
	
	//forbid assignment
	SinaApiMM& operator=(const SinaApiMM& );
    

    
};


#endif /* defined(__AgainstWar__SinaApiMM__) */
