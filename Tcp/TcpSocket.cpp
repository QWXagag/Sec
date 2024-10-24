#include "TcpSocket.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include "util.h"



TcpSocket::TcpSocket()
{
    this->m_Socket = socket(AF_INET, SOCK_STREAM, 0);
    ErrorIf(this->m_Socket == -1, "socket err");
}
// 有参构造、传递一个文件描述符用于通信
TcpSocket::TcpSocket(int fd)
{
    this->m_Socket = fd; 
}
// 连接服务器
int TcpSocket::connectToHost(std::string ip, unsigned int port, int timeout)
{
    int ret = 0;
    ErrorIf((port <= 0 || port > 65535), "param port err");
    ErrorIf(timeout < 0, "param timeout err");

    struct sockaddr_in addr;
    memset(&addr, 0x00, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    ErrorIf(inet_pton(AF_INET, ip.c_str(), &addr.sin_addr.s_addr) == -1, "inet_pton err");
    ret = connectTimeout(&addr, timeout);
    // -1：连接失败，超时
    // 0：已连接服务器
    return ret;

}
// // 发送数据
int TcpSocket::sendMsg(std::string sendData, int timeout)
{
    int ret = 0;
    ErrorIf(timeout < 0, "param timeout err");
    ret = writeTimeout(timeout);
    if (ret == 0)
    {
        int writed = 0;
        int datalen = sendData.size() + 4;
        // 在数据头部添加4个字节内容，内容为数据的长度
        unsigned char* netdata = (unsigned char*)malloc(datalen);
        ErrorIf(netdata == NULL, "sendMsg malloc err");
        int netlen = htonl(sendData.size());
        memcpy(netdata, &netlen, 4);
        memcpy(netdata + 4, sendData.data(), sendData.size());
        writed = writen(netdata, datalen);
        if (writed == datalen)
        {
            if (netdata != NULL)
            {
                free(netdata);
                netdata = NULL;
            }
        }
    }
    // 0：发送成功
    // -1：发送失败，超时
    return ret;

}
// // 接收数据
std::string TcpSocket::recvMsg(int timeout)
{
    int ret = 0;
    ErrorIf(timeout < 0, "param timeout err");
    ret = readTimeout(timeout);
    // 未超时
    if (ret == 0)
    {
        // 解析数据长度
        int datalen = 0;
        // 读取数据包头的前四个字节的内容到datalen中
        ret = readn(&datalen, 4);
        if (ret < 4)
        {
            ErrorIf(true, "client send data header format err");
        }
        int n = ntohl(datalen);
        char* tmpbuf = (char*)malloc(n + 1);
        ErrorIf(tmpbuf == NULL, "recvMsg malloc err");
        ret = readn(tmpbuf, n);
        if (ret < n)
        {
            ErrorIf(true, "client send data body format err");
        }
        tmpbuf[n] = '\0';
        std::string data = std::string(tmpbuf);
        free(tmpbuf);
        return data;
    }
    return std::string();

}
// // 断开链接
void TcpSocket::disConnect()
{
    if (this->m_Socket > 0)
    {
        close(this->m_Socket);
    }
}

// // 设置I/O非阻塞
void TcpSocket::setNonBlock(int fd)
{
    int flag = fcntl(fd, F_GETFL);
    ErrorIf(flag == -1, "setNonBlock fcntl F_GETFL err");
    flag |= O_NONBLOCK;
    flag = fcntl(fd, F_SETFL, flag);
    ErrorIf(flag == -1, "setNonBlock fcntl F_SETFL err");
}
// // 设置I/O阻塞
void TcpSocket::setBlock(int fd)
{
    int flag = fcntl(this->m_Socket, F_GETFL);
    ErrorIf(flag == -1, "setBlock fcntl F_GETFL err");
    flag &= ~O_NONBLOCK;
    flag = fcntl(this->m_Socket, F_SETFL, flag);
    ErrorIf(flag == -1, "setBlock fcntl F_SETFL err");
}
// // 读超时检测函数
int TcpSocket::readTimeout(unsigned int wait_seconds)
{
    int ret = 0;
    fd_set fd_sets;
    FD_ZERO(&fd_sets);
    std::cout << "m_Socket: "<< this->m_Socket << std::endl;
    FD_SET(this->m_Socket, &fd_sets);
    struct timeval tv;
    memset(&tv, 0x00, sizeof(tv));
    tv.tv_sec = wait_seconds;
    tv.tv_usec = 0;
    do
    {
        ret = select(this->m_Socket + 1, &fd_sets, NULL, NULL, &tv);
    } while (ret < 0 && errno == EINTR);
    if (ret == -1)
    {
        ErrorIf(true, "readTimeout select err");
    }
    // 未超时
    else if (ret == 1)
    {
        ret = 0;
    }
    // 超时
    else
    {
        ret = -1;
    }
    return ret;
}
// // 写超时检测函数
int TcpSocket::writeTimeout(unsigned int wait_seconds)
{
    int ret = 0;
    if (wait_seconds > 0)
    {
        fd_set fd_sets;
        FD_ZERO(&fd_sets);
        FD_SET(this->m_Socket, &fd_sets);
        struct timeval tv;
        memset(&tv, 0x00, sizeof(tv));
        tv.tv_sec = wait_seconds;
        tv.tv_usec = 0;
        do
        {
            ret = select(this->m_Socket + 1, NULL, &fd_sets, NULL, &tv);
        } while (ret < 0 && errno == EINTR);
        if (ret == -1)
        {
            ErrorIf(true, "writeTimeout select err");
        }
        // 未超时
        else if (ret == 1)
        {
            ret = 0;
        }
        // 超时
        else
        {
            ret = -1;
        }
    }

    return ret;
}
// // 连接超时检测函数
int TcpSocket::connectTimeout(struct sockaddr_in* addr, unsigned int wait_seconds)
{
    // 设置非阻塞
    if (wait_seconds > 0)
    {
        setNonBlock(this->m_Socket);
    }
    int ret = 0;
    socklen_t addr_len = sizeof(*addr);
    // 非阻塞进行connect连接
    ret = connect(this->m_Socket, (sockaddr*)addr, addr_len);
    // 没有立即建立连接
    if (ret == -1)
    {
        // 正在进行连接中
        if (errno == EINPROGRESS)
        {
            fd_set fd_sets;
            FD_ZERO(&fd_sets);
            FD_SET(this->m_Socket, &fd_sets);
            // 当可写时表示链接已经建立好了
            struct timeval tv;
            memset(&tv, 0x00, sizeof(tv));
            tv.tv_sec = wait_seconds;
            tv.tv_usec = 0;
            do
            {
                ret = select(this->m_Socket + 1, NULL, &fd_sets, NULL, &tv);
            } while (ret < 0 && errno == EINTR); // 在等待时被信号打断就再进行一次select
            // 超时
            if (ret == 0)
            {
                ret = -1;
            }
            else if (ret == 1)
            {
                int err;
                socklen_t errLen = sizeof(err);
                ErrorIf(getsockopt(this->m_Socket, SOL_SOCKET, SO_ERROR, &err, &errLen) == -1, 
                "getsockopt err");
                // 连接建立成功
                if (err == 0)
                {
                    ret = 0;
                }
                else
                {
                    ErrorIf(true, "getsockopt socket err");
                }
            }
            else
            {
                ErrorIf(true, "select err");
            }
        }
        // 其他错误
        else
        {
            ErrorIf(true, "connect err");
        }
    }
    // 设置阻塞
    if (wait_seconds > 0)
    {
        setBlock(this->m_Socket);
    }

    return ret;

}
// // 每次从缓冲区读取n个字符
int TcpSocket::readn(void* buf, int count)
{
    size_t nleft = count;
    size_t nread = 0;
    char* bufp = (char*)buf;
    while (nleft > 0)
    {
        nread = read(this->m_Socket, bufp, nleft);
        if (nread == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            ErrorIf(true, "readn err");
        }
        else if (nread == 0)
        {
            return count - nleft;
        }
        nleft -= nread;
        bufp += nread;
    }
    return count;
}
// // 每次从缓冲区写入n个字符
int TcpSocket::writen(const void* buf, int count)
{
    size_t nleft = count;
    size_t nwrited = 0;
    char* bufp = (char*)buf;
    while (nleft > 0)
    {
        nwrited = write(this->m_Socket, bufp, nleft);
        if (nwrited == -1)
        {
            if (errno == EINTR)
            {
                continue;
            }
            ErrorIf(true, "writen write err");
        }
        else if (nwrited == 0)
        {
            continue;
        }
        nleft -= nwrited;
        bufp += nwrited;
    }
    return count;
}