#import "SDL_uikitappdelegate_swizzling.h"
#import <FBSDKCoreKit/FBSDKCoreKit.h>
#import <FBSDKLoginKit/FBSDKLoginKit.h>

@implementation SDLUIKitDelegate (FacebookEvents)

// automatically invoked
+ (void)load {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        Class clazz = [self class];

        // # 1 - applicationDidBecomeActive swizzling
        SEL originalSelector1 = @selector(applicationDidBecomeActive:);
        SEL swizzledSelector1 = @selector(swizzle_applicationDidBecomeActive:);

        Method originalMethod1 = class_getInstanceMethod(clazz, originalSelector1);
        Method swizzledMethod1 = class_getInstanceMethod(clazz, swizzledSelector1);

        // When swizzling a class method, use the following:
        // Class class = object_getClass((id)self);
        // ...
        // Method originalMethod = class_getClassMethod(class, originalSelector);
        // Method swizzledMethod = class_getClassMethod(class, swizzledSelector);

        BOOL didAddMethod =
        class_addMethod(clazz,
                        originalSelector1,
                        method_getImplementation(swizzledMethod1),
                        method_getTypeEncoding(swizzledMethod1));

        if (didAddMethod) {
            class_replaceMethod(clazz,
                                swizzledSelector1,
                                method_getImplementation(originalMethod1),
                                method_getTypeEncoding(originalMethod1));
        } else {
            method_exchangeImplementations(originalMethod1, swizzledMethod1);
        }

        // # 2 - application:didFinishLaunchingWithOptions: swizzling
        SEL originalSelector2 = @selector(application:didFinishLaunchingWithOptions:);
        SEL swizzledSelector2 = @selector(swizzle_application:didFinishLaunchingWithOptions:);

        Method originalMethod2 = class_getInstanceMethod(clazz, originalSelector2);
        Method swizzledMethod2 = class_getInstanceMethod(clazz, swizzledSelector2);

        didAddMethod =
        class_addMethod(clazz,
                        originalSelector2,
                        method_getImplementation(swizzledMethod2),
                        method_getTypeEncoding(swizzledMethod2));

        if (didAddMethod) {
            class_replaceMethod(clazz,
                                swizzledSelector2,
                                method_getImplementation(originalMethod2),
                                method_getTypeEncoding(originalMethod2));
        } else {
            method_exchangeImplementations(originalMethod2, swizzledMethod2);
        }

        // # 3 - application:openURL:sourceApplication:annotation: swizzling
        SEL originalSelector3 = @selector(application:openURL:sourceApplication:annotation:);
        SEL swizzledSelector3 = @selector(swizzle_application:openURL:sourceApplication:annotation:);

        Method originalMethod3 = class_getInstanceMethod(clazz, originalSelector3);
        Method swizzledMethod3 = class_getInstanceMethod(clazz, swizzledSelector3);

        didAddMethod =
        class_addMethod(clazz,
                        originalSelector3,
                        method_getImplementation(swizzledMethod3),
                        method_getTypeEncoding(swizzledMethod3));

        if (didAddMethod) {
            class_replaceMethod(clazz,
                                swizzledSelector3,
                                method_getImplementation(originalMethod3),
                                method_getTypeEncoding(originalMethod3));
        } else {
            method_exchangeImplementations(originalMethod3, swizzledMethod3);
        }


    });
}

#pragma mark - Method Swizzling
-(void)swizzle_applicationDidBecomeActive:(UIApplication *)application
{
    NSLog(@"##### swizzle_applicationDidBecomeActive");
    [self swizzle_applicationDidBecomeActive:application]; // original method
    [FBSDKAppEvents activateApp];
}


-(BOOL) swizzle_application: (UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary*) launchOptions
{
    NSLog(@"##### swizzle_application:didFinishLaunchingWithOptions");
    [self swizzle_application:application didFinishLaunchingWithOptions:launchOptions]; // original method
    [[FBSDKApplicationDelegate sharedInstance] application:application
                                    didFinishLaunchingWithOptions:launchOptions];
    return YES;
}

-(BOOL) swizzle_application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
    NSLog(@"##### swizzle_application:openURL:sourceApplication...");
    [self swizzle_application:application openURL:url sourceApplication:sourceApplication annotation:annotation]; // original method
    return [[FBSDKApplicationDelegate sharedInstance] application:application
                                                          openURL:url
                                                sourceApplication:sourceApplication
                                                       annotation:annotation];
}


@end

//- (void)xxx_viewWillAppear:(BOOL)animated {
//    [self xxx_viewWillAppear:animated];
//    NSLog(@"viewWillAppear: %@", self);
//}
