#pragma once
#include "TokenLine.hpp"
#include "../SourceCode/SourceCode.hpp"
#include "../Utility/ErrorList.hpp"

namespace MOFA {
    class Tokenizer {
    public:
        Tokenizer(SourceCode _source);
        bool tokenize();
        const auto& getAllTokens() const noexcept { return allTokens; }
        const auto& getErrorList() const noexcept { return errorlist; }

    private:
        SourceCode source;
        AddOnlyVector<TokenLine> allTokens;
        ErrorList errorlist;

        TokenLine tokenizeLine(const SourceLine& _sourceline);
    };
}
