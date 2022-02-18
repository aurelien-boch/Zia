#ifndef VARIABLEALREADYFOUNDEXCEPTION_HPP
#define VARIABLEALREADYFOUNDEXCEPTION_HPP

#include <exception>
#include <string>

namespace env
{
    /**
     * @brief An exception that will be thrown if you are trying to create a variable that is already defined.
     */
    class VariableAlreadyDefinedException : public std::exception
    {
        public:
            explicit VariableAlreadyDefinedException(std::string const &name) :
                _msg {"Variable " + name + " is already defined in environment Manager."}
            {}

            explicit VariableAlreadyDefinedException(char const *name) :
                _msg {"Variable " + std::string(name) + " is already defined in environment Manager."}
            {}

            [[nodiscard]] const char *what() const override
            {
                return _msg.c_str();
            }

        private:
            std::string _msg;
    };
}

#endif //VARIABLEALREADYFOUNDEXCEPTION_HPP
