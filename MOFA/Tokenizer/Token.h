#pragma once
#include <string_view>

namespace MOFA {
    enum class TokenType {
        COMMENT,
        //POSITIVE,
        //NEGATIVE,
        COLON,
        PAREN_L,
        PAREN_R,
        BRACKET_L,
        BRACKET_R,
        UINT5,
        UINT16,
        UINT32,
        INT16,
        INT32,
        GEN_PUR_REG,
        VEC_REG,
        IDENTIFIER,
        ERROR
        //DELIMITER,
        //DOT,
        //OPERATOR,
    };


    struct Token {
        Token(const std::string_view _token);

        const std::string_view token;
        const TokenType type;
    };
}
