#pragma once
#include "../Register/RegFile.h"

namespace MOFA {
    class Architecture {
    public:
        Architecture() { newestInstance = this; }
        static const Architecture& getInstance() noexcept { return *newestInstance; }
        const auto& getRegFile() const noexcept { return regfile; }

    private:
        inline static Architecture* newestInstance = nullptr;
        RegFile regfile;
    };
}
