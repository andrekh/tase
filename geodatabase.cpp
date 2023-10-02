#include "geodatabase.h"
#include <filesystem>

//====================================================
GeoDatabase::GeoDatabase()
    {
    LoadDatabase();
    }

//====================================================
bool GeoDatabase::LoadDatabase()
    {
    m_pGeoSQLDB = std::make_unique<SQLite::Database>(GEODB_SQL_FILENAME, SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE);
    const std::string query = R"(
    CREATE TABLE IF NOT EXISTS geolocation (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    ip_address TEXT NOT NULL UNIQUE,
    country TEXT,
    city TEXT,
    continent_name TEXT,
    country_code TEXT,
    last_updated INTEGER)
    )";

    m_pGeoSQLDB->exec(query);

    return true;
    }

//====================================================
void GeoDatabase::UpsertEntry(const SGeoInfo& GeoInfo)
    {
    if(!IsIPv4AddressValid(GeoInfo.m_IP.c_str()))
        {
        return;
        }

    const std::string UpsertQuery = R"(INSERT OR REPLACE INTO geolocation (ip_address, country, city, continent_name, country_code, last_updated)
                                VALUES (:ip, :country, :city, :continent, :country_code, :last_updated))";
    
    SQLite::Statement query(*m_pGeoSQLDB, UpsertQuery);

    const auto T = Time::now().time_since_epoch();
    const int UnixTime = std::chrono::duration_cast<Seconds>(T).count();

    query.bind(":ip", GeoInfo.m_IP);
    query.bind(":country", GeoInfo.m_Country);
    query.bind(":city", GeoInfo.m_City);
    query.bind(":continent", GeoInfo.m_ContinentName);
    query.bind(":country_code", GeoInfo.m_CountryCode);
    query.bind(":last_updated", UnixTime);

    query.exec();
    }

//====================================================
bool GeoDatabase::LookupByIP(const std::string& IpAddress, SGeoInfo* const pSGeoInfo) const
    {
    const std::string LookupQuery = R"(SELECT * FROM geolocation WHERE ip_address = :ip_address)";
    SQLite::Statement query(*m_pGeoSQLDB, LookupQuery);
    query.bind(":ip_address", IpAddress);
    if(!query.executeStep())
        {
        return false;
        }
    if(pSGeoInfo != nullptr)
        {
        pSGeoInfo->m_IP = IpAddress;
        pSGeoInfo->m_Country = std::string(query.getColumn("country"));
        pSGeoInfo->m_City = std::string(query.getColumn("city"));
        pSGeoInfo->m_ContinentName = std::string(query.getColumn("continent_name"));
        pSGeoInfo->m_CountryCode = std::string(query.getColumn("country_code"));
        }
    return true;
    }        