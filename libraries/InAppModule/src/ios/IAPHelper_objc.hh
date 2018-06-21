#import <Foundation/Foundation.h>
#import "StoreKit/StoreKit.h"
#include <aex/IAPCallback.hpp>

#define kProductsLoadedNotification         @"ProductsLoaded"
#define kProductPurchasedNotification       @"ProductPurchased"
#define kProductPurchaseFailedNotification  @"ProductPurchaseFailed"

@interface IAPHelper_objc : NSObject  {
    NSSet * _productIdentifiers;    
    NSArray * _products;
    NSMutableSet * _purchasedProducts;
    SKProductsRequest * _request;
    aex::IAPCallback* latestIAPCallback;
}

@property (retain) NSSet *productIdentifiers;
@property (retain) NSArray * products;
@property (retain) NSMutableSet *purchasedProducts;
@property (retain) SKProductsRequest *request;

- (void)buyProductIdentifier:(NSString *)productIdentifier;
- (void)requestProducts;
- (id)initWithProductIdentifiers:(NSSet *)productIdentifiers;
- (bool)canMakePayments;
- (void)requestProductsWithCallback:(aex::IAPCallback*)callback;
- (void)restoreTransactions;
- (void)setCallback:(aex::IAPCallback*)callback;
@end