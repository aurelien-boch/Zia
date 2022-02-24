#include <Loader.hpp>

#include "Cgi.hpp"

extern "C"
{
    DllExport ziapi::IModule *LoadZiaModule()
    {
        return new modules::Cgi();
    }
}
