//
//  CompGachaBtn.cpp
//  AgainstWar
//
//  Created by 海桅 王 on 12-10-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#include "CompGachaBtn.h"
#include "CommonDef.h"
#include "MainScene.h"

CompGachaBtn::CompGachaBtn(CompGachaInfo *info,CCObject* target, SEL_CallFuncO selector)
{
    isExchangeAble = true;
    m_compGachaInfo = info;
    m_pListener = target;
    m_pfnSelector = selector;
    int lines ;
    if(info->material->count()%3 ==0)
        lines=info->material->count()/3;
    else
        lines = info->material->count()/3+1;
    
    int scaleY = CELLHEIGHT*lines;
    if(lines == 1)//如果只有一行，给以按钮空间
    {
        scaleY += EXCHANGEBTNHEIGHT;
    }
    cardLines = lines;
    
    btnBgTop = CCSprite::spriteWithFile("cg_panel_1.png");
    CCPoint point = btnBgTop->getPosition();
    this->addChild(btnBgTop);
    
    btnBgMiddle = CCSprite::spriteWithFile("cg_panel_2.png");
    btnBgMiddle->setScaleY(scaleY);
    point.y-=btnBgTop->getContentSize().height/2+btnBgMiddle->getContentSize().height/2*scaleY;
    btnBgMiddle->setPosition(point);
    this->addChild(btnBgMiddle);
    
    btnBgBottom = CCSprite::spriteWithFile("cg_panel_3.png");
    point.y-=btnBgMiddle->getContentSize().height/2*scaleY+btnBgBottom->getContentSize().height/2;
    btnBgBottom->setPosition(point);
    this->addChild(btnBgBottom);
    
    char buf[100];
    long long time =  m_compGachaInfo->rest_exchange_time;
    if(time>24*60*60)
        sprintf(buf,CGameData::Inst()->getLanguageValue("compGachaTip1"),time/(24*60*60));
    if(time>60*60&&time<24*60*60)
        sprintf(buf, CGameData::Inst()->getLanguageValue("compGachaTip2"),time/(60*60));
    if(time>60&&time<60*60)
        sprintf(buf, CGameData::Inst()->getLanguageValue("compGachaTip3"),time/60);
    
    exchangeCountDown = TextNode::textWithString(buf, 20);
    addChild(exchangeCountDown);
    exchangeCountDown->setColor(ccc3(255, 227, 70));
    exchangeCountDown->setPosition(ccp(-100, 0));
    
    //exchangeCountDown->setColor()
    
    sprintf(buf, CGameData::Inst()->getLanguageValue("compGachaTip4"),info->claimTimes);
    if(info->claimTimes == -1)
    {
        sprintf(buf,"%s",CGameData::Inst()->getLanguageValue("compGachaTip5"));
    }
    exchangeTimes = TextNode::textWithString(buf, 20);
    addChild(exchangeTimes);
    exchangeTimes->setColor(ccc3(255, 227, 70));
    exchangeTimes->setPosition(ccp(155, 0));
    
    addHead();
}

int CompGachaBtn::getCardLines()
{
    return cardLines;
}

float CompGachaBtn::getDefalutDelta()
{
    return (CELLHEIGHT*cardLines+31)/2;
}


CCSize CompGachaBtn::getSize()
{
    CCSize size ;
    size.width = btnBgTop->getTextureRect().size.width*2;
    size.height = btnBgTop->getTextureRect().size.height*2 +btnBgBottom->getTextureRect().size.height*2+CELLHEIGHT*cardLines;
    if(cardLines==1)
        size.height+=EXCHANGEBTNHEIGHT;
    return size;
}


void CompGachaBtn::addHead()
{
    isExchangeAble =true;
    int specailH=0;
    if(cardLines == 1)
        specailH = 40;
    char buf[50];
    
    for(int i=0;i<m_compGachaInfo->material->count();i++)
    {
        CCSprite *head = CGameData::Inst()->getHeadSprite(m_compGachaInfo->material->getObjectAtIndex(i)->cid);
        addChild(head,1000);
        head->setPosition(CCPoint(-190+105*(i%3),-80-(i/3)*110-specailH));
        head->setTag(MATERIALTAG+i);  
        if(!m_compGachaInfo->material->getObjectAtIndex(i)->isOwe)
        {
            head->setOpacity(128);
            isExchangeAble =false;
        }
        if(m_compGachaInfo->material->getObjectAtIndex(i)->isNew)
        {
            showNewCardEffect(head);

        }
    }
    
    CCSprite *targetHead;
    if(m_compGachaInfo->isHide)
    {
        sprintf(buf, "fr_query_box.PNG");
        targetHead = CCSprite::spriteWithFile(buf);
        m_ret.isUnKnowHero = true;
    }
    else 
    {   
        targetHead = CGameData::Inst()->getHeadSprite(m_compGachaInfo->targetCid);
        m_ret.isUnKnowHero = false;

    }
    addChild(targetHead,1000);
    targetHead->setPosition(CCPoint(160,-80));
    targetHead->setTag(SUPERHEROTAG);
    
    CCSprite *normalSp;
    CCSprite *selectedSp ;
    if(isExchangeAble)
    {
        normalSp = CCSprite::spriteWithFile("cg_button_1.png");
        selectedSp =  CCSprite::spriteWithFile("cg_button_1.png");
        
    }else {
        normalSp = CCSprite::spriteWithFile("cg_button_2.png");
        selectedSp =  CCSprite::spriteWithFile("cg_button_2.png");
        
        if(!m_compGachaInfo->isHide)
        {
            targetHead->setOpacity(128);
        }
    }
    
    CCMenuItem *item = CCMenuItemImage::itemFromNormalSprite(normalSp, selectedSp, this, menu_selector(CompGachaBtn::menuCallback));  
    item->setAnchorPoint(CCPoint(0, 0));
    CCMenu *menu = CCMenu::menuWithItem(item);
    menu->setPosition(ccp(targetHead->getPosition().x-70,targetHead->getPosition().y-145));
    addChild(menu);
    
    
}

void CompGachaBtn::menuCallback(CCObject *sender)
{
    if(isExchangeAble)
    {
        
        (m_pListener->*m_pfnSelector)(&m_ret);

    }else
    {
        if(CGameData::Inst()->getUsrInfo()->max_card_num <= CGameData::Inst()->getUserCards()->count())
        {
            MainScene::Inst()->showFullPackDlg(true);
        }
    }
    CGameData::Inst()->setCompGachaNumber(m_compGachaInfo->position);

}

void CompGachaBtn::showNewCardEffect(CCSprite *sp)
{
    
    CCSprite *newSp = CCSprite::spriteWithSpriteFrameName("ma_new.png");
    newSp->setScale(0.5);
    newSp->setPosition(ccp(85, 92));
    sp->addChild(newSp);
    
    newSp->runAction(CCRepeatForever::actionWithAction((CCActionInterval *)CCSequence::actions(CCFadeIn::actionWithDuration(1.0),CCFadeOut::actionWithDuration(1.0),NULL)));
//    CCParticleCardFrame *pCardFrame = CCParticleCardFrame::node();
//    pCardFrame->setPosition(CCPointMake(50, 492));
//    CCPoint point1 = pCardFrame->getPosition();
//    CCPoint point2 = CCPoint(point1.x+107,point1.y);
//    CCPoint point3 = CCPoint(point1.x+107,point1.y-107);
//    CCPoint point4 = CCPoint(point1.x,point1.y-107);
//    pCardFrame->setTexture( CCTextureCache::sharedTextureCache()->addImage("particle_fire.png"));
//	sp->addChild(pCardFrame);
//    
//    pCardFrame->runAction(CCRepeatForever::actionWithAction((CCActionInterval *)CCSequence::actions(CCMoveTo::actionWithDuration(0.5,point2),CCMoveTo::actionWithDuration(0.5, point3),CCMoveTo::actionWithDuration(0.5, point4),CCMoveTo::actionWithDuration(0.5, point1),NULL))) ;
    
}

void CompGachaBtn::onExit()
{
    CCLayer::onExit();
    
    removeAllChildrenWithCleanup(true);
}

