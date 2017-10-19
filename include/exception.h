#pragma once

class mapNotValidException: public std::exception
{
private:
    std::string fileName;
public:
    mapNotValidException();
    mapNotValidException(std::string);
    virtual const char* what() const throw();
};
