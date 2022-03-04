#include <Loader.hpp>
#include <ziapi/Module.hpp>

#include "StaticServe.hpp"

extern "C"
{
    DllExport ziapi::IModule *LoadZiaModule()
    {
        return new modules::StaticServe();
    }
}
