#include "Logger.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Tokenizer/Token.h"
#include "ErrorMessage.hpp"
#include <iostream>

namespace MOFA {
    void logAllTokens(const Tokenizer& _tokenizer) {
        auto& allTokens = _tokenizer.getTokenList();
        for(const auto& tokenline : allTokens) {
            for(const auto& token : tokenline) {
                std::cout << token.token;
                printf("|%d|%s\n", token.line, to_string(token.type));
            }
            puts("---------------------------");
        }
    }

    void logLabelAddress(const std::map<std::string_view, unsigned>& _labeladdress) {
        for(const auto& [name, addr] : _labeladdress)
            std::cout << std::string(name) << ": " << std::hex << addr << std::endl;
    }

    void logErrorList(const ErrorList& _errorlist) {
        for(auto& error : _errorlist)
            std::cout << to_string(error) << std::endl;
    }
}
