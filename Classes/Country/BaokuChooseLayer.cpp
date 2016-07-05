//
//  BaokuChooseLayer.cpp
//  AgainstWar
//
//  Created by user on 13-7-22.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "BaokuChooseLayer.h"


BaokuChooseLayer::BaokuChooseLayer(CCObject* target, SEL_CallFuncO selector, int total)
{
    m_iTotal = total;
    m_dlgNoTip = NULL;
    m_dlgAlert = NULL;
    m_dlgTip = NULL;
    m_pListener = target;
    m_pfnSelector = selector;
    m_textNumFiled = NULL;
    
    m_listBaoKu = new CListLayer(this, callfuncO_selector(BaokuChooseLayer::cbList),2,268,592);
    addChild(m_listBaoKu, -1);
    m_listBaoKu->release();
    m_listBaoKu->setReloadFunc(callfuncO_selector(BaokuChooseLayer::insertBaoKuItemByIdx));
    
    m_listBaoKu->setTouchBeginFunc(callfuncO_selector(BaokuChooseLayer::cbListBegin));
    m_listBaoKu->setTouchEndFunc(callfuncO_selector(BaokuChooseLayer::cbListEnd));
    m_listBaoKu->setTouchRemoveFunc(callfuncO_selector(BaokuChooseLayer::cbListRemove));
    
    showBaoKuChooseList();
}


BaokuChooseLayer::~BaokuChooseLayer()
{
    
}

void BaokuChooseLayer::showAlertNoTaskDlg()
{
    if (m_dlgNoTip == NULL)
    {
        m_dlgNoTip = new NoFriendLayer(this,callfuncO_selector(BaokuChooseLayer::cbAlertDlg2));
        addChild(m_dlgNoTip);
        m_dlgNoTip->release();
        m_dlgNoTip->setInfoLabel(CGameData::Inst()->getLanguageValue("cntry_xs_havenobaoku"));
        m_dlgNoTip->setPosition(CCPoint(960, 400));
        CCActionInterval* tempAction = CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(320, 400));
        m_dlgNoTip->runAction(CCEaseElasticOut::actionWithAction(tempAction, 0.8f));
    }
}

void BaokuChooseLayer::cbAlertDlg2()
{
    if (m_dlgNoTip)
    {
        m_dlgNoTip->removeFromParentAndCleanup(true);
        m_dlgNoTip = NULL;
    }
    
    m_ret.m_iType = 1;
    (m_pListener->*m_pfnSelector)(&m_ret);
}

void BaokuChooseLayer::moveDown()
{
    CCAction* act = CCMoveTo::actionWithDuration(0.3, ccp(getPosition().x, 0));
    runAction(act);
}

void BaokuChooseLayer::moveUp(float offy)
{
    CCAction* act = CCMoveTo::actionWithDuration(0.3, ccp(getPosition().x, getPosition().y + offy));
    runAction(act);
}

void BaokuChooseLayer::showBaoKuChooseList()
{
    CMenuBtn* newMenuBtn;
    
    CCMutableArray<GuildItemInStore*>* pGuildItemInStore = CGameData::Inst()->getCntryInfo()->m_pGuildItemInStore;
    if (pGuildItemInStore == NULL)
    {
        showAlertNoTaskDlg();
        return;
    }
    
    if (pGuildItemInStore->count() == 0)
    {
        showAlertNoTaskDlg();
        return;
    }
    
    for (int i = 0; i < pGuildItemInStore->count(); i++)
    {
        if (i >= m_listBaoKu->getPreTopShow()
            && i <= m_listBaoKu->getPreBottomShow())
        {
            newMenuBtn = new CMenuBtn(pGuildItemInStore->getObjectAtIndex(i), m_iTotal);
            
            CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
            CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
            
            CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                         spMenu2,
                                                                         this,
                                                                         menu_selector(BaokuChooseLayer::cbRewardItem));
            itemShow->setTag(i*10+1);
            itemShow->setAnchorPoint(ccp(0, 0));
            itemShow->setPosition(ccp(120,-50));
            
            char buf[100];
            snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_xs_xuanshangbtn"));
            TextNode* lbText = TextNode::textWithString(buf,kBtnTitleHeight);
            lbText->setAnchorPoint(CCPointZero);
            lbText->setPosition(CCPointMake(32,18));
            lbText->setColor(ccWHITE);
            lbText->setShadowColor(ccBLACK);
            itemShow->addChild(lbText, 1);
            
            
            CCMenu *menu = CCMenu::menuWithItem(itemShow);
            newMenuBtn->addChild(menu);
            menu->setPosition(CCPointZero);
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

void BaokuChooseLayer::cbList(CCObject* pObj)
{
    CListLayerRet* ret = (CListLayerRet*)pObj;
    
    float y = ret->pos.y - (268*ret->iBtnSel);
    
    y *= -1;
    
    CCPoint pos = ccp(ret->pos.x, y);
    
    CMenuBtn* menuBtn =  (CMenuBtn*)(m_listBaoKu->getChildByTag(deltaTag+ret->iBtnSel));
    
    if (menuBtn->getChildByTag(TAGCOLOCLAYER))
    {
        return;
    }
    
    CCSprite* spNum = (CCSprite*)menuBtn->getChildByTag(TAGXUANSHANGBAR);
    
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
                menuBtn1->chooseUpdate(0);
            
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

void BaokuChooseLayer::cbListBegin(CCObject* pObj)
{
    CListLayerRet* ret = (CListLayerRet*)pObj;
    
    float y = ret->pos.y - (268*ret->iBtnSel);
    
    y *= -1;
    
    CCPoint pos = ccp(ret->pos.x - 320, y);
    
    CMenuBtn* menuBtn =  (CMenuBtn*)(m_listBaoKu->getChildByTag(deltaTag+ret->iBtnSel));
    
    if (menuBtn->getChildByTag(TAGCOLOCLAYER))
    {
        return;
    }
    
    CCSprite* rewardIcon = (CCSprite*)menuBtn->getChildByTag(TAGXSREWARDICON);
    
    CCRect rect = CCRectMake(rewardIcon->getPosition().x - rewardIcon->getContentSize().width * 0.5,
                             rewardIcon->getPosition().y - rewardIcon->getContentSize().height * 0.5,
                             rewardIcon->getContentSize().width,
                             rewardIcon->getContentSize().height);
    
    if (CCRect::CCRectContainsPoint(rect,pos))
    {
        m_iTouchSel = ret->iBtnSel;
        TipsPopInfo* tips = (TipsPopInfo*)menuBtn->getChildByTag(TAGXUANSHANGTIP);
        tips->runAction(CCScaleTo::actionWithDuration(0.1f, 1.0f));
    }
}

void BaokuChooseLayer::cbListRemove(CCObject* pObj)
{
    CListLayerRet* ret = (CListLayerRet*)pObj;
    CMenuBtn* menuBtn =  (CMenuBtn*)(m_listBaoKu->getChildByTag(deltaTag+ret->iBtnSel));
    
    if (menuBtn && menuBtn->getChildByTag(TAGCOLOCLAYER))
    {
        return;
    }
    
    if (menuBtn && menuBtn->getChildByTag(TAGTEXTFEILD))
    {
        menuBtn->removeChildByTag(TAGTEXTFEILD, true);
        m_textNumFiled = NULL;
    }
}

void BaokuChooseLayer::cbListEnd(CCObject* pObj)
{
    
    CMenuBtn* menuBtn =  (CMenuBtn*)(m_listBaoKu->getChildByTag(deltaTag+m_iTouchSel));
    
    if (!menuBtn)
        return;
    
    if (menuBtn->getChildByTag(TAGCOLOCLAYER))
    {
        return;
    }
    
    TipsPopInfo* tips = (TipsPopInfo*)menuBtn->getChildByTag(TAGXUANSHANGTIP);
    if (tips)
        tips->runAction(CCScaleTo::actionWithDuration(0.1,0));
}


void BaokuChooseLayer::cbRewardItem(CCObject* pObj)
{
    CCMenuItem* mItem = (CCMenuItem*)pObj;
    int i_listSel = mItem->getTag() / 10;
    CMenuBtn* menuBtn =  (CMenuBtn*)(m_listBaoKu->getChildByTag(deltaTag+i_listSel));
    
    m_ret.lrewardNum = menuBtn->getRewardNum();
    
    if (m_ret.lrewardNum <= 0)
        return;
    
    if(m_ret.lrewardNum > menuBtn->getGuildItemInf()->lCnt)
    {
        showAlertCanotXuanShang();
        return;
    }
    
    
    m_ret.strImgName = menuBtn->getGuildItemInf()->strImgName;
    m_ret.strItemId = menuBtn->getGuildItemInf()->strItemId;
    m_ret.strItemName = menuBtn->getGuildItemInf()->strItemName;
    m_ret.m_iType = 0;
    
    (m_pListener->*m_pfnSelector)(&m_ret);
}

void BaokuChooseLayer::showAlertCanotXuanShang()
{
    if (m_dlgAlert == NULL)
    {
        m_dlgAlert = new Dialog1(this,
                                 callfuncO_selector(BaokuChooseLayer::cbAlertDlg),
                                 CGameData::Inst()->getLanguageValue("cntry_fabu_kucunbuzu"),
                                 false);
        addChild(m_dlgAlert, 100);
        m_dlgAlert->release();
    }
    
    m_dlgAlert->setPosition(CCPointMake(320, 480 - getPosition().y));
    m_dlgAlert->setScale(0.0);
    m_dlgAlert->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void BaokuChooseLayer::cbAlertDlg()
{
    if (m_dlgAlert)
    {
        m_dlgAlert->removeFromParentAndCleanup(true);
        m_dlgAlert = NULL;
    }
}


void BaokuChooseLayer::insertBaoKuItemByIdx(CCObject* pObj)
{
    CCMutableArray<GuildItemInStore*>* pGuildItemInStore = CGameData::Inst()->getCntryInfo()->m_pGuildItemInStore;
    if (pGuildItemInStore == NULL)
        return;
    
    CListLayerRet* pRet = (CListLayerRet*)pObj;
    int i = pRet->iBtnSel;
    
    CMenuBtn* newMenuBtn = new CMenuBtn(pGuildItemInStore->getObjectAtIndex(i), m_iTotal);
    m_listBaoKu->reloadItemByIdx(newMenuBtn, pRet->iBtnSel);
    newMenuBtn->release();
    
    CCSprite*   spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite*   spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    
    CCMenuItem *itemShow = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                 spMenu2,
                                                                 this,
                                                                 menu_selector(BaokuChooseLayer::cbRewardItem));
    itemShow->setTag(i*10+1);
    itemShow->setAnchorPoint(ccp(0, 0));
    itemShow->setPosition(ccp(120,-50));
    
    char buf[100];
    snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("cntry_xs_xuanshangbtn"));
    TextNode* lbText = TextNode::textWithString(buf,kBtnTitleHeight);
    lbText->setAnchorPoint(CCPointZero);
    lbText->setPosition(CCPointMake(32,18));
    lbText->setColor(ccWHITE);
    lbText->setShadowColor(ccBLACK);
    itemShow->addChild(lbText, 1);
    
    
    CCMenu *menu = CCMenu::menuWithItem(itemShow);
    newMenuBtn->addChild(menu);
    menu->setPosition(CCPointZero);
}


bool BaokuChooseLayer::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen) {
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

bool BaokuChooseLayer::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    return false;
}


void BaokuChooseLayer::afterUpdate()
{
    int num = atoi(m_textNumFiled->getString());
    CMenuBtn* menuBtn = (CMenuBtn*)m_textNumFiled->getParent();
    
    if (menuBtn->getGuildItemInf()->lCnt < num)
        num = menuBtn->getGuildItemInf()->lCnt;
    
    int maxNum = 0;
    
    maxNum = m_iTotal/menuBtn->getGuildItemInf()->lMinCondition;
    
    if (maxNum < num)
        num = maxNum;
    
    char buf[50];
    snprintf(buf, 49, "%d",num);
    m_textNumFiled->setString(buf);

    menuBtn->chooseUpdate(num);
}





