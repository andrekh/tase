#ifndef GEOLOCATER_H_
#define GEOLOCATER_H_

#include "utils.h"
#include <netdb.h>

#define HTTP_RESPONSE_BUF_SIZE 4096

class GeoLocater
    {
    public:
        virtual bool LookupIP(const std::string& IpAddress, SGeoInfo* const pSGeoInfo) = 0;
        ~GeoLocater();

    protected:
        GeoLocater();
        virtual void FillResultStruct() = 0;
        void CreateSocket();
        void ConnectToHost();
        void SendRequest(const std::string& Request);
        bool ParseResponse(std::string* const pJsonString);
        
        static int DetectEndOfDoubleCLRF(const std::string& HTTPResponse);
        static int GetContentLength(const std::string&HTTPResponse);

    protected:
        addrinfo m_Hints {};
        addrinfo * m_pResults = nullptr;
        int m_SocketFd {};
    };

#endif