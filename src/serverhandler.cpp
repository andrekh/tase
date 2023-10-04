#include "serverhandler.h"
#include "except.h"
#include "errno.h"
#include "utils.h"
#include <netdb.h>

//=====================================================================
void ServerHandler::Reset()
    {
    m_QuerySuccess = true;
    freeaddrinfo(m_pResults);
    m_pResults = nullptr;
    m_Host.clear();
    m_ServerName.clear();
    m_Map.clear();
    m_GameName.clear();
    m_PlayersCount = 0;
    m_MaxPlayers = 0;
    }
//=====================================================================
int ServerHandler::ReadFromSocket()
    {
    std::fill_n(m_ServerResponse, UDP_SERVER_RESPONSE_BUF_SIZE, '\0');
    return recvfrom(m_SocketFd, m_ServerResponse, sizeof(m_ServerResponse), 0, m_pResults->ai_addr, &m_pResults->ai_addrlen);
    }

//=====================================================================
void ServerHandler::InitializeConnection()
    {
    m_Hints.ai_family = AF_UNSPEC;
    m_Hints.ai_socktype = SOCK_DGRAM;
    m_Hints.ai_protocol = 0;
    m_Hints.ai_flags = AI_ADDRCONFIG;

    const std::string IPAddress = StripPortFromIP(m_Host);
    const std::string Port = GetQueryPort(StripIPFromIpAndPort(m_Host));
    int Error = getaddrinfo(IPAddress.c_str(), Port.c_str(), &m_Hints, &m_pResults);
    if(Error != 0 || !m_pResults)
        {
        throw ServerExWithErrno(IPAddress, Port, Error, "serverhandler failed to getaddrinfo");
        }

    m_SocketFd = socket(m_pResults->ai_family, m_pResults->ai_socktype, m_pResults->ai_protocol);
    if(m_SocketFd == -1)
        {
        throw ExceptionWithErrNo("serverhandler failed to create socet", errno);
        }

    timeval Timeout {};
    Timeout.tv_sec = 5;
    setsockopt(m_SocketFd, SOL_SOCKET, SO_RCVTIMEO, &Timeout, sizeof(Timeout));
    }

//=====================================================================
std::string ServerHandler::GetQueryPort(const std::string &Port)
    {
    return Port;
    }
//=====================================================================
ServerHandler::~ServerHandler()
    {
    freeaddrinfo(m_pResults);
    }