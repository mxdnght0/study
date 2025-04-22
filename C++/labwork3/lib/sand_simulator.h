#pragma once

#include <cstdint>
#include <filesystem>
#include "field.h"
#include "bmp.h"

void SimulateSandPile(Field& field, uint64_t iterations, uint64_t frequency, const std::filesystem::path& dir_path);

void BuildSandPile(Field& field, Vector cords);