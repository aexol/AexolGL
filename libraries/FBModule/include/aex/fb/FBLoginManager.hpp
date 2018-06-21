#ifndef FBLOGIN_AEX_APP_HEADER
#define FBLOGIN_AEX_APP_HEADER

#include <string>
#include <vector>
#include <functional>
#include "FBPermissions.hpp"
#include "FBResults.hpp"

namespace aex
{
    namespace fb
    {
        class FBLoginResult;
        class FBLoginManager;

        class FBLoginResult : public FBResult
        {
        public:
            friend class FBLoginManager;
        protected:
            FBLoginResult(
                int resultCode,
                const std::vector<PERMISSION>& declinedPerms,
                const std::vector<PERMISSION>& grantedPerms,
                const std::string& errorMsg);
        public:
            ~FBLoginResult();

            std::vector<PERMISSION> getDeclinedPermissions() const; // unselected by user
            std::vector<PERMISSION> getGrantedPermissions() const;
        protected:
            std::vector<PERMISSION> m_declinedPerms;
            std::vector<PERMISSION> m_grantedPerms;
        };

        class FBLoginManager : public FBPermissionsGroup
        {
        public:
            FBLoginManager();
            ~FBLoginManager();

            void login(const std::function<void(const FBLoginResult& result)> callback); // think about callbacks etc
            bool isLogged() const;
        public:
            using NativeCallback = std::function<void(int code, std::string declinedPerms, std::string grantedPerms, std::string errMsg)>;
        protected:
            void nativeLogin(const NativeCallback& nativeCb);
            bool nativeIsLogged() const;
        };
    }
}

#endif
