#ifndef __ui_master_receive_apprentice__
#define __ui_master_receive_apprentice__

#include "cocos2d.h"
USING_NS_CC;

class CUMasterReceiveApprentice : public cocos2d::CCLayer
{
public:
    CUMasterReceiveApprentice();
    virtual ~CUMasterReceiveApprentice();
    LAYER_NODE_FUNC(CUMasterReceiveApprentice);
    void show();
    void hide();
    
private:
    void createBasicChild();
    
};

#endif
