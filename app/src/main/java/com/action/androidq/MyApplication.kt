package com.action.androidq

import android.app.Activity
import android.app.Application
import android.os.Bundle
import android.util.Log

class MyApplication : Application() {
    override fun onCreate() {
        super.onCreate()
        registerActivityLifecycleCallbacks(object : ActivityLifecycleCallbacks {
            override fun onActivityCreated(p0: Activity, p1: Bundle?) {
                Log.d("${p0.javaClass.simpleName}", "======::onActivityCreated")
            }

            override fun onActivityStarted(p0: Activity) {
                Log.d("${p0.javaClass.simpleName}", "======::onActivityStarted")

            }

            override fun onActivityResumed(p0: Activity) {
                Log.d("${p0.javaClass.simpleName}", "======::onActivityResumed")

            }

            override fun onActivityPaused(p0: Activity) {
                Log.d("${p0.javaClass.simpleName}", "======::onActivityPaused")

            }

            override fun onActivityStopped(p0: Activity) {
                Log.d("${p0.javaClass.simpleName}", "======::onActivityStopped")

            }

            override fun onActivitySaveInstanceState(p0: Activity, p1: Bundle) {
                Log.d("${p0.javaClass.simpleName}", "======::onActivitySaveInstanceState")

            }

            override fun onActivityDestroyed(p0: Activity) {
                Log.d("${p0.javaClass.simpleName}", "======::onActivityDestroyed")

            }
        })
    }
}