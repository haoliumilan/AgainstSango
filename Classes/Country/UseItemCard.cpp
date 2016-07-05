//
//  UseItemCard.cpp
//  AgainstWar
//
//  Created by Hao Zhang on 13-4-12.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "UseItemCard.h"
#include "SimpleAudioEngine.h"
#include "TextNode.h"
#include "CGameData.h"
#include "Dialog2.h"

using namespace CocosDenshion;

#define enFrLBtn_Width      188
#define enFrLBtn_Height     79

static CCPoint butPosition[3] = {
    CCPointMake(320, 425), CCPointMake(320, 325), CCPointMake(320, 225),
};

static CCPoint butPosition2[3] = {
    CCPointMake(320, 425), CCPointMake(-3000, -3000), CCPointMake(320, 325),
};


static CCPoint butPosition3[3] = {
    CCPointMake(-3000, -3000), CCPointMake(-3000, -3000), CCPointMake(320, 325),
};

//******************************************************************************
// Constructor
//******************************************************************************
UseItemCard::UseItemCard(CCObject* target, SEL_CallFuncO selector, ItemCardInf* itemCard,string cardsType,int needBatch)
{
    m_ret.lCnt = itemCard->lCnt;
    m_needBatch = needBatch;
    m_pListener = target;
    m_pfnSelector = selector;

    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, true); //-130
    
    m_mvLayer = CCLayer::node();
    addChild(m_mvLayer);
    
    //三按钮
    const char* firstName;
    const char* secondName;
    if(cardsType=="stone"){
        firstName = CGameData::Inst()->getLanguageValue("cntry_new_donate");
        secondName = CGameData::Inst()->getLanguageValue("cntry_new_donateall");
    }else{
        firstName = CGameData::Inst()->getLanguageValue("cntry_item_use");
        secondName = CGameData::Inst()->getLanguageValue("cntry_item_useall");
    }
    const char* title[] = {
        firstName,
        secondName,
        CGameData::Inst()->getLanguageValue("con_back_")
    };
    
    for (int i = 0; i < 3; i++) {
        
//        if(cardsType=="compound" && i == 1)
//            continue;
        
        
        
        
        CCSprite* spBtn = CCSprite::spriteWithSpriteFrameName("fr_long button.png");
        if (spBtn) {
                TextNode* tnTemp = TextNode::textWithString(title[i],
                                                            spBtn->getContentSize(),
                                                            CCTextAlignmentCenter,
                                                            36);
                tnTemp->setShadowColor(ccBLACK);
                tnTemp->setPosition(CCPointMake(spBtn->getContentSize().width * 0.5,
                                                spBtn->getContentSize().height * 0.5));
                spBtn->addChild(tnTemp);

            if (m_needBatch == 1)
            {
                spBtn->setPosition(butPosition[i]);
                m_mvLayer->addChild(spBtn);
            }
            else if(m_needBatch == 2)
            {
                spBtn->setPosition(butPosition3[i]);
                m_mvLayer->addChild(spBtn);
            }
            else
            {
                spBtn->setPosition(butPosition2[i]);
                m_mvLayer->addChild(spBtn);
            }
        }
    }
    
    //头像
    CCSprite* spCard = CCSprite::spriteWithFile(itemCard->strResName.c_str());
    if (spCard)
    {
        spCard->setPosition(CCPointMake(226, 574));
        m_mvLayer->addChild(spCard);
        
        char buf[30];
        sprintf(buf, "/%ld",itemCard->lCnt);
        
        CCSprite* spTmp = CCSprite::spriteWithFile("rewardnum.png");
        if(spTmp){
            CCSize sz = spTmp->getContentSize();
            CCLabelAtlas* textNum = CCLabelAtlas::labelWithString(buf,
                                                                  "rewardnum.png",
                                                                  sz.width / 11,
                                                                  sz.height,
                                                                  '/');
            textNum->setAnchorPoint(ccp(0.5, 0.5));
            textNum->setPosition(ccp(54,10));
            spCard->addChild(textNum,0,9);
        }
        
    }
    
    TextNode* lbText = TextNode::textWithString(itemCard->strItemName.c_str(), 25);
    lbText->setPosition(CCPointMake(320, 676));
    lbText->setColor(ccWHITE);
    m_mvLayer->addChild(lbText);
    
    float PosY = 620;
    
    lbText = TextNode::textWithString(itemCard->strItemDsp.c_str(), CCSizeMake(180, 46), CCTextAlignmentLeft, 20);
    lbText->setPosition(CCPointMake(282, PosY));
    lbText->setAnchorPoint(ccp(0, 1));
    lbText->setColor(ccWHITE);
    m_mvLayer->addChild(lbText);
    
    TextNode* testText = TextNode::textWithString(itemCard->strItemDsp.c_str(), 20);
    if (testText->getContentSize().width > 180)
    {
        PosY -= (testText->getContentSize().height*2+6);
    }
    else
    {
        PosY -= (testText->getContentSize().height*1+16);
    }
    
    char buf[100];
    snprintf(buf,99, CGameData::Inst()->getLanguageValue("cntry_itemcardnum"),itemCard->lCnt);
    lbText = TextNode::textWithString(buf, 20);
    lbText->setPosition(CCPointMake(282, PosY));
    lbText->setAnchorPoint(ccp(0, 1));
    lbText->setColor(ccWHITE);
    m_mvLayer->addChild(lbText);
    
    PosY -= (lbText->getContentSize().height+6);
    
    if (itemCard->lNeedLv >0)
    {
        snprintf(buf,99, CGameData::Inst()->getLanguageValue("cntry_itemcardlvneed"),itemCard->lNeedLv);
        lbText = TextNode::textWithString(buf, 20);
        lbText->setPosition(CCPointMake(282, PosY));
        lbText->setAnchorPoint(ccp(0, 1));
        
        if (itemCard->lNeedLv>CGameData::Inst()->getUsrInfo()->lv)
        {
            lbText->setColor(ccRED);
        }
        else
        {
            lbText->setColor(ccWHITE);
        }
        m_mvLayer->addChild(lbText);
        
        PosY -= (lbText->getContentSize().height+6);
    }
    
    if (itemCard->lNeedCbu >0)
    {
        snprintf(buf,99,CGameData::Inst()->getLanguageValue("cntry_itemcardcbuneed"),itemCard->lNeedCbu);
        lbText = TextNode::textWithString(buf, 20);
        lbText->setPosition(CCPointMake(282, PosY));
        lbText->setAnchorPoint(ccp(0, 1));
        
        if (itemCard->lNeedCbu>CGameData::Inst()->getCntryInfo()->user_contribution)
        {
            lbText->setColor(ccRED);
        }
        else
        {
            lbText->setColor(ccWHITE);
        }
        m_mvLayer->addChild(lbText);

        PosY -= (lbText->getContentSize().height+6);
    }
    
    m_mvLayer->setPosition(CCPointMake(640, 0));
    m_mvLayer->runAction(CCEaseElasticOut::actionWithAction(CCMoveTo::actionWithDuration(kUiActionDur1, CCPointMake(0, 0)), 0.8f));
    
    //半透明层
    CCLayerColor* lcCover = CCLayerColor::layerWithColorWidthHeight( ccc4(0, 0, 0, 196), 640, 960);
	addChild(lcCover, -10);
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool UseItemCard::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    bool bRet = isSelectedForPos(pos, 3);
    
    if (bRet) {
        if(CGameData::Inst()->isSoundOn)
            SimpleAudioEngine::sharedEngine()->playEffect(kSndEffect(btnok));

        (m_pListener->*m_pfnSelector)(&m_ret);
    }
    
    return true;
}

//******************************************************************************
// isSelectedForPos
//******************************************************************************
bool UseItemCard::isSelectedForPos(CCPoint pos, int btnCount)
{
    bool bRet;
    
    m_ret.iBtnSel = kInvalidValue;
    
    if (m_needBatch == 1)
    {
        for (int i = 0; i < btnCount; i++) {
            bRet = CCRect::CCRectContainsPoint(CCRectMake(butPosition[i].x - enFrLBtn_Width * 0.5,
                                                          butPosition[i].y - enFrLBtn_Height * 0.5,
                                                          enFrLBtn_Width,
                                                          enFrLBtn_Height),
                                               pos);
            if (bRet) {
                m_ret.iBtnSel = i;
                return bRet;
            }
        }
    }
    else if(m_needBatch == 2)
    {
        for (int i = 0; i < btnCount; i++) {
            bRet = CCRect::CCRectContainsPoint(CCRectMake(butPosition3[i].x - enFrLBtn_Width * 0.5,
                                                          butPosition3[i].y - enFrLBtn_Height * 0.5,
                                                          enFrLBtn_Width,
                                                          enFrLBtn_Height),
                                               pos);
            if (bRet) {
                m_ret.iBtnSel = i;
                return bRet;
            }
        }
    }
    else
    {
        for (int i = 0; i < btnCount; i++) {
            bRet = CCRect::CCRectContainsPoint(CCRectMake(butPosition2[i].x - enFrLBtn_Width * 0.5,
                                                          butPosition2[i].y - enFrLBtn_Height * 0.5,
                                                          enFrLBtn_Width,
                                                          enFrLBtn_Height),
                                               pos);
            if (bRet) {
                m_ret.iBtnSel = i;
                return bRet;
            }
        }
    }
    
    return false;
}




#pragma mark -
#pragma mark 批量选取


void BatchUseCard::exit()
{
    setPosition(ccp(-700, 0));
    removeDispacher();
}

void BatchUseCard::onExit()
{
    CCLayer::onExit();
}

BatchUseCard::BatchUseCard(CCObject* target, SEL_CallFuncO selector,int lCnt,string useCardType)
{
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -135, true);
    
    m_lCnt = lCnt;
    cardType = useCardType;
    m_pListener = target;
    m_pfnSelector = selector;
    
    m_background = CCSprite::spriteWithFile("fr_window3.png");
    m_background->setScaleX(2.2);
    m_background->setScaleY(2.3);
    m_background->setPosition(ccp(320, 0));
    addChild(m_background);
    
    CCSize size ;
    size.height = 80;
    size.width = 640;
    
    if (useCardType=="stone") {
        TextNode *tintLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_input_num_of_stone"), 30);
        addChild(tintLabel,10);
        tintLabel->setPosition(ccp(320, 116));
        tintLabel->setColor(ccBLACK);
        
        tintLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_input_num_of_stone_detail"),25);
        tintLabel->setPosition(CCPointMake(320, 81));
        tintLabel->setColor(ccRED);
        addChild(tintLabel);
        
        textField = CCTextFieldTTF::textFieldWithPlaceHolder("",size ,CCTextAlignmentCenter,"huakangfont.ttf", 26);
        textField->setPosition(ccp(320, 0));
        textField->setColor(ccWHITE);
        
        textField->setDelegate(this);
        addChild(textField,10);
        
        CCSprite *myIdBack = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
        myIdBack->setPosition(ccp(320, 0));
        addChild(myIdBack);
        
        m_confirmBtn = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        m_cancelBtn = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        m_confirmBtn->setTag(1);
        m_confirmBtn->setPosition(ccp(170,-86));
        m_cancelBtn->setTag(2);
        m_cancelBtn->setPosition(ccp(470,-86));
        
        
        TextNode* itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"), kBtnTitleHeight);
        itemLabel->setColor(ccWHITE);
        itemLabel->setShadowColor(ccBLACK);
        itemLabel->setAnchorPoint(ccp(0.5,0.5));
        itemLabel->setPosition(CCPoint(m_confirmBtn->getContentSize().width/2-2, m_confirmBtn->getContentSize().height/2+4));
        m_confirmBtn->addChild(itemLabel);
        addChild(m_confirmBtn);
        
        TextNode* itemLabel2 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cancelTip"), kBtnTitleHeight);
        itemLabel2->setColor(ccWHITE);
        itemLabel2->setShadowColor(ccBLACK);
        itemLabel2->setAnchorPoint(ccp(0.5,0.5));
        itemLabel2->setPosition(CCPoint(m_cancelBtn->getContentSize().width/2-2, m_cancelBtn->getContentSize().height/2+4));
        m_cancelBtn->addChild(itemLabel2);
        addChild(m_cancelBtn);
        
        char buf[100];
        lCnt = 0;
        //strItem = "1101_item";
        CCMutableArray<ItemCardInf* > * itemCardInf =  CGameData::Inst()->getItemCardPkInf();
        for (int i = 0 ; i < itemCardInf->count(); i ++) //i < itemCardInf->count()
        {
            if (itemCardInf->getObjectAtIndex(i)->strType=="stone") {
                lCnt = itemCardInf->getObjectAtIndex(i)->lCnt;
                //strItem = itemCardInf->getObjectAtIndex(i)->strItemId;
                break;
            }
        }
        snprintf(buf,99, CGameData::Inst()->getLanguageValue("cntry_num_of_yuanbao_and_stone"),CGameData::Inst()->getUsrInfo()->can_contribute_coin>CGameData::Inst()->getUsrInfo()->coin?CGameData::Inst()->getUsrInfo()->coin:CGameData::Inst()->getUsrInfo()->can_contribute_coin,lCnt);
        itemLabel = TextNode::textWithString(buf,
                                            25);
        itemLabel->setPosition(CCPointMake(320, 51));
        itemLabel->setColor(ccRED);
        addChild(itemLabel);
    }else{
    
        TextNode *tintLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_usebatchitem"), 39);
        addChild(tintLabel,10);
        tintLabel->setPosition(ccp(320, 86));
        tintLabel->setColor(ccBLACK);
    
        textField = CCTextFieldTTF::textFieldWithPlaceHolder("",size ,CCTextAlignmentCenter,"huakangfont.ttf", 26);
        textField->setPosition(ccp(320, 0));
        textField->setColor(ccWHITE);
    
        textField->setDelegate(this);
        addChild(textField,10);
    
        CCSprite *myIdBack = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
        myIdBack->setPosition(ccp(320, 0));
        addChild(myIdBack);

        m_confirmBtn = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        m_cancelBtn = CCSprite::spriteWithSpriteFrameName("fr_button.png");
        m_confirmBtn->setTag(1);
        m_confirmBtn->setPosition(ccp(170,-86));
    
        TextNode* itemLabel = TextNode::textWithString(CGameData::Inst()->getLanguageValue("sureTip"), kBtnTitleHeight);
        itemLabel->setColor(ccWHITE);
        itemLabel->setShadowColor(ccBLACK);
        itemLabel->setAnchorPoint(ccp(0.5,0.5));
        itemLabel->setPosition(CCPoint(m_confirmBtn->getContentSize().width/2-2, m_confirmBtn->getContentSize().height/2+4));
        m_confirmBtn->addChild(itemLabel);
        addChild(m_confirmBtn);
    
        m_cancelBtn->setTag(2);
        m_cancelBtn->setPosition(ccp(470,-86));
    
        TextNode* itemLabel2 = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cancelTip"), kBtnTitleHeight);
        itemLabel2->setColor(ccWHITE);
        itemLabel2->setShadowColor(ccBLACK);
        itemLabel2->setAnchorPoint(ccp(0.5,0.5));
        itemLabel2->setPosition(CCPoint(m_cancelBtn->getContentSize().width/2-2, m_cancelBtn->getContentSize().height/2+4));
        m_cancelBtn->addChild(itemLabel2);
        addChild(m_cancelBtn);
    }
//    isAttach = true;
    CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardNumber();
    textField->attachWithIME();
}


BatchUseCard::~BatchUseCard()
{
    removeDispacher();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    
}


void BatchUseCard::menuCallback(CCObject *pSender)
{
    Dialog2Ret* ret = (Dialog2Ret*) pSender;
    
    if (ret->bOk)
    {
        m_ret.iBtnSel = atoi(textField->getString());
        (m_pListener->*m_pfnSelector)(&m_ret);
    }
    else
    {
        removeFromParentAndCleanup(true);
    }
    
 //   (m_pListener->*m_pfnSelector)(&m_ret);
    
    // bool schFriendRequest(const char* fid);
    
}

void BatchUseCard::removeDispacher()
{
    textField->detachWithIME();
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    CCEGLView * pGlView = CCDirector::sharedDirector()->getOpenGLView();
    pGlView->setIMEKeyboardDefault();
   // isAttach = false;
    
}

bool BatchUseCard::ccTouchBegan(CCTouch *touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    if (CCRect::CCRectContainsPoint(CCRectMake(m_confirmBtn->getPosition().x- m_confirmBtn->getContentSize().width/2,
                                                  m_confirmBtn->getPosition().y- m_confirmBtn->getContentSize().height/2,
                                                  m_confirmBtn->getContentSize().width,
                                                  m_confirmBtn->getContentSize().height),
                                       pos))
    {
        textField->detachWithIME();
        m_ret.iBtnSel = atoi(textField->getString());
        (m_pListener->*m_pfnSelector)(&m_ret);
//        Dialog2* dlg2 = new Dialog2(this, callfuncO_selector(BatchUseCard::menuCallback), CGameData::Inst()->getLanguageValue("cntry_querenuse"), CGameData::Inst()->getLanguageValue("cntry_askusemore"),enDl2TwoBtn);
//        addChild(dlg2,15,888);
//        dlg2->release();
//        dlg2->setPosition(ccp(320,-70));
    }
    else if (CCRect::CCRectContainsPoint(CCRectMake(m_cancelBtn->getPosition().x- m_cancelBtn->getContentSize().width/2,
                                                    m_cancelBtn->getPosition().y- m_cancelBtn->getContentSize().height/2,
                                                    m_cancelBtn->getContentSize().width,
                                                    m_cancelBtn->getContentSize().height),
                                         pos))
    {
   //     textField->detachWithIME();
        removeFromParentAndCleanup(true);
    }


//  //  else
//    {
//    
//        if(isAttach == false)
//        {
//            textField->attachWithIME();
//        }
//        else {
//            textField->detachWithIME();
//        }
//        isAttach = !isAttach;
//    }
    
    return true;
}

#pragma mark -
#pragma mark CCTextFieldDelegate
bool BatchUseCard::onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen) {
    CCLog("onTextFieldInsertText text = %s, nLen = %d, char count = %d char = %s", text, nLen, sender->getCharCount(), sender->getString());
    if(cardType!="stone"){
        char buf[100];
        snprintf(buf, 99,"%s%s", sender->getString(),text);
        int  num = atoi(buf);
        if (num > m_lCnt) {
            char lCnt[100];
            snprintf(lCnt, 99, "%d" , m_lCnt);
            sender->setString(lCnt);
            return true;
        }
    }
    
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

bool BatchUseCard::onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
{
    return false;
}





