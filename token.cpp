#include <vector>
#include <string>
#define vchar "_azertyuiopqsdfghjklmwxcvbnAZERTYUIOPQSDFGHJKLMWXCVBN1234567890"

std::vector<std::string> LPLtokenize(std::string file, bool pushSpace) {
    std::vector<std::string> tokens = {};
    std::string valid_chars = vchar, current_token = "";
    for (auto chr : file) {
        auto it = valid_chars.find(chr);
        if (it != std::string::npos) {
            current_token += chr;
            continue;
        }
        if (chr == ' ') {
            if (current_token != "") tokens.push_back(current_token);
            if (pushSpace) tokens.push_back(" ");
            current_token = "";
            continue;
        }
        if (current_token != "") tokens.push_back(current_token);
        tokens.push_back(std::string(1, chr));
        current_token = "";
    }
    tokens.push_back(current_token);
    return tokens;
}

std::vector<std::vector<std::string>> LPLtokenize(std::vector<std::string> str) {
    std::vector<std::vector<std::string>> tt;
    std::vector<std::string> t;
    for (auto s : str) {
        if (s == " ") {
            tt.push_back(t);
            t.clear();
            continue;
        }
        t.push_back(s);
    }
    tt.push_back(t);
    return tt;
}