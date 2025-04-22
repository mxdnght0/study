#include <TimeConverter.h>

std::string FormatTime(const std::string& time_str) {
    std::tm tm = {};
    std::istringstream ss(time_str);
    ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%S");

    std::ostringstream formatted_time;
    formatted_time << std::put_time(&tm, "%d-%m-%Y %H:%M");
    return formatted_time.str();
}