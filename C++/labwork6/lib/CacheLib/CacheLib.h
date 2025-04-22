#pragma once

#include "nlohmann/json.hpp"
#include <ctime>
#include <string>
#include <fstream>

using json = nlohmann::json;

class CacheHandler {
private:
    const time_t cahce_valid_time = 86400;
    const std::string cache_file_name = "../cache.json";

public:
    CacheHandler() = default;
    CacheHandler(const CacheHandler& other) = delete;
    ~CacheHandler() = default;

    bool IsValid();
    void CreateCache();
    json GetCache();
    void UpdateCache(json new_cache_json);
};