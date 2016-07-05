#include "ui_maa_apprentice_manage.h"
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

CUIMAAApprenticeManage::CUIMAAApprenticeManage()
{
}

CUIMAAApprenticeManage::~CUIMAAApprenticeManage()
{
}

CUIMAAApprenticeManage * CUIMAAApprenticeManage::create(CCObject *pBasic)
{
    if (pBasic)
    {
        if (CUIMAAApprenticeManage *pApprenticeManage = new CUIMAAApprenticeManage())
        {
            pApprenticeManage->m_pBasic = pBasic;
            return pApprenticeManage;
        }
    }
    return NULL;
}

void CUIMAAApprenticeManage::createBasicChild()
{
    createBackMenuItem();
    createTitleBar(CGameData::Inst()->getLanguageValue("maa_apprentice_manage"));
    createSortMenuItem();
    createNumBg(0,3);    
}

void CUIMAAApprenticeManage::show()
{
    if (getIsVisible())return;
    setIsVisible(true);
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 1, true);
    createBasicChild();
}

void CUIMAAApprenticeManage::hide()
{
    setIsVisible(false);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
}















