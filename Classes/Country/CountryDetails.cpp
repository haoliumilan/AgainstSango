//
//  CountryDetails.cpp
//  AgainstWar
//
//  Created by Liu Hao on 13-1-6.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#include "CountryDetails.h"
#include "TextNode.h"
#include "CommonDef.h"


CountryDetails::CountryDetails(CCObject* target, SEL_MenuHandler selector)
{
    m_pListener = target;
    m_pfnSelector = selector;
}

CountryDetails::~CountryDetails()
{

}

void CountryDetails::onEnter()
{
    CCLayer::onEnter();
    
}

void CountryDetails::onExit()
{
    removeAllChildrenWithCleanup(true);
    CCTextureCache::sharedTextureCache()->removeUnusedTextures();

    CCLayer::onExit();
}

void CountryDetails::showOneCntryDetails(CCntryBaseInfo *cntryInfo)
{
    char buf[100];
    
    CCSprite* spBg = CCSprite::spriteWithFile("fr_cntryDetails.png");
    spBg->setPosition(CCPointMake(320, 480));
    addChild(spBg);
    
    CCSprite* spTitle = CCSprite::spriteWithFile("title_cntryDetails.png");
    spTitle->setPosition(CCPointMake(320, 780));
    addChild(spTitle);
    
//    TextNode* tnCntryName = TextNode::textWithString(cntryInfo->name,
//                                                     CCSizeMake(500, 50),
//                                                     CCTextAlignmentCenter,
//                                                     45);
    CCLabelTTF* tnCntryName = CCLabelTTF::labelWithString(cntryInfo->name, CCSizeMake(500, 50), CCTextAlignmentCenter, FONTSTHEITISCM, 45);
    
    tnCntryName->setColor(ccBLACK);
    tnCntryName->setPosition(CCPointMake(320, 708));
    addChild(tnCntryName);
    
    snprintf(buf, 99, "(id:%s)", cntryInfo->cid);
    TextNode* tnCntryId = TextNode::textWithString(buf,
                                                   CCSizeMake(500, 25),
                                                   CCTextAlignmentCenter,
                                                   22);
    tnCntryId->setColor(ccBLACK);
    tnCntryId->setPosition(CCPointMake(320, 668));
    addChild(tnCntryId);
    
    CCSprite* spTextBg = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    spTextBg->setPosition(CCPointMake(320, 630));
    addChild(spTextBg);
    
    CCSize szTextBg = spTextBg->getContentSize();
    CCSize szText = CCSizeMake(szTextBg.width - 30, szTextBg.height);
    
    snprintf(buf, 99, "%s：", CGameData::Inst()->getLanguageValue("cntry_king"));
    TextNode* tnLeaderName = TextNode::textWithString(buf,
                                                      szText,
                                                      CCTextAlignmentLeft,
                                                      26);
    tnLeaderName->setColor(ccc3(168, 2, 6));
    tnLeaderName->setPosition(CCPointMake(szTextBg.width * 0.5,
                                          szTextBg.height * 0.5));
    spTextBg->addChild(tnLeaderName);
    
    CCLabelTTF* tnLeaderName2 = CCLabelTTF::labelWithString(cntryInfo->leader_name, szText, CCTextAlignmentLeft, FONTSTHEITISCM, 26);
    tnLeaderName2->setColor(ccc3(168, 2, 6));
    tnLeaderName2->setPosition(CCPointMake(szTextBg.width * 0.5 + 70.0f,
                                          szTextBg.height * 0.5));
    spTextBg->addChild(tnLeaderName2);
    
    
    
    spTextBg = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    spTextBg->setPosition(CCPointMake(320, 580));
    addChild(spTextBg);
    
    snprintf(buf, 99, "%s%d%s", CGameData::Inst()->getLanguageValue("cntry_country_level"), cntryInfo->lv, CGameData::Inst()->getLanguageValue("cntry_JI_alone"));
    TextNode* tnCntryLv = TextNode::textWithString(buf,
                                                   szText,
                                                   CCTextAlignmentLeft,
                                                   26);
    tnCntryLv->setColor(ccBLACK);
    tnCntryLv->setPosition(CCPointMake(szTextBg.width * 0.5,
                                       szTextBg.height * 0.5));
    spTextBg->addChild(tnCntryLv);
    
    spTextBg = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    spTextBg->setPosition(CCPointMake(320, 530));
    addChild(spTextBg);
    
    snprintf(buf, 99, "%s%s", CGameData::Inst()->getLanguageValue("cntry_country_amount"), cntryInfo->member_num);
    TextNode* tnMemberNum = TextNode::textWithString(buf,
                                                     szText,
                                                     CCTextAlignmentLeft,
                                                     26);
    tnMemberNum->setColor(ccBLACK);
    tnMemberNum->setPosition(CCPointMake(szTextBg.width * 0.5,
                                         szTextBg.height * 0.5));
    spTextBg->addChild(tnMemberNum);
    
    spTextBg = CCSprite::spriteWithSpriteFrameName("fr_id field.png");
    spTextBg->setPosition(CCPointMake(320, 480));
    addChild(spTextBg);
    
    snprintf(buf, 99, "%s%ld",
             CGameData::Inst()->getLanguageValue("cntry_country_exp"),
             cntryInfo->gexp_total);
    TextNode* tnCntryExp = TextNode::textWithString(buf,
                                                    szText,
                                                    CCTextAlignmentLeft,
                                                    26);
    tnCntryExp->setColor(ccBLACK);
    tnCntryExp->setPosition(CCPointMake(szTextBg.width * 0.5,
                                        szTextBg.height * 0.5));
    spTextBg->addChild(tnCntryExp);
    
    spTextBg = CCSprite::spriteWithFile("bg_declaration.png");
    spTextBg->setPosition(CCPointMake(320, 380));
    addChild(spTextBg);
    
    szTextBg = spTextBg->getContentSize();
    szText = CCSizeMake(szTextBg.width - 30, szTextBg.height - 40);
    
    TextNode* tnDeclarationTitle = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_country_declaration"),
                                                            szText,
                                                            CCTextAlignmentLeft,
                                                            26);
    tnDeclarationTitle->setColor(ccBLACK);
    tnDeclarationTitle->setPosition(CCPointMake(szTextBg.width * 0.5,
                                                szTextBg.height - 20));
    spTextBg->addChild(tnDeclarationTitle);

//    TextNode* tnDeclaration = TextNode::textWithString(cntryInfo->declaration,
//                                                       szText,
//                                                       CCTextAlignmentLeft,
//                                                       26);
    CCLabelTTF* tnDeclaration = CCLabelTTF::labelWithString(cntryInfo->declaration, szText, CCTextAlignmentLeft, FONTSTHEITISCM, 26);
    tnDeclaration->setColor(ccBLACK);
    
    //先获得title的contentSize 跟 Declaration的contentSize  tangaowen  2013.2.5
        
    tnDeclaration->setAnchorPoint(CCPoint(0, 1));
    
    tnDeclaration->setPosition(CCPointMake(10,
                                           tnDeclarationTitle->getPosition().y ));
    
    spTextBg->addChild(tnDeclaration);
    
    CCountryInfo* myCntryInfo = CGameData::Inst()->getCntryInfo();
    if (myCntryInfo->status == 0)
        showNormalBtn(cntryInfo->bApply);
    else
        showMemberBtn(false);
}

void CountryDetails::showCountryDetails()
{
    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    char buf[100];
    float effect;
    
    CCSprite* spBg = CCSprite::spriteWithFile("fr_cntryDetails.png");
    spBg->setPosition(CCPointMake(320, 480));
    addChild(spBg);
    
    CCSprite* spTitle = CCSprite::spriteWithFile("title_cntryDetails.png");
    spTitle->setPosition(CCPointMake(320, 780));
    addChild(spTitle);
    
    // 同盟ID
    snprintf(buf, 99, "%s", cntryInfo->cid);
    showCntryProperty(CGameData::Inst()->getLanguageValue("cntry_country_id"), buf, 705);
    
    // 同盟等级
    if (cntryInfo->lv == 10)
        snprintf(buf, 99, "%s", CGameData::Inst()->getLanguageValue("max"));
    else
        snprintf(buf, 99, "%d", cntryInfo->lv);
    showCntryProperty(CGameData::Inst()->getLanguageValue("cntry_country_level"), buf, 663);

    // 同盟人数
    snprintf(buf, 99, "%d/%d", cntryInfo->member_cur_num, cntryInfo->member_max_num);
    showCntryProperty(CGameData::Inst()->getLanguageValue("cntry_country_amount"), buf, 621);
    
    // 同盟经验
    snprintf(buf, 99, "%ld", cntryInfo->cur_exp);
    showCntryProperty(CGameData::Inst()->getLanguageValue("cntry_country_exp"), buf, 579);
    
    // 升级所需经验
    snprintf(buf, 99, "%ld", cntryInfo->need_exp);
    showCntryProperty(CGameData::Inst()->getLanguageValue("cntry_next_exp"), buf, 537);
    
    // 同盟粮饷
    snprintf(buf, 99, "%ld", cntryInfo->cntry_supply);
    showCntryProperty(CGameData::Inst()->getLanguageValue("cntry_country_money"), buf, 495);
    
    // 同盟元宝
    snprintf(buf, 99, "%ld", cntryInfo->cntry_coin);
    showCntryProperty(CGameData::Inst()->getLanguageValue("cntry_gold_alone2"), buf, 453);

    CCSprite* spIconCoin = CCSprite::spriteWithSpriteFrameName("id_gold.png");
    spIconCoin->setPosition(CCPointMake(330, 453));
    spIconCoin->setScale(0.5);
    addChild(spIconCoin);

    // 同盟贡献
    snprintf(buf, 99, "%ld", cntryInfo->cntry_contribution);
    showCntryProperty(CGameData::Inst()->getLanguageValue("cntry_country_contribution"), buf, 411);
    
    CCSprite* spIconSupply = CCSprite::spriteWithSpriteFrameName("icon_supply.png");
    spIconSupply->setPosition(CCPointMake(330, 411));
    addChild(spIconSupply);

    // 分割线
    CCSprite* spLine = CCSprite::spriteWithFile("cntryDetails_line.png");
    spLine->setPosition(CCPointMake(320, 383));
    addChild(spLine);
    
    // buff bg
    CCSprite* spBuffBg = CCSprite::spriteWithFile("cntryDetails_buffBg.png");
    spBuffBg->setPosition(CCPointMake(320, 338));
    addChild(spBuffBg);
    
//    // 同盟增益效果
//    TextNode* tnTitleBuff = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_country_promotion_effect"),
//                                                     30);
//    tnTitleBuff->setColor(ccc3(0, 99, 162));
//    tnTitleBuff->setPosition(CCPointMake(320, 395));
//    addChild(tnTitleBuff);
    
    // stamina buff
    effect = cntryInfo->buffInfo[enBuffType_Stamina].effect * 100;
    if (effect > int(effect))
        snprintf(buf, 99, "%s-%.1f%%", CGameData::Inst()->getLanguageValue("cntry_time_0"), effect);
    else
        snprintf(buf, 99, "%s-%d%%", CGameData::Inst()->getLanguageValue("cntry_time_0"), int(effect));

    showBuffEffect("buff_small_stamina.png", buf, CCPointMake(212, 355));

    // attack buff
    effect = cntryInfo->buffInfo[enBuffType_Attack].effect * 100;
    if (effect > int(effect))
        snprintf(buf, 99, "%s+%.1f%%", CGameData::Inst()->getLanguageValue("cntry_attack_0"), effect);
    else
        snprintf(buf, 99, "%s+%d%%", CGameData::Inst()->getLanguageValue("cntry_attack_0"), int(effect));

    showBuffEffect("buff_small_attack.png", buf, CCPointMake(450, 355));

    // recover buff
    effect = cntryInfo->buffInfo[enBuffType_Recover].effect * 100;
    if (effect > int(effect))
        snprintf(buf, 99, "%s+%.1f%%", CGameData::Inst()->getLanguageValue("cntry_recover_0"), effect);
    else
        snprintf(buf, 99, "%s+%d%%", CGameData::Inst()->getLanguageValue("cntry_recover_0"), int(effect));

    showBuffEffect("buff_small_recover.png", buf, CCPointMake(212, 320));

    // hp buff
    effect = cntryInfo->buffInfo[enBuffType_Hp].effect * 100;
    if (effect > int(effect))
        snprintf(buf, 99, "%s+%.1f%%", CGameData::Inst()->getLanguageValue("cntry_hp_0"), effect);
    else
        snprintf(buf, 99, "%s+%d%%", CGameData::Inst()->getLanguageValue("cntry_hp_0"), int(effect));

    showBuffEffect("buff_small_hp.png", buf, CCPointMake(450, 320));
        
    showLeaderBtn();
}

void CountryDetails::showLeaderBtn()
{
//    CCountryInfo* cntryInfo = CGameData::Inst()->getCntryInfo();
    
    CCSprite* spMenu = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSize szBtn = spMenu->getContentSize();
        
//    CCMenuItem *mItemManage;
//    newBtn(mItemManage, CGameData::Inst()->getLanguageValue("cntry_manage"),
//           CCPointMake(200 - szBtn.width * 0.5, 206), enCDBtn_Manage);

    
    CCMenuItem *mItemBack;
    newBtn(mItemBack, CGameData::Inst()->getLanguageValue("back"),
           CCPointMake(320 - szBtn.width * 0.5, 206), enCDBtn_Back);
    
    CCMenu* menu = CCMenu::menuWithItems(mItemBack, NULL);
    menu->setPosition(CCPointZero);
    addChild(menu);
}

void CountryDetails::showMemberBtn(bool bCntrySelf)
{
    CCSprite* spMenu = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSize szBtn = spMenu->getContentSize();

    CCMenuItem *mItemBack = NULL;
    CCMenuItem *mItemQuit = NULL;
    
    if (bCntrySelf)
    {
        newBtn(mItemQuit, CGameData::Inst()->getLanguageValue("cntry_quit_country"),
               CCPointMake(200 - szBtn.width * 0.5, 206), enCDBtn_Quit);
        
        newBtn(mItemBack, CGameData::Inst()->getLanguageValue("back"),
               CCPointMake(440 - szBtn.width * 0.5, 206), enCDBtn_Back);
    }
    else
    {
        newBtn(mItemBack, CGameData::Inst()->getLanguageValue("back"),
               CCPointMake(320 - szBtn.width * 0.5, 206), enCDBtn_Back);
    }
    
    CCMenu* menu = CCMenu::menuWithItems(mItemBack, mItemQuit, NULL);
    menu->setPosition(CCPointZero);
    addChild(menu);
}

void CountryDetails::showNormalBtn(bool bApply)
{
    CCSprite* spMenu = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSize szBtn = spMenu->getContentSize();

    CCMenuItem *mItemApply;
    if (bApply) {
        newBtn(mItemApply, CGameData::Inst()->getLanguageValue("cntry_applied_alone"),
               CCPointMake(220 - szBtn.width * 0.5, 206), enCDBtn_Apply);
        mItemApply->setIsEnabled(false);
        
        CCSprite* spShadow = CCSprite::spriteWithSpriteFrameName("fr_button black.png");
        spShadow->setPosition(CCPointMake(spMenu->getContentSize().width * 0.5,
                                          spMenu->getContentSize().height * 0.5));
        mItemApply->addChild(spShadow);
    }
    else
        newBtn(mItemApply, CGameData::Inst()->getLanguageValue("cntry_apply_alone"),
               CCPointMake(220 - szBtn.width * 0.5, 206), enCDBtn_Apply);
    
    CCMenuItem *mItemBack;
    newBtn(mItemBack, CGameData::Inst()->getLanguageValue("back"),
           CCPointMake(420 - szBtn.width * 0.5, 206), enCDBtn_Back);
    
    CCMenu* menu = CCMenu::menuWithItems(mItemApply, mItemBack, NULL);
    menu->setPosition(CCPointZero);
    addChild(menu);
}

void CountryDetails::newBtn(CCMenuItem *&menuItem, const char *label,
                         const CCPoint& position, int tag)
{
    CCSprite* spMenu1 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    CCSprite* spMenu2 = CCSprite::spriteWithSpriteFrameName("fr_button.png");
    menuItem = CCMenuItemImage::itemFromNormalSprite(spMenu1,
                                                     spMenu2,
                                                     m_pListener,
                                                     m_pfnSelector);
    menuItem->setTag(tag);
    menuItem->setAnchorPoint(CCPointZero);
    menuItem->setPosition(position);
    
    TextNode* tempText = TextNode::textWithString(label,
                                                  spMenu1->getContentSize(),
                                                  CCTextAlignmentCenter,
                                                  30);
    tempText->setShadowColor(ccBLACK);
    tempText->setPosition(CCPointMake(spMenu1->getContentSize().width * 0.5 - 2,
                                      spMenu1->getContentSize().height * 0.5 + 2));
    menuItem->addChild(tempText);
}

void CountryDetails::showCntryProperty(const char *title, const char *value, float posY)
{
    TextNode* tnTitle = TextNode::textWithString(title, 30);
    tnTitle->setColor(ccBLACK);
    tnTitle->setAnchorPoint(CCPointMake(0, 0.5));
    tnTitle->setPosition(CCPointMake(95, posY));
    addChild(tnTitle);
    
    CCSprite* spBg = CCSprite::spriteWithFile("cntryDetails_bg1.png");
    spBg->setPosition(CCPointMake(425, posY));
    addChild(spBg);
    
    TextNode* tnValue = TextNode::textWithString(value, 30);
    tnValue->setColor(ccc3(168, 2, 6));
    tnValue->setPosition(CCPointMake(spBg->getContentSize().width * 0.5,
                                     spBg->getContentSize().height * 0.5));
    spBg->addChild(tnValue);
}

void CountryDetails::showBuffEffect(const char *iconName, const char *effect,
                                    const cocos2d::CCPoint &position)
{    
    CCSprite* spIcon = CCSprite::spriteWithSpriteFrameName(iconName);
    spIcon->setPosition(CCPointMake(position.x - 95, position.y));
    addChild(spIcon);

    TextNode* tnEffect = TextNode::textWithString(effect, 24);
    tnEffect->setColor(ccc3(161, 226, 2));
    tnEffect->setAnchorPoint(CCPointMake(0, 0.5));
    tnEffect->setPosition(CCPointMake(position.x - 70, position.y));
    addChild(tnEffect);
}


