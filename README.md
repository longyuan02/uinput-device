[TOC]
# 嵌入式开发基础
## 使用文件
1. MainActivity.kt//执行activity
2. native-lib.cpp //JNI 文件
3. tool_lib.cpp   //C++工具类
4. CMakeLists.txt //配置文件

### 执行注意事项
首先/dev/uinput开发需要获取系统权限:
1. manifests 文件中在添加android:sharedUserId="android.uid.system"   //详见demo
2. 需要制作系统签名 从源码中获取详见   [Android系统签名制作](https://blog.csdn.net/d773689630/article/details/108755562)
3. 执行前需要获取root权限以及关闭防火墙信息:
```命令终端顺序执行
    adb shell
    su
    setenforce 0;
    chmod /dev/uinput
```
4. 创建设备时注意:
```
    uinp.id.version = 4;
    uinp.id.bustype = BUS_VIRTUAL;
    uinp.absmin[ABS_MT_SLOT] = 0;
    uinp.absmax[ABS_MT_SLOT] = 9; // MT代表multi touch 多指触摸 最大手指的数量我们设置9
    uinp.absmin[ABS_MT_TOUCH_MAJOR] = 0;
    uinp.absmax[ABS_MT_TOUCH_MAJOR] = 15;
    /*absmin absmax 即:[absmax-absmin=0]两个轴取值如果等于0,视为设置无效XY轴,需要注意.
    *报错信息:Touch device 'cCTouchEvent' did not report support for X or Y axis!  The device will be inoperable.
    * 如果不设置absmin 默认为0*/
    uinp.absmin[ABS_MT_POSITION_X] = 0; // 屏幕最小的X尺寸
    uinp.absmax[ABS_MT_POSITION_X] = 1020; // 屏幕最大的X尺寸
    uinp.absmin[ABS_MT_POSITION_Y] = 0; // 屏幕最小的Y尺寸
    uinp.absmax[ABS_MT_POSITION_Y] = 1020; //屏幕最大的Y尺寸
    uinp.absmin[ABS_MT_TRACKING_ID] = 0;
    uinp.absmax[ABS_MT_TRACKING_ID] = 65535;//按键码ID累计叠加最大值
    uinp.absmin[ABS_MT_PRESSURE] = 0;
    uinp.absmax[ABS_MT_PRESSURE] = 255;     //屏幕按下的压力值
```
5. 查看设备信息:  
getevent -p   //获取注册设备信息
```
  # getevent -p                                                    
  add device 1: /dev/input/event10
  name:     "kydroid_touchscreen"
  events:
    ABS (0003): 002f  : value 0, min 0, max 9, fuzz 0, flat 0, resolution 0
                0030  : value 0, min 0, max 16, fuzz 0, flat 0, resolution 0
                0035  : value 0, min 0, max 1919, fuzz 0, flat 0, resolution 0
                0036  : value 0, min 0, max 1199, fuzz 0, flat 0, resolution 0
                0039  : value 0, min 0, max 65535, fuzz 0, flat 0, resolution 0
                003a  : value 0, min 0, max 100, fuzz 0, flat 0, resolution 0
  input props:
    <none>
add device 2: /dev/input/event5
  name:     "gpio-keys"
  events:
    KEY (0001): 0074 
  input props:
    <none>
```
getevent -i 查看设备详细信息
```
 # getevent -i
add device 1: /dev/input/event5
  bus:      0019
  vendor    0001
  product   0001
  version   0100
  name:     "gpio-keys"
  location: "gpio-keys/input0"
  id:       ""
  version:  1.0.1
  events:
    KEY (0001): 0074 
  input props:
    <none>
add device 2: /dev/input/event9
  bus:      0000
  vendor    0001
  product   0001
  version   0100
  name:     "rk-headset"
  location: ""
  id:       ""
  version:  1.0.1
  events:
    KEY (0001): 00e2 
  input props:
    <none>

```


getevent -l dev/input/event10   //获取指定输入设备输出
```
# getevent -l /dev/input/event10                               
EV_ABS       ABS_MT_POSITION_X    00000378            
EV_ABS       ABS_MT_POSITION_Y    00000760            
EV_SYN       SYN_MT_REPORT        00000000            
EV_ABS       ABS_MT_POSITION_X    00000382            
EV_ABS       ABS_MT_POSITION_Y    0000076a            
EV_SYN       SYN_MT_REPORT        00000000            
EV_SYN       SYN_REPORT           00000000  
```
### Android 多点触摸协议（Multi-touch Protocol）
[多点触摸协议文档](https://www.kernel.org/doc/Documentation/input/multi-touch-protocol.txt)
1. A协议
A协议：处理无关联的接触：用于直接发送原始数据；
```
/*
两点触摸数据上报如下：
系统以SYN_MT_REPORT为一个点信息的结尾，以SYN_REPORT为一个事件的结束，
系统受到一个点之后并不会立即处理，而是一个事件完成后才会处理；
*/
ABS_MT_POSITION_X x[0]
ABS_MT_POSITION_Y y[0]
SYN_MT_REPORT
ABS_MT_POSITION_X x[1]
ABS_MT_POSITION_Y y[1]
SYN_MT_REPORT
…
SYN_REPOR
/*
手抬起的时候，只有SYNC，没有其他任何信息，系统就会认为此次事件为UP；
*/
SYN_MT_REPORT 
SYN_REPORT  
```
>A协议比较简单，我们也可以发现在上面的序列中根本就没有轨迹跟踪的信息，有的只是点坐标等信息，
报上去的信息简单粗暴不需要什么顺序，系统根本无法识别报上的点是属于哪一条线的，事件过滤和手指跟踪的工作留给用户空间来实现。
2. B协议
B协议：处理跟踪识别类的接触：通过事件slot把每个接触点的单独更新发给接受者；
```
ABS_MT_SLOT 0 
ABS_MT_TRACKING_ID 46 
ABS_MT_POSITION_X x[0] 
ABS_MT_POSITION_Y y[0] 
ABS_MT_SLOT 1 
ABS_MT_TRACKING_ID 47 
ABS_MT_POSITION_X x[1] 
ABS_MT_POSITION_Y y[1] 
SYN_REPORT 
手抬起的时候如下样子： 
ABS_MT_SLOT 0 
ABS_MT_TRACKING_ID -1 
SYN_REPORT 
ABS_MT_SLOT 1 
ABS_MT_TRACKING_ID -1 
SYN_REPORT
```
>对于协议B，内核驱动应该把每一个识别出的触控和一个slot相关联，并使用该 slot来传播触摸状态的改变，
通过修改关联slot的ABS_MT_TRACKING_ID来达到对触摸点的创建，替换和销毁。
上报 ABS_MT_TRACKING_ID -1系统会清除对应的ID和slot，再次按下手指时分配新ID值（ID值是每次+1的）;
使用ABS_MT_TRACKING_ID，即当前序列中某点的ID值，用来跟踪当前点属于哪一条线；
如果与前一次序列中某点的ID值相等，那么他们就属于同一条线，应用层就不用在去劳心劳神的算那个点是那条线上的啦。
如果按下并一直按同一个点，那么input子系统会做个处理来屏蔽上下两次相同的点，减少IO的负担。
注：使用B协议的时候会产生一个问题，当使用QQ或微信<按住说话>说话功能的时候，当按住说话的时间大于灭屏时间的时候，会出现灭屏现象；  
**具体操作详见文档**
[多点触摸协议文档](https://www.kernel.org/doc/Documentation/input/multi-touch-protocol.txt)

### 补充知识-NDK-BUILD
[ndk-build资料](https://www.jianshu.com/p/f23df3aa342c)
1. Android.mk
* 构建共享库  
    ```include $(BUILD_SHARED_LIBRARY)   ```
* 构建多个共享库
```LOCAL_PATH  := $(call my-dir)
    #模块1
    include $(CLEAR_VARS)
    LOCAL_MODULE  := module1
    LOCAL_SRC_FILES  := module1.c
    include $(BUILD_SHARED_LIBRARY)
    #模块2
    include $(CLEAR_VARS)
    LOCAL_MODULE  := module2
    LOCAL_SRC_FILES  := module2.c
    include $(BUILD_SHARED_LIBRARY)
```
* 构建静态库-通常将静态库并入共享库中
```
    LOCAL_PATH  := $(call my-dir)
    #第三方库
    include $(CLEAR_VARS)
    LOCAL_MODULE  := jsoncpp
    LOCAL_SRC_FILES  := jsoncpp.cpp
    include $(BUILD_STATIC_LIBRARY)
    #原生模块
    include $(CLEAR_VARS)
    LOCAL_MODULE  := module
    LOCAL_SRC_FILES  := module.c
    LOCAL_STATIC_LIBRARIES  := jsoncpp
    include $(BUILD_SHARED_LIBRARY)
```
* 共享库通用模块
```
        LOCAL_PATH  := $(call my-dir)
        #第三方库
        include $(CLEAR_VARS)
        LOCAL_MODULE  := jsoncpp
        LOCAL_SRC_FILES  := jsoncpp.cpp
        include $(BUILD_SHARED_LIBRARY)
        #模块1
        include $(CLEAR_VARS)
        LOCAL_MODULE  := module1
        LOCAL_SRC_FILES  := module1.c
        LOCAL_SHARED_LIBRARIES  := jsoncpp
        include $(BUILD_SHARED_LIBRARY)
        #模块2
        include $(CLEAR_VARS)
        LOCAL_MODULE  := module2
        LOCAL_SRC_FILES  := module2.c
        LOCAL_SHARED_LIBRARIES  := jsoncpp
        include $(BUILD_SHARED_LIBRARY)
```
* 多个NDK项目间共享模块
    同时使用静态库和动态库时，可以在模块间共享通用代码，但这些都必须在同一个NDK项目中
    将共享模块移动到NDK项目以外的其他目录
    本身的Android.mk文件
```
        LOCAL_PATH  := $(call my-dir)
        #第三方库
        include $(CLEAR_VARS)
        LOCAL_MODULE  := jsoncpp
        LOCAL_SRC_FILES  := jsoncpp.cpp
        include $(BUILD_SHARED_LIBRARY)
```
为了避免构建系统冲突，应该将import-module函数宏放在Android.mk末尾
```
        LOCAL_PATH  := $(call my-dir)
        include $(CLEAR_VARS)
        LOCAL_MODULE  := module1
        LOCAL_SRC_FILES  := module1.c
        LOCAL_SHARED_LIBRARIES  := jsoncpp
        include $(BUILD_SHARED_LIBRARY)
        $(call import-module,code/jsoncpp)
```
import-module函数宏先定位共享模块，在将它导入到NDK项目中
* Prebuilt库
使用共享库的时候要求有共享库的源代码，Android NDK构建系统简单的把这些源文件包含在NDK项目中再次构建它们
预构建共享模块的Android.mk文件
```
        LOCAL_PATH  := $(call my-dir)
        #第三方库
        include $(CLEAR_VARS)
        LOCAL_MODULE  := jsoncpp
        LOCAL_SRC_FILES  := libjsoncpp.so
        include $(PREBUILT_SHARED_LIBRARY)
```
注意:Prebuilt库定义中不包含任何关于该库所构建的实际机器体系结构的信息，开发人员要确保Prebuilt库是为了与NDK项目相同的机器体系构建的
PREBUILT_SHARED_LIBRARY变量纸箱prebuilt-shared-library.mk Makefile片段。它什么都没有构建，但是它将Prebuilt库复制到了NDK项目的libs目录下。通过使用PREBUILT_STATIC_LIBRARY变量，静态库可以像共享库一样被用作Prebuilt库，NDK项目可以想普通共享库一样使用Prebuilt库了
```
        LOCAL_SHARED_LIBRARIES  :=jsoncpp
```
* 构建独立的可执行文件
为了方便测试，Android NDK也支持构建独立的可执行文件
```
        LOCAL_PATH  := $(call my-dir)
        #模块
        include $(CLEAR_VARS)
        LOCAL_MODULE  := module
        LOCAL_SRC_FILES  := module.c
        include $(BUILD_EXECUTABLE)
```
BUILD_EXECUTABLE变量指向build-executable.mk Makefile片段。
* 其他构建系统变量
```
        TARGET_ARCH：目标CPU体系结构的名称，例如arm
        TARGET_PLATFORM：目标Android平台的名称，例如：android-3
        TARGET_ARCH_ABI：目标CPU体系结构和ABI的名称，例如：armeabi-v7a
        TARGET_ABI：目标平台和ABI的串联，例如：android-3-armeabi-v7a
        LOCAL_MODULE_FILENAME：可选变量，用来重新定义生成的输出文件名称，默认情况下，构建系统使用LOCAL_MODULE的值来作为生成的输出文件名称，使用此变量可以覆盖
        LOCAL_CPP_EXTENSION：C++源文件的扩展名为.cpp，这个变量可以用来为C++源代码指定一个或多个扩展名
        LOCAL_CPP_EXTENSION  := .cpp cxx
        LOCAL_CPP_FEATURES：可选变量，用来指明模块所依赖的具体C++特性，如RTTI，exceptions等
        LOCAL_CPP_FEATURES  := rtti
        LOCAL_C_INCLUDES：可选目录列表，NDK安装目录的相对路径，用来搜索头文件
        LOCAL_C_INCLUDES  := sources/shared-module
        LOCAL_C_INCLUDES  := $(LOCAL_PATH)/include
        LOCAL_CFLAGS：一组可选的编译器标志，在编译C和C++源文件的时候会被传送给编译器
        LOCAL_CFLAGS  := -DNDEBUG -DPORT=1234
        LOCAL_CPP_FLAGS：一组可选的编译器标志，在编译C++源文件的时候会被传送给编译器
        LOCAL_WHOLE_STATIC_LIBRARIES：LOCAL_STATIC_LIBRARIES的变体，用来指明应该被包含在生成的共享库中所有静态库内容，几个静态库有循环依赖时，很有用
        LOCAL_LDLIBS：链接标志的可选列表，当对目标文件进行链接以生成输出文件时该标志将被传送给链接器。它主要用于传送要进行动态链接的系统库列表，例如要与Android NDK日志库链接，使用以下代码
        LOCAL_LDFLAGS  := -llog
        LOCAL_ALLOW_UNDEFINED_SYMBOLS：可选参数，它禁止在生成的文件中进行缺失符号检查，若没有定义，链接器会在符号缺失生成错误信息
        LOCAL_ARM_MODE：可选参数，ARM机器体系结构特有变量，用于指定要生成的ARM二进制类型
        LOCAL_ARM_MODE  := arm
        LOCAL_SRC_FILES  := file.c file2.c.arm
        LOCAL_ARM_NEON：可选参数，ARM机器体系特有变量，用来指定在源文件中使用的ARM高级单指令流内联函数
        LOCAL_ARM_NEON  := true
        LOCAL_SRC_FILES  := file.c file2.c.neon
        LOCAL_DIABLE_NO_EXECUTE：可选变量，用来禁用NX Bit安全特性，用来隔离代码区和存储区，防止恶意软件通过将它的代码注入到应用程序的存储区来控制应用程序
        LOCAL_DISABLE_NO_EXECUTE  := true
        LOCAL_EXPORT_CFLAGS：该变量记录一组编译器标志，这些编译器标志会被添加到通过变量LOCAL_STATIC_LIBRARIES或LOCAL_SHARED_LIBRARIES使用模块的其他模块LOCAL_CFLAGS定义中
        LOCAL_MODULE  := jsoncpp
        ...
        LOCAL_EXPORT_CFLAGS  := -DENABLE_AUDIO
        ...
        LOCAL_MODULE  := module
        LOCAL_CFLAGS  := -DDEBUG
        ...
        LOCAL_SHARED_LIBRARIES  := jsoncpp
        编译器在构建module的时候会以-DENABLE_AUDIO -DDEBUG标志执行
        LOCAL_EXPORT_CPPFLAGS：和LOCAL_EXPORT_CFLAGS一样，不过它是C++特定代码编译器标志
        LOCAL_EXPORT_LDFLAGS：和LOCAL_EXPORT_CFLAGS一样，不过用作链接器标志
        LOCAL_EXPORT_C_INCLUDES：该变量运行记录路径集，这些路径会被添加到通过变量LOCAL_STATIC_LIBRARIES或LOCAL_SHARED_LIBRARIES使用该模块的LOCAL_C_INCLUDES定义中
        LOCAL_SHORT_COMMANDS：对于有大量的资源或独立的静态/共享库的模块，该变量应该被设置为true
        LOCAL_FILTER_ASM：该变量定义了用于过滤来自LOCAL_SRC_FILES变量的装配文件的应用程序
```
* 其他的构建系统函数宏
```
        all-subdir-makefiles：返回当前目录的所有子目录下的Android.mk构建文件列表，例如，调用一下命令可以将子目录下的所有Android.mk文件包含在构建过程中:
        include $(call all-subdir-makefiles)
        this-makefile：返回当前Android.mk构建文件的路径
        parent-makefile：返回包含当前构建文件的父Android.mk构建文件的路径
        grand-parent-makefile：和parent-makefile一样但用于祖父目录
```
* 定义新变量
开发人员可以定义其他变量来简化构建文件，以LOCAL_和NDK_开头的名称预留给Android NDK构建系统使用。
```
        ...
        MY_SRC_FILES  ;= jsoncpp.cpp
        LOCAL_SRC_FILES  := $(addprefix jsoncpp/,$(MY_SRC_FILES))
        ...
```
* 条件操作
Android.mk构建文件也可以包含关于这些变量的条件操作
```
        ...
        ifeq ($(TARGET_ARCH),arm)
              LOCAL_SRC_FILES  += abc.c
        else
              LOCAL_SRC_FILES  += xyz.c
        endif
        ...
```
2. Application.mk
它是Android NDK构建系统使用的一个可选的构建文件，也是一个GNU Makefile片段
下面是Application.mk构建文件支持的变量：
```
        APP_MODULES：默认情况下，Android NDK构建系统构建Android.mk文件声明的所有模块。
        该变量可以覆盖上述行为并提供一个空格分开的，需要被构建的模块列表
        
        APP_OPTIM：该变量可以设置为release和debug来改变生成的二进制文件的优化级别，
        默认为release模式。
        
        APP_CLAGS：列出编译器标识，在编译任何模块的C和C++源文件时这些标志都会被传给编译器
        
        APP_BUILD_SCRIPT：默认情况下，Android NDK构建系统在项目的JNI子目录下查找Android.mk构建文件。
        可以用该变量改变上述行为，并使用不同的生成文件
        
        APP_ABI：默认情况下，Android NDK构建系统为armeabi ABI生成二进制文件，
        可以用该变量改变上述行为，并为其他ABI生成二进制文件：例如
        APP_ABI  := mips
        另外可以设置多个ABI
        APP_ABI  := armeabi mips
        为所有支持的ABI生成二进制文件
        APP_ABI  := all
        
        APP_STL：默认情况下，Android NDK构建系统使用最小
        STL运行库，也被成为system库。可以用该变量选择不同的
        STL实现。
        APP_STL  := stlport_shared
        
        APP_GNUSTL_FORCE_CPP_FEATURES：与LOCAL_CPP_EXTENSIONS变量类似，改变量表明所有模
        块都依赖于具体的C++特性，如RTTI，exceptions等。
        APP_SHORT_COMMANDS：与LOCAL_SHORT_COMMANDS变量相似，该变量使得构建
        系统存在有大量源文件的情况下可以在项目里使用更短的命令。
```

### 补充知识-CMake
[CMake资料](https://www.cnblogs.com/qixingchao/p/11911764.html)
```
        #cmake最低版本要求
        cmake_minimum_required(VERSION 3.4.1)
        #添加库
        add_library(
                # 库名
                native-lib
                # 类型：
                # SHARED 是指动态库，对应的是.so文件
                # STATIC 是指静态库，对应的是.a文件
                # 其他类型:略
                SHARED
                # native类路径
                native-lib.cpp)
        
        # 查找依赖库
        find_library( 
                # 依赖库别名
                log-lib
                # 希望加到本地的NDK库名称，log指NDK的日志库
                log)
        # 链接库，建立关系（ 此处就是指把log-lib 链接给 native-lib使用 ）
        target_link_libraries( 
                # 目标库名称（native-lib 是咱们要生成的so库）
                native-lib
                # 要链接的库（log-lib 是上面查找的log库）
                ${log-lib})
```
CMakeList.txt可配置的内容远不止这些，如：so输出目录，生成规则等等，有需要的同学可查下官网。这里只做一个基本注释

app的gradle配置CMake
```
apply plugin: 'com.android.application'

android {
    compileSdkVersion 29
    buildToolsVersion "29.0.1"
    defaultConfig {
        applicationId "com.xxx.xxx"
        minSdkVersion 21
        targetSdkVersion 29
        versionCode 1
        versionName "1.0"
        testInstrumentationRunner "androidx.test.runner.AndroidJUnitRunner"
        //定义cmake默认配置属性
        externalNativeBuild {
            cmake {
                cppFlags ""
            }
        }
    }
    //定义cmake对应的CMakeList.txt路径（重要）
    externalNativeBuild {
        cmake {
            path "src/main/cpp/CMakeLists.txt"
        }
    }
}
dependencies {
    implementation fileTree(dir: 'libs', include: ['*.jar'])
    implementation 'androidx.appcompat:appcompat:1.1.0'
    implementation 'androidx.constraintlayout:constraintlayout:1.1.3'
    testImplementation 'junit:junit:4.12'
    androidTestImplementation 'androidx.test.ext:junit:1.1.1'
    androidTestImplementation 'androidx.test.espresso:espresso-core:3.2.0'
}
```
指定库文件的输出目录
如果希望将so库生成到特定目录，并让项目直接使用该目录下的so，应该如何配置呢？
比较简单：需要在CMakeList.txt中配置库的输出路径信息，即：
>CMAKE_LIBRARY_OUTPUT_DIRECTORY
```
# cmake最低版本要求
cmake_minimum_required(VERSION 3.4.1)

# 配置库生成路径
# CMAKE_CURRENT_SOURCE_DIR是指 cmake库的源路径，通常是build/.../cmake/
# /../jniLibs/是指与CMakeList.txt所在目录的同级目录：jniLibs (如果没有会新建)
# ANDROID_ABI 生成库文件时，采用gradle配置的ABI策略（即：生成哪些平台对应的库文件）
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../jniLibs/${ANDROID_ABI})

# 添加库
add_library( # 库名
        native-lib

        # 类型：
        # SHARED 是指动态库，对应的是.so文件
        # STATIC 是指静态库，对应的是.a文件
        # 其他类型:略
        SHARED

        # native类路径
        native-lib.cpp)

# 查找依赖库
find_library(
        # 依赖库别名
        log-lib

        # 希望加到本地的NDK库名称，log指NDK的日志库
        log)


# 链接库，建立关系（ 此处就是指把log-lib 链接给native-lib使用 ）
target_link_libraries(
        # 目标库名称（native-lib就是咱们要生成的so库）
        native-lib

        # 要链接的库（上面查找的log库）
        ${log-lib})
```
在gradle中配置 jniLibs.srcDirs 属性（即：指定了lib库目录）：
```
sourceSets {
        main {
            jniLibs.srcDirs = ['jniLibs']//指定lib库目录
        }
    }
```
```
注意事项：
1、配置了CMAKE_CURRENT_SOURCE_DIR，并非表示编译时直接将so生成在该目录中，实际编译时，so文件仍然是
先生成在build/.../cmake/中，然后再拷贝到目标目录中的

2、如果只配置了CMAKE_CURRENT_SOURCE_DIR，并未在gradle中配置 jniLibs.srcDirs，也会有问题，如下：
More than one file was found with OS independent path 'lib/arm64-v8a/libnative-lib.so'

此问题是指：在编译生成apk时，发现了多个so目录，android studio不知道使用哪一个了，所以需要咱们
告诉android studio当前工程使用的是jniLibs目录，而非build/.../cmake/目录
```
可以在cmake中设置abiFilters，也可在ndk中设置abiFilters，效果是一样的：
```
defaultConfig {
        applicationId "com.qxc.testnativec"
        minSdkVersion 21
        targetSdkVersion 29
        versionCode 1
        versionName "1.0"
        testInstrumentationRunner "androidx.test.runner.AndroidJUnitRunner"
        externalNativeBuild {
            cmake {
                cppFlags ""
                abiFilters "arm64-v8a"
            }
        }
    }
```
```
defaultConfig {
        applicationId "com.qxc.testnativec"
        minSdkVersion 21
        targetSdkVersion 29
        versionCode 1
        versionName "1.0"
        testInstrumentationRunner "androidx.test.runner.AndroidJUnitRunner"
        externalNativeBuild {
            cmake {
                cppFlags ""
            }
        }
        ndk {
            abiFilters "arm64-v8a"
        }
    }
```