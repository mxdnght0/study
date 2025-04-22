#include "file_parser.h"

Vector ParseTsv(const char* input) {
    std::ifstream file(input);
    Vector parsed_line;

    if (!file.is_open()) {
        std::cerr << "Cannot open input file!" << std::endl;
        exit(1);
    }

    const size_t BUFFER_SIZE = 1024;
    char buffer[BUFFER_SIZE];

    while (file.getline(buffer, BUFFER_SIZE)) {
        char* str = std::strtok(buffer, "	");

        int counter = 0;
        while (str != nullptr) {
            char* endptr;
            uint64_t value;

            if ((counter % 3) == 2) {
                value = strtoull(str, &endptr, 10);
            } else {
                value = strtol(str, &endptr, 10) + RANGE_OF_NEGATIVE;
            }

            if (*endptr != '\0') {
                std::cerr << "invalid tsv!" << std::endl;
                exit(1);
            }
            
            parsed_line.push_back(value);
            str = strtok(NULL, "	");
            ++counter;
        }
    }

    file.close();
    
    return parsed_line;
}
