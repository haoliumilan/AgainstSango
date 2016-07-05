#include "ui_maa_receive_apprentice.h"
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

CUIMAAReceiveApprentice::CUIMAAReceiveApprentice()
{
}

CUIMAAReceiveApprentice::~CUIMAAReceiveApprentice()
{
}

CUIMAAReceiveApprentice * CUIMAAReceiveApprentice::create(CCObject *pBasic)
{
    if (pBasic)
    {
        if (CUIMAAReceiveApprentice *pReceiveApprentice = new CUIMAAReceiveApprentice())
        {
            pReceiveApprentice->m_pBasic = pBasic;
            return pReceiveApprentice;
        }
    }
    return NULL;
}

void CUIMAAReceiveApprentice::createBasicChild()
{
    createBackMenuItem();
    createTitleBar(CGameData::Inst()->getLanguageValue("maa_receive_apprentice"));
    createSeekIDItem();
    createRefreshItem();
}

void CUIMAAReceiveApprentice::show()
{
    if (getIsVisible())return;
    setIsVisible(true);
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 1, true);
    createBasicChild();
}

void CUIMAAReceiveApprentice::hide()
{
    setIsVisible(false);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
}















