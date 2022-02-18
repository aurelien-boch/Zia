#ifndef VARIABLENOTFOUNDEXCEPTION_HPP
#define VARIABLENOTFOUNDEXCEPTION_HPP

#include <exception>
#include <string>

namespace env
{
    /**
     * @brief An exception that will be thrown if a required variable is not found.
     */
    class VariableNotDefinedException : public std::exception
    {
        public:
            explicit VariableNotDefinedException(std::string const &name) :
                _msg {"Variable " + name + " not found in environment Manager."}
            {}

            explicit VariableNotDefinedException(char const *name) :
                _msg {"Variable" + std::string(name) + " not found in environment Manager."}
            {}

            [[nodiscard]] const char *what() const noexcept override
            {
                return _msg.c_str();
            }

        private:
            std::string _msg;
    };

}

#endif //VARIABLENOTFOUNDEXCEPTION_HPP
