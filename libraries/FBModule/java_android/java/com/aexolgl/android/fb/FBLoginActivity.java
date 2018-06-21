package com.aexolgl.android.fb;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import com.aexolgl.android.app.AexActivity;
import com.facebook.*;
import com.facebook.login.*;
//import com.facebook.login.widget.LoginButton;
import java.util.Arrays;
import java.util.List;
import java.util.ArrayList;
import java.util.Set;
import android.support.v4.app.FragmentActivity;
import java.lang.StringBuilder;

public class FBLoginActivity  extends FragmentActivity {


    protected CallbackManager callbackManager = null;
    protected List<String> permList = new ArrayList<String>();
    protected long cppCallbackAddress = 0;
    @Override
       protected void onCreate(Bundle savedInstanceState) {
           super.onCreate(savedInstanceState);

           String permissions;
           Bundle bundle = getIntent().getExtras();
           if (bundle != null) {
              permissions = bundle.getString(FBBridge.PERMISSION_EXTRA);
              cppCallbackAddress = bundle.getLong(FBBridge.CPP_CALLBACK_ADDRESS_EXTRA);
              Log.d(FBBridge.FB_MODULE_TAG, "permissions list: "+permissions);
              String[] splited = permissions.split("\\s+");
              permList  = Arrays.asList(splited);
           }
           FacebookSdk.sdkInitialize(AexActivity.getAppContext()); // SDK 4.18 and earlier
           setUpFbLoginBtn();
       }

       @Override
       public void onBackPressed() {
           super.onBackPressed();
       }

       private void setUpFbLoginBtn() {
           final long cbAddressFinal = cppCallbackAddress;
           callbackManager = CallbackManager.Factory.create();
           LoginManager.getInstance().registerCallback(callbackManager, new FacebookCallback<LoginResult>() {
               @Override
               public void onSuccess(LoginResult loginResult) {
                   StringBuilder sb = new StringBuilder();
                   Set<String> declPermsSet = loginResult.getRecentlyDeniedPermissions();
                   if(declPermsSet != null)
                   {
                       Log.d(FBBridge.FB_MODULE_TAG, "DECLINED SIZE: "+declPermsSet.size());
                       for(String s : declPermsSet)
                       {
                           sb.append(s);
                           sb.append(' ');
                       }
                   }
                   String joinedDeclinedPerms = sb.toString().trim();

                   // devlined :
                   StringBuilder sb2 = new StringBuilder();
                   Set<String> grantedPermsSet = loginResult.getRecentlyGrantedPermissions();
                   if(grantedPermsSet != null)
                   {
                       Log.d(FBBridge.FB_MODULE_TAG, "GRANTED SIZE: "+grantedPermsSet.size());
                       for(String s : grantedPermsSet)
                       {
                           sb2.append(s);
                           sb2.append(' ');
                       }
                   }
                   String joinedGrantedPerms = sb2.toString().trim();

                   FBBridge.fbLoginResultCallback(FBBridge.RESULT_SUCCEED, joinedDeclinedPerms, joinedGrantedPerms, "", cbAddressFinal);
                   onBackPressed();
               }

               @Override
               public void onCancel() {
                   FBBridge.fbLoginResultCallback(FBBridge.RESULT_CANCELED, "", "", "", cbAddressFinal);
                   onBackPressed();
               }

               @Override
               public void onError(FacebookException error) {
                   FBBridge.fbLoginResultCallback(FBBridge.RESULT_ERROR,"", "", error.toString(), cbAddressFinal);
                   onBackPressed();
               }
           });
           LoginManager.getInstance().logInWithReadPermissions(FBLoginActivity.this, permList);
       }

       @Override
       protected void onActivityResult(int requestCode, int resultCode, Intent data) {
           super.onActivityResult(requestCode, resultCode, data);
           callbackManager.onActivityResult(requestCode, resultCode, data);
       }
}
