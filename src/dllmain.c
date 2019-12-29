#include <sdkddkver.h>
#include <windows.h>

extern BOOL StartSender();

DWORD WINAPI _StartSender(LPVOID unused)
{
    // We are starting this on a new thread because it could block waiting
    // indefinitely until a pipe server is available. Because DLL notifications
    // are serialized, entry-point blocking functions may cause a deadlock if
    // run on the original DLL thread by effectively preventing it from returning.
    return StartSender();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        CreateThread(0, 0, _StartSender, NULL, 0, NULL);
        break;
    case DLL_PROCESS_DETACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }

    return TRUE;
}
