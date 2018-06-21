package com.aexolgl.android.camera;

import android.content.Intent;
import android.media.MediaScannerConnection;
import android.net.Uri;

import com.aexolgl.android.app.AexActivity;

import java.io.File;

/**
 * Created by darek on 29/05/15.
 */
public class Gallery implements MediaScannerConnection.MediaScannerConnectionClient {
    public String[] allFiles;
    private String SCAN_PATH ;
    private static final String FILE_TYPE = "*/*";
    private MediaScannerConnection conn;
    private String path;

    public Gallery(String path){
        this.path = path;
    }

    public int open(){
        File folder = new File(path);
        allFiles = folder.list();
        final String SCAN_PATH= path +"/" +allFiles[0];

        if(conn!=null)
        {
            conn.disconnect();
        }
        final MediaScannerConnection conn = new MediaScannerConnection(AexActivity.getAppContext(), this );
        conn.connect();

        return Bridge.CONST_CAMERA_SUCCESS(); // TODO to modify if needed
    }

    @Override
    public void onMediaScannerConnected() {
        conn.scanFile(SCAN_PATH, FILE_TYPE);
    }

    @Override
    public void onScanCompleted(String path, Uri uri) {
        try {
            // Log.d("onScanCompleted",uri + "success"+conn);
            if (uri != null)
            {
                Intent intent = new Intent(Intent.ACTION_VIEW);
                intent.setData(uri);
                AexActivity.getAppContext().startActivity(intent);
            }
        } finally
        {
            conn.disconnect();
            conn = null;
        }
    }
}
