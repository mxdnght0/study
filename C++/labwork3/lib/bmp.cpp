#include "bmp.h"

void WriteBMP(const std::filesystem::path& file_path, const Field& field) {

    std::ofstream file(file_path, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot create file!" << std::endl;
        return;
    }

    uint32_t width = field[0].get_length();
    uint32_t height = field.get_length();
    uint32_t row_size = (width + 1) / 2;
    uint32_t padding_size = (4 - (row_size % 4)) % 4;
    uint32_t data_size = row_size * height;

    BMPHeader bmpHeader;
    BMPInfoHeader bmpInfoHeader;

    bmpHeader.bfType = 0x4D42; // 'BM'
    bmpHeader.bfSize = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + 16 * sizeof(Color) + data_size;
    bmpHeader.bfReserved1 = 0;
    bmpHeader.bfReserved2 = 0;
    bmpHeader.bfOffBits = sizeof(BMPHeader) + sizeof(BMPInfoHeader) + 16 * sizeof(Color);

    bmpInfoHeader.biSize = sizeof(BMPInfoHeader);
    bmpInfoHeader.biWidth = width;
    bmpInfoHeader.biHeight = height;
    bmpInfoHeader.biPlanes = 1;
    bmpInfoHeader.biBitCount = 4;
    bmpInfoHeader.biSizeImage = data_size;
    bmpInfoHeader.biClrUsed = 5;

    file.write(reinterpret_cast<char*>(&bmpHeader), sizeof(bmpHeader));
    file.write(reinterpret_cast<char*>(&bmpInfoHeader), sizeof(bmpInfoHeader));

    Color palette[16] = {
        {255, 255, 255, 0},
        {204, 153, 255, 0},
        {204, 102, 0, 0},
        {63, 224, 208, 0},
        {204, 0, 255, 0},
    };
    file.write(reinterpret_cast<char*>(palette), sizeof(palette));

    Vector bit_map_data(data_size);
    for (size_t i = 0; i < data_size; ++i) {
        bit_map_data.push_back(0);
    }

    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            uint8_t color_index = field[x][y];
            if (field[x][y] > 4) {
                color_index = 4;
            }

            size_t bit_map_index = y * row_size + x / 2;

            if (x % 2 == 0) {
                bit_map_data[bit_map_index] = color_index << 4;
            } else {
                bit_map_data[bit_map_index] |= color_index;
            }
        }
    }

    for (size_t y = height; y-- > 0;) {
        for (size_t x = 0; x < row_size; ++x) {
            uint8_t value = static_cast<uint8_t>(bit_map_data[y * row_size + x]);
            file.write(reinterpret_cast<char*>(&value), sizeof(uint8_t));
        }

        if (padding_size > 0) {
            uint8_t padding[3] = {0}; 
            file.write(reinterpret_cast<char*>(padding), padding_size);
        }
    }

    file.close();
}
