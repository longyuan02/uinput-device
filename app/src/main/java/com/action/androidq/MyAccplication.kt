package com.action.androidq

import android.app.Activity
import android.app.Application
import android.os.Bundle
import android.util.Log

class MyAccplication : Application(){
    override fun onCreate() {
        super.onCreate()
        this.registerActivityLifecycleCallbacks(object:ActivityLifecycleCallbacks{
            override fun onActivityCreated(activity: Activity, savedInstanceState: Bundle?) {
                TODO("Not yet implemented")
                Log.e(activity.javaClass.simpleName+"======","onActivityCreated")
            }

            override fun onActivityStarted(activity: Activity) {
                TODO("Not yet implemented")
                Log.e(activity.javaClass.simpleName+"======","onActivityStarted")
            }

            override fun onActivityResumed(activity: Activity) {
                TODO("Not yet implemented")
                Log.e(activity.javaClass.simpleName+"======","onActivityResumed")
            }

            override fun onActivityPaused(activity: Activity) {
                TODO("Not yet implemented")
                Log.e(activity.javaClass.simpleName+"======","onActivityPaused")
            }

            override fun onActivityStopped(activity: Activity) {
                TODO("Not yet implemented")
                Log.e(activity.javaClass.simpleName+"======","onActivityStopped")
            }

            override fun onActivitySaveInstanceState(activity: Activity, outState: Bundle) {
                TODO("Not yet implemented")
                Log.e(activity.javaClass.simpleName+"======","onActivitySaveInstanceState")
            }

            override fun onActivityDestroyed(activity: Activity) {
                TODO("Not yet implemented")
                Log.e(activity.javaClass.simpleName+"======","onActivityDestroyed")
            }
        })
    }
}