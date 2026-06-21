#pragma once
#include <windows.h>
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>

using json = nlohmann::json;
json config(std::string filename) {
	//std::string filename = "config.json"; // название конфига
	json config_in;
	json config_out;

	if (std::filesystem::exists(filename)) { // Проверка что конфиг существует
		// ЧТЕНИЕ КОНФИГА ИЗ ФАЙЛА (если есть)
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

		// ЗАПИСЬ КОНФИГА В ФАЙЛ
		config_out = config_in; // Сохраняет имеющиеся блоки в конфиге, в случае перезаписи
		bool recreate_conf = false;

		if (!config_in["swap desktops"].contains("enabled")) {
			config_out["swap desktops"]["enabled"] = 1;
			recreate_conf = true;
		}
		if (!config_in["swap desktops"].contains("modifiers") or 1 > config_in["swap desktops"]["modifiers"] or config_in["swap desktops"]["modifiers"] > 15) {
			config_out["swap desktops"]["modifiers"] = 1;
			recreate_conf = true;
		}

		if (!config_in["swap window to desktop"].contains("enabled")) {
			config_out["swap window to desktop"]["enabled"] = 1;
			recreate_conf = true;
		}
		if (!config_in["swap window to desktop"].contains("modifiers") or 1 > config_in["swap window to desktop"]["modifiers"] or config_in["swap window to desktop"]["modifiers"] > 15) {
			config_out["swap window to desktop"]["modifiers"] = 5;
			recreate_conf = true;
		}

		if (!config_in["close window"].contains("enabled")) {
			config_out["close window"]["enabled"] = 1;
			recreate_conf = true;
		}
		if (!config_in["close window"].contains("modifiers") or 1 > config_in["close window"]["modifiers"] or config_in["close window"]["modifiers"] > 15) {
			config_out["close window"]["modifiers"] = 5;
			recreate_conf = true;
		}
		if (!config_in["close window"].contains("button")) {
			config_out["close window"]["button"] = 81; // 81 == Q
			recreate_conf = true;
		}

		if (!config_in["terminate window"].contains("enabled")) {
			config_out["terminate window"]["enabled"] = 0;
			recreate_conf = true;
		}
		if (!config_in["terminate window"].contains("modifiers") or 1 > config_in["terminate window"]["modifiers"] or config_in["terminate window"]["modifiers"] > 15) {
			config_out["terminate window"]["modifiers"] = 7;
			recreate_conf = true;
		}
		if (!config_in["terminate window"].contains("button")) {
			config_out["terminate window"]["button"] = 81; // 81 == Q
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
		// ЗАПИСЬ КОНФИГА В ФАЙЛ (если его нет)
		config_out["swap desktops"]["enabled"] = 1;
		config_out["swap desktops"]["modifiers"] = 1;

		config_out["swap window to desktop"]["enabled"] = 1;
		config_out["swap window to desktop"]["modifiers"] = 5;

		config_out["close window"]["enabled"] = 1;
		config_out["close window"]["modifiers"] = 5;
		config_out["close window"]["button"] = 81; // 81 == Q

		config_out["terminate window"]["enabled"] = 0;
		config_out["terminate window"]["modifiers"] = 7;
		config_out["terminate window"]["button"] = 81; // 81 == Q


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
	return config_out;
}