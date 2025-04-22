#pragma once

#include <string>
#include <vector>
#include <map>
#include <any>
#include <cstdint>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <format>
#include <set>

bool IsAllDigits(const std::string& str);

namespace ArgumentParser {

    template <typename T>
    class Argument {
    private:
        T value;
        std::string type_of_arg;
        std::vector<T> values;
        bool value_is_initialized = false;
        int64_t multi_value_ = -1;
        bool multi_value_is_initialized = false;
        bool positional = false;
        T* store_value = nullptr;
        std::vector<T>* store_values = nullptr;
        bool is_help = false;
        
    public:
        Argument();
        ~Argument();
        Argument<T>& Default(const T default_value);
        Argument<T>& Positional();
        Argument<T>& MultiValue(const int64_t multi_value);
        Argument<T>& MultiValue();
        Argument<T>& StoreValue(T& val);
        Argument<T>& StoreValues(std::vector<T>& vals);
        T GetValue() const;
        T GetValue(const size_t index) const;
        void AddValue(T val);
        bool IsMultiValue() const;
        bool IsPositional() const;
        int64_t GetMultiValue() const;
        bool CheckStoreValues() const;
        bool IsHelp() const;
        void AddHelp();
        bool IsInitialized() const;
    };

    class ArgParser {
    private:
        const std::string parser_name_;
        std::string help_description = parser_name_ + "\n";
        bool help = false;
        std::map<std::string, Argument<std::string>*> string_arguments;
        std::map<std::string, Argument<int64_t>*> int_arguments;
        std::map<std::string, Argument<bool>*> bool_arguments;
        std::vector<Argument<std::string>*> pos_string_arguments;
        std::vector<Argument<int64_t>*> pos_int_arguments;  

    public:
        ArgParser(const std::string parser_name);
        ~ArgParser();
        bool Parse(int argc, char** argv);
        bool Parse(std::vector<std::string> parsed_string);
        Argument<std::string>& AddStringArgument(const std::string argument);
        Argument<std::string>& AddStringArgument(const char short_argument, const std::string argument);
        Argument<std::string>& AddStringArgument(const std::string argument, const std::string description);
        Argument<std::string>& AddStringArgument(const char short_argument, const std::string argument, const std::string description);
        std::string GetStringValue(const std::string argument) const;
        std::string GetStringValue(const std::string argument, const size_t index) const;
        Argument<int64_t>& AddIntArgument(const std::string argument);
        Argument<int64_t>& AddIntArgument(const char short_argument, const std::string argument);
        Argument<int64_t>& AddIntArgument(const std::string argument, const std::string description);
        Argument<int64_t>& AddIntArgument(const char short_argument, const std::string argument, const std::string description);
        void AddPositionals();
        int64_t GetIntValue(const std::string argument) const;
        int64_t GetIntValue(const std::string argument, const size_t index) const;
        Argument<bool>& AddFlag(const std::string argument);
        Argument<bool>& AddFlag(const char short_argument, const std::string argument);
        Argument<bool>& AddFlag(const std::string argument, const std::string description);
        Argument<bool>& AddFlag(const char short_argument, const std::string argument, const std::string description);
        bool GetFlag(const std::string argument) const;
        bool GetFlag(const std::string argument, const size_t index) const;
        void AddHelp(const char short_argument, const std::string argument, const std::string description);
        bool Help() const;
        std::string HelpDescription();
        std::string ASCIILogo();
    };
}