/* 
 * File:   IAPCallback.hpp
 * Author: mritke
 *
 * Created on 31 sierpnia 2015, 13:04
 */

#ifndef IAPCALLBACK_HPP
#define	IAPCALLBACK_HPP

#include <string>
#include <vector>
#include <functional>

namespace aex {

    struct IAPItem {
        std::string identification = "";
        std::string name = "";
        std::string localizedTitle = "";
        std::string localizedDescription = "";
        float price = 0.0f;
    };

    class IAPCallback {

    public:
        virtual void productPurchased();
        virtual void productRestored();
        virtual void purchaseFailed();
        virtual void purchaseCanceled();
        
        template<class T>
        void setOnPurchasedFunc (T* obj, void (T::*memFn)(const std::string& purchuase));
        template<class T>
        void setOnRestoredFunc (T* obj, void (T::*memFn)(const std::string& purchuase));
        template<class T>
        void setOnFailedFunc (T* obj, void (T::*memFn)(const std::string& purchuase));
        template<class T>
        void setOnCanceledFunc (T* obj, void (T::*memFn)(const std::string& purchuase));
        void setPurchuaseName(std::string purchuaseName);
        std::string getPurchuaseName() const;
        
    private:
        IAPItem m_item;
        std::string m_purchuaseName = "";
        std::function<void(const std::string& purchuase)> m_productPurchased;
        std::function<void(const std::string& purchuase)> m_productRestored;
        std::function<void(const std::string& purchuase)> m_purchasedFailed;
        std::function<void(const std::string& purchuase)> m_purchasedCanceled;
        
    };

    template<class T>
    void IAPCallback::setOnPurchasedFunc (T* obj, void (T::*memFn)(const std::string& purchuase))
    {
        m_productPurchased=std::bind(memFn, obj, std::placeholders::_1);
    }
    
    template<class T>
    void IAPCallback::setOnRestoredFunc (T* obj, void (T::*memFn)(const std::string& purchuase))
    {
        m_productRestored=std::bind(memFn, obj, std::placeholders::_1);
    }
    
    template<class T>
    void IAPCallback::setOnFailedFunc (T* obj, void (T::*memFn)(const std::string& purchuase))
    {
        m_purchasedFailed=std::bind(memFn, obj, std::placeholders::_1);
    }
    
    template<class T>
    void IAPCallback::setOnCanceledFunc (T* obj, void (T::*memFn)(const std::string& purchuase))
    {
        m_purchasedCanceled=std::bind(memFn, obj, std::placeholders::_1);
    }
    
}


#endif	/* IAPCALLBACK_HPP */

