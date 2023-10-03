#ifndef EXCEPT_H_
#define EXCEPT_H_

#include <stdexcept>
#include <string>

class ServerExWithErrno;
class ExceptionWithErrNo;

class Exception : public std::runtime_error
    {
    public:
        Exception(const std::string& ExceptionMessage) : std::runtime_error(ExceptionMessage) {}
    protected:
        virtual void WriteToLog() {}
    };


class ServerExWithErrno : public Exception
    {
    public:
        ServerExWithErrno(
            const std::string& IP,
            const std::string& Port,
            int Error,
            const std::string& Message) : Exception(Message)
        {
        WriteToLog();
        }
    };
    

class ExceptionWithErrNo : public Exception
    {
    public:
        ExceptionWithErrNo(const std::string& ExceptionMessage, int Error) : Exception(ExceptionMessage)
        {
        WriteToLog();
        }
    };

#endif