//
//  BonusPanel.cpp
//  test
//
//  Created by mac on 13-6-5.
//
//

#include "BonusPanel.h"
#include "BonusList.h"
#include "TextNode.h"
#include "CGameData.h"
#include "MainScene.h"
#include "CommDlg.h"
#include "CountryDialog.h"

BonusPanel::BonusPanel()
{
    m_commDlg = NULL;
    m_cntryDialog = NULL;
    
    m_lyPanel = CCLayer::node();
    addChild(m_lyPanel);
    
    CCSprite* spBg = CCSprite::spriteWithFile("bonuspanel.png");
    if (spBg) {
        m_lyPanel->addChild(spBg, -1);
        spBg->setAnchorPoint(ccp(0,1));
        spBg->setPosition(CCPointZero);
    }

    char buf[64];

    //元宝数
    long lGuildCoin = CGameData::Inst()->getGuildCoin();
    snprintf(buf, sizeof(buf), "%s:%ld", CGameData::Inst()->getLanguageValue("deadDlgtip6"), lGuildCoin);
    m_txtCoin = TextNode::textWithString(buf, 22);
    m_txtCoin->setPosition(CCPointMake(307, -34));
    m_txtCoin->setColor(ccBLACK);
    m_lyPanel->addChild(m_txtCoin);
    
    //成员、近5次伤害、上次伤害
    TextNode* txtMember = TextNode::textWithString(CGameData::Inst()->getLanguageValue("cntry_member_alone"), 22);
    txtMember->setPosition(CCPointMake(120, -70));
    txtMember->setColor(ccc3(93, 0, 4));
    m_lyPanel->addChild(txtMember);
    
    TextNode* txtRecentScore = TextNode::textWithString(CGameData::Inst()->getLanguageValue("jifeng_recentscore"), 22);
    txtRecentScore->setPosition(CCPointMake(270, -70));
    txtRecentScore->setColor(ccc3(93, 0, 4));
    m_lyPanel->addChild(txtRecentScore);
    
    TextNode* txtLastScore = TextNode::textWithString(CGameData::Inst()->getLanguageValue("jifeng_lastscore"), 22);
    txtLastScore->setPosition(CCPointMake(410, -70));
    txtLastScore->setColor(ccc3(93, 0, 4));
    m_lyPanel->addChild(txtLastScore);
    
    //列表
    m_pBonusList = new BonusList(this, callfuncO_selector(BonusPanel::cbBonusList));
    m_lyPanel->addChild(m_pBonusList);
    m_pBonusList->setPosition(CCPointZero);
}

void BonusPanel::toggleSort()
{
    if ((m_lyPanel->getIsVisible()) && (m_pBonusList)) {
        m_pBonusList->toggleSort();
    }
}

void BonusPanel::cbBonusList(CCObject *pObj)
{
    JiFengRecordInf* pInf = (JiFengRecordInf*)pObj;
    m_strUid = pInf->strUid;
    
    if (m_cntryDialog == NULL)
    {
        m_cntryDialog = new CountryDialog(this, callfunc_selector(BonusPanel::cbCntryDlg));
        addChild(m_cntryDialog);
        m_cntryDialog->release();
    }
    
    m_cntryDialog->setPosition(ccp(-getPosition().x, -getPosition().y));
    m_cntryDialog->showJiFengAssignOneDlg(pInf);
    
    m_lyPanel->setIsVisible(false);
}

void BonusPanel::cbCommDlg(CCObject* pObj)
{
    CommDlgRet* pRet = (CommDlgRet*)pObj;
    bool isOk = pRet->bOk;
    
    if(m_commDlg != NULL){
        removeChild(m_commDlg, true);
        m_commDlg = NULL;
    }
    
    MainScene::Inst()->enableBottomPanel(true);
    
    if(isOk == true){
        if (m_cntryDialog->assignOneCoin())
        {
            scheduleUpdate();
        }
    }
    else {
        CGameData::Inst()->clearReqStat();
    }
}

void BonusPanel::update(ccTime dt)
{
    int comState = CGameData::Inst()->getReqStat();
    if(comState == kReqStatRequesting){
        if(m_commDlg == NULL){
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this,
                                    callfuncO_selector(BonusPanel::cbCommDlg),
                                    enCommDlgTp_connecting);
            addChild(m_commDlg, 1000);
            CCPoint ptThis = getPosition();
            m_commDlg->setPosition(CCPointMake(320 - ptThis.x, 480 - ptThis.y));
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
    
    if(comState == kReqStatOk){
        if (m_cntryDialog) {
            m_cntryDialog->removeFromParentAndCleanup(true);
            m_cntryDialog = NULL;
        }
        
        char buf[64];
        long lGuildCoin = CGameData::Inst()->getGuildCoin();
        snprintf(buf, sizeof(buf), "%s:%ld", CGameData::Inst()->getLanguageValue("deadDlgtip6"), lGuildCoin);
        m_txtCoin->setString(buf);
        m_lyPanel->setIsVisible(true);
    }
    else {
        if(m_commDlg == NULL)
        {
            MainScene::Inst()->enableBottomPanel(false);
            m_commDlg = new CommDlg(this, callfuncO_selector(BonusPanel::cbCommDlg), enCommDlgTp_retry);
            addChild(m_commDlg, 1000);
            
            CCPoint ptThis = getPosition();
            m_commDlg->setPosition(CCPointMake(320 - ptThis.x, 480 - ptThis.y));
            m_commDlg->release();
            m_commDlg->setScale(0.0);
            m_commDlg->runAction(CCScaleTo::actionWithDuration(0.2, 1.0));
        }
    }
}

void BonusPanel::cbCntryDlg()
{
    if (m_cntryDialog->assignOneCoin())
    {
        scheduleUpdate();
    }
}

bool BonusPanel::backBtnProc()
{
    if (m_cntryDialog) {
        m_cntryDialog->removeFromParentAndCleanup(true);
        m_cntryDialog = NULL;
        
        m_lyPanel->setIsVisible(true);
        
        return true;
    }
    else{
        return false;
    }
}



