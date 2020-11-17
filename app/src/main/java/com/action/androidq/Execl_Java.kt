package com.action.androidq

import android.os.Handler
import android.os.IBinder
import android.os.Message
import android.util.Log
import java.io.BufferedReader
import java.io.DataOutputStream
import java.io.IOException
import java.io.InputStreamReader
import java.lang.reflect.InvocationTargetException
import java.lang.reflect.Method
import java.lang.reflect.Proxy

class Execl_Java {
    var handler: Handler = object : Handler() {
        override fun handleMessage(msg: Message) {
            super.handleMessage(msg)
        }
    }

    /*获取投屏相关信息*/
    fun runShellCommand(command: String, type: Int): Execl_Java {
        val Logcat = "runshellCommand======="
        Log.e(Logcat, ":$command")
        var process: Process? = null
        var bufferedReader: BufferedReader? = null
        val mShellCommandSB = StringBuilder()
        Log.d(Logcat, "runShellCommand :$command")
        mShellCommandSB.delete(0, mShellCommandSB.length)
        val cmd = arrayOf("/system/bin/sh", "-c", command) //调用bin文件
        try {
            val b = ByteArray(1024)
            process = Runtime.getRuntime().exec(cmd)
            bufferedReader = BufferedReader(
                InputStreamReader(
                    process.inputStream
                )
            )
            var line: String
            while (bufferedReader.readLine().also { line = it } != null) {
                mShellCommandSB.append(
                    """
    $line
    
    """.trimIndent()
                )
            }
            when (type) {
                1 -> {
                    val cmdx = arrayOf("/system/xbin/su", "-c", "su") //调用bin文件
                    Runtime.getRuntime().exec(cmdx)
                    runShellCommand("", 2)
                }
                2 -> runShellCommand("setenforce 0", 3)
                3 -> runShellCommand("chmod 666 /dev/uinput", 4)
            }
            Log.d(Logcat, "runShellCommand result : $mShellCommandSB")
            process.waitFor()
        } catch (e: IOException) {
            e.printStackTrace()
        } catch (e: InterruptedException) {
            e.printStackTrace()
        } finally {
            if (bufferedReader != null) {
                try {
                    bufferedReader.close()
                } catch (e: IOException) {
                }
            }
            process?.destroy()
        }
        return this
    }

    /**
     * 执行shell命令
     *  Execl_Java.create().runShellCommand("input tap 888 1888", 0);
     * @param cmd
     */
    fun execShellCmd(cmd: String?) {
        try {
            // 申请获取root权限，这一步很重要，不然会没有作用
            val process = Runtime.getRuntime().exec("su")
            // 获取输出流
            val outputStream = process.outputStream
            val dataOutputStream = DataOutputStream(
                outputStream
            )
            dataOutputStream.writeBytes(cmd)
            dataOutputStream.flush()
            dataOutputStream.close()
            outputStream.close()
        } catch (t: Throwable) {
            t.printStackTrace()
        }
    }
//            Execl_Java.create().AMS(24);
    /**
     * 反射AMS
     */
    var iASM: Any? = null

    public fun AMS(stackId: Int) {
        var getServiceMethod: Method? = null
        try {
            //加载得到ServiceManager类，然后得到方法getService。
            getServiceMethod = Class.forName("android.os.ServiceManager").getDeclaredMethod(
                "getService", *arrayOf<Class<*>>(
                    String::class.java
                )
            )
            //通过getServiceMethod得到ServiceManager的实例（隐藏类，所以使用Object）
            val ServiceManager = getServiceMethod.invoke(null, *arrayOf<Any>("activity"))
            //通过反射得到Stub
            val aStub = Class.forName("android.app.ActivityManagerNative")
            val asInterface = aStub.getMethod("asInterface", IBinder::class.java)
            iASM = asInterface.invoke(null, ServiceManager)
            val methods = iASM?.javaClass?.getDeclaredMethods()
            val setFocuse =
                iASM?.javaClass?.getMethod("setFocusedStack", Int::class.javaPrimitiveType)
            val `object` = Proxy.newProxyInstance(
                iASM?.javaClass?.getClassLoader(), javaClass.interfaces
            ) { proxy, method, args ->
                if (method.name == "") {
                }
                method.invoke(iASM, *args)
            }
            setFocuse?.invoke(iASM, stackId)
        } catch (e: NoSuchMethodException) {
            e.printStackTrace()
        } catch (e: ClassNotFoundException) {
            e.printStackTrace()
        } catch (e: IllegalAccessException) {
            e.printStackTrace()
        } catch (e: InvocationTargetException) {
            e.printStackTrace()
        }
    }

    companion object {
        fun create(): Execl_Java {
            return Execl_Java()
        }
    }
}