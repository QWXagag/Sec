#pragma once
#include "BaseShm.h"
#include <string.h>


class NodeShmInfo
{
public:
    int status;
    int seckeyID;
    char clientID[12];
    char serverID[12];
    char seckey[128];
    NodeShmInfo() : status(0), seckeyID(0)
    {
        bzero(clientID, sizeof(clientID));
        bzero(serverID, sizeof(serverID));
        bzero(seckey, sizeof(seckey));
    }
};


class SecKeyShm : public BaseShm
{
public:
    // 创建
    SecKeyShm(int key, int size);
    // 创建
    SecKeyShm(std::string path_name, int size);
    // 读取
    NodeShmInfo readShm(std::string client_id, std::string server_id);
    // 写入
    int writeShm(NodeShmInfo* node);
private:
    int m_maxNode;
};