#ifndef __ui_maa_request_master_request__
#define __ui_maa_request_master_request__
#include "ui_maa_basic.h"

//拜师请求
class CUIMAARequestMasterRequest : public CUIMAABasic
{
private:
    CUIMAARequestMasterRequest();
    virtual ~CUIMAARequestMasterRequest();
    
public:
    static CUIMAARequestMasterRequest * create(CCObject *pBasic);
    
    virtual void show();
    virtual void hide();
    
private:
    void createBasicChild();
};

#endif
