#include "sea_battle_bot.h"

void Game::SetWidth(const uint64_t width) {
    width_ = width;
}

void Game::SetHeight(const uint64_t height) {
    height_ = height;
}

uint64_t Game::GetWidth() const {
    return width_;
}

uint64_t Game::GetHeight() const {
    return height_;
}

void Game::StartGame() {
    game_is_started_ = true;
}

void Game::StopGame() {
    game_is_started_ = false;
    width_ = 0;
    height_ = 0;
    game_is_started_ = false;
    game_is_finished_ = false;
    len1_ships_counter_ = 0;
    len2_ships_counter_ = 0;
    len3_ships_counter_ = 0;
    len4_ships_counter_ = 0;
}


bool Game::GameIsFinished() const {
    return game_is_finished_;
}

void Game::FinishGame() {
    game_is_finished_ = true;
}

void Game::SetShipsCounter(uint8_t ship_type, uint64_t ships_counter) {
    switch (ship_type) {
    case 1:
        len1_ships_counter_ = ships_counter;
        break;

    case 2:
        len2_ships_counter_ = ships_counter;
        break;

    case 3:
        len3_ships_counter_ = ships_counter;
        break;

    case 4:
        len4_ships_counter_ = ships_counter;
        break;
    }
}

uint64_t Game::GetShipsCounter(uint8_t ship_type) const {
    switch (ship_type) {
    case 1:
        return len1_ships_counter_;

    case 2:
        return len2_ships_counter_;

    case 3:
        return len3_ships_counter_;

    case 4:
        return len4_ships_counter_;

    default:
        return 1;
    }
}

void Strategy::SetGame(Game* current_game) {
    current_game_ = current_game;
}

Ship::Ship(uint64_t x_coord, uint64_t y_coord, uint8_t length, uint8_t orient)
    : x{x_coord}
    , y{y_coord}
    , len(length)
    , orientation{orient}
    , hitted{0}
    {}

Ship::Ship()
    : x{0}
    , y{0}
    , len(0)
    , orientation{0}
    , hitted{0}
    {}

Ship::Ship(const Ship& other)
    : x{other.x}, y{other.y}, len{other.len}, orientation{other.orientation}, hitted{other.hitted} {}

Ship& Ship::operator=(const Ship& other) {
        if (this == &other) return *this;
        x = other.x;
        y = other.y;
        len = other.len;
        orientation = other.orientation;
        hitted = other.hitted;
        return *this;
    }

ShipsCords::~ShipsCords() {
    delete[] data;
}

void ShipsCords::AddShip(Ship ship) {

    if (size == 0) {
        size = 1;
        data = new Ship[size];
        data[0] = ship;
    } else {
        Ship* new_data = new Ship[++size];

        for (size_t i = 0; i < size - 1; i++) {
            new_data[i] = data[i];
        }

        new_data[size - 1] = ship;
        delete[] data;
        data = new_data;
    }
}

uint8_t ShipsCords::CheckShot(uint64_t x, uint64_t y) { 
    uint8_t response = 0;
    
    for (size_t i = 0; i < size; i++) {
        if (data[i].orientation == 0) {
            if (data[i].x <= x && data[i].x + data[i].len - 1 >= x && data[i].y == y) {
                if (++data[i].hitted >= data[i].len) {
                    response = 2;
                } else {
                    response = 1;
                }
            }
        } else {
            if (data[i].y <= y && data[i].y + data[i].len - 1 >= y && data[i].x == x) {
                if (++data[i].hitted >= data[i].len) {
                    response = 2;
                } else {
                    response = 1;
                }
            }
        }
    }

    return response;
    //RESPONSE CODES: 0 - miss, 1 - hit, 2 - kill
}

void Master::SetGame(Game* current_game) {
    current_game_ = current_game;
}

void Master::SetGameSettings() {
    current_game_->SetWidth(3);
    current_game_->SetHeight(15);
    current_game_->SetShipsCounter(1, 1);
    current_game_->SetShipsCounter(2, 0);
    current_game_->SetShipsCounter(3, 2);
    current_game_->SetShipsCounter(4, 3);
}

void Master::SetStrategy(Strategy* current_stretegy) {
    current_strategy_ = current_stretegy;
}

void Master::SetShipsCords(ShipsCords* ships_coords) {
    ships_coords_ = ships_coords;
}

uint8_t Master::Shot(uint64_t x, uint64_t y) {
    uint8_t response = ships_coords_->CheckShot(x, y);

    if (response == 2) {
        ++player_ships_killed;
    }

    if (player_ships_killed == current_game_->GetShipsCounter(1) + 
        current_game_->GetShipsCounter(2) + 
        current_game_->GetShipsCounter(3) + 
        current_game_->GetShipsCounter(4)) {
        current_game_->FinishGame();
    }

    return response;
}

std::pair<uint64_t, uint64_t> Master::GetShot() {
    return current_strategy_->GetShot();
}

void Master::SetResult(uint8_t result) {
    current_strategy_->SetResult(result);
}

void Slave::SetGame(Game* current_game) {
    current_game_ = current_game;
}

void Slave::SetGameSettings() {
    return;
}

void Slave::SetStrategy(Strategy* current_stretegy) {
    current_strategy_ = current_stretegy;
}

void Slave::SetShipsCords(ShipsCords* ships_coords) {
    ships_coords_ = ships_coords;
}

uint8_t Slave::Shot(uint64_t x, uint64_t y) {
    uint8_t response = ships_coords_->CheckShot(x, y);

    if (response == 2) {
        ++player_ships_killed;
    }

    if (player_ships_killed == current_game_->GetShipsCounter(1) + 
        current_game_->GetShipsCounter(2) + 
        current_game_->GetShipsCounter(3) + 
        current_game_->GetShipsCounter(4)) {
        current_game_->FinishGame();
    }

    return response;
}

std::pair<uint64_t, uint64_t> Slave::GetShot() {
    return current_strategy_->GetShot();
}

void Slave::SetResult(uint8_t result) {
    current_strategy_->SetResult(result);
}

bool DefaultStrategy::IsBlocked(uint64_t x, uint64_t y) {
    for (const auto& shot : blocked_cells) {
            if (shot.first == x && shot.second == y) {
                return true;
            }
        }

    return false;
}

bool DefaultStrategy::IsValid(uint64_t x, uint64_t y) const {
    if (x >= 0 && x < current_game_->GetWidth() && y >= 0 && y < current_game_->GetHeight()) {
        return true;
    }

    return false;
}

void DefaultStrategy::BlockAroundShip() {
    for (auto cords : last_killed_ship_cords) {
        for (int8_t dx = -1; dx <= 1; dx++) {
            for (int8_t dy = -1; dy <= 1; dy++) {
                if (IsValid(cords.first + dx, cords.second + dy) && !IsBlocked(cords.first + dx, cords.second + dy)) {
                    blocked_cells.emplace_back(std::pair<uint64_t, uint64_t>{cords.first + dx, cords.second + dy});          
                }
            }
        }
    }
}

std::pair<uint64_t, uint64_t> DefaultStrategy::CalculateShot() {
    uint64_t x = last_shot.first;
    uint64_t y = last_shot.second;
    
    if (!in_elemination_mode) {
        y += step;
        if (y > current_game_->GetHeight() - 1) {
            ++x;

            if (x > current_game_->GetWidth() - 1) {
                x = 0;
                y = 0;
            } else {
                y = x % step;
            }
        }
    } else {
        if (last_killed_ship_cords.size() == 1) {
            uint64_t x_ship = last_killed_ship_cords[0].first;
            uint64_t y_ship = last_killed_ship_cords[0].second;

            if (y == 0 && x == x_ship && y == y_ship) {
                x++;
            } else if (x == x_ship && y == y_ship) {
                y--;
            } else if (x == x_ship && y + 1 == y_ship) {
                x++;
                y++;
            } else if (x - 1 == x_ship && y == y_ship) {
                y++;
                x--;
            } else if (x == x_ship && y - 1 == y_ship) {
                x--;
                y--;
            }
        } else {
            uint64_t x1_ship = last_killed_ship_cords[0].first;
            uint64_t y1_ship = last_killed_ship_cords[0].second;
            uint64_t x2_ship = last_killed_ship_cords[1].first;
            uint64_t y2_ship = last_killed_ship_cords[1].second;
            uint8_t orient;

            if (x1_ship - x2_ship == 0) {
                orient = 1;
            } else {
                orient = 0;
            }

            if (last_killed_ship_cords.size() == 3) {
                if (orient == 0) {
                    uint64_t x3_ship = last_killed_ship_cords[2].first;

                    if (x != std::max(std::max(x1_ship, x2_ship), x3_ship) + 1 && IsValid(std::max(std::max(x1_ship, x2_ship), x3_ship) + 1, y)) {
                        x = std::max(std::max(x1_ship, x2_ship), x3_ship) + 1;
                    } else {
                        x = std::min(std::min(x1_ship, x2_ship), x3_ship) - 1;
                    }
                } else {
                    uint64_t y3_ship = last_killed_ship_cords[2].second;

                    if (y != std::max(std::max(y1_ship, y2_ship), y3_ship) + 1 && IsValid(x, std::max(std::max(y1_ship, y2_ship), y3_ship) + 1)) {
                        y = std::max(std::max(y1_ship, y2_ship), y3_ship) + 1;
                    } else {
                        y = std::min(std::min(y1_ship, y2_ship), y3_ship) - 1;
                    }
                }
            } else {
                if (orient == 0) {
                    if (x != std::max(x1_ship, x2_ship) + 1 && IsValid(std::max(x1_ship, x2_ship) + 1, y)) {
                        x = std::max(x1_ship, x2_ship) + 1;
                    } else {
                        x = std::min(x1_ship, x2_ship) - 1;
                    }
                } else {
                    if (y != std::max(y1_ship, y2_ship) + 1 && IsValid(x, std::max(y1_ship, y2_ship) + 1)) {
                        y = std::max(y1_ship, y2_ship) + 1;
                    } else {
                        y = std::min(y1_ship, y2_ship) - 1;
                    }
                }
            }
        }
    }

    last_shot.first = x;
    last_shot.second = y;

    return std::pair<uint64_t, uint64_t>{x, y};
}

std::pair<uint64_t, uint64_t> DefaultStrategy::GetShot() {
    std::pair<uint64_t, uint64_t> cords = CalculateShot();

    while (true) {
        if (!IsBlocked(cords.first, cords.second) && IsValid(cords.first, cords.second)) {
            blocked_cells.emplace_back(cords);
            break;
        } else {
            cords = CalculateShot();
        }
    }

    return cords;
}

void DefaultStrategy::SetResult(uint8_t result) {
    if (result == 1) {
        in_elemination_mode = true;
        last_killed_ship_cords.emplace_back(last_shot);
    } else if (result == 2) {
        in_elemination_mode = false;
        last_killed_ship_cords.emplace_back(last_shot);
        BlockAroundShip();
        last_shot = {current_game_->GetWidth() - 1, current_game_->GetHeight() - 1};

        if (last_killed_ship_cords.size() == 4) {
            len4_killed++;

            if (current_game_->GetShipsCounter(4) == len4_killed) {
                step--;
            }

            if (current_game_->GetShipsCounter(4) == len4_killed && 
                current_game_->GetShipsCounter(3) == len3_killed) {
                step--;
            }

            if (current_game_->GetShipsCounter(4) == len4_killed && 
                current_game_->GetShipsCounter(3) == len3_killed && 
                current_game_->GetShipsCounter(2) == len2_killed) {
                step--;
            }

            if (current_game_->GetShipsCounter(4) == len4_killed && 
                current_game_->GetShipsCounter(3) == len3_killed && 
                current_game_->GetShipsCounter(2) == len2_killed && 
                current_game_->GetShipsCounter(1) == len1_killed) {
                step--;
            }
        } else if (last_killed_ship_cords.size() == 3) {
            len3_killed++;

            if (current_game_->GetShipsCounter(4) == len4_killed && 
                current_game_->GetShipsCounter(3) == len3_killed) {
                step--;
            }

            if (current_game_->GetShipsCounter(4) == len4_killed && 
                current_game_->GetShipsCounter(3) == len3_killed && 
                current_game_->GetShipsCounter(2) == len2_killed) {
                step--;
            }

            if (current_game_->GetShipsCounter(4) == len4_killed && 
                current_game_->GetShipsCounter(3) == len3_killed && 
                current_game_->GetShipsCounter(2) == len2_killed && 
                current_game_->GetShipsCounter(1) == len1_killed) {
                step--;
            }
        } else if (last_killed_ship_cords.size() == 2) {
            len2_killed++;

            if (current_game_->GetShipsCounter(4) == len4_killed && 
                current_game_->GetShipsCounter(3) == len3_killed && 
                current_game_->GetShipsCounter(2) == len2_killed) {
                step--;
            }

            if (current_game_->GetShipsCounter(4) == len4_killed && 
                current_game_->GetShipsCounter(3) == len3_killed && 
                current_game_->GetShipsCounter(2) == len2_killed && 
                current_game_->GetShipsCounter(1) == len1_killed) {
                step--;
            }
        } else if (last_killed_ship_cords.size() == 1) {
            len1_killed++;

            if (current_game_->GetShipsCounter(4) == len4_killed && 
                current_game_->GetShipsCounter(3) == len3_killed && 
                current_game_->GetShipsCounter(2) == len2_killed && 
                current_game_->GetShipsCounter(1) == len1_killed) {
                step--;
            }
        }

        last_killed_ship_cords.clear();

        if (current_game_->GetShipsCounter(4) == len4_killed &&
            current_game_->GetShipsCounter(3) == len3_killed && 
            current_game_->GetShipsCounter(2) == len2_killed &&
            current_game_->GetShipsCounter(1) == len1_killed) {
                current_game_->FinishGame();
        }
    }

}

std::pair<uint64_t, uint64_t> OrderedStrategy::GetShot() {
    std::pair<uint64_t, uint64_t> shot = last_shot;

    ++last_shot.first;
    if (last_shot.first > current_game_->GetWidth() - 1) {
        last_shot.first = 0;

        if (++last_shot.second > current_game_->GetHeight() - 1) {
            current_game_->FinishGame();
        }
    }

    return shot;
}

void OrderedStrategy::SetResult(uint8_t result) {
    if (result == 2) {
        ++ships_killed;
    }

    if (ships_killed == current_game_->GetShipsCounter(1) + 
        current_game_->GetShipsCounter(2) + 
        current_game_->GetShipsCounter(3) + 
        current_game_->GetShipsCounter(4)) {
            current_game_->FinishGame();
    }
}

Master::~Master() {
    delete current_strategy_;
    delete ships_coords_;
}

Slave::~Slave() {
    delete current_strategy_;
    delete ships_coords_;
}

Strategy* Master::GetStrategy() const {
    return current_strategy_;
}

Strategy* Slave::GetStrategy() const {
    return current_strategy_;
}

std::vector<Ship> ShipsCords::GetData() const {
    std::vector<Ship> result;

    for (size_t i = 0; i < size; i++) {
        result.push_back(data[i]);
    }

    return result;
}

std::vector<Ship> Master::GetShipCords() const {
    return ships_coords_->GetData();
}

std::vector<Ship> Slave::GetShipCords() const {
    return ships_coords_->GetData();
}

bool ShipsCords::CanPlaceShip(uint64_t x, uint64_t y, uint8_t len, uint8_t orient, uint64_t height, uint64_t width) const {
    if (orient == 0) {
        if (x + len >= width) {
            return false;
        }
    } else if (orient == 1) {
        if (y + len >= height) {
            return false;
        }
    }

    for (size_t i = 0; i < size; ++i) {
        Ship other = data[i];

        uint64_t other_min_x = other.x > 0 ? other.x - 1 : 0;
        uint64_t other_min_y = other.y > 0 ? other.y - 1 : 0;
        uint64_t other_max_x = other.x;
        uint64_t other_max_y = other.y;

        if (other.orientation == 0) {
            other_max_x += other.len - 1;
        } else if (other.orientation == 1) {
            other_max_y += other.len - 1;
        }

        if (other_max_x < width - 1) {
            other_max_x++;
        }
        if (other_max_y < height - 1) {
            other_max_y++;
        }

        for (uint64_t j = 0; j < len; ++j) {
            uint64_t ship_x = x;
            uint64_t ship_y = y;

            if (orient == 0) {
                ship_x += j;
            } else if (orient == 1) {
                ship_y += j;
            }

            if (ship_x >= other_min_x && ship_x <= other_max_x &&
                ship_y >= other_min_y && ship_y <= other_max_y) {
                return false;
            }
        }
    }

    return true;
}

void Master::StartGame() {
    if (cords_wad_loaded) {
        for (uint8_t i = 4; i > 0; i--) {
            if (current_game_->GetShipsCounter(i) != 0) {
                current_strategy_->SetStep(i);
                break;
            }
        }

        return;
    }

    Ship len4_ship1(0, 0, 4, 1);
    Ship len4_ship2(0, 5, 4, 1);
    Ship len4_ship3(0, 11, 4, 1);
    Ship len3_ship1(2, 1, 3, 1);
    Ship len3_ship2(2, 5, 3, 1);
    Ship len1_ship1(2, 12, 1, 1);
    current_strategy_->SetStep(4);

    ships_coords_->AddShip(len1_ship1);
    ships_coords_->AddShip(len3_ship1);
    ships_coords_->AddShip(len3_ship2);
    ships_coords_->AddShip(len4_ship1);
    ships_coords_->AddShip(len4_ship2);
    ships_coords_->AddShip(len4_ship3);
}

void Slave::StartGame() {
    if (cords_wad_loaded) {
        return;
    }

    uint64_t m = current_game_->GetHeight();
    uint64_t n = current_game_->GetWidth();

    uint64_t len1_emplaced = 0;
    uint64_t len2_emplaced = 0;
    uint64_t len3_emplaced = 0;
    uint64_t len4_emplaced = 0;

    for (uint64_t y = 0; y < m && len4_emplaced < current_game_->GetShipsCounter(4); ++y) {
        for (uint64_t x = 0; x < n && len4_emplaced < current_game_->GetShipsCounter(4); ++x) {
            if (ships_coords_->CanPlaceShip(x, y, 4, false, m, n)) {
                Ship ship_horizontal(x, y, 4, 0);
                ships_coords_->AddShip(ship_horizontal);
                ++len4_emplaced;
            } else if (ships_coords_->CanPlaceShip(x, y, 4, true, m, n)) {
                Ship ship_vertical(x, y, 4, 1);
                ships_coords_->AddShip(ship_vertical);
                ++len4_emplaced;
            }
        }
    }

    for (uint64_t y = 0; y < m && len3_emplaced < current_game_->GetShipsCounter(3); ++y) {
        for (uint64_t x = 0; x < n && len3_emplaced < current_game_->GetShipsCounter(3); ++x) {
            if (ships_coords_->CanPlaceShip(x, y, 3, false, m, n)) {
                Ship ship_horizontal(x, y, 3, 0);
                ships_coords_->AddShip(ship_horizontal);
                ++len3_emplaced;
            } else if (ships_coords_->CanPlaceShip(x, y, 3, true, m, n)) {
                Ship ship_vertical(x, y, 3, 1);
                ships_coords_->AddShip(ship_vertical);
                ++len3_emplaced;
            }
        }
    }

    for (uint64_t y = 0; y < m && len2_emplaced < current_game_->GetShipsCounter(2); ++y) {
        for (uint64_t x = 0; x < n && len2_emplaced < current_game_->GetShipsCounter(2); ++x) {
            if (ships_coords_->CanPlaceShip(x, y, 2, false, m, n)) {
                Ship ship_horizontal(x, y, 2, 0);
                ships_coords_->AddShip(ship_horizontal);
                ++len2_emplaced;
            } else if (ships_coords_->CanPlaceShip(x, y, 2, true, m, n)) {
                Ship ship_vertical(x, y, 2, 1);
                ships_coords_->AddShip(ship_vertical);
                ++len2_emplaced;
            }
        }
    }

    for (uint64_t y = 0; y < m && len1_emplaced < current_game_->GetShipsCounter(1); ++y) {
        for (uint64_t x = 0; x < n && len1_emplaced < current_game_->GetShipsCounter(1); ++x) {
            if (ships_coords_->CanPlaceShip(x, y, 2, true, m, n)) {
                Ship ship_single(x, y, 1, 0);
                ships_coords_->AddShip(ship_single);
                ++len1_emplaced;
            }
        }
    }
}

bool Game::GameIsStarted() const {
    return game_is_started_;
}

std::string Master::WhoIs() const {
    return "master";
}

std::string Slave::WhoIs() const {
    return "slave";
}

uint64_t DefaultStrategy::GetShipsKilled(uint8_t type_of_ship) const {
    switch (type_of_ship) {
    case 1:
        return len1_killed;
    
    case 2:
        return len2_killed;

    case 3:
        return len3_killed;

    case 4:
        return len4_killed;
    
    default:
        return 0;
    }        
}

uint64_t OrderedStrategy::GetShipsKilled(uint8_t type_of_ship) const {
    switch (type_of_ship) {
    case 1:
        return len1_killed;
    
    case 2:
        return len2_killed;

    case 3:
        return len3_killed;

    case 4:
        return len4_killed;
    
    default:
        return 0;
    }        
}

ShipsCords* Master::GetShipsCordsPtr() const {
    return ships_coords_;
}

ShipsCords* Slave::GetShipsCordsPtr() const {
    return ships_coords_;
}

void Master::CordsWasLoaded() {
    cords_wad_loaded = true;
}

void Slave::CordsWasLoaded() {
    cords_wad_loaded = true;
}

void DefaultStrategy::SetStep(uint8_t step_) {
    step = step_;
}

void OrderedStrategy::SetStep(uint8_t step_) {
    return;
}
