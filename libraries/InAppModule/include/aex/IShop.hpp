/* 
 * File:   IShop.hpp
 * Author: mritke
 *
 * Created on 19 sierpnia 2015, 16:35
 */

#ifndef ISHOP_HPP
#define	ISHOP_HPP

#include <string>
#include <memory>
#include <aex/ShopInterface.hpp>


namespace aex{
   
    class IShop : public ShopInterface
    {
    protected:
        
    public:
        IShop();
        
        void buyProduct(const std::string& productName, float qaunity=1.0f, IAPCallback* callback = nullptr);
        void restore(IAPCallback* callback = nullptr);
    };
    
}


#endif	/* ISHOP_HPP */

