#include <exception>
#include <string>
#include "../include/exception.h"

mapNotValidException::mapNotValidException() {
    fileName = "";
}
mapNotValidException::mapNotValidException(std::string fn) : fileName(fn)
{ };
const char* mapNotValidException::what() const throw()
{
    std::string s = "The current map inside of file " + fileName + "is not valid";
    return s.c_str();
}