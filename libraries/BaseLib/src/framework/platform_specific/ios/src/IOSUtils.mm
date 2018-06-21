#import "../include/IOSUtils.hh"

#include <aex/Logger.hpp>
#include <sstream>
#include <string>

@implementation IOSUtils

+ (void) openITunesURL: (const std::string&) developerName
{
    std::stringstream ss;
    //ss << "itms://itunes.com/apps/" << developerName;
    ss << developerName;
    std::string tmp = ss.str();
    NSString* iTunesLink = [NSString stringWithUTF8String:tmp.c_str()];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:iTunesLink]];
}

+ (void) openAppStoreURL: (const std::string&) developerName
{
  std::stringstream ss;
  ss << developerName;
  std::string tmp = ss.str();
  NSString* iTunesLink = [NSString stringWithUTF8String:tmp.c_str()];
  [[UIApplication sharedApplication] openURL:[NSURL URLWithString:iTunesLink]];
}

+ (void) openURL: (const std::string&) url
{
  std::stringstream ss;
  ss << url;
  std::string tmp = ss.str();
  NSString* link = [NSString stringWithUTF8String:tmp.c_str()];
  [[UIApplication sharedApplication] openURL:[NSURL URLWithString:link]];
}

+ (bool) checkCameraPermissions
{
    AVAuthorizationStatus status =  [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    if(status == AVAuthorizationStatusDenied || status == AVAuthorizationStatusRestricted)
    {
        return false;
    }
    return true;
}

+ (bool) checkAudioPermissions
{
    AVAuthorizationStatus status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeAudio];
    if(status == AVAuthorizationStatusDenied || status == AVAuthorizationStatusRestricted){
        return false;
    }
    return true;
}

static bool alertLock;
static bool alertApply;
+ (bool) showGoToSettingsAllert: (const std::string&) message : (const std::string&) okButtonText : (const std::string&) cancelButtonText
{
    NSString* nsstr = [NSString stringWithUTF8String : message.c_str()];
    NSString* okStr = [NSString stringWithUTF8String : okButtonText.c_str()];
    NSString* cancelStr = [NSString stringWithUTF8String : cancelButtonText.c_str()];

    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"" message:nsstr delegate:self cancelButtonTitle:okStr otherButtonTitles:cancelStr, nil];
    LOG_DEBUG("showing alert");
    [alert show];
    alertLock = true;
    while(alertLock){
     //[[NSRunLoop currentRunLoop] runUntilDate:[NSDate distantFuture]];
        [[NSRunLoop currentRunLoop] runUntilDate:[NSDate dateWithTimeIntervalSinceNow:0.5]];
        //LOG_DEBUG("wtf");
    }
    [alert release];
    return alertApply;
}

+ (void)alertView:(UIAlertView *)alertView clickedButtonAtIndex:(NSInteger)buttonIndex{
    if (buttonIndex == 0){ // Go to settings
        [IOSUtils goToSettings];
        alertApply = true;
    }else{
        alertApply = false;
    }
    alertLock = false;
}

+ (void) goToSettings
{
    if (&UIApplicationOpenSettingsURLString != NULL) { // if iOS >= 8.0
        NSURL *url = [NSURL URLWithString:UIApplicationOpenSettingsURLString];
        [[UIApplication sharedApplication] openURL:url];
    }
}

+ (void) setSleepMode: (bool) value
{
    if(value)
        [UIApplication sharedApplication].idleTimerDisabled = NO;
    else
        [UIApplication sharedApplication].idleTimerDisabled = YES;
}

@end
