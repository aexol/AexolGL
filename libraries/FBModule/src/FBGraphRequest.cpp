#include "aex/fb/FBGraphRequest.hpp"
#include <sstream>
#include <json/json.h>
#include <aex/Logger.hpp>

namespace aex
{
    namespace fb
    {
        FBRequestResult::FBRequestResult(
            int resultCode,
            const std::string& jsonResult,
            const std::string& errMsg) :
        FBResult(resultCode, errMsg),
        m_jsonResults(jsonResult)
        {
            Json::Value root;
            Json::Reader reader;
            reader.parse(jsonResult, root);
            Json::Value::Members membersNames = root.getMemberNames();
            for(const auto& name : membersNames)
            {
                m_results[name] = root[name].asString();
            }
        }

        FBRequestResult::~FBRequestResult()
        {
        }

        std::string FBRequestResult::getParamValue(const std::string& key)
        {
            if(m_results.find(key) != m_results.end())
            {
                return m_results.at(key);
            }

            return "";
        }

        std::string FBRequestResult::getFullJsonResult() const
        {
            return m_jsonResults;
        }

        std::ostream& operator << (std::ostream& out, const FBRequestResult& result)
        {
            out << result.getFullJsonResult();
            // out << std::endl;
            // for(const auto& elem : result.m_results)
            // {
            //     out << elem.first << " : " << elem.second << std::endl;
            // }
            return out;
        }

        FBGraphRequest::FBGraphRequest()
        {
        }

        FBGraphRequest::~FBGraphRequest()
        {
        }

        void FBGraphRequest::requestGET(
            const std::string& graphPath,
            const std::map<std::string, std::string>& parameters,
            const std::function<void(const FBRequestResult& result)>& callback)
        {
            NativeCallback nativeCb
            =   [callback](int code, const std::string& jsonResults, std::string errMsg){
                    FBRequestResult reqResult(code, jsonResults, errMsg);
                    callback(reqResult);
                };

            nativeRequestGET(graphPath, parameters, nativeCb);
        }

        void FBGraphRequest::requestDELETE(const std::string& graphPath, const std::function<void(bool success, const std::string& errMsg)>& callback)
        {
            nativeRequestDELETE(graphPath, callback);
        }
    }
}
