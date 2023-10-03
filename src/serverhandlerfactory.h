#include <memory>
#include <string>
#include "valvehandler.h"
#include "mohaahandler.h"
#include "utils.h"
#include "except.h"

//====================================================
class ServerHandlerFactory
    {
    public:
        std::unique_ptr<ServerHandler> CreateServerHandler(EProtocol Protocol);
    };
    

//====================================================
std::unique_ptr<ServerHandler> ServerHandlerFactory::CreateServerHandler(EProtocol Protocol)
    {
    switch(Protocol)
        {
        case EProtocol::eGameSpyV1:
            return std::make_unique<MohaaHandler>();
        case EProtocol::eValve:
            return std::make_unique<ValveHandler>();
        default:
            throw Exception("server handler not implemented for protocol.");
        }
    }