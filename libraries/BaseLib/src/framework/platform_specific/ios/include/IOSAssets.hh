// Objective C
#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <AssetsLibrary/AssetsLibrary.h>

// C++
#include <cmath>
#include <iostream>
#include <string>

@interface IOSAssets : NSObject<UINavigationControllerDelegate, UIImagePickerControllerDelegate, UIAlertViewDelegate>
{
    // member values declaration
}

+ (void) openAlbumInGallery: (const std::string&) albumName;
+ (void) imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info; // delegate method
+ (void) imagePickerControllerDidCancel:(UIImagePickerController *)picker; // delegate method

@end
