#pragma once
#include <string>

namespace MOFA {
    struct SourceLine {
        SourceLine(const std::string_view _linecode,
                   const unsigned _linenum) : linecode(_linecode),
                                              linenum(_linenum) {}

        const std::string linecode;
        const unsigned linenum;
    };
}
