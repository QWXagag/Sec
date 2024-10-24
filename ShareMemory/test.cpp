#include <iostream>
#include "BaseShm.h"
#include "SecKeyShm.h"
#include <string.h>
#include <unistd.h>
using namespace std;





// void testBaseShm1()
// {
//     BaseShm b1(100, 1024);
//     BaseShm b2("/home", 2048);
//     char* p1 = static_cast<char*>(b1.shmMap());
//     strcpy(p1, "hello");
//     char* p2 = static_cast<char*>(b2.shmMap());
//     strcpy(p2, "world");
//     sleep(100);
// }


// void testBaseShm2()
// {
//     BaseShm b1(100, 1024);
//     BaseShm b2("/home", 2048);
//     char* p1 = static_cast<char*>(b1.shmMap());
//     cout << p1 << endl;
//     char* p2 = static_cast<char*>(b2.shmMap());
//     cout << p2 << endl;
// }


// void testSecKeyShm3()
// {
//     SecKeyShm s1(100, 1024);
//     SecKeyShm s2("/home", 2048);
//     NodeShmInfo n1;
//     strcpy(n1.clientID, "client001");
//     strcpy(n1.serverID, "server001");
//     strcpy(n1.seckey, "seckey001");
//     n1.status = 1;
//     n1.seckeyID = 1;
//     s1.writeShm(&n1);
//     NodeShmInfo n2;
//     strcpy(n2.clientID, "client002");
//     strcpy(n2.serverID, "server002");
//     strcpy(n2.seckey, "seckey002");
//     n2.status = 2;
//     n2.seckeyID = 2;
//     s2.writeShm(&n2);
// }

void testSecKeyShm4()
{
    SecKeyShm s1(100, 1024);
    SecKeyShm s2("/home", 2048);
    s1.readShm("client001", "server001");
    s2.readShm("client002", "server002");
    s2.readShm("001", "001");
}

int main()
{
    // testBaseShm1();
    // testBaseShm2();
    // testSecKeyShm3();
    testSecKeyShm4();
    return 0;
}