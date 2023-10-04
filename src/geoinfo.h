#ifndef GEOINFO_H_
#define GEOINFO_H_

#include <string>
#include "json.hpp"

using json = nlohmann::json;

class SGeoInfo
    {
    public:
        void ToJson(const std::string& ServerIpAndPort, json* const pJ) const;
    public:
        std::string m_IP {};
        std::string m_Country {};
        std::string m_City {};
        std::string m_ContinentName {};
        std::string m_CountryCode {};
    };

#endif