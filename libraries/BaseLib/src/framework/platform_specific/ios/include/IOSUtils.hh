// Objective C
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>

// FB imports

//#import <FBSDKCoreKit.h>

// C++
#include <cmath>
#include <iostream>
#include <string>

@interface IOSUtils : NSObject<UINavigationControllerDelegate, UIImagePickerControllerDelegate, UIAlertViewDelegate>
{
    // member values declaration
}

+ (void) openITunesURL: (const std::string&) developerName;
+ (void) openAppStoreURL: (const std::string&) developerName;
+ (void) openURL: (const std::string&) url;
+ (bool) checkCameraPermissions;
+ (bool) checkAudioPermissions;
+ (bool) showGoToSettingsAllert: (const std::string&) message : (const std::string&) okButtonText : (const std::string&) cancelButtonText;
+ (void) alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex;
+ (void) goToSettings;
+ (void) setSleepMode: (bool) value;

@end
