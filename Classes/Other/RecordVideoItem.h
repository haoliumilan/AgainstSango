//
//  RecordVideoItem.h
//  AgainstWar
//
//  Created by caojiye on 13-7-24.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#ifndef __AgainstWar__RecordVideoItem__
#define __AgainstWar__RecordVideoItem__

////////////////////////////////////////////////////////////
#include "cocos2d.h"
#include "RecordData.h"
////////////////////////////////////////////////////////////
using namespace cocos2d;

class TextNode;
////////////////////////////////////////////////////////////
enum enRecordVideoItemRetValue
{
    en_rtValue_none = 1,
    en_rtValue_uploading_wait,    // 有其他视频正在上传
    en_rtValue_show_commdlg,      // 显示正在通讯的匡
    en_rtValue_remove_commdlg,
    en_rtValue_retry_commdlg,     // 401错误的重试
    en_rtValue_show_got_award,    // 成功领取奖励
    en_rtValue_refresh,           // 刷新
};

////////////////////////////////////////////////////////////
class RecordVideoItemRet : public CCObject
{
public:
    int rtValue;
    CCObject*         m_pListener = NULL;
    SEL_CallFuncO     m_pfnSelector = NULL;
};
////////////////////////////////////////////////////////////
class RecordVideoItem : public CCLayer
{
    CCObject*         m_pListener;
    SEL_CallFuncO     m_pfnSelector;
    
    TextNode*         m_text_button;
    TextNode*         m_text_upload_progress;
    short             m_en_upload_state;
    ConfigData        m_configdata;
    
    RecordVideoItemRet m_ret;
    
public:
    
    static RecordVideoItem* CreateWith(CCObject* target, SEL_CallFuncO selector, ConfigData data);
    
private:
    
    RecordVideoItem(CCObject* target, SEL_CallFuncO selector, ConfigData data);
    
    void SetInitialValue();
    void SetState(int state);
    
    void cbButton();
    
    void StartUploadVideo();
    void UploadSuccess();
    void UploadFail();
    
    void CancelUploading();
    
    void GetAward();
    
    void ViewRecord();
    
    // update
    void updateUploading(ccTime dt);
    void UpdateGetAward(ccTime dt);
    void cbCommDlg(CCObject* pObj);
    
};

////////////////////////////////////////////////////////////


#endif /* defined(__AgainstWar__RecordVideoItem__) */
