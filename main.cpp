#include <iostream>
#include "ConfigParser.hpp"

#include "Core.hpp"

int main()
{
    std::string zizi{"../../config.yml"};
    core::Core zazou{zizi};

    zazou.config();
    zazou.run();
    return 0;
}
