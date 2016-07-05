#include "ui_maa_request_master.h"
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

using namespace CocosDenshion;

#define UI_TAB_MAA_ARM_LIST     100
#define UI_TAB_MAA_CONFIRM      200
#define UI_TAB_MAA_COLOR        300
#define UI_TAB_MAA_CARD_INFO    400


CUIMAARequestMaster::CUIMAARequestMaster()
{
}

CUIMAARequestMaster::~CUIMAARequestMaster()
{
}

CUIMAARequestMaster * CUIMAARequestMaster::create(CCObject *pBasic)
{
    if (pBasic)
    {
        if (CUIMAARequestMaster *pRequestMaster = new CUIMAARequestMaster())
        {
            pRequestMaster->m_pBasic = pBasic;
            return pRequestMaster;
        }
    }
    return NULL;
}

void CUIMAARequestMaster::show()
{
    if (getIsVisible())return;
    setIsVisible(true);
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 1, true);
    createBasicChild();
    
    if (CListLayer *pListLayer = getListLayer())
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
        
        pListLayer->updatePosition();
        pListLayer->setPosition(ccp(640,pListLayer->getPosition().y));
        pListLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, ccp(0,pListLayer->getPosition().y)),0.8f));
    }
}

void CUIMAARequestMaster::hide()
{
    setIsVisible(false);
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    removeAllChildrenWithCleanup(true);
}

bool CUIMAARequestMaster::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    return true;
}

CListLayer * CUIMAARequestMaster::getListLayer()
{
    if (CCNode *pNode = getChildByTag(UI_TAB_MAA_ARM_LIST))
    {
        return (CListLayer*)pNode;
    }
    
    if (CListLayer *pListLayer = new CListLayer(this, callfuncO_selector(CUIMAARequestMaster::onTouchEndedArmList)))
    {
        pListLayer->setReloadFunc(callfuncO_selector(CUIMAARequestMaster::onReloadCellArmList));
        addChild(pListLayer, 0, UI_TAB_MAA_ARM_LIST);
        return pListLayer;
    }
    return NULL;
}

CConfirmLayer * CUIMAARequestMaster::getConfirmLayer()
{
    if (CCNode *pNode = getChildByTag(UI_TAB_MAA_CONFIRM))
    {
        return (CConfirmLayer*)pNode;
    }
    
    if (CConfirmLayer *pConfirmLayer = new CConfirmLayer(this, callfuncO_selector(CUIMAARequestMaster::onConfirmFunc)))
    {
        addChild(pConfirmLayer, 2, UI_TAB_MAA_CONFIRM);
        return pConfirmLayer;
    }
    return NULL;
}

CCLayerColor * CUIMAARequestMaster::getLayerColor()
{
    if (CCNode *pNode = getChildByTag(UI_TAB_MAA_COLOR))
    {
        return (CCLayerColor*)pNode;
    }
    
    if (CCLayerColor *pLayerColor =  CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 196), 640, 960))
    {
        addChild(pLayerColor,2,UI_TAB_MAA_COLOR);
        return pLayerColor;
    }
    return NULL;
}

CCObject * CUIMAARequestMaster::getCurOPCall()
{
    //test
    CFriendInf *pOPObject = NULL;
    if (CCMutableArray<CFriendInf*> *pFriendList = CGameData::Inst()->getFriendList())
    {
        if (CConfirmLayer *pConfirmLayer = getConfirmLayer())
        {
            pOPObject = pFriendList->getObjectAtIndex(pConfirmLayer->getOPIndex());
        }
    }
    return pOPObject;
}

CShowCardInf * CUIMAARequestMaster::getShowCardInfo()
{
    if (CCNode *pNode = getChildByTag(UI_TAB_MAA_CARD_INFO))
    {
        return (CShowCardInf*)pNode;
    }
    
    if (CShowCardInf *pShowCardInf = new CShowCardInf())
    {
        addChild(pShowCardInf, 3, UI_TAB_MAA_CARD_INFO);
        return pShowCardInf;
    }
    return NULL;
}

void CUIMAARequestMaster::hideConfirmLayer()
{
    if (CConfirmLayer *pConfirmLayer = getConfirmLayer())
    {
        removeChild(pConfirmLayer,true);
    }
    if (CCLayerColor *pLayerColor = getLayerColor())
    {
        removeChild(pLayerColor,true);
    }
}

void CUIMAARequestMaster::createBasicChild()
{
    createBackMenuItem();
    createTitleBar(CGameData::Inst()->getLanguageValue("maa_request_master"));
    createSortMenuItem();
    createSeekIDItem();
    createRefreshItem();
}

void CUIMAARequestMaster::btnCallLine(CCObject* pSender)
{
}

void CUIMAARequestMaster::onConfirmFunc(CCObject* pSender)
{
    //播放声音
    if(CGameData::Inst()->isSoundOn)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    }
    
    fa_return(pSender);
    CCfmLayerRet* pRet = (CCfmLayerRet*) pSender;

    //test
    CFriendInf *pOPFriendInfo = NULL;
    if (CCMutableArray<CFriendInf*> *pFriendList = CGameData::Inst()->getFriendList())
    {
        if (CConfirmLayer *pConfirmLayer = getConfirmLayer())
        {
            pOPFriendInfo = pFriendList->getObjectAtIndex(pConfirmLayer->getOPIndex());
        }
    }
    fa_return(pOPFriendInfo);
    
    //send mail
    if (0==pRet->iBtnSel)
    {
        OcProxy::Inst()->showMailViewController(pOPFriendInfo->name);
        schedule(schedule_selector(CUIMAARequestMaster::onUpdateMail));
    }
    else if (1==pRet->iBtnSel)
    {
        //...
    }
    else if (2==pRet->iBtnSel)
    {
        if (CShowCardInf *pShowCardInf = getShowCardInfo())
        {
            pShowCardInf->show(&pOPFriendInfo->card_info,false);
            MainScene::Inst()->showBottomPanel(false);
        }
    }
    else if (3==pRet->iBtnSel)
    {
        hideConfirmLayer();
    }
}

void CUIMAARequestMaster::onTouchEndedArmList(CCObject* pSender)
{
    fa_return(pSender);
    
    //播放声音
    if(CGameData::Inst()->isSoundOn)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    }
    
    //show color child
    if (CCLayerColor *pLayerColor = getLayerColor())
    {
        pLayerColor->setIsVisible(true);
    }
    
    //test
    if (CCMutableArray<CFriendInf*> *pFriendList = CGameData::Inst()->getFriendList())
    {
        CListLayerRet* pRet = (CListLayerRet*)pSender;
        CFriendInf *pFriendInf = pFriendList->getObjectAtIndex(pRet->iBtnSel);
        if (CConfirmLayer * pConfirm = getConfirmLayer())
        {
            pConfirm->show(&pFriendInf->card_info, pFriendInf->name, enCConfirmlayer_maa_arm,pRet->iBtnSel);
        }
    }
}

void CUIMAARequestMaster::onReloadCellArmList(CCObject* pSender)
{
    if (CListLayer *pListLayer = getListLayer())
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

void CUIMAARequestMaster::onUpdateMail(ccTime dt)
{
    if (OcProxy::Inst()->getIsHasSentMail())
    {
        unschedule(schedule_selector(CUIMAARequestMaster::onUpdateMail));
        const char *pchMailText = OcProxy::Inst()->getMailText();
        if (0<strlen(pchMailText))
        {
            //test
            if (CFriendInf *pOPObject = (CFriendInf*)getCurOPCall())
            {
                if (CGameData::Inst()->sendMailRequest(pOPObject->fid,pchMailText))
                {
                    hideConfirmLayer();
                }
            }
        }
        OcProxy::Inst()->deleteMailViewController();
    }
}



