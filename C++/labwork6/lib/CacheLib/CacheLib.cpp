#include "CacheLib.h"


bool CacheHandler::IsValid() {
    json cache_json;
    
    std::ifstream cache_file(cache_file_name);

    if (cache_file.is_open()) {
        cache_file >> cache_json;

        if (!cache_json.contains("date")) {
            cache_file.close();
            return false;
        }
        
        if (!cache_json["date"].is_number_unsigned()) {
            cache_file.close();
            return false;
        }

        time_t time_now = time(0);
        time_t cache_init_date = cache_json["date"];

        if (time_now - cache_init_date > cahce_valid_time) {
            cache_file.close();
            return false;
        }

        cache_file.close();
        return true;
    } else {
        return false;
    }
}

void CacheHandler::CreateCache() {
    std::ofstream cache_file(cache_file_name);

    json cache_json{{"date", time(0)}};

    if (cache_file.is_open()) {
        cache_file << cache_json;
        cache_file.close();
    }
}

void CacheHandler::UpdateCache(json new_cache_json) {
    json old_cache_json;

    std::ifstream cache_file_input(cache_file_name);

    if (cache_file_input.is_open()) {
        cache_file_input >> old_cache_json;
        cache_file_input.close();
    }

    new_cache_json["date"] = old_cache_json["date"];

    std::ofstream cache_file_output(cache_file_name);

    if (cache_file_output.is_open()) {
        cache_file_output << new_cache_json;
    }
}

json CacheHandler::GetCache() {
    json cache_json;

    std::ifstream cache_file(cache_file_name);

    if (cache_file.is_open()) {
        cache_file >> cache_json;
        cache_file.close();
    }
    
    return cache_json;
}
