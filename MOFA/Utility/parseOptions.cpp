#include "ParseOptions.h"
#include <boost/program_options.hpp>
#include <filesystem>
#include <iostream>

namespace MOFA {
    namespace bpo = boost::program_options;

    std::pair<std::string, std::string> parseOptions(int _argc, char** _argv) {
        bpo::options_description desc("Accepted options");
        desc.add_options()
            ("help,h", "Display help message.")
            ("input,i", bpo::value<std::string>(), "Input file name. Positional option.")
            ("output,o", bpo::value<std::string>(), "Optional output file name. Replace extension by '.hex' unless specified.");

        bpo::positional_options_description pdesc;
        pdesc.add("input", 1);

        bpo::variables_map vm;
        try {
            bpo::store(bpo::command_line_parser(_argc, _argv).options(desc).positional(pdesc).run(), vm);
        }
        catch(...) {
            std::cout << "Error options. Use -h to see usage." << std::endl;
            return std::pair<std::string, std::string>();
        }
        bpo::notify(vm);

        if(vm.count("help")) {
            std::cout << desc << std::endl;
            return std::pair<std::string, std::string>();
        }

        std::string input, output;

        if(vm.count("input"))
            input = vm["input"].as<std::string>();
        else {
            std::cout << "Input file not specified. Use -h to see usage." << std::endl;
            return std::pair<std::string, std::string>();
        }

        if(vm.count("output"))
            output = vm["output"].as<std::string>();
        else {
            std::filesystem::path p(input);
            p.replace_extension(".hex");
            output = p.string();
        }

        return std::pair<std::string, std::string>(input, output);
    }
}
