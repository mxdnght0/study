cmake_minimum_required(VERSION 3.14)
project(FindTrackApp)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
    cpr
    GIT_REPOSITORY https://github.com/libcpr/cpr.git
    GIT_TAG master
)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz 
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

add_subdirectory(lib)
add_subdirectory(bin)