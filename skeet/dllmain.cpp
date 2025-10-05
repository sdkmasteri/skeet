#include "pch.h"
#include "skCrypter.h"
//#define DLOG

unsigned int start_signal = 0; // indicates that skeet fully loaded
volatile HANDLE hthread = 0;
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

    while (!InterlockedCompareExchange(&start_signal, 0, 1))
        Sleep(0);

    std::cout << skCrypt("[INFO] entry succed, trying to load settings\n");

    skeet_t::load_settings();

    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), const_cast<HANDLE*>(&hthread), 0, FALSE, DUPLICATE_SAME_ACCESS);

    uint8_t* bp_adr = reinterpret_cast<uint8_t*>(0x4338414C);
    std::cout << skCrypt("[INFO] setting breakpoint at ") << static_cast<void*>(bp_adr) << '\n';
    *bp_adr = 0xCC;
    while (1)
    {
        SuspendThread(hthread);
        skeet_t::save_settings();
    };
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
    case DLL_PROCESS_DETACH:
        skeet_t::flush_settings();
        break;
    }
    return TRUE;
}

