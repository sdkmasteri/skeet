#include "pch.h"
#include "SkeetSDK/skeetsdk.h"

using namespace SkeetSDK;

static void load_settings()
{
    InitAndWaitForSkeet();

    HKEY regkey;
    if (RegOpenKeyW(HKEY_CURRENT_USER, L"SOFTWARE", &regkey) != ERROR_SUCCESS) return;

    static void(__thiscall* setpossize)(CMenu*, uint32_t&) = reinterpret_cast<decltype(setpossize)>(Memory::CheatChunk.find("56 57 8B 7C 24 ?? 8B F1 ?? ?? ?? 0F 85"));
    static void(__thiscall* apply_dpi)(bool) = reinterpret_cast<decltype(apply_dpi)>(Memory::CheatChunk.find("55 8B EC 83 E4 ?? A1 ?? ?? ?? ?? 53"));

    DWORD size = 0x400;
    uint8_t* data = Memory::bytealloc.allocate(size);

    DWORD keytype;
    if (RegQueryValueExW(regkey, Menu->RegValueName, 0, &keytype, data, &size) == ERROR_SUCCESS)
    {
        if (keytype != REG_BINARY)
        {
            RegDeleteValueW(HKEY_CURRENT_USER, Menu->RegValueName);
            goto end;
        };

        auto* cursor = data;
        ConfigHead* header = reinterpret_cast<ConfigHead*>(cursor);

        if (size > sizeof(ConfigHead) && header->sig == SKEET_HEAD_SIGNATURE)
        {
            while ((uintptr_t)cursor - (uintptr_t)header < size)
            {
                ConfigDataUnit* cfg_unit = reinterpret_cast<ConfigDataUnit*>(cursor + sizeof(ConfigHead));
                switch (cfg_unit->data_type)
                {
                case LCOLOR:
                    *(uint32_t*)0x43468FB0 = cfg_unit->get_ref<uint32_t>();
                    Menu->Tabs[MISC]->Childs[2]->Elements[3]->GetAs<CPicker>()->OnConfigLoad();
                    break;
                case LPOSSIZE:
                    setpossize(Menu, cfg_unit->get_ref<uint32_t>());
                    break;
                case LBOOL:
                    *(bool*)0x43475798 = cfg_unit->get_ref<bool>();
                    break;
                case LHOTKEY:
                    *(uint32_t*)0x43478DC8 = cfg_unit->get_ref<uint32_t>();
                    break;
                case LARRAY:
                    memcpy(Menu->OnStartupHash, cfg_unit->data, cfg_unit->data_size);
                    break;
                case LINTEGER:
                    switch (cfg_unit->hash)
                    {
                    case 0x1F495BA0:
                        Menu->Size.x = cfg_unit->get_ref<int>();
                        break;
                    case 0xEAA92CD1:
                        Menu->Size.y = cfg_unit->get_ref<int>();
                        break;
                    case 0x27BA18FA:
                        *(uint32_t*)0x43475A94 = cfg_unit->get_ref<uint32_t>();
                        apply_dpi(true);
                        break;
                    case 0xD3F1456E:
                        *(bool*)0x43467E4B = cfg_unit->get_ref<bool>();
                        break;
                    default:
                        LPRINT(std::hex << cfg_unit->hash << ' ' << std::dec << cfg_unit->get_ref<uint32_t>() << '\n');
                        break;
                    };
                    break;
                default:
                    break;
                };

                cursor += sizeof(ConfigDataUnit) + cfg_unit->data_size;
            };
        };
    }
end:
    RegCloseKey(regkey);
    Memory::bytealloc.deallocate(data, 0);
};


static decltype(load_settings)* fn = &load_settings;
__declspec(naked) __declspec(noreturn) void LoadStub()
{
    __asm
    {
        mov eax, fn
        call eax
        mov eax, 0x434938FF
        jmp eax
    };
};