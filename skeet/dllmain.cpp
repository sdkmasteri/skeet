#include "pch.h"
#include "skCrypter.h"

static void thread(HMODULE base) {
#ifdef DLOG
    system("cls");
#endif
    skeet_t* skeet = skeet_t::getInstance(base);
    if (!skeet->map())
        std::cout << skCrypt("[INFO] failed to allocate memory, reboot PC\n");
    while (GetModuleHandleA("serverbrowser.dll") == 0);
    skeet->fix_imports();
    std::cout << skCrypt("[INFO] fixed!\n");
    skeet->extra();
    skeet->entry();
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
#ifdef DLOG
        AllocConsole();
        SetConsoleTitleA("SkeetCrack");
        freopen("CONOUT$", "w", stdout);
#endif
        DisableThreadLibraryCalls(hModule);
        CloseHandle(CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(thread), hModule, 0, 0));
        break;
    }
    return TRUE;
}

