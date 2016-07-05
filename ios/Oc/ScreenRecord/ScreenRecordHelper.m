//
//  ScreenRecordHelper.m
//  AgainstWar
//
//  Created by caojiye on 13-7-2.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//
//////////////////////////////////////////////////////////////////
#import "ScreenRecordHelper.h"
#import "JSONKit.h"
#import "NSString+SBJSON.h"
#import "THCapture.h"
#import "JSONKit.h"
//////////////////////////////////////////////////////////////////
#define k56sID        @"kksjcue5558cw3x8wz"  // 随便写 不要重复
const int k_local_max = 10;
//////////////////////////////////////////////////////////////////
static ScreenRecordHelper* inst_screen_record_helper = nil;
//////////////////////////////////////////////////////////////////
@interface ScreenRecordHelper()
<UploadEngineDelegate, APIEngineDelegate>
{
    short             m_progress; // 开始上传=0  成功=100 失败=-1
}
@end
//////////////////////////////////////////////////////////////////
@implementation ScreenRecordHelper
//////////////////////////////////////////////////////////////////
+ (ScreenRecordHelper*) ShareHelper
{
    NSAssert(inst_screen_record_helper != nil, @"use ShareWithUid");

    return inst_screen_record_helper;
}

+ (ScreenRecordHelper*) ShareWithUid:(NSString*) _uid
{
    if (inst_screen_record_helper != nil)
    {
        [inst_screen_record_helper release];
    }
    inst_screen_record_helper = [[ScreenRecordHelper alloc] initWithUid:_uid];

    return inst_screen_record_helper;
}

- (id) initWithUid:(NSString*)_uid;
{
    self = [super init];
    if (self)
    {
        m_progress = -1;
        
        APIEngine* eng = [APIEngine sharedAPIEngine];
        [eng setDelegate:self];
        self.apiengine = eng;
        
        UploadEngine* eng1 = [UploadEngine sharedUploadEngine];
        [eng1 setDelegate:self];
        self.uploadEngine = eng1;
        
        self.game_uid = [NSString stringWithString:_uid];
        self.path_current_video = [NSHomeDirectory() stringByAppendingFormat:@"/Documents/%@/%@", _uid, @"CurrentVideo"];
        self.path_previous_video = [NSHomeDirectory() stringByAppendingFormat:@"/Documents/%@/%@", _uid, @"PreviousVideo"];
        
        [[NSFileManager defaultManager] createDirectoryAtPath:[NSHomeDirectory() stringByAppendingPathComponent:[NSString stringWithFormat:@"Documents/%@", _uid]] withIntermediateDirectories:YES attributes:nil error:nil];
        
        [self SetVideoData];
    }
    return self;
}

- (void) SetVideoData
{
    if (self.current_video == nil)
    {
        self.current_video = [[NSMutableArray alloc] init];
    }
    else
    {
        [self.current_video removeAllObjects];
    }
    NSArray* arr = [self GetLocalCurrentVideo];
    [self.current_video addObjectsFromArray:arr];
    
    if (self.previous_video == nil)
    {
        self.previous_video = [[NSMutableDictionary alloc] init];
    }
    else
    {
        [self.previous_video removeAllObjects];
    }
    NSDictionary* dic = [self GetLocalPreviousVideo];
    [self.previous_video setDictionary:dic];

}

- (void) UploadVideoWithName:(NSString*)_name
{
    NSString* video_path = [NSHomeDirectory() stringByAppendingFormat:@"/Documents/%@/%@.mov", self.game_uid, _name];  // 获取视频的地址
    NSDictionary* dic = [NSDictionary dictionaryWithObject:_name forKey:@"name"];
    
    m_progress = 0;
    [self.uploadEngine close];
    [self.uploadEngine uploadVideoWithLocalPath:video_path videoTitle:@"title" videoTag:@"视频" videoDescription:_name videoType:@"clear" sID:self.game_uid category:@"16" userInfo:dic];
}

- (void) finishedUpload:(NSString*)result userInfo:(NSDictionary*)info
{
    NSLog(@"upload success = %@", result);
    NSString* time0 = [info objectForKey:@"name"];

    for (NSDictionary* each in self.current_video)
    {
        if ([[each objectForKey:@"time0"] isEqualToString:time0])
        {
            int index = [self.current_video indexOfObject:each];
            NSMutableDictionary* mu_dic = [NSMutableDictionary dictionaryWithDictionary:each];
            [mu_dic setObject:result forKey:@"vid"];
            [self.current_video replaceObjectAtIndex:index withObject:mu_dic];
            break;
        }
    }
    [self SaveCurrentVideo];
    m_progress = 100;
}

-(void) failUpload:(NSError*)error videoResumeInfo:(NSString*)videoInfo userInfo:(NSDictionary*)info
{
    NSLog(@"upload failed");
    m_progress = -1;
}

- (void) progressUpload:(float)progress userInfo:(NSDictionary*)info
{
    NSLog(@"uploading = %f", progress);
    m_progress = progress*100;
}

- (float) GetUploadProgress
{
    return m_progress;
}

- (void) SaveToAlbum
{
 //   NSString* name = [[self.current_video lastObject] objectForKey:@"time0"];
//    NSString* last_replay_path = [NSHomeDirectory() stringByAppendingFormat:@"/Documents/%@/%@.mov", self.game_uid, name];
    
    if (UIVideoAtPathIsCompatibleWithSavedPhotosAlbum(self.path_recording)) //last_replay_path
    {
        UISaveVideoAtPathToSavedPhotosAlbum(self.path_recording, nil, nil, nil);
    }
}

- (void) SetPathRecording:(NSString*)_path
{
    self.path_recording = [NSHomeDirectory() stringByAppendingFormat:@"/Documents/%@/%@.mov", self.game_uid, _path];
}

// --------------------  服务器 ----------------------
- (void) RequestVideoInfo
{
    for (NSDictionary* each in self.current_video)
    {
        if ([[each objectForKey:@"state"] intValue] == en_upload_state_reviewing)
        {
            [self RequestVideoInfo:[each objectForKey:@"vid"]];
        }
    }
}
- (void) RequestVideoInfo:(NSString*) _vid
{
    if (_vid.length<5)
        return;
    NSLog(@"begin api request");
    [self.apiengine getVideoInfoWithID:_vid userInfo:nil];
}

-(void)finishedAPIReqeust:(NSString*)result userInfo:(NSDictionary*)info
{
    NSLog(@"finish api request");
    NSDictionary* data = (NSDictionary*)[result JSONValue];
    
    if (![data objectForKey:@"0"])
    {
        NSLog(@"00000");
        return;
    }
    
    if (![[[data objectForKey:@"0"] objectForKey:@"chk_yn"] isEqualToString:@"y"])
    {
        NSLog(@"nnnnn");
        return;
    }
    NSLog(@"pass review");
    
    NSString* img_url = [[data objectForKey:@"0"] objectForKey:@"bimg"];
    NSString* video_url = [[data objectForKey:@"0"] objectForKey:@"url"];
    for (NSDictionary* each in self.current_video)
    {
        NSString* str1 = STRING_VAL([each objectForKey:@"vid"]);
        NSString* str2 = STRING_VAL([[data objectForKey:@"0"] objectForKey:@"vid"]);  // vid 不是一个string
        if ([str1 isEqualToString:str2])
        {
            int index = [self.current_video indexOfObject:each];
            NSMutableDictionary* mu_dic = [NSMutableDictionary dictionaryWithDictionary:each];
            [mu_dic setObject:video_url forKey:@"video_url"];
            [mu_dic setObject:img_url forKey:@"img_url"];
            [mu_dic setObject:[NSString stringWithFormat:@"%d", en_upload_state_reviewed] forKey:@"state"];
            [self.current_video replaceObjectAtIndex:index withObject:mu_dic];
            break;
        }
    }
    [self SaveCurrentVideo];
}

-(void)failAPIRequest:(NSError*)error userInfo:(NSDictionary*)info
{
    NSLog(@"fail api request");
}

- (void) OpenUrl:(NSString*)_url
{
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:_url]];
}

// -----------------------------------  local data --------------------------------------------
- (void) AddNewVideo:(struct ConfigData)data SaveType:(int)_type
{
    NSMutableDictionary* new_video = [self StructToDictionary:data];
    if (_type == en_local_type_current)
    {
        while (self.current_video.count > k_local_max)
        {
            [self RemoveCurrentLastVideo];
        }
        [self.current_video insertObject:new_video atIndex:0];
        [self SaveCurrentVideo];
    }
    else if (_type == en_local_type_previous)
    {
        NSString* vid = [NSString stringWithUTF8String:data.vid];
        [new_video setObject:[NSString stringWithFormat:@"%d", en_upload_state_ok] forKey:@"state"];
        [self.previous_video setObject:new_video forKey:vid];
        [self SavePreviousVideo];
        [self RemoveCurrentVideoWithTime0:[NSString stringWithFormat:@"%lu", data.time0]];
    }
}

- (void) RemoveCurrentLastVideo
{
    NSString* video_name = [[self.current_video lastObject] objectForKey:@"time0"];
    NSString* path = [NSHomeDirectory() stringByAppendingFormat:@"/Documents/%@/%@.mov", self.game_uid, video_name];
    [[NSFileManager defaultManager] removeItemAtPath:path error:nil];
    [self.current_video removeLastObject];
    [self SaveCurrentVideo];
}

- (void) RemoveCurrentVideoWithTime0:(NSString*)_time0
{
    for (NSDictionary* each in self.current_video)
    {
        if ([[each objectForKey:@"time0"] isEqualToString:_time0])
        {
            NSString* path = [NSHomeDirectory() stringByAppendingFormat:@"/Documents/%@/%@.mov", self.game_uid, _time0];
            [[NSFileManager defaultManager] removeItemAtPath:path error:nil];
            [self.current_video removeObject:each];
            [self SaveCurrentVideo];
            break;
        }
    }
}

- (void) AddPreviousVideoData:(struct ConfigData)data
{
    NSDictionary* dic = [self StructToDictionary:data];
    [self AddPreviousVideo:dic];
}

- (void) AddPreviousVideo:(NSDictionary*)_dic 
{
    [self.previous_video setObject:_dic forKey:[_dic objectForKey:@"vid"]];
}

- (void) RemovePreviousVideoBehindIndex:(int)_index
{
    NSArray* keys = [self.previous_video allKeys];
    NSMutableArray* sort_keys = [NSMutableArray arrayWithArray:[self SortArray:keys]];
    for (int i=0; i<_index; i++)
    {
        [sort_keys removeObjectAtIndex:0];
    }
    [self.previous_video removeObjectsForKeys:sort_keys];
    [self SavePreviousVideo]; 
}

- (NSMutableDictionary*) StructToDictionary:(struct ConfigData)data
{
    NSMutableDictionary* rt = [NSMutableDictionary dictionary];
    [rt setObject:[NSString stringWithFormat:@"%hd", data.max_combo] forKey:@"max_combo"];
    [rt setObject:[NSString stringWithFormat:@"%f", data.average_combo] forKey:@"average_combo"];
    [rt setObject:[NSString stringWithFormat:@"%lu", data.max_damage] forKey:@"max_damage"];
    [rt setObject:[NSString stringWithFormat:@"%hd", data.total_rounds] forKey:@"total_rounds"];
    [rt setObject:[NSString stringWithFormat:@"%hd", data.group_general] forKey:@"group_general"];
    [rt setObject:[NSString stringWithFormat:@"%lu", data.group_attack] forKey:@"group_attack"];
    [rt setObject:[NSString stringWithFormat:@"%lu", data.group_hp] forKey:@"group_hp"];
    [rt setObject:[NSString stringWithFormat:@"%hd", data.skill_rounds] forKey:@"skill_rounds"];
    [rt setObject:[NSString stringWithUTF8String: data.fight_type] forKey:@"dungeon_type"];
    [rt setObject:[NSString stringWithUTF8String: data.fight_id] forKey:@"dungeon_id"];
    [rt setObject:[NSString stringWithUTF8String: data.fight_name] forKey:@"dungeon_name"];
    [rt setObject:[NSString stringWithFormat:@"%hd", data.room_id] forKey:@"room_id"];
    [rt setObject:[NSString stringWithFormat:@"%hd", data.floor_id] forKey:@"floor_id"];
    [rt setObject:[NSString stringWithUTF8String: data.vid] forKey:@"vid"];
    [rt setObject:[NSString stringWithUTF8String: data.video_url] forKey:@"video_url"];
    [rt setObject:[NSString stringWithUTF8String: data.img_url] forKey:@"img_url"];
    [rt setObject:[NSString stringWithFormat:@"%lu", data.time0] forKey:@"time0"];
    [rt setObject:[NSString stringWithFormat:@"%hd", data.upload_state] forKey:@"state"];
    
    return rt;
}

- (struct ConfigData) DictionaryToStruct:(NSDictionary*)_dic
{
    struct ConfigData rt;
    rt.max_combo = [[_dic objectForKey:@"max_combo"] intValue];
    rt.average_combo = [[_dic objectForKey:@"average_combo"] floatValue];
    rt.max_damage = [[_dic objectForKey:@"max_damage"] longLongValue];
    rt.total_rounds = [[_dic objectForKey:@"total_rounds"] intValue];
    rt.group_general = [[_dic objectForKey:@"group_general"] intValue];
    rt.group_attack = [[_dic objectForKey:@"group_attack"] longLongValue];
    rt.group_hp = [[_dic objectForKey:@"group_hp"] longLongValue];
    rt.skill_rounds = [[_dic objectForKey:@"skill_rounds"] intValue];
    snprintf(rt.fight_type, 16, "%s", [[_dic objectForKey:@"dungeon_type"] UTF8String]);
    snprintf(rt.fight_id, 16, "%s", [[_dic objectForKey:@"dungeon_id"] UTF8String]);
    snprintf(rt.fight_name, 32, "%s", [[_dic objectForKey:@"dungeon_name"] UTF8String]);
    rt.room_id = [[_dic objectForKey:@"room_id"] intValue];
    rt.floor_id = [[_dic objectForKey:@"floor_id"] intValue];
    snprintf(rt.vid, 16, "%s", [[_dic objectForKey:@"vid"] UTF8String]);
    snprintf(rt.video_url, 64, "%s", [[_dic objectForKey:@"video_url"] UTF8String]);
    snprintf(rt.img_url, 128, "%s", [[_dic objectForKey:@"img_url"] UTF8String]);
    rt.time0 = [[_dic objectForKey:@"time0"] longLongValue];
    rt.upload_state = [[_dic objectForKey:@"state"] intValue];

    return rt;
}

- (void) SaveCurrentVideo
{
    [self SaveLocal:self.path_current_video Object:self.current_video];
}

- (void) SavePreviousVideo
{
    [self SaveLocal:self.path_previous_video Object:self.previous_video];
}

- (void) SaveLocal:(NSString*)_path Object:(id)_dic
{
    NSData* data = [_dic JSONData];
    bool save = [[NSFileManager defaultManager] createFileAtPath:_path contents:data attributes:nil];
    if (!save)
    {
        NSAssert(0, @"save failed");
    }
}

- (NSArray*) GetLocalCurrentVideo
{
    NSArray* rt = (NSArray*)[self GetLocalData:self.path_current_video];
    return rt;
}

- (NSDictionary*) GetLocalPreviousVideo
{
    NSDictionary* rt = (NSDictionary*) [self GetLocalData:self.path_previous_video];
    return rt;
}

- (id) GetLocalData:(NSString*)_path
{
    NSData* data = [[NSFileManager defaultManager] contentsAtPath:_path];
    id rt = [data objectFromJSONData];
    return rt;
}

- (BOOL) CheckRecordExist
{
    if (self.previous_video.count > 0)
        return YES;
    if (self.current_video.count > 0)
        return YES;
    return NO;
}

- (struct ConfigData*) GetVideoAtIndex:(int)_index Type:(int)_type
{
    struct ConfigData* rt = NULL;
    if (_type == en_local_type_current)
    {
        if (_index < [self.current_video count])
        {
            NSDictionary* dic = [self.current_video objectAtIndex:_index];
            struct ConfigData data = [self DictionaryToStruct:dic];
            rt = &data;
        }
    }
    else if (_type == en_local_type_previous)
    {
        NSArray* arr = [self.previous_video allKeys];
        if (_index <[arr count])
        {
            NSArray* sort_arr = [self SortArray:arr];       // 貌似不排序也可以
            NSString* key = [sort_arr objectAtIndex:_index];
            NSDictionary* dic = [self.previous_video objectForKey:key];
            struct ConfigData data = [self DictionaryToStruct:dic];
            rt = &data;
        }
    }
    return rt;
}

- (NSArray*) SortArray:(NSArray*)_arr
{
    NSArray* rt = [_arr sortedArrayUsingComparator:^NSComparisonResult(NSString* obj1, NSString* obj2)
    {
        if ([obj2 compare:obj1] == 1)
            return (NSComparisonResult)NSOrderedDescending;
        else
            return (NSComparisonResult)NSOrderedAscending;
    }];
    return rt;
}

- (void) SetCurrentVideoState:(int)_state Time0:(NSString*)_time0
{
    if (_state == en_upload_state_ok || _state == en_upload_state_unuploaded)
    {
        NSAssert(0, @"it can't be here");
    }
    else
    {
        for (NSDictionary* each in self.current_video)
        {
            if ([[each objectForKey:@"time0"] isEqualToString:_time0])
            {
                int index = [self.current_video indexOfObject:each];
                NSMutableDictionary* mu_dic = [NSMutableDictionary dictionaryWithDictionary:each];
                [mu_dic setObject:[NSString stringWithFormat:@"%d", _state] forKey:@"state"];
                [self.current_video replaceObjectAtIndex:index withObject:mu_dic];
                
                break;
            }
        }
        
        if (_state == en_upload_state_reviewing || _state == en_upload_state_reviewed)
        {
            // 保存本地
            [self SaveCurrentVideo];
        }
        else if (_state == en_upload_state_upload_fail || _state == en_upload_state_uploading)
        {
            // 只需要存在 inst 里
        }
    }
}

- (void) SetUploading:(NSString*)_time0 Uploading:(BOOL)_uploading
{
    if (_uploading)
    {
        [self SetCurrentVideoState:en_upload_state_uploading Time0:_time0];
        [self UploadVideoWithName:_time0];
    }
    else
    {
        [self.uploadEngine close];
        [self SetCurrentVideoState:en_upload_state_upload_fail Time0:_time0];
    }
}

- (void) SetUploadSuccess:(NSString*)_time0
{
    [self SetCurrentVideoState:en_upload_state_reviewing Time0:_time0];
}

- (int) GetUploadState:(NSString*)_time0
{
    int rt = 1;
    for (NSDictionary* each in self.current_video)
    {
        if ([[each objectForKey:@"time0"] isEqualToString:_time0])
        {
            rt = [[each objectForKey:@"state"] intValue];
            break;
        }
    }
    return rt;
}

- (BOOL) CheckUploading
{
    BOOL rt = NO;
    for (NSDictionary* each in self.current_video)
    {
        if ([[each objectForKey:@"state"] intValue] == en_upload_state_uploading)
        {
            rt = YES;
            if (m_progress >=100 || m_progress < 0)
                rt = NO;
            break;
        }
    }
    return rt;
}

- (BOOL) CheckUploadedUpto10
{
    BOOL rt = 0;
    int count = self.current_video.count;
    if (count>k_local_max)
    {
        rt = YES;
    }
    else
    {
        rt = NO;
    }
    return rt;
        
}


@end
/////////////////////////////////////////////////////////////////////////////////////////

