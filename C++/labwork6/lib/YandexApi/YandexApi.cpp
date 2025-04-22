#include "YandexApi.h"

YandexApi::YandexApi(const std::string& api_key, const std::string& from, const std::string& to, const std::string& date, const bool& delete_cache) : api_key_{ api_key }, date_{ date } {
    if (delete_cache) {
        cache.CreateCache();
    }

    if (std::filesystem::exists("../yandex_codes.json")) {

        std::fstream city_codes_file("../yandex_codes.json");

        if (city_codes_file.is_open()) {
            json city_codes_json;

            city_codes_file >> city_codes_json;

            for (const auto& [city, code] : city_codes_json.items()) {
                if (city == from) {
                    from_code = code;
                    from_code_is_initialized = true;
                }
            
                if (city == to) {
                    to_code = code;
                    to_code_is_initialized = true;
                }
            }

            city_codes_file.close();
        } else {
            std::cerr << "couldn't open yandex_codes.json" << std::endl;
        }
    } else {
        json yandex_codes_json;

        std::string url = "https://api.rasp.yandex.net/v3.0/stations_list/?apikey=" + api_key + "&lang=ru_RU&format=json";
    
        cpr::Response cities = cpr::Get(cpr::Url{url});
    
        if (cities.status_code == 200) {
            json cities_json = json::parse(cities.text);
    
            for (const auto& country : cities_json["countries"]) {
                for (const auto& region : country["regions"]) {
                    for (const auto& settlement : region["settlements"]) {
                        std::string city = settlement["title"];
    
                        if (!city.empty()) {
                            std::string yandex_code = settlement["codes"]["yandex_code"];
    
                            if (yandex_code.rfind("c", 0) == 0) {
                                yandex_codes_json[city] = yandex_code;
                            }
                        }
                    }
                }
            }

            std::ofstream yandex_codes_file("../yandex_codes.json");

            if (yandex_codes_file.is_open()) {
                yandex_codes_file << yandex_codes_json;

                yandex_codes_file.close();
            }
        } else {
            std::cerr << "couldn't get the data, error code: " << cities.status_code << std::endl;
        }
    }
}

bool YandexApi::GetTracks() {
    std::string url = "https://api.rasp.yandex.net/v3.0/search/?apikey=" + api_key_ + "&format=json&transfers=true&from=" + from_code + "&to=" + to_code + "&lang=ru_RU&limit=100&date=" + date_;
    
    if (!to_code_is_initialized || !from_code_is_initialized) {
        std::cerr << "from/to code wasn't initialized" << std::endl;

        return false;
    }

    if (!cache.IsValid()) {
        cache.CreateCache();
    } 

    json cache_json = cache.GetCache();

    if (cache_json.contains(from_code + to_code + date_)) {
        tracks_json = cache_json[from_code + to_code + date_];

        return true;
    } else {
        cpr::Response tracks = cpr::Get(cpr::Url{url});
    
        if (tracks.status_code == 200) {
            tracks_json = json::parse(tracks.text);
            
            cache_json[from_code + to_code + date_] = tracks_json;
            cache.UpdateCache(cache_json);
            
            return true;
        } else {
            return false;
        }
    }
}

void YandexApi::Print(std::ostream& os) const {
    tabulate::Table table;

    table.add_row({"Type", "Departure", "Arrival", "Transport", "Number", "From", "To", "Duration"});

    if (tracks_json.contains("segments") && tracks_json["segments"].is_array()) {
        for (const auto& segment : tracks_json["segments"]) {
            if (segment.contains("departure") && segment.contains("arrival") &&
                segment.contains("thread") && segment["thread"].is_object() &&
                segment["thread"].contains("transport_type") &&
                segment["thread"].contains("number") &&
                segment.contains("from") && segment["from"].is_object() &&
                segment["from"].contains("title") &&
                segment.contains("to") && segment["to"].is_object() &&
                segment["to"].contains("title") &&
                segment.contains("duration")) {

                std::string departure = FormatTime(segment["departure"]);
                std::string arrival = FormatTime(segment["arrival"]);
                std::string transport_type = segment["thread"]["transport_type"];
                std::string number = Transliterate(segment["thread"]["number"]);
                std::string from = Transliterate(segment["from"]["title"]);
                std::string to = Transliterate(segment["to"]["title"]);
                double duration = segment["duration"];

                int hours = static_cast<int>(duration) / 3600;
                int minutes = (static_cast<int>(duration) % 3600) / 60;
                std::ostringstream duration_stream;
                duration_stream << std::setw(2) << std::setfill('0') << hours << ":"
                               << std::setw(2) << std::setfill('0') << minutes;
                std::string duration_str = duration_stream.str();

                table.add_row({"Straight", departure, arrival, transport_type, number, from, to, duration_str});
            }
        }
    }

    if (tracks_json.contains("segments") && tracks_json["segments"].is_array()) {
        for (const auto& segment : tracks_json["segments"]) {
            if (segment.contains("has_transfers") && segment["has_transfers"] == true &&
                segment.contains("details") && segment["details"].is_array()) {

                for (const auto& detail : segment["details"]) {
                    if (detail.contains("departure") && detail.contains("arrival") &&
                        detail.contains("thread") && detail["thread"].is_object() &&
                        detail["thread"].contains("transport_type") &&
                        detail["thread"].contains("number") &&
                        detail.contains("from") && detail["from"].is_object() &&
                        detail["from"].contains("title") &&
                        detail.contains("to") && detail["to"].is_object() &&
                        detail["to"].contains("title") &&
                        detail.contains("duration")) {

                        std::string departure = FormatTime(detail["departure"]);
                        std::string arrival = FormatTime(detail["arrival"]);
                        std::string transport_type = detail["thread"]["transport_type"];
                        std::string number = Transliterate(detail["thread"]["number"]);
                        std::string from = Transliterate(detail["from"]["title"]);
                        std::string to = Transliterate(detail["to"]["title"]);
                        double duration = detail["duration"];

                        int hours = static_cast<int>(duration) / 3600;
                        int minutes = (static_cast<int>(duration) % 3600) / 60;
                        std::ostringstream duration_stream;
                        duration_stream << std::setw(2) << std::setfill('0') << hours << ":"
                                       << std::setw(2) << std::setfill('0') << minutes;
                        std::string duration_str = duration_stream.str();

                        table.add_row({"Transfer", departure, arrival, transport_type, number, from, to, duration_str});
                    }
                }
            }
        }
    }

    os << table << std::endl;
}