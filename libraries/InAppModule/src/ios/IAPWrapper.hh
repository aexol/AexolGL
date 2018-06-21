/* 
 * File:   IAPWrapper.h
 * Author: mritke
 *
 * Created on 19 sierpnia 2015, 13:23
 */


#ifndef xxx_IAPWrapper_h
#define xxx_IAPWrapper_h
#include <memory>
#include <mutex>
//#import "IAp.h"
#import <aex/IAPCallback.hpp>

class IAPWrapper {
private:

    void* m_sharedHelper = nullptr;
    bool m_initialized = false;
    void init();

    IAPWrapper();
    IAPWrapper(const IAPWrapper& rs) = delete;
    IAPWrapper& operator=(const IAPWrapper& rs) = delete;
    
public:
    // Some users might have IAP disabled
    static IAPWrapper& GetInstance();
    ~IAPWrapper();


    bool canMakePayments();

    void setCallback(aex::IAPCallback* callback);
    
    void requestProducts(aex::IAPCallback* callback);
    
    void restore(aex::IAPCallback* callback);

    void buyProductIdentifier(const std::string& productID);

};

#endif