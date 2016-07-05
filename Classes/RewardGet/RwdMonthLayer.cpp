//
//  RwdMonthLayer.cpp
//  AgainstWar
//
//  Created by caojiye on 13-8-13.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//
//////////////////////////////////////////////////////////////////
#include "RwdMonthLayer.h"
#include "CGameData.h"
#include "TextNode.h"
//////////////////////////////////////////////////////////////////
# define  kCellTag        987
# define  kOrangeColor    ccc3(247,157,1)
//////////////////////////////////////////////////////////////////
RwdMonthLayer* RwdMonthLayer::Create(CCObject* target, SEL_CallFuncO selector)
{
    RwdMonthLayer* rt = new RwdMonthLayer(target, selector);
    rt->autorelease();
    return rt;
}

RwdMonthLayer::RwdMonthLayer(CCObject* target, SEL_CallFuncO selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
    Set25Cell();
    
    // 哪几天 能有奖励拿
    CCMutableArray<RwdMonthInf*> * pRwdMonthInfArr = CGameData::Inst()->getRwdMonthInfoArr();
    for (int i=0; i<pRwdMonthInfArr->count(); i++)
    {
        CCSprite* sp_head;
        
        RwdMonthInf* info = pRwdMonthInfArr->getObjectAtIndex(i);
        
        if(info->hide)
        {
            sp_head = CCSprite::spriteWithFile("rwdwenhao.png");
        }
        else if (info->WujiangCardList.size() > 0){
            RwdWuJiangCard ItmInf = (info->WujiangCardList).front();
            
            int id = atoi(ItmInf.strCardId.c_str());
            sp_head = CGameData::Inst()->getHeadSprite(id);
        }else if(info->ItemCardList.size() > 0){
            RwdItemCard ItemInf = (info->ItemCardList).front();
            ItemCardInf* pItemCard = CGameData::Inst()->getItemCardByID(ItemInf.strCardId);
            sp_head = CCSprite::spriteWithFile(pItemCard->strResName.c_str());
            char buf[16];
            snprintf(buf, 16, "/%d", ItemInf.num);
            CCSprite* spTmp = CCSprite::spriteWithFile("rewardnum.png");
            if(spTmp){
                CCSize sz = spTmp->getContentSize();
                CCLabelAtlas* lbItemCount = CCLabelAtlas::labelWithString(buf,
                                                                          "rewardnum.png",
                                                                          sz.width / 11,
                                                                          sz.height,
                                                                          '/');
                lbItemCount->setAnchorPoint(ccp(0.5, 0));
                lbItemCount->setPosition(CCPointMake(sp_head->getContentSize().width/2, 0));
                if (ItemInf.num>=10000) {
                    lbItemCount->setScale(0.8f);
                }
                sp_head->addChild(lbItemCount);
            }
            
        }
        
        CCNode* node = getChildByTag(info->day+kCellTag);
        node->addChild(sp_head,2);
        sp_head->setPosition(CCPointMake(node->getContentSize().width/2, node->getContentSize().height/2));
    }
    
    // 现在登录到了第几天
    if (pRwdMonthInfArr->count()>0) {
        int day = pRwdMonthInfArr->getObjectAtIndex(0)->lDays;
        if (day>25) {
            day = 25;
        }
        for (int i=1; i<=day; i++)
        {
            CCNode* node = getChildByTag(i+kCellTag);
            
            CCSprite* sp_login = CCSprite::spriteWithFile("rwdgaizhang.png");
            node->addChild(sp_login, 3);
            sp_login->setPosition(CCPointMake(node->getContentSize().width/2, node->getContentSize().height/2));
        }
    }
    
}

string RwdMonthLayer::ConvertCardToPng(string card)
{
    int i=0;
    while (card[i])
    {
        if (card[i]>'9' || card[i]<'0')
        {
            card[i] = '\0';
            break;
        }
        i++;
    }
    char buf[16];
    snprintf(buf, 16, "%s%s", &card[0], "_head.png");
    string rt = (string)buf;
    return rt;
}

void RwdMonthLayer::Set25Cell()
{
    for (int r=0; r<5; r++)
    {
        for (int c=0; c<5; c++)
        {
            CCSprite* sp = GetCellSprite(c+1+r*5);
            sp->setPosition(CCPointMake(126*c+68, 660-r*112));
            addChild(sp);
        }
    }
}

CCSprite* RwdMonthLayer::GetCellSprite(int _number)
{
    CCSprite* rt = CCSprite::spriteWithFile("rwdyuefenkuang.png");
    if (rt)
    {
        rt->setTag(_number+kCellTag);
        char buf[4];
        snprintf(buf, 4, "%d", _number);
        TextNode* number = TextNode::textWithString(buf, 52);
        number->setColor(kOrangeColor);
        number->setPosition(CCPointMake(rt->getContentSize().width/2, rt->getContentSize().height/2));
        rt->addChild(number);
    }
    return rt;
}

//******************************************************************************
// ccTouchBegan
//******************************************************************************
/*bool RwdMonthLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    CCPoint touchLocation = touch->locationInView( touch->view() );
    touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
    CCPoint pos = convertToNodeSpace( touchLocation );
    
    CCPoint pt;
    CCSize sz;
    
    for (int i=0; i<CGameData::Inst()->getRwdMonthInfoArr()->count(); i++)
    {
        RwdMonthInf* info = CGameData::Inst()->getRwdMonthInfoArr()->getObjectAtIndex(i);
        CCNode* node = getChildByTag(info->day+kCellTag);
        pt = node->getPosition();
        sz = node->getContentSize();
        bool bRet;
        bRet = CCRect::CCRectContainsPoint(CCRectMake(pt.x - sz.width * 0.5,
                                                      pt.y - sz.height * 0.5,
                                                      sz.width,
                                                      sz.height),
                                           pos);
        if (bRet) {
            CCLOG("%d",info->day);
            CCNode* node1 = getChildByTag(info->day+kCellTag)->getChildByTag(info->day+kZhangTag);
            node->reorderChild(node1, 2);
            break;
        }
    }
    return true;
}*/






