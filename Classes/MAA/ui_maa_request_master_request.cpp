#include "ui_maa_request_master_request.h"
#include "CGameData.h"
#include "MainScene.h"
#include "TitleBar.h"
#include "FriendMain.h"
//#include "CListLayer.h"
#include "CMenuBtn.h"
#include "SimpleAudioEngine.h"
#include "CConfirmLayer.h"
#include "OcProxy.h"
#include "CShowCardInf.h"
#include "ui_seek_user_id.h"

CUIMAARequestMasterRequest::CUIMAARequestMasterRequest()
{
}

CUIMAARequestMasterRequest::~CUIMAARequestMasterRequest()
{
}

CUIMAARequestMasterRequest * CUIMAARequestMasterRequest::create(CCObject *pBasic)
{
    if (pBasic)
    {
        if (CUIMAARequestMasterRequest *pRequestMasterRequest = new CUIMAARequestMasterRequest())
        {
            pRequestMasterRequest->m_pBasic = pBasic;
            return pRequestMasterRequest;
        }
    }
    return NULL;
}

void CUIMAARequestMasterRequest::createBasicChild()
{
    createBackMenuItem();
    createTitleBar(CGameData::Inst()->getLanguageValue("maa_request_master_request"));
    createSortMenuItem();
    createNumBg(0,3);
    
}

void CUIMAARequestMasterRequest::show()
{
    if (getIsVisible())return;
    setIsVisible(true);
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 1, true);
    createBasicChild();
}

void CUIMAARequestMasterRequest::hide()
{
    setIsVisible(false);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
}















