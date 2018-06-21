package com.aexolgl.android.camera;

import android.os.Environment;

import java.io.File;

/**
 * Created by darek on 23/04/15.
 */
public class Directory {

    public static final int ALARMS = 0x00;
    public static final int DCIM = 0x01;
    public static final int DOWNLOADS = 0x03;
    public static final int MOVIES = 0x04;
    public static final int MUSIC = 0x05;
    public static final int NOTIFICATIONS = 0x06;
    public static final int PICTURES = 0x07;
    public static final int PODCAST = 0x08;
    public static final int RINGTONES = 0x09;
    public static final int DOCUMENTS = 0x0A;

    public Directory(){

    }

    public static String getUserDataDirectory(){
        return Environment.getDataDirectory().getPath();
    }

    public static String getDownloadCacheDirectory(){
        return Environment.getDownloadCacheDirectory().getPath();
    }

    public static String getExternalStorageDirectory(){
        return Environment.getExternalStorageDirectory().getPath();
    }

    public static String getExternalStoragePublicDirectory(int type){
        switch(type){
            case ALARMS:
                return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_ALARMS).getPath();
            case DCIM:
                return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DCIM).getPath();
            case DOWNLOADS:
                return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOWNLOADS).getPath();
            case MOVIES:
                return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MOVIES).getPath();
            case MUSIC:
                return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_MUSIC).getPath();
            case NOTIFICATIONS:
                return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_NOTIFICATIONS).getPath();
            case PICTURES:
                return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).getPath();
            case PODCAST:
                return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PODCASTS).getPath();
            case RINGTONES:
                return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_RINGTONES).getPath();
            case DOCUMENTS:
                //return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_DOCUMENTS).getPath(); // TODO may throw error on andr ? check it compiler throw - cannot find symbol
                return getUserDataDirectory();
            default:
                return Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES).getPath();
        }
    }

    public static int mkdir(String dir){
        File path = new File(dir);
        try{
            path.mkdirs();
            return Bridge.CONST_CAMERA_SUCCESS();
        }catch(Exception e){
            Bridge.setErrorMessage(e.getMessage());
            return Bridge.CONST_CAMERA_FAILURE();
        }
    }
}
