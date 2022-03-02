#include "Core.hpp"

int main()
{
    core::Core pipeline{"../../config.yml"};

    pipeline.config();
    pipeline.run();
    return 0;
}
