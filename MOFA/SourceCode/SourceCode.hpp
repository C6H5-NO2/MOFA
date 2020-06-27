#pragma once
#include "../Utility/AddOnlyVector.hpp"

namespace MOFA {
    struct SourceLine {
        SourceLine(const std::string_view _code,
                   const unsigned _line) : code(_code),
                                           line(_line) {}

        const std::string code;
        const unsigned line;
    };

    using SourceCode = AddOnlyVector<SourceLine>;
}
