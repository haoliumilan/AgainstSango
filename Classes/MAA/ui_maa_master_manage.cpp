#include "ui_maa_master_manage.h"
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

CUIMAAMasterManage::CUIMAAMasterManage()
{
}

CUIMAAMasterManage::~CUIMAAMasterManage()
{
}

CUIMAAMasterManage * CUIMAAMasterManage::create(CCObject *pBasic)
{
    if (pBasic)
    {
        if (CUIMAAMasterManage *pMasterManage = new CUIMAAMasterManage())
        {
            pMasterManage->m_pBasic = pBasic;
            return pMasterManage;
        }
    }
    return NULL;
}

void CUIMAAMasterManage::createBasicChild()
{
    createBackMenuItem();
    createTitleBar(CGameData::Inst()->getLanguageValue("maa_master_manage"));
    createSortMenuItem();
    createSeekIDItem();
    createRefreshItem();
}

void CUIMAAMasterManage::show()
{
    if (getIsVisible())return;
    setIsVisible(true);
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 1, true);
    createBasicChild();
}

void CUIMAAMasterManage::hide()
{
    setIsVisible(false);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
}















