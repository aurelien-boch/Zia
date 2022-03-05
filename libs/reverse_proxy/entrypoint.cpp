#include "ReverseProxy.hpp"

#ifdef _WIN32
    #define DllExport   __declspec( dllexport )
#else
    #define DllExport
#endif

extern "C"
{
    DllExport ziapi::IModule *LoadZiaModule()
    {
        return new modules::ReverseProxy();
    }
}
