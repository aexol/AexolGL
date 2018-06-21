/* 
 * File:   ShopHelper.hpp
 * Author: mritke
 *
 * Created on 31 lipca 2015, 14:01
 */

#ifndef SHOPHELPER_HPP
#define	SHOPHELPER_HPP
#include <aex/ShopInterface.hpp>

#include <string>
#include <vector>

enum Product_type {
    CONSUMABLE = 0,
    NONCONSUMABLE = 1,
    SUBSCRIPTON = 2
};

struct Product {
    std::string m_name = "";
    std::string m_ID = "";
    std::string m_productDescription = "";
    float m_displayPrice = 0.0f;
    Product_type m_type = Product_type::CONSUMABLE;
};

struct PurchuasedProduct {
    std::string m_product;
    std::string mOrderId;
    std::string mPackageName;
    std::string mSku;
    long mPurchaseTime;
    int mPurchaseState;
    std::string mDeveloperPayload;
    std::string mToken;
    std::string mOriginalJson;
    std::string mSignature;

};

class Shop {
    std::vector<Product> m_storeList;

    void LoadProductList() {

    }

};

class Inventory {
    
    std::vector<PurchuasedProduct> m_purchuaseList;
    std::vector<PurchuasedProduct> m_purchuaseHistory;
    
    
};

class ShopHelper {
private:


public:
    ShopHelper();

};

#endif	/* SHOPHELPER_HPP */

