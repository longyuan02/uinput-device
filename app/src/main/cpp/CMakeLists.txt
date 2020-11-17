# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)
project("TEST")

add_library(fnative-lib SHARED native-lib.cpp)
add_library(tool-lib SHARED tool_lib.cpp)
find_library(log-lib log)
target_link_libraries(fnative-lib ${log-lib})
#将log添加到tool-lib库
target_link_libraries(tool-lib ${log-lib})