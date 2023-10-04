#ifndef UTILS_H_
#define UTILS_H_

#include <arpa/inet.h>
#include <string>
#include <vector>

//====================================================
enum class EProtocol
    {
    eValve,
    eGameSpyV1,
    };
    
//====================================================
inline bool IsIPv4AddressValid(const char* IpAddress)
    {
    sockaddr_in SA {};
    return inet_pton(AF_INET, IpAddress, &(SA.sin_addr)) == 1;
    }

//====================================================
inline std::string StripPortFromIP(const std::string& IpAndPort)
    {
    size_t ColonPos = IpAndPort.find(":");
    if(ColonPos != std::string::npos)
        {
        return IpAndPort.substr(0, ColonPos);
        }
    return IpAndPort;
    }

//====================================================
inline std::string StripIPFromIpAndPort(const std::string& IpAndPort)
    {
    size_t ColonPos = IpAndPort.find(":");
    if(ColonPos != std::string::npos)
        {
        return IpAndPort.substr(ColonPos+1);
        }
    return IpAndPort;
    }


#endif