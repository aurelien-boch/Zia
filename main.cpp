#include <iostream>

#include "ConfigParser.hpp"

int main()
{
    configParser::ConfigParser parser("config.yml");

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
