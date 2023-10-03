#include "geolocater.h"
#include "except.h"
#include "errno.h"
#include "string.h"

//=====================================================================
GeoLocater::GeoLocater()
    {
    m_Hints.ai_family = AF_INET;
    m_Hints.ai_socktype = SOCK_STREAM;
    m_Hints.ai_protocol = IPPROTO_TCP;
    }

//=====================================================================
void GeoLocater::CreateSocket()
    {
    if(!m_pResults)
        {
        throw Exception("GeoLocater results pointer is null");
        }

    m_SocketFd = socket(m_pResults->ai_family, m_pResults->ai_socktype, m_pResults->ai_protocol);

    if(m_SocketFd == -1)
        {
        throw ExceptionWithErrNo("geolocater failed to create socket", errno);
        }
    }

//=====================================================================
void GeoLocater::ConnectToHost()
    {
    if(!m_pResults)
        {
        throw Exception("GeoLocater results pointer is null");
        }

    if(connect(m_SocketFd, m_pResults->ai_addr, m_pResults->ai_addrlen) == -1)
        {
        throw ExceptionWithErrNo("geolocater failed to connect to host", errno);
        }
    }

//=====================================================================
void GeoLocater::SendRequest(const std::string& Request)
    {
    int BytesToSend = Request.size();
    int TotalBytesSent {0};
    const char * R = Request.c_str();
    while(TotalBytesSent < BytesToSend)
        {
        int BytesSent = send(m_SocketFd, R + TotalBytesSent, BytesToSend - TotalBytesSent, 0);
        bool Success = BytesSent != -1;
        TotalBytesSent += BytesSent;
        if(!Success)
            {
            throw ExceptionWithErrNo("geolocater failed to send request", errno);
            }
        }
    }

//=====================================================================
bool GeoLocater::ParseResponse(std::string* const pJsonString)
    {
    if(!pJsonString)
        return false;

    char HttpResponseBuf[HTTP_RESPONSE_BUF_SIZE] = {'\0'};
    std::string Response {};

    int DoubleCLRFEndPosition {-1};
    while(DoubleCLRFEndPosition == -1)
        {
        std::fill_n(HttpResponseBuf, HTTP_RESPONSE_BUF_SIZE, '\0');
        const int BytesReceived = recv(m_SocketFd, HttpResponseBuf, sizeof(HttpResponseBuf) - 1, 0);
        if(BytesReceived == -1 || BytesReceived == 0)
            return false;
        Response.append(HttpResponseBuf);
        DoubleCLRFEndPosition = DetectEndOfDoubleCLRF(Response);
        }

    const int ContentLength = GetContentLength(Response);
    if(ContentLength == -1)
        return false;
    
    pJsonString->append(Response.substr(DoubleCLRFEndPosition));
    while(pJsonString->size() < ContentLength)
        {
        std::fill_n(HttpResponseBuf, HTTP_RESPONSE_BUF_SIZE, '\0');
        const int BytesReceived = recv(m_SocketFd, HttpResponseBuf, sizeof(HttpResponseBuf) - 1, 0);
        if(BytesReceived == -1 || BytesReceived == 0)
            return false;
        pJsonString->append(HttpResponseBuf);
        }

    *pJsonString = pJsonString->substr(0, ContentLength);
    return true;
    }

//=====================================================================
int GeoLocater::DetectEndOfDoubleCLRF(const std::string& HTTPResponse)
    {
    const std::string DoubleCLRF {"\r\n\r\n"};
    int DoubleCLRFPos = HTTPResponse.find(DoubleCLRF);
    if(DoubleCLRFPos == std::string::npos)
        {
        return -1;
        }
    return DoubleCLRFPos + DoubleCLRF.size();
    }

//=====================================================================
int GeoLocater::GetContentLength(const std::string& HTTPResponse)
    {
    const std::string ContentLengthTok {"Content-Length: "};
    int ContentLength {};
    int ContentLengthPos = HTTPResponse.find(ContentLengthTok);
    if(ContentLengthPos == std::string::npos)
        {
        return -1;
        }
    int ContentLengthValueStartPos = ContentLengthPos + ContentLengthTok.size();
    int ContentLengthValueEndPos = HTTPResponse.substr(ContentLengthValueStartPos).find("\r\n");
    std::string ContentLengthValue = HTTPResponse.substr(ContentLengthValueStartPos, ContentLengthValueEndPos);
        try
        {
        ContentLength = std::stoi(ContentLengthValue);
        }
    catch(const std::exception& e)
        {
        ContentLength = -1;
        }
    return ContentLength;
    }

//=====================================================================
GeoLocater::~GeoLocater()
    {
    freeaddrinfo(m_pResults);
    }