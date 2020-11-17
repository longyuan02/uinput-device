//
// Created by zjl on 20-11-10.
//

#ifndef ANDROIDQ_TOOL_LIB_H
#define ANDROIDQ_TOOL_LIB_H

#include <jni.h>
#include <string>
#include <stdint.h>
#include <android/log.h>
#include <linux/input.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/system_properties.h>
#include <linux/uinput.h>
//wait和waitpid函数

#define LOG_TAG "tool-lib"
#define ALOGD(...)__android_log_print(ANDROID_LOG_DEBUG,__VA_ARGS__)
#define ALOGES(...)__android_log_print(ANDROID_LOG_DEBUG,__VA_ARGS__)
#define ALOGE(...)__android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__) // 定义LOGE类型

class tool_lib {
public:
    tool_lib(int input_fd);

    /**
     * 创建touch事件
     * @param uinp_fd
     * @param uinp
     * @return
     */
    int createTouchEvent(int uinp_fd, uinput_user_dev uinp) const;

/**
 * 创建触摸屏
 * @param uinp_fd
 * @param uinp
 * @return
 */
    int createTouchScreen(int uinp_fd, uinput_user_dev uinp) const;

/**
 * 执行 adb命令
 * @param cmdstring
 * @return
 */
    int syncCommandBin(const char *cmdstring) const;

/**
 * 执行 su
 * @param cmdstring
 * @return
 */
    int syncCommandXBin(const char *cmdstring) const;

    void print() const;

    int nvr_execute_touch(int uinp_fd, int startX, int startY, int endX, int endY,
                          int global_tracking_id) const;

    void write_event_to_device(int uinp_fd, unsigned char u8KeyCode, unsigned char u8Repeat) const;

    void changeFile() const;

    int EventDown(int uinp_fd, int global_tracking_id) const;


    //创建放大缩小收拾操作
    int create_virtual_touchscreen(int fd, int rc, uinput_user_dev uidev) const;

    //写入单个事件
    int get_events_then_send(int eventType, int eventCode, int eventValue) const;

    //事件创建序列
    void img_change(int id, int type) const;

    bool device_writeEvent(int fd, uint16_t type, uint16_t keycode, int32_t value) const;

    void protocolA(int uinp_fd) const;

    void protocolB(int uinp_fd) const;
};


#endif //ANDROIDQ_TOOL_LIB_H
