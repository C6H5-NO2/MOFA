#pragma once
#include "Token.h"
#include "TokenView.hpp"
#include "../Utility/ErrorMessage.hpp"
#include "../Utility/ErrorList.hpp"
#include "../SourceCode/SourceCode.hpp"

namespace MOFA {
    class Tokenizer {
    public:
        Tokenizer(SourceCode _source);
        bool tokenize();
        const auto& getTokenList() const noexcept { return tokenlist; }
        const auto& getErrorList() const noexcept { return errorlist; }

    private:
        const SourceCode source;
        TokenList tokenlist;
        ErrorList errorlist;

        TokenLine tokenizeLine(const SourceLine& _sourceline);
    };
}
