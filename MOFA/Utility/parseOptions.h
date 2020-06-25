#pragma once
#include <string>

namespace MOFA {
    std::pair<std::string, std::string> parseOptions(int _argc, char** _argv);
}
