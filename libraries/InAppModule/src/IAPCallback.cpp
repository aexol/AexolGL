#include <aex/IAPCallback.hpp>


namespace aex {

    void IAPCallback::setPurchuaseName(std::string purchuaseName) {
        m_purchuaseName = purchuaseName;
    }

    std::string IAPCallback::getPurchuaseName() const {
        return m_purchuaseName;
    }
    
    void IAPCallback::productPurchased() {

        if (m_productPurchased) {
            m_productPurchased(m_purchuaseName);
        }

    }

    void IAPCallback::productRestored() {
        if (m_productRestored) {
            m_productRestored(m_purchuaseName);
        }

    }

    void IAPCallback::purchaseFailed() {
        if (m_purchasedFailed) {
            m_purchasedFailed(m_purchuaseName);
        }

    }

    void IAPCallback::purchaseCanceled() {
        if (m_purchasedCanceled) {
            m_purchasedCanceled(m_purchuaseName);
        }
    }
}
