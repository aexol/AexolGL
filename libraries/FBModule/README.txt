# +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ USAGE +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# ======================================================================== Android ==========================================================
# In CMakeLists.txt file of your project add (change YOUR_PROJECT_NAME to correct value):
    find_package(AexolFB)
    include_directories(${AexolFB_INCLUDE_DIR})
    target_link_libraries(YOUR_PROJECT_NAME ${AexolFB_LIBRARIES})

# In build.gradle file be shure to have entry:
    repositories {
        mavenCentral()
    }

# And add to dependencies facebook library:
    dependencies {
        //...
    	compile 'com.aexol.android:aexlib:1.0'
        compile 'com.aexol.android:aexfblib:1.0'
        compile 'com.facebook.android:facebook-android-sdk:4.23.0'
        //...
}

# In file android-res/values/strings.xml add entry with correct id:
    <string name="facebook_app_id">PASTE_YOUR_APP_ID_FROM_FACEBOOK_DASHBOARD</string>
    <string name="fb_login_protocol_scheme">fbPASTE_YOUR_APP_ID_FROM_FACEBOOK_DASHBOARD</string>

# In Java main activity add linking libAexolFB.so library
    String[] mainLibrary = new String[] {"AexolFB","main"};

# In main code on app create add lines, invoke one time:
        aex::fb::FBAppEventsLogger fbEvents;
        fbEvents.activateApp();

# ======================================================================== iOS ==========================================================
# In CMakeLists.txt file of your project add (replace YOUR_APP_TARGET_NAME on correct value):
    find_package(AexolFB)
    include_directories(${AexolFB_INCLUDE_DIR})
    target_link_libraries(YOUR_APP_TARGET_NAME "-Wl,-force_load,${AexolFB_LIBRARIES}") # force_load is neccesary for method swizzling
    find_package(FacebookSDK)
    set_target_properties(YOUR_APP_TARGET_NAME
     PROPERTIES
     COMPILE_FLAGS "-F${FacebookSDK_INCLUDE_DIR}"
    )

# Add frameworks:
    FBSDKCoreKit
    FBSDKLoginKit

# For app identification add in plist or use CMake macro from AexlGL like in this example (remember to replace YOUR_FB_APP_ID on correct id from fb dashboard):
    SET(IPHONE_INFO_PLIST_USER_VARS "
        <key>CFBundleURLTypes</key>
          <array>
            <dict>
            <key>CFBundleURLSchemes</key>
            <array>
              <string>fbYOUR_FB_APP_ID</string>
            </array>
            </dict>
          </array>
          <key>FacebookAppID</key>
          <string>YOUR_FB_APP_ID</string>
          <key>FacebookDisplayName</key>
          <string>APP_DISPLAY_NAME</string>

          <key>LSApplicationQueriesSchemes</key>
          <array>
             <string>fbapi</string>
             <string>fb-messenger-api</string>
             <string>fbauth2</string>
             <string>fbshareextension</string>
          </array>

          <key>FacebookAutoLogAppEventsEnabled</key>
          <false/>
    ")

# In main code on app create add lines, invoke one time:
    aex::fb::FBAppEventsLogger fbEvents;
    fbEvents.activateApp();
