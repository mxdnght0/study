#pragma once

#include "sea_battle_bot.h"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>


class CommandsHandler {
private:
    Game* current_game_ = nullptr;
    Player* current_player_ = nullptr;
    std::string last_command = "";
public:
    CommandsHandler(Game* current_game);
    ~CommandsHandler();
    std::string HandleCommand(std::string cmd);
};