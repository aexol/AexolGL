#include <aex/fb/FBLoginManager.hpp>
#include <aex/Logger.hpp>

// needed to have specific fb id placed in android-res or .plist file, described in README.txt

int main()
{
    aex::fb::FBLoginManager fbLoginManager;
    fbLoginManager.addPermission(aex::fb::PERMISSION::EMAIL);
    fbLoginManager.addPermission(aex::fb::PERMISSION::USER_FRIENDS);
    fbLoginManager.addPermission(aex::fb::PERMISSION::USER_LIKES);
    fbLoginManager.addPermission(aex::fb::PERMISSION::USER_POSTS);
    fbLoginManager.addPermission(aex::fb::PERMISSION::USER_ABOUT_ME);
    fbLoginManager.addPermission(aex::fb::PERMISSION::USER_VIDEOS);
    fbLoginManager.addPermission(aex::fb::PERMISSION::USER_WORK_HISTORY);
    fbLoginManager.addPermission(aex::fb::PERMISSION::USER_WEBSITE);
    fbLoginManager.addPermission(aex::fb::PERMISSION::USER_RELATIONSHIPS);

    fbLoginManager.login([this](const aex::fb::FBLoginResult& result){
      if(result.isSucceed())
      {
          LOG("aexfblib guisamples succeed");
      }
      if(result.isCanceled())
      {
          LOG("aexfblib guisamples cancelled");
      }
      if(result.isError())
      {
          LOG("aexfblib guisamples error: ", result.getErrorMsg());
      }

      std::vector<aex::fb::PERMISSION> declinedPerms = result.getDeclinedPermissions();
      for(int i = 0 ; i < declinedPerms.size() ; i++)
      {
          LOG("aexfblib guisamples next declined perm: ", aex::fb::permissionToString(declinedPerms[i]) );
      }

      std::vector<aex::fb::PERMISSION> grantedPerms = result.getGrantedPermissions();
      for(int i = 0 ; i < grantedPerms.size() ; i++)
      {
          LOG("aexfblib guisamples next granted perm: ", aex::fb::permissionToString(grantedPerms[i]) );
      }
    });
}
