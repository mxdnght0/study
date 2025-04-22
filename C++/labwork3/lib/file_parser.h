#pragma once
#include <cstdint>
#include <fstream>
#include "field.h"

const uint64_t RANGE_OF_NEGATIVE = 32768;

Vector ParseTsv(const char* input);