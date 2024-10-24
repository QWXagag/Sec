#include "TcpSocket.h"
#include <string.h>
#include <iostream>
using namespace std;




int main()
{
    TcpSocket t;
    t.connectToHost("192.168.1.3", 8888, 3);
    string s;
    while (true)
    {
        cin >> s;
        t.sendMsg(s, 6);
        string s = t.recvMsg(6);
        cout << s << endl;
    }
    
    
    return 0;
}