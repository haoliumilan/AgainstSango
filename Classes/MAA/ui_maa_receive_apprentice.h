#ifndef __ui_maa_receive_apprentice__
#define __ui_maa_receive_apprentice__
#include "ui_maa_basic.h"

//收 徒
class CUIMAAReceiveApprentice : public CUIMAABasic
{
private:
    CUIMAAReceiveApprentice();
    virtual ~CUIMAAReceiveApprentice();
    
public:
    static CUIMAAReceiveApprentice * create(CCObject *pBasic);
    
    virtual void show();
    virtual void hide();
    
private:
    void createBasicChild();
};

#endif
