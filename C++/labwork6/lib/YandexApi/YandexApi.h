#pragma once

#include "cpr/cpr.h"
#include "nlohmann/json.hpp"
#include "tabulate/table.hpp"
#include "Wstr/Wstr.h"
#include "TimeConverter/TimeConverter.h"
#include "CacheLib/CacheLib.h"
#include <string>
#include <fstream>
#include <iostream>


using json = nlohmann::json;

class YandexApi {
public:
    YandexApi(const std::string& api_key, const std::string& from, const std::string& to, const std::string& date, const bool& delete_cache);
    YandexApi(const YandexApi& other) = delete;
    ~YandexApi() = default;
    
    bool GetTracks();
    void Print(std::ostream& os) const;

private:
    std::string api_key_;    
    std::string from_code;
    std::string to_code;
    std::string date_;
    bool from_code_is_initialized{ false };
    bool to_code_is_initialized{ false };
    json tracks_json;
    CacheHandler cache;
};