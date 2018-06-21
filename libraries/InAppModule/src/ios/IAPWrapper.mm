#import "IAPWrapper.hh"
#import "IAPHelper_objc.hh"

IAPWrapper::IAPWrapper() {
    if (!m_initialized) {
        init();
    }
}

IAPWrapper::~IAPWrapper() {

}

void IAPWrapper::init() {

    
    if(!m_initialized)
    {
        m_sharedHelper=[[IAPHelper_objc alloc] init];
        
        m_initialized = true;
    }
}

bool IAPWrapper::canMakePayments() {
    return [[IAPHelper_objc sharedHelper] canMakePayments];
}

void IAPWrapper::requestProducts(aex::IAPCallback * callback) {
    [[IAPHelper_objc sharedHelper] requestProductsWithCallback : callback];
}

void IAPWrapper::setCallback(aex::IAPCallback* callback)
{
    if([(id)m_sharedHelper canMakePayments])
    {
        [(id)m_sharedHelper setCallback : callback];
    }
}

void IAPWrapper::restore(aex::IAPCallback* callback)
{
    if(callback!=nullptr)
        { 
            if([(id)m_sharedHelper canMakePayments])
            {
                [(id)m_sharedHelper setCallback : callback];
            }

           [(id)m_sharedHelper restoreTransactions];
        }
}

void IAPWrapper::buyProductIdentifier(const std::string & productID) {

    NSLog(@"buyProductIdentifier");
    NSString *nsID = [NSString stringWithCString : productID.c_str()
            encoding : [NSString defaultCStringEncoding]];

    if([(id)m_sharedHelper canMakePayments])
    {
        [(id)m_sharedHelper buyProductIdentifier : nsID];
    }
    
}

std::unique_ptr<IAPWrapper>& IAPWrapperInstance() {
    static std::unique_ptr<IAPWrapper> instance_;
    return instance_;
}

IAPWrapper &
IAPWrapper::GetInstance() {
    static std::once_flag only_one;

    std::call_once(only_one,
            []() {
                IAPWrapperInstance().reset(new IAPWrapper());
            }
    );

    return *IAPWrapperInstance();
}