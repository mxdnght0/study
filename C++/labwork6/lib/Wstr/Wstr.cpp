#include <Wstr.h>

std::wstring StringToWstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.from_bytes(str);
}

std::unordered_map<wchar_t, std::string> translit_map = {
    {L'А', "A"}, {L'Б', "B"}, {L'В', "V"}, {L'Г', "G"}, {L'Д', "D"},
    {L'Е', "E"}, {L'Ё', "Yo"}, {L'Ж', "Zh"}, {L'З', "Z"}, {L'И', "I"},
    {L'Й', "Y"}, {L'К', "K"}, {L'Л', "L"}, {L'М', "M"}, {L'Н', "N"},
    {L'О', "O"}, {L'П', "P"}, {L'Р', "R"}, {L'С', "S"}, {L'Т', "T"},
    {L'У', "U"}, {L'Ф', "F"}, {L'Х', "Kh"}, {L'Ц', "Ts"}, {L'Ч', "Ch"},
    {L'Ш', "Sh"}, {L'Щ', "Sch"}, {L'Ь', ""}, {L'Ы', "Y"}, {L'Ъ', ""}, 
    {L'Э', "E"}, {L'Ю', "Yu"}, {L'Я', "Ya"},
    {L'а', "a"}, {L'б', "b"}, {L'в', "v"}, {L'г', "g"}, {L'д', "d"},
    {L'е', "e"}, {L'ё', "yo"}, {L'ж', "zh"}, {L'з', "z"}, {L'и', "i"},
    {L'й', "y"}, {L'к', "k"}, {L'л', "l"}, {L'м', "m"}, {L'н', "n"},
    {L'о', "o"}, {L'п', "p"}, {L'р', "r"}, {L'с', "s"}, {L'т', "t"},
    {L'у', "u"}, {L'ф', "f"}, {L'х', "kh"}, {L'ц', "ts"}, {L'ч', "ch"},
    {L'ш', "sh"}, {L'щ', "sch"}, {L'ь', ""}, {L'ы', "y"}, {L'ъ', ""}, 
    {L'э', "e"}, {L'ю', "yu"}, {L'я', "ya"}
};

std::string Transliterate(const std::string& input) {
    std::wstring wstr = StringToWstring(input);

    std::string output = "";
    for (wchar_t c : wstr) {
        if (translit_map.find(c) != translit_map.end()) {
            output += translit_map[c];
        } else {
            output += static_cast<char>(c);
        }
    }
    return output;
}