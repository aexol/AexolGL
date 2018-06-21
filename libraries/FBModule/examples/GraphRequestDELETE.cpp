#include <aex/fb/FBLoginManager.hpp>
#include <aex/fb/FBGraphRequest.hpp>
#include <aex/Logger.hpp>

// needed to have specific fb id placed in android-res or .plist file, described in README.txt

int main()
{

    // be shure to be logged in using FBLoginManager before use FBGraphRequest object
    std::string graphPath  = "me/permissions"; // docs in Facebook SDK developers.facebook graphAPI
    aex::fb::FBGraphRequest request;
    request.requestDELETE(graphPath, [this](bool success, const std::string& errMsg){
        if(success)
        {
            LOG("aexfblib removed succesfully");
        }else
        {
            LOG("aexfblib error: ", errMsg);
        }
    });
    // m_threadedTasks needed in ALoopNode,
}
