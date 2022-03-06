#include <ziapi/Module.hpp>

#include "Vhosts.hpp"

#ifndef DllExport
    #ifdef _WIN32
        #define DllExport __declspec(dllexport)
    #else
        #define DllExport
    #endif
#endif

extern "C"
{
    DllExport ziapi::IModule *LoadZiaModule()
    {
        return new modules::Vhosts();
    }
}
