//
// Created by zjl on 20-11-10.
//

#include "tool_lib.h"
//wait和waitpid函数
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <linux/input.h>

using namespace std;
int fd;

tool_lib::tool_lib(int input_fd) {
    fd = input_fd;
}

//创建触摸屏 通过systemproperty保存fd
int tool_lib::createTouchScreen(int uinp_fd, uinput_user_dev uinp) const {
    uinp_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (uinp_fd <= 0) {
        ALOGE("Unable to open /dev/uinput======:%d", uinp_fd);
        return 0;
    }
    // configure touch device event properties
    memset(&uinp, 0, sizeof(uinp));
    //设备的别名
    strncpy(uinp.name, "ShaoTouchScreen", UINPUT_MAX_NAME_SIZE);
    uinp.id.version = 4;
    uinp.id.bustype = BUS_VIRTUAL;
    uinp.absmin[ABS_MT_SLOT] = 0;
    uinp.absmax[ABS_MT_SLOT] = 9; // MT代表multi touch 多指触摸 最大手指的数量我们设置9
    uinp.absmin[ABS_MT_TOUCH_MAJOR] = 0;
    uinp.absmax[ABS_MT_TOUCH_MAJOR] = 15;
    uinp.absmin[ABS_MT_POSITION_X] = 0; // 屏幕最小的X尺寸
    uinp.absmax[ABS_MT_POSITION_X] = 1020; // 屏幕最大的X尺寸
    uinp.absmin[ABS_MT_POSITION_Y] = 0; // 屏幕最小的Y尺寸
    uinp.absmax[ABS_MT_POSITION_Y] = 1020; //屏幕最大的Y尺寸
    uinp.absmin[ABS_MT_TRACKING_ID] = 0;
    uinp.absmax[ABS_MT_TRACKING_ID] = 65535;//按键码ID累计叠加最大值
    uinp.absmin[ABS_MT_PRESSURE] = 0;
    uinp.absmax[ABS_MT_PRESSURE] = 255;     //屏幕按下的压力值

    // Setup the uinput device
    ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);   //该设备支持按键
    ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);   //支持鼠标

    // Touch
    ioctl(uinp_fd, UI_SET_EVBIT, EV_ABS); //支持触摸
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_SLOT);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_TOUCH_MAJOR);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_PRESSURE);
    ioctl(uinp_fd, UI_SET_PROPBIT, INPUT_PROP_DIRECT);

    char str[100];
    memset(str, 0, sizeof(str));
    sprintf(str, "%d", uinp_fd);

    __system_property_set("nvr_touch_screen_device", str);
    ALOGD("nvr touch screen device strfd = %s , id = %d\n", str, uinp_fd);

    /* Create input device into input sub-system */
    write(uinp_fd, &uinp, sizeof(uinp));
    ioctl(uinp_fd, UI_DEV_CREATE);
    return uinp_fd;
}

int tool_lib::createTouchEvent(int uinp_fd, uinput_user_dev uinp) const {
    uinp_fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (uinp_fd <= 0) {
        ALOGE("unopen file======", "/dev/uinput");
        return 0;
    }
    memset(&uinp, 0, sizeof(uinp));
    //设备的别名
    strncpy(uinp.name, "cCTouchEvent", UINPUT_MAX_NAME_SIZE);
    uinp.id.version = 4;
//    uinp.id.bustype = BUS_USB;
    uinp.id.bustype = BUS_VIRTUAL;
    uinp.absmin[ABS_MT_SLOT] = 0;
    uinp.absmax[ABS_MT_SLOT] = 9; // MT代表multi touch 多指触摸 最大手指的数量我们设置9
    uinp.absmin[ABS_MT_TOUCH_MAJOR] = 0;
    uinp.absmax[ABS_MT_TOUCH_MAJOR] = 15;
    uinp.absmin[ABS_MT_POSITION_X] = 0; // 屏幕最小的X尺寸
    uinp.absmax[ABS_MT_POSITION_X] = 1536; // 屏幕最大的X尺寸
    uinp.absmin[ABS_MT_POSITION_Y] = 0; // 屏幕最小的Y尺寸
    uinp.absmax[ABS_MT_POSITION_Y] = 2048; //屏幕最大的Y尺寸
    uinp.absmin[ABS_MT_TRACKING_ID] = 0;
    uinp.absmax[ABS_MT_TRACKING_ID] = 65535;//按键码ID累计叠加最大值
    uinp.absmin[ABS_MT_PRESSURE] = 0;
    uinp.absmax[ABS_MT_PRESSURE] = 55;     //屏幕按下的压力值

    // Setup the uinput device
    ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);   //该设备支持按键
    ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);   //支持鼠标
    // Touch
    ioctl(uinp_fd, UI_SET_EVBIT, EV_ABS); //支持触摸
    ioctl(fd, UI_SET_KEYBIT, BTN_TOUCH);

    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_SLOT);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_TOUCH_MAJOR);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_WIDTH_MAJOR);//ABS_MT_WIDTH_MAJOR
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_X);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_Y);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_POSITION_X);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y);
    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID);
//    ioctl(uinp_fd, UI_SET_ABSBIT, ABS_MT_PRESSURE);
    ioctl(uinp_fd, UI_SET_PROPBIT, INPUT_PROP_DIRECT);
    ioctl(uinp_fd, UI_SET_KEYBIT, KEY_DOWN);
    ioctl(uinp_fd, UI_SET_KEYBIT, KEY_UP);
    ioctl(uinp_fd, UI_SET_KEYBIT, KEY_X);
    ioctl(uinp_fd, UI_SET_KEYBIT, KEY_Y);
    char str[20];
    memset(str, 0, sizeof(str));
    sprintf(str, "%d", uinp_fd);
//
    __system_property_set("nvr_touch_screen_device", str);
    ALOGD("nvr touch screen device strfd = %s , id = %d\n", str, uinp_fd);

    /* Create input device into input sub-system */
    write(uinp_fd, &uinp, sizeof(uinp));
    ioctl(uinp_fd, UI_DEV_CREATE);
    return uinp_fd;
}

/**
 * 同步
 * @param cmdstring
 * @return
 */
int tool_lib::syncCommandBin(const char *cmdstring) const {
    ALOGE("syncCommandBin", "======");
    pid_t pid;
    int status = 10;
    if (cmdstring == NULL) {

        return (1);
    }
    if ((pid = fork()) < 0) {
        status = -1;
    } else if (pid == 0) {
        execl("/system/bin/sh", "sh", "-c", cmdstring, (char *) 0);

    } else {
        while (waitpid(pid, &status, 0) < 0) {
            break;
        }
    }
    return status;
}

int tool_lib::syncCommandXBin(const char *cmdstring) const {
    ALOGE("syncCommandBin", "===XXX===");
    pid_t pid;
    int status = 10;
    if (cmdstring == NULL) {

        return (1);
    }
    if ((pid = fork()) < 0) {
        status = -1;
    } else if (pid == 0) {
        execl("/system/xbin/su", "sh", "-c", cmdstring, (char *) 0);

    } else {
        while (waitpid(pid, &status, 0) < 0) {
            break;
        }
    }
    return status;
}

void tool_lib::print() const {
    ALOGD("===========tool_lib::print%s", "print");
}

bool tool_lib::device_writeEvent(int fd, uint16_t type, uint16_t keycode, int32_t value) const {
    struct input_event ev;

    memset(&ev, 0, sizeof(struct input_event));

    ev.type = type;
    ev.code = keycode;
    ev.value = value;
    if (write(fd, &ev, sizeof(struct input_event)) < 0) {
        char *mesg = strerror(errno);
        ALOGE("nibiru uinput errormag info :%s\n", mesg);
        return false;
    }

    return true;
}

//startX startY 代表触摸down的坐标 endX 和 endY代表Up的坐标
//如果startX = startY 同时  endX = endY ，没有actionMove事件产生只有actionDown和actionUp事件
int tool_lib::nvr_execute_touch(int uinp_fd, int startX, int startY, int endX, int endY,
                                int global_tracking_id) const {

//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, global_tracking_id++);
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_X, startX);
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_Y, startY);
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_PRESSURE, 60);
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 5);
//    device_writeEvent(uinp_fd, EV_ABS, SYN_REPORT, 0);
//
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
//    device_writeEvent(uinp_fd, EV_ABS, SYN_REPORT, 0);
//

    //actionDown事件
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, global_tracking_id++);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_X, startX);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_Y, startY);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_PRESSURE, 60);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TOUCH_MAJOR, 5);
    device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);

//    //action_move事件
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_X, endX);
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_Y, endY);
//    device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);

    //action_up事件
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
    //事件发送完毕需要sync
    device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);
    ALOGE(" one touch operation send end");
    return global_tracking_id;
}

void write_event_to_device(int uinp_fd, unsigned char u8KeyCode, unsigned char u8Repeat) {
    struct input_event event; // Input device structure
    struct timespec s;
    s.tv_nsec = 5000000L;
    s.tv_sec = 0;
    memset(&event, 0x00, sizeof(event));
    gettimeofday(&event.time, 0);
    event.type = EV_KEY;
    event.code = u8KeyCode;
    event.value = 1;
    write(uinp_fd, &event, sizeof(event));

    memset(&event, 0x00, sizeof(event));
    gettimeofday(&event.time, 0);
    event.type = EV_KEY;
    event.code = u8KeyCode;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));
}

int tool_lib::EventDown(int uinp_fd, int global_tracking_id) const {
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_SLOT, 0);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, 20);
//    device_writeEvent(uinp_fd, EV_KEY, BTN_TOUCH, 1);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_X, 880);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_Y, 1000);
//    device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);
//    usleep(8000);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_SLOT, 1);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, 21);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_X, 920);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_Y, 1000);
    device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);
    //抬起点击事件
    //插槽0
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_SLOT, 0);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
    device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);
    //插槽1
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_SLOT, 1);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
    device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);

//    usleep(8000);

//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_SLOT, 0);
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_X, 840);
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_Y, 1000);
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_SLOT, 1);
//    usleep(8000);
//
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_X, 950);
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_Y, 1000);
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_SLOT, 0);
//    usleep(8000);
//
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_SLOT, 1);
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
////    device_writeEvent(uinp_fd, EV_KEY, BTN_TOUCH, 0);
//    device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);

    return global_tracking_id;
}


void changeFile() {
    FILE *file = fopen("/mnt/sdcard/create.txt", "r+w");
    if (file != NULL) {
        fputs("HELLO WORLD _AAAAAAAS!", file);
        fflush(file);
        fclose(file);
    } else {
        ALOGE("文件===========NULL========:%d", 11);
    }
}

/**
 * 创建手势放大缩小
 * @param fd
 * @param rc
 * @param uidev
 * @return
 */
int tool_lib::create_virtual_touchscreen(int fd, int rc, uinput_user_dev uidev) const {
    //打开uinput文件
    fd = open("/dev/uinput", O_WRONLY);
    if (fd < 0) {
        printf("Unable to open /dev/uinput\n");
        return -1;
    }

    //配置设备属性
    ioctl(fd, UI_SET_EVBIT, EV_ABS); //支持触摸
    ioctl(fd, UI_SET_EVBIT, EV_SYN); //支持同步，用于report
    ioctl(fd, UI_SET_EVBIT, EV_KEY);

    //multitouch对应的属性，即input_event结构体中的type
    ioctl(fd, UI_SET_ABSBIT, ABS_MT_SLOT); //支持几个触摸点
    ioctl(fd, UI_SET_ABSBIT, ABS_MT_TOUCH_MAJOR); //接触面直径大小，这里没用MINOR
    ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_X); //x坐标
    ioctl(fd, UI_SET_ABSBIT, ABS_MT_POSITION_Y); //y坐标
    ioctl(fd, UI_SET_ABSBIT, ABS_MT_TRACKING_ID); //对某一次触摸的标记,按键码ID
    ioctl(fd, UI_SET_ABSBIT, ABS_MT_PRESSURE); //触摸的压力

    //对当前设备设置以上属性的数值范围
    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "kydroid_touchscreen");
    //以上 kydroid_touchscreen 为设备名
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;

    uidev.absmin[ABS_MT_POSITION_X] = 0;
    uidev.absmax[ABS_MT_POSITION_X] = 1919;//坐标x的最大最小值，笔者的屏幕为显示器
    uidev.absfuzz[ABS_MT_POSITION_X] = 0;
    uidev.absflat[ABS_MT_POSITION_X] = 0;
    uidev.absmin[ABS_MT_POSITION_Y] = 0;
    uidev.absmax[ABS_MT_POSITION_Y] = 1199;//坐标y的最大最小值
    uidev.absfuzz[ABS_MT_POSITION_Y] = 0;
    uidev.absflat[ABS_MT_POSITION_Y] = 0;
    uidev.absmin[ABS_MT_PRESSURE] = 0;
    uidev.absmax[ABS_MT_PRESSURE] = 100;//触摸压力的最大最小值
    uidev.absfuzz[ABS_MT_PRESSURE] = 0;
    uidev.absflat[ABS_MT_PRESSURE] = 0;
    uidev.absmax[ABS_MT_SLOT] = 9; //同时支持最多9个触点
    uidev.absmax[ABS_MT_TOUCH_MAJOR] = 16; //与屏接触面的最大值
    uidev.absmax[ABS_MT_TRACKING_ID] = 65535; //按键码ID累计叠加最大值

    //利用以上属性创建设备
    rc = write(fd, &uidev, sizeof(uidev));
    if (rc != sizeof(uidev)) {
        printf("Unable to write uidev to fd\n");
        return -1;
    }

    rc = ioctl(fd, UI_DEV_CREATE);
    if (rc < 0) {
        printf("Unable to create UINPUT device.\n");
        return -1;
    }

//    while (1) {
//        sleep(3); //使程序一直运行，否则退出后设备就不存在了
//    }
    return fd;
}

int tool_lib::get_events_then_send(int eventType, int eventCode, int eventValue) const {
    struct input_event ev; //input_event定义在 /usr/include/linux/input.h
    memset(&ev, 0, sizeof(struct input_event));
    //将事件存入结构体
    ev.type = eventType;
    ev.code = eventCode;
    ev.value = eventValue;
    //写入到设备的事件文件/dev/uinput，在/dev/input/eventX文件中可查看
    if (write(fd, &ev, sizeof(struct input_event)) < 0) {
        char *mesg = strerror(errno);
        printf("nibiru uinput errormag info :%s\n", mesg);
        return -1;
    }


    return 1;
}

void tool_lib::img_change(int id, int type) const {
    int x0 = 600, y0 = 600, x1 = 1200, y1 = 600; //两个触点落下的位置
    //两个触点依次落下事件
    usleep(20 * 1000);//事件间隔
    id += 1;
    this->get_events_then_send(EV_ABS, ABS_MT_SLOT, 0);//轴最小值
    this->get_events_then_send(EV_ABS, ABS_MT_TRACKING_ID, id);
    this->get_events_then_send(EV_ABS, ABS_MT_TOUCH_MAJOR, 4);
    this->get_events_then_send(EV_ABS, ABS_MT_PRESSURE, 80);
    this->get_events_then_send(EV_ABS, ABS_MT_POSITION_X, x0);//落点x坐标
    this->get_events_then_send(EV_ABS, ABS_MT_POSITION_Y, y0);//落点y坐标
    this->get_events_then_send(EV_SYN, SYN_REPORT, 0);//事件分批次上报，以使输出设备做出反应
    usleep(1 * 1000);
    id += 1;
    this->get_events_then_send(EV_ABS, ABS_MT_SLOT, 1);
    this->get_events_then_send(EV_ABS, ABS_MT_TRACKING_ID, id);
    this->get_events_then_send(EV_ABS, ABS_MT_TOUCH_MAJOR, 5);
    this->get_events_then_send(EV_ABS, ABS_MT_PRESSURE, 80);
    this->get_events_then_send(EV_ABS, ABS_MT_POSITION_X, x1);
    this->get_events_then_send(EV_ABS, ABS_MT_POSITION_Y, y1);
    this->get_events_then_send(EV_SYN, SYN_REPORT, 0);

    //两个触点分别左右移动实现放大或缩小
    usleep(20 * 1000);
    for (int i = 0; i < 50; i++) {
        if (type == 0) { //放大
            x0 -= 3;
            x1 += 3;
        } else { //缩小
            x0 += 3;
            x1 -= 3;
        }

        //移动过后，两个触点所处的状态
        usleep(40 * 1000);
        this->get_events_then_send(EV_ABS, ABS_MT_SLOT, 0);
        this->get_events_then_send(EV_ABS, ABS_MT_PRESSURE, 80);
        this->get_events_then_send(EV_ABS, ABS_MT_POSITION_X, x0);
        this->get_events_then_send(EV_SYN, SYN_REPORT, 0);

        this->get_events_then_send(EV_ABS, ABS_MT_SLOT, 1);
        this->get_events_then_send(EV_ABS, ABS_MT_PRESSURE, 80);
        this->get_events_then_send(EV_ABS, ABS_MT_POSITION_X, x1);
        this->get_events_then_send(EV_SYN, SYN_REPORT, 0);
    }

    //两指依次离开屏幕
    usleep(10 * 1000);
    this->get_events_then_send(EV_ABS, ABS_MT_SLOT, 1);
    this->get_events_then_send(EV_ABS, ABS_MT_PRESSURE, 0);//压力变为0
    this->get_events_then_send(EV_ABS, ABS_MT_POSITION_X, x1);
    this->get_events_then_send(EV_SYN, SYN_REPORT, 0);
    this->get_events_then_send(EV_ABS, ABS_MT_TRACKING_ID, -1);//-1表示抬起
    this->get_events_then_send(EV_SYN, SYN_REPORT, 0);
    usleep(10 * 1000);
    this->get_events_then_send(EV_ABS, ABS_MT_SLOT, 0);
    this->get_events_then_send(EV_ABS, ABS_MT_PRESSURE, 0);
    this->get_events_then_send(EV_ABS, ABS_MT_POSITION_X, x0);
    this->get_events_then_send(EV_SYN, SYN_REPORT, 0);
    this->get_events_then_send(EV_ABS, ABS_MT_TRACKING_ID, -1);
    this->get_events_then_send(EV_SYN, SYN_REPORT, 0);
}

void tool_lib::protocolA(int uinp_fd) const {
    //    协议A
    int x = rand() % 1536 + 1;
    int y = rand() % 2048 + 1;
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_X, x);
    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_Y, y);
    device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);
//     device_writeEvent(uinp_fd, EV_SYN, SYN_MT_REPORT, 0);
//     device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);
//     device_writeEvent(uinp_fd, EV_SYN, SYN_MT_REPORT, 0);
//     device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);
}

//void tool_lib::protocolB(int uinp_fd) const {
//    int x = rand() % 1536 + 1;
//    int y = rand() % 2048 + 1;
//    //    //协议B
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_SLOT, 0);
//    device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, id++);
//     device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_X, x);
//     device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_Y, y);
//
//     device_writeEvent(uinp_fd, EV_ABS, ABS_MT_SLOT, 1);
//     device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, id++);
//     device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_X, x);
//     device_writeEvent(uinp_fd, EV_ABS, ABS_MT_POSITION_Y, y);
//     device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);
//    usleep(1 * 1000);
//     device_writeEvent(uinp_fd, EV_ABS, ABS_MT_SLOT, 0);
//     device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
//     device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);
//     device_writeEvent(uinp_fd, EV_ABS, ABS_MT_SLOT, 1);
//     device_writeEvent(uinp_fd, EV_ABS, ABS_MT_TRACKING_ID, -1);
//     device_writeEvent(uinp_fd, EV_SYN, SYN_REPORT, 0);
//}




