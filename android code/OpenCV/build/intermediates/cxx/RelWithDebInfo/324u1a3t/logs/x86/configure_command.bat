@echo off
"C:\\Users\\User\\AppData\\Local\\Android\\Sdk\\cmake\\3.22.1\\bin\\cmake.exe" ^
  "-HD:\\codes\\PROJECTS\\third year\\second term\\embedded systems\\Carcotech\\android code\\OpenCV\\libcxx_helper" ^
  "-DCMAKE_SYSTEM_NAME=Android" ^
  "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON" ^
  "-DCMAKE_SYSTEM_VERSION=21" ^
  "-DANDROID_PLATFORM=android-21" ^
  "-DANDROID_ABI=x86" ^
  "-DCMAKE_ANDROID_ARCH_ABI=x86" ^
  "-DANDROID_NDK=C:\\Users\\User\\AppData\\Local\\Android\\Sdk\\ndk\\23.1.7779620" ^
  "-DCMAKE_ANDROID_NDK=C:\\Users\\User\\AppData\\Local\\Android\\Sdk\\ndk\\23.1.7779620" ^
  "-DCMAKE_TOOLCHAIN_FILE=C:\\Users\\User\\AppData\\Local\\Android\\Sdk\\ndk\\23.1.7779620\\build\\cmake\\android.toolchain.cmake" ^
  "-DCMAKE_MAKE_PROGRAM=C:\\Users\\User\\AppData\\Local\\Android\\Sdk\\cmake\\3.22.1\\bin\\ninja.exe" ^
  "-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=D:\\codes\\PROJECTS\\third year\\second term\\embedded systems\\Carcotech\\android code\\OpenCV\\build\\intermediates\\cxx\\RelWithDebInfo\\324u1a3t\\obj\\x86" ^
  "-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=D:\\codes\\PROJECTS\\third year\\second term\\embedded systems\\Carcotech\\android code\\OpenCV\\build\\intermediates\\cxx\\RelWithDebInfo\\324u1a3t\\obj\\x86" ^
  "-DCMAKE_BUILD_TYPE=RelWithDebInfo" ^
  "-BD:\\codes\\PROJECTS\\third year\\second term\\embedded systems\\Carcotech\\android code\\OpenCV\\.cxx\\RelWithDebInfo\\324u1a3t\\x86" ^
  -GNinja ^
  "-DANDROID_STL=c++_shared"
