#ifndef __ui_maa_apprentice_manage__
#define __ui_maa_apprentice_manage__
#include "ui_maa_basic.h"

//徒弟管理
class CUIMAAApprenticeManage : public CUIMAABasic
{
private:
    CUIMAAApprenticeManage();
    virtual ~CUIMAAApprenticeManage();
    
public:
    static CUIMAAApprenticeManage * create(CCObject *pBasic);
    
    virtual void show();
    virtual void hide();
    
private:
    void createBasicChild();
};

#endif
