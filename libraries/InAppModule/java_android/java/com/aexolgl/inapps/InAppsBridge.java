package com.aexolgl.inapps;

import android.app.Activity;
import com.android.vending.billing.IInAppBillingService;
import android.os.Bundle;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;
import android.util.Log;
import android.content.ServiceConnection;
import android.content.ComponentName;
import android.os.IBinder;
import com.aexolgl.inapps.*;
import android.content.Intent;


public class InAppsBridge {

    private static Activity mActivity;
    public static IabHelper mIabHelper;

    public static String mDeveloperPayload = "";
    public static String mbase64EncodedPublicKey = "";
    public static String TAG = "AexolInApp";
    public static Inventory mInventory;
    static final int RC_REQUEST = 10001;

    public static void init(Activity activity) {
        try {
            mActivity = activity;

            String base64EncodedPublicKey = "CONSTRUCT_YOUR_KEY_AND_PLACE_IT_HERE";

            mIabHelper = new IabHelper(mActivity, base64EncodedPublicKey);
            mIabHelper.enableDebugLogging(true);

            mIabHelper.startSetup(new IabHelper.OnIabSetupFinishedListener() {
                public void onIabSetupFinished(IabResult result) {
                    Log.d("AexolInApp", "Setup finished.");

                    if (!result.isSuccess()) {
                        // Oh noes, there was a problem.
                        Log.v("AexolInApp", "Problem setting up in-app billing: " + result);
                        //complain("Problem setting up in-app billing: " + result);
                        return;
                    }

                    // Have we been disposed of in the meantime? If so, quit.
                    if (mIabHelper == null) {
                        return;
                    }

                    // IAB is fully set up. Now, let's get an inventory of stuff we own.
                    Log.d("AexolInApp", "Setup successful. Querying inventory.");
                    mIabHelper.queryInventoryAsync(mGotInventoryListener);
                }
            });

            
        } catch (Exception e) {
            Log.v("AexolInApp", e.toString());
        }
    }

    public static void onDestroy() {

        Log.d("AexolInApp", "Destroying helper.");
        if (mIabHelper != null) {
            mIabHelper.dispose();
            mIabHelper = null;
        }
    }

    public static IabHelper.QueryInventoryFinishedListener mGotInventoryListener = new IabHelper.QueryInventoryFinishedListener() {
        public void onQueryInventoryFinished(IabResult result, Inventory inventory) {
            Log.d(TAG, "Query inventory finished.");

            // Have we been disposed of in the meantime? If so, quit.
            if (mIabHelper == null) {
                return;
            }

            // Is it a failure?
            if (result.isFailure()) {
                Log.v(TAG, "Failed to query inventory: " + result);
                return;
            }
            mInventory = inventory;
        }
    };

    public static IabHelper.OnIabPurchaseFinishedListener mPurchaseFinishedListener = new IabHelper.OnIabPurchaseFinishedListener() {
        public void onIabPurchaseFinished(IabResult result, Purchase purchase) {
            Log.d(TAG, "Purchase finished: " + result + ", purchase: " + purchase + " sku: ");

            
            if(result.getResponse()== IabHelper.IABHELPER_USER_CANCELLED)
            {
                purchaseCanceledNative("");
            }
            // if we were disposed of in the meantime, quit.
            if (mIabHelper == null) {
                Log.v(TAG, "Error purchasing: mIabHelper == null" + result);
                purchaseFailedNative("");
                return;
            }
            
            
            if (result.isFailure()) {
                Log.v(TAG, "Error purchasing: " + result);
                purchaseFailedNative("");
                return;
            }
            /* if (!verifyDeveloperPayload(purchase)) {
             Log.v(TAG, "Error purchasing. Authenticity verification failed.");
             return;
             }*/
            Log.v(TAG, "Purchase successful.");
            mInventory.addPurchase(purchase);
            
            productPurchasedNative(purchase.mSku);
            
        }
    };

    IabHelper.OnConsumeFinishedListener mConsumeFinishedListener = new IabHelper.OnConsumeFinishedListener() {
        public void onConsumeFinished(Purchase purchase, IabResult result) {
            Log.d(TAG, "Consumption finished. Purchase: " + purchase + ", result: " + result);

            // if we were disposed of in the meantime, quit.
            if (mIabHelper == null) {
                
                return;
            }

            // We know this is the "gas" sku because it's the only one we consume,
            // so we don't check which sku was consumed. If you have more than one
            // sku, you probably should check...
            if (result.isSuccess()) {
                
                // successfully consumed, so we apply the effects of the item in our
                // game world's logic, which in our case means filling the gas tank a bit
                Log.v(TAG, "Consumption successful. Provisioning.");
            } else {
                Log.v(TAG, "Error while consuming: " + result);
    
            }
           
            mInventory.erasePurchase(purchase.getSku());
            
            Log.v(TAG, "End consumption flow.");
        }
    };

    public static void purchuaseProduct(String Product, float quanity) {
        try{
        mIabHelper.launchPurchaseFlow(mActivity, Product, RC_REQUEST,
                mPurchaseFinishedListener, mDeveloperPayload);
        }catch (Exception e)
                {
                    mIabHelper.flagEndAsync();
                    Log.v(TAG, "fail on purchuaseProduct: " + e.toString());
                }
    }

    public static void queryPurchuases() {
        mIabHelper.queryInventoryAsync(mGotInventoryListener);
    }

    public static boolean handleActivityResult(int requestCode, int resultCode, Intent data) {
        if (mIabHelper!=null) {
             Log.v(TAG, "handleActivityResultBridge");
            return mIabHelper.handleActivityResult(requestCode, resultCode, data);
        } else {
            Log.v(TAG, "handleActivityResult  mIabHelper is null");
            return false;
        }
    }

    public static void sucscribeProduct(String Product, float time) {

    }

    public static void consumeProduct() {

    }

    public static void getUserId() {

    }
    
    public static void updateNativeInventory() {

    }

    public static void setUserId() {

    }

    public static void getProductList() {

    }

    public static void setDeveloperPayload(String payload) {
        mDeveloperPayload = payload;
    }

    public static void setPublicKey(String key) {
        mbase64EncodedPublicKey = key;
        mIabHelper.setSignatureKey(key);
    }

    ///////////////////////////////////////NATIVE
     public static native void addProductToList();
     public static native void productPurchasedNative(String name);
     public static native void productRestoredNative(String name);
     public static native void purchaseFailedNative(String name);
     public static native void purchaseCanceledNative(String name);
}
