package com.aexolgl.android.app;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.provider.Settings;
import android.provider.Settings.Secure;
import android.util.Log;
import android.view.WindowManager;

import java.io.File;
import java.io.FileFilter;
import java.util.Locale;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.regex.Pattern;


@SuppressWarnings("unused")
public class OSUtils {

    public static void showErrorDialog(final String message, final Activity activity) {
        final AtomicBoolean waitForAlert = new AtomicBoolean(true);
        final Object lock = new Object();
        activity.runOnUiThread(new Runnable() {
                           @Override
                           public void run() {
                AlertDialog.Builder dlgAlert = AexActivity.getAlertDialogBuilder();
                dlgAlert.setMessage(message);
                dlgAlert.setTitle("AppError");
                dlgAlert.setPositiveButton("Exit",
                        new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int id) {
                        // if this button is clicked, close current activity
                        waitForAlert.getAndSet(false);
                    }
                });
                dlgAlert.setCancelable(false);
                dlgAlert.create().show();
        }
    });
        while (true) {
            if (!waitForAlert.get()) {
                break;
            }
        }
    }

    public static void openMoreApps(String developerName, final Activity activity) {
        final String devName = developerName;
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                try {
                    activity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://search?q=" + devName)));
                } catch (android.content.ActivityNotFoundException anfe) {
                    activity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://play.google.com/store/apps/details?id=" + devName)));
                }
            }
        });
    }

    public static void showAppInStore(final String appPackageName, final Activity activity)
    {
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                try {
                    activity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=" + appPackageName)));
                } catch (android.content.ActivityNotFoundException ignored){}
            }
        });
    }

    public static String getLocaleLanguage() {
        return Locale.getDefault().getLanguage();
    }

    public static void setSleepMode(int boolValue, final Activity activity) {
        Log.d("aexol", "setSleepMode with value: "+boolValue);
        final boolean sleepIt = boolValue != 0;
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if (sleepIt) // true, sleep
                {
                    activity.getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
                } else {
                    // no sleep
                    activity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
                }
            }
        });
    }

    public static int showGoToSettingsDialog(final String message,
                                             final String okBttnText,
                                             final String cancelBttnText,
                                             final Activity activity)
    {
        final AtomicBoolean retFlag = new AtomicBoolean(false);
        final AtomicBoolean waitForAlert = new AtomicBoolean(true);
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                AlertDialog.Builder dlgAlert = AexActivity.getAlertDialogBuilder();
                dlgAlert.setMessage(message);
                dlgAlert.setTitle(""); // no title
                dlgAlert.setPositiveButton(okBttnText,
                    new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int id) {
                        Context context = activity.getApplicationContext();
                        final Intent i = new Intent();
                        i.setAction(Settings.ACTION_APPLICATION_DETAILS_SETTINGS);
                        i.addCategory(Intent.CATEGORY_DEFAULT);
                        i.setData(Uri.parse("package:" + context.getPackageName()));
                        i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                        i.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY);
                        i.addFlags(Intent.FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS);
                        context.startActivity(i);

                        // ok button clicked:
                        retFlag.getAndSet(true);

                        waitForAlert.getAndSet(false);
                }
            });

                dlgAlert.setNegativeButton(cancelBttnText,
                    new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int id) {
                        waitForAlert.getAndSet(false);
                }
            });
                //dialog.setIcon(android.R.drawable.ic_dialog_alert);
                dlgAlert.setCancelable(false);
                dlgAlert.create().show();
            }
        });

        while (true) {
            if (!waitForAlert.get()) {
                break;
            }
        }
        if(retFlag.get())
            return 1; // true
        return 0; // false
    }

    public static int showYesOrNoDialog(final String title,
                                        final String message,
                                        final String okBttnTxt,
                                        final String noBttnTxt,
                                        final Activity activity)
    {
        final AtomicBoolean retFlag = new AtomicBoolean(false);
        final AtomicBoolean waitForAlert = new AtomicBoolean(true);
        activity.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                AlertDialog.Builder dlgAlert = AexActivity.getAlertDialogBuilder();
                dlgAlert.setMessage(message);
                dlgAlert.setTitle(title); // if empty ("") - will miss title bar
                dlgAlert.setPositiveButton(okBttnTxt,
                    new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int id) {
                        retFlag.getAndSet(true);
                        waitForAlert.getAndSet(false);
                    }
                });

                dlgAlert.setNegativeButton(noBttnTxt,
                    new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int id) {
                        waitForAlert.getAndSet(false);
                }
            });
                //dialog.setIcon(android.R.drawable.ic_dialog_alert);
                dlgAlert.setCancelable(false);
                dlgAlert.create().show();
            }
        });

        while (true) {
            if (!waitForAlert.get()) {
                break;
            }
        }
        if(retFlag.get())
            return 1; // true
        return 0; // false
    }

    public static int checkCameraPermissions(final Activity activity)
    {
        Context context = activity.getApplicationContext();
        PackageManager pm = context.getPackageManager();
        int hasPerm = pm.checkPermission(android.Manifest.permission.CAMERA, context.getPackageName()); // CAPTURE_VIDEO_OUTPUT added in 19 API level, so use camera
        if (hasPerm != PackageManager.PERMISSION_GRANTED) {
           return 0;
        }
        return 1;
    }

    public static int checkAudioPermissions(final Activity activity)
    {
        Context context = activity.getApplicationContext();
        PackageManager pm = context.getPackageManager();
        int hasPerm = pm.checkPermission(android.Manifest.permission.CAMERA, context.getPackageName()); // CAPTURE_AUDIO_OUTPUT added in 19 API level, so use camera
        if (hasPerm != PackageManager.PERMISSION_GRANTED) {
           return 0;
        }
        return 1;
    }

    public static int checkPhotoPermissions(final Activity activity) // save/read photos == WRITE_EXTERNAL_STORAGE
    {
        Context context = activity.getApplicationContext();
        PackageManager pm = context.getPackageManager();
        int hasPerm = pm.checkPermission(android.Manifest.permission.WRITE_EXTERNAL_STORAGE, context.getPackageName()); // CAPTURE_AUDIO_OUTPUT added in 19 API level, so use camera
        if (hasPerm != PackageManager.PERMISSION_GRANTED) {
           return 0;
        }
        return 1;
    }

    public static void killApp(final Activity activity)
    {
        AexActivity aexActivity = (AexActivity)activity;
        aexActivity.kill();
    }

    public static int getNumberOfCores() {
    if(android.os.Build.VERSION.SDK_INT >= 17) {
        return Runtime.getRuntime().availableProcessors();
    }
    else {
       return getNumCoresOldPhones();
    }
  }

  public static int getNumCoresOldPhones() {
    //Private Class to display only CPU devices in the directory listing
    class CpuFilter implements FileFilter {
        @Override
        public boolean accept(File pathname) {
            //Check if filename is "cpu", followed by a single digit number
            if(Pattern.matches("cpu[0-9]+", pathname.getName())) {
                return true;
            }
            return false;
        }
    }

    try {
        //Get directory containing CPU info
        File dir = new File("/sys/devices/system/cpu/");
        //Filter to only list the devices we care about
        File[] files = dir.listFiles(new CpuFilter());
        //Return the number of cores (virtual CPU devices)
        return files.length;
    } catch(Exception e) {
        //Default to return 1 core
        return 1;
    }
  }

  public static String getUniqueDeviceID(final Activity activity) {
    String deviceID = Secure.getString(activity.getContentResolver(),
                                                        Secure.ANDROID_ID);
     return deviceID;
  }

  public static void openURL(String url, final Activity activity) {
      final String urlFINAL = new String(url);
      activity.runOnUiThread(new Runnable() {
          @Override
          public void run() {
              try {
                  activity.startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse(urlFINAL)));
              } catch (android.content.ActivityNotFoundException anfe) {
              }
          }
      });
  }
}
