#include <jni.h>
#include <string>
#include <android/log.h>
#include <iostream>

#include <stdint.h>
//#include <cutils/log.h>
#include <linux/uinput.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/system_properties.h>
//wait和waitpid函数
#include <sys/types.h>
#include <sys/wait.h>
#include "tool_lib.cpp"
#include <unistd.h>
//执行系统命令
#include <cstdlib>
//chmod
#include <sys/stat.h>
#include <stdlib.h>

//#ifdef HAVE_LIBC_SYSTEM_PROPERTIES
//#include <cutils/properties.h>
//#endif

#define LOG_TAG "BT"
#define LOGD(...)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__) // 定义LOGD类型
#define LOGI(...)__android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)  // 定义LOGI类型
#define LOGW(...)__android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)  // 定义LOGW类型
#define LOGE(...)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__) // 定义LOGE类型
#define LOGF(...)__android_log_print(ANDROID_LOG_FATAL, LOG_TAG, __VA_ARGS__) // 定义LOGF类
#define ALOGD(...)__android_log_print(ANDROID_LOG_DEBUG,__VA_ARGS__)


#define API_EXPORT __attribute__((visibility("default")))
int uinp_fd = 0;//事件id
struct uinput_user_dev uinp;
struct input_event event;
const tool_lib toolLib = tool_lib(uinp_fd);
int global_tracking_id = 1;
using namespace std;

int rc;
jint index = 0;
extern "C"
JNIEXPORT void JNICALL
Java_com_action_androidq_NativeAction_callMethod(JNIEnv *env, jobject thiz) {
    jclass clazz;
    index++;
    LOGE("开始=======================:%d", "begin");
    clazz = (*env).FindClass("com/action/androidq/NativeAction");
    /*toolLib.syncCommandBin("adb shell");
    toolLib.syncCommandXBin("su");
    chmod("/dev/uinput", S_IRWXU);
    LOGE("权限=====%d", chmod("/dev/uinput", S_IRWXU));*/
//    toolLib.syncCommandBin("setenforce 0");
    system("chmod 666 /dev/uinput");
//    chmod("/dev/uinput",0666);
    uinp_fd = toolLib.createTouchEvent(uinp_fd, uinp);
//    //创建手指缩放
//    uinp_fd =toolLib.create_virtual_touchscreen(uinp_fd, rc, uinp);
    jfieldID instanceFieldId;
    jmethodID instanceMethod;
    const char *functionName1 = "showToast";
    const char *type1 = "()V";
    instanceMethod = (*env).GetMethodID(clazz, functionName1, type1);
    env->CallVoidMethod(thiz, instanceMethod
    );
}
int id = 1;

void nvr_execute_touch(int fd, int startX, int startY, int endX, int endY) {
    //actionDown事件
    toolLib.device_writeEvent(fd, EV_ABS, ABS_MT_TRACKING_ID, global_tracking_id++);
    toolLib.device_writeEvent(fd, EV_KEY, KEY_DOWN, startX);
    toolLib.device_writeEvent(fd, EV_KEY, KEY_X, startX);
    toolLib.device_writeEvent(fd, EV_KEY, KEY_Y, startY);
    toolLib.device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_X, startX);
    toolLib.device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_Y, startY);
    toolLib.device_writeEvent(fd, EV_ABS, ABS_MT_PRESSURE, 60);
    toolLib.device_writeEvent(fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 5);
    toolLib.device_writeEvent(fd, EV_KEY, KEY_UP, startX);
    toolLib.device_writeEvent(fd, EV_SYN, SYN_REPORT, 0);

    //action_move事件
    toolLib.device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_X, endX);
    toolLib.device_writeEvent(fd, EV_ABS, ABS_MT_POSITION_Y, endY);
    toolLib.device_writeEvent(fd, EV_SYN, SYN_REPORT, 0);

    //action_up事件
    toolLib.device_writeEvent(fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
    //事件发送完毕需要sync
    toolLib.device_writeEvent(fd, EV_SYN, SYN_REPORT, 0);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_action_androidq_NativeAction_touchXY(JNIEnv *env, jobject thiz, jint startx, jint starty,
                                              jint endx, jint endy) {
    sleep(1);
    int x = rand() % 1536 + 1;
    int y = rand() % 2048 + 1;
//    nvr_execute_touch(uinp_fd, 888, 1888, 999, 1988);
    toolLib.device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_X, x);
    toolLib.device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_Y, y);
    toolLib.device_writeEvent(uinp_fd, EV_SYN, SYN_MT_REPORT, 0);
    toolLib.device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);
    toolLib.device_writeEvent(uinp_fd, EV_SYN, SYN_MT_REPORT, 0);
    toolLib.device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);
    printf("global_tracking_id====%d", global_tracking_id);
    LOGE("XXXX=======================:%d", x);
    LOGE("YYYY=======================:%d", y);
//    //执行一次后关闭,需要再次开启
//    sleep(3);
//    ioctl(uinp_fd, UI_DEV_DESTROY);
//    close(uinp_fd);
    ALOGD("文件===========Nnvr_execute_touch%s", "执行");
}