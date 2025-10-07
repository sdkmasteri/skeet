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

WNDPROC g_OriginalWindowProc = nullptr;

uint32_t bussy_flag = 0;
LRESULT WINAPI WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (uMsg == 0x1337)
    {
        SkeetSDK::Utils::LoadScript(reinterpret_cast<wchar_t*>(wParam), true);
        InterlockedExchange(&bussy_flag, 0);
        return 1;
    };
    return CallWindowProc(g_OriginalWindowProc, hWnd, uMsg, wParam, lParam);
};

void __fastcall OnStartupCheckbox(void* ecx, void* ebx);

typedef int(__thiscall* IntThisFn)(void*);

nlohmann::json saver::settings;

std::vector<std::unique_ptr<char[]>> saver::luas;

static std::filesystem::path file_path = std::filesystem::current_path() / "csgo" / "cfg" / "crosshair_openmode.cfg";

void saver::load_settings()
{
    using namespace SkeetSDK;

    InitAndWaitForSkeet();

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
                Menu->CurrentTab = tab.get<uint32_t>(); //UI::SetTab(tab.get<uint32_t>());
        };

        if (settings.contains("luas"))
        {
            nlohmann::json& luas = settings["luas"];
            if (luas.is_array())
            {

                Utils::InitConfig();
                Utils::AllowUnsafe(true);

                g_OriginalWindowProc = (WNDPROC)SetWindowLongPtr(*(HWND*)(0x433A068A), GWLP_WNDPROC, (LONG)WndProc);

                wchar_t* namebuffer = new wchar_t[_MAX_FNAME];

                for (nlohmann::json& lua : luas)
                {
                    if (!lua.is_string()) continue;

                    // wait until past lua loads
                    while (InterlockedCompareExchange(&bussy_flag, 1, 0)) Sleep(0);

                    std::string& strname = lua.get_ref<std::string&>();
                    namebuffer[mbstowcs(namebuffer, strname.c_str(), strname.length())] = L'\0';

                    SendMessage(*(HWND*)(0x433A068A), 0x1337, reinterpret_cast<WPARAM>(namebuffer), 0);
                };
                // wait last lua to be loaded
                while (InterlockedCompareExchange(&bussy_flag, 1, 0)) Sleep(0);

                delete[] namebuffer;

                // it will break skeet input sometimes
                //SetWindowLongPtr(*(HWND*)(0x433A068A), GWLP_WNDPROC, (LONG)g_OriginalWindowProc);
            };
        };

        if (settings.contains("cfg"))
        {
            nlohmann::json& cfg = settings["cfg"];
            if (cfg.is_string())
                Utils::LoadCfg(cfg.get<std::string>().c_str());
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
    
    //purposed by viera
    UI::SetCallback(config_t->Childs[1]->Elements[5], OnStartupCheckbox);
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