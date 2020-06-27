#pragma once
#include "../Utility/AddOnlyVector.hpp"

namespace MOFA {
    struct Token;
    using TokenLine = AddOnlyVector<Token>;
    using TokenList = AddOnlyVector<TokenLine>;
}
