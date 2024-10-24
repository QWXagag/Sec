#include "BaseShm.h"
#include <iostream>



int BaseShm::_shmget(int key, int size, int flag)
{
    this->m_shmID = shmget(key, size, flag);
    if (this->m_shmID == -1)
    {
        std::cout << "shmget func err" << std::endl;
    }
    return this->m_shmID;
    
}

// // 打开
BaseShm::BaseShm(int key)
{
    this->_shmget(key, 0, 0);
}
// // 创建
BaseShm::BaseShm(int key, int size)
{
    this->_shmget(key, size, IPC_CREAT | 0664);
}
// // 打开
BaseShm::BaseShm(std::string path_name)
{
    key_t k = ftok(path_name.data(), 'x');
    this->_shmget(k, 0, 0);
}
// // 创建
BaseShm::BaseShm(std::string path_name, int size)
{
    key_t k = ftok(path_name.data(), 'x');
    this->_shmget(k, size, IPC_CREAT | 0664);
}
// // 创建联系
void* BaseShm::shmMap()
{
    this->m_shmAddr = shmat(this->m_shmID, NULL, 0);
    if (this->m_shmAddr == (void*) -1)
    {
        return NULL;
    }
    return this->m_shmAddr;
}
// // 解除联系
int BaseShm::shmUnMap()
{
    int ret = shmdt(this->m_shmAddr);
    return ret;
}
// // 标记删除
int BaseShm::shmdel()
{
    int ret = shmctl(this->m_shmID, IPC_RMID, NULL);
    return ret;
}