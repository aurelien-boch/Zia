#include <iostream>
#include "ConfigParser.hpp"

#include "Core.hpp"

int main()
{
    std::string zizi{"/"};
    core::Core zazou{zizi};

    zazou.run();
    return 0;
}
