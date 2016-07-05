#ifndef __ui_maa_master_manage__
#define __ui_maa_master_manage__
#include "ui_maa_basic.h"

//师傅管理
class CUIMAAMasterManage : public CUIMAABasic
{
private:
    CUIMAAMasterManage();
    virtual ~CUIMAAMasterManage();
    
public:
    static CUIMAAMasterManage * create(CCObject *pBasic);
    
    virtual void show();
    virtual void hide();
    
private:
    void createBasicChild();
};

#endif
