#pragma once
#include <windows.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>

const std::unordered_map<std::string, UINT> modifierMap = {
	{"ALT", MOD_ALT},
	{"CTRL", MOD_CONTROL},
	{"SHIFT", MOD_SHIFT},
	{"WIN", MOD_WIN}
};

const std::unordered_map<std::string, UINT> keyMap = {
		{"A", 65},
		{"B", 66},
		{"C", 67},
		{"D", 68},
		{"E", 69},
		{"F", 70},
		{"G", 71},
		{"H", 72},
		{"I", 73},
		{"J", 74},
		{"K", 75},
		{"L", 76},
		{"M", 77},
		{"N", 78},
		{"O", 79},
		{"P", 80},
		{"Q", 81},
		{"R", 82},
		{"S", 83},
		{"T", 84},
		{"U", 85},
		{"V", 86},
		{"W", 87},
		{"X", 88},
		{"Y", 89},
		{"Z", 90}
};

std::vector<std::string> SerializeModifiers(UINT mods)
{
	std::vector<std::string> result;

	if (mods & MOD_ALT)
		result.push_back("ALT");

	if (mods & MOD_CONTROL)
		result.push_back("CTRL");

	if (mods & MOD_SHIFT)
		result.push_back("SHIFT");

	if (mods & MOD_WIN)
		result.push_back("WIN");

	return result;
}

std::string SerializeKey(UINT vk)
{
	if (vk >= 'A' && vk <= 'Z')
		return std::string(1, static_cast<char>(vk));

	switch (vk)
	{
		// case VK_SPACE:  return "SPACE"; подсказка для будущих обновлений
	}

	return std::to_string(vk); // запасной вариант
}

UINT ParseModifiers(const nlohmann::json& mods)
{
	UINT result = 0;

	for (const auto& mod : mods)
	{
		auto it = modifierMap.find(mod.get<std::string>());
		if (it != modifierMap.end())
			result |= it->second;
	}

	return result;
}

UINT ParseKey(const std::string& name)
{
	auto it = keyMap.find(name);

	if (it == keyMap.end())
		throw;
	//throw MessageBoxW(NULL, L"Error", L"Unknown key", MB_OK);

	return it->second;
}

using json = nlohmann::json;
json config(std::string filename) {
	//std::string filename = "config.json"; // название конфига
	json config_in;
	json config_out;

	if (std::filesystem::exists(filename)) { // Проверка что конфиг существует
		/*===================================
		ЧТЕНИЕ КОНФИГА ИЗ ФАЙЛА (если есть)
		===================================*/
		std::ifstream file_in(filename);
		if (!file_in.is_open()) {
			MessageBoxW(NULL, L"Error", L"Config file read error!", MB_OK);
			return 1;
		}
		else {
			//json config_in;
			file_in >> config_in; // Парсинг файла напрямую в объект json
			file_in.close();
		}

		/*=====================
		ЗАПИСЬ КОНФИГА В ФАЙЛ
		=====================*/
		config_out = config_in; // Сохраняет имеющиеся блоки в конфиге, в случае перезаписи
		bool recreate_conf = false;

		if (!config_in["swap desktops"].contains("enabled")) {
			config_out["swap desktops"]["enabled"] = 1;
			recreate_conf = true;
		}
		if (!config_in["swap desktops"].contains("modifiers")) {
			config_out["swap desktops"]["modifiers"] = SerializeModifiers(1);
			recreate_conf = true;
		}

		if (!config_in["swap window to desktop"].contains("enabled")) {
			config_out["swap window to desktop"]["enabled"] = 1;
			recreate_conf = true;
		}
		if (!config_in["swap window to desktop"].contains("modifiers")) {
			config_out["swap window to desktop"]["modifiers"] = SerializeModifiers(5);
			recreate_conf = true;
		}

		if (!config_in["close window"].contains("enabled")) {
			config_out["close window"]["enabled"] = 1;
			recreate_conf = true;
		}
		if (!config_in["close window"].contains("modifiers")) {
			config_out["close window"]["modifiers"] = SerializeModifiers(5);
			recreate_conf = true;
		}
		if (!config_in["close window"].contains("key")) {
			config_out["close window"]["key"] = SerializeKey(81); // 81 == Q
			recreate_conf = true;
		}

		if (!config_in["terminate window"].contains("enabled")) {
			config_out["terminate window"]["enabled"] = 0;
			recreate_conf = true;
		}
		if (!config_in["terminate window"].contains("modifiers")) {
			config_out["terminate window"]["modifiers"] = SerializeModifiers(7);
			recreate_conf = true;
		}
		if (!config_in["terminate window"].contains("key")) {
			config_out["terminate window"]["key"] = SerializeKey(81); // 81 == Q
			recreate_conf = true;
		}

		if (recreate_conf) {
			std::ofstream file_out(filename);
			if (file_out.is_open()) {
				file_out << config_out.dump(4);
				file_out.close();
				MessageBoxW(NULL, L"Notify", L"Config file recreated", MB_OK);
			}
			else {
				MessageBoxW(NULL, L"Error", L"Config create error!", MB_OK);
				return 1;
			}
		}
	}
	else {
		/*====================================
		ЗАПИСЬ КОНФИГА В ФАЙЛ (если его нет)
		====================================*/
		config_out["swap desktops"]["enabled"] = 1;
		config_out["swap desktops"]["modifiers"] = SerializeModifiers(1);

		config_out["swap window to desktop"]["enabled"] = 1;
		config_out["swap window to desktop"]["modifiers"] = SerializeModifiers(5);

		config_out["close window"]["enabled"] = 1;
		config_out["close window"]["modifiers"] = SerializeModifiers(5);
		config_out["close window"]["key"] = SerializeKey('Q'); // 81 == Q

		config_out["terminate window"]["enabled"] = 0;
		config_out["terminate window"]["modifiers"] = SerializeModifiers(7);
		config_out["terminate window"]["key"] = SerializeKey('Q'); // 81 == Q


		std::ofstream file_out(filename);
		if (file_out.is_open()) {
			file_out << config_out.dump(4);
			file_out.close();
			MessageBoxW(NULL, L"Notify", L"Config file created", MB_OK);
		}
		else {
			MessageBoxW(NULL, L"Error", L"Config create error!", MB_OK);
			return 1;
		}
	}
	/*============================
		 Парсинг клавиш из конфига
		============================*/
		// Swap Vdesktops
	if (config_out["swap desktops"]["modifiers"].is_array()) {
		config_out["swap desktops"]["modifiers"] = ParseModifiers(config_out["swap desktops"]["modifiers"]);
	}

	// Swap window to Vdesktops
	if (config_out["swap window to desktop"]["modifiers"].is_array()) {
		config_out["swap window to desktop"]["modifiers"] = ParseModifiers(config_out["swap window to desktop"]["modifiers"]);
	}

	// Close window
	if (config_out["close window"]["modifiers"].is_array()) {
		config_out["close window"]["modifiers"] = ParseModifiers(config_out["close window"]["modifiers"]);
	}
	if (config_out["close window"]["key"].is_number_integer()) {
	}
	else if (config_out["close window"]["key"].is_string()) {
		config_out["close window"]["key"] = ParseKey(config_out["close window"]["key"]);
	}

	// Terminate window
	if (config_out["terminate window"]["modifiers"].is_array()) {
		config_out["terminate window"]["modifiers"] = ParseModifiers(config_out["terminate window"]["modifiers"]);
	}
	if (config_out["terminate window"]["key"].is_number_integer()) {
	}
	else if (config_out["terminate window"]["key"].is_string()) {
		config_out["terminate window"]["key"] = ParseKey(config_out["terminate window"]["key"]);
	}
	return config_out;
}