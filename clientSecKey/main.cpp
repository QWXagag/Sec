#include <iostream>
#include "ClientOP.h"
using namespace std;


int  usage();
int main()
{
    ClientOP c("server.json");
    while (1)
    {
        int nSel = usage();
        bool ret;
        switch (nSel)
        {
        // 密钥协商
        case 1:
            ret = c.seckeyConsult();
            if (ret)
            {
                cout << "密钥协商成功" << endl;
            }
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break; 
        default:
            break;
        }
    }
    cout << "退出" << endl;
    return 0;
}


int usage()
{
    printf("/*-----------------------------------*/\n");
    printf("/*----------1.密钥协商----------------*/\n");
    printf("/*----------2.密钥校验----------------*/\n");
    printf("/*----------3.密钥注销----------------*/\n");
    printf("/*----------4.查看密钥----------------*/\n");
    printf("/*----------5.退出系统----------------*/\n");
    printf("/*-----------------------------------*/\n");
    int nSel = -1;
    scanf("%d", &nSel);
    while (getchar() != '\n');
    return nSel;
}