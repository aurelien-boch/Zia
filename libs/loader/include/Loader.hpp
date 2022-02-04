#ifndef LOADER_HPP
#define LOADER_HPP

#include <string>
#include <functional>
#include <stdexcept>
#include <iostream>

#ifdef _WIN32
    #include "Windows.h"
    #define DllExport __declspec(dllexport)
#else
    #include <dlfcn.h>
    #define DllExport
#endif

namespace loader
{
    class Loader
    {
        public:

            /**
            * @param path[in] This parameter refers to the path of the library to load.
            */
#ifdef _WIN32
            explicit Loader(std::string const &path) :
                _handle(LoadLibrary(TEXT(path.c_str())))
#else
            explicit Loader(std::string const &path, int flags = RTLD_LAZY) :
                _handle(dlopen(path.c_str(), RTLD_LAZY))
#endif
            {
                if (_handle == nullptr)
                    throw std::runtime_error("Library not found: " + path);
            }

            ~Loader()
            {
                if (_handle != nullptr) {
#ifdef _WIN32
                    FreeLibrary(_handle);
#else
                    dlclose(_handle);
#endif
                }

            }

            Loader &operator=(Loader &other) = delete;

            Loader(const Loader &other) = delete;

            Loader(Loader &&other) noexcept :
                _handle(other._handle)
            {
                other._handle = nullptr;
            }

            /**
             * @brief This method loads a symbol from the shared library
             * @tparam T The prototype of the symbol to load
             * @param name[in] The name of the symbol to load
             * @return A std::function containing the symbol.
             */
            template <typename T>
            std::function<T> getSymbol(std::string const &name)
            {
#ifdef _WIN32
                auto fun = reinterpret_cast<T *>(GetProcAddress(_handle, name.c_str()));
#else
                auto fun = reinterpret_cast<T *>(dlsym(_handle, name.c_str()));
#endif

                if (fun == nullptr)
                    throw std::runtime_error("Symbol not found: " + name);
                return (std::function<T>(fun));
            }

        private:
#ifdef _WIN32
            HINSTANCE _handle;
#else
            void *_handle;
#endif
    };
}

#endif //LOADER_HPP
