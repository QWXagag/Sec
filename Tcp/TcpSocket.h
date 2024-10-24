#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>


static const int TIMEOUT = 10000;

// 用于通信的套接字类
class TcpSocket
{
private:
    int m_Socket;
public:
    TcpSocket();
    // 有参构造、传递一个文件描述符用于通信
    TcpSocket(int fd);
    // 连接服务器
    int connectToHost(std::string ip, unsigned int port, int timeout = TIMEOUT);
    // 发送数据
    int sendMsg(std::string sendData, int timeout = TIMEOUT);
    // 接收数据
    std::string recvMsg(int timeout = TIMEOUT);
    // 断开链接
    void disConnect();

private:
    // 设置I/O非阻塞
    void setNonBlock(int fd);
    // 设置I/O阻塞
    void setBlock(int fd);
    // 读超时检测函数
    int readTimeout(unsigned int wait_seconds);
    // 写超时检测函数
    int writeTimeout(unsigned int wait_seconds);
    // 连接超时检测函数
    int connectTimeout(struct sockaddr_in* addr, unsigned int wait_seconds);
    // 每次从缓冲区读取n个字符
    int readn(void* buf, int count);
    // 每次从缓冲区写入n个字符
    int writen(const void* buf, int count);


};