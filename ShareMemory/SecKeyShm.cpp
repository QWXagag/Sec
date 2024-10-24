#include "SecKeyShm.h"
#include <iostream>

// 创建
SecKeyShm::SecKeyShm(int key, int size) : 
BaseShm(key, size * sizeof(NodeShmInfo)), m_maxNode(size)
{

}
// 创建
SecKeyShm::SecKeyShm(std::string path_name, int size) : 
BaseShm(path_name, size * sizeof(NodeShmInfo)), m_maxNode(size)
{

}
// 读取
NodeShmInfo SecKeyShm::readShm(std::string client_id, std::string server_id)
{
    NodeShmInfo* beginNode = static_cast<NodeShmInfo*>(shmMap());
    if (beginNode == NULL)
    {
        std::cout << "共享内存关联失败......."<< std::endl;
        return NodeShmInfo();
    }
    NodeShmInfo* curNode = NULL;
    NodeShmInfo temp;
    int i = 0;
    for (i = 0; i < m_maxNode; i++)
    {
        curNode = beginNode + i;
        // 遍历查询整个共享内存
        if (strcmp(client_id.data(), curNode->clientID) == 0 && 
            strcmp(server_id.data(), curNode->serverID) == 0)
            {
                temp = *curNode;
                std::cout << "查找到client_id=" << client_id << std::endl;
                std::cout << "查找到server_id=" << server_id << std::endl;
                std::cout << "-----------------------------"<< std::endl;
                std::cout << "seckey: " << curNode->seckey << std::endl;
                std::cout << "seckeyID: " << curNode->seckeyID << std::endl;
                std::cout << "status: " << curNode->status << std::endl;
                std::cout << "------------------------------"  << std::endl;
                break;
            }
    }
    if (i == this->m_maxNode)
    {
        std::cout << "未找到：client:" << client_id << std::endl;
        std::cout << "未找到：server:" << server_id << std::endl;
    }
    this->shmUnMap();
    return temp;
    
}
// 写入
int SecKeyShm::writeShm(NodeShmInfo* node)
{
    int ret = 0;
    NodeShmInfo* beginNode = static_cast<NodeShmInfo*>(shmMap());
    if (beginNode == NULL)
    {
        return -1;
    }
    NodeShmInfo* curNode = NULL;
    for (int i = 0; i < this->m_maxNode; i++)
    {
        curNode = beginNode + i;
        if (strcmp(node->clientID, curNode->clientID) == 0 &&
            strcmp(node->serverID, curNode->serverID) == 0)
            {
                std::cout << "找到要覆盖的Node节点" << std::endl;
                memcpy(curNode, node, sizeof(NodeShmInfo));
                std::cout << "覆盖旧数据完成" << std::endl;
                std::cout << "打印新数据" << std::endl;
                std::cout << "------------------------------" << std::endl;
                std::cout << "client_id: " << curNode->clientID << std::endl;
                std::cout << "server_id: " << curNode->serverID << std::endl;
                std::cout << "seckey: " << curNode->seckey << std::endl;
                std::cout << "seckeyID: " << curNode->seckeyID << std::endl;
                std::cout << "status: " << curNode->status << std::endl;
                std::cout << "打印完成" << std::endl;
                std::cout << "------------------------------" << std::endl;
                return ret;
            }
    }
    std::cout << "未找到要覆盖的Node节点，开始寻找空节点来存放数据" << std::endl;
    NodeShmInfo tempNode;
    int i = 0;
    for (i = 0; i < this->m_maxNode; i++)
    {
        curNode = beginNode + i;
        if (memcmp(&tempNode, curNode, sizeof(NodeShmInfo)) == 0)
        {
            std::cout << "找到空节点" << std::endl;
            memcpy(curNode, node, sizeof(NodeShmInfo));
            std::cout << "写入新数据完成" << std::endl;
            std::cout << "打印新数据" << std::endl;
            std::cout << "------------------------------" << std::endl;
            std::cout << "client_id: " << curNode->clientID << std::endl;
            std::cout << "server_id: " << curNode->serverID << std::endl;
            std::cout << "seckey: " << curNode->seckey << std::endl;
            std::cout << "seckeyID: " << curNode->seckeyID << std::endl;
            std::cout << "status: " << curNode->status << std::endl;
            std::cout << "打印完成" << std::endl;
            std::cout << "------------------------------" << std::endl;
            break;
        }
    }
    if (i == this->m_maxNode)
    {
        ret = -1;
    }
    this->shmUnMap();
    return ret;
}