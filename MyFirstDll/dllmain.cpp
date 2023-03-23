// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include <cstdio>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            MessageBox(NULL, TEXT("INJECTED"), TEXT("INJECTED!!"), MB_OK | MB_ICONWARNING);
            break;
        case DLL_THREAD_ATTACH:
            //MessageBox(NULL, TEXT("INJECTED"), TEXT("INJECTED!!"), MB_OK | MB_ICONWARNING);
            break;
        case DLL_PROCESS_DETACH:
            //MessageBox(NULL, TEXT("INJECTED"), TEXT("INJECTED!!"), MB_OK | MB_ICONWARNING);
            break;
        case DLL_THREAD_DETACH:
            //MessageBox(NULL, TEXT("INJECTED"), TEXT("INJECTED!!"), MB_OK | MB_ICONWARNING);
            break;
    }
    


    return TRUE;
}

