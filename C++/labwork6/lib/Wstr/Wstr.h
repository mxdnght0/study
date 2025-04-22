#pragma once

#include <string>
#include <unordered_map>
#include <codecvt>
#include <locale>

extern std::unordered_map<wchar_t, std::string> translit_map;

std::wstring StringToWstring(const std::string& str);

std::string Transliterate(const std::string& input);