#include "commands_handler.h"

CommandsHandler::CommandsHandler(Game* current_game) : current_game_{current_game} {}

CommandsHandler::~CommandsHandler() {
    delete current_player_;
    delete current_game_;
}

std::string CommandsHandler::HandleCommand(std::string cmd) {
    std::string answer = "failed";

    if (cmd == "ping") {
        answer = "pong";
    }

    if (cmd == "exit") {
        answer = "exit";
    }

    if (cmd == "create master" && current_player_ == nullptr && !current_game_->GameIsStarted()) {
        current_player_ = new Master;
        DefaultStrategy* strat = new DefaultStrategy;
        strat->SetGame(current_game_);
        current_player_->SetGame(current_game_);
        current_player_->SetGameSettings();
        current_player_->SetShipsCords(new ShipsCords);
        current_player_->SetStrategy(strat);
        answer = "ok";
    }

    if (cmd == "create slave" && current_player_ == nullptr && !current_game_->GameIsStarted()) {
        current_player_ = new Slave;
        DefaultStrategy* strat = new DefaultStrategy;
        strat->SetGame(current_game_);
        current_player_->SetGame(current_game_);
        current_player_->SetShipsCords(new ShipsCords);
        current_player_->SetStrategy(strat);
        answer = "ok";
    }

    if (cmd == "start" && current_player_ != nullptr) {
        current_game_->StartGame();
        current_player_->StartGame();
        answer = "ok";
    }

    if (cmd == "stop" && current_game_->GameIsStarted()) {
        current_game_->StopGame();
        delete current_player_;
        current_player_ = nullptr;
        answer = "ok";
    }

    if (cmd.find("set width") == 0 && current_player_ != nullptr && !current_game_->GameIsStarted() && current_player_->WhoIs() == "master") {
        std::istringstream iss(cmd);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word) {
            words.push_back(word);
        }

        try {
            size_t pos;
            uint64_t result = std::stoull(words[2], &pos, 10);
            current_game_->SetWidth(result);
            answer = "ok";
        } catch (const std::out_of_range& e) {
            answer = "failed";
        }
    }

    if (cmd.find("set height") == 0 && current_player_ != nullptr && !current_game_->GameIsStarted() && current_player_->WhoIs() == "master") {
        std::istringstream iss(cmd);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word) {
            words.push_back(word);
        }

        try {
            size_t pos;
            uint64_t result = std::stoull(words[2], &pos, 10);
            current_game_->SetHeight(result);
            answer = "ok";
        } catch (const std::out_of_range& e) {
            answer = "failed";
        }
    }

    if (cmd == "get width") {
        answer = std::to_string(current_game_->GetWidth());
    }

    if (cmd == "get height") {
        answer = std::to_string(current_game_->GetHeight());
    }

    if (cmd.find("set count") == 0 && current_player_ != nullptr && !current_game_->GameIsStarted() && current_player_->WhoIs() == "master") {
        std::istringstream iss(cmd);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word) {
            words.push_back(word);
        }

        try {
            size_t pos;
            uint8_t ship = static_cast<uint8_t>(std::stoull(words[2], &pos, 10));
            uint64_t result = std::stoull(words[3], &pos, 10);
            current_game_->SetShipsCounter(ship, result);
            answer = "ok";
        } catch (const std::out_of_range& e) {
            answer = "failed";
        }
    }

    if (cmd.find("get count") == 0) {
        std::istringstream iss(cmd);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word) {
            words.push_back(word);
        }

        size_t pos;
        uint8_t ship = static_cast<uint8_t>(std::stoull(words[2], &pos, 10));
        answer = std::to_string(current_game_->GetShipsCounter(ship));
    }

    if (cmd.find("set strategy") == 0 && current_player_ != nullptr && !current_game_->GameIsStarted()) {
        std::istringstream iss(cmd);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word) {
            words.push_back(word);
        }

        if (words[2] == "ordered") {
            OrderedStrategy* strat = new OrderedStrategy;
            strat->SetGame(current_game_);
            delete current_player_->GetStrategy();
            current_player_->SetStrategy(strat);
        }

        answer = "ok";
    }

    if (cmd.find("shot") == 0 && cmd.length() != 4 && current_game_->GameIsStarted() && !current_game_->GameIsFinished()) {
        std::istringstream iss(cmd);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word) {
            words.push_back(word);
        }
        
        if (words.size() == 3) {
            size_t pos;
            uint8_t x = static_cast<uint8_t>(std::stoul(words[1], &pos, 10));
            uint8_t y = static_cast<uint8_t>(std::stoul(words[2], &pos, 10));

            uint8_t result = current_player_->Shot(x, y);

            if (result == 0) {
                answer = "miss";
            } else if (result == 1) {
                answer = "hit";
            } else {
                answer = "kill";
            }
        }
        
    }

    if (cmd == "shot" && current_game_->GameIsStarted() && !current_game_->GameIsFinished()) {
        std::pair<uint64_t, uint64_t> cords = current_player_->GetShot();

        answer = std::to_string(cords.first) + " " + std::to_string(cords.second);
    }

    if (cmd.find("set result") == 0 && last_command == "shot" && current_game_->GameIsStarted() && !current_game_->GameIsFinished()) {
        std::istringstream iss(cmd);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word) {
            words.push_back(word);
        }

        if (words[2] == "miss") {
            current_player_->SetResult(0);
            answer = "ok";
        } else if (words[2] == "hit") {
            current_player_->SetResult(1);
            answer = "ok";
        } else {
            current_player_->SetResult(2);
            answer = "ok";
        }
    }

    if (cmd == "finished") {
        if (current_game_->GameIsFinished()) {
            answer = "yes";
        } else {
            answer = "no";
        }
    }

    if (cmd == "win") {
        answer = "yes";

        if (!current_game_->GameIsFinished()) {
            answer = "no";
        } else {
            for (uint8_t i = 1; i < 5; i++) {
                if (current_player_->GetStrategy()->GetShipsKilled(i) != current_game_->GetShipsCounter(i)) {
                    answer = "no";
                }   
            }
        }
    }

    if (cmd == "lose") {
        answer = "no";

        if (!current_game_->GameIsFinished()) {
            answer = "no";
        } else {
            for (uint8_t i = 1; i < 5; i++) {
                if (current_player_->GetStrategy()->GetShipsKilled(i) != current_game_->GetShipsCounter(i)) {
                    answer = "yes";
                }   
            }
        }
    }

    if (cmd.find("dump") == 0 && current_player_ != nullptr) {
        std::istringstream iss(cmd);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word) {
            words.push_back(word);
        }
        if (words.size() == 2) {
            std::ofstream dump(words[1]);

            if (dump) {
                dump << current_game_->GetWidth() << " " << current_game_->GetHeight() << "\n";
                
                std::vector<Ship> ships = current_player_->GetShipCords();

                for (auto ship : ships) {
                    dump << static_cast<uint64_t>(ship.len) << " " << (ship.orientation == 1 ? 'v' : 'h') << " " << ship.x << " " << ship.y << "\n";
                }

                dump.close();
            }

            answer = "ok";
        }
    }

    if (cmd.find("load") == 0 && current_player_ != nullptr) {
        std::istringstream iss(cmd);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word) {
            words.push_back(word);
        }

        if (words.size() == 2) {
            std::ifstream load(words[1]);
            uint32_t len;
            std::string orient_str;
            uint64_t x;
            uint64_t y;

            if (load) {
                current_player_->CordsWasLoaded();
                delete current_player_->GetShipsCordsPtr();
                uint64_t len1_counter = 0;
                uint64_t len2_counter = 0;
                uint64_t len3_counter = 0;
                uint64_t len4_counter = 0;
                
                load >> x >> y;
                current_game_->SetWidth(x);
                current_game_->SetHeight(y);

                ShipsCords* cords = new ShipsCords;
                while (load >> len >> orient_str >> x >> y) {
                    switch (len) {
                    case 1:
                        ++len1_counter;
                        break;
                    
                    case 2:
                        ++len2_counter;
                        break;

                    case 3:
                        ++len3_counter;
                        break;

                    case 4:
                        ++len4_counter;
                        break;    
                    
                    default:
                        break;
                    }

                    uint8_t orient = (orient_str == "h") ? 0 : 1;
                    cords->AddShip(Ship(x, y, len, orient));
                }

                current_player_->SetShipsCords(cords);
                current_game_->SetShipsCounter(1, len1_counter);
                current_game_->SetShipsCounter(2, len2_counter);
                current_game_->SetShipsCounter(3, len3_counter);
                current_game_->SetShipsCounter(4, len4_counter);
                answer = "ok";
            }
        }
    }

    if (answer != "failed") {
        last_command = cmd;
    }

    return answer;
}