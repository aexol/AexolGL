#include <aex/fb/FBAppEventsLogger.hpp>

// needed to have specific fb id placed in android-res or .plist file, described in README.txt

int main()
{
    aex::fb::FBAppEventsLogger fbEvents;
    fbEvents.activateApp();
}
