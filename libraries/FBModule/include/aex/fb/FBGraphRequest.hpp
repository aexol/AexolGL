#ifndef FB_GRAPH_REQUEST_AEX_HEADER_FILE
#define FB_GRAPH_REQUEST_AEX_HEADER_FILE

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include "aex/fb/FBPermissions.hpp"
#include "aex/fb/FBResults.hpp"

namespace aex
{
    namespace fb
    {
        class FBRequestResult;
        class FBGraphRequest;

        class FBRequestResult : public FBResult
        {
        public:
            friend class FBGraphRequest;
        public:
            FBRequestResult(
                int resultCode,
                const std::string& jsonResult,
                const std::string& errMsg);
            ~FBRequestResult();

            std::string getParamValue(const std::string& key);
            std::string getFullJsonResult() const;

            friend std::ostream& operator << (std::ostream& out, const FBRequestResult& result);
        protected:
            std::string m_jsonResults;
            std::map<std::string, std::string> m_results;
        };
        std::ostream& operator << (std::ostream& out, const FBRequestResult& result);

        class FBGraphRequest
        {
        public:
        public:
            FBGraphRequest();
            ~FBGraphRequest();

            void requestGET(
                const std::string& graphPath,
                const std::map<std::string,std::string>& parameters,
                const std::function<void(const FBRequestResult& result)>& callback);
            void requestDELETE(const std::string& graphPath, const std::function<void(bool success, const std::string& errMsg)>& callback);
        public:
            using NativeCallback = std::function<void(int code, const std::string& jsonResult, std::string errMsg)>;
        protected:
            void nativeRequestGET(
                const std::string& graphPath,
                const std::map<std::string,std::string>& parameters,
                const NativeCallback& natCB
            );
            void nativeRequestDELETE(const std::string& graphPath, const std::function<void(bool success, const std::string& errMsg)>& callback);
        };
    }
}


#endif
