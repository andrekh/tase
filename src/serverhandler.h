#ifndef SERVERHANDLER_H_
#define SERVERHANDLER_H_

#include <string>
#include <memory>
#include <netdb.h>

#define UDP_SERVER_RESPONSE_BUF_SIZE 2000

class ServerHandler
    {

    public:
        ServerHandler(const std::string& ServerIP);
        void InitializeConnection();
        virtual void AskServerForInfo() = 0;
        virtual ~ServerHandler();
        std::string GetServerName() const { return m_ServerName; }
        std::string GetCurrentMap() const { return m_Map; }
        std::string GetServerIP() const { return m_Host; }
        uint8_t GetPlayersCount() const { return m_PlayersCount; }
        uint8_t GetMaxPlayers() const { return m_MaxPlayers; }
        bool QuerySuccess() const { return m_QuerySuccess; }

    protected:
        int ReadFromSocket();
        virtual std::string GetQueryPort(const std::string &Port);

    protected:
        bool m_QuerySuccess {false};
        char m_ServerResponse[UDP_SERVER_RESPONSE_BUF_SIZE] = {'\0'};
        int m_SocketFd {};
        addrinfo * m_pResults = nullptr;
        std::string m_Host {};
        std::string m_ServerName {};
        std::string m_Map {};
        std::string m_GameName {};
        uint8_t m_PlayersCount {};
        uint8_t m_MaxPlayers {};
        addrinfo m_Hints {};
    };

#endif