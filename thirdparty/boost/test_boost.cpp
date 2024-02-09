#include <iostream>

#include <boost/program_options.hpp>

int main(int argc, char **argv) {
    // Parse the options string and stack them into variables map.
    namespace options = boost::program_options;
    options::options_description desc("Allowed options");
    desc.add_options()("help", "produce help message")("test", options::value<bool>(), "make test");

    options::variables_map vm;
    options::store(options::parse_command_line(argc, argv, desc), vm);
    options::notify(vm);
    std::cout << "Arguments number: " << vm.size() << std::endl;
    if (vm.count("test") > 0) {
        std::cout << "Test option set to " << vm["test"].as<bool>() << std::endl;
    }
    return 0;
}