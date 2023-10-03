#include "mohaahandler.h"


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

    int a = 0;
    }

//===============================================================================
std::string MohaaHandler::GetQueryPort(const std::string &Port)
    {
    return std::to_string(std::stoi(Port) + 97);
    }