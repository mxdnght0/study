#include "args_parser.h"


bool CmdParser(CmdArgs* Args, int32_t argc, char** argv) {
    for (int32_t i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-o") == 0 || strstr(argv[i], "--output") != nullptr) {
            if (Args->output != nullptr) {
                std::cerr << "Invalid " << argv[i] << " argument!" << std::endl;
                return false;
            }

            if (strchr(argv[i], '=') != nullptr) {
                strtok(argv[i], "=");
                Args->output = strtok(NULL, "=");
            } else {
                if (i + 1 == argc) {
                    std::cerr << "Invalid " << argv[i] << " argument!" << std::endl;
                    return false;
                }

                Args->output = argv[i + 1];
                i++;
            }
        } else if (strcmp(argv[i], "-i") == 0 || strstr(argv[i], "--input") != nullptr) {
            if (Args->input != nullptr) {
                std::cerr << "Invalid " << argv[i] << " argument!" << std::endl;
                return false;
            }

            if (strchr(argv[i], '=') != nullptr) {
                strtok(argv[i], "=");
                Args->input = strtok(NULL, "=");
            } else {
                if (i + 1 == argc) {
                    std::cerr << "Invalid " << argv[i] << " argument!" << std::endl;
                    return false;
                }

                Args->input = argv[i + 1];
                i++;
            }
        } else if (strcmp(argv[i], "-m") == 0 || strstr(argv[i], "--max-iter") != nullptr) {
            bool flag = (Args->max != Args->max_default);

            if (strchr(argv[i], '=') != nullptr) {
                char* error_handler;
                strtok(argv[i], "=");
                char* arg = strtok(NULL, "=");
                Args->max = strtol(arg, &error_handler, 10);

                if (strcmp(error_handler, "\0") != 0 || flag) {
                    std::cerr << "Invalid " << argv[i] << " argument!" << std::endl;
                    return false;
                }
            } else {
                if (i + 1 == argc) {
                    std::cerr << "Invalid " << argv[i] << " argument!" << std::endl;
                    return false;
                }

                char* error_handler;
                Args->max = strtol(argv[i + 1], &error_handler, 10);

                if (error_handler == argv[i + 1] || flag) {
                    std::cerr << "Invalid " << argv[i] << " argument!" << std::endl;
                    return false;
                }

                i++;
            }
        } else if (strcmp(argv[i], "-f") == 0 || strstr(argv[i], "--freq") != nullptr) {
            bool flag = (Args->freq != Args->freq_default);

            if (strchr(argv[i], '=') != nullptr) {
                char* error_handler;
                strtok(argv[i], "=");
                char* arg = strtok(NULL, "=");
                Args->freq = strtol(arg, &error_handler, 10);

                if (strcmp(error_handler, "\0") != 0 || flag) {
                    std::cerr << "Invalid " << argv[i] << " argument!" << std::endl;
                    return false;
                }
            } else {
                if (i + 1 == argc) {
                    std::cerr << "Invalid " << argv[i] << " argument!" << std::endl;
                    return false;
                }

                char* error_handler;
                Args->freq = strtol(argv[i + 1], &error_handler, 10);

                if (strcmp(error_handler, "\0") != 0 || flag) {
                    std::cerr << "Invalid " << argv[i] << " argument!" << std::endl;
                    return false;
                }

                i++;
            }
        } else {
            std::cerr << "Invalid argument!" << std::endl;
            return false;
        }
    }

    return true;
}



