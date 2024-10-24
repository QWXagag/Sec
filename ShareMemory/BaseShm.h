#pragma once
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string>


class BaseShm
{
public:
    // 打开
    BaseShm(int key);
    // 创建
    BaseShm(int key, int size);
    // 打开
    BaseShm(std::string path_name);
    // 创建
    BaseShm(std::string path_name, int size);
    // 创建联系
    void* shmMap();
    // 解除联系
    int shmUnMap();
    // 标记删除
    int shmdel();

private:
    int _shmget(int key, int size, int flag);

private:
    int m_shmID;
protected:
    void* m_shmAddr = NULL;
};