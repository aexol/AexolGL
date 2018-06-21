package com.aexolgl.android.camera;

import android.graphics.BitmapFactory;
import android.hardware.Camera;
import android.media.ExifInterface;
import android.util.Log;
import android.graphics.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by darek on 16/04/15.
 */


public class PhotoHandler implements Camera.PictureCallback{
    public static final int FORMAT_JPG = 0x01;

    public static final int FILTER_NONE = 0;
    public static final int FILTER_NOCTOVISION = 1;

    private int camID;
    String fullPath;
    int format;

    int filter;

    public PhotoHandler(String path, int format, int filter, int camID ) throws Exception {
        this.camID = camID;
        this.fullPath = path;
        this.format = format;
        this.filter = filter;

        if( ! PhotoHandler.isSupportedPictureFormat(format) ){
            throw new Exception("Unsupported picture format");
        }
    }

    public static boolean isSupportedPictureFormat(int format){
        if(format == FORMAT_JPG){
            return true;
        }

        return false;
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
            if(format == FORMAT_JPG) {
                switch(filter){
                    case FILTER_NONE:
                        saveNONEFilterFile(pictureFile, data);
                        break;
                    case FILTER_NOCTOVISION:
                        saveNOCTOVISIONFilterFile(pictureFile, data);
                        break;
                    default:
                        saveNONEFilterFile(pictureFile, data);
                }
            }

            NativeCameraLib.nativeCallbackTakePicture(CameraDevice.SUCCESS, camID);
        }catch(FileNotFoundException exc){
            NativeCameraLib.nativeCallbackTakePicture(CameraDevice.FILE_NOT_FOUND, camID);
            Log.d("CAM_TUT_3", "File not found: " + exc.getMessage());
        }catch(IOException exc){
            NativeCameraLib.nativeCallbackTakePicture(CameraDevice.IO_EXCEPTION, camID);
            Log.d("CAM_TUT_3", "Error accessing file: " + exc.getMessage());
        }catch(Exception exc){
            NativeCameraLib.nativeCallbackTakePicture(CameraDevice.FAILURE, camID);

            Bridge.setErrorMessage(exc.getMessage());
        }
    }

    private void saveNONEFilterFile(File pictureFile, byte[] data) throws IOException {
        FileOutputStream fos = new FileOutputStream(pictureFile);
        fos.write(data);
        fos.close();
    }

    private void saveNOCTOVISIONFilterFile(File pictureFile, byte[] data) throws IOException {
        // save normal file to generate exif data
        FileOutputStream fos = new FileOutputStream(pictureFile);
        fos.write(data);
        fos.close();

        ExifInterface exif = new ExifInterface(pictureFile.getAbsolutePath());
        String orientation = exif.getAttribute(ExifInterface.TAG_ORIENTATION);

        // save modified img
        fos = new FileOutputStream(pictureFile);
        Bitmap bitmap = getNoctovisionFilteredBitmapUsing(data);
        bitmap.compress(Bitmap.CompressFormat.JPEG, 100, fos); // saving the Bitmap to a file compressed as a JPEG with 100% quality
        fos.flush();
        fos.close();

        // set properly exif data
        exif = new ExifInterface(pictureFile.getAbsolutePath());
        exif.setAttribute(ExifInterface.TAG_ORIENTATION, orientation);
        exif.saveAttributes();
    }

    private Bitmap getNoctovisionFilteredBitmapUsing(byte[] data){
        BitmapFactory.Options opt = new BitmapFactory.Options();
        //opt.inDither = true;
        opt.inPreferredConfig = Bitmap.Config.ARGB_8888;
        opt.inMutable = true;

        //BitmapFactory.decodeFile()
        Bitmap bitmap = BitmapFactory.decodeByteArray(data, 0, data.length, opt);

//        int pixels[] = new int[bitmap.getWidth()*bitmap.getHeight()];
//        bitmap.getPixels(pixels, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());
//        for(int i = 0 ; i < pixels.length ; i++){
//            int p = pixels[i];
//
//            int a = Color.alpha(p);
//            int r = Color.red(p);
//            int g = Color.green(p);
//            int b = Color.blue(p);
//
//            int green = (r + g + b) / 3;
//            if(green > 255) green = 255;
//            int wspol = 255 - green;
//
//            r = 255 - wspol*2;
//            g = green;
//            b = 255 - wspol*2;
//
//            if(r < 0)
//                r = 0;
//            if(b < 0)
//                b = 0;
//
//            pixels[i] = Color.argb(a, r, g, b);
//        }
//        bitmap.setPixels(pixels, 0, bitmap.getWidth(), 0, 0, bitmap.getWidth(), bitmap.getHeight());

        for(int i = 0 ; i < bitmap.getWidth() ; i++){
            for(int j = 0 ; j < bitmap.getHeight() ; j++){
                int p = bitmap.getPixel(i, j);

                int a = Color.alpha(p);
                float r = Color.red(p) / 255.0f;
                float g = Color.green(p) / 255.0f;
                float b = Color.blue(p) / 255.0f;

                r = r / 0.2f;
                g = g / 0.2f;
                b = b / 0.2f;

                float green = (r + g + b) / 3.0f;
                if(green > 1.0f) green = 1.0f;
                float wspol = 1.0f - green;

                r = 1.0f - wspol*2;
                g = green;
                b = 1.0f - wspol*2;

                if(r < 0.0f)
                    r = 0.0f;
                if(b < 0.0f)
                    b = 0.0f;

                p = Color.argb(a, (int)(r*255), (int)(g*255), (int)(b*255));
                bitmap.setPixel(i,j,p);
            }
        }

        return bitmap;
    }
}
