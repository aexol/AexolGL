package com.aexolgl.android.camera;

import android.content.Context;
import android.hardware.Camera;
import android.net.Uri;
import android.os.Environment;
import android.util.Log;
import android.view.Surface;
import android.view.WindowManager;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by darek on 16/04/15.
 */
public class MyCameraUtils  {

        public static Uri getOutputMediaFileUri(int type){
            return Uri.fromFile(getOutputMediaFile(type));
        }

        public static File getOutputPictureFile(){
            return getOutputMediaFile(MEDIA_TYPE_IMAGE);
        }

        public static File getOutputVideoFile(){
            return getOutputMediaFile(MEDIA_TYPE_VIDEO);
        }


        private static final int MEDIA_TYPE_IMAGE = 1;
        private static final int MEDIA_TYPE_VIDEO  = 1;

      /** Create a File for saving an image or video */
        private static File getOutputMediaFile(int type){
            // To be safe, you should check that the SDCard is mounted
            // using Environment.getExternalStorageState() before doing this.

            File mediaStorageDir = new File(Environment.getExternalStoragePublicDirectory(
                    Environment.DIRECTORY_PICTURES), "/MyCameraApps");
            // This location works best if you want the created images to be shared
            // between applications and persist after your app has been uninstalled.

            Log.d("MyCameraApp", "PICS_DIR: " + Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES));
            Log.d("MyCameraApp", "FILE: "+mediaStorageDir.getAbsolutePath());
            // Create the storage directory if it does not exist
            if (! mediaStorageDir.exists()){
                if (! mediaStorageDir.mkdirs()){
                    Log.d("MyCameraApp", "failed to create directory");
                    return null;
                }
            }


            // Create a media file name
            String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
            File mediaFile;
            if (type == MEDIA_TYPE_IMAGE){
                mediaFile = new File(mediaStorageDir.getPath() + File.separator +
                        "IMG_"+ timeStamp + ".jpg");
            } else if(type == MEDIA_TYPE_VIDEO) {
                mediaFile = new File(mediaStorageDir.getPath() + File.separator +
                        "VID_"+ timeStamp + ".mp4");
            } else {
                return null;
            }

            return mediaFile;
        }

    public String getRotation(Context context){
        final int rotation = ((WindowManager) context.getSystemService(Context.WINDOW_SERVICE)).getDefaultDisplay().getOrientation();
        switch (rotation) {
            case Surface.ROTATION_0:
                return "portrait";
            case Surface.ROTATION_90:
                return "landscape";
            case Surface.ROTATION_180:
                return "reverse portrait";
            default:
                return "reverse landscape";
        }
    }
}
