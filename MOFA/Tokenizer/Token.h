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
        IMM5,
        IMM16,
        INSTRUCTION,
        LABEL,
        UNSUPPORTED_CHAR_CANDIDATE,
        UNSUPPORTED_IMM32
    };

    inline const char* to_string(TokenType e) {
        switch(e) {
            case TokenType::ERROR: return "ERROR";
            case TokenType::COMMENT: return "COMMENT";
            case TokenType::COLON: return "COLON";
            case TokenType::PAREN_L: return "PAREN_L";
            case TokenType::PAREN_R: return "PAREN_R";
            case TokenType::GEN_PUR_REG: return "GEN_PUR_REG";
            case TokenType::IMM5: return "IMM5";
            case TokenType::IMM16: return "IMM16";
            case TokenType::INSTRUCTION: return "INSTRUCTION";
            case TokenType::LABEL: return "LABEL";
            case TokenType::UNSUPPORTED_CHAR_CANDIDATE: return "UNSUPPORTED_CHAR_CANDIDATE";
            case TokenType::UNSUPPORTED_IMM32: return "UNSUPPORTED_IMM32";
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
