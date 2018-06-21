#import "IAPHelper_objc.hh"


@implementation IAPHelper_objc

@synthesize productIdentifiers = _productIdentifiers;
@synthesize products = _products;
@synthesize purchasedProducts = _purchasedProducts;
@synthesize request = _request;

- (id)initWithProductIdentifiers:(NSSet *)productIdentifiers{
    latestIAPCallback = nil;

    if ((self = [super init])) {
        _productIdentifiers = [productIdentifiers retain];

        // Check for previously purchased products
        NSMutableSet * purchasedProducts = [NSMutableSet set];
        for (NSString * productIdentifier in _productIdentifiers) {
            BOOL productPurchased = [[NSUserDefaults standardUserDefaults] boolForKey:productIdentifier];
            if (productPurchased) {
                [purchasedProducts addObject:productIdentifier];
                NSLog(@"Previously purchased: %@", productIdentifier);
            }
            NSLog(@"Not purchased: %@", productIdentifier);
        }
        self.purchasedProducts = purchasedProducts;

    }
    return self;
}

-(id)init{
    
    latestIAPCallback = nil;
    if ((self = [super init])) {
        [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
        [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(productPurchased:) name:kProductPurchasedNotification object:nil];
        [[NSNotificationCenter defaultCenter] addObserver:self selector: @selector(productPurchaseFailed:) name:kProductPurchaseFailedNotification object: nil];
    }
    return self;
    
}

- (void)setCallback:(aex::IAPCallback*)callback
{
    latestIAPCallback=callback;
}

- (bool)canMakePayments
{
    return [SKPaymentQueue canMakePayments];
}

- (void)requestProductsWithCallback:(aex::IAPCallback*)callback
{
    latestIAPCallback = callback;
    [self requestProducts];
}

- (void)requestProducts {
    self.request = [[[SKProductsRequest alloc] initWithProductIdentifiers:_productIdentifiers] autorelease];
    _request.delegate = self;
    [_request start];

}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {

    NSLog(@"Received products results...");   
    self.products = response.products;
    self.request = nil;    

    //[[NSNotificationCenter defaultCenter] postNotificationName:kProductsLoadedNotification object:_products];  
}

- (void)productsLoaded:(NSNotification *)notification {
    [NSObject cancelPreviousPerformRequestsWithTarget:self]; 
}

- (void)recordTransaction:(SKPaymentTransaction *)transaction {    
    // Optional: Record the transaction on the server side...    
}

- (void)provideContent:(NSString *)productIdentifier {

    NSLog(@"Toggling flag for: %@", productIdentifier);
    [[NSUserDefaults standardUserDefaults] setBool:TRUE forKey:productIdentifier];
    [[NSUserDefaults standardUserDefaults] synchronize];
    [_purchasedProducts addObject:productIdentifier];

    //[[NSNotificationCenter defaultCenter] postNotificationName:kProductPurchasedNotification object:productIdentifier];

}

- (void)completeTransaction:(SKPaymentTransaction *)transaction {

    NSLog(@"completeTransaction...");

    [self recordTransaction: transaction];
    [self provideContent: transaction.payment.productIdentifier];
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
    if(latestIAPCallback!=nil)
    {
        latestIAPCallback->productPurchased();
    }

}

- (void)restoreTransaction:(SKPaymentTransaction *)transaction {

    NSLog(@"restoreTransaction...");

    [self recordTransaction: transaction];
    [self provideContent: transaction.originalTransaction.payment.productIdentifier];
    [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
    
    if(latestIAPCallback!=nil)
    {
        latestIAPCallback->productRestored();
    }
    

}

- (void)failedTransaction:(SKPaymentTransaction *)transaction {

     NSLog(@"Transaction failed");
    if (transaction.error.code != SKErrorPaymentCancelled)
    {
        if(latestIAPCallback!=nil)
        {
            latestIAPCallback->purchaseFailed();
        }
        NSLog(@"Transaction error: %@", transaction.error.localizedDescription);
    }
    else
    {//[[NSNotificationCenter defaultCenter] postNotificationName:kProductPurchaseFailedNotification object:transaction];

        [[SKPaymentQueue defaultQueue] finishTransaction: transaction];
        if(latestIAPCallback!=nil)
        {
            latestIAPCallback->purchaseCanceled();
        }
        NSLog(@"Transaction cancelled");
    }
    


}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
    
    NSLog(@"paymentQueue");
    for (SKPaymentTransaction *transaction in transactions)
    {
        switch (transaction.transactionState)
        {
            case SKPaymentTransactionStatePurchased:
                [self completeTransaction:transaction];
                break;
            case SKPaymentTransactionStateFailed:
                [self failedTransaction:transaction];
                break;
            case SKPaymentTransactionStateRestored:
                [self restoreTransaction:transaction];
            default:
                break;
        }
    }
}

- (void)buyProductIdentifier:(NSString *)productIdentifier   {

    NSLog(@"Buying %@...", productIdentifier);

    SKPayment *payment = [SKPayment paymentWithProductIdentifier:productIdentifier];
    
    [[SKPaymentQueue defaultQueue] addPayment:payment];  
            
}

- (void)restoreTransactions
{
    
    NSLog(@"Restoring...");
    [[SKPaymentQueue defaultQueue]restoreCompletedTransactions];
}

            
- (void)dealloc
{
    [_productIdentifiers release];
    _productIdentifiers = nil;
    [_products release];
    _products = nil;
    [_purchasedProducts release];
    _purchasedProducts = nil;
    [_request release];
    _request = nil;
    [super dealloc];
}

@end
        
