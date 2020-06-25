#pragma once
#include <string>

namespace MOFA {
    // todo: use storage-view pattern to save memory
    class SourceLine {
    public:
        SourceLine(std::string _linecode, const int _linenum) : linecode(std::move(_linecode)),
                                                                linenum(_linenum) {}

        const auto& getCode() const noexcept { return linecode; }
        int getLinenum() const noexcept { return linenum; }

    private:
        std::string linecode;
        int linenum;
    };
}
