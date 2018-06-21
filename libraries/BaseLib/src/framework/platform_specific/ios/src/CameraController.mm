#import <CoreVideo/CVOpenGLESTextureCache.h>
#import <OpenGLES/ES2/glext.h>
#import "../include/CameraController.hh"


#include <aex/Logger.hpp>

static const int RANGE = 100;
// Values for antibanding settings.
static const int ANTIBANDING_DEFAULT = (RANGE * 1);
static const int ANTIBANDING_AUTO = (RANGE * 1) + 1;
//static const int ANTIBANDING_50HZ = (RANGE * 1) + 2;
//static const int ANTIBANDING_60HZ = (RANGE * 1) + 3;
//static const int ANTIBANDING_OFF = (RANGE * 1) + 4;

// Values for white balance settings.
static const int WHITE_BALANCE_DEFAULT = (RANGE * 2);
static const int WHITE_BALANCE_AUTO = (RANGE * 2) + 1;
//static const int WHITE_BALANCE_INCANDESCENT = (RANGE * 2) + 2;
//static const int WHITE_BALANCE_FLUORESCENT = (RANGE * 2) + 3;
//static const int WHITE_BALANCE_WARM_FLUORESCENT = (RANGE * 2) + 4;
//static const int WHITE_BALANCE_DAYLIGHT = (RANGE * 2) + 5;
//static const int WHITE_BALANCE_CLOUDY_DAYLIGHT = (RANGE * 2) + 6;
//static const int WHITE_BALANCE_TWILIGHT = (RANGE * 2) + 7;
//static const int WHITE_BALANCE_SHADE = (RANGE * 2) + 8;
static const int WHITE_BALANCE_LOCKED = (RANGE * 2) + 9;
static const int WHITE_BALANCE_CONTINUOUS_AUTO = (RANGE * 2) + 10;

// Values for color effect settings.
static const int COLOR_EFFECT_DEFAULT = (RANGE * 3);
static const int COLOR_EFFECT_NONE = (RANGE * 3) + 1;
//static const int COLOR_EFFECT_MONO = (RANGE * 3) + 2;
//static const int COLOR_EFFECT_NEGATIVE = (RANGE * 3) + 3;
//static const int COLOR_EFFECT_SOLARIZE = (RANGE * 3) + 4;
//static const int COLOR_EFFECT_SEPIA = (RANGE * 3) + 5;
//static const int COLOR_EFFECT_POSTERIZE = (RANGE * 3) + 6;
//static const int COLOR_EFFECT_WHITEBOARD = (RANGE * 3) + 7;
//static const int COLOR_EFFECT_BLACKBOARD = (RANGE * 3) + 8;
//static const int COLOR_EFFECT_AQUA = (RANGE * 3) + 9;

// Values flash mode
static const int FLASH_MODE_DEFAULT = (RANGE * 4);
static const int FLASH_MODE_OFF = (RANGE * 4) + 1;
static const int FLASH_MODE_ON = (RANGE * 4) + 2;
static const int FLASH_MODE_AUTO = (RANGE * 4) + 3;
//static const int FLASH_MODE_RED_EYE = (RANGE * 4) + 4;

// Keys scene mode
static const int SCENE_MODE_DEFAULT = (RANGE * 5);
static const int SCENE_MODE_AUTO = (RANGE * 5) + 1;
//static const int SCENE_MODE_ACTION = (RANGE * 5) + 2;
//static const int SCENE_MODE_PORTRAIT = (RANGE * 5) + 3;
//static const int SCENE_MODE_LANDSCAPE = (RANGE * 5) + 4;
//static const int SCENE_MODE_NIGHT = (RANGE * 5) + 5;
//static const int SCENE_MODE_NIGHT_PORTRAIT = (RANGE * 5) + 6;
//static const int SCENE_MODE_THEATRE = (RANGE * 5) + 7;
//static const int SCENE_MODE_BEACH = (RANGE * 5) + 8;
//static const int SCENE_MODE_SNOW = (RANGE * 5) + 9;
//static const int SCENE_MODE_SUNSET = (RANGE * 5) + 10;
//static const int SCENE_MODE_STEADYPHOTO = (RANGE * 5) + 11;
//static const int SCENE_MODE_FIREWORKS = (RANGE * 5) + 12;
//static const int SCENE_MODE_SPORTS = (RANGE * 5) + 13;
//static const int SCENE_MODE_PARTY = (RANGE * 5) + 14;
//static const int SCENE_MODE_CANDLELIGHT = (RANGE * 5) + 15;
//static const int SCENE_MODE_BARCODE = (RANGE * 5) + 16;
//static const int SCENE_MODE_HDR = (RANGE * 5) + 17;

// Focus values
static const int FOCUS_MODE_DEFAULT = (RANGE * 6);
static const int FOCUS_MODE_AUTO = (RANGE * 6) + 1;
static const int FOCUS_MODE_INFINITY = (RANGE * 6) + 2;
static const int FOCUS_MODE_MACRO = (RANGE * 6) + 3;
static const int FOCUS_MODE_FIXED = (RANGE * 6) + 4;
static const int FOCUS_MODE_EDOF = (RANGE * 6) + 5;
static const int FOCUS_MODE_CONTINUOUS_VIDEO = (RANGE * 6) + 6;
static const int FOCUS_MODE_CONTINUOUS_PICTURE = (RANGE * 6) + 7;

// Torch values
static const int TORCH_MODE_DEFAULT = (RANGE * 7);
static const int TORCH_MODE_OFF = (RANGE * 7) + 1;
static const int TORCH_MODE_ON = (RANGE * 7) + 2;
static const int TORCH_MODE_AUTO = (RANGE * 7) + 3;

// Exposure values
static const int EXPOSURE_MODE_DEFAULT = (RANGE * 8);
static const int EXPOSURE_MODE_AUTO = (RANGE * 8) + 1;
static const int EXPOSURE_MODE_FIXED = (RANGE * 8) + 2;
static const int EXPOSURE_MODE_CONTINUOUS_AUTO_EXPOSURE = (RANGE * 8) + 3;
static const int EXPOSURE_MODE_CUSTOM = (RANGE * 8) + 4;


#define SYSTEM_VERSION_EQUAL_TO(v)                  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedSame)
#define SYSTEM_VERSION_GREATER_THAN(v)              ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedDescending)
#define SYSTEM_VERSION_GREATER_THAN_OR_EQUAL_TO(v)  ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN(v)                 ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] == NSOrderedAscending)
#define SYSTEM_VERSION_LESS_THAN_OR_EQUAL_TO(v)     ([[[UIDevice currentDevice] systemVersion] compare:v options:NSNumericSearch] != NSOrderedDescending)

inline bool isType(int param, int first)
{
    first = ((int)first / (int)RANGE) * RANGE;
    //LOG("Param base type: ", first);
    return ((param >= first) && (param < (first + RANGE))) ? true : false;
}

@implementation CameraController

- (int) isParamSupported: (int) param
{
    // antibanding on ios cant be seted, is auto
    if(isType(param, ANTIBANDING_DEFAULT))
    {
        switch(param)
        {
            case ANTIBANDING_DEFAULT:
                return [self codeSuccess];

            case ANTIBANDING_AUTO:
                return [self codeSuccess];

            default:
                return [self codeFailure];
        }
    }

    if(isType(param, COLOR_EFFECT_DEFAULT))
    {
        switch(param)
        {
            case COLOR_EFFECT_DEFAULT:
                return [self codeSuccess];

            case COLOR_EFFECT_NONE:
                return [self codeSuccess];

            default:
                return [self codeFailure];
        }
    }

    if(isType(param, WHITE_BALANCE_DEFAULT))
    {
        switch(param)
        {
            case WHITE_BALANCE_DEFAULT:
                return [self codeSuccess];

            case WHITE_BALANCE_AUTO:
                if([videoDevice isWhiteBalanceModeSupported:AVCaptureWhiteBalanceModeAutoWhiteBalance])
                {
                    return [self codeSuccess];
                }

                break;

            case WHITE_BALANCE_CONTINUOUS_AUTO:
                if([videoDevice isWhiteBalanceModeSupported:AVCaptureWhiteBalanceModeContinuousAutoWhiteBalance])
                {
                    return [self codeSuccess];
                }

                break;

            case WHITE_BALANCE_LOCKED:
                if([videoDevice isWhiteBalanceModeSupported:AVCaptureWhiteBalanceModeLocked])
                {
                    return [self codeSuccess];
                }

                break;

            default:
                return [self codeFailure];
        }
    }

    if(isType(param, FLASH_MODE_DEFAULT))
    {
        if((![videoDevice hasFlash]) && (param != FLASH_MODE_DEFAULT))
        {
            return [self codeFailure];
        }

        switch(param)
        {
            case FLASH_MODE_DEFAULT:
                return [self codeSuccess];

            case FLASH_MODE_OFF:
                if([videoDevice isFlashModeSupported:AVCaptureFlashModeOff])
                {
                    return [self codeSuccess];
                }

                break;

            case FLASH_MODE_ON:
                if([videoDevice isFlashModeSupported:AVCaptureFlashModeOn])
                {
                    return [self codeSuccess];
                }

                break;

            case FLASH_MODE_AUTO:
                if([videoDevice isFlashModeSupported:AVCaptureFlashModeAuto])
                {
                    return [self codeSuccess];
                }

                break;

            default:
                return [self codeFailure];
        }
    }

    if(isType(param, SCENE_MODE_DEFAULT))
    {
        switch(param)
        {
            case SCENE_MODE_DEFAULT:
                return [self codeSuccess];

            case SCENE_MODE_AUTO:
                return [self codeSuccess];

            default:
                return [self codeFailure];
        }
    }

    if(isType(param, FOCUS_MODE_DEFAULT))
    {
        switch(param)
        {
            case FOCUS_MODE_DEFAULT:
                return [self codeSuccess];

            case FOCUS_MODE_FIXED:
                if([videoDevice isFocusModeSupported:AVCaptureFocusModeLocked])
                {
                    return [self codeSuccess];
                }

                break;

            case FOCUS_MODE_AUTO:
                if([videoDevice isFocusModeSupported:AVCaptureFocusModeAutoFocus])
                {
                    return [self codeSuccess];
                }

                break;

            case FOCUS_MODE_CONTINUOUS_PICTURE:
            case FOCUS_MODE_CONTINUOUS_VIDEO:
            case FOCUS_MODE_EDOF:
                if([videoDevice isFocusModeSupported:AVCaptureFocusModeContinuousAutoFocus])
                {
                    return [self codeSuccess];
                }

                break;

            case FOCUS_MODE_MACRO:
            case FOCUS_MODE_INFINITY:
                if([videoDevice isAutoFocusRangeRestrictionSupported])
                {
                    return [self codeSuccess];
                }

                break;

            default:
                return [self codeFailure];
        }
    }

    if(isType(param, TORCH_MODE_DEFAULT))
    {
        if((![videoDevice hasTorch]) && (param != TORCH_MODE_DEFAULT))
        {
            return [self codeFailure];
        }

        switch(param)
        {
            case TORCH_MODE_DEFAULT:
                return [self codeSuccess];

            case TORCH_MODE_OFF:
                if([videoDevice isTorchModeSupported:AVCaptureTorchModeOff])
                {
                    return [self codeSuccess];
                }

                break;

            case TORCH_MODE_ON:
                if([videoDevice isTorchModeSupported:AVCaptureTorchModeOn])
                {
                    return [self codeSuccess];
                }

                break;

            case TORCH_MODE_AUTO:
                if([videoDevice isTorchModeSupported:AVCaptureTorchModeAuto])
                {
                    return [self codeSuccess];
                }

                break;

            default:
                return [self codeFailure];
        }
    }

    if(isType(param, EXPOSURE_MODE_DEFAULT))
    {
        switch(param)
        {
            case EXPOSURE_MODE_DEFAULT:
                return [self codeSuccess];

            case EXPOSURE_MODE_FIXED:
                if([videoDevice isExposureModeSupported:AVCaptureExposureModeLocked])
                {
                    return [self codeSuccess];
                }

                break;

            case EXPOSURE_MODE_AUTO:
                if([videoDevice isExposureModeSupported:AVCaptureExposureModeAutoExpose])
                {
                    return [self codeSuccess];
                }

                break;

            case EXPOSURE_MODE_CONTINUOUS_AUTO_EXPOSURE:
                if([videoDevice isExposureModeSupported:AVCaptureExposureModeContinuousAutoExposure])
                {
                    return [self codeSuccess];
                }

                break;

            case EXPOSURE_MODE_CUSTOM:
                if([videoDevice isExposureModeSupported:AVCaptureExposureModeCustom])
                {
                    return [self codeSuccess];
                }

                break;

            default:
                return [self codeFailure];
        }
    }

    return [self codeFailure];
}

- (int) setParam: (int) param
{
    if([self isParamSupported:param] == [self codeSuccess])
    {
        NSError* error = nil;

        if([videoDevice lockForConfiguration:&error])
        {

            if(isType(param, ANTIBANDING_DEFAULT))
            {
                // no antibanding param in iOS
            }

            if(isType(param, COLOR_EFFECT_DEFAULT))
            {
                // no color effects in iOS
            }

            if(isType(param, WHITE_BALANCE_DEFAULT))
            {
                switch(param)
                {
                    case WHITE_BALANCE_DEFAULT:
                        if([videoDevice isWhiteBalanceModeSupported:whiteBalanceDefault])
                        {
                            videoDevice.whiteBalanceMode = whiteBalanceDefault;
                        }

                        //LOG("SEETTING DEFAULT WHITE BALANCE");
                        break;

                    case WHITE_BALANCE_AUTO:
                        videoDevice.whiteBalanceMode = AVCaptureWhiteBalanceModeAutoWhiteBalance;
                        break;

                    case WHITE_BALANCE_LOCKED:
                        videoDevice.whiteBalanceMode = AVCaptureWhiteBalanceModeLocked;
                        break;

                    case WHITE_BALANCE_CONTINUOUS_AUTO:
                        videoDevice.whiteBalanceMode = AVCaptureWhiteBalanceModeContinuousAutoWhiteBalance;
                        break;
                }
            }

            if(isType(param, FLASH_MODE_DEFAULT))
            {
                switch(param)
                {
                    case FLASH_MODE_DEFAULT:
                        if([videoDevice isFlashAvailable])
                            if([videoDevice isFlashModeSupported:flashModeDefault])
                            {
                                videoDevice.flashMode = flashModeDefault;
                            }

                        //LOG("SEETTING DEFAULT FLASH BALANCE");
                        break;

                    case FLASH_MODE_ON:
                        videoDevice.flashMode = AVCaptureFlashModeOn;
                        break;

                    case FLASH_MODE_OFF:
                        videoDevice.flashMode = AVCaptureFlashModeOff;
                        break;

                    case FLASH_MODE_AUTO:
                        videoDevice.flashMode = AVCaptureFlashModeAuto;
                        break;
                }
            }

            if(isType(param, SCENE_MODE_DEFAULT))
            {
                // no scene modes on iOS (Android only)
            }

            if(isType(param, FOCUS_MODE_DEFAULT))
            {
                switch(param)
                {
                    case FOCUS_MODE_DEFAULT:
                        if([videoDevice isFocusModeSupported:focusModeDefault])
                        {
                            videoDevice.focusMode = focusModeDefault;

                            if([videoDevice isAutoFocusRangeRestrictionSupported])
                            {
                                [videoDevice setAutoFocusRangeRestriction:AVCaptureAutoFocusRangeRestrictionNone];
                            }

                        }

                        //LOG("SEETTING DEFAULT FOCS BALANCE");
                        break;

                    case FOCUS_MODE_FIXED:
                        videoDevice.focusMode = AVCaptureFocusModeLocked;
                        break;

                    case FOCUS_MODE_AUTO:
                        videoDevice.focusMode = AVCaptureFocusModeAutoFocus;
                        break;

                    case FOCUS_MODE_CONTINUOUS_PICTURE:
                    case FOCUS_MODE_CONTINUOUS_VIDEO:
                    case FOCUS_MODE_EDOF:
                        videoDevice.focusMode = AVCaptureFocusModeContinuousAutoFocus;
                        break;

                    case FOCUS_MODE_MACRO:
                        [videoDevice setAutoFocusRangeRestriction:AVCaptureAutoFocusRangeRestrictionNear];
                        break;

                    case FOCUS_MODE_INFINITY:
                        [videoDevice setAutoFocusRangeRestriction:AVCaptureAutoFocusRangeRestrictionFar];
                        break;
                }
            }

            if(isType(param, TORCH_MODE_DEFAULT))
            {
                switch(param)
                {
                    case TORCH_MODE_DEFAULT:
                        if([videoDevice isTorchAvailable])
                            if([videoDevice isTorchModeSupported:torchModeDefault])
                            {
                                videoDevice.torchMode = torchModeDefault;
                            }

                        //LOG("SEETTING DEFAULT TORCH BALANCE");
                        break;

                    case TORCH_MODE_OFF:
                        videoDevice.torchMode = AVCaptureTorchModeOff;
                        break;

                    case TORCH_MODE_ON:
                        videoDevice.torchMode = AVCaptureTorchModeOn;
                        break;

                    case TORCH_MODE_AUTO:
                        videoDevice.torchMode = AVCaptureTorchModeAuto;
                        break;
                }
            }

            if(isType(param, EXPOSURE_MODE_DEFAULT))
            {
                switch(param)
                {
                    case EXPOSURE_MODE_DEFAULT:
                        if([videoDevice isExposureModeSupported:exposureModeDefault])
                        {
                            videoDevice.exposureMode = exposureModeDefault;
                        }
                        else if([videoDevice isExposureModeSupported:AVCaptureExposureModeAutoExpose])
                        {
                            videoDevice.exposureMode = AVCaptureExposureModeAutoExpose;
                        }

                        break;

                    case EXPOSURE_MODE_FIXED:
                        videoDevice.exposureMode = AVCaptureExposureModeLocked;
                        break;

                    case EXPOSURE_MODE_AUTO:
                        videoDevice.exposureMode = AVCaptureExposureModeAutoExpose;
                        break;

                    case EXPOSURE_MODE_CONTINUOUS_AUTO_EXPOSURE:
                        videoDevice.exposureMode = AVCaptureExposureModeContinuousAutoExposure;
                        break;

                    case EXPOSURE_MODE_CUSTOM:
                        videoDevice.exposureMode = AVCaptureExposureModeCustom;
                        break;
                }
            }

            [videoDevice unlockForConfiguration];
            return [self codeSuccess];
        }
        else
        {
            return [self codeFailure];
        }
    }
    else
    {
        return [self codeFailure];
    }
}

- (int)viewDidLoad
{
    _context = [EAGLContext currentContext];

    if(!_context)
    {
        LOG("--(!): Failed to create ES context");
        return [self codeFailure];
    }

    _screenWidth = [UIScreen mainScreen].bounds.size.width;
    _screenHeight = [UIScreen mainScreen].bounds.size.height;
    bRecording = false;
    bPrepared = false;
    assetWriter = nil;
    return [self codeSuccess];
}

- (void)viewDidUnload
{
    if([EAGLContext currentContext] == _context)
    {
        [EAGLContext setCurrentContext:nil];
    }
}

- (void)cleanUpTextures
{
    while(!m_unneededCamFrames.empty())
    {
        LumaChromaTexIds camFrame = m_unneededCamFrames.front();
        CFRelease(camFrame.lumaTexture);
        CFRelease(camFrame.chromaTexture);
        m_unneededCamFrames.pop();
    }

    while(!m_camFrames.empty())
    {
        LumaChromaTexIds camFrame = m_camFrames.front();
        CFRelease(camFrame.lumaTexture);
        CFRelease(camFrame.chromaTexture);
        m_camFrames.pop();
    }

    // Periodic texture cache flush every frame
    CVOpenGLESTextureCacheFlush(_videoTextureCache, 0);
}


- (bool) isOpened: (int) camID
{
    if(_context == [EAGLContext currentContext])
    {
        return true;
    }

    return false;
}

- (void) setCamDevActiveTexUnit: (int) texUnit
{
    camDevActiveTextureUnit = texUnit;
}

- (id) init
{
    self = [super init];
    _session = nil;
    library = nil;
    input = nil;
    audioInput = nil;
    dataOutput = nil;
    audioOutput = nil;
    _pixels = nil;
    recordingQueue = nil;
    return self;
}

- (int)openCamera: (int) camID : (bool) bWithAudio
{
    m_bWithAudio = bWithAudio;
//    if([self isOpened:camID])
//    {
//        return [self codeFailure];    // is opened now, cant open again
//    }
    int code = [self viewDidLoad];

    if(code == [self codeFailure])
    {
        return [self codeFailure];
    }

    //-- Create CVOpenGLESTextureCacheRef for optimal CVImageBufferRef to GLES texture conversion.
#if COREVIDEO_USE_EAGLCONTEXT_CLASS_IN_API
    CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, _context, NULL, &_videoTextureCache);
#else
    CVReturn err = CVOpenGLESTextureCacheCreate(kCFAllocatorDefault, NULL, (__bridge void*)_context, NULL, &_videoTextureCache);
#endif

    if(err)
    {
        LOG("--(!): Error at CVOpenGLESTextureCacheCreate ");
        return [self codeFailure];
    }

    lumaChromaTexGetsCounter = 0;
    _textureWidth = 0;
    _textureHeight = 0;

    if(recordingQueue == nil)
        recordingQueue = dispatch_queue_create("Movie Recording Queue", DISPATCH_QUEUE_SERIAL);

    //-- Setup Capture Session.
    if(_session == nil)
        _session = [[AVCaptureSession alloc] init];

    [_session beginConfiguration];
    videoDevice = [self getCaptureDevice:camID];
    if(videoDevice == nil)
    {
        return [self codeFailure];
    }


    //-- Add the device to the session.
    NSError* error = nil;
    if(input == nil){
        input = [AVCaptureDeviceInput deviceInputWithDevice:videoDevice error:&error];
        LOG_DEBUG("Allocated input");
    }else{
        LOG_DEBUG("STAY WITH OLD INPUT");
    }
    if(error)
    {
        return [self codeFailurePermissions];
    }
    if(input == nil){
        return [self codeFailurePermissions];
    }
    [_session addInput:input];

    if(bWithAudio){
        // for recording video:
        audioDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeAudio];
        audioInput = [AVCaptureDeviceInput deviceInputWithDevice:audioDevice error:nil];
        if(audioInput == nil){
            return [self codeFailurePermissions];
        }
        [_session addInput:audioInput];
    }

    //-- Create the output for the capture session.

    if(dataOutput == nil){
        dataOutput = [[AVCaptureVideoDataOutput alloc] init];
        [dataOutput setAlwaysDiscardsLateVideoFrames:NO]; // Probably want to set this to NO when recording

        //-- Set to YUV420.
        [dataOutput setVideoSettings:[NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCVPixelFormatType_420YpCbCr8BiPlanarFullRange]
                                                             forKey:(id)kCVPixelBufferPixelFormatTypeKey]]; // Necessary for manual preview

        // Set dispatch to be on the main thread so OpenGL can do things with the data
        [dataOutput setSampleBufferDelegate:self queue:dispatch_get_main_queue()];
    }
        // for preview:f
        [_session addOutput:dataOutput];

        if(bWithAudio){
            // for videoVoice
            audioOutput = [[AVCaptureAudioDataOutput alloc] init];
            [audioOutput setSampleBufferDelegate:self queue:dispatch_get_main_queue()];
            [_session addOutput:audioOutput];
        }

    // for taking pictures:
    if(library == nil)
        library = [[ALAssetsLibrary alloc] init];
    [self prepareStillImageOutput];
    [_session addOutput:stillImageOutput];

    [_session commitConfiguration];

    // --- remember default videoDevice parameters
    whiteBalanceDefault = videoDevice.whiteBalanceMode;
    flashModeDefault = videoDevice.flashMode;
    focusModeDefault = videoDevice.focusMode;
    torchModeDefault = videoDevice.torchMode;
    exposureModeDefault = videoDevice.exposureMode;
    // ---

    checkingSmoothZoom = false;

    // remmeber connections;
    _videoConnection = [dataOutput connectionWithMediaType:AVMediaTypeVideo];
    if(bWithAudio){
        _audioConnection = [audioOutput connectionWithMediaType:AVMediaTypeAudio];
    }
    [_videoConnection setVideoOrientation:AVCaptureVideoOrientationLandscapeRight];

    _pixels = nil;

    return [self codeSuccess];
}

- (AVCaptureDevice*) getCaptureDevice: (int) id
{
  AVCaptureDevice *retDev = nil;
  NSArray* devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
  switch(id)
  {
    case 0:
    for(AVCaptureDevice * device in devices)
    {
      if([device position] == AVCaptureDevicePositionBack)
      {
        retDev = device;
        break;
      }
    }
    break;

    case 1:
    for(AVCaptureDevice * device in devices)
    {
      if([device position] == AVCaptureDevicePositionFront)
      {
        retDev = device;
        break;
      }
    }
    break;
    default:
      // videoDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo]; // should be back cam here wide angle
      for(AVCaptureDevice * device in devices)
      {
        if([device position] == AVCaptureDevicePositionBack)
        {
          retDev = device;
          break;
        }
      }
    break;
  }

  return retDev;

  // iOS 10+ possibility to dual camera usage
  // switch(camID)
  // {
  //   case 0:
  //   // TODO for dual cameras, return nil if no device
  //   // device = [AVCaptureDevice defaultDeviceWithDeviceType: AVCaptureDeviceTypeBuiltInDuoCamera
  //   //                                         mediaType: AVMediaTypeVideo
  //   //                                          position: AVCaptureDevicePositionBack];
  //     videoDevice = [AVCaptureDevice defaultDeviceWithDeviceType: AVCaptureDeviceTypeBuiltInWideAngleCamera
  //                                        mediaType: AVMediaTypeVideo
  //                                        position: AVCaptureDevicePositionBack];
  //   break;
  //   case 1:
  //     videoDevice = [AVCaptureDevice defaultDeviceWithDeviceType: AVCaptureDeviceTypeBuiltInWideAngleCamera
  //                                         mediaType: AVMediaTypeVideo
  //                                         position: AVCaptureDevicePositionFront];
  //     if(videoDevice != nil)
  //         break;
  //
  //   default:
  //     videoDevice = [AVCaptureDevice defaultDeviceWithDeviceType: AVCaptureDeviceTypeBuiltInWideAngleCamera
  //                                        mediaType: AVMediaTypeVideo
  //                                        position: AVCaptureDevicePositionBack];
  //     break;
  // }
}

- (void)tearDownAVCapture
{
    [self cleanUpTextures];
    CFRelease(_videoTextureCache);
}


- (void) startPreview
{
    @synchronized(self){
        if([_session isRunning] == NO)
        {
        @try
        {
            [_session startRunning];
        }
        @catch(NSException* e)
        {
            LOG_DEBUG("Exception while start running");
        }

        }
    }
}

- (void) stopPreview
{
    @synchronized(self){
        if([_session isRunning] == YES)
        {
            [_session stopRunning];
        }
    }
}

- (void) releaseCamera
{
    @synchronized(self){
        [_session stopRunning];

        NSArray* inputs = [_session inputs];
        for(AVCaptureInput* input in inputs)
        {
            [_session removeInput: input];
        }

        NSArray* outputs = [_session outputs];
        for(AVCaptureOutput* output in outputs)
        {
            [_session removeOutput: output];
        }

        @try
        {
            [videoDevice removeObserver:self forKeyPath:@"videoZoomFactor"];
        }
        @catch(NSException* e)
        {
        }

        @try
        {
            [videoDevice removeObserver:self forKeyPath:@"adjustingFocus"];
        }
        @catch(NSException* e)
        {
        }

        [self tearDownAVCapture];
    }
}

- (void) startVideo: (std::string) fullPath : (std::string) albumName : (bool) inAlbum : (int) width : (int) height;
{
    @synchronized(self){
        if(! bRecording){
            _videoSaveInAlbum = inAlbum;
            _videoFullPath = fullPath;
            _videoAlbumName = albumName;

            NSString* pathStr = @(fullPath.c_str());

            if( ! inAlbum){
                _URL = [[NSURL alloc] initFileURLWithPath:pathStr];
            }else{
                _URL = [[NSURL alloc] initFileURLWithPath:[NSString pathWithComponents:@[NSTemporaryDirectory(), pathStr]]];
            }

            [self prepareToRecord: width : height];
            bRecording = true;
        }
    }
}

- (void) stopVideo
{
    dispatch_async(recordingQueue, ^{
        @synchronized(self){
            if( bRecording )
            {
                [assetWriterVideoIn markAsFinished];
                if(m_bWithAudio)
                    [assetWriterAudioIn markAsFinished];
                [assetWriter finishWriting];
                [self cleanAfterRecording];

                if(_videoSaveInAlbum){
                    [self addVideoToAlbum]; // it use members names
                }else{
                    cbStopVideo(RESULT::SUCCESS);
                    // do nothing, is where it was saved
                }
                NSLog(@"video recording stopped");
                bRecording = NO;
            }
        }
    });
}

- (void) addVideoToAlbum
{
    // add video to album
    if(_videoAlbumName == ""){
        [library writeVideoAtPathToSavedPhotosAlbum:_URL completionBlock:^(NSURL* assetURL, NSError* error2){
            if(error2 != 0){
                cbStopVideo(RESULT::FAILURE);
            }else{
                cbStopVideo(RESULT::SUCCESS);
            }
        }];
    }else{
        // remove this line after repaired below HUGE block
        [library writeVideoAtPathToSavedPhotosAlbum:_URL completionBlock:^(NSURL* assetURL, NSError* error2){
            if(error2 != 0){
                cbStopVideo(RESULT::FAILURE);
            }else{
                cbStopVideo(RESULT::SUCCESS);
            }
        }];

        // TODO later, is not neccesary now
        //        // Code below was adding it to
        //        [library addAssetsGroupAlbumWithName: @(_videoAlbumName.c_str())
        //                                 resultBlock:^(ALAssetsGroup *group) {
        //                                     if(group != nil){
        //                                         LOG("Created NEW Group: ", [[group description] UTF8String]);
        //                                     }else{
        //                                         LOG("Group exists: ", _videoAlbumName);
        //                                     }
        //
        //                                     // add to album
        //                                     [library enumerateGroupsWithTypes:ALAssetsGroupAlbum
        //                                                            usingBlock:^(ALAssetsGroup *group2, BOOL *stop){
        //                                                                if ([[group2 valueForProperty:ALAssetsGroupPropertyName] isEqualToString: @(_videoAlbumName.c_str()) ])
        //                                                                {
        //                                                                    LOG("FOUNDED NEEDED ALBUM");
        //                                                                    [library assetForURL:_URL
        //                                                                             resultBlock:^(ALAsset *asset) {
        //                                                                                 [group2 addAsset:asset];
        //
        //                                                                                 LOG("ADDED TO ALBUM");
        //                                                                                 *stop = true;
        //                                                                             }
        //                                                                            failureBlock:^(NSError* error) {
        //
        //                                                                                LOG("NOT ADDED TO ALBUM");
        //                                                                                // not added to album
        //                                                                            }];
        //                                                                }else{
        //                                                                    //LOG("IT ISNT NEEDED ALBUM");
        //                                                                }
        //                                                            }
        //                                                          failureBlock:^(NSError* error3) {
        //                                                              LOG("ALBUM NOT FOUNDED FUCK");
        //                                                              return;
        //                                                          }];
        //
        //                                }
        //                                failureBlock:^(NSError *error) {

        //                                    return;
        //                                }];
    }
}

- (void) prepareToRecord : (int) width : (int) height
{
    // AVAssetWriter will not write over an existing file.
    [[NSFileManager defaultManager] removeItemAtURL:_URL error : NULL];
    NSError *error = nil;
    assetWriter = [[AVAssetWriter alloc] initWithURL:_URL fileType:AVFileTypeQuickTimeMovie error:&error];
    NSParameterAssert(assetWriter);
    // Add video input
    NSDictionary *videoCompressionProps =  [NSDictionary dictionaryWithObjectsAndKeys:
                                           [NSNumber numberWithDouble:128.0*1024.0], AVVideoAverageBitRateKey,
                                           nil ];
    //LOG_DEBUG("--!!!--: Preparing wideo with size: ", width, "-", height);

    // TODO find how to parametrize this width and height of video
    NSDictionary *videoSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                                   AVVideoCodecH264, AVVideoCodecKey,
                                   [NSNumber numberWithInt:720], AVVideoWidthKey,
                                   [NSNumber numberWithInt:480], AVVideoHeightKey,
                                   videoCompressionProps, AVVideoCompressionPropertiesKey,
                                   nil];

    assetWriterVideoIn = [[AVAssetWriterInput assetWriterInputWithMediaType:AVMediaTypeVideo
                                                             outputSettings:videoSettings] retain];
    NSParameterAssert(assetWriterVideoIn);
    assetWriterVideoIn.expectsMediaDataInRealTime = YES;

    // Add the audio input
    AudioChannelLayout acl;
    bzero( &acl, sizeof(acl));
    acl.mChannelLayoutTag = kAudioChannelLayoutTag_Mono;

    NSDictionary* audioOutputSettings = nil;
    // Both type of audio inputs causes output video file to be corrupted.
    if( NO ) {
        // should work from iphone 3GS on and from ipod 3rd generation
        audioOutputSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                               [ NSNumber numberWithInt: kAudioFormatMPEG4AAC ], AVFormatIDKey,
                               [ NSNumber numberWithInt: 1 ], AVNumberOfChannelsKey,
                               [ NSNumber numberWithFloat: 44100.0 ], AVSampleRateKey,
                               [ NSNumber numberWithInt: 64000 ], AVEncoderBitRateKey,
                               [ NSData dataWithBytes: &acl length: sizeof( acl ) ], AVChannelLayoutKey,
                               nil];
    } else {
        // should work on any device requires more space
        audioOutputSettings =  [ NSDictionary dictionaryWithObjectsAndKeys:
                               [ NSNumber numberWithInt: kAudioFormatAppleLossless ], AVFormatIDKey,
                               [ NSNumber numberWithInt: 16 ], AVEncoderBitDepthHintKey,
                               [ NSNumber numberWithFloat: 44100.0 ], AVSampleRateKey,
                               [ NSNumber numberWithInt: 1 ], AVNumberOfChannelsKey,
                               [ NSData dataWithBytes: &acl length: sizeof( acl ) ], AVChannelLayoutKey,
                               nil ];
    }
    [assetWriter addInput:assetWriterVideoIn];

    if(m_bWithAudio){
        LOG_DEBUG("PREPARING WITH AUDIO !!!!!!");
        assetWriterAudioIn = [[AVAssetWriterInput
                           assetWriterInputWithMediaType: AVMediaTypeAudio
                           outputSettings: audioOutputSettings ] retain];

        assetWriterAudioIn.expectsMediaDataInRealTime = YES;
        [assetWriter addInput:assetWriterAudioIn];
    }
    bPrepared = true;
}

- (void) cleanAfterRecording
{
    [assetWriter release];
    [assetWriterVideoIn release];
    if(m_bWithAudio){
        [assetWriterAudioIn release];
    }
    assetWriter = nil;
    assetWriterVideoIn = nil;
    assetWriterAudioIn = nil;
}

- (void) writeSampleBuffer:(CMSampleBufferRef)sampleBuffer ofType:(NSString *)mediaType
{
    CMTime presentationTime = CMSampleBufferGetPresentationTimeStamp(sampleBuffer);

    if ( assetWriter.status == AVAssetWriterStatusUnknown ) {
        if ([assetWriter startWriting]) {
            [assetWriter startSessionAtSourceTime:presentationTime];
        } else {
            NSLog(@"Error writing initial buffer");
        }
    }

    if ( assetWriter.status == AVAssetWriterStatusWriting ) {
        if (mediaType == AVMediaTypeVideo) {
            if (assetWriterVideoIn.readyForMoreMediaData) {
                if (![assetWriterVideoIn appendSampleBuffer:sampleBuffer]) {
                    NSLog(@"Error writing video buffer");
                }
            }
        }
        else if (mediaType == AVMediaTypeAudio) {
            if (assetWriterAudioIn.readyForMoreMediaData) {
                if (![assetWriterAudioIn appendSampleBuffer:sampleBuffer]) {
                    NSLog(@"Error writing audio buffer");
                }
            }
        }
    }
}

- (void)captureOutput:(AVCaptureOutput*)captureOutput
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection*)connection
{
    @synchronized(self){
        // VideoRecording:
        if(bRecording){
            CFRetain(sampleBuffer);
            dispatch_async(recordingQueue, ^{
                if (assetWriter) {
                    if (connection == _videoConnection) {
                        [self writeSampleBuffer:sampleBuffer ofType:AVMediaTypeVideo];
                    } else if (connection == _audioConnection) {
                        LOG_DEBUG("writed audio sample");
                        [self writeSampleBuffer:sampleBuffer ofType:AVMediaTypeAudio];
                    }
                }
                CFRelease(sampleBuffer);
            });
        }
    }

    if(connection == _audioConnection){
        return;
    }

    // Block for provide preview textures to AexolGL
    @synchronized(self)
    {
        // if freezed preview, check that frameAvaible isn't still true
        if(m_camFrames.size() >= 4)
        {
            return;
        }

        CVImageBufferRef pixelBuffer = CMSampleBufferGetImageBuffer(sampleBuffer);
        size_t width = CVPixelBufferGetWidth(pixelBuffer);
        size_t height = CVPixelBufferGetHeight(pixelBuffer);

        if(width != _textureWidth || height != _textureHeight)
        {
            _textureWidth = width;
            _textureHeight = height;
        }

        if(!_videoTextureCache)
        {
            LOG_ERR("--(!): No video texture cache");
            return;
        }

        // Periodic texture cache flush every frame
        CVOpenGLESTextureCacheFlush(_videoTextureCache, 0);
        CVReturn err;
        LumaChromaTexIds camFrame;

        // Y-plane:
  //    glActiveTexture(GL_TEXTURE0+camDevActiveTextureUnit);
        err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,
                                                           _videoTextureCache,
                                                           pixelBuffer,
                                                           NULL,
                                                           GL_TEXTURE_2D,
                                                           GL_RED_EXT,
                                                           _textureWidth,
                                                           _textureHeight,
                                                           GL_RED_EXT,
                                                           GL_UNSIGNED_BYTE,
                                                           0,
                                                           &camFrame.lumaTexture);

        if(err)
        {
            LOG_ERR("--(!): Error at CVOpenGLESTextureCacheCreateTextureFromImage");
        }
//       #Now this operation are made in aex graphics api: !!!
//        glBindTexture(CVOpenGLESTextureGetTarget(_lumaTexture), CVOpenGLESTextureGetName(_lumaTexture));
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // UV-plane
//      glActiveTexture(GL_TEXTURE0+camDevActiveTextureUnit);
        err = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault,
                                                           _videoTextureCache,
                                                           pixelBuffer,
                                                           NULL,
                                                           GL_TEXTURE_2D,
                                                           GL_RG_EXT,
                                                           _textureWidth / 2,
                                                           _textureHeight / 2,
                                                           GL_RG_EXT,
                                                           GL_UNSIGNED_BYTE,
                                                           1,
                                                           &camFrame.chromaTexture);

        if(err)
        {
            LOG_ERR("--(!): Error at CVOpenGLESTextureCacheCreateTextureFromImage");
        }
//        #Now this operation are made in aex graphics api: !!!
//        glBindTexture(CVOpenGLESTextureGetTarget(_chromaTexture), CVOpenGLESTextureGetName(_chromaTexture));
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        camFrame.luma = CVOpenGLESTextureGetName(camFrame.lumaTexture);
        camFrame.chroma = CVOpenGLESTextureGetName(camFrame.chromaTexture);
        m_camFrames.push(camFrame);
        [self updateTextures]; // clean set as unused now textures
        cbOnNewFrame(); // textures rdy
    }
}

- (void)updateTextures
{
    //LOG("Update texture invoked before synchronized");
    // @synchronized(self) // no needed here, invoked in captureOutput
    // {
        while(!m_unneededCamFrames.empty())
        {
            LumaChromaTexIds frame = m_unneededCamFrames.front();
            CFRelease(frame.lumaTexture);
            CFRelease(frame.chromaTexture);
            m_unneededCamFrames.pop();
        }
//    }
}

- (bool) isFrameAvaible
{
       return m_camFrames.size() >= 1;
}

- (LumaChromaTexIds) getLumaChromaTextureID
{
    @synchronized(self){
        if(lumaChromaTexGetsCounter != 0)
        {
            m_unneededCamFrames.push(m_camFrames.front());
            m_camFrames.pop();
        }
        lumaChromaTexGetsCounter++;

        return m_camFrames.front();
    }
}

+ (int) getCamerasNumber
{
    int counter = 0;
    NSArray* devices = [AVCaptureDevice devices];

    for(AVCaptureDevice * device in devices)
    {
        if([device hasMediaType:AVMediaTypeVideo])
        {
            counter++;
        }
    }

    return counter;
}

- (int) codeSuccess
{
    return 1;
}
- (int) codeFailure
{
    return 0;
}
- (int) codeFailurePermissions
{
    return -1;
}

- (int) getFormatsNum
{
    // TODO remove later, print all formats descriptions
    for(int i = 0 ; i < [[videoDevice formats]count] ; i++)
    {
        AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:i];
        std::string descriptionStr( [[format description] UTF8String] );
        //LOG("INDEX:[", i, "]:", descriptionStr);
    } //

    return (int)[[videoDevice formats]count];
}

- (bool) isYColorFullRangeFormat420fAt: (int) index
{
    AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:index];

    if([[format description] rangeOfString:@"420f"].location == NSNotFound)
    {
        return false; // is not 0 to 255 Y value (no full Range - kCVPixelFormatType_420YpCbCr8BiPlanarFullRange)
    }
    else
    {
        return true; // is full Y range (0 to 255) == kCVPixelFormatType_420YpCbCr8BiPlanarFullRange, we used just this color version now
    }
}

- (int) getPreviewWidhForFormatAt: (int) index
{
    AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:index];
    return format.highResolutionStillImageDimensions.width;
}

- (int) getPreviewHeightForFormatAt: (int) index
{
    AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:index];
    return format.highResolutionStillImageDimensions.height;
}

- (float) getZoom
{
    return videoDevice.videoZoomFactor;
}

- (float) getZoomMaxForFormatAt: (int) index
{
    AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:index];
    return format.videoMaxZoomFactor;
}

- (float) getZoomUpscaleTresholdForFormatAt: (int) index
{
    AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:index];
    return format.videoZoomFactorUpscaleThreshold;
}

- (int) setFormatAt: (int) index
{
    if([videoDevice lockForConfiguration:NULL] == YES)
    {
        videoDevice.activeFormat = [[videoDevice formats]objectAtIndex:index];
        [videoDevice unlockForConfiguration];

        return [self codeSuccess];
    }
    else
    {
        return [self codeFailure];
    }

}

- (int) setZoomFactor: (float) factor
{
    if([videoDevice lockForConfiguration:NULL] == YES)
    {
        [videoDevice setVideoZoomFactor:factor];
        [videoDevice unlockForConfiguration];

        return [self codeSuccess];
    }
    else
    {
        return [self codeFailure];
    }
}

- (int) setZoomFactorSmoothly: (float) factor : (float) rate
{
    //LOG("Rate:", rate);
    NSError* error = nil;

    // wait for end last zooming, TODO - to check
    if(checkingSmoothZoom)
    {
        return [self codeSuccess];
    }

    if([videoDevice lockForConfiguration:&error])
    {
        [videoDevice rampToVideoZoomFactor:factor withRate:rate];
        [videoDevice unlockForConfiguration];

        // callback for ended smoothing
        if(! checkingSmoothZoom)
        {
            checkingSmoothZoom = true;
            endZoomFactor = factor;
            [videoDevice addObserver:self forKeyPath:@"videoZoomFactor" options:NSKeyValueObservingOptionNew context:nil];
        }

    }
    else
    {
        std::string exceptionStr([[error description] UTF8String]);
        LOG("--(!): error while setting smooth zoom !!!", exceptionStr);
        return [self codeFailure];
    }

    return [self codeSuccess];
}

- (bool) isSmoothlyZoomSupported
{
    return true; // TODO can check this somewhere ?
}

- (int) getFocalLength
{
    return -1.0f; // TODO can check this somewhere ?
}

- (float) getExposureCompensation
{
    return [videoDevice exposureTargetBias]; // EV
}

- (float) getMinExposureCompensation
{
    return [videoDevice minExposureTargetBias];
}

- (float) getMaxExposureCompensation
{
    return [videoDevice maxExposureTargetBias];
}

- (float) getExposureCompensationStep
{
    return 0.1f; // Documentation: Exposure compensation is expressed in f-stops. +1 f-stop doubles the brightness, -1 f-stop halves the brightness.
}

- (int) setExposureCompensation: (float) ev
{
    NSError* error = nil;
    if([videoDevice lockForConfiguration:&error])
    {
        // ev = lroundf(ev);
        // Developers can currently set exposure target biases between -8 and +8 for all existing iOS devices. However, Apple warns that that could change in the future.
        [videoDevice setExposureTargetBias:ev completionHandler: ^ (CMTime time)
         {
             if(cbExpComp) cbExpComp(); // c++ callback invocation xD
         }];
//        [videoDevice setActiveVideoMaxFrameDuration:kCMTimeInvalid];
//        [videoDevice setActiveVideoMinFrameDuration:kCMTimeInvalid];
        [videoDevice unlockForConfiguration];
    }
    else
    {
        std::string exceptionStr([[error description] UTF8String]);
        LOG("--(!): error while setting exposure compensation !!!", exceptionStr);
        return [self codeFailure];
    }

    return [self codeSuccess];
}

- (float) getMinISOForFormatAt: (int) index
{
    AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:index];
    return format.minISO;
}

- (float) getMaxISOForFormatAt: (int) index
{
    AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:index];
    return format.maxISO;
}

- (float) getISO
{
    return videoDevice.ISO;
}

- (int) setISO: (float) iso
{
    NSError* error = nil;
    if([videoDevice lockForConfiguration:&error])
    {
        [videoDevice setExposureModeCustomWithDuration:AVCaptureExposureDurationCurrent ISO:iso completionHandler:^ (CMTime time)
         {
             if(cbISO) cbISO(); // c++ callback invocation
         }];
        [videoDevice unlockForConfiguration];
    }
    else
    {
        std::string exceptionStr([[error description] UTF8String]);
        LOG("--(!): error while setting iso !!!", exceptionStr);
        return [self codeFailure];
    }
    return [self codeSuccess];
}

- (float) getMinExposureDurationForFormatAt: (int) index
{
    AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:index];
    return CMTimeGetSeconds(format.minExposureDuration);
}
- (float) getMaxExposureDurationForFormatAt: (int) index
{
    AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:index];
    return CMTimeGetSeconds(format.maxExposureDuration);
}

- (float) getExposureDuration
{
    return CMTimeGetSeconds(videoDevice.exposureDuration);
}

- (int) setExposureDuration: (float) durationSeconds
{
    NSError* error = nil;
    if([videoDevice lockForConfiguration:&error])
    {
        [videoDevice setExposureModeCustomWithDuration:CMTimeMakeWithSeconds(durationSeconds, 1000*1000*1000)  ISO:AVCaptureISOCurrent completionHandler:^ (CMTime time)
         {
             if(cbExposureDuration) cbExposureDuration(); // c++ callback invocation
         }];
        [videoDevice unlockForConfiguration];
    }
    else
    {
        std::string exceptionStr([[error description] UTF8String]);
        LOG("--(!): error while setting exposureDuration !!!", exceptionStr);
        return [self codeFailure];
    }
    return [self codeSuccess];
}

- (int) setISOAndED: (float) iso : (float) ed
{
    NSError* error = nil;
    if([videoDevice lockForConfiguration:&error])
    {
        [videoDevice setExposureModeCustomWithDuration:CMTimeMakeWithSeconds(ed, 1000*1000*1000)  ISO:iso completionHandler:^ (CMTime time)
         {
             if(cbISOAndED) cbISOAndED(); // c++ callback invocation
         }];
        [videoDevice unlockForConfiguration];
    }
    else
    {
        std::string exceptionStr([[error description] UTF8String]);
        LOG("--(!): error while setting ISO and ED !!!", exceptionStr);
        return [self codeFailure];
    }
    return [self codeSuccess];
}

- (bool) isLowLightBoostSupported
{
    return [videoDevice isLowLightBoostSupported];
}

- (void) setLowLightBoost : (bool) enabled
{
    if([videoDevice respondsToSelector:@selector(isLowLightBoostSupported)]){
        if([videoDevice lockForConfiguration:nil]){
            if([videoDevice isLowLightBoostSupported]){
                videoDevice.automaticallyEnablesLowLightBoostWhenAvailable = TRUE;
            }
            [videoDevice unlockForConfiguration];
        }
    }
}

- (int) isSupportedExposurePointOfInterest
{
    if([videoDevice isExposurePointOfInterestSupported])
    {
        return [self codeSuccess];
    }
    else
    {
        return [self codeFailure];
    }
}

- (int) setExposurePointOfInterest: (float) clampfX : (float) clampfY
{
    if([videoDevice isExposurePointOfInterestSupported])
    {
        NSError* error = nil;

        if([videoDevice lockForConfiguration:&error])
        {
            CGPoint p;
            p.x = clampfX;
            p.y = clampfY;
            //LOG("setted point of interest in iOS:", p.x, ", and ", p.y);
            [videoDevice setExposurePointOfInterest:p];
            [videoDevice unlockForConfiguration];
        }
        else
        {
            std::string exceptionStr([[error description] UTF8String]);
            LOG_ERR("--(!): error while setting exposure point of interest (area) !!!", exceptionStr);
            return [self codeFailure];
        }

        return [self codeSuccess];
    }
    else
    {
        return [self codeFailure];
    }
}

- (float) getExposurePointOfInterestX
{
    return videoDevice.exposurePointOfInterest.x;
}

- (float) getExposurePointOfInterestY
{
    return videoDevice.exposurePointOfInterest.y;
}

- (int) isSupportedFocusPointOfInterest
{
    if([videoDevice isFocusPointOfInterestSupported])
    {
        return [self codeSuccess];
    }
    else
    {
        return [self codeFailure];
    }
}

- (int) setFocusPointOfInterest: (float) clampfX : (float) clampfY
{
    if([videoDevice isFocusPointOfInterestSupported])
    {
        NSError* error = nil;

        if([videoDevice lockForConfiguration:&error])
        {
            LOG_DEBUG("setting focus point of interest to: ", clampfX, ", ", clampfY );
            CGPoint p;
            p.x = clampfX;
            p.y = clampfY;
            //LOG("setted point of interest in iOS:", p.x, ", and ", p.y);
            [videoDevice setFocusPointOfInterest:p];
            [videoDevice unlockForConfiguration];
        }
        else
        {
            std::string exceptionStr([[error description] UTF8String]);
            LOG_ERR("--(!): error while setting focus point of interest (area) !!!", exceptionStr);
            return [self codeFailure];
        }

        return [self codeSuccess];
    }
    else
    {
        return [self codeFailure];
    }
}

- (float) getFocusPointOfInterestX
{
    return videoDevice.focusPointOfInterest.x;
}

- (float) getFocusPointOfInterestY
{
    return videoDevice.focusPointOfInterest.y;
}

- (std::vector<float>) getFPSRangeForActiveFormat
{
    return [self getFPSRangeForFormat:videoDevice.activeFormat];
}

- (std::vector<float>) getFPSRangeForFormatAt: (int) index
{
    AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:index];
    return [self getFPSRangeForFormat:format];
}

- (std::vector<float>) getFPSRangeForFormat: (AVCaptureDeviceFormat*) format
{
    // find that fps
    AVFrameRateRange* chosenRateRange = nil;

    for(AVFrameRateRange * range in format.videoSupportedFrameRateRanges)
    {
        if(range.minFrameDuration.value == videoDevice.activeVideoMinFrameDuration.value)
        {
            if(range.maxFrameDuration.value == videoDevice.activeVideoMaxFrameDuration.value)
            {
                chosenRateRange = range;
                break;
            }
        }
    }

    if(chosenRateRange)
    {
        std::vector<float> fpsRange;
        fpsRange.push_back(chosenRateRange.minFrameRate);
        fpsRange.push_back(chosenRateRange.maxFrameRate);
        return fpsRange;
    }

    // should not invoke this
    return std::vector<float> {0, 0}; // empty
}

- (std::vector<float>) getSupportedFPSRangesForFormatAt: (int) index
{
    AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:index];

    std::vector<float> rangesVector;

    for(AVFrameRateRange * range in format.videoSupportedFrameRateRanges)
    {
        rangesVector.push_back(range.minFrameRate);
        rangesVector.push_back(range.maxFrameRate);
    }

    return rangesVector;
}

- (int) setFPSRange: (float) minFPS : (float) maxFPS
{

    AVCaptureDeviceFormat* format = videoDevice.activeFormat;

    // find that fps
    AVFrameRateRange* chosenRateRange = nil;

    for(AVFrameRateRange * range in format.videoSupportedFrameRateRanges)
    {
        if(range.maxFrameRate >= maxFPS)
        {
            if(range.minFrameRate <= minFPS)
            {
                chosenRateRange = range;
                break;
            }
        }
    }

    if(chosenRateRange)
    {
        if([videoDevice lockForConfiguration:NULL] == YES)
        {
            //videoDevice.activeVideoMinFrameDuration = chosenRateRange.minFrameDuration;
            //videoDevice.activeVideoMaxFrameDuration = chosenRateRange.maxFrameDuration;
            [videoDevice setActiveVideoMinFrameDuration:CMTimeMake(1, minFPS)];
            [videoDevice setActiveVideoMaxFrameDuration:CMTimeMake(1, maxFPS)];
            [videoDevice unlockForConfiguration];
            return [self codeSuccess];
        }
    }

    return [self codeFailure];
}

- (float) getHorizontalViewAngle
{
    return -1.0f;
}

- (float) getVerticalViewAngle
{
    return -1.0f;
}

- (float) getLensAperture
{
    return videoDevice.lensAperture;
}

- (float) getLensPosition
{
    return videoDevice.lensPosition;
}

- (float) getFOVForFormatAt: (int) index
{
    AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:index];
    return format.videoFieldOfView;
}

- (float) getFOVAvtiveFormat
{
    return videoDevice.activeFormat.videoFieldOfView;
}

- (int) setLensPositionWithFixedFocusMode: (float) position
{
    if(! [self isParamSupported:FOCUS_MODE_FIXED])
    {
        return [self codeFailure];    // must be supported
    }

    if([videoDevice lockForConfiguration:NULL] == YES)
    {
        [videoDevice setFocusModeLockedWithLensPosition: position completionHandler: ^ (CMTime time)
         {
             if(cbLensPos) cbLensPos(); // c++ callback invocation xD
         }];
        [videoDevice unlockForConfiguration];
        return [self codeSuccess];
    }

    return [self codeFailure];
}

- (int) autoFocus
{
    if([self isParamSupported:FOCUS_MODE_AUTO])
    {
        [videoDevice addObserver:self forKeyPath:@"adjustingFocus" options:NSKeyValueObservingOptionNew context:nil];
        [self setParam:FOCUS_MODE_AUTO];

        // ON 99% its not needed, after setParam FOCUS_MODE_AUTO it always try to focus so change valuie adjustingFocus to true and than false
        //        if(videoDevice.adjustingFocus == false)
        //        {
        //            LOG("ENDED AUTO FOCUS INMEDIATELY, NOT STARTED FOCUSING YET");
        //            cbAutoFocus(true);
        //        }

        return [self codeSuccess];
    }
    else
    {
        return [self codeFailure];
    }
}

enum FORMAT{
    JPG = 0x01,
};

enum RESULT{
    SUCCESS = 0x01,
    FAILURE = 0x00,
    NOT_ENOUGHT_MEMORY = 0x02,
    FILE_NOT_FOUND = 0x03,
    IO_EXCEPTION = 0x04
};


- (void) prepareStillImageOutput{
    if(stillImageOutput == nil){
    stillImageOutput = [[AVCaptureStillImageOutput alloc] init];
    NSDictionary *outputSettings = @{ AVVideoCodecKey : AVVideoCodecJPEG};
    [stillImageOutput setOutputSettings:outputSettings];
    }
}

- (bool) isSupportedPictureFormat: (int) format{
    if(format == FORMAT::JPG)
        return true;

    return false;
}

const static int FILTER_NONE = 0;
const static int FILTER_NOCTOVISION = 1;

- (int) takePicture: (std::string) pictureFilePath : (std::string) albumNameCpp : (int) width : (int) height : (int) format : (int) JPGQuality : (int) filter : (bool) saveInAlbum {

    _picPath =  @(pictureFilePath.c_str());
    _albumName= @(albumNameCpp.c_str() );
    picWidth = width;
    picHeight = height;
    picFormat = format;
    picJPGQuality = JPGQuality;
    picSaveInAlbum = saveInAlbum;
    picFilter = filter;

    AVCaptureConnection *videoConnection = nil;
    for (AVCaptureConnection *connection in stillImageOutput.connections) {
        for (AVCaptureInputPort *port in [connection inputPorts]) {
            if ([[port mediaType] isEqual:AVMediaTypeVideo] ) {
                videoConnection = connection;
                break;
            }
        }
        if (videoConnection) { break; }
    }


    [stillImageOutput captureStillImageAsynchronouslyFromConnection:videoConnection completionHandler:
     ^(CMSampleBufferRef imageSampleBuffer, NSError *error) {

         if (imageSampleBuffer != NULL)
         {
             _imageData = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation:imageSampleBuffer];

             if(picSaveInAlbum){
                 if(picFilter == FILTER_NONE){
                     _image = [[UIImage alloc] initWithData: _imageData];
                 }
                 if(picFilter == FILTER_NOCTOVISION){
                     if(picFilter == FILTER_NOCTOVISION){
                         UIImage *image      = [[UIImage alloc] initWithData: _imageData];
                         CGImageRef imageRef = image.CGImage;
                         NSData *data        = (NSData *)CGDataProviderCopyData(CGImageGetDataProvider(imageRef));

                         _pixels  = (unsigned char *)[data bytes];

                         //LOG("BEFORE MODIFY");
                         for(int i = 0; i < [data length]; i += 4)
                         {
                             float rf = _pixels[i] / 255.0f;
                             float gf = _pixels[i+1] / 255.0f;
                             float bf = _pixels[i+2] / 255.0f;
                             int a =  _pixels[i+3];

                             rf = rf / 0.2f;
                             gf = gf / 0.2f;
                             bf = bf / 0.2f;

                             float green = (rf + gf + bf) / 3.0f;
                             if(green > 1.0f) green = 1.0f;
                             float wspol = 1.0f - green;

                             rf = 1.0f - wspol*2;
                             gf = green;
                             bf = 1.0f - wspol*2;

                             if(rf < 0.0f)
                                 rf = 0.0f;
                             if(bf < 0.0f)
                                 bf = 0.0f;

                             _pixels[i]   = rf * 255;
                             _pixels[i+1] = gf * 255;
                             _pixels[i+2] = bf * 255;
                             _pixels[i+3] = a;
                         }
                         //LOG("LENGHT: ", [_imageData length]);
                         //LOG("LENGTH new: ", [data length]);
                         //LOG("AFTER MODIFY");

                         size_t width                    = CGImageGetWidth(imageRef);
                         size_t height                   = CGImageGetHeight(imageRef);
                         size_t bitsPerComponent         = CGImageGetBitsPerComponent(imageRef);
                         size_t bitsPerPixel             = CGImageGetBitsPerPixel(imageRef);
                         size_t bytesPerRow              = CGImageGetBytesPerRow(imageRef);

                         CGColorSpaceRef colorspace      = CGColorSpaceCreateDeviceRGB();
                         CGBitmapInfo bitmapInfo         = CGImageGetBitmapInfo(imageRef);
                         CGDataProviderRef provider      = CGDataProviderCreateWithData(NULL, _pixels, [data length], NULL);

                         CGImageRef newImageRef = CGImageCreate (
                                                                 width,
                                                                 height,
                                                                 bitsPerComponent,
                                                                 bitsPerPixel,
                                                                 bytesPerRow,
                                                                 colorspace,
                                                                 bitmapInfo,
                                                                 provider,
                                                                 NULL,
                                                                 false,
                                                                 kCGRenderingIntentDefault
                                                                 );
                         // the modified image
                         // UIImage *newImage   = [UIImage imageWithCGImage:newImageRef];
                         _image = [[UIImage alloc]  initWithCGImage:newImageRef
                                                    scale:1.0
                                                    orientation: image.imageOrientation];


                         // cleanup
                         //free(pixels);
                         CGImageRelease(imageRef);
                         CGColorSpaceRelease(colorspace);
                         CGDataProviderRelease(provider);
                         CGImageRelease(newImageRef);
                         //[image release];
                         //[image dealloc];
                     }
                 }

                 [self saveImgInAlbum]; // it calls callbacks for AexolGL
             }else{

                 [self saveImgInFile]; // it calls callbacks for AexolGL
             }
         }
         else
         {
             cbTakePicture(RESULT::FAILURE);
             return;
         }
     }];

    return [self codeSuccess];
}

- (void) cleanMemoryAfterPicture
{
    if(picFilter == FILTER_NONE)
    {
        [_image release];
    }

    if(picFilter == FILTER_NOCTOVISION)
    {

    }
}

- (void) saveImgInFile{
    BOOL writeSuccess = [_imageData writeToFile : _picPath atomically:YES]; //Write the file
    if(! writeSuccess){
        cbTakePicture(RESULT::IO_EXCEPTION);
        return;
    }else{
        cbTakePicture(RESULT::SUCCESS);
        return;
    }
}

- (void) saveImgInAlbum{
    if( [_albumName length] != 0){
        LOG("TAKING PICTURE IN NAMED ALBUM: ", [_albumName UTF8String]);

        [library addAssetsGroupAlbumWithName: _albumName
                                 resultBlock:^(ALAssetsGroup *group) {
                                     if(group != nil){
                                         LOG("Created NEW Group: ", [[group description] UTF8String]);
                                     }else{
                                         LOG("Group exists: ",[_albumName UTF8String]);
                                     }

                                     // take picture and add to album
                                     [library writeImageToSavedPhotosAlbum:[_image CGImage]
                                                               orientation:(ALAssetOrientation)[_image imageOrientation]
                                                           completionBlock:^(NSURL* assetURL, NSError* error2){
                                                               if (error2.code == 0) {

                                                                   // add to album
                                                                   [library enumerateGroupsWithTypes:ALAssetsGroupAlbum
                                                                                          usingBlock:^(ALAssetsGroup *group2, BOOL *stop){

                                                                                              // LOG("looking for file: ", [_albumName UTF8String] );
                                                                                              //if(group2 != nil){
                                                                                              //    LOG("is file: ", [[group2 valueForProperty:ALAssetsGroupPropertyName] UTF8String]);
                                                                                              //}

                                                                                              if ([[group2 valueForProperty:ALAssetsGroupPropertyName] isEqualToString: _albumName ])
                                                                                              {
                                                                                                  LOG("FOUNDED NEEDED ALBUM");
                                                                                                  [library assetForURL:assetURL
                                                                                                           resultBlock:^(ALAsset *asset) {
                                                                                                               [group2 addAsset:asset];

                                                                                                               LOG("ADDED TO ALBUM");
                                                                                                           }
                                                                                                          failureBlock:^(NSError* error) {

                                                                                                              LOG("NOT ADDED TO ALBUM");
                                                                                                              // not added to album
                                                                                                          }];
                                                                                              }else{
                                                                                                  //LOG("IT ISNT NEEDED ALBUM");
                                                                                              }
                                                                                          }
                                                                                        failureBlock:^(NSError* error3) {
                                                                                            cbTakePicture(RESULT::FAILURE); // NO ALBUM
                                                                                            return;
                                                                                        }];
                                                                   [_image release];
                                                                   if(_pixels != nil){
                                                                       free(_pixels);
                                                                       _pixels = nil;
                                                                   }

                                                                   cbTakePicture(RESULT::SUCCESS);
                                                                   return;
                                                               }
                                                               else {
                                                                   [_image release];
                                                                   if(_pixels != nil){
                                                                       free(_pixels);
                                                                       _pixels = nil;
                                                                   }

                                                                   cbTakePicture(RESULT::FAILURE);
                                                                   return;
                                                               }
                                                           }];
                                 }
                                failureBlock:^(NSError *error) {
                                    LOG("ALBUM NOT CREATED");
                                    [_image release];
                                    if(_pixels != nil){
                                        free(_pixels);
                                        _pixels = nil;
                                    }
                                    cbTakePicture(RESULT::FAILURE);
                                    return;
                                }];

        return;
    }else{
        LOG("TAKING PICTURE IN GLOBAL ALBUM: ", [_albumName UTF8String]);
        [library writeImageToSavedPhotosAlbum:[_image CGImage]
                                  orientation:(ALAssetOrientation)[_image imageOrientation]
                              completionBlock:^(NSURL *assetURL, NSError *error)
         {
             // nothing here, global album
             [_image release];
             if(_pixels != nil){
                 free(_pixels);
                 _pixels = nil;
             }
             cbTakePicture(RESULT::SUCCESS);
             return;
         }];
    }
}

// FINDING ALBUM :
//    [library enumerateGroupsWithTypes:ALAssetsGroupAlbum
//                           usingBlock:^(ALAssetsGroup *group, BOOL *stop){
//                               NSString* albumName = [NSString stringWithUTF8String : picAlbumNameCpp.c_str()];
//                               LOG("looking for file ", [albumName UTF8String] );
//                               if ([[group valueForProperty:ALAssetsGroupPropertyName] isEqualToString: albumName ])
//                               {
//                                   LOG("ALBUM FOUNDED");
//                               }else{
//                                   LOG("ALBUM NOT FOUNDED");
//                               }
//                           }
//                         failureBlock:^(NSError* error3) {
//                             cbTakePicture(RESULT::FAILURE); // NO ALBUM
//                             return;
//                         }];



- (int) setRotation : (int) degrees{
    int deg = degrees % 360;

    AVCaptureVideoOrientation orient;

    switch(deg){
        case 0:
            orient = AVCaptureVideoOrientationLandscapeRight; // home button on right
            break;
        case 90:
            orient = AVCaptureVideoOrientationPortrait; // normal
            break;
        case 180:
            orient = AVCaptureVideoOrientationLandscapeLeft; // home button on left
            break;
        case 270:
            orient = AVCaptureVideoOrientationPortraitUpsideDown; // inversed
            break;
        default:
            return [self codeFailure]; // bad degree provided
            break;
    }

    //LOG("----- Setting rotation to: ", deg);
    [[stillImageOutput connectionWithMediaType:AVMediaTypeVideo] setVideoOrientation: orient]; // set image orientation for this output

    // TODO - handle video rotation
    //    @synchronized(self){
    //        if(!bRecording)
    //            [_videoConnection setVideoOrientation:orient]; // for video orientation
    //    }
    return [self codeSuccess];
}

- (void) setCallbackOnNewFrame: (const std::function<void()>&) cb{
    cbOnNewFrame = cb;
}

- (void) setCallbackExposureCompensation: (const std::function<void()>&) cb
{
    cbExpComp = cb;
}

- (void) setCallbackLensPosition: (const std::function<void()>&) cb
{
    cbLensPos = cb;
}

- (void) setCallbackSmoothZoom: (const std::function<void()>&) cb
{
    cbSmoothZoom = cb;
}

- (void) setCallbackAutoFocus: (const std::function<void(bool)>&) cb
{
    cbAutoFocus = cb;
}

- (void) setCallbackTakePicture: (const std::function<void(int)>&) cb{
    cbTakePicture = cb;
}

- (void) setCallbackISO: (const std::function<void()>&) cb
{
    cbISO = cb;
}

- (void) setCallbackExposureDuration: (const std::function<void()>&) cb
{
    cbExposureDuration = cb;
}

- (void) setCallbackISOAndED: (const std::function<void()>&) cb
{
    cbISOAndED = cb;
}

- (void) setCallbackStopVideo: (const std::function<void(int)>&) cb
{
    cbStopVideo = cb;
}

// key-value observing
- (void)observeValueForKeyPath:(NSString*)keyPath ofObject:(id)object change:(NSDictionary*)change context:(void*)context
{
    if([keyPath isEqualToString:@"adjustingFocus"])
    {
        BOOL adjustingFocus = [ [change objectForKey:NSKeyValueChangeNewKey] isEqualToNumber:[NSNumber numberWithInt:1] ];
        //        NSLog(@"Is adjusting focus? %@", adjustingFocus ? @"YES" : @"NO");
        //        NSLog(@"Change dictionary: %@", change);

        if(adjustingFocus == false)
        {
            [videoDevice removeObserver:self forKeyPath:@"adjustingFocus"];
            cbAutoFocus(true);
        }
    }

    if([keyPath isEqualToString:@"videoZoomFactor"])
    {
        BOOL endedZooming = [ [change objectForKey:NSKeyValueChangeNewKey] isEqualToNumber:[NSNumber numberWithFloat:endZoomFactor] ];
        //        NSLog(@"Is ended zooming? %@", endedZooming ? @"YES" : @"NO" );
        //        NSLog(@"Change dictionary: %@", change);

        if(endedZooming == true)
        {
            checkingSmoothZoom = false;
            [videoDevice removeObserver:self forKeyPath:@"videoZoomFactor"]; // must be before callback invocation (user can invoke zooming method again in callback)
            cbSmoothZoom();
        }
    }
}

-(void) dealloc
{
    if(_session)
        [_session release];
    if(dataOutput)
        [dataOutput release];
    if(stillImageOutput)
        [stillImageOutput release];
    if(audioInput)
        [audioInput release];
    if(audioOutput)
        [audioOutput release];
    if(library)
        [library release];
    if(recordingQueue)
        [recordingQueue release];

    [super dealloc];
}

// -  (float) getZoom{
//    NSString *activeFormat = [videoDevice.activeFormat description];
//    NSString *allFormats = [videoDevice.formats description];
//
//    std::string activeFormatStr([activeFormat UTF8String]);
//    std::string allFormatsStr([allFormats UTF8String]);
//
//     AVCaptureDeviceFormat *bestFormat = nil;
//     AVFrameRateRange *bestFrameRateRange = nil;
//    for ( AVCaptureDeviceFormat *format in [videoDevice formats] ) {
//         int width = format.highResolutionStillImageDimensions.width;
//         int height = format.highResolutionStillImageDimensions.height;
//         LOG(width, "xxx", height);
//    }
//

//    LOG(activeFormatStr, ", max zoom factor: ", videoDevice.activeFormat.videoMaxZoomFactor);
//    LOG("All formats", allFormatsStr);
//    return videoDevice.videoZoomFactor;
//    }
//-   (float) getZoomMax{
//    return videoDevice.activeFormat.videoZoomFactorUpscaleThreshold;
//}

//- (std::vector<int>) getSupportedPreviewSizes{
//    std::vector<int> suppSizes;
//
//    //AVCaptureDeviceFormat *bestFormat = nil;
//    //AVFrameRateRange *bestFrameRateRange = nil;
//    for(int i = 1 ; i < [[videoDevice formats]count] ; i+=2){
//        AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:i];
//        int w = format.highResolutionStillImageDimensions.width;
//        int h = format.highResolutionStillImageDimensions.height;
//
//        suppSizes.push_back(w);
//        suppSizes.push_back(h);
//        LOG(w, "-xxx-", h);
//        std::string descriptionStr([[format description] UTF8String]);
//        LOG(descriptionStr);
//    }
//
//    return suppSizes;
//}

//- (void) setPreviewSize: (int)width : (int) height{
//    //AVFrameRateRange *bestFrameRateRange = nil;
//
//    AVCaptureDeviceFormat* findedFormat;
//    for(int i = 1 ; i < [[videoDevice formats]count] ; i+=2){
//        AVCaptureDeviceFormat* format = [[videoDevice formats]objectAtIndex:i];
//        int w = format.highResolutionStillImageDimensions.width;
//        int h = format.highResolutionStillImageDimensions.height;
//
//        LOG(w, "-xxx-", h);
//
//        if(width == w){
//            if(height == h){
//                findedFormat = format;
//                break;
//            }
//        }
//     }
//
//     if ([videoDevice lockForConfiguration:NULL] == YES ) {
//         videoDevice.activeFormat = findedFormat;
//         [videoDevice unlockForConfiguration];
//     }
//}

@end
