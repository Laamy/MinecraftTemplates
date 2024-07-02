#include "common.h"

void Entry(HANDLE module)
{
    Console::CreateConsole("TemplateCheat");
    InitHooks();
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        // disable multi-inject (useless stuff)
        DisableThreadLibraryCalls(module);

        // main thread
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Entry, module, 0, 0);
    }
    return TRUE;
}