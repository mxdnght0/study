#pragma once

#include <cstdint>
#include <fstream>
#include <filesystem>
#include "field.h"

#pragma pack(push, 1)
struct BMPHeader {
    uint16_t bfType = 0;
    uint32_t bfSize = 0;
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits = 0;
};

struct BMPInfoHeader {
    uint32_t biSize = 0;
    int32_t  biWidth = 0;
    int32_t  biHeight = 0;
    uint16_t biPlanes = 0;
    uint16_t biBitCount = 0;
    uint32_t biCompression = 0;
    uint32_t biSizeImage = 0;
    int32_t  biXPelsPerMeter = 0;
    int32_t  biYPelsPerMeter = 0;
    uint32_t biClrUsed = 0;
    uint32_t biClrImportant = 0;
};


struct Color {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved = 0;
};
#pragma pack(pop)

void WriteBMP(const std::filesystem::path& file_path, const Field& field);
