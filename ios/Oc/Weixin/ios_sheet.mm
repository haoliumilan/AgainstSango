//
//  ios_sheet.m
//  Cocos2d_ios
//
//  Created by user on 12-11-17.
//
//



#import "ios_sheet.h"
#import "AppController.h"
#import "Sanguoshare.h"
#import "CGameData.h"
#include "MainScene.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

@implementation ios_sheet



- (void)viewDidLoad {
    [super viewDidLoad];
    wxtype=0;
    sinaft=YES;
    qqft=YES;
    weixinft=YES;
    //向微信注册
    [WXApi registerApp:@"wx5e2fd0219cc58026"];
    
}


- (void) RespImageContent
{
    NSArray *paths                  = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString *documentsDirectory    = [paths objectAtIndex:0];
    
    NSString* path                  = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithUTF8String:"captureImage.png"]];
    
    
    WXMediaMessage *message         = [WXMediaMessage message];
    message.thumbData               =  [NSData dataWithContentsOfFile:path] ;
    //  [message setThumbImage:[UIImage imageNamed:@"Icon.png"]];
    
    
    WXImageObject *ext              = [WXImageObject object];
    path                            = [documentsDirectory stringByAppendingPathComponent:[NSString stringWithUTF8String:"captureImage.jpg"]];
    
    ext.imageData                   = [NSData dataWithContentsOfFile:path] ;
    
    
    message.mediaObject             = ext;
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText               = NO;
    req.message             = message;
    req.scene               = WXSceneTimeline;
    
    [WXApi sendReq:req];
}



#define BUFFER_SIZE 1024 * 100
- (void) sendAppContent
{
    // 发送内容给微信
    
    NSString* weixin_title      =   [NSString stringWithUTF8String:CGameData::Inst()->getCommonInfo()->voice_8  ];
    NSString* weixin_descripion =   [NSString stringWithUTF8String:CGameData::Inst()->getCommonInfo()->voice_9  ];
    NSString* weixin_url        =   [NSString stringWithFormat:@"%s",CGameData::Inst()->getCommonInfo()->app_url];
    NSString* weixin_extInfo    =   [self RerutnNSstringutf8:"weixin_extInfo" ];
    
    if (wxtype==1) {
        
        CUserInfo *userInfo     =   CGameData::Inst()->getUsrInfo();
        NSString *inviteCode    =   [NSString stringWithUTF8String:userInfo->invite_code];
        NSString*invite         =   [self RerutnNSstringutf8:"weixininvite" ];
        weixin_title            =   [NSString stringWithFormat:@"%@%@%@",invite,inviteCode,weixin_title];
        [invite release];
    }
    
    
    
    WXMediaMessage *message     =   [WXMediaMessage message];
    message.title               =   weixin_title;
    message.description         =   weixin_descripion;
    [message setThumbImage:[UIImage imageNamed:@"icon.jpg"]];
    
    
    
    WXAppExtendObject *ext      =   [WXAppExtendObject object];
    ext.extInfo                 =   weixin_extInfo;
    ext.url                     =   weixin_url;
    
    [weixin_extInfo release];
    
    Byte* pBuffer   = (Byte *)malloc(BUFFER_SIZE);
    memset(pBuffer, 0, BUFFER_SIZE);
    NSData* data    = [NSData dataWithBytes:pBuffer length:BUFFER_SIZE];
    free(pBuffer);
    
    ext.fileData            = data;
    message.mediaObject     = ext;
    
    SendMessageToWXReq* req = [[[SendMessageToWXReq alloc] init]autorelease];
    req.bText               = NO;
    req.message             = message;
    if (CGameData::Inst()->getCommonInfo()->weixin_share_type == "0")
        req.scene               = WXSceneSession;
    else
        req.scene               = WXSceneTimeline;
    
    //  [WXApi sendReq:req];
    
    if (![WXApi sendReq:req]) {
       
        theshareSanguo->showWeixinUnLoadDlg();
    }
    
}

-(void) RespAppContent
{
    
    WXMediaMessage *message = [WXMediaMessage message];
    message.title           = @"逆转三国";
    message.description     = @"逆转三国app";
    [message setThumbImage:[UIImage imageNamed:@"icon.jpg"]];
    
    WXAppExtendObject *ext  = [WXAppExtendObject object];
    ext.extInfo             = @"<xml>逆转三国</xml>";
    
    
    
    Byte* pBuffer   = (Byte *)malloc(BUFFER_SIZE);
    memset(pBuffer, 0, BUFFER_SIZE);
    NSData* data    = [NSData dataWithBytes:pBuffer length:BUFFER_SIZE];
    free(pBuffer);
    
    ext.fileData            = data;
    message.mediaObject     = ext;
    
    GetMessageFromWXResp* resp  = [[[GetMessageFromWXResp alloc] init] autorelease];
    resp.message                = message;
    resp.bText                  = NO;
    
    [WXApi sendResp:resp];
}



-(NSString*)RerutnNSstringutf8:(const char*)  string{
    
    const char *utf8Replace = CGameData::Inst()->getLanguageValue(string);
    NSData* data            = [NSData dataWithBytes:utf8Replace length:strlen(utf8Replace)];
    NSString* Share_Sanguo  = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    return Share_Sanguo;
    
}

-(void) setWxTpe:(NSInteger)type{
    
    wxtype=type;
    
}

// show UIActionSheet
-(void) boxSheet:(NSInteger) type0
{
    
    if (mySheet) {
        [mySheet release];
    }
    
    NSString* Share_code    =   [self RerutnNSstringutf8:"share_copy_code" ];
    NSString* Share_Sanguo  =   [self RerutnNSstringutf8:"share_sanguo" ];
    NSString* Share_Weixin  =   [self RerutnNSstringutf8:"share_Weixin" ];
    NSString* Share_TX_XL   =   [self RerutnNSstringutf8:"share_TX_XL"  ];
    NSString* Share_cancel  =   [self RerutnNSstringutf8:"cancelTip"    ];
    
    sinaft=CGameData::Inst()->getCommonInfo()->sina_voice_fg;
    qqft=CGameData::Inst()->getCommonInfo()->qq_voice_fg ;
    weixinft=CGameData::Inst()->getCommonInfo()->weixin_voice_fg;
    invitation_code = type0;  // true -> 邀请码
    
    if ((sinaft||qqft)&&weixinft) {
        if (!invitation_code)
            mySheet = [[UIActionSheet alloc]
                       initWithTitle:Share_Sanguo
                       delegate:self
                       cancelButtonTitle:Share_cancel
                       destructiveButtonTitle:nil
                       otherButtonTitles:Share_Weixin, Share_TX_XL,nil];
        else
            mySheet = [[UIActionSheet alloc]
                       initWithTitle:Share_Sanguo
                       delegate:self
                       cancelButtonTitle:Share_cancel
                       destructiveButtonTitle:nil
                       otherButtonTitles:Share_code, Share_Weixin, Share_TX_XL,nil];

    }
    
    else if (!sinaft&&!qqft&&weixinft) {
        if (!invitation_code)
            mySheet = [[UIActionSheet alloc]
                       initWithTitle:Share_Sanguo
                       delegate:self
                       cancelButtonTitle:Share_cancel
                       destructiveButtonTitle:nil
                       otherButtonTitles:Share_Weixin,nil];
        else
            mySheet = [[UIActionSheet alloc]
                       initWithTitle:Share_Sanguo
                       delegate:self
                       cancelButtonTitle:Share_cancel
                       destructiveButtonTitle:nil
                       otherButtonTitles:Share_code, Share_Weixin,nil];

    }
    
    else if ((sinaft||qqft)&&!weixinft) {
        if (!invitation_code)
            mySheet = [[UIActionSheet alloc]
                       initWithTitle:Share_Sanguo
                       delegate:self
                       cancelButtonTitle:Share_cancel
                       destructiveButtonTitle:nil
                       otherButtonTitles:Share_TX_XL,nil];
        else
            mySheet = [[UIActionSheet alloc]
                       initWithTitle:Share_Sanguo
                       delegate:self
                       cancelButtonTitle:Share_cancel
                       destructiveButtonTitle:nil
                       otherButtonTitles:Share_code, Share_TX_XL,nil];

    }else{
        return;
    }
    
    [mySheet showInView:self.view];
    
    [ Share_code    release];
    [ Share_Sanguo  release];
    [ Share_Weixin  release];
    [ Share_TX_XL   release];
    [ Share_cancel  release];
    
    
    
}





- (void)actionSheet:(UIActionSheet *)actionSheet didDismissWithButtonIndex:(NSInteger)buttonIndex
{
    if ((sinaft||qqft)&&weixinft){
    
        switch (buttonIndex) {
            case 0:
                if (invitation_code)
                {
                    [self CopyCode];
                    break;
                }
                if(CGameData::Inst()->isSoundOn)
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("btnok.caf");
                
               // CGameData::Inst()->setWeiXinType(1);
                theshareSanguo->setShareType(1);
                
                [self sendAppContent];
                break;
            case 1:
                if (invitation_code)
                {
                    if(CGameData::Inst()->isSoundOn)
                        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("btnok.caf");
                    theshareSanguo->setShareType(1);
                    [self sendAppContent];
                    break;
                }
                
               // CGameData::Inst()->setWeiXinType(0);
                theshareSanguo->setShareType(0);
                theshareSanguo->callweibo();
                break;
            case 2:
                if (invitation_code)
                {
                    theshareSanguo->setShareType(0);
                    theshareSanguo->callweibo();
                    break;
                }
                break;
            default:
                break;
        }

    
    } else if (!sinaft&&!qqft&&weixinft) {
        switch (buttonIndex) {
            case 0:
                if (invitation_code)
                {
                    [self CopyCode];
                    break;
                }
                if(CGameData::Inst()->isSoundOn)
                    CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("btnok.caf");
                
                //CGameData::Inst()->setWeiXinType(1);
                theshareSanguo->setShareType(1);
                [self sendAppContent];
                break;
            case 1:
                if (invitation_code)
                {
                    if(CGameData::Inst()->isSoundOn)
                        CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("btnok.caf");
                    theshareSanguo->setShareType(1);
                    [self sendAppContent];
                    break;
                }
                break;
            default:
                break;
        }

    }
    
    else if ((sinaft||qqft)&&!weixinft) {
        
        switch (buttonIndex) {
            case 0:
                if (invitation_code)
                {
                    [self CopyCode];
                    break;
                }
                //CGameData::Inst()->setWeiXinType(0);
                theshareSanguo->setShareType(0);
                theshareSanguo->callweibo();
                break;
            case 1:
                break;
            default:
                break;
        }

    }
    
}

-(void) CopyCode{
    NSString* str_copy = [NSString stringWithFormat:@"%s", CGameData::Inst()->getUsrInfo()->invite_code];
    [[UIPasteboard generalPasteboard] setString:str_copy];
}

- (void)dealloc {
    [mySheet release];
    [super dealloc];
}


@end


