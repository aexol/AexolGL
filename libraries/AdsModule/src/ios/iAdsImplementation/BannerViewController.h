#import <UIKit/UIKit.h>
#import <iAd/iAd.h>

@interface BannerViewController : UIViewController<ADBannerViewDelegate>
{
    ADBannerView *bannerView;
}
@end