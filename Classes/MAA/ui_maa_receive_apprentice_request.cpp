#include "ui_maa_receive_apprentice_request.h"
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


#define UI_TAB_MAA_CALL_LIST    100

CUIMAAReceiveApprenticeRequest::CUIMAAReceiveApprenticeRequest()
{
}

CUIMAAReceiveApprenticeRequest::~CUIMAAReceiveApprenticeRequest()
{
}

CUIMAAReceiveApprenticeRequest * CUIMAAReceiveApprenticeRequest::create(CCObject *pBasic)
{
    if (pBasic)
    {
        if (CUIMAAReceiveApprenticeRequest *pReceiveApprenticeRequest = new CUIMAAReceiveApprenticeRequest())
        {
            pReceiveApprenticeRequest->m_pBasic = pBasic;
            return pReceiveApprenticeRequest;
        }
    }
    return NULL;
}

void CUIMAAReceiveApprenticeRequest::createBasicChild()
{
    createBackMenuItem();
    createTitleBar(CGameData::Inst()->getLanguageValue("maa_receive_apprentice"));
    createSortMenuItem();
    createNumBg(0,3);
    createCallList();
}

void CUIMAAReceiveApprenticeRequest::show()
{
    if (getIsVisible())return;
    setIsVisible(true);
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 1, true);
    createBasicChild();
    
    if (CListLayer *pListLayer = getCallList())
    {
        //test
        if (CCMutableArray<CFriendInf*> *pFriendList = CGameData::Inst()->getFriendList())
        {
            int nCount = pFriendList->count();
            for (int i = 0; i < nCount; ++i)
            {
                if (i <= pListLayer->getPreBottomShow() && i >= pListLayer->getPreTopShow())
                {
                    if (CMenuBtn *pMenuBtn = new CMenuBtn(pFriendList->getObjectAtIndex(i),enFriendRangePro))
                    {
                        pListLayer->push_back(pMenuBtn);
                    }
                }
                else
                {
                    pListLayer->push_back(NULL);
                }
            }
        }
    }
}

void CUIMAAReceiveApprenticeRequest::hide()
{
    setIsVisible(false);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
}

//Call列表
void CUIMAAReceiveApprenticeRequest::createCallList()
{
    if (!getChildByTag(UI_TAB_MAA_CALL_LIST))
    {
        if (CListLayer *pListLayer = new CListLayer(this, callfuncO_selector(CUIMAAReceiveApprenticeRequest::onTouchEndedCallList)))
        {
            pListLayer->setReloadFunc(callfuncO_selector(CUIMAAReceiveApprenticeRequest::onReloadCellList));
            addChild(pListLayer, 0, UI_TAB_MAA_CALL_LIST);
            pListLayer->updatePosition();
            pListLayer->setPosition(ccp(640,pListLayer->getPosition().y));
            pListLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(0,pListLayer->getPosition().y)),0.8f));
        }
    }
}

CListLayer * CUIMAAReceiveApprenticeRequest::getCallList()
{
    return (CListLayer*)getChildByTag(UI_TAB_MAA_CALL_LIST);
}

void CUIMAAReceiveApprenticeRequest::onTouchEndedCallList(CCObject *pSender)
{
    fa_return(pSender);
    
    //播放声音
    if(CGameData::Inst()->isSoundOn)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    }
    
//    //show color child
//    if (CCLayerColor *pLayerColor = getLayerColor())
//    {
//        pLayerColor->setIsVisible(true);
//    }

    //test
//    if (CCMutableArray<CFriendInf*> *pFriendList = CGameData::Inst()->getFriendList())
//    {
//        CListLayerRet* pRet = (CListLayerRet*)pSender;
//        CFriendInf *pFriendInf = pFriendList->getObjectAtIndex(pRet->iBtnSel);
//        if (CConfirmLayer * pConfirm = getConfirmLayer())
//        {
//            pConfirm->show(&pFriendInf->card_info, pFriendInf->name, enCConfirmlayer_maa_arm,pRet->iBtnSel);
//        }
//    }
}

void CUIMAAReceiveApprenticeRequest::onReloadCellList(CCObject *pSender)
{
    if (CListLayer *pListLayer = getCallList())
    {
        //test
        if (CCMutableArray<CFriendInf*> *pFriendList = CGameData::Inst()->getFriendList())
        {
            CListLayerRet* pRet = (CListLayerRet*)pSender;
            CFriendInf *pFriendInf = pFriendList->getObjectAtIndex(pRet->iBtnSel);
            CMenuBtn *btn = new CMenuBtn(pFriendInf,enFriendRangePro);
            pListLayer->reloadItemByIdx(btn, pRet->iBtnSel);
            btn->release();
        }
    }
}
















