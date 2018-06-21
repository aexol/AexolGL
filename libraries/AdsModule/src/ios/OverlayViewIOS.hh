#include <SDL.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>


@interface OverlayViewIOS :
    NSObject
    @property(readonly) UIView* view;

-(id)init;
-(void)addView:
    (UIView*)subview;
@end
