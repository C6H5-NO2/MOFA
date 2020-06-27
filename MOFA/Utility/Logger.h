#pragma once
#include "ErrorList.hpp"
#include <map>

namespace MOFA {
    class Tokenizer;
    void logAllTokens(const Tokenizer& _tokenizer);

    void logLabelAddress(const std::map<std::string_view, unsigned>& _labeladdress);

    void logErrorList(const ErrorList& _errorlist);
}
