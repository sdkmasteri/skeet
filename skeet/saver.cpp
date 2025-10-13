#include "pch.h"
#include "saver.h"
#include "SkeetSDK/skeetsdk.h"

// The main idea lies under saving cfg data after menu is closed(so no further interaction will be performed)
// and flush to disk on DLL_PROCESS_DETACH so it will be automated.

/*
    settings
    {
        menu_color: VecCol,
        menu_key: uint32_t,
        menu_dpi: uint32_t,
        lock_layout: bool,
        menu_size: Vec2,
        menu_pos: Vec2,
        menu_tab: uint32_t,
        cfg_hash: uint32_t,
        luas: array
    }
*/

namespace nlohmann {
    void to_json(json& j, const std::vector<std::unique_ptr<char[]>>& vec)
    {
        j = json::array();
        for (const auto& ptr : vec)
            j.push_back(ptr.get());
    }
}

#ifdef HOOK_SUBCLASS
SkeetSDK::Memory::CHooked* subhook = nullptr;
LRESULT WINAPI Subclassproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
#else
WNDPROC g_OriginalWindowProc = nullptr;
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif // HOOK_SUBCLASS

void __fastcall OnStartupCheckbox(void* ecx, void* ebx);

typedef int(__thiscall* IntThisFn)(void*);

nlohmann::json saver::settings;

std::vector<std::unique_ptr<char[]>> saver::luas;

static std::filesystem::path file_path = std::filesystem::current_path() / "csgo" / "cfg" / "crosshair_openmode.cfg";

static HWND* p_hWnd = reinterpret_cast<HWND*>(0x433A068A);

#pragma pack(push, 1)
struct LoadInfo
{
    char* cfgname;
    std::vector<std::unique_ptr<wchar_t[]>> luas;
    bool has_data;
};
#pragma pack(pop)
void saver::load_settings()
{
    using namespace SkeetSDK;
    InitAndWaitForSkeet();

#ifdef HOOK_SUBCLASS
    subhook = Memory::DetourHook::Hook(Memory::CheatChunk.find("55 8B EC 8B 45 ?? 83 EC ?? 56"), Subclassproc, 6);
#endif // HOOK_SUBCLASS

    if (!std::filesystem::exists(file_path) || std::filesystem::is_empty(file_path)) return;

    std::ifstream fs(file_path, std::ofstream::in | std::ifstream::binary);

    if (!fs.is_open()) return;

    auto* config_t = UI::GetTab(CONFIG);
    auto* misc_t = UI::GetTab(MISC);
    auto* setting_c = misc_t->Childs[2];
    auto* chunk_ptr = &config_t->Chunk;

    settings = nlohmann::json::parse(fs);
    fs.close();

    if (!settings.is_null())
    {
        LoadInfo* linfo = new LoadInfo;
        if (settings.contains("luas"))
        {
            nlohmann::json& luas = settings["luas"];
            if (luas.is_array())
            {
                Utils::InitConfig();
                Utils::AllowUnsafe(true);
#ifndef HOOK_SUBCLASS
                g_OriginalWindowProc = (WNDPROC)SetWindowLongPtr(*p_hWnd, GWLP_WNDPROC, (LONG)WndProc);
#endif // !HOOK_SUBCLASS
                linfo->has_data = true;
                linfo->luas.reserve(255);
                for (nlohmann::json& lua : luas)
                {
                    if (!lua.is_string()) continue;

                    std::unique_ptr<wchar_t[]> name = std::make_unique<wchar_t[]>(_MAX_FNAME);
                    std::string& strname = lua.get_ref<std::string&>();

                    size_t count = MultiByteToWideChar(CP_UTF8, 0, strname.c_str(), -1, name.get(), _MAX_FNAME);
                    if (!count)
                        continue;

                    name.get()[count] = L'\0';
                    linfo->luas.push_back(std::move(name));
                };
                linfo->luas.shrink_to_fit();

                // it will break skeet input sometimes
                //SetWindowLongPtr(*p_hWnd, GWLP_WNDPROC, (LONG)g_OriginalWindowProc);
            };
        };

        if (settings.contains("cfg"))
        {
            nlohmann::json& cfg = settings["cfg"];

            if (cfg.is_string())
            {
                linfo->has_data = true;
                const std::string& cfgnamestr = cfg.get_ref<std::string&>().c_str();
                size_t size = cfgnamestr.length();
                char* cfgname = new char[size + 1];
                memcpy(cfgname, cfgnamestr.c_str(), size);
                cfgname[size] = '\0';
                linfo->cfgname = cfgname;
            }
        };

        PostMessage(*p_hWnd, 0x1337, reinterpret_cast<WPARAM>(linfo), 0x7ADAEEF4);

        if (settings.contains("menu_pos"))
        {
            nlohmann::json& pos = settings["menu_pos"];
            if (pos.is_array())
                Menu->Pos = pos.get<Vec2>();
        };

        if (settings.contains("menu_size"))
        {
            nlohmann::json& size = settings["menu_size"];
            if (size.is_array())
            {
                Menu->Size = size.get<Vec2>();
                UI::ResetLayout();
            }
        };

        if (settings.contains("menu_tab"))
        {
            nlohmann::json& tab = settings["menu_tab"];
            if (tab.is_number_unsigned())
                UI::SetTab(tab.get<uint32_t>());
        };

        if (settings.contains("menu_color"))
        {
            nlohmann::json& color = settings["menu_color"];
            if (color.is_array())
               UI::SetMenuCol(color.get<VecCol>());
        };

        if (settings.contains("menu_dpi"))
        {
            nlohmann::json& dpi = settings["menu_dpi"];
            if (dpi.is_number_unsigned())
                UI::SetCombobox(setting_c->Elements[4]->GetAs<Combobox>(), dpi.get<uint32_t>());
        };

        if (settings.contains("lock_layout"))
        {
            nlohmann::json& layout = settings["lock_layout"];
            if (layout.is_boolean())
            {
                UI::SetCheckbox(setting_c->Elements[9]->GetAs<Checkbox>(), layout.get<bool>());
            };
        };

        if (settings.contains("menu_key"))
        {
            nlohmann::json& key = settings.at("menu_key");
            if (key.is_number_unsigned())
                UI::SetHotkey(UI::GetElement<Hotkey>(setting_c, 1), key.get<uint32_t>());
        };

    }
};

DWORD saver::LastSavedTick = 0;
static uint32_t atomiclock = 0;
void saver::save_settings()
{
    if (InterlockedCompareExchange(&atomiclock, 1, 0)) return;

    DWORD current_tick = GetTickCount();

    if ((current_tick - LastSavedTick) <= 500)
    {
        LastSavedTick = current_tick;
        InterlockedExchange(&atomiclock, 0);
        return;
    };

    using namespace SkeetSDK;

    auto* settings_c = UI::GetChild(MISC, 2);

    // harvesting data
    VecCol menu_color;
    uint32_t menu_key;
    uint32_t menu_dpi;
    bool lock_layout;
    Vec2 menu_size;
    Vec2 menu_pos;
    uint32_t menu_tab;
    char cfg[0x40];

    menu_color = UI::GetMenuCol();

    menu_dpi = *settings_c->Elements[4]->GetAs<Combobox>()->Value;

    lock_layout = *settings_c->Elements[9]->GetAs<Checkbox>()->Value;

    menu_size = Menu->Size;

    menu_pos = Menu->Pos;

    menu_tab = Menu->CurrentTab;

    menu_key = UI::ExtractKey(settings_c->Elements[1]->GetAs<Hotkey>());

    {
        const wchar_t* cfgnameW = Utils::CurrentConfig();
        size_t count = WideCharToMultiByte(CP_UTF8, 0, cfgnameW, -1, cfg, sizeof(cfg), 0, 0);
        cfg[count] = '\0';
    };


    {
        size_t lua_count = Utils::LuaCount();
        if (lua_count)
        {
            auto& chunk = *reinterpret_cast<skeetvec<LuaInfo>*>(&UI::GetTab(CONFIG)->Chunk);
            if (!luas.capacity())
                luas.reserve(lua_count);
            luas.clear();
            for (auto& lua : chunk)
            {
                if (lua.OnStartup)
                {
                    int len = lua.Name.size() * 2;
                    if (!len) continue;

                    std::unique_ptr<char[]> name = std::make_unique<char[]>(len);

                    size_t count = WideCharToMultiByte(CP_UTF8, 0, lua.Name.data(), -1, name.get(), len, 0, 0);

                    if (!count)
                    {
                        continue;
                    };

                    name.get()[count] = '\0';
                    luas.push_back(std::move(name));
                };
            };
        }
    }

    settings.clear();
    settings["menu_color"] = menu_color;
    settings["menu_key"] = menu_key;
    settings["menu_dpi"] = menu_dpi;
    settings["lock_layout"] = lock_layout;
    settings["menu_size"] = menu_size;
    settings["menu_pos"] = menu_pos;
    settings["menu_tab"] = menu_tab;
    settings["cfg"] = cfg;
    settings["luas"] = luas;

    LastSavedTick = current_tick;

    InterlockedExchange(&atomiclock, 0);
};

void saver::flush_settings()
{
    std::ofstream fs(file_path, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
    if (!fs.is_open()) return;
    if (!settings.is_null())
    {
        fs << settings.dump(4);
        fs.flush();
    }
    fs.close();
};

void __fastcall OnStartupCheckbox(void* ecx, void* ebx)
{
    // maybe I should do resume thread instead
    saver::save_settings();
};

#ifdef HOOK_SUBCLASS
LRESULT WINAPI Subclassproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
    if (*(UINT_PTR*)(0x434798D4) == uIdSubclass && uMsg == 0x1337 && lParam == 0x7ADAEEF4)
    {
        LoadInfo* linfo = reinterpret_cast<LoadInfo*>(wParam);
        if (linfo)
        {
            if (linfo->has_data)
            {
                for (auto& name : linfo->luas)
                {
                    SkeetSDK::Utils::LoadScript(name.get(), true);
                };

                char* cfgname = linfo->cfgname;
                if (cfgname)
                {
                    SkeetSDK::Utils::LoadCfg(cfgname);
                    delete cfgname;
                };
            };
            delete linfo;
        };
        delete subhook;
        return 1;
    };
    return reinterpret_cast<decltype(Subclassproc)*>(subhook->Naked())(hWnd, uMsg, wParam, lParam, uIdSubclass, dwRefData);
};
#else
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == 0x1337 && lParam == 0x7ADAEEF4)
    {
        LoadInfo* linfo = reinterpret_cast<LoadInfo*>(wParam);
        if (linfo)
        {
            if (linfo->has_data)
            {
                for (auto& name : linfo->luas)
                {
                    SkeetSDK::Utils::LoadScript(name.get(), true);
                };

                char* cfgname = linfo->cfgname;
                if (cfgname)
                {
                    SkeetSDK::Utils::LoadCfg(cfgname);
                    delete cfgname;
                };
            };
            delete linfo;
        };
        return 1;
    };
    return CallWindowProc(g_OriginalWindowProc, hWnd, uMsg, wParam, lParam);
};
#endif // HOOK_SUBCLASS