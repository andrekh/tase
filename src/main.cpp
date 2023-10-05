#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <tuple>
#include <thread>
#include <mutex>
#include "inputfileparser.h"
#include "serverhandlerfactory.h"
#include "geodatabase.h"
#include "ipapi.h"
#include "geoinfo.h"
#include "json.hpp"

using json = nlohmann::json;

// Function prototypes
void Process(const std::string& IpAddressAndPort, std::unique_ptr<ServerHandler> Sh);
void GetGeoLocation(const std::string& IpAddress, SGeoInfo* const pSGeoInfo, GeoLocater* const pGeoLocater);

// Globals
std::mutex GeoMutex;
std::mutex JsonMutex;
json OutputJson;
GeoDatabase GeoDB;


//===============================================================================
/* 
   Main function for the application.
   - Parses the input file to obtain server addresses and protocol type.
   - Creates worker threads to process server information concurrently.
   - Writes the final output JSON data to a file.
*/
//===============================================================================
int main(int argc, char* argv[])
    {
    char InputFilePath[] = "/home/smallguy/PORTSCAN2/serverlists/mohaa.txt";

    std::vector<std::string> Servers;
    EProtocol eProtocol;
    ParseServersFile(InputFilePath, &eProtocol, &Servers);

    std::vector<std::thread> Threads {};
    ServerHandlerFactory ServerHandlerFac;

    for(const auto& Server : Servers)
        {
        auto Sh = ServerHandlerFac.CreateServerHandler(eProtocol);
        Threads.emplace_back(Process, Server, std::move(Sh));
        }
    
    for(auto& Thread : Threads)
        {
        Thread.join();
        }

    std::ofstream out("/home/smallguy/PORTSCAN2/output/output.json");
    out << std::setw(4) << OutputJson;
    out.close();
    return 0;
    }


//===============================================================================
/* 
   Thread function to process server information:

   @param IpAddressAndPort  IP address and port of the server to process "xxx.xxx.xxx.xxx:port"
   @param Sh                Unique ptr to the ServerHandler responsible for handling the server.
   
   This function performs the following tasks:
   - IP address geolocation lookup.
   - Retrieval of server information using the ServerHandler.
   - Updates the global JSON data (OutputJson) with the server information.

   @note Can be called concurrently by multiple threads to process different servers.
*/
//===============================================================================
void Process(
    const std::string& IpAddressAndPort,
    std::unique_ptr<ServerHandler> Sh)
    {
    SGeoInfo GeoInfo {};
    IpApi IPApi;

    std::string IPAddress = StripPortFromIP(IpAddressAndPort);
    GetGeoLocation(IPAddress, &GeoInfo, &IPApi);
    Sh->AskServerForInfo(IpAddressAndPort, nullptr);
    
    std::lock_guard<std::mutex> Lock(JsonMutex);
    Sh->ToJson(&OutputJson);
    if(Sh->QuerySuccess())
        {
        GeoInfo.ToJson(Sh->GetServerIP(), &OutputJson);
        }
    return;
    }

//===============================================================================
/* 
   GetGeoLocation function for IP address geolocation lookup:
   - Looks up the geolocation information for the given IP address.
   - Updates the provided SGeoInfo structure with the geolocation data.
   - If not found in the GeoDB, attempts a lookup using the provided GeoLocater.
   - If successful, updates the GeoDB in a thread-safe manner.

   @param IpAddress       The IP address to perform the geolocation lookup for.
   @param pSGeoInfo       A pointer to the SGeoInfo structure to store the geolocation data.
   @param pGeoLocater     A pointer to the GeoLocater used for geolocation lookups.

   @note If pSGeoInfo or pGeoLocater is nullptr, this function will return without performing any action.
*/
//===============================================================================
void GetGeoLocation(
    const std::string& IpAddress,
    SGeoInfo* const pSGeoInfo,
    GeoLocater* const pGeoLocater)
    {
    if(!pSGeoInfo || !pGeoLocater) { return; }
    if(!GeoDB.LookupByIP(IpAddress, pSGeoInfo))
        {
        if(pGeoLocater->LookupIP(IpAddress, pSGeoInfo))
            {
            std::lock_guard<std::mutex> Lock(GeoMutex);
            GeoDB.UpsertEntry(*pSGeoInfo);
            }
        else
            {
            // fill with default values?
            }
        }
    }