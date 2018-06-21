#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import <objc/objc.h>
#import <SDL.h>
#import <SDL_uikitappdelegate.h>

@interface SDLUIKitDelegate (FacebookEvents)

 -(void)swizzle_applicationDidBecomeActive:(UIApplication *)application;
 -(BOOL)swizzle_application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions;
 -(BOOL)swizzle_application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation;

@end
