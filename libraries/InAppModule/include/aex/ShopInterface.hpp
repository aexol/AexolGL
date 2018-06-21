/* 
 * File:   ShopInterface.hpp
 * Author: mritke
 *
 * Created on 24 sierpnia 2015, 11:16
 */

#ifndef SHOPINTERFACE_HPP
#define	SHOPINTERFACE_HPP

#include <aex/IAPCallback.hpp>
namespace aex{

    class ShopInterface
    {
    protected:
        
    public: 
        virtual void buyProduct(const std::string& productName, float qaunity,IAPCallback* callback = nullptr)=0;
        virtual void restore(IAPCallback* callback = nullptr)=0;
    
    };

}
#endif	/* SHOPINTERFACE_HPP */

