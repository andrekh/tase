#include "valvehandler.h"

//=====================================================================
void ValveHandler::Reset()
    {
    ServerHandler::Reset();
    m_Folder.clear();
    m_GameID = 0;
    }

//===============================================================================
void ValveHandler::AskServerForInfo(const std::string& IpAddressAndPort, json* const pJ)
    {
    Reset();
    m_Host = IpAddressAndPort;
    InitializeConnection();
    SendA2SInfo();
    if(m_QuerySuccess)
        {
        ParseA2SInfoResponse();
        }
    if(pJ)
        {
        ToJson(pJ);
        }
    }

//===============================================================================
void ValveHandler::SendA2SInfo()
    {
    std::string Payload = std::string(MAGIC_NUMBER) + A2S_INFO_HEADER + '\0';

    if((sendto(m_SocketFd, Payload.c_str(), Payload.length(), 0, m_pResults->ai_addr, m_pResults->ai_addrlen)) == -1)
        {
        //error
        }

    // receive response
    if(ReadFromSocket() < 0)
        {
        m_QuerySuccess = false;
        return;
        }
    else
        {
        RespondToChallenge();
        }
    }

//===============================================================================
void ValveHandler::RespondToChallenge()
    {
    const int ResponseLengthWhenChallenge = 9;
    const int ChallengeOffset = 4;
    std::string Response(m_ServerResponse);
    const int ResponseLength = Response.size();
    if(Response[ResponseLength - 5] == '\x41' && ResponseLength == ResponseLengthWhenChallenge)
        {
        std::fill_n(m_ServerResponse, UDP_SERVER_RESPONSE_BUF_SIZE, '\0');
        std::string Payload = std::string(MAGIC_NUMBER) + A2S_INFO_HEADER + '\0' + Response.substr(ResponseLength -  ChallengeOffset);
        sendto(m_SocketFd, Payload.c_str(), Payload.length(), 0, m_pResults->ai_addr, m_pResults->ai_addrlen);

        if(ReadFromSocket() < 0)
            {
            m_QuerySuccess = false;
            }
        }
    }

//===============================================================================
void ValveHandler::ParseA2SInfoResponse()
    {
    char * pChar = m_ServerResponse;
    pChar += 4; // points to I
    pChar += 1; // points to protocol version
    pChar += 1; // points to beginning of server name

    m_ServerName = std::string(pChar);
    pChar += m_ServerName.size() + 1;
    m_Map = std::string(pChar);
    pChar += m_Map.size() + 1;
    m_Folder = std::string(pChar);
    pChar += m_Folder.size() + 1;
    m_GameName = std::string(pChar);
    pChar += m_GameName.size() + 1;
    m_GameID = (((short)*(pChar + 1)) << 8) | (0x00FF & *pChar);
    pChar += 2;
    m_PlayersCount = (uint8_t) *pChar;
    pChar += 1;
    m_MaxPlayers = (uint8_t) *pChar;
    m_QuerySuccess = true;
    }


//===============================================================================
void ValveHandler::ToJson(json* const pJ) const
    {
        if(!m_QuerySuccess)
            {
            (*pJ)[GetServerIP()].update({{"status", "false"}});
            return;
            }

        (*pJ)[GetServerIP()].update(
            {
                {"status", "true"},
                {"name", GetServerName()},
                {"map", GetCurrentMap()},
                {"players", GetPlayersCount()},
                {"max players", GetMaxPlayers()},
            });
    }