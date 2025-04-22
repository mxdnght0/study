#include "ArgParser.h"

bool IsAllDigits(const std::string& str) {
    for (char ch : str) {
        if (!std::isdigit(ch)) {
            return false;
        }
    }
    return !str.empty();
}

namespace ArgumentParser { 

    template class Argument<int64_t>;
    template class Argument<std::string>;
    template class Argument<bool>;

    ArgParser::ArgParser(const std::string parser_name) : parser_name_{parser_name} {}

    ArgParser::~ArgParser() {
        std::set<Argument<std::string>*> unique_string_arguments;
        std::set<Argument<int64_t>*> unique_int_arguments;
        std::set<Argument<bool>*> unique_bool_arguments;

        for (auto& entry : string_arguments) {
            unique_string_arguments.insert(entry.second);
        }
        for (auto& entry : int_arguments) {
            unique_int_arguments.insert(entry.second);
        }
        for (auto& entry : bool_arguments) {
            unique_bool_arguments.insert(entry.second);
        }

        for (auto* arg : unique_string_arguments) {
            delete arg;
        }
        for (auto* arg : unique_int_arguments) {
            delete arg;
        }
        for (auto* arg : unique_bool_arguments) {
            delete arg;
        }

        std::set<Argument<std::string>*> unique_pos_string_arguments;
        std::set<Argument<int64_t>*> unique_pos_int_arguments;

        for (auto& entry : pos_string_arguments) {
            unique_pos_string_arguments.insert(entry);
        }
        for (auto& entry : pos_int_arguments) {
            unique_pos_int_arguments.insert(entry);
        }

        for (auto* arg : unique_pos_string_arguments) {
            delete arg;
        }
        for (auto* arg : unique_pos_int_arguments) {
            delete arg;
        }
    }

    bool ArgParser::Parse(int argc, char** argv) {
        std::vector<std::string> spltted_string;

        for (int i = 0; i < argc; ++i) {
            char* token = std::strtok(argv[i], " ");
            while (token != nullptr) {
                spltted_string.emplace_back(token);
                token = std::strtok(nullptr, " ");
            }
        }

        return Parse(spltted_string);
    }

    bool ArgParser::Parse(std::vector<std::string> args) {
        this->AddPositionals();
        
        std::string type;

        std::vector<std::string> splitted_string;

            for (size_t i = 0; i < args.size(); ++i) {
            const std::string& line = args[i];
            size_t pos = line.find('=');

            if (pos != std::string::npos) {
                std::string left_part = line.substr(0, pos);
                std::string right_part = line.substr(pos + 1);
                splitted_string.push_back(left_part);
                splitted_string.push_back(right_part);
            } else {
                splitted_string.push_back(line);
            }
        }

        for (size_t i = 1; i < splitted_string.size(); i++) {
            std::string key = splitted_string[i];

            if (key[0] != '-') {
                if (IsAllDigits(key)) {
                    try {
                        size_t index = 0;
                        Argument<int64_t>* arg = this->pos_int_arguments.at(index);
                        if (arg->IsMultiValue()) {
                            int64_t multi_count = arg->GetMultiValue();
                            int64_t count = 0;
                            while (i + index < splitted_string.size()) {
                                size_t pos;
                                int64_t number;

                                if (!IsAllDigits(splitted_string[i + index])) {
                                    i--;
                                    break;
                                }

                                number = std::stoll(splitted_string[i + index], &pos);

                                if (pos != splitted_string[i + index].size()) {
                                    break;
                                } else if (arg->CheckStoreValues()) {
                                    arg->AddValue(number);
                                    index++;
                                    count++;
                                } else {
                                    return false;
                                }
                            }
                            if (multi_count > count) {
                                return false;
                            }
                            i += count;
                            pos_int_arguments.erase(pos_int_arguments.begin());
                        } else {
                            size_t pos;
                            int64_t number = std::stoll(splitted_string[i], &pos);

                            arg->AddValue(number);
                            pos_int_arguments.erase(pos_int_arguments.begin());
                            i++;
                        }
                    } catch (std::out_of_range&) {
                        return false;
                    }
                } else {
                    try {
                        size_t index = 0;
                        Argument<std::string>* arg = this->pos_string_arguments.at(index);
                        if (arg->IsMultiValue()) {
                            int64_t multi_count = arg->GetMultiValue();
                            int64_t count = 0;

                            while (i + index < splitted_string.size()) {
                                std::string str = splitted_string[i + index];
                                if (arg->CheckStoreValues()) {
                                    arg->AddValue(str);
                                    index++;
                                    count++;
                                } else {
                                    return false;
                                }
                            }
                            if (multi_count > count) {
                                return false;
                            }
                            i += count;
                            pos_string_arguments.erase(pos_string_arguments.begin());
                        } else {
                            std::string str = splitted_string.at(i);
                            arg->AddValue(str);
                            i++;
                        }
                    } catch (std::out_of_range&) {
                        return false;
                    }
                }
                continue;
            }

            while (key[0] == '-') {
                key = key.substr(1);
            }

            if (this->string_arguments.count(key) == 0 && this->int_arguments.count(key) == 0 && this->bool_arguments.count(key) == 0) {
                int16_t flags_counter = 0;

                for (size_t j = 0; j < key.length(); j++) {
                    if (this->bool_arguments.count(std::string(1, key[j])) != 0) {
                        
                        this->bool_arguments.at(std::string(1, key[j]))->AddValue(true);
                        ++flags_counter;
                    } 
                }

                if (flags_counter) {
                    continue;
                } 

                return false;
            }

            if (this->string_arguments.count(key) == 1) {
                type = "str";
            } else if (this->int_arguments.count(key) == 1) {
                type = "int";
            } else {
                type = "bool";
            }

            if (type == "bool") {
                this->bool_arguments.at(key)->AddValue(true);
            } else if (type == "int") {
                Argument<int64_t>* arg = this->int_arguments.at(key);

                if (arg->IsMultiValue()) {
                    int64_t multi_count = arg->GetMultiValue();
                    int64_t count = 0;
                    int64_t j = 1;

                    while (i + j < splitted_string.size()) {
                        size_t pos;
                        int64_t number;
                        try {
                            number = std::stoll(splitted_string[i + j], &pos);
                        } catch (std::invalid_argument&) {
                            break;
                        }
                        if (pos != splitted_string[i + j].size()) {
                            break;
                        }
                        if (arg->CheckStoreValues()) {
                            arg->AddValue(number);
                            j++;
                            count++;
                        } else {
                            return false;
                        }
                    }
                    if (multi_count > count) {
                        return false;
                    }
                    i += count;
                } else {
                    try {   
                        size_t pos;
                        int64_t number = std::stoll(splitted_string[i + 1], &pos);
                        
                        if (pos != splitted_string[i + 1].size()) {
                            return false;
                        }

                        arg->AddValue(number);
                        i++;
                    } catch (std::out_of_range&) {
                        return false;
                    } catch (std::invalid_argument&) {
                        return false;
                    }             
                }
            } else {
                Argument<std::string>* arg = this->string_arguments.at(key); 

                if (arg->IsHelp()) {
                    this->help = true;
                    return true;
                }

                if (arg->IsMultiValue()) {
                    int64_t multi_count = arg->GetMultiValue();
                    int64_t count = 0;
                    int64_t j = 1;

                    while (i + j < splitted_string.size()) {
                        std::string str = splitted_string[i + j];
                        if (arg->CheckStoreValues()) {
                            arg->AddValue(str);
                            j++;
                            count++;
                        } else {
                            return false;
                        }
                    }
                    if (multi_count > count) {
                        return false;
                    }
                    i += count;
                } else {
                    try {
                        std::string str = splitted_string.at(i + 1);
                        arg->AddValue(str);
                        i++;
                    } catch (std::out_of_range&) {
                        return false;
                    }
                }
            }
        }

        for (const auto& pair : this->int_arguments) {
            if (!pair.second->IsInitialized()) {
                return false;
            }
        } 

        for (const auto& pair : this->string_arguments) {
            if (!pair.second->IsInitialized() && !pair.second->IsHelp()) {
                return false;
            }
        } 

        return true;
    }

    Argument<std::string>& ArgParser::AddStringArgument(const std::string argument) {
        auto* arg = new Argument<std::string>();
        this->help_description += "--" + argument + "=<string>" + "\n";
        this->string_arguments[argument] = arg;

        return *this->string_arguments.at(argument);
    }

    Argument<std::string>& ArgParser::AddStringArgument(const char short_argument, const std::string argument) {
        auto* arg = new Argument<std::string>();
        this->help_description += "-" + std::string(1, short_argument) + " <string>," + " --" + argument + "=<string>" + "\n";
        this->string_arguments[argument] = arg;
        this->string_arguments[std::string(1, short_argument)] = arg;

        return *this->string_arguments.at(argument);
    }

    Argument<std::string>& ArgParser::AddStringArgument(const std::string argument, const std::string description) {
        auto* arg = new Argument<std::string>();
        this->help_description += "--" + argument + "=<string>, " + description + "\n";
        this->string_arguments[argument] = arg;

        return *this->string_arguments.at(argument);
    }

    Argument<std::string>& ArgParser::AddStringArgument(const char short_argument, const std::string argument, const std::string description) {
        auto* arg = new Argument<std::string>();
        this->help_description += "-" + std::string(1, short_argument) + " <string>," + " --" + argument + "=<string>, " + description + "\n";
        this->string_arguments[argument] = arg;
        this->string_arguments[std::string(1, short_argument)] = arg;

        return *this->string_arguments.at(argument);
    }

    std::string ArgParser::GetStringValue(const std::string argument) const {
        try {
            auto* arg = this->string_arguments.at(argument);
            return arg->GetValue();
        } catch (const std::out_of_range& e) {
            std::cerr << "Exception(out_of_range): " << e.what() << std::endl;
            std::terminate();
        }
    }

    std::string ArgParser::GetStringValue(const std::string argument, const size_t index) const {
        try {
            auto* arg = this->string_arguments.at(argument);
            return arg->GetValue();
        } catch (const std::out_of_range& e) {
            std::cerr << "Exception(out_of_range): " << e.what() << std::endl;
            std::terminate();
        }
    }

    Argument<int64_t>& ArgParser::AddIntArgument(const std::string argument) {
        auto* arg = new Argument<int64_t>();
        this->help_description += "--" + argument + "=<integer>" + "\n";
        this->int_arguments[argument] = arg;

        return *this->int_arguments.at(argument);
    }

    Argument<int64_t>& ArgParser::AddIntArgument(const char short_argument, const std::string argument) {
        auto* arg = new Argument<int64_t>();
        this->help_description += "-" + std::string(1, short_argument) + " <integer>," + " --" + argument + "=<integer>" + "\n";
        this->int_arguments[argument] = arg;
        this->int_arguments[std::string(1, short_argument)] = arg;

        return *this->int_arguments.at(argument);
    }

    Argument<int64_t>& ArgParser::AddIntArgument(const std::string argument, const std::string description) {
        auto* arg = new Argument<int64_t>();
        this->help_description += "--" + argument + "=<integer>, " + description + "\n";
        this->int_arguments[argument] = arg;
        

        return *this->int_arguments.at(argument);
    }

    Argument<int64_t>& ArgParser::AddIntArgument(const char short_argument, const std::string argument, const std::string description) {
        auto* arg = new Argument<int64_t>();
        this->help_description += "-" + std::string(1, short_argument) + " <integer>," + " --" + argument + "=<integer>, " + description + "\n";
        this->int_arguments[argument] = arg;
        this->int_arguments[std::string(1, short_argument)] = arg;
        

        return *this->int_arguments.at(argument);
    }

    int64_t ArgParser::GetIntValue(const std::string argument) const {
        try {
            auto* arg = this->int_arguments.at(argument);
            return arg->GetValue();
        } catch (const std::out_of_range& e) {
            std::cerr << "Exception(out_of_range): " << e.what() << std::endl;
            std::terminate();
        }
    }

    int64_t ArgParser::GetIntValue(const std::string argument, const size_t index) const {
        try {
            auto* arg = this->int_arguments.at(argument);
            return arg->GetValue(index);
        } catch (const std::out_of_range& e) {
            std::cerr << "Exception(out_of_range): " << e.what() << std::endl;
            std::terminate();
        }
    }

    Argument<bool>& ArgParser::AddFlag(const std::string argument) {
        auto* arg = new Argument<bool>();
        this->help_description += "--" + argument + "\n";
        this->bool_arguments[argument] = arg;

        return *this->bool_arguments.at(argument);
    }

    Argument<bool>& ArgParser::AddFlag(const char short_argument, const std::string argument) {
        auto* arg = new Argument<bool>();
        this->help_description += "-" + std::string(1, short_argument) + ", --" + argument + "\n";
        this->bool_arguments[argument] = arg;
        this->bool_arguments[std::string(1, short_argument)] = arg;

        return *this->bool_arguments.at(argument);
    }

    Argument<bool>& ArgParser::AddFlag(const std::string argument, const std::string description) {
        auto* arg = new Argument<bool>();
        this->help_description += "--" + argument + ", " + description + "\n";
        this->bool_arguments[argument] = arg;

        return *this->bool_arguments.at(argument);
    }

    Argument<bool>& ArgParser::AddFlag(const char short_argument, const std::string argument, const std::string description) {
        auto* arg = new Argument<bool>();
        this->help_description += "-" + std::string(1, short_argument) + ", --" + argument + ", " + description + "\n";
        this->bool_arguments[argument] = arg;
        this->bool_arguments[std::string(1, short_argument)] = arg;

        return *this->bool_arguments.at(argument);
    }

    bool ArgParser::GetFlag(const std::string argument) const {
        try {
            auto* arg = this->bool_arguments.at(argument);
            return arg->GetValue();
        } catch (std::out_of_range& e) {
            std::cerr << "Exception(out_of_range): " << e.what() << std::endl;
            std::terminate();
        }
    }

    bool ArgParser::GetFlag(const std::string argument, const size_t index) const {
        try {
            auto* arg = this->bool_arguments.at(argument);
            return arg->GetValue(index);
        } catch (std::out_of_range& e) {
            std::cerr << "Exception(out_of_range): " << e.what() << std::endl;
            std::terminate();
        }
    }

    void ArgParser::AddHelp(const char short_argument, const std::string argument, const std::string description) {
        auto* arg = new Argument<std::string>();
        this->help_description += "-" + std::string(1, short_argument) + ", --" + argument + ", " + description + "\n";
        arg->AddHelp();
        this->string_arguments[argument] = arg;
        this->string_arguments[std::string(1, short_argument)] = arg;
    }

    template <typename T>
    Argument<T>::Argument() {
        if (std::is_same<T, std::string>::value) {
            this->type_of_arg = "str";
        } else if (std::is_same<T, int64_t>::value) {
            this->type_of_arg = "int";
        } else {
            this->type_of_arg = "bool";
            this->value = false;
        }
    }

    template <typename T>
    Argument<T>& Argument<T>::Default(const T default_value) {
        if (this->value_is_initialized) {
            std::cerr << "Value was initialized before" << std::endl;
            std::terminate();
        } else if (this->multi_value_is_initialized) {
            std::cerr << "Default() cannot be used wiht MultiValue()" << std::endl;
            std::terminate();
        } else {
            this->value = default_value;
            if (this->store_value != nullptr) {
                *this->store_value = default_value;
            }
            this->value_is_initialized = true;
        }

        return *this;
    }

    template <typename T>
    Argument<T>& Argument<T>::Positional() {
        if (this->positional != false) {
            std::cerr << "Value was initialized before" << std::endl;
            std::terminate();
        } else {
            this->positional = true;
            
        }

        return *this;
    }

    template <typename T>
    Argument<T>& Argument<T>::MultiValue(const int64_t multi_value) {
        if (multi_value_is_initialized) {
            std::cerr << "Value was initialized before" << std::endl;
            std::terminate();
        } else if (store_value != nullptr) {
            std::cerr << "MultiValue() cannot be used with StoreValue()" << std::endl;
            std::terminate();
        } else {
            this->multi_value_ = multi_value;
            this->multi_value_is_initialized = true;
        }

        return *this;
    }

    template <typename T>
    Argument<T>& Argument<T>::MultiValue() {
        if (multi_value_is_initialized) {
            std::cerr << "Value was initialized before" << std::endl;
            std::terminate();
        } else if (this->store_value != nullptr) {
            std::cerr << "MultiValue() cannot be used with StoreValue()" << std::endl;
            std::terminate();
        } else {
            this->multi_value_is_initialized = true;
        }

        return *this;
    }

    template <typename T>
    Argument<T>& Argument<T>::StoreValue(T& val) {
        if (this->store_value != nullptr) {
            std::cerr << "Value was initialized before" << std::endl;
            std::terminate();
        } else if (this->multi_value_is_initialized) {
            std::cerr << "StoreValue() cannot be used with MultiValue()" << std::endl;
            std::terminate();
        } else {
            this->store_value = &val;
        }

        return *this;
    }

    template <typename T>
    Argument<T>& Argument<T>::StoreValues(std::vector<T>& vals) {
        if (this->store_values != nullptr) {
            std::cerr << "Value was initialized before" << std::endl;
            std::terminate();
        } else if (this->store_value != nullptr) {
            std::cerr << "StoreValues() cannot be used with StoreValue()" << std::endl;
            std::terminate();
        } else {
            this->store_values = &vals;
        }

        return *this;
    }

    template <typename T>
    T Argument<T>::GetValue() const {
        if (this->value_is_initialized) {
            return this->value;
        } else {
            std::cerr << "Value not initialized" << std::endl;
            std::terminate();
        }
    }

    template <typename T>
    T Argument<T>::GetValue(const size_t index) const {
        if (this->store_values != nullptr) {
            return (*this->store_values)[index];
        } else {
            std::cerr << "Value not initialized" << std::endl;
            std::terminate();
        }
    }

    template <typename T>
    void Argument<T>::AddValue(T val) {
        if (this->multi_value_is_initialized) {
            this->store_values->push_back(val);
            this->value_is_initialized = true;
        } else if (this->store_value != nullptr) {
            *(this->store_value) = val;
            this->value = val;
            this->value_is_initialized = true;
        } else {
            this->value = val;
            this->value_is_initialized = true;
        }
    }

    template <typename T>
    bool Argument<T>::IsMultiValue() const {
        return this->multi_value_is_initialized;
    }

    template <typename T>
    int64_t Argument<T>::GetMultiValue() const {
        return this->multi_value_;
    }

    template <typename T>
    bool Argument<T>::CheckStoreValues() const {
        return !(this->store_values == nullptr);
    }

    template <typename T>
    void Argument<T>::AddHelp() {
        this->is_help = true;
    }

    template <typename T>
    bool Argument<T>::IsHelp() const {
        return this->is_help;
    }

    template <typename T>
    bool Argument<T>::IsInitialized() const {
        return this->value_is_initialized;
    }

    template <typename T>
    bool Argument<T>::IsPositional() const {
        return this->positional;
    }

    template <typename T>
    Argument<T>::~Argument() = default;

    std::string ArgParser::HelpDescription() {
        return this->help_description;
    }

    std::string ArgParser::ASCIILogo() {
        return ascii;
    }

    void ArgParser::AddPositionals() {
        for (const auto& pair : this->string_arguments) {
            if (pair.second->IsPositional()) {
                this->pos_string_arguments.push_back(pair.second);
            }
        }  

        for (const auto& pair : this->int_arguments) {
            if (pair.second->IsPositional()) {
                this->pos_int_arguments.push_back(pair.second);
            }
        }   
    }


    bool ArgParser::Help() const {
        return this->help;
    }
}