#include "common.h"

void* __o__keypress;

void KeypressDetour(int key, bool held) {
    std::cout << "key pressed " << key << " " << held << std::endl;

    return CallFunc<void, int, bool>(__o__keypress, key, held);
}

void Entry(HANDLE module)
{
    Console::CreateConsole("TemplateCheat");

    // keyboard feed method (https://www.youtube.com/watch?v=Cpmfp5KvDYk&t=576s)
    {
        uintptr_t keymapAddr = findOffset("48 83 EC 48 ? ? C1 4C 8D", "Keymap");

        // CTRL + click
        HookFunction((void*)keymapAddr, &KeypressDetour, &__o__keypress); // this also returns a boolean if it fails
    }
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