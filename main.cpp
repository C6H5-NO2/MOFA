#include "MOFA/Utility/ParseOptions.h"
#include "MOFA/Utility/Global.h"
#include "MOFA/Utility/ErrorMessage.hpp"
#include "MOFA/Instruction/InstructionSet.h"
#include "MOFA/Register/RegisterFile.h"
#include "MOFA/SourceCode/SourceCode.hpp"
#include "MOFA/SourceCode/SourceLine.hpp"
#include "MOFA/Tokenizer/Tokenizer.h"
#include "MOFA/Tokenizer/Token.h"

#include <fstream>
#include <iostream>

using namespace MOFA;


//void inputtest(int argc, char** argv) {
//    std::string str;
//    while(1) {
//        std::cin >> str;
//        puts("---------------------------");
//    }
//}


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
    while(std::getline(ifs, line)) {
        sourcecode.add(line, sourcecode.length() + 1);
    }

    Tokenizer tokenizer(sourcecode);
    tokenizer.tokenize();
    auto& allTokens = tokenizer.getAllTokens();
    for(auto linenum = 0u; linenum < allTokens.length(); ++linenum) {
        auto& tokline = allTokens[linenum];
        for(auto toknum = 0u; toknum < tokline.length(); ++toknum) {
            auto& tok = tokline[toknum];
            std::cout << tok.token;
            printf("|>>>>>>>| %s\n", to_string(tok.type));
        }
        puts("---------------------------");
    }

    return 0;
}
