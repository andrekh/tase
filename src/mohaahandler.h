#ifndef MOHAAHANDLER_H_
#define MOHAAHANDLER_H_

#include "serverhandler.h"
#include <string>
#include "json.hpp"

using json = nlohmann::json;

class MohaaHandler : public ServerHandler
    {
    public:
        MohaaHandler(const std::string& ServerIP);
        virtual void AskServerForInfo() override;
        std::string GetGameType() const { return m_GameType; }

    protected:
        virtual std::string GetQueryPort(const std::string &Port) override;
        void ParseInfoResponse();

    private:
        std::string m_GameType {};
    };

void to_json(json& j, const MohaaHandler& Mh)
    {
        j[Mh.GetServerIP()] =
            {
                {"status", Mh.QuerySuccess()},
                {"name", Mh.GetServerName()},
                {"map", Mh.GetCurrentMap()},
                {"gametype", Mh.GetGameType()},
                {"players", Mh.GetPlayersCount()},
                {"max players", Mh.GetMaxPlayers()},
            };
    }

#endif