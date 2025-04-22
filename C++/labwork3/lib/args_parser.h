#pragma once
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdint>


struct CmdArgs {
    const uint64_t max_default = 0;
    const uint64_t freq_default = 0;

    const char* input = nullptr;
    const char* output = nullptr;

    uint64_t max = 0;
    uint64_t freq = 0;

};

bool CmdParser(CmdArgs* Args, int32_t argc, char** argv);



