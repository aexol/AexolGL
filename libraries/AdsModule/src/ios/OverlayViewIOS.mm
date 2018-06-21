#import "OverlayViewIOS.hh"


#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import <objc/runtime.h>
#import <objc/objc.h>
#import <SDL.h>

@implementation OverlayViewIOS
BOOL _initialized;
UIView* _view;
@synthesize view = _view;


-(id)init
{
    self = [super init];
    _initialized = NO;
    [self lazyInit];
    return self;
}

-(void)lazyInit
{
    if(!_initialized)
    {
        SDL_Rect r;
        if(SDL_GetDisplayUsableBounds(0, &r) != 0) {
            // TODO: Error logging.
            return;
        }
        _view = [[UIView alloc] initWithFrame:CGRectMake(r.x, r.y, r.w, r.h)];
        self.view.backgroundColor = [[UIColor redColor] colorWithAlphaComponent:0.0f];
        self.view.multipleTouchEnabled=YES;
        UIViewController *vc = [[[UIApplication sharedApplication] keyWindow] rootViewController];
        [vc.view addSubview:self.view];
        _initialized=YES;
    }
}

-(void)addView:(UIView*)subview
{
    [self lazyInit];
    [self.view addSubview:subview];
}

@end
