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

// Globals
std::mutex Mtx;
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
    char InputFilePath[] = "/home/smallguy/PORTSCAN2/src/csgo.txt";

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
   - IP address geolocation lookup.
   - Server information retrieval.
   - Output JSON data update.
*/
//===============================================================================
void Process(
    const std::string& IpAddressAndPort,
    std::unique_ptr<ServerHandler> Sh)
    {
    SGeoInfo GeoInfo {};
    IpApi IPApi;

    std::string IPAddress = StripPortFromIP(IpAddressAndPort);
    if(!GeoDB.LookupByIP(IPAddress, &GeoInfo))
        {
        if(IPApi.LookupIP(IPAddress, &GeoInfo))
            {
            std::lock_guard<std::mutex> Lock(Mtx);
            GeoDB.UpsertEntry(GeoInfo);
            }
        }
    Sh->AskServerForInfo(IpAddressAndPort, nullptr);
    
    std::lock_guard<std::mutex> Lock(Mtx);
    Sh->ToJson(&OutputJson);
    if(Sh->QuerySuccess())
        {
        GeoInfo.ToJson(Sh->GetServerIP(), &OutputJson);
        }
    return;
    }