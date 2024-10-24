#pragma once
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <iostream>
#include <string>
#include <string.h>


enum SignLevel
{
    Level1 = NID_md5,
    Level2 = NID_sha1,
    Level3 = NID_sha224,
    Level4 = NID_sha256,
    Level5 = NID_sha384,
    Level6 = NID_sha512
};


class MyRsa
{
public:
    MyRsa();
    MyRsa(std::string fileName, bool isPrivate = true);
    ~MyRsa();
    // 生成公钥、私钥
    void generateRsaKey(int bits, std::string pub = "public.pem", std::string pri = "private.pem");
    // 公钥加密
    std::string rsaPublicKeyEnc(std::string data);
    // 私钥解密
    std::string rsaPrivateKeyDec(std::string data);
    // 数据签名
    std::string rsaSign(std::string data, SignLevel level = Level3);
    // 签名验证
    bool rsaVerify(std::string data, std::string signData, SignLevel level = Level3);
    // base64编码
    std::string toBase64(const char* str, int len);
    // base64解码
    char* fromBase64(std::string str);
private:
    bool initPublicKey(std::string pubfile);
    bool initPrivateKey(std::string prifile);
private:
    RSA* privateKey;
    RSA* publicKey;
};