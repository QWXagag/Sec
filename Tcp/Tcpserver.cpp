#include "TcpServer.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "util.h"
#include <iostream>

TcpServer::TcpServer()
{
}

TcpServer::~TcpServer()
{
}

int TcpServer::setListen(unsigned short port)
{
	int ret = 0;
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
    inet_pton(AF_INET, "192.168.1.3", &servaddr.sin_addr.s_addr);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// 创建监听的套接字
	m_lfd = socket(AF_INET, SOCK_STREAM, 0);
    ErrorIf(m_lfd == -1, "setListen socket err");

	int on = 1;
	// 设置端口复用
	ret = setsockopt(m_lfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    ErrorIf(ret == -1, "setListen setsockopt err");

	// 监听的套接字绑定本地IP和端口
	ret = bind(m_lfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    ErrorIf(ret == -1, "setListen bind err");

	ret = listen(m_lfd, 128);
    ErrorIf(ret == -1, "setListen listen err");

	return ret;
}

TcpSocket* TcpServer::acceptConn(int wait_seconds)
{
	int ret;
	if (wait_seconds > 0)
	{
		fd_set accept_fdset;
		struct timeval timeout;
		FD_ZERO(&accept_fdset);
		FD_SET(m_lfd, &accept_fdset);
		timeout.tv_sec = wait_seconds;
		timeout.tv_usec = 0;
		do
		{
			// 检测读集合
			ret = select(m_lfd + 1, &accept_fdset, NULL, NULL, &timeout);
		} while (ret < 0 && errno == EINTR);	// 被信号中断, 再次进入循环
		if (ret <= 0)
		{
            ErrorIf(ret < 0, "acceptConn select err");
		}
	}

	// 一但检测出 有select事件发生，表示对等方完成了三次握手，客户端有新连接建立
	// 此时再调用accept将不会堵塞
	struct sockaddr_in addrCli;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	int connfd = accept(m_lfd, (struct sockaddr*)&addrCli, &addrlen); //返回已连接套接字
    ErrorIf(connfd == -1, "acceptConn accept err");
	return new TcpSocket(connfd);
}

void TcpServer::closefd()
{
	close(m_lfd);
}