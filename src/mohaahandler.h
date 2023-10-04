#ifndef MOHAAHANDLER_H_
#define MOHAAHANDLER_H_

#include "serverhandler.h"
#include <string>
#include "json.hpp"

using json = nlohmann::json;

class MohaaHandler : public ServerHandler
    {
    public:
        virtual void AskServerForInfo(const std::string& IpAddressAndPort, json* const pJ) override;
        virtual void ToJson(json* const pJ) const override;
        std::string GetGameType() const { return m_GameType; }

    protected:
        virtual std::string GetQueryPort(const std::string &Port) override;
        void ParseInfoResponse();

    private:
        virtual void Reset() override;
        
    private:
        std::string m_GameType {};
    };

#endif