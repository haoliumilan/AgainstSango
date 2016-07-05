//
//  CommDlg3.cpp
//  AgainstWar
//
//  Created by caojiye on 13-7-2.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//
//////////////////////////////////////////////////////////////////
#include "CommDlg3.h"
#include "TextNode.h"
#include "CGameData.h"
//////////////////////////////////////////////////////////////////
Dialog3* Dialog3::CreateBind(CCObject* target, SEL_CallFuncO selector)
{
    Dialog3* rt = new Dialog3(target, selector, CGameData::Inst()->getLanguageValue("cntry_hint"),
                                                CGameData::Inst()->getLanguageValue("arena_record_bind_content"),
                                                "fr_cntryDetails.png",
                                                NULL, NULL, NULL,
                                                "bind_sina.png", "bind_tencent.png", "bind_none.png");
    rt->autorelease();
    return rt;    
}

Dialog3* Dialog3::CreateUpload(CCObject* target, SEL_CallFuncO selector)
{
    Dialog3* rt = new Dialog3(target, selector, CGameData::Inst()->getLanguageValue("record_upload_title"),
                                                CGameData::Inst()->getLanguageValue("record_upload_content"),
                                                "fr_cntryDetails.png",
                                                CGameData::Inst()->getLanguageValue("record_upload_btn1"),
                                                CGameData::Inst()->getLanguageValue("record_upload_btn2"),
                                                CGameData::Inst()->getLanguageValue("record_upload_btn3"),
                                                "", "", "");
    rt->autorelease();
    return rt;
}

Dialog3::Dialog3(CCObject* target, SEL_CallFuncO selector, const char* title,const char* dsp, const char* bkg, const char* btn_text1, const char* btn_text2, const char* btn_text3, const char* btn_img1, const char* btn_img2, const char* btn_img3)
{
    m_pListener = target;
    m_pfnSelector = selector;
    
    
    CCSprite* sp = CCSprite::spriteWithFile(bkg);
    if(sp)
    {
        addChild(sp);
    }
    
    CCSprite* reverse = CCSprite::spriteWithFile(bkg);
    sp->addChild(reverse);
    reverse->setAnchorPoint(CCPointZero);
    reverse->setRotation(180);
    
    float sp_width = sp->getContentSize().width;
    float sp_height = sp->getContentSize().height;
    
    // title
    TextNode* text_title = GetTextTitle(title, sp_width);
    sp->addChild(text_title, 10);
    text_title->setPosition(ccp(sp_width/2, sp_height-120));
    
    // content
    TextNode* text_content = GetTextContent(dsp, 30, CCSizeMake(sp_width-200, 300));
    sp->addChild(text_content);
    text_content->setPosition(ccp(sp_width/2, text_title->getPosition().y-120));
    
    // 3 个按钮
    CCMenuItem* item1 = NULL;
    CCMenuItem* item2 = NULL;
    CCMenuItem* item3 = NULL;
    
    if (btn_text1 != NULL)
    {
        item1 = GetMenuItem(btn_text1);
        item2 = GetMenuItem(btn_text2);
        item3 = GetMenuItem(btn_text3);
    }
    else
    {
        item1 = GetMenuItemWithImage(btn_img1);
        item2 = GetMenuItemWithImage(btn_img2);
        item3 = GetMenuItemWithImage(btn_img3);
    }
    
    item1 -> setTag(1);
    item2 -> setTag(2);
    item3 -> setTag(3);
    int height = 286;
    item1 -> setPosition(CCPointMake(119, height));
    item2 -> setPosition(CCPointMake(119, height-90));
    item3 -> setPosition(CCPointMake(119, height-180));
    
    // menu
    CCMenu* menu = CCMenu::menuWithItems(item1, item2, item3, NULL);
    menu->setPosition(CCPointZero);
    sp->addChild(menu);
    
}


TextNode* Dialog3::GetTextTitle(const char* ch, float width)
{
    return GetTextNode(ch, CCSizeMake(width, kBtnTitleHeight), CCTextAlignmentCenter, kBtnTitleHeight+10, ccBLACK, false);
}

TextNode* Dialog3::GetTextContent(const char* ch, float font_size, CCSize text_size)
{
    return GetTextNode(ch, text_size, CCTextAlignmentLeft, font_size, ccc3(122, 37, 8), false);
}

TextNode* Dialog3::GetTextNode(const char* ch, CCSize text_size ,CCTextAlignment text_alignment, float font_size, ccColor3B text_color, bool shadow_black)
{
    TextNode* rt = TextNode::textWithString(ch, text_size, text_alignment, font_size);
    rt->setColor(text_color);
    if (shadow_black)
    {
         rt->setShadowColor(ccBLACK);
    }
    
    return rt;
}

CCMenuItem* Dialog3::GetMenuItem(const char* title)
{
    CCSprite* spMenu1 = CCSprite::spriteWithFile("long_btn_img.png");
    CCSprite* spMenu2 = CCSprite::spriteWithFile("long_btn_img.png");
    CCMenuItem *rt = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(Dialog3::CallBack));
    TextNode* itemLabel = TextNode::textWithString(title, CCSizeMake(188, 30), CCTextAlignmentCenter, 36);
    itemLabel->setColor(ccWHITE);
    itemLabel->setShadowColor(ccBLACK);
    itemLabel->setPosition(ccp(spMenu1->getContentSize().width/2, spMenu1->getContentSize().height/2+4));
    rt->addChild(itemLabel);
    rt->setAnchorPoint(CCPointZero);
    return rt;
}

CCMenuItem* Dialog3::GetMenuItemWithImage(const char* img)
{
    CCSprite* spMenu1 = CCSprite::spriteWithFile("long_btn_img.png");
    CCSprite* spMenu2 = CCSprite::spriteWithFile("long_btn_img.png");
    CCMenuItem *rt = CCMenuItemImage::itemFromNormalSprite(spMenu1, spMenu2, this, menu_selector(Dialog3::CallBack));
    CCSprite* sp = CCSprite::spriteWithFile(img);
    rt->addChild(sp);
    sp->setPosition(CCPointMake(spMenu1->getContentSize().width/2, spMenu1->getContentSize().height/2+4));
    rt->setAnchorPoint(CCPointZero);
    return rt;
}

void Dialog3::CallBack(CCNode* _node)
{
    m_ret.bSel = _node->getTag();
    (m_pListener->*m_pfnSelector)(&m_ret);
    removeFromParentAndCleanup(true);
}






