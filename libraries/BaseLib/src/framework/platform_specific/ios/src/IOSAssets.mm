#import "../include/IOSAssets.hh"

#include <aex/Logger.hpp>
#include <sstream>
#include <string>

@implementation IOSAssets

+ (void) openAlbumInGallery: (const std::string&) albumName
{
    UIViewController * rootViewController = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    UIImagePickerController *picker = [[UIImagePickerController alloc] init];
    picker.delegate = self;
    picker.allowsEditing = YES;

    // Choose on of two following:
    //picker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    picker.sourceType = UIImagePickerControllerSourceTypeSavedPhotosAlbum;
    [rootViewController presentViewController:picker animated:YES completion:NULL];
}


// delegate method
+ (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    // UIImage *chosenImage = info[UIImagePickerControllerEditedImage];
    [picker dismissViewControllerAnimated:YES completion:NULL];
    LOG_DEBUG("IMAGE PICKED ACTION");
}

+ (void) imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [picker dismissViewControllerAnimated:YES completion:NULL];
    LOG_DEBUG("DID CANCEL METHOD FROM DELEGATE");
}


+ (bool) checkPhotoPermissions
{
    ALAuthorizationStatus status = [ALAssetsLibrary authorizationStatus];
    if (status != ALAuthorizationStatusAuthorized) {
        return false;
    }
    return true;
}


@end
