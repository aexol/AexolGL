#import <UIKit/UIKit.h>
#import <iAd/iAd.h>

@interface InterstitialViewController : UIViewController <ADInterstitialAdDelegate>
-(void)showFullScreenAd;
-(void)loadFullScreenAd;
@end