#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include "inputfileparser.h"
#include "serverhandlerfactory.h"
#include "geodatabase.h"
#include "ipapi.h"
#include "json.hpp"

using json = nlohmann::json;

int main(int argc, char* argv[])
    {
    char InputFilePath[] = "/home/smallguy/PORTSCAN2/src/mohaa.txt";

    std::vector<std::string> Servers;
    EProtocol eProtocol;
    ParseServersFile(InputFilePath, &eProtocol, &Servers);

    ServerHandlerFactory ServerHandlerFac;
    GeoDatabase GeoDB;
    IpApi IPApi;
    json OutputJson;

    for(const auto& Server : Servers)
        {
        std::string IPAddress = StripPortFromIP(Server);
        SGeoInfo GeoInfo {};
        if(!GeoDB.LookupByIP(IPAddress, &GeoInfo))
            {
            if(IPApi.LookupIP(IPAddress, &GeoInfo))
                {
                GeoDB.UpsertEntry(GeoInfo);
                }
            }

        auto Sh = ServerHandlerFac.CreateServerHandler(Server, eProtocol);
        Sh->InitializeConnection();
        Sh->AskServerForInfo();
        OutputJson[Sh->GetServerIP()] =
            {
                {"status", Sh->QuerySuccess()},
                {"name", Sh->GetServerName()},
                {"map", Sh->GetCurrentMap()},
                {"players", Sh->GetPlayersCount()},
                {"max players", Sh->GetMaxPlayers()},
                {"country", GeoInfo.m_Country},
                {"country_code", GeoInfo.m_CountryCode},
                {"city", GeoInfo.m_City},
                {"continent", GeoInfo.m_ContinentName}
            };
        }

    std::ofstream out("../output/results.json");
    out << std::setw(4) << OutputJson;
    out.close();
    return 0;
    }