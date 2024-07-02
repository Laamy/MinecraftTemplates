#include "common.h"

void Init(HANDLE module) {
    Console::CreateConsole("TemplateCheat");

    // Inject once to setup files then reinject to print data from the client files

    if (FileIO::setupClientPath())
    {
        // means the folder just got created
        // lets add the things we want by default here
        FileIO::createPath("Workspace");
        FileIO::createPath("Script");
        FileIO::createPath("Data");

        FileIO::writeFile("Data\\data.txt", "Hello, World!");

        std::cout << "setup files" << std::endl;
    }

    std::cout << FileIO::readFile("Data\\data.txt", true) << std::endl;
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved)
{
    if (reason == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, module, 0, 0);
    }
    return TRUE;
}