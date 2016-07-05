//
//  RewardRelease.cpp
//  AgainstWar
//
//  Created by user on 13-7-22.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "RewardRelease.h"
#include "Dialog1.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

#define kDlgAlertItemCount 1111

RewardRelease::RewardRelease(CCObject* target, SEL_CallFuncO selector)
{
    m_lcontribution = 0;
    m_ideadLine = 0;
    m_iPerson = 0;
    m_iBaozhengCoin = 0;
    m_iItemCount = 0;
    
    m_spDisMenu = NULL;
    m_iCurChoose = -1;
    m_listBaokuChoose = NULL;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true);
    
    m_spBG1 = NULL;
    m_textfiled1 = NULL;
    m_spBG2 = NULL;
    m_textfiled2 = NULL;
    m_spBG3 = NULL;
    m_textfiled3 = NULL;
    m_spBG4 = NULL;
    m_textfiled4 = NULL;
    
    m_pListener = target;
    m_pfnSelector = selector;
    
    CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardNumber();
    
    m_mubiaoGx = SaveData::Inst()->getMubiaoGongXianBylv(CGameData::Inst()->getCntryInfo()->lv);

    m_lcontribution = m_mubiaoGx.minMubiaoGX;
    m_ideadLine = m_mubiaoGx.ideadLine;
    m_iPerson = m_mubiaoGx.iPerson;
    m_iBaozhengCoin = m_mubiaoGx.m_iBaozhengCoin;
    
    drawAllChildren();
}

RewardRelease::~RewardRelease()
{
    m_textfiled1->detachWithIME();
    m_textfiled2->detachWithIME();
    m_textfiled3->detachWithIME();
    m_textfiled4->detachWithIME();
}


void RewardRelease::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(RewardRelease::cbCommDlg),
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
            case enReqXuanShangFaBu:
                m_ret.iBtnSel = en_fabuXuanShang_end;
                (m_pListener->*m_pfnSelector)(&m_ret);
                break;
                
            default:
                break;
                
        }
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(RewardRelease::cbCommDlg), enCommDlgTp_retry);
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
void RewardRelease::cbCommDlg(CCObject* pObj)
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
            case enReqXuanShangFaBu:
                reqFabuXuanshang();
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

void RewardRelease::drawAllChildren()
{
    CCSprite* spBg = CCSprite::spriteWithFile("bonuspanel.png");
    addChild(spBg,-1);
    spBg->setPosition(ccp(320, 438));
    
    TextNode* TitleText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_xuanshangfabu"),38);
    TitleText->setAnchorPoint(ccp(0.5, 0.5));
    TitleText->setPosition(CCPointMake(320,666));
    TitleText->setColor(ccc3(142, 21, 15));
    addChild(TitleText);
    
    float fStartx = 56.0f;
    float fStarty = 596.0f;
    float fOffy = -58.0f;
    float fOffX = 182.0f;
    float fontSize = 23;
    
    TextNode* TmubiaoText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_xs_mubiaosheding"),fontSize);
    TmubiaoText->setAnchorPoint(ccp(0,0.5));
    TmubiaoText->setPosition(CCPointMake(fStartx,fStarty));
    TmubiaoText->setColor(ccBLACK);
    addChild(TmubiaoText);
    
    m_spBG1 = CCSprite::spriteWithFile("fr_reward_bar.png");
    addChild(m_spBG1);
    m_spBG1->setAnchorPoint(ccp(0,0.5));
    m_spBG1->setPosition(ccp(fStartx+fOffX,fStarty+fOffy*0));
    
    CCSize size = m_spBG1->getContentSize();

    char buf[50];
    snprintf(buf, 49, "%ld",m_lcontribution);

    m_textfiled1 = CCTextFieldTTF::textFieldWithPlaceHolder(buf,size ,CCTextAlignmentCenter,"huakangfont.ttf", 26);
    m_textfiled1->setPosition(ccp(m_spBG1->getPosition().x + size.width/2, m_spBG1->getPosition().y));
    m_textfiled1->setColor(ccWHITE);
    
    m_textfiled1->setDelegate(this);
    addChild(m_textfiled1,10);
    
    TextNode* TqixianText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_xs_wanchengqixian"),fontSize);
    TqixianText->setAnchorPoint(ccp(0,0.5));
    TqixianText->setPosition(CCPointMake(fStartx,fStarty+fOffy*1));
    TqixianText->setColor(ccBLACK);
    addChild(TqixianText);
    
    m_spBG2 = CCSprite::spriteWithFile("fr_reward_bar.png");
    addChild(m_spBG2);
    m_spBG2->setAnchorPoint(ccp(0,0.5));
    m_spBG2->setPosition(ccp(fStartx+fOffX,fStarty+fOffy*1));
    
    snprintf(buf, 49, "%d",m_ideadLine);
    m_textfiled2 = CCTextFieldTTF::textFieldWithPlaceHolder(buf,size ,CCTextAlignmentCenter,"huakangfont.ttf", 26);
    m_textfiled2->setPosition(ccp(m_spBG2->getPosition().x + size.width/2, m_spBG2->getPosition().y));
    m_textfiled2->setColor(ccWHITE);
    m_textfiled2->setDelegate(this);
    addChild(m_textfiled2,10);
    
    TextNode* TrenshuText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_xs_canyurenshu"),fontSize);
    TrenshuText->setAnchorPoint(ccp(0,0.5));
    TrenshuText->setPosition(CCPointMake(fStartx,fStarty+fOffy*2));
    TrenshuText->setColor(ccBLACK);
    addChild(TrenshuText);
    
    m_spBG3 = CCSprite::spriteWithFile("fr_reward_bar.png");
    addChild(m_spBG3);
    m_spBG3->setAnchorPoint(ccp(0,0.5));
    m_spBG3->setPosition(ccp(fStartx+fOffX,fStarty+fOffy*2));
    
    snprintf(buf, 49, "%d",m_iPerson);
    m_textfiled3 = CCTextFieldTTF::textFieldWithPlaceHolder(buf,size ,CCTextAlignmentCenter,"huakangfont.ttf", 26);
    m_textfiled3->setPosition(ccp(m_spBG3->getPosition().x + size.width/2, m_spBG3->getPosition().y));
    m_textfiled3->setColor(ccWHITE);
    m_textfiled3->setDelegate(this);
    addChild(m_textfiled3,10);
    
    TextNode* TbaozhengText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_xs_baozhengjin"),fontSize);
    TbaozhengText->setAnchorPoint(ccp(0,0.5));
    TbaozhengText->setPosition(CCPointMake(fStartx,fStarty+fOffy*3));
    TbaozhengText->setColor(ccBLACK);
    addChild(TbaozhengText);
    
    m_spBG4 = CCSprite::spriteWithFile("fr_reward_bar.png");
    addChild(m_spBG4);
    m_spBG4->setAnchorPoint(ccp(0,0.5));
    m_spBG4->setPosition(ccp(fStartx+fOffX,fStarty+fOffy*3));

    snprintf(buf, 49, "%d",m_iBaozhengCoin);
    m_textfiled4 = CCTextFieldTTF::textFieldWithPlaceHolder(buf,size ,CCTextAlignmentCenter,"huakangfont.ttf", 26);
    m_textfiled4->setPosition(ccp(m_spBG4->getPosition().x + size.width/2, m_spBG4->getPosition().y));
    m_textfiled4->setColor(ccWHITE);
    m_textfiled4->setDelegate(this);
    addChild(m_textfiled4,10);
    
    TextNode* TjiangliText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_xs_shedingjiangli"),fontSize);
    TjiangliText->setAnchorPoint(ccp(0,0.5));
    TjiangliText->setPosition(CCPointMake(fStartx,fStarty+fOffy*4));
    TjiangliText->setColor(ccBLACK);
    addChild(TjiangliText);
    
    m_spJiangli = CCSprite::spriteWithFile("fr_query_box.PNG");
    m_spJiangli->setAnchorPoint(CCPointMake(0, 1));
    m_spJiangli->setPosition(CCPointMake(fStartx+fOffX, fStarty+fOffy*4+22));
    addChild(m_spJiangli);
    
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
    CCMenuItem*  querenFaBu = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                                       spMenu2,
                                                                       this,
                                                                       menu_selector(RewardRelease::querenFabu));
    querenFaBu->setAnchorPoint(CCPointZero);
    querenFaBu->setPosition(ccp(320-(spMenu1->getContentSize().width/2), 192));
    
    CCMenu* menu = CCMenu::menuWithItem(querenFaBu);
    menu->setAnchorPoint(CCPointZero);
    menu->setPosition(CCPointZero);
    addChild(menu);
    
    m_spDisMenu = CCSprite::spriteWithSpriteFrameName("fr_long button black.png");
    m_spDisMenu->setPosition(ccp(230,196));
    m_spDisMenu->setAnchorPoint(CCPointZero);
    addChild(m_spDisMenu,1);
    
    TextNode* FaBuText = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_xs_querenfabu"),36);
    FaBuText->setPosition(CCPointMake(320,230));
    FaBuText->setColor(ccWHITE);
    FaBuText->setShadowColor(ccBLACK);
    addChild(FaBuText);
}

void RewardRelease::querenFabu(CCObject* pObj)
{
    reqFabuXuanshang();
}

void RewardRelease::reqFabuXuanshang()
{
    if (!bCanFabu())
        return;
    
    CCMutableArray<GuildItemInStore*>* pGuildItemInStore = CGameData::Inst()->getCntryInfo()->m_pGuildItemInStore;
    if (pGuildItemInStore->count() > 0)
    {
        for (int i = 0; i < pGuildItemInStore->count(); i++) {
            GuildItemInStore* itemInStore = pGuildItemInStore->getObjectAtIndex(i);
            if (itemInStore->strItemId == m_strItemId)
            {
                if (m_iPerson * m_iItemCount > itemInStore->lCnt)   // 发布的奖品数量超出库存
                {
                    showAlertItemCount();
                    return;
                }
            }
        }
    }
    
    if (CGameData::Inst()->postRewardRequest(m_lcontribution, m_ideadLine, m_iPerson, m_iBaozhengCoin, m_strItemId, m_iItemCount))
    {
        m_curReqType = enReqXuanShangFaBu;
        scheduleUpdate();
    }
}

void RewardRelease::showAlertItemCount()
{    
    char buf[150];
    snprintf(buf, 149, CGameData::Inst()->getLanguageValue("reward_alert_item_count"),
             m_iPerson, m_iPerson * m_iItemCount, m_strItemName.c_str());
    Dialog1* dlgAlertItemCount = new Dialog1(this,
                                             callfuncO_selector(RewardRelease::cbShowAlertItemCount),
                                             buf,
                                             false);
    addChild(dlgAlertItemCount, 100);
    dlgAlertItemCount->release();
    dlgAlertItemCount->setTag(kDlgAlertItemCount);
    dlgAlertItemCount->setPosition(CCPointMake(320, 480));
    dlgAlertItemCount->setScale(0.0);
    dlgAlertItemCount->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
}

void RewardRelease::cbShowAlertItemCount(cocos2d::CCObject *pObj)
{
    if(CGameData::Inst()->isSoundOn)
        SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));
    
    Dialog1* dlgAlertItemCount = (Dialog1*)getChildByTag(kDlgAlertItemCount);
    if (dlgAlertItemCount)
    {
        dlgAlertItemCount->removeFromParentAndCleanup(true);
        dlgAlertItemCount = NULL;
    }
}

bool RewardRelease::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView(touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    

        
    if (m_iItemCount <= 0 && m_spBG1 && CCRect::CCRectContainsPoint(CCRectMake(m_spBG1->getPosition().x - m_spBG1->getContentSize().width * 0,
                                                          m_spBG1->getPosition().y - m_spBG1->getContentSize().height * 0.5,
                                                          m_spBG1->getContentSize().width,
                                                          m_spBG1->getContentSize().height),
                                               pos))
        {
            OpenATextField(1);
            return true;
        }
    else if (m_iItemCount <= 0 && m_spBG2 && CCRect::CCRectContainsPoint(CCRectMake(m_spBG2->getPosition().x - m_spBG2->getContentSize().width * 0,
                                                                   m_spBG2->getPosition().y - m_spBG2->getContentSize().height * 0.5,
                                                                   m_spBG2->getContentSize().width,
                                                                   m_spBG2->getContentSize().height),
                                                        pos))
    {
        OpenATextField(2);
        return true;
    }
    else if (m_iItemCount <= 0 && m_spBG3 && CCRect::CCRectContainsPoint(CCRectMake(m_spBG3->getPosition().x - m_spBG3->getContentSize().width * 0,
                                                               m_spBG3->getPosition().y - m_spBG3->getContentSize().height * 0.5,
                                                               m_spBG3->getContentSize().width,
                                                               m_spBG3->getContentSize().height),
                                                    pos))
    {
        OpenATextField(3);
        return true;
    }
    else if (m_iItemCount <= 0 && m_spBG4 && CCRect::CCRectContainsPoint(CCRectMake(m_spBG4->getPosition().x - m_spBG4->getContentSize().width * 0,
                                                               m_spBG4->getPosition().y - m_spBG4->getContentSize().height * 0.5,
                                                               m_spBG4->getContentSize().width,
                                                               m_spBG4->getContentSize().height),
                                                    pos))
    {
        OpenATextField(4);
        return true;
    }
    
    OpenATextField(0);
    
    if (m_spJiangli && CCRect::CCRectContainsPoint(CCRectMake(m_spJiangli->getPosition().x,
                                                               m_spJiangli->getPosition().y - m_spJiangli->getContentSize().height,
                                                               m_spJiangli->getContentSize().width,
                                                               m_spJiangli->getContentSize().height),
                                                    pos))
    {
        //OpenATextField(0);
        
        if (m_listBaokuChoose == NULL)
            m_listBaokuChoose = new BaokuChooseLayer(this,callfuncO_selector(RewardRelease::cbFromChooseBaoku), m_lcontribution / m_ideadLine);
        setPosition(ccp(-3000,0));
        addChild(m_listBaokuChoose,11);
        m_listBaokuChoose->setPosition(ccp(3000,0));
        OpenATextField(0);
        
        m_ret.iBtnSel = en_ChooseItemFromFabu;
        (m_pListener->*m_pfnSelector)(&m_ret);
        return true;
    }
    
    return false;
}

void RewardRelease::layerMoveUpOrDown(bool up)
{
    CCAction* act;
    
    if (up)
        act = CCMoveTo::actionWithDuration(0.2, CCPointMake(getPosition().x, 100));
    else
        act = CCMoveTo::actionWithDuration(0.2, ccp(getPosition().x,0));
    
    runAction(act);
}

void RewardRelease::OpenATextField(int idx)
{
    switch (m_iCurChoose)
    {
        case 1:
        {
            m_lcontribution = (atol(m_textfiled1->getString()));
            
            long minCon =  (long)(CGameData::Inst()->getDefaultCntryBu(1));
            
            if (m_lcontribution < minCon)
            {
                m_lcontribution = minCon;
                char buf[50];
                snprintf(buf, 49, "%ld",m_lcontribution);
                m_textfiled1->setString(buf);
            }
        }
            break;
        case 2:
            m_ideadLine = atoi(m_textfiled2->getString());
            if (m_ideadLine > 7)
            {
                m_ideadLine = 7;
                m_textfiled2->setString("7");
            }
            else if (m_ideadLine < 1)//m_mubiaoGx.ideadLine)
            {
                m_ideadLine = 1;//m_mubiaoGx.ideadLine;
                char buf[50];
                snprintf(buf, 49, "%d",m_ideadLine);
                m_textfiled2->setString(buf);
            }
            break;
        case 3:
            m_iPerson = atoi(m_textfiled3->getString());
            if (m_iPerson > CGameData::Inst()->getCntryInfo()->member_cur_num)
            {
                m_iPerson = CGameData::Inst()->getCntryInfo()->member_cur_num;
                char buf[30];
                snprintf(buf, 29, "%d",m_iPerson);
                m_textfiled3->setString(buf);
            }
            else if (m_iPerson < 1)//m_mubiaoGx.iPerson)
            {
                m_iPerson = 1;//m_mubiaoGx.iPerson;
                char buf[50];
                snprintf(buf, 49, "%d",m_iPerson);
                m_textfiled3->setString(buf);
            }
            break;
        case 4:
            m_iBaozhengCoin = atoi(m_textfiled4->getString());
            
            if (m_iBaozhengCoin > 99999)
            {
                m_iBaozhengCoin = 99999;
                m_textfiled4->setString("99999");
            }
            else if (m_iBaozhengCoin < 1)
            {
                m_iBaozhengCoin = 1;
                m_textfiled4->setString("1");
            }
                
            break;
            
        default:
            break;
    }
    
    checkButtonFabu();
    
    switch (idx)
    {
        case 1:
            layerMoveUpOrDown(true);
            m_textfiled1->attachWithIME();
            m_textfiled2->detachWithIME();
            m_textfiled3->detachWithIME();
            m_textfiled4->detachWithIME();
            m_spBG1->setOpacity(150);
            m_spBG2->setOpacity(255);
            m_spBG3->setOpacity(255);
            m_spBG4->setOpacity(255);
            break;
        
        case 2:
            layerMoveUpOrDown(true);
            m_textfiled2->attachWithIME();
            m_textfiled1->detachWithIME();
            m_textfiled3->detachWithIME();
            m_textfiled4->detachWithIME();
            m_spBG2->setOpacity(150);
            m_spBG1->setOpacity(255);
            m_spBG3->setOpacity(255);
            m_spBG4->setOpacity(255);
            break;
            
        case 3:
            layerMoveUpOrDown(true);
            m_textfiled3->attachWithIME();
            m_textfiled2->detachWithIME();
            m_textfiled1->detachWithIME();
            m_textfiled4->detachWithIME();
            m_spBG3->setOpacity(150);
            m_spBG2->setOpacity(255);
            m_spBG1->setOpacity(255);
            m_spBG4->setOpacity(255);
            break;
            
        case 4:
            layerMoveUpOrDown(true);
            m_textfiled4->attachWithIME();
            m_textfiled2->detachWithIME();
            m_textfiled3->detachWithIME();
            m_textfiled1->detachWithIME();
            m_spBG4->setOpacity(150);
            m_spBG2->setOpacity(255);
            m_spBG3->setOpacity(255);
            m_spBG1->setOpacity(255);
            break;
            
        default:
            layerMoveUpOrDown(false);
            m_textfiled1->detachWithIME();
            m_textfiled2->detachWithIME();
            m_textfiled3->detachWithIME();
            m_textfiled4->detachWithIME();
            m_spBG1->setOpacity(255);
            m_spBG2->setOpacity(255);
            m_spBG3->setOpacity(255);
            m_spBG4->setOpacity(255);
            break;
    }
    
    m_iCurChoose = idx;
}


bool RewardRelease::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen) {
    CCLog("onTextFieldInsertText text = %s, nLen = %d, char count = %d char = %s", text, nLen, sender->getCharCount(), sender->getString());
    
    if (sender->getCharCount() + nLen > 9) {
        return true;
    }
    if (*text <= 57 && *text >= 48) {
        return false;
    }
    else {
        return true;
    }
}

bool RewardRelease::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    return false;
}

bool RewardRelease::bCanFabu()
{
    if (m_lcontribution < CGameData::Inst()->getDefaultCntryBu(1) ||
        m_ideadLine < 1||
        m_iPerson < 1||
        m_iBaozhengCoin < 1 ||
        m_iItemCount < 1)
        return false;
    
    return true;
}

void RewardRelease::checkButtonFabu()
{
    if (bCanFabu() && m_spDisMenu != NULL)
    {
        removeChild(m_spDisMenu, true);
        m_spDisMenu = NULL;
    }
    else if ((!bCanFabu()) && m_spDisMenu == NULL)
    {
        m_spDisMenu = CCSprite::spriteWithSpriteFrameName("fr_long button black.png");
        m_spDisMenu->setPosition(ccp(230,196));
        m_spDisMenu->setAnchorPoint(CCPointZero);
        addChild(m_spDisMenu,1);
    }
}

void RewardRelease::removeChooseLayer()
{
    removeChild(m_listBaokuChoose, true);
    m_listBaokuChoose = NULL;
    setPosition(CCPointZero);
    
    m_ret.iBtnSel = en_fabuFromChooseItem;
    (m_pListener->*m_pfnSelector)(&m_ret);
}

void RewardRelease::cbFromChooseBaoku(CCObject* pObj)
{
    BaokuChooseRet* ret = (BaokuChooseRet*)pObj;
    
    if (ret->m_iType == 1)
    {
        m_ret.iBtnSel = en_toBaoKuFromCFabu;
        (m_pListener->*m_pfnSelector)(&m_ret);
        return;
    }
    
    removeChooseLayer();

    CCPoint pos = m_spJiangli->getPosition();
    
    removeChild(m_spJiangli, true);
    
    m_spJiangli = CCSprite::spriteWithFile(ret->strImgName.c_str());
    m_spJiangli->setAnchorPoint(CCPointMake(0, 1));
    m_spJiangli->setPosition(pos);
    addChild(m_spJiangli);
    
    char buf[100];
    snprintf(buf, 99,"/%ld",ret->lrewardNum);
    
    CCSprite* spTmp = CCSprite::spriteWithFile("rewardnum.png");
    if(spTmp){
        CCSize sz = spTmp->getContentSize();
        CCLabelAtlas* lbItemCount = CCLabelAtlas::labelWithString(buf,
                                                                  "rewardnum.png",
                                                                  sz.width / 11,
                                                                  sz.height,
                                                                  '/');
        lbItemCount->setAnchorPoint(ccp(0.5, 0));
        lbItemCount->setPosition(CCPointMake(m_spJiangli->getContentSize().width/2, 0));
        m_spJiangli->addChild(lbItemCount, 1);
    }
    
    m_strItemId = ret->strItemId;
    m_iItemCount = ret->lrewardNum;
    m_strItemName = ret->strItemName;
    
    checkButtonFabu();
}




