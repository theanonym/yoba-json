#ifndef JERROR_HPP
#define JERROR_HPP

#include <exception>
#include <string>

namespace json {

class Error : public std::exception
{
public:
   Error(const std::string & what);

   const char * what() const noexcept(true);

private:
   std::string _what;
};

class TypeError : public Error
{
public:
   TypeError(int expected, int actual);
};

} // ns json

#endif // JERROR_HPP
