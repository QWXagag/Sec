#pragma once
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <map>
#include "TcpServer.h"
#include "RequestCodecFactory.h"
#include "ResponseCodecFactory.h"
#include "MyRsa.h"
#include "Hash.h"
using namespace Json;
using namespace std;


enum KeyLen
{
    Len16 = 16,
    Len24 = 24,
    Len32 = 32
};

class ServerOP
{
public:
    ServerOP();
    ~ServerOP();
    void startServer();
    static void* workling(void* arg);
    // 密钥协商
    string seckeyConsult(RequestMsg* msg);
private:
    string generateRandomStr(KeyLen num);
private:
    TcpServer* m_server;
    unsigned short m_port;
    string m_serverID;
    map<pthread_t, TcpSocket*> m_list;

};