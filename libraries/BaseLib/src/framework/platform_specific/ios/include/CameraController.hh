#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <AssetsLibrary/AssetsLibrary.h>
#import <ImageIO/CGImageProperties.h>
#include <vector>
#include <functional>
#include <string>
#include <algorithm>
#include <queue>
#include <aex/Logger.hpp>

struct LumaChromaTexIds
{
    CVOpenGLESTextureRef lumaTexture;
    CVOpenGLESTextureRef chromaTexture;
    int luma;
    int chroma;
    LumaChromaTexIds()
    {
        lumaTexture = nil;
        chromaTexture = nil;
        luma = 0;
        chroma = 0;
    }
};

@interface CameraController : NSObject <AVCaptureVideoDataOutputSampleBufferDelegate, AVCaptureAudioDataOutputSampleBufferDelegate>
{
    CGFloat _screenWidth;
    CGFloat _screenHeight;
    size_t _textureWidth;
    size_t _textureHeight;

    EAGLContext* _context;

    std::queue<LumaChromaTexIds> m_camFrames;
    std::queue<LumaChromaTexIds> m_unneededCamFrames;
    unsigned long long lumaChromaTexGetsCounter;

//    CVOpenGLESTextureRef _lumaTexture;
//    CVOpenGLESTextureRef _chromaTexture;
//    CVOpenGLESTextureRef _lumaTexture2;
//    CVOpenGLESTextureRef _chromaTexture2;

    AVCaptureSession* _session;
    AVCaptureDevice* videoDevice;
    AVCaptureDevice* audioDevice;
    CVOpenGLESTextureCacheRef _videoTextureCache;

    AVCaptureWhiteBalanceMode whiteBalanceDefault;
    AVCaptureFlashMode flashModeDefault;
    AVCaptureFocusMode focusModeDefault;
    AVCaptureTorchMode torchModeDefault;
    AVCaptureExposureMode exposureModeDefault;

    std::function<void()> cbOnNewFrame;
    std::function<void()> cbExpComp;
    std::function<void()> cbLensPos;
    std::function<void()> cbSmoothZoom;
    std::function<void(bool)> cbAutoFocus;
    std::function<void(int)> cbTakePicture;
    std::function<void(int)> cbStopVideo;
    std::function<void()> cbISO;
    std::function<void()> cbExposureDuration;
    std::function<void()> cbISOAndED;
    std::function<void()> cbQuitGallery;

    bool checkingSmoothZoom;
    float endZoomFactor;

    AVCaptureStillImageOutput* stillImageOutput;
    ALAssetsLibrary *library;

    __block NSString* _picPath;
    __block NSString* _albumName;
    int picWidth;
    int picHeight;
    int picFormat;
    int picJPGQuality;
    bool picSaveInAlbum;
    bool picFilter;
    bool m_bWithAudio;
    __block NSData *_imageData; //  variable can be changed in blocks if __block is used
    __block UIImage *_image;
    __block ALAssetsGroup *_group;

    // for video recording
    bool bRecording;
    bool bPrepared;
    dispatch_queue_t recordingQueue;
    AVAssetWriter* assetWriter;
    AVAssetWriterInput* assetWriterVideoIn;
    AVAssetWriterInput* assetWriterAudioIn;
    AVCaptureConnection* _videoConnection;
    AVCaptureConnection* _audioConnection;
    NSURL *_URL;
    bool _videoSaveInAlbum;
    std::string _videoFullPath;
    std::string _videoAlbumName;

    AVCaptureDeviceInput* input;
    AVCaptureDeviceInput * audioInput;

    AVCaptureVideoDataOutput* dataOutput;
    AVCaptureAudioDataOutput* audioOutput;

    unsigned char *_pixels;

    int camDevActiveTextureUnit;
}


- (int) isParamSupported: (int) param;
- (int) setParam: (int) param;

- (int) viewDidLoad;
- (void) viewDidUnload;
- (bool) isOpened: (int) camID;
- (void) setCamDevActiveTexUnit: (int) texUnit;
- (id) init;
- (int)  openCamera: (int) camID : (bool) bWithAudio;
- (AVCaptureDevice*) getCaptureDevice: (int) id;
- (void)tearDownAVCapture;

- (void)updateTextures;
- (void)freeCameraFrameFlag;
- (void)cleanUpTextures;

- (void) startPreview;
- (void) stopPreview;
- (void) releaseCamera;

- (void) startVideo: (std::string) fullPath : (std::string) albumName : (bool) inAlbum : (int) width : (int) height;
- (void) stopVideo;
- (void) addVideoToAlbum;
- (void) prepareToRecord: (int) width : (int) height;
- (void) cleanAfterRecording;
- (void) writeSampleBuffer:(CMSampleBufferRef)sampleBuffer ofType:(NSString *)mediaType;

- (bool) isFrameAvaible;
- (LumaChromaTexIds) getLumaChromaTextureID;

+ (int) getCamerasNumber;

- (int) codeSuccess;
- (int) codeFailure;
- (int) codeFailurePermissions;

- (int) getFormatsNum;
- (bool) isYColorFullRangeFormat420fAt: (int) index;
- (int) getPreviewWidhForFormatAt: (int) index;
- (int) getPreviewHeightForFormatAt: (int) index;

// zoom settings
- (float) getZoom;
- (float) getZoomMaxForFormatAt: (int) index;
- (float) getZoomUpscaleTresholdForFormatAt: (int) index;
- (int) setFormatAt: (int) index;
- (int) setZoomFactor: (float) factor;
- (int) setZoomFactorSmoothly: (float) factor : (float) rate;
- (bool) isSmoothlyZoomSupported;
- (int) getFocalLength;

// exposure compensation settings
- (float) getExposureCompensation;
- (float) getMinExposureCompensation;
- (float) getMaxExposureCompensation;
- (float) getExposureCompensationStep;
- (int) setExposureCompensation: (float) ev;
- (float) getMinISOForFormatAt: (int) index;
- (float) getMaxISOForFormatAt: (int) index;
- (float) getISO;
- (int) setISO: (float) iso;
- (float) getMinExposureDurationForFormatAt: (int) index;
- (float) getMaxExposureDurationForFormatAt: (int) index;
- (float) getExposureDuration;
- (int) setExposureDuration: (float) durationSeconds;
- (int) setISOAndED: (float) iso : (float) ed;
- (bool) isLowLightBoostSupported;
- (void) setLowLightBoost : (bool) enabled;

- (int) isSupportedExposurePointOfInterest;
- (int) setExposurePointOfInterest: (float) clampfX : (float) clampfY;
- (float) getExposurePointOfInterestX;
- (float) getExposurePointOfInterestY;

- (int) isSupportedFocusPointOfInterest;
- (int) setFocusPointOfInterest: (float) clampfX : (float) clampfY;
- (float) getFocusPointOfInterestX;
- (float) getFocusPointOfInterestY;

- (std::vector<float>) getFPSRangeForActiveFormat;
- (std::vector<float>) getFPSRangeForFormatAt: (int) index;
- (std::vector<float>) getFPSRangeForFormat: (AVCaptureDeviceFormat*) format;
- (std::vector<float>) getSupportedFPSRangesForFormatAt: (int) index;
- (int) setFPSRange: (float) minFPS : (float) maxFPS;

- (float) getHorizontalViewAngle;
- (float) getVerticalViewAngle;

- (float) getLensAperture;
- (float) getLensPosition;

- (float) getFOVForFormatAt: (int) index;
- (float) getFOVAvtiveFormat;

- (int) setLensPositionWithFixedFocusMode: (float) position;

- (int) autoFocus;

- (void) prepareStillImageOutput;
- (bool) isSupportedPictureFormat: (int) format;

- (int)  takePicture: (std::string) pictureFilePath : (std::string) albumNameCpp : (int) width : (int) height : (int) format : (int) JPGQuality : (int) filter : (bool) saveInAlbum;
- (void) cleanMemoryAfterPicture;
- (void) saveImgInFile;
- (void) saveImgInAlbum;
- (int)  setRotation : (int) degrees;

- (void) setCallbackOnNewFrame: (const std::function<void()>&) cb;
- (void) setCallbackExposureCompensation: (const std::function<void()>&) cb;
- (void) setCallbackLensPosition: (const std::function<void()>&) cb;
- (void) setCallbackSmoothZoom: (const std::function<void()>&) cb;
- (void) setCallbackAutoFocus: (const std::function<void(bool)>&) cb;
- (void) setCallbackTakePicture: (const std::function<void(int)>&) cb;
- (void) setCallbackISO: (const std::function<void()>&) cb;
- (void) setCallbackExposureDuration: (const std::function<void()>&) cb;
- (void) setCallbackISOAndED: (const std::function<void()>&) cb;
- (void) setCallbackStopVideo: (const std::function<void(int)>&) cb;

- (void) observeValueForKeyPath:(NSString*)keyPath ofObject:(id)object change:(NSDictionary*)change context:(void*)context;

-(void) dealloc;

//- (void)setupBuffers;
//- (void)setupGL;
//- (void)tearDownGL;
@end
