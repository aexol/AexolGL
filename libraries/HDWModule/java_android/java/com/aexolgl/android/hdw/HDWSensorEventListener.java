package com.aexolgl.android.hdw;
import android.hardware.*;

class HDWSensorEventListener implements SensorEventListener
{
  private static int MAX_SENSORS_VALUES = 5; // check it TODO

  private int mSensorID;
  private int mSensorType;
  private long mCPPCallbackAddress;

  public HDWSensorEventListener(int sensorID, int sensorType, long cppCBAddress)
  {
    mSensorID = sensorID;
    mSensorType = sensorType;    
    mCPPCallbackAddress = cppCBAddress;
  }

  @Override
  public final void onAccuracyChanged(Sensor sensor, int accuracy) {
  }

  @Override
  public final void onSensorChanged(SensorEvent event) {
    // if removeing listeners in SensorManager android class in synced, no sync is needed here
    HDWBridge.nativeOnSensorChanged(event.values, mCPPCallbackAddress );
  }

}
