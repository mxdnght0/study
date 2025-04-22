#include <string>
#include <iostream>
#include "lib/sea_battle_bot.h"
#include "lib/commands_handler.h"

int main() {
    Game* game = new Game;
    CommandsHandler handler{game};
    std::string cmd;
    std::string answer;

    while (!game->GameIsFinished() || 1) {
        std::getline(std::cin, cmd);
        answer = handler.HandleCommand(cmd);

        if (answer == "exit") {
            std::cout << "ok" << std::endl;
            break;
        } else {
            std::cout << answer << std::endl;
        }
    }
}