//
//  THCapture.m
//  ScreenCaptureViewTest
//
//  Created by wayne li on 11-8-24.
//  Copyright 2011年 __MyCompanyName__. All rights reserved.
//

#import "THCapture.h"
#include <sys/time.h>
#import "JSONKit.h"
#import "ScreenRecordHelper.h"

static CGFloat DegreesToRadians(CGFloat degrees) {return degrees * M_PI / 180;};


@interface THCapture()
//配置录制环境
- (void)setUpWriter;
//清理录制环境
- (void)cleanupWriter;

@end

static THCapture *m_THCapture = NULL;

@implementation THCapture


+(THCapture*)shareInstance
{
    if (m_THCapture == NULL)
    {
        m_THCapture = [[THCapture alloc] init];
    }
    return m_THCapture;
}

- (id)init
{
    self = [super init];
    if (self)
    {
        CGRect screenRect = [UIScreen mainScreen].bounds;
        float scale = [UIScreen mainScreen].scale;
        _recordSize=  CGSizeMake(screenRect.size.width*scale,screenRect.size.height*scale);
        _isPause = NO;
        _kbps = 2000;
        _frameRate=15;
        _videoQueue = NULL;
    }
    
    return self;
}

- (void)dealloc
{
	[self cleanupWriter];
	[super dealloc];
}

- (bool)startRecording
{
    if (_videoQueue != NULL)
    {
        dispatch_release(_videoQueue);
        _videoQueue = NULL;
    }
    _videoQueue = dispatch_queue_create("video_queue", DISPATCH_QUEUE_SERIAL);
    // Remove the old video
    [[NSFileManager defaultManager] removeItemAtPath:[ScreenRecordHelper ShareHelper].path_recording error:nil];
    
    bool result = NO;
    if (! _recording)
    {
        [self setUpWriter];
        
        _recording = YES;
        _writing = NO;
        
        //绘屏的定时器
        NSDate *nowDate = [NSDate date];
        timer = [[NSTimer alloc] initWithFireDate:nowDate interval:1.0/_frameRate target:self selector:@selector(getPixels) userInfo:nil repeats:YES];
        [[NSRunLoop currentRunLoop] addTimer:timer forMode:NSRunLoopCommonModes];
        [timer release];
        
        dispatch_async(_videoQueue, ^{

            int msBeforeNextCapture = 1000 / _frameRate;
            
            struct timeval lastCapture, currentTime, startTime;
            lastCapture.tv_sec = 0;
            lastCapture.tv_usec = 0;
            
            //recording start time
            gettimeofday(&startTime, NULL);
            startTime.tv_usec /= 1000;
            
            int iFrameNum = 0;
            
            while (_recording)
            {
                
                //time passed since last capture
                gettimeofday(&currentTime, NULL);
                
                //convert to milliseconds to avoid overflows
                currentTime.tv_usec /= 1000;
                
                unsigned long long diff = (currentTime.tv_usec + (1000 * currentTime.tv_sec) ) - (lastCapture.tv_usec + (1000 * lastCapture.tv_sec) );
                
                // if enough time has passed, capture another shot
                if(diff >= msBeforeNextCapture)
                {
                    if (_isPause)
                    {
                        iFrameNum ++;
                        lastCapture = currentTime;
                        continue;
                    }
                    //time since start
                    long int msSinceStart = (currentTime.tv_usec + (1000 * currentTime.tv_sec) ) - (startTime.tv_usec + (1000 * startTime.tv_sec) );
                    
                    // Generate the frame number
                    int frameNumber = msSinceStart / msBeforeNextCapture;
                  //  NSLog(@"%d = %d", iFrameNum, frameNumber);
                    
                    // Capture next shot and repeat
                    [self drawFrame:CMTimeMake(frameNumber - iFrameNum, _frameRate)];
                    lastCapture = currentTime;
                }
            }
        });
    }
	return result;
}


- (void)stopRecording
{
    if (_recording)
    {
        _recording = NO;
        [timer invalidate];
        timer = nil;
       // [self SaveFightData];
        
    dispatch_async(_videoQueue, ^{
        [self cleanupWriter];
        
        dispatch_release(_videoQueue);
        _videoQueue = NULL;
        });
    }
}

- (void)cancelRecord
{
    if (_recording)
    {
        _recording = NO;
        
        if (buffer != NULL)
        {
            free(buffer);
            buffer = NULL;
        }
        
        if (buffer2 != NULL)
        {
            free(buffer2);
            buffer2 = NULL;
        }
        
        [timer invalidate];
        timer = nil;
        
    //    dispatch_async(_videoQueue, ^{
   //         [self cleanupWriter];
            
        [[NSFileManager defaultManager] removeItemAtPath:[ScreenRecordHelper ShareHelper].path_recording error:nil];
  //      });
        
        [avAdaptor release];
        avAdaptor = nil;
        
        [videoWriterInput release];
        videoWriterInput = nil;
        
        [videoWriter release];
        videoWriter = nil;
        
        dispatch_release(_videoQueue);
        _videoQueue = NULL;
    }
}

- (void)pauseRecord
{
    _isPause = YES;
}

- (void)resumeRecord
{
    _isPause = NO;
}

static GLubyte *buffer = NULL;
static GLubyte *buffer2 = NULL;

- (void)getPixels
{
    if (_isPause)
        return;
    
    if (!_writing)
    {
        NSInteger myDataLength = _recordSize.width * _recordSize.height * 4;
        
        if (buffer==NULL)
        {
            buffer = (GLubyte *) malloc(myDataLength);
        }
        
        if (buffer2==NULL)
        {
            buffer2 = (GLubyte *) malloc(myDataLength);
        }
        

        glReadPixels(0,0,_recordSize.width,_recordSize.height,GL_BGRA,GL_UNSIGNED_BYTE, buffer);

//        iFrameNum++;
//        
//        [self drawFrame:CMTimeMake(iFrameNum, _frameRate)];
        
    }
}

- (void)drawFrame:(CMTime)frameTime;
{
    if (buffer == NULL)
        return;
    
    if (_recording)
    {
        if (![videoWriterInput isReadyForMoreMediaData])
        {
            NSLog(@"Not ready for video data");
        }
        else
        {
            int _width = (int) _recordSize.width;
            int _height = (int)_recordSize.height;
            
            for (int i = 0; i < _height; ++i)
            {
                memcpy(&buffer2[i * _width * 4],
                       &buffer[(0 + _height - i - 1) * _width * 4 + 0 * 4],
                       _width * 4);
            }
            
            CVPixelBufferRef pixelBuffer =  NULL;
            
            CVPixelBufferCreateWithBytes (kCFAllocatorDefault, _recordSize.width, _recordSize.height, kCVPixelFormatType_32BGRA, buffer2, 4 * _recordSize.width, NULL, 0, NULL, &pixelBuffer);
            
            BOOL success = [avAdaptor appendPixelBuffer:pixelBuffer withPresentationTime:frameTime];
            if (!success)
                NSLog(@"Warning:  Unable to write buffer to video");
            
        }
    }
}

-(void) setUpWriter
{
	NSError  *error = nil;
    
    NSAssert(([ScreenRecordHelper ShareHelper].path_recording != nil) , @"A valid videoOutPath must be set before the recording starts!");
    
    //Configure videoWriter
	videoWriter = [[AVAssetWriter alloc] initWithURL:[NSURL fileURLWithPath:[ScreenRecordHelper ShareHelper].path_recording] fileType:AVFileTypeMPEG4 error:&error];
	NSParameterAssert(videoWriter);
	
	//Configure videoWriterInput
    NSMutableDictionary * compressionProperties = [NSMutableDictionary dictionary];
    [compressionProperties setObject: [NSNumber numberWithInt: _kbps * 1000] forKey: AVVideoAverageBitRateKey];
    [compressionProperties setObject: [NSNumber numberWithInt: _frameRate] forKey: AVVideoMaxKeyFrameIntervalKey];
   // [compressionProperties setObject: AVVideoProfileLevelH264Main41 forKey: AVVideoProfileLevelKey];
    
    CGSize size = _recordSize;
	
	NSDictionary* videoSettings = [NSDictionary dictionaryWithObjectsAndKeys:
								   AVVideoCodecH264,                      AVVideoCodecKey,
								   [NSNumber numberWithInt:size.width],   AVVideoWidthKey,
								   [NSNumber numberWithInt:size.height],  AVVideoHeightKey,
								   compressionProperties,                 AVVideoCompressionPropertiesKey,
               //                    AVVideoScalingModeResizeAspectFill,    AVVideoScalingModeKey,
								   nil];
    
    videoWriterInput = [[AVAssetWriterInput alloc] initWithMediaType:AVMediaTypeVideo outputSettings:videoSettings];
	
	NSParameterAssert(videoWriterInput);
	videoWriterInput.expectsMediaDataInRealTime = NO;
	NSDictionary* bufferAttributes =  [NSDictionary dictionaryWithObjectsAndKeys: [NSNumber numberWithInt:kCVPixelFormatType_32BGRA], kCVPixelBufferPixelFormatTypeKey,
                                       [NSNumber numberWithInt:size.width], kCVPixelBufferWidthKey,
                                       [NSNumber numberWithInt:size.height], kCVPixelBufferHeightKey,
                                       nil];
	
	avAdaptor = [[AVAssetWriterInputPixelBufferAdaptor assetWriterInputPixelBufferAdaptorWithAssetWriterInput:videoWriterInput sourcePixelBufferAttributes:bufferAttributes] retain];
    
    CGFloat rotationDegrees;
	switch ([[UIDevice currentDevice] orientation]) {
		case UIDeviceOrientationPortraitUpsideDown:
			rotationDegrees = -90.;
			break;
		case UIDeviceOrientationLandscapeLeft: // no rotation
			rotationDegrees = 0.;
			break;
		case UIDeviceOrientationLandscapeRight:
			rotationDegrees = 180.;
			break;
		case UIDeviceOrientationPortrait:
		case UIDeviceOrientationUnknown:
		case UIDeviceOrientationFaceUp:
		case UIDeviceOrientationFaceDown:
		default:
			rotationDegrees = 90.;
			break;
	}
	
    videoWriterInput.mediaTimeScale = _frameRate;
    videoWriter.movieTimeScale = _frameRate;
 
	//add input
	[videoWriter addInput:videoWriterInput];
	[videoWriter startWriting];
	[videoWriter startSessionAtSourceTime:kCMTimeZero];
}

- (void) cleanupWriter {
    
    if (buffer != NULL)
    {
        free(buffer);
        buffer = NULL;
    }
    
    if (buffer2 != NULL)
    {
        free(buffer2);
        buffer2 = NULL;
    }
    
    [videoWriterInput markAsFinished];
    [videoWriter finishWriting];
    
	[avAdaptor release];
	avAdaptor = nil;
	
	[videoWriterInput release];
	videoWriterInput = nil;
	
	[videoWriter release];
	videoWriter = nil;
}



@end
