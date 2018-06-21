#include <aex/IShop.hpp>
#include <aex/IAPCallback.hpp>

#include "IAPWrapper.hh"


namespace aex {

    
    IShop::IShop()
    {
        
    }

    void IShop::buyProduct(const std::string& productName, float qaunity, IAPCallback* callback)
    {
        if(callback!=nullptr)
        {
            callback->setPurchuaseName(productName);
            IAPWrapper::GetInstance().setCallback(callback);
        }
        IAPWrapper::GetInstance().buyProductIdentifier(productName);
    }
    
    void IShop::restore(IAPCallback* callback)
    {
          
        IAPWrapper::GetInstance().restore(callback);
    }
    

}