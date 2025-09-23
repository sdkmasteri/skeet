#include "pch.h"
#include "skCrypter.h"

static void thread(HMODULE base) {
    skeet_t* skeet = skeet_t::getInstance(base);
    if (!skeet->map())
      std::cout << skCrypt("[t.me/g00d_crack] failed to allocate memory, reboot PC\n");
    while (GetModuleHandleA("serverbrowser.dll") == 0);
    system("cls");
    std::cout << skCrypt("[t.me/g00d_crack] cracked by qhide.\n$$$ Buy SUGAR ELITE for EFT $$$\nSubscribe to t.me/raze_club");
    skeet->fix_imports();
    std::cout << skCrypt("[t.me/g00d_crack] fixed!\n");
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
        AllocConsole();
        SetConsoleTitleA("t.me/g00d_crack / t.me/raze_club");
        freopen("CONOUT$", "w", stdout);
        DisableThreadLibraryCalls(hModule);
        CloseHandle(CreateThread(0, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(thread), 0, 0, 0));
        break;
    }
    return TRUE;
}

