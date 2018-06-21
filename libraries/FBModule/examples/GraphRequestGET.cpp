#include <aex/fb/FBLoginManager.hpp>
#include <aex/fb/FBGraphRequest.hpp>
#include <aex/Logger.hpp>

// needed to have specific fb id placed in android-res or .plist file, described in README.txt

int main()
{

    ThreadedTasks m_threadedTask;
    aex::fb::FBLoginManager fbLoginManager;
    fbLoginManager.addPermission(aex::fb::PERMISSION::EMAIL);

    // be shure to be logged in using FBLoginManager before use FBGraphRequest object
    fbLoginManager.login([&m_threadedTask](const aex::fb::FBLoginResult& result){
      if(result.isSucceed())
      {
          LOG("aexfblib guisamples succeed");
          // if email perm granted and is shure to be login and have accessToken valid:
          if(std::find(grantedPerms.begin(), grantedPerms.end(), aex::fb::PERMISSION::EMAIL ) != grantedPerms.end())
          {
              m_threadedTask.pushTask([&m_threadedTask](){ // this object always run tasks on thread, where is added to ALoop (gui thread needed here).
                  std::string graphPath;
                  std::map<std::string, std::string> params;
                  graphPath = "me"; // docs in Facebook SDK developers.facebook graphAPI
                  params["fields"] = "id,email,name"; // etc // docs in Facebook SDK developers.facebook graphAPI

                  aex::fb::FBGraphRequest request;
                  request.requestGET(graphPath, params, [this](const aex::fb::FBRequestResult& result){
                      LOG("aexfblib Request Result: ", result);
                  });
              });
          }
        });
      }
    });

    // m_threadedTasks needed in ALoopNode in this example.
    // you can do graph request not in login callback, but be shure to invoke fbLoginManager.login method before and then just:
    std::string graphPath;
    std::map<std::string, std::string> params;
    graphPath = "me"; // docs in Facebook SDK developers.facebook graphAPI
    params["fields"] = "id,email,name"; // etc, // docs in Facebook SDK developers.facebook graphAPI

    aex::fb::FBGraphRequest request;
    request.requestGET(graphPath, params, [this](const aex::fb::FBRequestResult& result){
        LOG("aexfblib Request Result: ", result);
    });


}
