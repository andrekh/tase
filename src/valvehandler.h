#ifndef VALVEHANDLER_H_
#define VALVEHANDLER_H_

#define MAGIC_NUMBER "\xFF\xFF\xFF\xFF"
#define A2S_INFO_HEADER "\x54\x53\x6F\x75\x72\x63\x65\x20\x45\x6E\x67\x69\x6E\x65\x20\x51\x75\x65\x72\x79"

#include "serverhandler.h"
#include <string>

class ValveHandler : public ServerHandler
    {
    public:
        virtual void AskServerForInfo(const std::string& IpAddressAndPort, json* const pJ) override;
        virtual void ToJson(json* const pJ) const override;

    private:
        void ParseA2SInfoResponse();
        void RespondToChallenge();
        void SendA2SInfo();
        virtual void Reset() override;


    private:
        std::string m_Folder {};
        uint16_t m_GameID {};

    };

#endif