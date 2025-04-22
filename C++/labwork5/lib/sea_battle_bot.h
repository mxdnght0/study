#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <utility>
#include <algorithm>
#include <iostream>

#pragma pack(push, 1)

class Game {
private:
    uint64_t width_ = 0;
    uint64_t height_ = 0;
    bool game_is_started_ = false;
    bool game_is_finished_ = false;
    uint64_t len1_ships_counter_ = 0;
    uint64_t len2_ships_counter_ = 0;
    uint64_t len3_ships_counter_ = 0;
    uint64_t len4_ships_counter_ = 0;

public:
    Game() = default;
    void SetWidth(const uint64_t width);
    void SetHeight(const uint64_t height);
    uint64_t GetWidth() const;
    uint64_t GetHeight() const;
    void StartGame();
    void StopGame();
    bool GameIsStarted() const;
    bool GameIsFinished() const;
    void FinishGame();
    void SetShipsCounter(uint8_t ship_type, uint64_t ships_counter);
    uint64_t GetShipsCounter(uint8_t ship_type) const;
};

class Strategy {
protected:
    Game* current_game_ = nullptr;
    uint64_t len4_killed = 0;
    uint64_t len3_killed = 0;
    uint64_t len2_killed = 0;
    uint64_t len1_killed = 0;

public:
    virtual ~Strategy() = default;
    void SetGame(Game* current_game);
    virtual std::pair<uint64_t, uint64_t> GetShot() = 0;
    virtual void SetResult(uint8_t result) = 0;
    virtual uint64_t GetShipsKilled(uint8_t type_of_ship) const = 0;
    virtual void SetStep(uint8_t step_) = 0;
};

class DefaultStrategy : public Strategy {
private:
    uint8_t step = 4;
    std::pair<uint64_t, uint64_t> last_shot{0, 0};
    std::vector<std::pair<uint64_t, uint64_t>> blocked_cells;
    std::vector<std::pair<uint64_t, uint64_t>> last_killed_ship_cords;
    bool in_elemination_mode = false;
    bool IsBlocked(uint64_t x, uint64_t y);
    bool IsValid(uint64_t x, uint64_t y) const;
    void BlockAroundShip();
    std::pair<uint64_t, uint64_t> CalculateShot();

public:
    ~DefaultStrategy() = default;
    std::pair<uint64_t, uint64_t> GetShot() override;
    void SetResult(uint8_t result) override;
    uint64_t GetShipsKilled(uint8_t type_of_ship) const override;
    void SetStep(uint8_t step_) override;
};

class OrderedStrategy : public Strategy {
private:
    std::pair<uint64_t, uint64_t> last_shot{0, 0};
    uint64_t ships_killed = 0;

public:
    ~OrderedStrategy() = default;
    std::pair<uint64_t, uint64_t> GetShot() override;
    void SetResult(uint8_t result) override;
    uint64_t GetShipsKilled(uint8_t type_of_ship) const override;
    void SetStep(uint8_t step_) override;
};  

struct Ship {
    uint64_t x;
    uint64_t y;
    uint8_t len;
    uint8_t orientation;
    uint8_t hitted;

    Ship(uint64_t x_coord, uint64_t y_coord, uint8_t length, uint8_t orient);
    Ship();
    Ship(const Ship& other);
    Ship& operator=(const Ship& other);
    ~Ship() = default;
};

class ShipsCords {
private:
    uint64_t size{0};
    Ship* data{nullptr};

public:
    ~ShipsCords();
    void AddShip(Ship ship);
    bool CanPlaceShip(uint64_t x, uint64_t y, uint8_t len, uint8_t orient, uint64_t height, uint64_t width) const;
    uint8_t CheckShot(uint64_t x, uint64_t y);
    std::vector<Ship> GetData() const;
};

class Player {
protected:
    Game* current_game_ = nullptr;
    Strategy* current_strategy_ = nullptr;
    ShipsCords* ships_coords_ = nullptr;
    uint64_t player_ships_killed = 0;
    bool cords_wad_loaded = false;

public:
    virtual ~Player() = default;
    virtual void SetGame(Game* current_game) = 0;
    virtual void SetGameSettings() = 0;
    virtual void SetStrategy(Strategy* current_stretegy) = 0;
    virtual Strategy* GetStrategy() const = 0;
    virtual void SetShipsCords(ShipsCords* ships_coords) = 0;
    virtual uint8_t Shot(uint64_t x, uint64_t y) = 0;
    virtual std::pair<uint64_t, uint64_t> GetShot() = 0;
    virtual void SetResult(uint8_t result) = 0;
    virtual std::vector<Ship> GetShipCords() const = 0;
    virtual void StartGame() = 0;
    virtual std::string WhoIs() const = 0;
    virtual ShipsCords* GetShipsCordsPtr() const = 0;
    virtual void CordsWasLoaded() = 0;
};

class Master : public Player {
public:
    ~Master();
    void SetGame(Game* current_game);
    void SetGameSettings();
    void SetStrategy(Strategy* current_stretegy);
    Strategy* GetStrategy() const;
    void SetShipsCords(ShipsCords* ships_coords);
    uint8_t Shot(uint64_t x, uint64_t y);
    std::pair<uint64_t, uint64_t> GetShot();
    void SetResult(uint8_t result);
    std::vector<Ship> GetShipCords() const;
    void StartGame();
    std::string WhoIs() const;
    ShipsCords* GetShipsCordsPtr() const;
    void CordsWasLoaded();
};

class Slave : public Player {
public:
    ~Slave();
    void SetGame(Game* current_game);
    void SetGameSettings();
    void SetStrategy(Strategy* current_stretegy);
    Strategy* GetStrategy() const;
    void SetShipsCords(ShipsCords* ships_coords);
    uint8_t Shot(uint64_t x, uint64_t y);
    std::pair<uint64_t, uint64_t> GetShot();
    void SetResult(uint8_t result);
    std::vector<Ship> GetShipCords() const;
    void StartGame();
    std::string WhoIs() const;
    ShipsCords* GetShipsCordsPtr() const;
    void CordsWasLoaded();
};

#pragma pack(pop)