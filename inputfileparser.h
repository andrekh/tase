#include "utils.h"
#include <fstream>
#include <vector>
#include <string>

//====================================================
inline void ParseServersFile(
    const char* PathToFile,
    EProtocol* const pEProtocol,
    std::vector<std::string>* const pIpAddresses)
    {
    std::ifstream ServersFile(PathToFile);
    std::string Line;

    std::getline(ServersFile, Line);
    if(Line == "valve")
        {
        *pEProtocol = EProtocol::eValve;
        }
    else if(Line == "gamespyv1")
        {
        *pEProtocol = EProtocol::eGameSpyV1;
        }

    while(std::getline(ServersFile, Line))
        {
        if(IsIPv4AddressValid(StripPortFromIP(Line).c_str()))
            pIpAddresses->push_back(Line);
        }

    ServersFile.close();
    return;
    }