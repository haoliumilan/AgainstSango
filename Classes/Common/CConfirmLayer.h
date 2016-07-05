//
//  CConfirmLayer.h
//  AgainstWar
//
//  Created by Hao Zhang on 12-5-22.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef AgainstWar_CConfirmLayer_h
#define AgainstWar_CConfirmLayer_h

#include "cocos2d.h"
#include "CommonDef.h"

using namespace cocos2d;


class CUserCardInfo;

enum enCConfirmLayerType {
    enCConfirmLayer_NORMAL = 0,
    enCConfirmLayer_LOCK,
    enCConfirmLayer_UNLOCK,
    enCConfirmLayer_FRIEND,
    enCConfirmLayer_LViceleader,    // leader's viceleader(friend)
    enCConfirmLayer_LVStranger,     // leader's viceleader(not friend)
    enCConfirmLayer_LMember,        // leader's member(friend)
    enCConfirmLayer_LMStranger,     // leader's member(not friend)
    enCConfirmLayer_VLMember,       // viceleader's member(friend, not leader and other viceleader)
    enCConfirmLayer_VLMStranger,    // viceleader's member(not friend, not leader and other viceleader)
    enCConfirmLayer_MMember,        // member's member(friend)
    enCConfirMLayer_MMStranger,     // member's member(not friend)
    enCConfirmLayer_EVO,            // 转生
    enCConfirmLayer_SEVO,           // 超转生
    enCConfirmLayer_Wake,           // 觉醒
    enCConfirmLayer_POWUP,          // 强化
    enCConfirmLayer_NIEPAN,         // 涅槃
    enCConfirmLayer_POWUPNIEPAN,    // 强化&涅槃
    enCConfirmlayer_maa_arm,        // 拜师
    enCConfirmLayer_nEVO_Wake,      // 转生和觉醒
    enCConfirmLayer_nFiveTeam,      // 5队齐上阵
};

//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CCfmLayerRet : public CCObject
{
public:
    int     iBtnSel;
    int     iConfirmType;
};


//_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
class CConfirmLayer : public CCLayer
{

    CCObject*       m_pListener;
    SEL_CallFuncO   m_pfnSelector;
    CCfmLayerRet    m_ret;
    enCConfirmLayerType m_cfmLayerType;
    int             m_nOPIndex;
    
    bool            m_NewbieFirst;

public:
    CConfirmLayer(CCObject* target, SEL_CallFuncO selector);

    virtual void onExit();
    
    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
    
    void show(CUserCardInfo*, const char*, enCConfirmLayerType type = enCConfirmLayer_NORMAL,int nOPIndex = -1);
    void showFiveTeam(CCMutableArray<CUserCardInfo *> *pUsrCardInfoArr,enCConfirmLayerType type = enCConfirmLayer_NORMAL,int nOPIndex = -1);
    void hide();
    enCConfirmLayerType getCfmLayerType();
    int getOPIndex()const{return m_nOPIndex;}
    
private:
    void showBtnNormal();
    void showBtnLock();
    void showBtnUnlock();
    void showBtnFriend();
    void showBtnLViceleader();      // leader's viceleader
    void showBtnLVStranger();
    void showBtnLMember();          // leader's member
    void showBtnLMStranger();
    void showBtnVLMember();         // viceleader's member
    void showBtnVLMStranger();
    void showBtnMMember();          // member's member
    void showBtnMMStranger();
    void showBtn_Evo_Wake();        //转生＋觉醒
    void showBtnEvo();              //转生
    void showBtnSevo();             //超转生
    void showBtnWake();             //觉醒
    void showBtnPowUp();            //强化
    void showBtnNiePan();           //涅槃
    void showBtnPowUpNiePan();      //强化&涅槃
    void showBtnMAAarm();           //拜师
    void showBtnFiveTeam();         //五队同时上阵
    
    void showSameBtn(const char **title, int btnCount);
    
    bool isSelectedForPos(CCPoint pos, int btnCount);
};

#endif
