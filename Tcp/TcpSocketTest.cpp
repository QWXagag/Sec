#include <iostream>
#include "TcpSocket.h"
#include <string>
#include <unistd.h>
#include <string.h>
#include "util.h"
#include "TcpServer.h"
using namespace std;



int main()
{
    TcpServer server;
    server.setListen(8888);
    TcpSocket* server_sock = server.acceptConn(10);
    while (true)
    {
        string s = server_sock->recvMsg(6);
        cout << "client:" << s << endl;
        server_sock->sendMsg(s, 6);
    }
    


    return 0;
}
