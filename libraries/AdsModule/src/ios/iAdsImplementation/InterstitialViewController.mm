
#import "InterstitialViewController.hh"
#import "../OverlayViewIOS.hh"
#import "../iAdsIos.hh"
#import <iAd/iAd.h>



@implementation InterstitialViewController
{
    ADInterstitialAd* _interstitial;
    BOOL _requestingAd;
    BOOL _showRighOnLoad;
    BOOL _adLoaded;
    BOOL _adShown;
    UIView* _adView;
    OverlayViewIOS* _overlay;
    

}

-(void) setOverlay : (OverlayViewIOS*) overlay {
    _overlay = overlay;
}


-(id) init {
    NSLog(@"Ad  init");
    
    self=[super init];
    
     _requestingAd=NO;
     _showRighOnLoad=NO;
     _adLoaded=NO;
     _adShown=NO;
     
    return self;
}

-(void) dealloc {
    NSLog(@"Ad Request dealloc");
    [super dealloc];
}

-(void) loadFullScreenAd {
if (_requestingAd == NO) {
        _showRighOnLoad = NO;
        NSLog(@"Ad load request");
        [self requestAd];
        
    } else {
        NSLog(@"Ad Request in progress!");
    }
}
    

-(void) showFullScreenAd {
    
    if (_adLoaded)
    {
        NSLog(@"showFullScreenAd displayAd");
        [self displayAd:_interstitial];
    }
    else
    {
        if(!_requestingAd)
        {
            NSLog(@"showFullScreenAd requestAd");
            [self requestAd];
            _showRighOnLoad=YES;
        }
        else{
            NSLog(@"showFullScreenAd _showRighOnLoad=yes");
            _showRighOnLoad=YES;
        }
    }
}

-(void) requestAd {
    
    if(!_requestingAd)
    {
        _adLoaded=NO;
        _interstitial = [[ADInterstitialAd alloc] init];
        _interstitial.delegate = self;
        //self.interstitialPresentationPolicy = ADInterstitialPresentationPolicyManual;
        //[self requestInterstitialAdPresentation];
        _requestingAd = YES;
        NSLog(@"requesting Ad");
    }
    
}

-(void) showFullScreenAdNew {

    self.interstitialPresentationPolicy = ADInterstitialPresentationPolicyManual;
    BOOL canPresent = [self requestInterstitialAdPresentation];
    NSLog(@"Ad Request in progress!(new api) ");
    NSLog(canPresent ? @"Yes" : @"No");
}

-(void) interstitialAdWillLoad : (ADInterstitialAd *) interstitialAd {

    NSLog(@"Ad WillLOAD");
}

-(void) interstitialAdDidLoad : (ADInterstitialAd *) interstitialAd {
    NSLog(@"Ad DidLOAD");
    if (interstitialAd.loaded) {
        if (_showRighOnLoad) {
            [self displayAd:interstitialAd];
            _adLoaded=YES;
        } else {
            _adLoaded=YES;
        }
    }
    
    [IadsCallback onLoadCallback];
    
}

-(void) displayAd : (ADInterstitialAd *) interstitialAd {

    CGRect interstitialFrame = self.view.bounds;
    interstitialFrame.origin = CGPointMake(0, 0);
    _adView = [[UIView alloc] initWithFrame : interstitialFrame];
    _adView.multipleTouchEnabled=YES;
    [_overlay addView : _adView];

    [_interstitial presentInView : _adView];

    UIButton* button = [UIButton buttonWithType : UIButtonTypeCustom];
    [button addTarget : self action : @selector(closeAd :) forControlEvents : UIControlEventTouchDown];
    button.backgroundColor = [UIColor clearColor];

    [button setBackgroundImage : [UIImage imageNamed : @"close_button.png"] forState : UIControlStateNormal];
    button.frame = CGRectMake(0, 0, 35, 35);
    [_adView addSubview : button];

    [UIView beginAnimations : @"animateAdBannerOn" context : nil];
    [UIView setAnimationDuration : 1];
    [_adView setAlpha : 1];
    [UIView commitAnimations];
    _adShown=YES;
    
}

-(void) closeAd : (id) sender {
    [self fadeOut];
}

-(void) fadeOut {
    NSLog(@"Ad close");
    [UIView beginAnimations : @"animateAdBannerOff" context : nil];
    [UIView setAnimationDuration : 1];
    [_adView setAlpha : 0];
    [UIView setAnimationDelegate : self];
    [UIView setAnimationDidStopSelector : @selector(closedAd:finished:context:)];
    [UIView commitAnimations];
}


-(void)closedAd:(NSString *) animationID finished: (NSNumber *) finished context: (void *) context{
    _adView = nil;
    _requestingAd = NO;
    _adLoaded=NO;
    _adShown=NO;
    [_interstitial release];
    [IadsCallback onCloseCallback];
    NSLog(@"Ad closedAd");
}

-(BOOL) interstitialAdActionShouldBegin : (ADInterstitialAd *) interstitialAd
willLeaveApplication : (BOOL) willLeave {
    NSLog(@"Ad willLeaveApplication");
    return willLeave;

}

-(void) interstitialAdDidUnload : (ADInterstitialAd *) interstitialAd {
    if (_adShown) {
        [self fadeOut];
    }
    
    NSLog(@"Ad DidUNLOAD");
}

-(void) interstitialAdActionDidFinish : (ADInterstitialAd *) interstitialAd {
    _requestingAd = NO;
    NSLog(@"Ad DidFINISH");
}

-(void) interstitialAd : (ADInterstitialAd *) interstitialAd didFailWithError : (NSError *) error {

    NSLog(@"interstitialAd <%@> recieved error <%@>", _interstitial, error);
    _requestingAd = NO;
    _adLoaded=NO;
    [_interstitial release];

}


@end