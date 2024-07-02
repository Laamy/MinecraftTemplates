#include "common.h"

void Init(HANDLE module) {
    Console::CreateConsole("TemplateCheat");

    std::cout << "Hello world!" << std::endl;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, module, 0, 0);
    }
    return TRUE;
}