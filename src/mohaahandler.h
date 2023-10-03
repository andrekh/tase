#ifndef MOHAAHANDLER_H_
#define MOHAAHANDLER_H_

#include "serverhandler.h"
#include <string>

class MohaaHandler : public ServerHandler
    {
    public:
        MohaaHandler(const std::string& ServerIP);
        virtual void AskServerForInfo() override;

    protected:
        virtual std::string GetQueryPort(const std::string &Port) override;
    };

#endif