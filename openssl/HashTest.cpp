#include <string.h>
#include <iostream>
#include "Hash.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
using namespace std;

// be3c9f2044d3e6d721184
void shaTest()
{
    // 1、初始化
    Hash h1(HashType::T_MD5);
    // 2、添加数据
    h1.addData(string("hello"));
    // h1.addData(string(", world"));
    // // 3、哈希计算
    string retH1 = h1.result();
    cout << retH1 << endl;

    Hash h2(HashType::T_SHA1);
    h2.addData(string("hello, world"));
    // h2.addData(string(", world"));
    string retH2 = h2.result();
    cout << retH2 << endl;
    


}

int main()
{
    shaTest();
    return 0;
}