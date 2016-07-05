//
//  ScreenRecordHelper.h
//  AgainstWar
//
//  Created by caojiye on 13-7-2.
//  Copyright (c) 2013年 OneClick Co.,Ltd. All rights reserved.
//

#define STRING_VAL(_val)	([_val isKindOfClass: [NSString class]] == YES ? ((NSString*)_val) : [(NSNumber*)_val stringValue])

#import <Foundation/Foundation.h>
#import "APIEngine.h"
#import "UploadEngine.h"
#import "RecordData.h"
////////////////////////////////////////////////////////////////
@interface ScreenRecordHelper : NSObject

@property (nonatomic, retain) UploadEngine*         uploadEngine;
@property (nonatomic, retain) APIEngine*            apiengine;
@property (nonatomic, strong) NSMutableArray*       current_video;
@property (nonatomic, strong) NSMutableDictionary*  previous_video;
@property (nonatomic, strong) NSString*             game_uid;
@property (nonatomic, strong) NSString*             path_current_video;
@property (nonatomic, strong) NSString*             path_previous_video;
@property (nonatomic, strong) NSString*             path_recording;



+ (ScreenRecordHelper*) ShareHelper;
+ (ScreenRecordHelper*) ShareWithUid:(NSString*) _uid;


- (float) GetUploadProgress;
- (void) SaveToAlbum;

- (void) SetPathRecording:(NSString*)_path;

- (BOOL) CheckRecordExist;

- (struct ConfigData*) GetVideoAtIndex:(int)_index Type:(int)_type;

- (void) SetUploading:(NSString*)_time0 Uploading:(BOOL)_uploading;
- (void) SetUploadSuccess:(NSString*)_time0;
- (int) GetUploadState:(NSString*)_time0;
- (BOOL) CheckUploading;

- (BOOL) CheckUploadedUpto10;


// 服务器
- (void) RequestVideoInfo;


// 查看他人视频
- (void) OpenUrl:(NSString*)_url;

// local data
- (void) AddNewVideo:(struct ConfigData)data SaveType:(int)_type;
- (void) RemovePreviousVideoBehindIndex:(int)_index;


@end
