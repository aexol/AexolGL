#include "aex/fb/FBLoginManager.hpp"
#include <algorithm>
#include <sstream>
#include <aex/Logger.hpp>

namespace aex
{
    namespace fb
    {
        FBLoginResult::FBLoginResult(
            int resultCode,
            const std::vector<PERMISSION>& declinedPerms,
            const std::vector<PERMISSION>& grantedPerms,
            const std::string& errorMsg) :
        FBResult(resultCode, errorMsg),
        m_declinedPerms(declinedPerms),
        m_grantedPerms(grantedPerms)
        {
        }

        FBLoginResult::~FBLoginResult()
        {
        }

        std::vector<PERMISSION> FBLoginResult::getDeclinedPermissions() const
        {
            return m_declinedPerms;
        }

        std::vector<PERMISSION> FBLoginResult::getGrantedPermissions() const
        {
            return m_grantedPerms;
        }

        FBLoginManager::FBLoginManager()
        {
        }

        FBLoginManager::~FBLoginManager()
        {
        }

        void FBLoginManager::login(const std::function<void(const FBLoginResult& result)> callback)
        {
            NativeCallback nativeCb = [callback](int code, std::string joinedDeclinedPerms, std::string joinedGrantedPerms, std::string errMsg){

                    std::vector<PERMISSION> declPerms;
                    std::vector<PERMISSION> grantedPerms;

                    std::vector<std::string> declPermsStrs = splitJoinedPermissions(joinedDeclinedPerms);
                    std::vector<std::string> grantedPermsStrs = splitJoinedPermissions(joinedGrantedPerms);

                    for(int i = 0 ; i < declPermsStrs.size() ; i++)
                    {
                        declPerms.push_back(stringToPermission(declPermsStrs[i]));
                    }

                    for(int i = 0 ; i < grantedPermsStrs.size() ; i++)
                    {
                        grantedPerms.push_back(stringToPermission(grantedPermsStrs[i]));
                    }

                    FBLoginResult logRes(
                        code,
                        declPerms,
                        grantedPerms,
                        errMsg
                    );
                    callback(logRes);
                };

            nativeLogin(nativeCb);
        }

        bool FBLoginManager::isLogged() const
        {
            return nativeIsLogged();
        }
    }
}
