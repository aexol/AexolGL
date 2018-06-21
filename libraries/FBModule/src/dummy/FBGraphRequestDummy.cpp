#include "aex/fb/FBGraphRequest.hpp"

namespace aex
{
    namespace fb
    {
        void FBGraphRequest::nativeRequestGET(
            const std::string& graphPath,
            const std::map<std::string,std::string>& parameters,
            const NativeCallback& natCB)
        {
        }

        void FBGraphRequest::nativeRequestDELETE(const std::string& graphPath, const std::function<void(bool success, const std::string& errMsg)>& callback)
        {
        }
    }
}
