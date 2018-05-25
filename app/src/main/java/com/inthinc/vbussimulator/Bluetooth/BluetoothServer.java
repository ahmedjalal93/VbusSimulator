package com.inthinc.vbussimulator.Bluetooth;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothServerSocket;
import android.bluetooth.BluetoothSocket;
import android.net.wifi.WifiInfo;
import android.net.wifi.SupplicantState;
import android.util.Log;
import android.net.NetworkInfo.DetailedState;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.Field;
import java.util.UUID;

import static com.inthinc.vbussimulator.home.TAG;

/**
 * Created by ajalal on 4/17/18.
 */

public class BluetoothServer extends Thread{
    private BluetoothSocket bTSocket;

    public BluetoothServer() { }

    public void acceptConnect(BluetoothAdapter mBluetoothAdapter, UUID mUUID) {
        BluetoothServerSocket temp = null;
        InputStream mmInStream;
        OutputStream mmOutStream;

        try {
            Log.i(TAG, "Bluetooth Listening");
            temp = mBluetoothAdapter.listenUsingInsecureRfcommWithServiceRecord("VbusConnection", mUUID);
        } catch(IOException e) {
            Log.d("SERVERCONNECT", "Could not get a BluetoothServerSocket:" + e.toString());
        }
        while(true) {
            try {
                Log.i(TAG, "Bluetooth accepting");
                bTSocket = temp.accept();
            } catch (IOException e) {
                Log.d("SERVERCONNECT", "Could not accept an incoming connection.");
                break;
            }
            if (bTSocket != null) {
                try {
                    Log.i(TAG, "Device connected");
                    temp.close();
                } catch (IOException e) {
                    Log.d("SERVERCONNECT", "Could not close ServerSocket:" + e.toString());
                }

                InputStream tmpIn = null;
                OutputStream tmpOut = null;

                try {
                    tmpIn = bTSocket.getInputStream();
                    tmpOut = bTSocket.getOutputStream();
                } catch (IOException e) {
                    Log.e(TAG, "temp sockets not created", e);
                }

                mmInStream = tmpIn;
                mmOutStream = tmpOut;

                byte[] buffer = new byte[1024];
                int bytes;
                try {
                    // Read from the InputStream
                    bytes = mmInStream.read(buffer);

                    String readMessage = new String(buffer, 0, bytes);
                    Log.i(TAG, "Received message" + readMessage);

                } catch (IOException e) {
                    Log.e(TAG, "disconnected", e);
                    break;
                }

            }
        }
    }

    public void closeConnect() {
        try {
            bTSocket.close();
        } catch(IOException e) {
            Log.d("SERVERCONNECT", "Could not close connection:" + e.toString());
        }
    }
}