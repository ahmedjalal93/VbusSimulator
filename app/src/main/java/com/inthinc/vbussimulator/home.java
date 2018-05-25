package com.inthinc.vbussimulator;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.IBinder;
import android.support.annotation.Nullable;
import android.util.Log;

import com.inthinc.vbussimulator.Bluetooth.BluetoothServer;

import java.lang.reflect.Method;
import java.util.UUID;

/**
 * Created by ajalal on 2/27/18.
 */

public class home extends Service {

    public final static String TAG = "Vbussimulator";

    private static final UUID MY_UUID_INSECURE = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private static final UUID MY_UUID_SECURE = UUID.fromString("fa87c0d0-afac-11de-8a39-0800200c9a66");
    private BluetoothServer mSecureAcceptThread;
    private BluetoothAdapter mBluetoothAdapter;

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        // Let it continue running until it is stopped.
        Log.i(TAG, "Service Started");

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();


//        if (mBluetoothAdapter.getScanMode() != BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE) {
//            Method method;
//            try {
//                method = mBluetoothAdapter.getClass().getMethod("setScanMode", int.class, int.class);
//                method.invoke(mBluetoothAdapter,BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE,120);
//                Log.i("invoke","method invoke successfully");
//            }
//            catch (Exception e){
//                e.printStackTrace();
//            }
//        }
//
//
//        mSecureAcceptThread = new BluetoothServer();
//
//        mSecureAcceptThread.acceptConnect(mBluetoothAdapter, MY_UUID_INSECURE);

        System.loadLibrary("VbusSimulator");


        int inited = init();

        Log.i(TAG, "jni initialized " + inited);

        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        Log.i(TAG, "Service Destroyed");
    }

    private native int init();

}