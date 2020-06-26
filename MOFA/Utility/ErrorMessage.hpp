#pragma once
#include <string>

namespace MOFA {
    struct ErrorMessage {
        ErrorMessage(const unsigned _linenum,
                     const unsigned _linepos,
                     std::string _message) : linenum(_linenum),
                                             linepos(_linepos),
                                             message(std::move(_message)) {}

        const unsigned linenum;
        const unsigned linepos;
        const std::string message;
    };
}
