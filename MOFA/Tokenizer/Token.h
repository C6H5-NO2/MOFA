#pragma once
#include <string>

namespace MOFA {
    enum class TokenType {
        ERROR,
        COMMENT,
        //POSITIVE,
        //NEGATIVE,
        COLON,
        PAREN_L,
        PAREN_R,
        BRACKET_L,
        BRACKET_R,
        CHAR_LITERAL,
        GEN_PUR_REG,
        VEC_REG,
        UINT5,
        UINT16,
        UINT32,
        INT16,
        INT32,
        //INSTRUCTION,
        IDENTIFIER,
        TOKENIZER_ERROR
    };

    inline const char* to_string(TokenType e) {
        switch(e) {
            case TokenType::ERROR: return "ERROR";
            case TokenType::COMMENT: return "COMMENT";
            case TokenType::COLON: return "COLON";
            case TokenType::PAREN_L: return "PAREN_L";
            case TokenType::PAREN_R: return "PAREN_R";
            case TokenType::BRACKET_L: return "BRACKET_L";
            case TokenType::BRACKET_R: return "BRACKET_R";
            case TokenType::CHAR_LITERAL: return "CHAR_LITERAL";
            case TokenType::GEN_PUR_REG: return "GEN_PUR_REG";
            case TokenType::VEC_REG: return "VEC_REG";
            case TokenType::UINT5: return "UINT5";
            case TokenType::UINT16: return "UINT16";
            case TokenType::UINT32: return "UINT32";
            case TokenType::INT16: return "INT16";
            case TokenType::INT32: return "INT32";
            case TokenType::IDENTIFIER: return "IDENTIFIER";
            case TokenType::TOKENIZER_ERROR: return "TOKENIZER_ERROR";
            default: return "unknown";
        }
    }


    struct Token {
        Token(const std::string_view _token);

        const std::string token;
        const TokenType type;
    };
}
