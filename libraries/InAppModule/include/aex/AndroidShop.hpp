/* 
 * File:   AndroidShop.hpp
 * Author: mritke
 *
 * Created on 24 sierpnia 2015, 11:16
 */

#ifndef ANDROIDSHOP_HPP
#define	ANDROIDSHOP_HPP
#include <aex/ShopInterface.hpp>
#include <map>
#include <memory>
#include <mutex>

namespace aex{
    
    class AndroidShop : public ShopInterface{

    public:
        AndroidShop();
        void buyProduct(const std::string& productName, float qaunity =1.0f, IAPCallback* callback = nullptr);
        virtual void restore(IAPCallback* callback = nullptr){};
        void setDeveloperPayload(const std::string& payload);
        void setPublicKey(const std::string& key);
        
    private:
        
        bool m_canMakcepurchuase;
    };
    
    
}

#endif	/* ANDROIDSHOP_HPP */

