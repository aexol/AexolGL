package com.aexolgl.android.hdw;

import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;
import android.util.SparseArray;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;


class SensorPack
{
  Sensor sensor;
  List<SensorEventListener> registeredListeners = new ArrayList<SensorEventListener>();
  List<SensorEventListener> idleListeners = new ArrayList<SensorEventListener>();
  int sensorType;  // static final field in Sensor class
  int sensorDelay; // static final fields in SensorManager class
  boolean started;
}
// mSensorManager.registerListener(this, mLight, SensorManager.SENSOR_DELAY_NORMAL);
// mSensorManager.unregisterListener(this);

@SuppressWarnings("unused")
public class HDWBridge
{
  private static final String HDW_MODULE_TAG = "aexhdwlib";
  private static SensorManager gSensorManager = null;
  private static final SparseArray<SensorPack> gSensors = new SparseArray<SensorPack>(); //  generated sensorID, Sensor ref
  private static int gSensorIDCounter = 1;

  public HDWBridge()
  {
  }

  private static int genNextSensorID()
  {
    return gSensorIDCounter++;
  }

  public static void sensorStart(int sensorID, final Activity activity)
  {
    synchronized(gSensors)
    {
      innerSensorStart(sensorID, activity);
    }
  }

  public static void sensorStop(int sensorID, final Activity activity)
  {
    synchronized(gSensors)
    {
      innerSensorStop(sensorID, activity);
    }
  }

  private static void innerSensorStart(int sensorID, final Activity activity)
  {
    SensorPack sp = gSensors.get(sensorID);
    if(sp == null){
        Log.e(HDW_MODULE_TAG, "sensorStart - sensorID not exists: "+sensorID);
        return;
    }

    sp.started = true;
    for(SensorEventListener sel : sp.idleListeners){
      getSensorManager(activity).registerListener(sel, sp.sensor, sp.sensorType);
    }

    for(SensorEventListener sel : sp.idleListeners){
      sp.registeredListeners.add(sel);
    }
    sp.idleListeners.clear();
  }

  private static void innerSensorStop(int sensorID, final Activity activity)
  {
    SensorPack sp = gSensors.get(sensorID);
    if(sp == null)
    {
      Log.e(HDW_MODULE_TAG, "sensorStop - sensorID not exists: " + sensorID);
      return;
    }

    sp.started = false;
    for(SensorEventListener sel : sp.registeredListeners)
    {
      getSensorManager(activity).unregisterListener(sel);
    }
    for(SensorEventListener sel : sp.registeredListeners)
    {
      sp.idleListeners.add(sel);
    }
    sp.registeredListeners.clear();
  }

  public static void addListener(int sensorID, long cppCallbackAddress, final Activity activity)
  {
    synchronized(gSensors)
    {
      SensorPack sp = gSensors.get(sensorID);
      if(sp == null)
      {
        Log.e(HDW_MODULE_TAG, "addListener - sensorID not exists: " + sensorID);
        return;
      }

      SensorEventListener sel = new HDWSensorEventListener(sensorID, sp.sensorType, cppCallbackAddress);
      sp.idleListeners.add(sel);
      if(sp.started)
        innerSensorStart(sensorID, activity);
      sp.idleListeners.add(sel);
      if(sp.started)
        innerSensorStart(sensorID, activity);
    }
  }
  public static native void nativeOnSensorChanged(float values[], long cppCallbackAddress);

  private static SensorManager getSensorManager(final Activity activity)
  {
    if(gSensorManager == null){
        gSensorManager = (SensorManager) activity.
                getApplicationContext().
                getSystemService(Context.SENSOR_SERVICE);
    }
    return gSensorManager;
  }

  private static boolean isDefaultSensorCreated(int sensorType)
  {
    for(int i = 0; i < gSensors.size(); i++)
    {
      SensorPack sp = gSensors.valueAt(i);
      if(sp.sensorType == sensorType)
        return true;
    }

    return false;
  }

  public static int getDefaultSensor(int sensorCPPCode, final Activity activity) // != 0 id if created
  {
    int sensorType = codeToType(sensorCPPCode);

    if(isDefaultSensorCreated(sensorType)) // only one default created at once
      return 0;

    Sensor defaultSensor = getSensorManager(activity).getDefaultSensor(sensorType);
    if (defaultSensor != null){

      int sensorID = genNextSensorID();
      SensorPack pack = new SensorPack();
      pack.sensor = defaultSensor;
      pack.sensorType = sensorType;
      pack.sensorDelay = SensorManager.SENSOR_DELAY_NORMAL; // TODO add parametrization
      gSensors.put(sensorID, pack);

      return sensorID;
    }
    else{
      return 0;
    }
  }

  public static int getSensor(int sensorType, final Activity activity)
  {
    Sensor sensor = getSensorManager(activity).getDefaultSensor(sensorType);
    if (sensor != null){

      int sensorID = genNextSensorID();
      SensorPack pack = new SensorPack();
      pack.sensor = sensor;
      pack.sensorType = sensorType;
      pack.sensorDelay = SensorManager.SENSOR_DELAY_NORMAL; // TODO add parametrization
      gSensors.put(sensorID, pack);
      return sensorID;
    }
    else{
      return 0;
    }
  }

  public static void removeSensor(int sensorID, final Activity activity)
  {
    synchronized(gSensors)
    {
      SensorPack sp = gSensors.get(sensorID);
      if(sp == null)
      {
        Log.e(HDW_MODULE_TAG, "removeSensor - sensorID not exists: " + sensorID);
        return;
      }

      innerSensorStop(sensorID, activity);
      gSensors.remove(sensorID);
    }
  }

  public static boolean defaultSensorExists(int sensorCPPCode, final Activity activity)
  {
    return getSensorManager(activity).getDefaultSensor(codeToType(sensorCPPCode)) != null;
  }

  public static String getAvailableSensors(final Activity activity)
  {
    JSONObject jsonRoot = new JSONObject();
    try{
      List<Sensor> deviceSensors = getSensorManager(activity).getSensorList(Sensor.TYPE_ALL);
      for(Sensor s : deviceSensors)
      {
        JSONObject jsonSensor = new JSONObject();
        jsonSensor.put("sensorCode", typeToCode(s.getType()));
        jsonSensor.put("sensorType", s.getType());
        jsonSensor.put("sensorName", s.getName());
        //jsonRoot.put((""+counter++), jsonSensor);
      }
    }catch(Exception exc)
    {
      return exc.toString();
    }

    Log.d(HDW_MODULE_TAG, "list: "+jsonRoot.toString());
    return jsonRoot.toString();
  }

  private static int codeToType(int code)
  {
    switch(code)
    {
      case 1: return Sensor.TYPE_ACCELEROMETER;
      case 2: return Sensor.TYPE_GYROSCOPE;
      case 3: return Sensor.TYPE_MAGNETIC_FIELD;
      //case 4: return Sensor.TYPE_MOTION_DETECT;
      case 5: return Sensor.TYPE_LIGHT;
      // to expand in future
      default: return 0;
    }
  }

  private static int typeToCode(int type)
  {
    switch(type)
    {
      case Sensor.TYPE_ACCELEROMETER: return 1;
      case Sensor.TYPE_GYROSCOPE: return 2;
      case Sensor.TYPE_MAGNETIC_FIELD: return 3;
      //case Sensor.TYPE_MOTION_DETECT: return 4;
      case Sensor.TYPE_LIGHT: return 5;
      // to expand in future
      default: return 0;
    }
  }

}
