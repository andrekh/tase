#include "geoinfo.h"

void SGeoInfo::ToJson(const std::string& ServerIpAndPort, json* const pJ) const
    {
        (*pJ)[ServerIpAndPort].update(
            {
            {"country", m_Country},
            {"country_code", m_CountryCode},
            {"city", m_City},
            {"continent", m_ContinentName}
            }
        );
    }