#include "sand_simulator.h"

void SimulateSandPile(Field& field, uint64_t iterations, uint64_t frequency, const std::filesystem::path& dir_path) {
    size_t length = field.get_length();
    uint64_t count = 0;
    
    for (uint64_t i = 0; i < iterations; ++i) {
        bool flag = false;
        Field tmp_field = field;

        for (size_t y = 0; y < field.get_length(); y++) {
            for (size_t x = 0; x < field.get_length(); x++) {
                tmp_field[x][y] = 0;
            }
        }

        length = field.get_length();

        for (size_t y = 0; y < field.get_length(); ++y) {
            for (size_t x = 0; x < field.get_length(); ++x) {
                if (field[x][y] >= 4) {
                    if ((x == 0 || y == 0 || y == length - 1 || x == length - 1) && !flag) {
                        flag = true;
                        field.expand();
                        tmp_field.expand();
                        ++x;
                        ++y;
                    }

                    uint64_t add = field[x][y] / 4;
                    field[x][y] %= 4;
                    tmp_field[x + 1][y] += add;
                    tmp_field[x][y + 1] += add;
                    tmp_field[x - 1][y] += add;
                    tmp_field[x][y - 1] += add;

                    

                    // while (field[x][y] >= 4) {
                    //     field[x + 1][y]++;
                    //     field[x][y + 1]++;
                    //     field[x - 1][y]++;
                    //     field[x][y - 1]++;
                    //     field[x][y] -= 4;
                    // }
                }
            }
        }

        for (size_t y = 0; y < field.get_length(); ++y) {
            for (size_t x = 0; x < field.get_length(); ++x) { 
                field[x][y] += tmp_field[x][y];
            }
        }


        if (frequency != 0 and ((i % frequency) == 0)) {
            char file_name[256];
            snprintf(file_name, sizeof(file_name), "%d.bmp", count);
            std::filesystem::path file_path = dir_path / file_name;
            count++;

            WriteBMP(file_path, field);
        }

        std::filesystem::path file_path = dir_path / "result.bmp";
        WriteBMP(file_path, field);
    }    
}

void BuildSandPile(Field& field, Vector cords) {
    Vector null;
    null.push_back(0);
    field.push_back(null);

    int16_t max_x = static_cast<int16_t>(cords[0] - NUM_OF_NEGATIVE);
    for (size_t i = 0; i < cords.get_length(); i += 3) {
        int16_t val = static_cast<int16_t>(cords[i] - NUM_OF_NEGATIVE);
        if (val > max_x and i < cords.get_length()) {
            max_x = val;
        }
    }

    int16_t min_x = static_cast<int16_t>(cords[0] - NUM_OF_NEGATIVE);
    for (size_t i = 0; i < cords.get_length(); i += 3) {
        int16_t val = static_cast<int16_t>(cords[i] - NUM_OF_NEGATIVE);
        if (val < min_x and i < cords.get_length()) {
            min_x = val;
        }
    }

    int16_t max_y = static_cast<int16_t>(cords[1] - NUM_OF_NEGATIVE);
    for (size_t i = 1; i < cords.get_length(); i += 3) {
        int16_t val = static_cast<int16_t>(cords[i] - NUM_OF_NEGATIVE);
        if (val > max_y and i < cords.get_length()) {
            max_y = val;
        }
    }

    int16_t min_y = static_cast<int16_t>(cords[1] - NUM_OF_NEGATIVE);
    for (size_t i = 1; i < cords.get_length(); i += 3) {
        int16_t val = static_cast<int16_t>(cords[i] - NUM_OF_NEGATIVE);
        if (val < min_y and i < cords.get_length()) {
            min_y = val;
        }
    }

    if (min_x < 0) {
        min_x *= -1;
    }

    if (min_y < 0) {
        min_y *= -1;
    }

    if (max_x < 0) {
        max_x *= -1;
    }

    if (max_y < 0) {
        max_y *= -1;
    }


    uint64_t delta = std::max(std::max(min_x, min_y), std::max(max_x, max_y));

    for (uint64_t i = 0; i < delta; i++) {
        field.expand();
    }

    for (size_t i = 0; i < cords.get_length(); i += 3) {
        size_t x = cords[i] - NUM_OF_NEGATIVE + delta;
        size_t y = cords[i] - NUM_OF_NEGATIVE + delta;
        field[x][y] = cords[i + 2];
    }

}