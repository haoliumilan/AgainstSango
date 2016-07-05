//
//  THCapture.h
//  ScreenCaptureViewTest
//
//  Created by wayne li on 11-8-24.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>


@interface THCapture : NSObject{
    //video writing
	AVAssetWriter *videoWriter;
	AVAssetWriterInput *videoWriterInput;
	AVAssetWriterInputPixelBufferAdaptor *avAdaptor;
    
    //recording state
    BOOL            _isPause;                    //暂停录制
	BOOL            _recording;                  //正在录制中
    BOOL            _writing;                    //正在将帧写入文件
    NSTimer         *timer;                      //按帧率写屏的定时器
    NSUInteger      _frameRate;                  //帧速
    
    dispatch_queue_t    _videoQueue;
    
    CGSize              _recordSize;
    int                 _kbps;
    
  //  int                 iFrameNum;
}

//开始录制
- (bool)startRecording;
//结束录制
- (void)stopRecording;

// 取消录制
- (void)cancelRecord;

//录制每一帧
- (void)getPixels;
- (void)drawFrame:(CMTime)frameTime;
- (void)pauseRecord;
- (void)resumeRecord;


+(THCapture*)shareInstance;

@end



