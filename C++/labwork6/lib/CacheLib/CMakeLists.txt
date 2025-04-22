add_library(CacheLib CacheLib.cpp)
target_link_libraries(CacheLib PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(CacheLib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${json_SOURCE_DIR}/include)