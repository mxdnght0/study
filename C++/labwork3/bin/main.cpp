#include <iostream>
#include <filesystem>
#include "../lib/args_parser.h"
#include "../lib/file_parser.h"
#include "../lib/field.h"
#include "../lib/sand_simulator.h"
#include "../lib/bmp.h"



int main(int argc, char** argv) {
    CmdArgs args;
    Field field;

    if (!CmdParser(&args, argc, argv)) {
        return 1;
    }

    if (args.output == nullptr) {
        std::cerr << "There is no path to the output file!";
        exit(1);
    }

    if (args.input == nullptr) {
        std::cerr << "There is no input file!";
        exit(1);
    }

    std::filesystem::path dir_path = args.output;

    if (!std::filesystem::exists(dir_path)) {
        std::filesystem::create_directories(dir_path);
    }

    Vector parsed = ParseTsv(args.input);

    BuildSandPile(field, parsed);

    SimulateSandPile(field, args.max, args.freq, dir_path);
}
































    // Vector null;

    // null.push_back(1123);

    // Field heap;

    // heap.push_back(null);

    // SimulateSandPile(heap, 121234, 1);
    
    // for (int i = 0; i < heap.get_length(); i++) {
    //     for (int j = 0; j < heap[0].get_length(); j++) {
    //         std::cout << (int)heap[j][i] << " ";
    //     }
    //     std::cout << std::endl;
    // }