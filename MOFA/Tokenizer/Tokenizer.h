#pragma once
#include "TokenView.hpp"
#include "../Utility/ErrorList.hpp"
#include "../SourceCode/SourceCode.hpp"

namespace MOFA {
    class StringViewBuffer;

    class Tokenizer {
    public:
        Tokenizer(SourceCode _source);
        bool tokenize();
        const TokenList& getTokenList() const noexcept;
        const ErrorList& getErrorList() const noexcept;

    private:
        const SourceCode source;
        TokenList tokenlist;
        ErrorList errorlist;

        TokenLine tokenizeLine(const SourceLine& _sourceline);

        /// @brief Add token and generate error message.
        void addToken(TokenLine& _dst, const std::string_view& _candidate, const unsigned _line);

        /// @brief Add token constructed by buffer to line. buffer is cleared afterwards.
        void pushBuffer(TokenLine& _dst, StringViewBuffer& _buf, const unsigned _line);
    };
}
