//
//  CountryDialog.h
//  AgainstWar
//
//  Created by Liu Hao on 12-12-27.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__CountryPutIn__
#define __AgainstWar__CountryPutIn__

#include <iostream>
#include "cocos2d.h"
#include "CGameData.h"

using namespace cocos2d;

enum enCDialogType
{
    enCDialogType_null = 0,
    enCDialogType_DonateCoin,
    enCDialogType_Search,
    enCDialogType_SetDeclaration,
    enCDialogType_CntryLvUp,
    enCDialogType_CreateTip,
    enCDialogType_BuffLvUp,
    enCDialogType_CurrentInfo,
    enCDialogType_AssignOne,
    enCDialogType_AssignAll,
    enCDialogType_AssignOut,
    enCDialogType_SetNameCard,
    enCDialogType_SureNameCard,
    enCDialogType_JiFengAssignOne,
    enCDialogType_AssignWeek,
};

class CntryDlgRet : public CCObject
{
public:
    bool bOk;
};

class TextNode;
class Dialog0;
class Dialog1;

class CountryDialog : public CCLayer, public CCTextFieldDelegate
{
public:
    CountryDialog(CCObject* target, SEL_CallFuncO selector);
    CountryDialog(CCObject* target, SEL_CallFunc selector);
    ~CountryDialog();
    
    virtual void onEnter();
    virtual void onExit();
    
    void showSetNameCardDialog();   // 修改同盟名片
    void showSureNameCardDialog();  // 确认同盟名片修改
    void showDonateCoinDialog();    // 捐献元宝
    void showCntrySearchDialog();   // 搜索国家
    void showSetDeclarationDialog();// 修改宣言
    void showCntryLevelUpDialog();  // 国家升级
    void showCntryCreateTipDialog();// 国家创建提示
    void showBuffLevelUpDialog(enBuffTypeDef buffType);   // Buff升级
    void showCntryCurrentInfo();    // 国家近况
    void showSupplyAssignOneDialog(CMember* memberInfo);   // 分配个人粮饷
    void showSupplyAssignAllDialog();                       // 平均分配给所有人
    void showSupplyAssignOutDialog();                       // 按贡献分配给所有人
    void showSupplyAssignWeekDialog();                      // 按周贡献分配给所有人
    
    void showCntryRank();        // 伤害排行
    void showFbdwords();
    void showJiFengAssignOneDlg(JiFengRecordInf* pInf);
    
    bool searchCountry();
    bool setDeclaration();
    bool donateCoin();
    bool levelUpCntry();
    bool assignOneSupply();
    bool assignAllSupply();
    bool assignOutSupply();
    bool assignWeekSupply();
    bool setNameCard();
    bool assignOneCoin();
    
    void Dlg2Callback(CCObject* pSender);               //是否进入元宝商店
    
private:
    enCDialogType   m_type;
    CCObject*       m_pListener;
    SEL_CallFunc    m_pfnSelector;
    SEL_CallFuncO   m_pfnOSelector;

    CCTextFieldTTF* m_textField;
    bool            m_bShowKeyboard;
    char            m_memberId[100];
    
    CCMenuItem*     m_mItemOK;
    CCMenuItem*     m_mItemCancel;
    
    TextNode*       m_tnCntrySupply;
    bool            m_bPutInError;
    Dialog0*        m_fbdwords;
    
    CntryDlgRet     m_ret;
    
    Dialog1*        m_dlgAlert;
    
    int             lCnt;       //上古彩石剩余数量
    string          strItem;    //上古彩石item  1101_item
    
private:
    virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen);
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    void menuCallBack(CCObject* pObj);
        
    void showKeyboard();
    void hidekeyboard();
    
    void cbshowFbdwords();
    void setSameDonateDialog(); // 捐献
    void setSameAssignDialog(); // 分配
    
    void showAlertNoGold();
    void cbAlertNoGold();
    
};


#endif /* defined(__AgainstWar__CountryPutIn__) */
