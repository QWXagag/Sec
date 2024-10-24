#pragma once
#include <string>
#include <json/json.h>
#include <sstream>
#include "RequestCodecFactory.h"
#include "ResponseCodecFactory.h"
#include "MyRsa.h"
#include "Hash.h"
#include "TcpSocket.h"
using namespace std;
using namespace Json;



struct ClientInfo
{
    string clientID;
    string serverID;
    string ip;
    unsigned short port;
};


class ClientOP
{
public:
    ClientOP(string file_name);
    ~ClientOP();
    // 密钥协商
    bool seckeyConsult();
    void seckeyCheck();
    void seckeyDestruction();
private:
    ClientInfo m_info;
};