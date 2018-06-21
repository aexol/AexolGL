package com.aexolgl.android.app;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.Surface;
import android.view.WindowManager;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import org.libsdl.app.SDLActivity;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.concurrent.atomic.AtomicBoolean;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

class SimpleLibraryDependecyMap implements LibDependencyMap {
    private final HashMap<String, List<String>> mDeps;
    SimpleLibraryDependecyMap() {
        mDeps = new HashMap<String, List<String>>();
    }

    SimpleLibraryDependecyMap(String fileName) {
        this();
        FileInputStream jsonFile;
        try {
            jsonFile = new FileInputStream(fileName);
            StringBuffer jsonBuffer = new StringBuffer();
            byte[] buffer = new byte[1024];
            int n;
            while (( n = jsonFile.read(buffer)) != -1 ) {
                jsonBuffer.append(new String(buffer, 0, n));
            }
            JSONObject jDeps = new JSONObject(jsonBuffer.toString());
            Iterator<String> keys = jDeps.keys();
            while(keys.hasNext()) {
                String key = keys.next();
                ArrayList<String> deps = new ArrayList<String>();
                mDeps.put(key, deps);
                JSONArray arr = jDeps.getJSONArray(key);
                for(int i = 0; i < arr.length(); i++) {
                    deps.add(arr.getString(i));
                }
            }
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (JSONException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private LinkedHashSet<String> recusriveKeyVisit(String key) {
        LinkedHashSet<String> keyDeps = new LinkedHashSet<String>();
        if(mDeps.containsKey(key)) {
            for (String dep : mDeps.get(key)) {
                keyDeps.addAll(recusriveKeyVisit(dep));
                keyDeps.add(dep);
            }
        }
        return keyDeps;
    }

    @Override
    public List<String> getSortedDeps() {
        LinkedHashSet<String> sortedDeps = new LinkedHashSet<String>();
        for(Map.Entry<String, List<String>> entry : mDeps.entrySet()) {
            String key = entry.getKey();
            sortedDeps.addAll(recusriveKeyVisit(key));
            sortedDeps.add(key);
        }
        return new ArrayList<String>(sortedDeps);
    }

    @Override
    public Map<String, List<String>> getDeps() {
        return mDeps;
    }

    @Override
    public LibDependencyMap join(LibDependencyMap m) {
        for(Map.Entry<String, List<String>> entry : m.getDeps().entrySet()) {
            if(mDeps.containsKey(entry.getKey())) {
                throw new RuntimeException("Dependency map collision.");
            }
            mDeps.put(entry.getKey(), entry.getValue());
        }
        return this;
    }
}

class SimpleLibDependencyMapFactory implements  LibDependencyMapFactory {
    @Override
    public LibDependencyMap createDependecyMap() {
        return new SimpleLibraryDependecyMap();
    }

    public LibDependencyMap createDependecyMap(String fileName) {
        return new SimpleLibraryDependecyMap(fileName);
    }
}

public class AexActivity extends SDLActivity
{
    protected String[] concatStringArrays(String[] s1, String[] s2) {
        int s1Len = s1.length;
        int s2Len = s2.length;
        String[] result = new String[s1Len+s2Len];
        System.arraycopy(s1, 0, result, 0, s1Len);
        System.arraycopy(s2, 0, result, s1Len, s2Len);
        return result;
    }
    private class LibTuple {
        public final String name;
        public final Boolean exists;
        LibTuple(String name) {
            this.name = name;
            exists = Boolean.FALSE;
        }
        LibTuple(String name, Boolean exists) {
            this.name = name;
            this.exists = exists;
        }
    }

    private static String AEXOL_LIB_NAME = "AexolGL";

    protected Pattern getDependencyMapPattern() {
        return Pattern.compile("^lib_dep_map.*\\.txt\\.so$");
    }

    protected LibDependencyMapFactory getMapFactory() {
        return new SimpleLibDependencyMapFactory();
    }

    protected List<String> loadLibraryList() {
        File nativeLibDir = new File(getApplicationContext().getApplicationInfo().nativeLibraryDir);
        Pattern depFilePattern = getDependencyMapPattern();
        LibDependencyMap deps = getMapFactory().createDependecyMap();
        for(File f : nativeLibDir.listFiles()) {
            Matcher m = depFilePattern.matcher(f.getName());
            if(m.matches()) {
                deps.join(getMapFactory().createDependecyMap(f.getAbsolutePath()));
            }
        }
        return deps.getSortedDeps();
    }

    protected String[] filterExistingLibs(List<String> libList) {
        File nativeLibDir = new File(getContext().getApplicationInfo().nativeLibraryDir);
        List<String> existingLibs = new ArrayList<String>();
        List<String> notFound = new ArrayList<String>();
        for(File l : nativeLibDir.listFiles()) {
            existingLibs.add(l.getName());
        }
        for(int i = 0; i < libList.size(); i++) {
            String fullLibName = libList.get(i);
            if(!existingLibs.contains(fullLibName)) {
                notFound.add(fullLibName);
            } else {
                int start = 3;
                int end = fullLibName.length() - 3;
                libList.set(i, libList.get(i).substring(start, end));
            }
        }
        for(String notFoundLib : notFound) {
            Log.d(AEXOL_LIB_NAME, "Removing library: ".concat(notFoundLib));
            libList.remove(notFoundLib);
        }
        return libList.toArray(new String[libList.size()]);
    }
    @Override
    protected String[] getLibraries()
    {
        return filterExistingLibs(loadLibraryList());
    }

    public void loadLibraries() {
        for(String lib : getLibraries()) {
            Log.d(AEXOL_LIB_NAME, "trying load lib: "+lib);
            System.loadLibrary(lib);
        }
    }

    private File LIB_PATH;
    private static Context context;

    public static Context getAppContext(){
        return context;
    }

    @Override
    public void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        File DATA_PATH = getApplicationContext().getFilesDir();
        String LIB_SUBDIR = "";
        LIB_PATH = new File(DATA_PATH, LIB_SUBDIR);
        context = this;

    }
    @Override
    public void onDestroy()
    {
        super.onDestroy();
        context = null;
    }

    public void kill()
    {
        this.finish();
        //getActivity().finish();
        System.exit(0);
    }

    /*
    @Override
    public void onStart()
    {
        super.onStart();
        context = this;
        thisActivity = this;
    }

        @Override
    public void onStop()
    {
        super.onStop();
        context = null;
        thisActivity = null;
    }
    */

    public String getStringResource( int field_id )
    {
        return getResources().getString( field_id );
    }

    public int getDefaultRotation()
    {
        WindowManager wingdowManager = (WindowManager) getSystemService(WINDOW_SERVICE);
        int rotationIdx = 0;
        int rotation = wingdowManager.getDefaultDisplay().getRotation();
        if(rotation == Surface.ROTATION_0)
            rotationIdx = 0;
        if(rotation == Surface.ROTATION_90)
            rotationIdx = 90;
        if(rotation == Surface.ROTATION_180)
            rotationIdx = 180;
        if(rotation == Surface.ROTATION_270)
            rotationIdx = 270;
        return rotationIdx;
    }
    private void recursiveFileCopy(final String relPath, final String outBasePath)
    {
        AssetManager mgr = getAssets();
        String[] contents = new String[0];
        try {
            contents = mgr.list(relPath);
        } catch (IOException e) {
            e.printStackTrace();
        }
        if(contents != null && contents.length > 0) //Contains files, must be a directory
        {
            (new File(outBasePath, relPath)).mkdirs();
            for(String fn : contents)
            {
                recursiveFileCopy((new File(relPath, fn)).getPath(), outBasePath);
            }
        } else { //Assume it is a file.
            InputStream in = null;
            OutputStream out = null;
            try {
                in = mgr.open(relPath);
                out = new BufferedOutputStream(new FileOutputStream(new File(outBasePath, relPath)));
                int BUFFER_SIZE = 1024;
                byte[] buff = new byte[BUFFER_SIZE];
                int len = 0;
                while((len=in.read(buff)) > 0)
                {
                    out.write(buff, 0, len);
                }
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                try {
                    if (in != null) {
                        in.close();
                    }
                } catch (IOException ignored){}
                try {
                    if (out != null) {
                        out.close();
                    }
                } catch (IOException ignored){}
            }
        }
    }

    public void buildPythonData()
    {
        String PYTHON_LIB_NAME = "APP";
        final File python_libs = new File(LIB_PATH, PYTHON_LIB_NAME);
        if(!python_libs.exists())
        {
            recursiveFileCopy(PYTHON_LIB_NAME, LIB_PATH.getPath());
        }
    }

    public String getLibDir() {
        return getContext().getApplicationInfo().nativeLibraryDir;
    }

    public static AlertDialog.Builder getAlertDialogBuilder()
    {
        return new AlertDialog.Builder(AexActivity.mSingleton);
    }

    public void showErrorDialog(final String message) {
        final AtomicBoolean waitForAlert = new AtomicBoolean(true);
        final Object lock = new Object();
        synchronized(lock) {
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    AlertDialog.Builder dlgAlert  = new AlertDialog.Builder(AexActivity.mSingleton);
                    dlgAlert.setMessage(message);
                    dlgAlert.setTitle("AppError");
                    dlgAlert.setPositiveButton("Exit",
                        new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog,int id) {
                                // if this button is clicked, close current activity
                                waitForAlert.getAndSet(false);
                            }
                        });
                   dlgAlert.setCancelable(false);
                   dlgAlert.create().show();
               }
            });
           while(true)
           {
               if(!waitForAlert.get())
                   break;
           }
        }
    }
}
