package com.aexolgl.android.ads;
import android.app.Activity;
import android.content.Context;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.provider.Settings;
import android.util.Log;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.FrameLayout;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.InterstitialAd;

import java.security.MessageDigest;
import java.util.ArrayList;

class AdMobBanner extends FrameLayout{ 

    private AdView adView;

    public AdMobBanner(Context context) {
        super(context);
    }
    public AdMobBanner(Context context, String uId) {
        this(context);
        adView = new AdView(context);
        FrameLayout.LayoutParams layoutParams = new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        layoutParams.gravity = Gravity.CENTER;
        adView.setLayoutParams(layoutParams);
        adView.setAdSize(AdSize.BANNER);
        setAdUnitId(uId);
        addView(adView);
    }

    public void showAdvert(boolean isTest, String testDevId, ArrayList<String> testIdList) {
        AdRequest.Builder builder = new AdRequest.Builder();

        if (isTest) {

            builder.addTestDevice(AdRequest.DEVICE_ID_EMULATOR);
            if (testDevId != null) {
                builder.addTestDevice(testDevId);
            }

            if (testIdList != null) {
                for (int i = 0; i < testIdList.size(); i++) {
                    builder.addTestDevice(testIdList.get(i));
                }
            }
        }

        adView.loadAd(builder.build());
    }

    public void pause() {
        adView.pause();
    }

    public void resume() {
        adView.resume();
    }

    public void setAdUnitId(String id) {
        adView.setAdUnitId(id);
    }
}

@SuppressWarnings("unused")
public class AdMobHandler extends Activity {

    private static InterstitialAd mInterstitialAd;
    
    private static String mBannerUnitID = "ca-app-pub-123456789/123456789";//
    private static String mInterstitialUnitID = "ca-app-pub-123456789/123456789";
    private static ArrayList<String> mTestDeviceIDList;
    private static AdMobBanner adBanner;
    private static boolean isTest = true;


    public static String md5(String s) {
        try {
            // Create MD5 Hash
            MessageDigest digest = java.security.MessageDigest.getInstance("MD5");
            digest.update(s.getBytes());
            byte messageDigest[] = digest.digest();

            // Create Hex String
            StringBuilder hexString = new StringBuilder();
            for (byte aMessageDigest : messageDigest) {
                hexString.append(Integer.toHexString(0xFF & aMessageDigest));
            }
            return hexString.toString();

        } catch (Exception e) {
            Log.d("aexolAdMob", e.toString());
        }
        return "";
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        /*
         * Init your stuff here.
         */
    }

    @Override
    public void onPause() {
        super.onPause();
        adBanner.pause();
    }

    @Override
    public void onResume() {
        super.onResume();
        adBanner.resume();

    }

    public static void addTestDeviceId(String Id) {

        if (mTestDeviceIDList == null) {
            mTestDeviceIDList = new ArrayList<String>();
        }

        mTestDeviceIDList.add(Id);
    }

    public static void setTesting(boolean Test) {
        isTest = Test;
    }

    public static void disableTesting(boolean Test) {
        isTest = false;
    }

    public static String getDeviceId(final Activity activity) {

        String android_id = Settings.Secure.getString(activity.getContentResolver(),
                Settings.Secure.ANDROID_ID);
        String deviceId = md5(android_id).toUpperCase();
        String mTestDeviceID = "";
        Log.d("aexolAdMob", "is Admob Test Device ? " + deviceId + " " + ", " + mTestDeviceID); //to confirm it worked
        return deviceId;

    }
    private static boolean isNetworkOnline(final Activity activity) {
        boolean status = false;
        try {
            ConnectivityManager cm = (ConnectivityManager) activity.getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo netInfo = cm.getNetworkInfo(0);
            if (netInfo != null && netInfo.getState() == NetworkInfo.State.CONNECTED) {
                status = true;
            } else {
                netInfo = cm.getNetworkInfo(1);
                if (netInfo != null && netInfo.getState() == NetworkInfo.State.CONNECTED) {
                    status = true;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        return status;

    }

    public static void loadInterstitialAd(final Activity activity) {
        try {
            activity.runOnUiThread(new Runnable() {
                
                @Override
                public void run() {
                    try {
                        getNewInterstitial(activity);
                    } catch (Exception e) {
                        Log.d("aexolAdMob", e.toString());
                    }
                }
            }
                    
            );
        } catch (Exception e) {
            Log.d("aexolAdMob", e.toString());
        }
    }

    public static void getNewInterstitial(final Activity activity) {

        try {
            if (isNetworkOnline(activity)) {
                Log.d("aexolAdMob", "spublic static void loadInterstitialAd()");
                if (mInterstitialAd == null) {
                    mInterstitialAd = new InterstitialAd(activity.
                            getApplicationContext());
                    mInterstitialAd.setAdUnitId(mInterstitialUnitID);
                }

                AdRequest.Builder builder = new AdRequest.Builder();
                
                if (isTest) {

                    builder.addTestDevice(AdRequest.DEVICE_ID_EMULATOR);
                    builder.addTestDevice(getDeviceId(activity));

                    if (mTestDeviceIDList != null) {

                        for (int i = 0; i < mTestDeviceIDList.size(); i++) {
                            builder.addTestDevice(mTestDeviceIDList.get(i));
                        }
                    }
                }
                
                AdRequest mRequest = builder.build();

                mInterstitialAd.loadAd(mRequest);
                mInterstitialAd.setAdListener(new AdListener() {
                    
                    @Override
                    public void onAdLoaded() {
                        try {
                            InterstitialAdLoaded();
                        } catch (Exception e) {
                            Log.d("aexolAdMob", e.toString());
                           
                        }
                    }
                                        
                    @Override
                    public void onAdFailedToLoad(int errorCode) {
                        try {
                            InterstitialFailedToLoadd();
                        } catch (Exception e) {
                            Log.d("aexolAdMob",  errorCode + ": "+ e.toString());
                        }
                    }
                });

            }
        } catch (Exception e) {
            Log.d("aexolAdMob", e.toString());
        }
        //return true;
    }

    public static void showInterstitialAd(final Activity activity) {
        Log.d("aexolAdMob", "showInterstitialAd");
        Log.d("aexolAdMob","thread showInterstitialAd: " + Thread.currentThread().getName());
        if (isNetworkOnline(activity)) {
            activity.runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    Log.d("aexolAdMob","thread Runnable: " + Thread.currentThread().getName());
                    showInterstitial();
                }
            });
        }
    }

    public static void showInterstitial() {
        
        if (mInterstitialAd.isLoaded()) {
            mInterstitialAd.show();
            InterstitialAdShown();
            mInterstitialAd.setAdListener(new AdListener() {
                @Override
                public void onAdClosed() {
                    InterstitialAdClosed();
                }
                
            });
            
        } else {
            
            mInterstitialAd.setAdListener(new AdListener() {
                @Override
                public void onAdLoaded() {
                    // TODO Auto-generated method stub
                    super.onAdLoaded();
                    mInterstitialAd.show();
                    InterstitialAdShown();
                }

                @Override
                public void onAdClosed() {
                    InterstitialAdClosed();
                }
                
                @Override
                public void onAdFailedToLoad(int errorCode) {
                        try {
                            Log.d("aexolAdMob", "Ad failed to load!" + errorCode);
                            InterstitialFailedToLoadd();
                        } catch (Exception e) {
                            Log.d("aexolAdMob",  errorCode + ": "+ e.toString());
                        }
                }
                
            }
            );
        }
    }

    public static void hideInterstitialAd() {
        //return true;
    }

    public static void loadBannerAd(final Activity activity) {
        try {
            Log.d("aexolAdMob", "spublic static void loadBannerAd()");
            if (isNetworkOnline(activity)) {
                activity.runOnUiThread(new Runnable() {
                    @Override
                    public void run() {

                        try {
                            adBanner = new AdMobBanner(activity.getApplicationContext(),
                                    mBannerUnitID);
                        } catch (Exception e) {
                            Log.d("aexolAdMob", e.toString());
                        }
                        //getNewInterstitial();
                    }
                });
            }
        } catch (Exception e) {
            Log.d("aexolAdMob", e.toString());
        }
    }

    public static void showBannerAd(final Activity activity) {
        Log.d("aexolAdMob", "public static void showBannerAd()");
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {

                try {
                    adBanner.showAdvert(isTest,
                            getDeviceId(activity),
                            mTestDeviceIDList);
                } catch (Exception e) {
                    Log.d("aexolAdMob", e.toString());
                }
            }
        });
    }

    public static void hideBannerAd() {
        //return true;
    }

    //\|/\|/\|/\|/\|/\|/\|/\|/\|/\|/ C functions we call
    public static native void adMobInitialized();

    public static native void nativeInterstitialAdLoaded();

    public static native void nativeInterstitialAdShown();

    public static native void nativeInterstitialAdClosed();
    
    public static native void nativeInterstitialFailedToLoad();

    public static native void nativeBannerAdLoaded();

    public static native void nativeBannerAdShown();
    //|\/|\/|\/|\/|\/|\/|\/|\ C functions we call

    public static void InterstitialAdLoaded() {
        nativeInterstitialAdLoaded();
    }

    public static void InterstitialAdShown() {
        nativeInterstitialAdShown();
    }

    public static void InterstitialAdClosed() {
        nativeInterstitialAdClosed();
    }
    
    public static void InterstitialFailedToLoadd() {
        nativeInterstitialFailedToLoad();
    }

    public static void BannerAdLoaded() {
        nativeBannerAdLoaded();
    }

    public static void BannerAdShown() {
        nativeBannerAdShown();
    }

    public static void setInterstitialUnitID(String ID) {
        mInterstitialUnitID = ID;
    }

    public static void setBannerUnitID(String ID) {
        mBannerUnitID = ID;
    }
}
