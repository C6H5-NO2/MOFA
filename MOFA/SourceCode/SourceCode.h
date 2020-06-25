#pragma once
#include "SourceLine.h"
#include <vector>

namespace MOFA {
    class SourceCode {
    public:
        SourceCode() = default;

        void addLine(std::string _line) { lines.emplace_back(_line, lines.size()); }

        // todo: pack this
        auto& getlines() { return lines; }

    private:
        std::vector<SourceLine> lines;
    };
}
