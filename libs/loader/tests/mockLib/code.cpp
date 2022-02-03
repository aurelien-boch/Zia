#include <Loader.hpp>

extern "C" {
    DllExport int symbol()
    {
        return 12345;
    }
}
