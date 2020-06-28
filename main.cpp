#include "MOFA/Utility/ParseOptions.h"
#include "MOFA/Utility/ErrorMessage.hpp"
#include "MOFA/Tokenizer/Token.h"
#include "MOFA/Tokenizer/Tokenizer.h"
#include "MOFA/Assembler/Assembler.h"
#include "MOFA/Utility/Logger.h"
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace MOFA;

int main(int argc, char** argv) {
    auto [input, output] = parseOptions(argc, argv);
    if(input.empty())
        return 0;

    std::ifstream ifs(input);
    if(!ifs.is_open()) {
        std::cout << "Failed to open file: " << input << std::endl;
        return 0;
    }

    SourceCode sourcecode;
    std::string line;
    while(std::getline(ifs, line))
        sourcecode.add(line, sourcecode.length() + 1);

    Tokenizer tokenizer(sourcecode);
    auto noerr = tokenizer.tokenize();
    //logAllTokens(tokenizer);
    if(!noerr) {
        logErrorList(tokenizer.getErrorList());
        return 0;
    }

    Assembler assembler(tokenizer.getTokenList());
    noerr = assembler.assemble();
    //logLabelAddress(assembler.getLabelAddress());
    if(!noerr) {
        logErrorList(assembler.getErrorList());
        return 0;
    }

    std::ofstream ofs(output);
    if(!ofs.is_open()) {
        std::cout << "Failed to open file: " << output << std::endl;
        return 0;
    }

    ofs << std::hex;
    for(auto code : assembler.getMachineCode())
        ofs << std::setw(8) << std::setfill('0') << code << std::endl;

    std::cout << "Success" << std::endl;

    return 0;
}
