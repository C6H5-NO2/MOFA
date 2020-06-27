#pragma once
#include <string>
#include <sstream>

namespace MOFA {
    enum class ErrorType {
        INVALID_TOKEN,
        INVALID_INSTURCTION,
        INVALID_OPERAND,
        ALREADY_DEFINED,
        UNSUPPORTED_FEATURE
    };

    inline const char* to_string(ErrorType e) {
        switch(e) {
            case ErrorType::INVALID_TOKEN: return "INVALID_TOKEN";
            case ErrorType::INVALID_INSTURCTION: return "INVALID_INSTURCTION";
            case ErrorType::INVALID_OPERAND: return "INVALID_OPERAND";
            case ErrorType::ALREADY_DEFINED: return "ALREADY_DEFINED";
            case ErrorType::UNSUPPORTED_FEATURE: return "UNSUPPORTED_FEATURE";
            default: return "unknown";
        }
    }


    struct ErrorMessage {
        ErrorMessage(const unsigned _line,
                     const ErrorType _type,
                     std::string _info) : line(_line),
                                          type(_type),
                                          info(std::move(_info)) {}

        const unsigned line;
        const ErrorType type;
        const std::string info;
    };

    inline std::string to_string(const ErrorMessage& _error) {
        std::stringstream ss;
        ss << "error in line " << _error.line << ". " << to_string(_error.type) << ": " << _error.info;
        return ss.str();
    }
}
