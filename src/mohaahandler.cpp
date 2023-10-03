#include "mohaahandler.h"
#include <string>
#include <map>

//===============================================================================
MohaaHandler::MohaaHandler(const std::string& ServerIP)
: ServerHandler(ServerIP)
    {
    }

//===============================================================================
void MohaaHandler::AskServerForInfo()
    {
    m_QuerySuccess = false;
    const std::string Payload = "\\info\\";

    if((sendto(m_SocketFd, Payload.c_str(), Payload.length(), 0, m_pResults->ai_addr, m_pResults->ai_addrlen)) == -1)
        {
        //error
        }

    if(ReadFromSocket() < 0)
        {
        //error
        }

    ParseInfoResponse();
    }

//===============================================================================
std::string MohaaHandler::GetQueryPort(const std::string &Port)
    {
    return std::to_string(std::stoi(Port) + 97);
    }

//===============================================================================
void MohaaHandler::ParseInfoResponse()
    {
    std::map<std::string, std::string> ResponseMap;
    std::string Response(&m_ServerResponse[1]);
    std::string Key {};
    std::string Value {};

    do {
        std::size_t Pos = Response.find("\\");
        Key = Response.substr(0, Pos);
        Response = Response.substr(Pos + 1);
        Pos = Response.find("\\");
        if(Pos != std::string::npos)
            {
            Value = Response.substr(0, Pos);
            Response = Response.substr(Pos + 1);
            }
        else
            {
            Value = Response;
            Response = {};
            }

        ResponseMap[Key] = Value;
        } while(!Response.empty());

    // at throws exception if key not found
    m_ServerName = ResponseMap.at("hostname");
    m_Map = ResponseMap.at("mapname");
    m_GameType = ResponseMap.at("gametype");
    m_MaxPlayers = (uint8_t) std::stoi(ResponseMap.at("maxplayers"));
    m_PlayersCount =  (uint8_t) std::stoi(ResponseMap.at("numplayers"));
    m_QuerySuccess = true;
    }