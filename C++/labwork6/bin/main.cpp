#include "nlohmann/json.hpp"
#include "ArgParser/ArgParser.h"
#include "YandexApi/YandexApi.h"
#include <iostream>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

struct Options {
    std::string from;
    std::string to;
    std::string date;
    bool enter_api;
    bool delete_cache;
};

std::string API_KEY;

int main(int argc, char** argv) {
    Options opt;

    ArgumentParser::ArgParser parser("Parser");
    parser.AddHelp('h', "help", "FindTrackApp allows you find ways from one city to another.");
    parser.AddStringArgument('f', "from", "departure city").StoreValue(opt.from);
    parser.AddStringArgument('t', "to", "destination city").StoreValue(opt.to);
    parser.AddStringArgument('d', "date", "departure date").StoreValue(opt.date);
    parser.AddFlag('a', "api", "specify if you want to enter the api").StoreValue(opt.enter_api).Default(false);
    parser.AddFlag('c', "delete-cache", "this option deletes cahce, may help if your app is not working").StoreValue(opt.delete_cache).Default(false);
    
    if (!parser.Parse(argc, argv)) {
        std::cerr << parser.HelpDescription() << std::endl;

        return 1;
    }

    if (std::filesystem::exists("../api_key_settings.json")) {
        std::fstream api_key_file("../api_key_settings.json");
        json api_key_json;
        api_key_file >> api_key_json;

        if (api_key_file.is_open()) {
            if (opt.enter_api) {
                std::cout << "please, enter api key" << std::endl;;
                
                std::cin >> API_KEY;
    
                api_key_json["api_key"] = API_KEY;
                
                api_key_file.seekp(0);
                api_key_file << "";

                api_key_file << api_key_json;
            } else {
                API_KEY = api_key_json["api_key"];
            }
        } else {
            std::cout << "coldn't open api_key_settings.json" << std::endl;

            return 1;
        }
        
    } else {
        std::string answer;

        while (1) {
            std::cout << "there ins't api key. would you like to enter it? [y/n]" << std::endl;
            
            std::cin >> answer;

            if (answer == "y") {
                std::cout << "please, enter api key" << std::endl;

                std::cin >> API_KEY;

                json api_key_json{{"api_key", API_KEY}};

                std::ofstream api_key_file("../api_key_settings.json");

                if (api_key_file.is_open()) {
                    api_key_file << api_key_json;
                    api_key_file.close();
                } else {
                    std::cout << "coldn't open api_key_settings.json" << std::endl;

                    return 1;
                }
                
                break;
            } else if (answer == "n") {
                std::cout << "goodbye!" << std::endl;

                return 0;
            }
        }
    }

    YandexApi Api(API_KEY, opt.from, opt.to, opt.date, opt.delete_cache);

    if (Api.GetTracks()) {
        Api.Print(std::cout);
    } else {
        std::cout << "something went wrong, maybe your api key isn't correct" << std::endl;
    }

    return 0;
}
