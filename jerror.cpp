#include "json/jerror.hpp"
#include "json/jobject.hpp"

namespace json {

Error::Error(const std::string & what)
   : _what("JSON: " + what)
{
}

const char * Error::what() const noexcept(true)
{
   return _what.c_str();
}

TypeError::TypeError(int expected, int actual)
   : Error((boost::format("Ожидается `%1%', но есть `%2%'.")
            % Object::TypeMap.at(static_cast<Object::Type>(expected))
            % Object::TypeMap.at(static_cast<Object::Type>(actual))).str())
{
}

} // ns json
