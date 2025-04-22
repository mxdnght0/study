#include <functional>
#include <./lib/ArgParser.h>

#include <iostream>
#include <numeric>
#include <vector>


struct Options {
    bool sum = false;
    bool mult = false;
};

int main(int argc, char** argv) {
    Options opt;
    std::vector<int64_t> values;
    
    ArgumentParser::ArgParser parser("My Parser");
    parser.AddHelp('h', "help", "Some Description about program");
    parser.AddStringArgument('i', "input", "File path for input file").MultiValue(1);
    parser.AddFlag('s', "flag1", "Use some logic").Default(true);
    parser.AddFlag('p', "flag2", "Use some logic");
    parser.AddIntArgument("number", "Some Number");

    std::cout << parser.ASCIILogo() << std::endl;
}
