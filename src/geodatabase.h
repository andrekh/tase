#ifndef GEODATABASE_H_
#define GEODATABASE_H_

#include <string>
#include <memory>
#include <chrono>
#include <mutex>
#include "geoinfo.h"
#include <SQLiteCpp/SQLiteCpp.h>
#include "utils.h"

#define GEODB_SQL_FILENAME "/home/smallguy/PORTSCAN2/output/geo.db"

typedef std::chrono::system_clock Time;
typedef std::chrono::seconds Seconds;

class GeoDatabase
    {
    public:
        GeoDatabase();
        bool LookupByIP(const std::string& IpAddress, SGeoInfo* const pSGeoInfo) const;
        void UpsertEntry(const SGeoInfo& GeoInfo) const;

    private:
        bool LoadDatabase();

    private:
        std::unique_ptr<SQLite::Database> m_pGeoSQLDB;
    };

#endif