package com.action.androidq;

import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.os.Environment;
import android.util.Log;
import android.util.Xml;

import org.xmlpull.v1.XmlSerializer;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;

public class PortFiles {
    private static final String TAG = PortFiles.class.getSimpleName();
    private Context context;

    public PortFiles(Context context) {
        this.context = context;
        creatXML();
        String bluToothAdress = getBluTooTh();
        Log.e(TAG, "bluetooth_address:" + bluToothAdress);
    }

    private String getBluTooTh() {
        String macAddress = android.provider.Settings.Secure.getString(context.getContentResolver(), "bluetooth_address");
        return macAddress;
    }

    private void creatXML() {
        List<Ports> ports = new ArrayList<Ports>();
        /**
         * <ports>
         *     <port display="0" input="usb-xhci-hcd.0.auto-1.1/input0" />
         *     <port display="1" input="usb-xhci-hcd.0.auto-1.2/input0" />
         * </ports>
         *
         * usb-fe380000.usb-1.2/input1
         * usb-fe380000.usb-1.3/input0
         *鍵盤
         * usb-fe3a0000.usb-1/input1
         *触摸屏
         * usb-fe3a0000.usb-1/input0
         * usb-fe3a0000.usb-1/input0
         *
         * virtual:com.screenmirror.app,1000,TV Screen Mirror,0
         */
        Ports port = new Ports("1", "usb-fe3a0000.usb-1/input0");

        ports.add(port);
        XmlSerializer serializer = Xml.newSerializer();
        // 指定流目录
        try {
            OutputStream os = openFileOutput("input-port-associations.xml", Context.MODE_PRIVATE);
            serializer.setOutput(os, "UTF-8");
            serializer.startDocument("UTF-8", true);
            serializer.startTag(null, "ports");
            for (Ports portItem : ports) {
                serializer.startTag(null, "port");
                serializer.attribute(null, "display", portItem.getDisplay());
                serializer.attribute(null, "input", portItem.getInput());
                serializer.endTag(null, "port");
            }
            serializer.endTag(null, "ports");
            serializer.endDocument();
        } catch (IOException e) {
            e.printStackTrace();
        }


    }

    private OutputStream openFileOutput(String s, int modePrivate) {
        OutputStream outputStream = null;
        try {
            //
//            outputStream = new FileOutputStream(new File("/vendor/etc") + "/" + s);
            outputStream = new FileOutputStream(new File(context.getExternalFilesDir(Environment.DIRECTORY_DOCUMENTS).getAbsolutePath()) + "/" + s);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            Log.e(TAG, e.getLocalizedMessage().toString());
        } finally {
            return outputStream;
        }
    }

    /**
     * 获取蓝牙地址
     *
     * @return
     */
    public static String getBluetoothAddress() {
        // BluetoothAdapter bluetoothAdapter = BluetoothAdapter
        // .getDefaultAdapter();
        // String bluetooth = bluetoothAdapter.getAddress();
        // return bluetooth;

        // --------

        // BluetoothAdapter bluetoothAdapter =
        // BluetoothAdapter.getDefaultAdapter();
        // Object bluetoothManagerService = new
        // Mirror().on(bluetoothAdapter).get().field("mService");
        // if (bluetoothManagerService == null) {
        //
        // return null;
        // }
        // Object address = new
        // Mirror().on(bluetoothManagerService).invoke().method("getAddress").withoutArgs();
        // if (address != null && address instanceof String) {
        //
        // return (String) address;
        // } else {
        // return null;
        // }

        try {
            BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
            Field field = bluetoothAdapter.getClass().getDeclaredField("mService");
            // 参数值为true，禁用访问控制检查
            field.setAccessible(true);
            Object bluetoothManagerService = field.get(bluetoothAdapter);
            if (bluetoothManagerService == null) {
                return null;
            }
            Method method = bluetoothManagerService.getClass().getMethod("getAddress");
            Object address = method.invoke(bluetoothManagerService);
            if (address != null && address instanceof String) {

                return (String) address;
            } else {
                return null;
            }

        } catch (IllegalArgumentException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (NoSuchMethodException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (IllegalAccessException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (InvocationTargetException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        } catch (NoSuchFieldException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        return null;
    }

}
