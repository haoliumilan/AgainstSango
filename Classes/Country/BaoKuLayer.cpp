//
//  BaoKuLayer.cpp
//  AgainstWar
//
//  Created by user on 13-7-18.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "BaoKuLayer.h"
#include "SimpleAudioEngine.h"

#define delatTAGBaoWu  20130828
#define ListItemHeight 268
#define delatTAGTip  20130902

BaoKuLayer::BaoKuLayer(CCObject* target, SEL_CallFuncO selector)
{
    m_iBtnSel = -1;
    m_textNumFiled = NULL;
    m_dlgAlert = NULL;
    m_commDlg = NULL;
    m_dlgTip = NULL;
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_listBaoKu = new CListLayer(this, callfuncO_selector(BaoKuLayer::cbList),2,ListItemHeight,592);
    addChild(m_listBaoKu, -1);
    m_listBaoKu->release();
    m_listBaoKu->setReloadFunc(callfuncO_selector(BaoKuLayer::insertBaoKuItemByIdx));
    
    m_listBaoKu->setTouchBeginFunc(callfuncO_selector(BaoKuLayer::cbListBegin));
    m_listBaoKu->setTouchEndFunc(callfuncO_selector(BaoKuLayer::cbListEnd));
    m_listBaoKu->setTouchRemoveFunc(callfuncO_selector(BaoKuLayer::cbListRemove));
    
    CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardNumber();
    
    showBaoKuList();
}


BaoKuLayer::~BaoKuLayer()
{

}


void BaoKuLayer::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(BaoKuLayer::cbCommDlg),
                                    enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480 - getPosition().y));
            m_commDlg->release();
        }
        return;
    }
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
        MainScene::Inst()->enableBottomPanel(true);
    }
    
    unscheduleUpdate();
    
    if(comState == kReqStatOk)
    {
        switch (m_curReqType)
        {
            case enReqBaoKuItemBuy:
                showAlertBuySuccessDlg();

                break;
            
            default:
                break;
                
        }
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(BaoKuLayer::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480 - getPosition().y));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}


//******************************************************************************
// cbCommDlg
//******************************************************************************
void BaoKuLayer::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    MainScene::Inst()->enableBottomPanel(true);
    
    if(isOk == true){
        switch (m_curReqType) {
            case enReqBaoKuItemBuy:
                BuyGuildItemShop();
                break;
                
            default:
                break;
        }
    }
    else
    {
        CGameData::Inst()->clearReqStat();
    }
}


void BaoKuLayer::showAlertBuySuccessDlg()
{
    if (m_dlgAlert == NULL)
    {
        m_dlgAlert = new Dialog1(this,
                                 callfuncO_selector(BaoKuLayer::cbAlertDlg),
                                 CGameData::Inst()->getLanguageValue("cntry_baoku_baokubuysuccess"),
                                 false);
        addChild(m_dlgAlert, 100);
        m_dlgAlert->release();
    }
    
    m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
    m_dlgAlert->setScale(0.0);
    m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void BaoKuLayer::cbAlertDlg()
{
    if (m_dlgAlert)
    {
        m_dlgAlert->removeFromParentAndCleanup(true);
        m_dlgAlert = NULL;
    }
    
    if (m_textNumFiled != NULL)
    {
        m_textNumFiled->detachWithIME();
        m_textNumFiled->removeFromParentAndCleanup(true);
        m_textNumFiled = NULL;
    }
    
    m_iBtnSel = -1;
    
    m_listBaoKu->clearAll();
    showBaoKuList();
}

void BaoKuLayer::cbListBegin(CCObject* pObj)
{
    CListLayerRet* ret = (CListLayerRet*)pObj;
    
    float y = ret->pos.y - (ListItemHeight*ret->iBtnSel);
    
    y *= -1;
    
    CCPoint pos = ccp(ret->pos.x - 320, y);
    
    CMenuBtn* menuBtn =  (CMenuBtn*)(m_listBaoKu->getChildByTag(deltaTag+ret->iBtnSel));
    
    CCSprite* rewardIcon = (CCSprite*)menuBtn->getChildByTag(TAGXSREWARDICON);
    
    CCRect rect = CCRectMake(rewardIcon->getPosition().x - rewardIcon->getContentSize().width * 0.5,
                             rewardIcon->getPosition().y - rewardIcon->getContentSize().height * 0.5,
                             rewardIcon->getContentSize().width,
                             rewardIcon->getContentSize().height);
    
    if (CCRect::CCRectContainsPoint(rect,pos))
    {
        m_iTouchSel = ret->iBtnSel;
        TipsPopInfo* tips = (TipsPopInfo*)menuBtn->getChildByTag(delatTAGTip);
        tips->runAction(CCScaleTo::actionWithDuration(0.1f, 1.0f));
    }
}

void BaoKuLayer::cbListEnd(CCObject* pObj)
{
    CMenuBtn* menuBtn =  (CMenuBtn*)(m_listBaoKu->getChildByTag(deltaTag+m_iTouchSel));
    
    if (!menuBtn)
        return;
    
    TipsPopInfo* tips = (TipsPopInfo*)menuBtn->getChildByTag(delatTAGTip);
    if (tips)
        tips->runAction(CCScaleTo::actionWithDuration(0.1,0));
}

void BaoKuLayer::cbListRemove(CCObject* pObj)
{
    CListLayerRet* ret = (CListLayerRet*)pObj;
    CMenuBtn* menuBtn =  (CMenuBtn*)(m_listBaoKu->getChildByTag(deltaTag+ret->iBtnSel));
    if (menuBtn && menuBtn->getChildByTag(TAGTEXTFEILD))
    {
        menuBtn->removeChildByTag(TAGTEXTFEILD, true);
        m_textNumFiled = NULL;
    }
}

void BaoKuLayer::cbList(CCObject* pObj)
{
    CListLayerRet* ret = (CListLayerRet*)pObj;
    
    float y = ret->pos.y - (ListItemHeight*ret->iBtnSel);
    
    y *= -1;
    
    CCPoint pos = ccp(ret->pos.x, y);
    
    CMenuBtn* menuBtn =  (CMenuBtn*)(m_listBaoKu->getChildByTag(deltaTag+ret->iBtnSel));
    
    CCSprite* spNum = (CCSprite*)menuBtn->getChildByTag(delatTAGBaoWu);

    if (CCRect::CCRectContainsPoint(CCRectMake(spNum->getPosition().x - spNum->getContentSize().width * 0.5,
                                           spNum->getPosition().y - spNum->getContentSize().height * 0.5,
                                           spNum->getContentSize().width,
                                           spNum->getContentSize().height),
                                pos))
    {
        if (m_textNumFiled && m_iBtnSel != ret->iBtnSel)
        {
            CMenuBtn* menuBtn1 = (CMenuBtn*)m_textNumFiled->getParent();
            if (menuBtn1)
            menuBtn1->baokuPriceChange(0);
            
            m_textNumFiled->removeFromParentAndCleanup(true);
            m_textNumFiled = NULL;
        }
        else if (m_textNumFiled &&  m_iBtnSel == ret->iBtnSel)
        {
            m_textNumFiled->attachWithIME();
            if (ret ->screenPos.y < 500)
            {
                moveUp(500- ret->screenPos.y);
            }
            return;
        }
        
        m_iBtnSel = ret->iBtnSel;
        
        CCSize size = spNum->getContentSize();
        m_textNumFiled = CCTextFieldTTF::textFieldWithPlaceHolder("",size ,CCTextAlignmentCenter,"huakangfont.ttf", 26);
        m_textNumFiled->setPosition(ccp(spNum->getPosition().x + size.width/2, spNum->getPosition().y));
        m_textNumFiled->setColor(ccWHITE);
        
        m_textNumFiled->setDelegate(this);
        menuBtn->addChild(m_textNumFiled,10,TAGTEXTFEILD);
        m_textNumFiled->attachWithIME();
        
        if (ret ->screenPos.y < 500)
        {
            moveUp(500- ret->screenPos.y);
        }
    }
    else
    {
        if (m_textNumFiled)
        {
            m_textNumFiled->detachWithIME();
            moveDown();
        }
    }
}

void BaoKuLayer::moveDown()
{
    CCAction* act = CCMoveTo::actionWithDuration(0.3, CCPointZero);
    runAction(act);
}

void BaoKuLayer::moveUp(float offy)
{
    CCAction* act = CCMoveTo::actionWithDuration(0.3, ccp(0, getPosition().y + offy));
    runAction(act);
}

void BaoKuLayer::insertBaoKuItemByIdx(CCObject *pObj)
{
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    int i = pRet->iBtnSel;
    
    CMenuBtn* newMenuBtn = new CMenuBtn(CGameData::Inst()->getGuildShopItemInfArr()->getObjectAtIndex(i));
    m_listBaoKu->reloadItemByIdx(newMenuBtn, pRet->iBtnSel);
    newMenuBtn->release();
    
    CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                 spMenu2,
                                                                 this,
                                                                 menu_selector(BaoKuLayer::cbBuyItem));
    itemShow->setTag(i*10+1);
    itemShow->setAnchorPoint(ccp(0, 0));
    itemShow->setPosition(ccp(120,-50));
    
    char buf[100];
    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("gachaDlgTip11"));
    TextNode* lbText = TextNode::textWithString(buf,kBtnTitleHeight);
    lbText->setAnchorPoint(CCPointZero);
    lbText->setPosition(CCPointMake(32,18));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    itemShow->addChild(lbText, 1);
    
    CCMenu *menu = CCMenu::menuWithItem(itemShow);
    newMenuBtn->addChild(menu);
    menu->setPosition(CCPointZero);
    
    CCSprite* m_numBG1 = CCSprite::spriteWithFile("fr_reward_bar.png");
    newMenuBtn->addChild(m_numBG1,0,delatTAGBaoWu);
    m_numBG1->setAnchorPoint(ccp(0,0.5));
    m_numBG1->setPosition(ccp(-90,-76));
    
    TipsPopInfo* tips = TipsPopInfo::tipWithContent(CGameData::Inst()->getGuildShopItemInfArr()->getObjectAtIndex(pRet->iBtnSel)->strItemDsp.c_str());
    newMenuBtn->addChild(tips,10,delatTAGTip);
    tips->setPosition(ccp(50, 10));
    tips->setScale(0);
}

void BaoKuLayer::showBaoKuList()
{
    CMenuBtn* newMenuBtn;
    CCMutableArray<GuildShopItemInf* > * pGuildShopItemInfArr = CGameData::Inst()->getGuildShopItemInfArr();
    for (int i = 0; i < pGuildShopItemInfArr->count(); i++)
    {
        if (i >= m_listBaoKu->getPreTopShow()
            && i <= m_listBaoKu->getPreBottomShow())
        {
            newMenuBtn = new CMenuBtn(pGuildShopItemInfArr->getObjectAtIndex(i));
            
            CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
            CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
            
            CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                         spMenu2,
                                                                         this,
                                                                         menu_selector(BaoKuLayer::cbBuyItem));
            itemShow->setTag(i*10+1);
            itemShow->setAnchorPoint(ccp(0, 0));
            itemShow->setPosition(ccp(120,-50));
            
            char buf[100];
            snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("gachaDlgTip11"));
            TextNode* lbText = TextNode::textWithString(buf,kBtnTitleHeight);
            lbText->setAnchorPoint(CCPointZero);
            lbText->setPosition(CCPointMake(32,18));
            lbText->setColor(ccWHITE);
            lbText->setShadowColor(ccBLACK);
            itemShow->addChild(lbText, 1);
            
            
            CCMenu *menu = CCMenu::menuWithItem(itemShow);
            newMenuBtn->addChild(menu);
            menu->setPosition(CCPointZero);
            
            CCSprite* m_numBG1 = CCSprite::spriteWithFile("fr_reward_bar.png");
            newMenuBtn->addChild(m_numBG1,0,delatTAGBaoWu);
            m_numBG1->setAnchorPoint(ccp(0,0.5));
            m_numBG1->setPosition(ccp(-90,-76));
            
            
            TipsPopInfo* tips = TipsPopInfo::tipWithContent(pGuildShopItemInfArr->getObjectAtIndex(i)->strItemDsp.c_str());
            
//            const char* chr = "物色遇袭:可以得到10个社呢么";
//            TipsPopInfo* tips = TipsPopInfo::tipWithContent(chr);
            
            newMenuBtn->addChild(tips,10,delatTAGTip);
            tips->setPosition(ccp(50, 10));
            tips->setScale(0);
        }
        else
        {
            newMenuBtn = NULL;
        }
        
        m_listBaoKu->push_back(newMenuBtn);
        
        if (newMenuBtn)
            newMenuBtn->release();
    }
}


void BaoKuLayer::cbBuyItem(CCObject* pObj)
{
    CCMenuItem* mItem = (CCMenuItem*)pObj;
    int i_listSel = mItem->getTag() / 10;
    CMenuBtn* menuBtn =  (CMenuBtn*)(m_listBaoKu->getChildByTag(deltaTag+i_listSel));
    m_pCurMBtn = menuBtn;
    if (menuBtn->getBuyNum() <= 0)
        return;
    
    if (m_textNumFiled != NULL)
    {
        m_textNumFiled->detachWithIME();
        moveDown();
    }
    
    if (menuBtn->getGShopItemInf()->lCoin != 0)
    {
        if (menuBtn->getGShopItemInf()->lCoin*menuBtn->getBuyNum() > CGameData::Inst()->getCntryInfo()->cntry_coin)
            ShowCanNotBuy(0);
        else
        {
            m_strCurItemId = menuBtn->getGShopItemInf()->strItemId;
            m_lCurbuyNum = menuBtn->getBuyNum();
            BuyGuildItemShop();
        }
    }
    else
    {
        if (menuBtn->getGShopItemInf()->lCntrbt*menuBtn->getBuyNum() > CGameData::Inst()->getCntryInfo()->cntry_supply)
            ShowCanNotBuy(1);
        else
        {
            m_strCurItemId = menuBtn->getGShopItemInf()->strItemId;
            m_lCurbuyNum = menuBtn->getBuyNum();
            BuyGuildItemShop();
        }
    }
}


void BaoKuLayer::BuyGuildItemShop()
{
    if (CGameData::Inst()->reqBuyGuildItem(m_strCurItemId,m_lCurbuyNum))
    {
        m_curReqType = enReqBaoKuItemBuy;
        scheduleUpdate();
    }
}


void BaoKuLayer::ShowCanNotBuy(int iType)
{
    if(m_dlgTip == NULL)
    {
        const char* buf;
        switch (iType) {
            case 0:
                buf = CGameData::Inst()->getLanguageValue("cntry_baoku_yuanbaobuzu");
                break;
                
            case 1:
                buf = CGameData::Inst()->getLanguageValue("cntry_baoku_junliangbuzu");
                break;
                
            default:
                break;
        }
        
        m_dlgTip = new Dialog2(this,
                               callfuncO_selector(BaoKuLayer::cbCanNotBuy),
                               CGameData::Inst()->getLanguageValue("cntry_hint"),
                               buf,
                               enDl2TwoBtnJuanXian);
        addChild(m_dlgTip, 100);
        m_dlgTip->release();
    }
    
    m_dlgTip->setPosition(CCPointMake(320, 480));
    m_dlgTip->setScale(0.0);
    m_dlgTip->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void BaoKuLayer::cbCanNotBuy(CCObject *pObj)
{
    Dialog2Ret* pRet = (Dialog2Ret*) pObj;
    
    if (m_dlgTip) {
        m_dlgTip->removeFromParentAndCleanup(true);
        m_dlgTip = NULL;
    }
    
    if(!pRet->bOk == true)
    {
        m_ret.iBtnSel = en_cbBaoKuJuanXian;
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
}



bool BaoKuLayer::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen) {
    CCLog("onTextFieldInsertText text = %s, nLen = %d, char count = %d char = %s", text, nLen, sender->getCharCount(), sender->getString());
    
    if (sender->getCharCount() + nLen > 9) {
        return true;
    }
    if (*text <= 57 && *text >= 48)
    {
        int  num = atoi(m_textNumFiled->getString());
        if (num > 999)
            return true;
        
        return false;
    }
    else {
        return true;
    }
}

bool BaoKuLayer::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    return false;
}


void BaoKuLayer::afterUpdate()
{
    int num = atoi(m_textNumFiled->getString());
    CMenuBtn* menuBtn = (CMenuBtn*)m_textNumFiled->getParent();
    menuBtn->baokuPriceChange(num);
}





