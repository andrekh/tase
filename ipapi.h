#ifndef IPAPISERVICE_H_
#define IPAPISERVICE_H_

#include "geolocater.h"

class IpApi : public GeoLocater
    {
    public:
        IpApi();
        virtual bool LookupIP(const std::string& IpAddress, SGeoInfo* const pSGeoInfo) override;

    private:
        virtual void FillResultStruct() override;

    };

#endif