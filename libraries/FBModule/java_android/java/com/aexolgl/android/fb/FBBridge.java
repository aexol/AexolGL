package com.aexolgl.android.fb;

import com.aexolgl.android.app.AexActivity;
import android.content.Intent;
import android.app.Activity;
import com.facebook.*;
import android.util.Log;
import org.json.*;
import android.os.Bundle;

public class FBBridge
{
    public static final int RESULT_SUCCEED = 1;
    public static final int RESULT_CANCELED = 2;
    public static final int RESULT_ERROR = 3;
    public static final String PERMISSION_EXTRA = "permissions";
    public static final String CPP_CALLBACK_ADDRESS_EXTRA = "cbAddress";
    public static final String FB_MODULE_TAG = "aexfblib";
    public FBBridge()
    {
    }

    public static void openLoginActivity(final String permissionsJoined, final long cppCallbackAddress)
    {
        ((Activity) AexActivity.getAppContext()).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                try {
                    Intent intent = new Intent((Activity)AexActivity.getAppContext(), FBLoginActivity.class);
                    intent.putExtra(PERMISSION_EXTRA, permissionsJoined);
                    intent.putExtra(CPP_CALLBACK_ADDRESS_EXTRA, cppCallbackAddress);
                    ((Activity) AexActivity.getAppContext()).startActivity(intent);
                } catch (android.content.ActivityNotFoundException anfe) {
                }
            }
        });
    }

    public static int isLogged()
    {
        if(AccessToken.getCurrentAccessToken() != null)
            return 1;

        return 0;
    }

    public static void graphRequestGET(final String graphPath, final String jsonStr, final long cppCallbackAddress)
    {
        Log.d(FBBridge.FB_MODULE_TAG, "graphPath: "+graphPath+", jsonStr: "+jsonStr+", addr:"+cppCallbackAddress);

        Bundle parameters = new Bundle();

        try{
            JSONObject jObject = new JSONObject(jsonStr);
            JSONArray names = jObject.names();
            for(int i = 0 ; i < names.length() ; i++)
            {
                String name = names.getString(i);
                String value = jObject.getString(name);
                parameters.putString(name, value);
            }
        }
        catch(JSONException exc)
        {
            Log.e(FBBridge.FB_MODULE_TAG, exc.toString());
        }

        GraphRequest request = new GraphRequest(AccessToken.getCurrentAccessToken(), graphPath, parameters, HttpMethod.GET, new GraphRequest.Callback(){
            @Override
            public void onCompleted(GraphResponse response) {
                String errMsg = "";
                int errCode = RESULT_SUCCEED;
                if(response.getError() != null)
                {
                    errCode = RESULT_ERROR;
                    errMsg = response.getError().toString();
                }
                // native cpp call
                fbRequestGETResultCallback(errCode, response.getJSONObject().toString(), errMsg, cppCallbackAddress);
            }
        });
        request.executeAsync();
    }

    public static void graphRequestDELETE(final String graphPath, final long cppCallbackAddress)
    {
        Log.d(FBBridge.FB_MODULE_TAG, "graphPath: "+graphPath+" addr:"+cppCallbackAddress);
        GraphRequest request = new GraphRequest(AccessToken.getCurrentAccessToken(), graphPath, null, HttpMethod.DELETE, new GraphRequest.Callback(){
            @Override
            public void onCompleted(GraphResponse response) {
                String errMsg = "";
                int errCode = 1; // true
                if(response.getError() != null)
                {
                    errCode = 0; // false
                    errMsg = response.getError().toString();
                }
                // native cpp call
                fbRequestDELETEResultCallback(errCode, errMsg, cppCallbackAddress);
            }
        });
        request.executeAsync();
    }

    public static native void fbLoginResultCallback(int code, String joinedDeclinedPerms, String joinedGrantedPerms, String errMsg, long cppCallbackAddress);
    public static native void fbRequestGETResultCallback(int code, String jsonResult, String errMsg, long cppCallbackAddress);
    public static native void fbRequestDELETEResultCallback(int trueOfFalse, String errMsg, long cppCallbackAddress);
}
