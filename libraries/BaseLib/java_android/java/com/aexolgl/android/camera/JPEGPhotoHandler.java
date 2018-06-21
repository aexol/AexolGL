package com.aexolgl.android.camera;

import android.hardware.Camera;
import android.util.Log;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by darek on 16/04/15.
 */
public class JPEGPhotoHandler implements Camera.PictureCallback{

    private int camID;
    String fullPath;

    public JPEGPhotoHandler(String path, int camID){
        this.camID = camID;
        this.fullPath = path;
    }

    @Override
    public void onPictureTaken(byte[] data, Camera camera) {
        //File pictureFile = MyCameraUtils.getOutputPictureFile();
        File pictureFile = new File(fullPath);

        if(pictureFile == null){
            Log.d("CAM_TUT_3: ", "Error creating media file. Check storage permissions in manifest or size of sdcard left. ");
            NativeCameraLib.nativeCallbackTakePicture(CameraDevice.NOT_ENOUGHT_MEMORY, camID);
            return;
        }

        try{
            FileOutputStream fos = new FileOutputStream(pictureFile);
            fos.write(data);
            fos.close();

            NativeCameraLib.nativeCallbackTakePicture(CameraDevice.SUCCESS, camID);
        }catch(FileNotFoundException exc){
            NativeCameraLib.nativeCallbackTakePicture(CameraDevice.FILE_NOT_FOUND, camID);
            Log.d("CAM_TUT_3", "File not found: " + exc.getMessage());
        }catch(IOException exc){
            NativeCameraLib.nativeCallbackTakePicture(CameraDevice.IO_EXCEPTION, camID);
            Log.d("CAM_TUT_3", "Error accessing file: " + exc.getMessage());
        }catch(Exception exc){
            NativeCameraLib.nativeCallbackTakePicture(CameraDevice.FAILURE, camID);
        }
    }
}
