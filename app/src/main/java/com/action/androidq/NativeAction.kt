package com.action.androidq

import android.util.Log

class NativeAction {
    companion object {
        init {
            System.loadLibrary("fnative-lib")
        }
    }

    external fun touchXY(startx: Int, starty: Int, endx: Int, endy: Int)
    external fun callMethod()
    fun showToast() {
        Log.e("调用输出======", "printAction")
    }
}