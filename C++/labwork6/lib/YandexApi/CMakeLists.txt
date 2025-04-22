add_library(YandexApi STATIC YandexApi.cpp)

target_include_directories(YandexApi PRIVATE
    ${cpr_SOURCE_DIR}/include
    ${json_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/lib
)

target_link_libraries(YandexApi PUBLIC 
    cpr::cpr 
    nlohmann_json::nlohmann_json 
    Wstr
    TimeConverter
    CacheLib
)