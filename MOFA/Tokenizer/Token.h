#pragma once
#include <string>

namespace MOFA {
    enum class TokenType {
        ERROR,
        COMMENT,
        COLON,
        PAREN_L,
        PAREN_R,
        GEN_PUR_REG,
        UINT5,
        UINT16,
        INT16,
        INSTRUCTION,
        LABEL,
        UNSUPPORTED_CHAR_CANDIDATE,
        UNSUPPORTED_UINT32,
        UNSUPPORTED_INT32
    };

    inline const char* to_string(TokenType e) {
        switch(e) {
            case TokenType::ERROR: return "ERROR";
            case TokenType::COMMENT: return "COMMENT";
            case TokenType::COLON: return "COLON";
            case TokenType::PAREN_L: return "PAREN_L";
            case TokenType::PAREN_R: return "PAREN_R";
            case TokenType::GEN_PUR_REG: return "GEN_PUR_REG";
            case TokenType::UINT5: return "UINT5";
            case TokenType::UINT16: return "UINT16";
            case TokenType::INT16: return "INT16";
            case TokenType::INSTRUCTION: return "INSTRUCTION";
            case TokenType::LABEL: return "LABEL";
            case TokenType::UNSUPPORTED_CHAR_CANDIDATE: return "UNSUPPORTED_CHAR_CANDIDATE";
            case TokenType::UNSUPPORTED_UINT32: return "UNSUPPORTED_UINT32";
            case TokenType::UNSUPPORTED_INT32: return "UNSUPPORTED_INT32";
            default: return "unknown";
        }
    }


    struct Token {
        Token(const std::string_view _token, const unsigned _line);

        const std::string token;
        const unsigned line;
        const TokenType type;
    };
}
