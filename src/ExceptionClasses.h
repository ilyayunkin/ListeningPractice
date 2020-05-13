#ifndef EXCEPTIONCLASSES_H
#define EXCEPTIONCLASSES_H
#include <stdexcept>
#include <QByteArray>

class HttpLoadException : public std::runtime_error
{
public:
    HttpLoadException(const QByteArray &error) :
        std::runtime_error(QByteArray("Unable to load file: ").append(error).data())
    {
    }
};
class EmptyFileException : public std::runtime_error
{
public:
    EmptyFileException() :
        std::runtime_error("Downloaded file is empty")
    {
    }
};
class ParsingFailedException : public std::runtime_error
{
public:
    ParsingFailedException(const char *const element) :
        std::runtime_error(QByteArray("Parsing failed: ").append(element).data())
    {
    }
};

#endif // EXCEPTIONCLASSES_H
