package com.action.androidq

import android.os.*
import android.util.Log
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*
import java.io.File
import java.lang.reflect.InvocationTargetException
import java.lang.reflect.Method
import java.lang.reflect.Proxy

class MainActivity : AppCompatActivity() {
    val handler: Handler = object : Handler() {
        override fun handleMessage(msg: Message) {
            super.handleMessage(msg)
            if (msg.what == 1) {
                nativeAction.touchXY(777, 1866, 777, 1866)
            }
        }
    }
    val nativeAction: NativeAction = NativeAction()
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        tv_click.setOnClickListener {
            initNative()
            Log.e("click======", "tv_click~~~~~~");
        }
        //打开权限读取uinput
        tv_describtion.setOnClickListener {
            nativeAction.touchXY(777, 1866, 777, 1866)
            Toast.makeText(this@MainActivity, "tv_descrivetion", Toast.LENGTH_LONG).show()
            Log.e("click======", "tv_describtion~~~~~~");
        }
    }

    fun readVendorFiles() {
        val file: File = File("/vendor", "etc/input-port-associations.xml")
        if (!file.exists()) {//不存 直接推出
            return
        } else {

        }
    }

    /* InputRouting*/
    fun initPorts() {
        PortFiles(this)
        PortFiles.getBluetoothAddress()
        Log.e(
            "111111111=====",
            ":" + PortFiles.getBluetoothAddress() + "Environment.getVendorDirectory():" + Environment.getDataDirectory()
        )
    }

    /*native 写入system/bin*/
    fun initNative() {
        nativeAction.callMethod()
    }

}