//
//  ParcelPageTwo.cpp
//  AgainstWar
//
//  Created by AllenSK on 14-2-13.
//  Copyright (c) 2014年 OneClick Co.,Ltd. All rights reserved.
//

#include "ParcelPageTwo.h"

#include "ParcelScrewSelf.h"
#include "ParcelScrew.h"
#include "CommDlg.h"


#define kParceHeadIconTAG    58



ParcelPageTwo::ParcelPageTwo(CCObject* target, SEL_CallFuncO selector)
{
    
    ExpLayer = NULL;
    m_bIspress = false;
    m_pListener = target;
    m_pfnSelector = selector;
    
    
   

    
    
}


ParcelPageTwo::~ParcelPageTwo()
{
    
}


void ParcelPageTwo::showInfo(int num)
{

    crtExpLayer();
    
    
    
    
    if(m_pType == 1)
    {

        CCMutableArray<ParcelChildInfo* > * parcelChildInfoArr =  CGameData::Inst()->getParcelChildInfoAry();
        
        ParcelChildInfo * tempPar = parcelChildInfoArr->getObjectAtIndex(m_pNum);

        
        
        
        CCMutableArray<ParcelNeedElementInfo* > * parcelNeedElementInfoArr =  tempPar->m_ParcelNeedElementInfo;
        
        if(num >= parcelNeedElementInfoArr->count())
            return;
        
        ParcelNeedElementInfo * temp = parcelNeedElementInfoArr->getObjectAtIndex(num);
        
        
        
        ItemCardInf * tempBaseInfo = (ItemCardInf*)CGameData::Inst()->getItemCardByID(temp->objId);
        
        
        
        TextNode * curLevel = TextNode::textWithString(tempBaseInfo->strItemName.c_str(), CCSize(400,100), CCTextAlignmentCenter, 32);
        ExpLayer->addChild(curLevel, 10);
        curLevel->setPosition(ccp(169,52));
    }
    else
    {
        CCMutableArray<ParcelItemInfo* > * parcelItemInfoArr =  CGameData::Inst()->getParcelItemInfoAry();
        
        ParcelItemInfo * tempPar = parcelItemInfoArr->getObjectAtIndex(m_pNum);
        


        
        CCMutableArray<ParcelNeedElementInfo* > * parcelNeedElementInfoArr =  tempPar->m_ParcelNeedElementInfo;
        
        if(num >= parcelNeedElementInfoArr->count())
            return;
        
        ParcelNeedElementInfo * temp = parcelNeedElementInfoArr->getObjectAtIndex(num);
        

        
        ItemCardInf * tempBaseInfo = (ItemCardInf*)CGameData::Inst()->getItemCardByID(temp->objId);
        
        
        
        TextNode * curLevel = TextNode::textWithString(tempBaseInfo->strItemName.c_str(), CCSize(400,100), CCTextAlignmentCenter, 32);
        ExpLayer->addChild(curLevel, 10);
        curLevel->setPosition(ccp(169,52));
        
        
    }
    
    
    
    
    
    
    ExpLayer->setPosition(ccp(320 , 480 + 120));
    ExpLayer->runAction(CCScaleTo::actionWithDuration(0.1f, 1.0f));
}

void ParcelPageTwo::crtExpLayer()
{
    ExpLayer = CCSprite::spriteWithSpriteFrameName("propertyTip_dlg.png");
    ExpLayer->setPosition(ccp(220, 832+(ExpLayer->getContentSize().height/2)));
    addChild(ExpLayer, 100);
    

    ExpLayer->setScale(0);
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
bool ParcelPageTwo::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (m_showCardInf->getIsVisible()) {
        return false;
    }
    CCPoint touchLocation = touch->locationInView(touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    checkTouchHeadIcon(pos);
    
    if(pos.y >= 431 && pos.y <= 521)
    {
        if(pos.x >= 46 && pos.x <= 135)
        {
            showInfo(0);
            return true;
        }
        else if(pos.x >= 160 && pos.x <= 246)
        {
            showInfo(1);
            return true;
        }
        else if(pos.x >= 277 && pos.x <= 366)
        {
            showInfo(2);
            return true;
        }
        else if(pos.x >= 391 && pos.x <= 481)
        {
            showInfo(3);
            return true;
        }
        else if(pos.x >= 506 && pos.x <= 595)
        {
            showInfo(4);
            return true;
        }
    }
    if (m_bIspress) {
        return true;
    }else
        return false;
}

//******************************************************************************
// ccTouchMoved
//******************************************************************************
void ParcelPageTwo::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    
}

//******************************************************************************
// ccTouchEnded
//******************************************************************************
void ParcelPageTwo::ccTouchEnded(CCTouch* touch, CCEvent* event)
{

//        ExpLayer->removeFromParentAndCleanup(true);

    if (m_bIspress) {
        CCMutableArray<ParcelChildInfo* > * parcelChildInfoArr =  CGameData::Inst()->getParcelChildInfoAry();
        ParcelChildInfo * tempPar = parcelChildInfoArr->getObjectAtIndex(m_pNum);
        short tempSht = atoi(tempPar->targetId.c_str());
        CCardBaseInfo * tempBaseInfo = (CCardBaseInfo*)CGameData::Inst()->getCardBaseInfByCid(tempSht);
        if (tempBaseInfo)
            m_showCardInf->show(tempBaseInfo);
    }else
       ExpLayer->removeFromParentAndCleanup(true);
    
    
}


void ParcelPageTwo::checkTouchHeadIcon(CCPoint pos)
{
    m_bIspress = false;
    CCNode* node = getChildByTag(kParceHeadIconTAG);
    if (node) {
        CCPoint pt = node->getPosition();
        CCSize sz = node->getContentSize();
        bool bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5,
                                                           pt.y - sz.height * 0.5,
                                                           sz.width,
                                                           sz.height),
                                                pos);
        if (bRet) {
            m_bIspress = true;
        }
    }

}


//******************************************************************************
// onExit
//******************************************************************************
void ParcelPageTwo::exit()
{
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("exchange.plist");
    
    CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
    
    removeAllChildrenWithCleanup(true);
    
    
}


void ParcelPageTwo::enter()
{
    
    canParcel = true;
    
    aniPlayIng = false;
    
    success = false;
    
    setIsTouchEnabled(true);
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, -130, true);

    
    
    
    //title
    m_spTitle = TitleBar::spriteWithSpriteFrameName("id_scene display.png");
    addChild(m_spTitle, 100);
    

    
    if(m_pType == 1)
    m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("Parcel_strChild"));
    else
        m_spTitle->setTitleString(CGameData::Inst()->getLanguageValue("Parcel_strItem"));
    
    
//    m_spTitle->setColor(ccc3(255, 229, 101));
    m_spTitle->setPosition(CCPointMake(320, 766));

    


    
    
    CCSprite * temp  = CCSprite::spriteWithFile("parcal_background.png");
    
    temp->setPosition(CCPointMake(320, 470));
    
    addChild(temp );
    
    
    m_showCardInf = new CShowCardInf();
    addChild(m_showCardInf, 301);
    m_showCardInf->release();
    m_showCardInf->setIsVisible(false);
    
    
    
    
    if(m_pType == 1)
        showChild(m_pNum);
    else
        showItem(m_pNum);
    


    
}

void ParcelPageTwo::showItem(int num)
{
    CCMutableArray<ParcelItemInfo* > * parcelItemInfoArr =  CGameData::Inst()->getParcelItemInfoAry();
    
    ParcelItemInfo * tempPar = parcelItemInfoArr->getObjectAtIndex(num);
    
    ItemCardInf * tempBaseInfo = (ItemCardInf*)CGameData::Inst()->getItemCardByID(tempPar->targetId);
    
    CCSprite* tempItem = CCSprite::spriteWithFile(tempBaseInfo->strResName.c_str());//头像显示
    
    
    
    tempItem->setPosition(CCPointMake(93, 644));
    
    addChild(tempItem );
    
    
    
    TextNode* name = TextNode::textWithString(tempBaseInfo->strItemName.c_str(), CCSizeMake(300, 80), CCTextAlignmentLeft, 26);
    name->setColor(ccc3(120, 26, 5));
    name->setAnchorPoint(CCPointMake(0.5, 0.5));
    name->setPosition(CCPointMake(298, 656));
    addChild(name , 100);
    
    
    
    
    TextNode* info = TextNode::textWithString(tempBaseInfo->strItemDsp.c_str(), CCSizeMake(420, 80), CCTextAlignmentLeft, 24);
    info->setColor(ccBLACK);
    info->setAnchorPoint(CCPointMake(0.5, 0.5));
    info->setPosition(CCPointMake(298 + 65, 610));
    addChild(info , 100);
    
    
    CUserInfo* pUsrInf = CGameData::Inst()->getUsrInfo();
    

    
    TextNode* tongQian = TextNode::textWithString(CGameData::Inst()->getLanguageValue("Parcel_tongQian"), CCSizeMake(300, 80), CCTextAlignmentLeft, 26);
    tongQian->setColor(ccc3(120, 26, 5));
    tongQian->setAnchorPoint(CCPointMake(0.5, 0.5));
    tongQian->setPosition(CCPointMake(208, 322));
    addChild(tongQian , 100);
    
    
    char buf[100];
    snprintf(buf, 99,"%qu/%ld", pUsrInf->gold, tempPar->gold );

    
    
    
    
    
    TextNode* tongQianNum = TextNode::textWithString(buf, CCSizeMake(420, 80), CCTextAlignmentLeft, 26);
    tongQianNum->setColor(ccBLACK);
    if(tempPar->gold > pUsrInf->gold)
    {
        tongQianNum->setColor(ccRED);
        canParcel = false;
    }
    
    tongQianNum->setAnchorPoint(CCPointMake(0.5, 0.5));
    tongQianNum->setPosition(CCPointMake(208 + 130 + 60, 322));
    addChild(tongQianNum , 100);

    
    
    
    
    TextNode* geRenJunLiang = TextNode::textWithString(CGameData::Inst()->getLanguageValue("Parcel_geRenYuanBao"), CCSizeMake(300, 80), CCTextAlignmentLeft, 26);
    geRenJunLiang->setColor(ccc3(120, 26, 5));
    geRenJunLiang->setAnchorPoint(CCPointMake(0.5, 0.5));
    geRenJunLiang->setPosition(CCPointMake(208, 322 - 30));
    addChild(geRenJunLiang , 100);
    
    
    
    CCountryInfo* pConInf = CGameData::Inst()->getCntryInfo();
    
    

    
    char buf1[100];
    snprintf(buf1, 99,"%ld/%ld" , pConInf->user_supply, tempPar->wealth);
    
    
    TextNode* geRenJunLiangNum = TextNode::textWithString(buf1, CCSizeMake(420, 80), CCTextAlignmentLeft, 26);
    geRenJunLiangNum->setColor(ccBLACK);
    
    if(tempPar->wealth > pConInf->user_supply)
    {
        geRenJunLiangNum->setColor(ccRED);
        canParcel = false;
    }
    
    geRenJunLiangNum->setAnchorPoint(CCPointMake(0.5, 0.5));
    geRenJunLiangNum->setPosition(CCPointMake(208 + 130 + 60, 322 - 30));
    addChild(geRenJunLiangNum , 100);

    
    
    TextNode* yuanBao = TextNode::textWithString(CGameData::Inst()->getLanguageValue("Parcel_yuanBao"), CCSizeMake(300, 80), CCTextAlignmentLeft, 26);
    yuanBao->setColor(ccc3(120, 26, 5));
    yuanBao->setAnchorPoint(CCPointMake(0.5, 0.5));
    yuanBao->setPosition(CCPointMake(208, 322 - 30 * 2));
    addChild(yuanBao , 100);


    
    
    char buf2[100];
    snprintf(buf2, 99,"%ld/%ld", pUsrInf->coin, tempPar->coin );
    
    
    TextNode* yuanBaoNum = TextNode::textWithString(buf2, CCSizeMake(420, 80), CCTextAlignmentLeft, 26);
    yuanBaoNum->setColor(ccBLACK);
    
    if(tempPar->coin > pUsrInf->coin)
    {
        yuanBaoNum->setColor(ccRED);
        canParcel = false;
    }
    
    yuanBaoNum->setAnchorPoint(CCPointMake(0.5, 0.5));
    yuanBaoNum->setPosition(CCPointMake(208 + 130 + 60, 322 - 30 * 2));
    addChild(yuanBaoNum , 100);

    
    
    

    
    CCMutableArray<ParcelNeedElementInfo* > * parcelNeedElementInfoArr =  tempPar->m_ParcelNeedElementInfo;
    
    for(int i = 0 ; i < parcelNeedElementInfoArr->count() ; i ++)
    {
        
        ParcelNeedElementInfo * temp = parcelNeedElementInfoArr->getObjectAtIndex(i);
        
        ItemCardInf * tempBaseInfo = (ItemCardInf*)CGameData::Inst()->getItemCardByID(temp->objId);
        
        CCSprite* tempItem = CCSprite::spriteWithFile(tempBaseInfo->strResName.c_str());//头像显示

        tempItem->setPosition(CCPointMake(88 + i * 115, 480));
        
        addChild(tempItem );
        
        
        
        
        CCMutableArray<ItemCardInf* > * itemCardInf =  CGameData::Inst()->getItemCardPkInf();
        
        int haveNum = 0;
        
        for (int i = 0 ; i < itemCardInf->count(); i ++) //i < itemCardInf->count()
        {
            if (itemCardInf->getObjectAtIndex(i)->strItemId == temp->objId) {

                haveNum = itemCardInf->getObjectAtIndex(i)->lCnt;
                
                break;
            }
        }
        
        
        
        
        
        
        char buf[100];
        snprintf(buf, 99,CGameData::Inst()->getLanguageValue("Parcel_have"), haveNum);

        
        TextNode* have = TextNode::textWithString(buf, CCSizeMake(300, 80), CCTextAlignmentLeft, 22);
        have->setColor(ccWHITE);
        have->setAnchorPoint(CCPointMake(0.5, 0.5));
        have->setPosition(CCPointMake(188 + i * 115, 390 + 25));
        addChild(have , 100);
        
        
        
        
        
        char buf1[100];
        snprintf(buf1, 99,CGameData::Inst()->getLanguageValue("Parcel_need"), temp->num);

        
        TextNode* need = TextNode::textWithString(buf1, CCSizeMake(300, 80), CCTextAlignmentLeft, 22);
        need->setColor(ccWHITE);
        
        if(temp->num > haveNum)
        {
            need->setColor(ccRED);
            canParcel = false;
        }
        
        need->setAnchorPoint(CCPointMake(0.5, 0.5));
        need->setPosition(CCPointMake(188 + i * 115, 368 + 25));
        addChild(need , 100);



        
        
    }

    
    
    
    //back button
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    m_miBack = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(ParcelPageTwo::btnCallback) );
    m_miBack->setTag(3);
    m_miBack->setAnchorPoint(CCPointZero);
    
    m_miBack->setPosition(CCPointMake(1, 731));
    
    
    spMenu1 = CCSprite::spriteWithFile("parcal_button.png");
    
    if(!canParcel)
    spMenu1->setColor(ccGRAY);
    
    spMenu2 = CCSprite::spriteWithFile("parcal_button.png");
    
    if(!canParcel)
    spMenu2->setColor(ccGRAY);
    
    m_HeCheng = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(ParcelPageTwo::btnCallHeCheng) );
    m_HeCheng->setTag(4);
    m_HeCheng->setAnchorPoint(CCPointZero);
    
    m_HeCheng->setPosition(CCPointMake(224, 149));
    
    
    CCMenu * m_menu = CCMenu::menuWithItems( m_miBack, m_HeCheng , NULL);
    m_menu->setPosition(CCPointZero);
    addChild(m_menu, 300);

    
    
}
void ParcelPageTwo::showChild(int num)
{
    CCMutableArray<ParcelChildInfo* > * parcelChildInfoArr =  CGameData::Inst()->getParcelChildInfoAry();
    
    ParcelChildInfo * tempPar = parcelChildInfoArr->getObjectAtIndex(num);
    
    
    short tempSht = atoi(tempPar->targetId.c_str());
    
    CCardBaseInfo * tempBaseInfo = (CCardBaseInfo*)CGameData::Inst()->getCardBaseInfByCid(tempSht);
    
    
    CCSprite* tempItem = CGameData::Inst()->getHeadSprite(tempBaseInfo->cid);//头像显示
    
    tempItem->setPosition(CCPointMake(93, 644));
    
    tempItem->setTag(kParceHeadIconTAG);
    
    addChild(tempItem );
    
    
    
    char buff1[100];
    snprintf(buff1, 99,CGameData::Inst()->getLanguageValue("Parcel_LV") , tempPar->lv);
    
    TextNode* LV = TextNode::textWithString(buff1, CCSizeMake(300, 80), CCTextAlignmentLeft, 26);
    LV->setColor(ccc3(120, 26, 5));
    LV->setAnchorPoint(CCPointMake(0.5, 0.5));
    LV->setPosition(CCPointMake(298 + 5, 653));
    addChild(LV , 100);
    
    
    
    long hpbase = tempBaseInfo->hp;
    float fHpGrowth = tempBaseInfo->hp_growth;
    
    long tt = (long)(hpbase + (tempPar->lv - 1) * fHpGrowth);
    
    
    
    char buff2[100];
    snprintf(buff2, 99,CGameData::Inst()->getLanguageValue("Parcel_HP"),tt );
    
    TextNode* HP = TextNode::textWithString(buff2, CCSizeMake(300, 80), CCTextAlignmentLeft, 26);
    HP->setColor(ccc3(120, 26, 5));
    HP->setAnchorPoint(CCPointMake(0.5, 0.5));
    HP->setPosition(CCPointMake(298 + 5, 653 - 26));
    addChild(HP , 100);
    
    
    
    
    long recoverbase = tempBaseInfo->recover;
    float fRecoverGrowth = tempBaseInfo->recover_growth;
    
    long tt1 = (long)(recoverbase + (tempPar->lv - 1) * fRecoverGrowth);
    
    char buff3[100];
    snprintf(buff3, 99,CGameData::Inst()->getLanguageValue("Parcel_res") ,
             tt1 );
    
    TextNode* RES  = TextNode::textWithString(buff3, CCSizeMake(300, 80), CCTextAlignmentLeft, 26);
    RES->setColor(ccc3(120, 26, 5));
    RES->setAnchorPoint(CCPointMake(0.5, 0.5));
    RES->setPosition(CCPointMake(298 + 5, 653 - 26 * 2));
    addChild(RES , 100);
    
    
    
    char buff4[100];
    snprintf(buff4, 99,CGameData::Inst()->getLanguageValue("Parcel_skillLV") , tempPar->skill_lv);
    
    TextNode* SKLV  = TextNode::textWithString(buff4, CCSizeMake(300, 80), CCTextAlignmentLeft, 26);
    SKLV->setColor(ccc3(120, 26, 5));
    SKLV->setAnchorPoint(CCPointMake(0.5, 0.5));
    SKLV->setPosition(CCPointMake(298 + 5 + 200, 653));
    addChild(SKLV , 100);
    
    long attackbase = tempBaseInfo->attack;
    float fAtkGrowth = tempBaseInfo->attack_growth;
    
    long tt2 = (long)(attackbase + (tempPar->lv - 1) * fAtkGrowth);
    
    char buff5[100];
    snprintf(buff5, 99,CGameData::Inst()->getLanguageValue("Parcel_atk") ,
             tt2);
    
    TextNode* ATK  = TextNode::textWithString(buff5, CCSizeMake(300, 80), CCTextAlignmentLeft, 26);
    ATK->setColor(ccc3(120, 26, 5));
    ATK->setAnchorPoint(CCPointMake(0.5, 0.5));
    ATK->setPosition(CCPointMake(298 + 5 + 200, 653 - 26));
    addChild(ATK , 100);

    
    
    

    
    
    CUserInfo* pUsrInf = CGameData::Inst()->getUsrInfo();
    
    
    
    TextNode* tongQian = TextNode::textWithString(CGameData::Inst()->getLanguageValue("Parcel_tongQian"), CCSizeMake(300, 80), CCTextAlignmentLeft, 26);
    tongQian->setColor(ccc3(120, 26, 5));
    tongQian->setAnchorPoint(CCPointMake(0.5, 0.5));
    tongQian->setPosition(CCPointMake(208, 322));
    addChild(tongQian , 100);
    
    
    char buf[100];
    snprintf(buf, 99,"%qu/%ld" , pUsrInf->gold, tempPar->gold);
    
    

    
    
    TextNode* tongQianNum = TextNode::textWithString(buf, CCSizeMake(420, 80), CCTextAlignmentLeft, 26);
    tongQianNum->setColor(ccBLACK);
    
    if(tempPar->gold > pUsrInf->gold)
    {
        tongQianNum->setColor(ccRED);
        canParcel = false;
    }
    
    tongQianNum->setAnchorPoint(CCPointMake(0.5, 0.5));
    tongQianNum->setPosition(CCPointMake(208 + 130 + 60, 322));
    addChild(tongQianNum , 100);
    
    
    
    
    
    TextNode* geRenJunLiang = TextNode::textWithString(CGameData::Inst()->getLanguageValue("Parcel_geRenYuanBao"), CCSizeMake(300, 80), CCTextAlignmentLeft, 26);
    geRenJunLiang->setColor(ccc3(120, 26, 5));
    geRenJunLiang->setAnchorPoint(CCPointMake(0.5, 0.5));
    geRenJunLiang->setPosition(CCPointMake(208, 322 - 30));
    addChild(geRenJunLiang , 100);
    
    
    
    CCountryInfo* pConInf = CGameData::Inst()->getCntryInfo();
    
    
    
    
    char buf1[100];
    snprintf(buf1, 99,"%ld/%ld" , pConInf->user_supply, tempPar->wealth);
    
    
    TextNode* geRenJunLiangNum = TextNode::textWithString(buf1, CCSizeMake(420, 80), CCTextAlignmentLeft, 26);
    geRenJunLiangNum->setColor(ccBLACK);
    
    if(tempPar->wealth > pConInf->user_supply)
    {
        geRenJunLiangNum->setColor(ccRED);
        canParcel = false;
    }
    
    geRenJunLiangNum->setAnchorPoint(CCPointMake(0.5, 0.5));
    geRenJunLiangNum->setPosition(CCPointMake(208 + 130 + 60, 322 - 30));
    addChild(geRenJunLiangNum , 100);
    
    
    
    TextNode* yuanBao = TextNode::textWithString(CGameData::Inst()->getLanguageValue("Parcel_yuanBao"), CCSizeMake(300, 80), CCTextAlignmentLeft, 26);
    yuanBao->setColor(ccc3(120, 26, 5));
    yuanBao->setAnchorPoint(CCPointMake(0.5, 0.5));
    yuanBao->setPosition(CCPointMake(208, 322 - 30 * 2));
    addChild(yuanBao , 100);
    
    
    
    
    char buf2[100];
    snprintf(buf2, 99,"%ld/%ld", pUsrInf->coin, tempPar->coin );
    
    
    TextNode* yuanBaoNum = TextNode::textWithString(buf2, CCSizeMake(420, 80), CCTextAlignmentLeft, 26);
    yuanBaoNum->setColor(ccBLACK);
    
    if(tempPar->coin > pUsrInf->coin)
    {
        yuanBaoNum->setColor(ccRED);
        canParcel = false;
    }
    
    yuanBaoNum->setAnchorPoint(CCPointMake(0.5, 0.5));
    yuanBaoNum->setPosition(CCPointMake(208 + 130 + 60, 322 - 30 * 2));
    addChild(yuanBaoNum , 100);
    
    
    
    
    
    
    CCMutableArray<ParcelNeedElementInfo* > * parcelNeedElementInfoArr =  tempPar->m_ParcelNeedElementInfo;
    
    for(int i = 0 ; i < parcelNeedElementInfoArr->count() ; i ++)
    {
        
        ParcelNeedElementInfo * temp = parcelNeedElementInfoArr->getObjectAtIndex(i);
        
        ItemCardInf * tempBaseInfo = (ItemCardInf*)CGameData::Inst()->getItemCardByID(temp->objId);
        
        CCSprite* tempItem = CCSprite::spriteWithFile(tempBaseInfo->strResName.c_str());//头像显示
        
        tempItem->setPosition(CCPointMake(88 + i * 115, 480));
        
        addChild(tempItem );
        
        
        
        
        CCMutableArray<ItemCardInf* > * itemCardInf =  CGameData::Inst()->getItemCardPkInf();
        
        int haveNum = 0;
        
        for (int i = 0 ; i < itemCardInf->count(); i ++) //i < itemCardInf->count()
        {
            if (itemCardInf->getObjectAtIndex(i)->strItemId == temp->objId) {
                
                haveNum = itemCardInf->getObjectAtIndex(i)->lCnt;
                
                break;
            }
        }
        
        
        
        char buf[100];
        snprintf(buf, 99,CGameData::Inst()->getLanguageValue("Parcel_have"), haveNum);
        
        
        TextNode* have = TextNode::textWithString(buf, CCSizeMake(300, 80), CCTextAlignmentLeft, 22);
        have->setColor(ccWHITE);
        have->setAnchorPoint(CCPointMake(0.5, 0.5));
        have->setPosition(CCPointMake(188 + i * 115, 390 + 25));
        addChild(have , 100);
        
        
        
        
        
        char buf1[100];
        snprintf(buf1, 99,CGameData::Inst()->getLanguageValue("Parcel_need"), temp->num);
        
        
        TextNode* need = TextNode::textWithString(buf1, CCSizeMake(300, 80), CCTextAlignmentLeft, 22);
        need->setColor(ccWHITE);
        
        if(temp->num > haveNum)
        {
            need->setColor(ccRED);
            canParcel = false;
        }
        
        need->setAnchorPoint(CCPointMake(0.5, 0.5));
        need->setPosition(CCPointMake(188 + i * 115, 368 + 25));
        addChild(need , 100);
        
        
        
        
        
    }

    

    
    
    //back button
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("id_back.png");
    m_miBack = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(ParcelPageTwo::btnCallback) );
    m_miBack->setTag(3);
    m_miBack->setAnchorPoint(CCPointZero);
    
    m_miBack->setPosition(CCPointMake(1, 731));
    
    
    spMenu1 = CCSprite::spriteWithFile("parcal_button.png");
    
    if(!canParcel)
    spMenu1->setColor(ccGRAY);
    
    spMenu2 = CCSprite::spriteWithFile("parcal_button.png");
    
    if(!canParcel)
    spMenu2->setColor(ccGRAY);
    
    m_HeCheng = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(ParcelPageTwo::btnCallHeCheng) );
    m_HeCheng->setTag(4);
    m_HeCheng->setAnchorPoint(CCPointZero);
    
    m_HeCheng->setPosition(CCPointMake(224, 149));
    
    
    CCMenu * m_menu = CCMenu::menuWithItems( m_miBack, m_HeCheng , NULL);
    m_menu->setPosition(CCPointZero);
    addChild(m_menu, 300);

    
}

void ParcelPageTwo::aniZhuanEnd2()
{
    
    unschedule(schedule_selector(ParcelPageTwo::aniZhuanEnd2));
    
    CCLayerColor * lay = (CCLayerColor * )MainScene::Inst()->getChildByTag(989898);
    
    if(lay)
        lay->removeFromParentAndCleanup(true);
    
    
    CCSprite * sp = (CCSprite * )MainScene::Inst()->getChildByTag(989898 + 3);
    
    if(sp)
        sp->removeFromParentAndCleanup(true);
    
    
    sp = (CCSprite * )MainScene::Inst()->getChildByTag(989898 + 4);
    
    if(sp)
        sp->removeFromParentAndCleanup(true);
    
    
    
    
    
    
    
    
    
    aniPlayIng = false;
    MainScene::Inst()->enableBottomPanel(true);
    
    (m_pListener->*m_pfnSelector)(NULL);

    

}
void ParcelPageTwo::aniZhuanEnd()
{
    
    
    for(int i = 0 ; i < 5 ; i ++)
    {
        CCSprite * sp = (CCSprite * )MainScene::Inst()->getChildByTag(100000 + i);
        
        if(sp)
            sp->removeFromParentAndCleanup(true);
        
        
    }
    

    
    CCSprite * sp = (CCSprite * )MainScene::Inst()->getChildByTag(989898 + 1);
    
    if(sp)
        sp->removeFromParentAndCleanup(true);
    
    
    sp = (CCSprite * )MainScene::Inst()->getChildByTag(989898 + 2);
    
    if(sp)
        sp->removeFromParentAndCleanup(true);
    
    
    
    if(success)
    {
        CCSprite * spp = CCSprite::spriteWithFile("parcel_yes.png");
        
        spp->setPosition(ccp(320, 380));
        spp->setTag(989898 + 3);
        MainScene::Inst()->addChild(spp, 20000);
        
        spp->runAction(CCSequence::actions(
                                           
                                           CCScaleTo::actionWithDuration(0.2, 1.3),
                                           CCScaleTo::actionWithDuration(0.04, 1.1),
                                           
                                           
                                           NULL));
        
        
        
        
        
        if(m_pType == 1)
        {
            CCMutableArray<ParcelChildInfo* > * parcelChildInfoArr =  CGameData::Inst()->getParcelChildInfoAry();
            
            ParcelChildInfo * tempPar = parcelChildInfoArr->getObjectAtIndex(m_pNum);
            
            
            short tempSht = atoi(tempPar->targetId.c_str());
            
            CCardBaseInfo * tempBaseInfo = (CCardBaseInfo*)CGameData::Inst()->getCardBaseInfByCid(tempSht);
            
            
            CCSprite* sppp = CGameData::Inst()->getHeadSprite(tempBaseInfo->cid);//头像显示
            
            
            sppp->setPosition(ccp(320, 480 + 100));
            sppp->setTag(989898 + 4);
            MainScene::Inst()->addChild(sppp, 20000);
            
            sppp->runAction(CCSequence::actions(
                                                
                                                CCScaleTo::actionWithDuration(0.2, 2.6),
                                                CCScaleTo::actionWithDuration(0.04, 2.0),
                                                
                                                
                                                NULL));
        }
        else
        {
            CCMutableArray<ParcelItemInfo* > * parcelItemInfoArr =  CGameData::Inst()->getParcelItemInfoAry();
            
            ParcelItemInfo * tempPar = parcelItemInfoArr->getObjectAtIndex(m_pNum);
            
            ItemCardInf * tempBaseInfo = (ItemCardInf*)CGameData::Inst()->getItemCardByID(tempPar->targetId);
            
            
            
            
            
            CCSprite* sppp = CCSprite::spriteWithFile(tempBaseInfo->strResName.c_str());//头像显示
            
            
            sppp->setPosition(ccp(320, 480 + 100));
            sppp->setTag(989898 + 4);
            MainScene::Inst()->addChild(sppp, 20000);
            
            sppp->runAction(CCSequence::actions(
                                                
                                                CCScaleTo::actionWithDuration(0.2, 2.6),
                                                CCScaleTo::actionWithDuration(0.04, 2.0),
                                                
                                                
                                                NULL));
        }

        
        

    }
    else
    {
        CCSprite * spp = CCSprite::spriteWithFile("parcel_no.png");
        
        spp->setPosition(ccp(320, 480));
        spp->setTag(989898 + 3);
        MainScene::Inst()->addChild(spp, 20000);
        
        spp->runAction(CCSequence::actions(
                                           
                                           CCScaleTo::actionWithDuration(0.2, 1.3),
                                           CCScaleTo::actionWithDuration(0.04, 1.1),
                                           
                                           
                                           NULL));

    }
    
    schedule(schedule_selector(ParcelPageTwo::aniZhuanEnd2 ), 1.5f);

}


void ParcelPageTwo::aniZhuan()
{


    
    if(m_pType == 1)
    {
        CCMutableArray<ParcelChildInfo* > * parcelChildInfoArr =  CGameData::Inst()->getParcelChildInfoAry();
        
        ParcelChildInfo * tempPar = parcelChildInfoArr->getObjectAtIndex(m_pNum);

        
        CCMutableArray<ParcelNeedElementInfo* > * parcelNeedElementInfoArr =  tempPar->m_ParcelNeedElementInfo;
        
        for(int i = 0 ; i < parcelNeedElementInfoArr->count() ; i ++)
        {

            
            ParcelNeedElementInfo * temp = parcelNeedElementInfoArr->getObjectAtIndex(i);
            
            ItemCardInf * tempBaseInfo = (ItemCardInf*)CGameData::Inst()->getItemCardByID(temp->objId);
            
            CCSprite* sp = CCSprite::spriteWithFile(tempBaseInfo->strResName.c_str());//头像显示

            
            sp->setOpacity(0);
            sp->setTag(100000 + i);
            MainScene::Inst()->addChild(sp, 1000000);
            
            
            sp->runAction(CCSequence::actions(
                                              
                                              CCSpawn::actions(
                                                               CCRotateTo::actionWithDuration(2.2f, 360 * 5 ),
                                                               CCFadeIn::actionWithDuration(0.0000001),
                                                               ParcelScrew::actionWithDuration( sp ,2.2f , 10 * i , 180),
                                                               //                                                       CCScaleTo::actionWithDuration(3, 2.0),
                                                               NULL),
                                              CCCallFunc::actionWithTarget(this, callfunc_selector(ParcelPageTwo::aniZhuanEnd)),
                                              
                                              NULL));
            
        }
    }
    else
    {
        
        CCMutableArray<ParcelItemInfo* > * parcelItemInfoArr =  CGameData::Inst()->getParcelItemInfoAry();
        
        ParcelItemInfo * tempPar = parcelItemInfoArr->getObjectAtIndex(m_pNum);

        
        
        CCMutableArray<ParcelNeedElementInfo* > * parcelNeedElementInfoArr =  tempPar->m_ParcelNeedElementInfo;
        
        for(int i = 0 ; i < parcelNeedElementInfoArr->count() ; i ++)
        {
            
            
            ParcelNeedElementInfo * temp = parcelNeedElementInfoArr->getObjectAtIndex(i);
            
            ItemCardInf * tempBaseInfo = (ItemCardInf*)CGameData::Inst()->getItemCardByID(temp->objId);
            
            CCSprite* sp = CCSprite::spriteWithFile(tempBaseInfo->strResName.c_str());//头像显示
            
            
            sp->setOpacity(0);
            sp->setTag(100000 + i);
            MainScene::Inst()->addChild(sp, 1000000);
            
            
            sp->runAction(CCSequence::actions(
                                              
                                              CCSpawn::actions(
                                                               CCRotateTo::actionWithDuration(2.2f, 360 * 5 ),
                                                               CCFadeIn::actionWithDuration(0.0000001),
                                                               ParcelScrew::actionWithDuration( sp ,2.2f , 10 * i , 180),
                                                               //                                                       CCScaleTo::actionWithDuration(3, 2.0),
                                                               NULL),
                                              CCCallFunc::actionWithTarget(this, callfunc_selector(ParcelPageTwo::aniZhuanEnd)),
                                              
                                              NULL));
            
        }
        
    }
    
    
    
    
    
    

    
    
    aniPlayIng = true;
    MainScene::Inst()->enableBottomPanel(false);
    
    
    CCLayerColor* lc = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 128), 640, 960);
    lc->setPosition(ccp(0, 0));
    lc->setTag(989898);
    MainScene::Inst()->addChild(lc, 20000);
    

    
    CGameData::Inst()->addBigImageByRGBA4444("exchange.plist");
    
    
    CCSprite *starLight = CCSprite::spriteWithSpriteFrameName("star0001.png");
    starLight->setPosition(ccp(320, 480));
    starLight->setScale(2.5);
    MainScene::Inst()->addChild(starLight,2000000);
    
    starLight->setTag(989898 + 1);
    
    CCAnimation *starLightAnim ;
    starLightAnim = CCAnimation::animation();
    CCActionInterval *starLightActionInt;
    for(int i= 1;i<15;i++)
    {
        char frameName[30];
        sprintf(frameName, "star00%02d.png",i);
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
        starLightAnim->addFrame(frame);
    }
    starLightActionInt = CCAnimate::actionWithDuration(2.20 / 6 * 4, starLightAnim, false);
    starLight->runAction(CCSequence::actions(
//                                             CCDelayTime::actionWithDuration(0.1),
                                             starLightActionInt,
                                             NULL));
    
    
    
    
    CCSprite *lightBall = CCSprite::spriteWithSpriteFrameName("goldlight0001.png");
    lightBall->setPosition(ccp(320, 480));
    lightBall->setOpacity(0);
    lightBall->setScale(2.5);
    
    MainScene::Inst()->addChild(lightBall,2000000);
    lightBall->setTag(989898 + 2);

    
    CCAnimation *lightBallAnim ;
    lightBallAnim = CCAnimation::animation();
    CCActionInterval *lightBallActionInt;
    for(int i= 0;i<14;i++)
    {
        char frameName[30];
        sprintf(frameName, "goldlight00%02d.png",i);
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName);
        lightBallAnim->addFrame(frame);
    }
    lightBallActionInt = CCAnimate::actionWithDuration(2.20 / 6 * 2, lightBallAnim, false);
    lightBall->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(2.20 / 6 * 4),
                                             CCFadeIn::actionWithDuration(0.0000001f),
                                             lightBallActionInt,
                                             
//                                             CCFadeOut::actionWithDuration(0.2),
                                             NULL));

    
}






//******************************************************************************
// cbCommDlg
//******************************************************************************
void ParcelPageTwo::cbCommDlg(CCObject* pObj)
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
                
                
            case enParcelPageTwoReqType_AllCard:
            {
                if(CGameData::Inst()->getUserCards())
                {
                    
                    m_curReqType = enParcelPageTwoReqType_AllCard;
                    scheduleUpdate();
                }
            }
                break;
            case enParcelPageTwoReqType_HeCheng:
                
            {
                if(m_pType == 1)
                {
                    
                    CCMutableArray<ParcelChildInfo* > * parcelChildInfoArr =  CGameData::Inst()->getParcelChildInfoAry();
                    
                    ParcelChildInfo * tempPar = parcelChildInfoArr->getObjectAtIndex(m_pNum);
                    
                    
                    if(CGameData::Inst()->useCompoundReq("1", tempPar->toID))
                    {
                        
                        m_curReqType = enParcelPageTwoReqType_HeCheng;
                        scheduleUpdate();
                    }
                }
                else
                {
                    
                    CCMutableArray<ParcelItemInfo* > * parcelItemInfoArr =  CGameData::Inst()->getParcelItemInfoAry();
                    
                    ParcelItemInfo * tempPar = parcelItemInfoArr->getObjectAtIndex(m_pNum);
                    
                    
                    
                    if(CGameData::Inst()->useCompoundReq("0", tempPar->toID))
                    {
                        
                        m_curReqType = enParcelPageTwoReqType_HeCheng;
                        scheduleUpdate();
                    }
                }
                
            }
                break;
                
            
                
            default:
                break;
        }
    }
    else {
        CGameData::Inst()->clearReqStat();
    }
}

void ParcelPageTwo::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(ParcelPageTwo::cbCommDlg),
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
        switch (m_curReqType) {
            case enParcelPageTwoReqType_HeCheng:
            {
               
                success = CGameData::Inst()->getUseCompoundResult();
                
                
                if(CGameData::Inst()->allCardRequest())
                {
                    
                    m_curReqType = enParcelPageTwoReqType_AllCard;
                    scheduleUpdate();
                }
                
                
                
            }
                break;
                
            case enParcelPageTwoReqType_AllCard:
            {
                aniZhuan();

            }
                break;
                
            
                
            default:
                break;
                
        }
    }
    else {
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(ParcelPageTwo::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            m_commDlg->setPosition(CCPointMake(320, 480 - getPosition().y));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}



void ParcelPageTwo::btnCallHeCheng()
{
    
    if(aniPlayIng)
        return;
    

    if(!canParcel)
        return;
    
    
    if(m_pType == 1)
    {
        
        CCMutableArray<ParcelChildInfo* > * parcelChildInfoArr =  CGameData::Inst()->getParcelChildInfoAry();
        
        ParcelChildInfo * tempPar = parcelChildInfoArr->getObjectAtIndex(m_pNum);

        
        if(CGameData::Inst()->useCompoundReq("1", tempPar->toID))
        {
            
            m_curReqType = enParcelPageTwoReqType_HeCheng;
            scheduleUpdate();
        }
    }
    else
    {
        
        CCMutableArray<ParcelItemInfo* > * parcelItemInfoArr =  CGameData::Inst()->getParcelItemInfoAry();
        
        ParcelItemInfo * tempPar = parcelItemInfoArr->getObjectAtIndex(m_pNum);

        
        
        if(CGameData::Inst()->useCompoundReq("0", tempPar->toID))
        {
            
            m_curReqType = enParcelPageTwoReqType_HeCheng;
            scheduleUpdate();
        }
    }
    
    

    
    
    
}

//******************************************************************************
// btnCallback
//******************************************************************************
void ParcelPageTwo::btnCallback()
{
    if(aniPlayIng)
        return;
    

    (m_pListener->*m_pfnSelector)(NULL);



}