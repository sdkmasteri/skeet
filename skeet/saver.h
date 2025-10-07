#ifndef SKEET_SAVER_H
#define SKEET_SAVER_H
#include "Json/json.hpp"
#include <Windows.h>
#include <fstream>
#include <memory>

class saver final
{
	static nlohmann::json settings;
	static DWORD LastSavedTick;
	static std::vector<std::unique_ptr<char[]>> luas;
public:
	static void load_settings();
	static void save_settings();
	static void flush_settings();
};

#endif // SKEET_SAVER_H