#include "ipapi.h"
#include "except.h"
#include "json.hpp"

using json = nlohmann::json;

//====================================================
IpApi::IpApi()
    {
    try
        {
        FillResultStruct();
        }
    catch(const Exception& e)
        {
        // do something
        }
    }
//====================================================
bool IpApi::LookupIP(const std::string& IpAddress, SGeoInfo* const pSGeoInfo)
    {
    std::string JsonString {};
    //http://ip-api.com/json/198.16.253.49?fields=1073171
    std::string Request = "GET /json/";
    Request += IpAddress;
    Request += "?fields=1073171 HTTP/1.1\r\nHost: ip-api.com\r\nConnection: close\r\n\r\n";
    try
        {
        CreateSocket();
        ConnectToHost();
        SendRequest(Request);
        if(!ParseResponse(&JsonString))
            return false;
        }
    catch(const Exception& Ex)
        {
        // handle the exception
        return false;
        }
    catch(const std::exception& e)
        {
        // handle
        return false;
        }

    json ResponseJson = json::parse(JsonString);
    if(ResponseJson["status"] != "success")
        return false;

    if(pSGeoInfo)
        {
        pSGeoInfo->m_ContinentName = ResponseJson["continent"];
        pSGeoInfo->m_Country = ResponseJson["country"];
        pSGeoInfo->m_City = ResponseJson["city"];
        pSGeoInfo->m_CountryCode = ResponseJson["countryCode"];
        pSGeoInfo->m_IP = IpAddress;
        }

    return true;    
    }

//====================================================
 void IpApi::FillResultStruct()
    {
    const char BaseUrl[] {"ip-api.com"};
    int Error = getaddrinfo(BaseUrl, "http", &m_Hints, &m_pResults);
    if(Error != 0 || !m_pResults)
        {
        throw ExceptionWithErrNo("ipapi fill restult struct fail", Error);
        }
    }