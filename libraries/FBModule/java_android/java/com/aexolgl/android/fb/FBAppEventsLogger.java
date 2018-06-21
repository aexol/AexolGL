package com.aexolgl.android.fb;

import android.util.Log;

import com.facebook.FacebookSdk;
import com.facebook.appevents.AppEventsLogger;

import com.facebook.*;

import com.aexolgl.android.app.AexActivity;
import android.content.Context;
import android.app.Activity;
import android.app.Application;

public class FBAppEventsLogger
{
    public FBAppEventsLogger()
    {
    }

    public static void activateApp()
    {
        try{
            //FacebookSdk.sdkInitialize(AexActivity.getAppContext()); // SDK 4.18 and earlier
            AppEventsLogger.activateApp((Application)((Activity)AexActivity.getAppContext()).getApplication());
            Log.d(FBBridge.FB_MODULE_TAG, "FBAppEventsLogger activateApp(Application app)");
        }catch(Exception exc){
            Log.e(FBBridge.FB_MODULE_TAG, exc.getMessage());
        }
    }

    public static void deactivateApp()
    {
        // ========== not needed when use AppEventsLogger.activateApp(Application app); method =============
        // try{
        //     //FacebookSdk.sdkInitialize(AexActivity.getAppContext()); // SDK 4.18 and earlier
        //     AppEventsLogger.deactivateApp((Activity)AexActivity.getAppContext());
        //     Log.d(FBBridge.FB_MODULE_TAG, "FBAppEventsLogger deactivateApp()");
        // }catch(Exception exc){
        //     Log.e(FBBridge.FB_MODULE_TAG, exc.getMessage());
        // }
    }
}
