#include <iostream>

#include <boost/program_options.hpp>

#include "config/GlobalEngineConfig.h"
#include "config/ConfigUtils.h"

int main(int argc, char * argv[])
{
  using namespace rcbe;
  namespace po = boost::program_options;
  po::options_description desc("Allowed options");
  desc.add_options()
        ("help,h", "produce help message")
        ("config,c", po::value<std::string>(), "provide configuration file");

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  if (vm.count("help"))
  {
    std::cout << desc << std::endl;
    return 0;
  }

  config::GlobalEngineConfig conf;

  if(vm.count("config"))
  {
    auto config_file_path = vm["config"].as<std::string>();
    try
    {
      config::GlobalEngineConfig gec = config::utils::readFromFile<config::GlobalEngineConfig>(config_file_path);

      std::cout << std::boolalpha << gec.debug << std::endl;
    }
    catch(const std::exception &e)
    {
      std::cerr << "Exception in main(): " << e.what() << std::endl;
    }
  }

  return 0;
}