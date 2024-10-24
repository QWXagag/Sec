#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <string.h>
#include <iostream>
#include "MyRsa.h"
using namespace std;

#include <openssl/aes.h>


void generateRsaKey()
{
    // 得到RSA变量
    RSA* rsa = RSA_new();
    // 得到大数变量
    BIGNUM* e =  BN_new();
    // 初始化大数
    BN_set_word(e, 12345);
    // 生成密钥对
    RSA_generate_key_ex(rsa, 1024, e, NULL);
    
    // 将公钥写入文件
    FILE* fp = fopen("./public.pem", "w");
    PEM_write_RSAPublicKey(fp, rsa);
    fclose(fp);
    // 将私钥写入文件 
    fp = fopen("./private.pem", "w");
    PEM_write_RSAPrivateKey(fp, rsa, NULL, NULL, 0, NULL, NULL);
    fclose(fp);

    // 通过openssl自带bio对象对公钥私钥进行写入文件的操作
    BIO* bio = BIO_new_file("./bio-public.pem", "w");
    PEM_write_bio_RSAPublicKey(bio, rsa);
    BIO_free(bio);
    bio = BIO_new_file("./bio-private.pem", "w");
    PEM_write_bio_RSAPrivateKey(bio, rsa, NULL, NULL, 0, NULL, NULL);
    BIO_free(bio);
    RSA_free(rsa);
}


void rsaEncAndDec()
{
    // 公钥加密
    // 1、准备数据
    string text = "让编程改变世界...";
    // 2、准备密钥
    // 2.1、从内存中读取密钥
    // RSA* pubKey = RSAPublicKey_dup(rsa);
    // RSA* privateKey = RSAPrivateKey_dup(rsa);

    // 2.2、从磁盘读取密钥（一般情况下都是从磁盘读取）
    // 使用BIO方式从磁盘读取密钥
    BIO* bio = BIO_new_file("./bio-public.pem", "r");
    RSA* publicKey = RSA_new();
    if (PEM_read_bio_RSAPublicKey(bio, &publicKey, NULL, NULL) == NULL)
    {
        cout << "读取公钥失败"<< endl;
        return ;
    }
    BIO_free(bio);
    // 3、加密
    // 由于要加密的数据长度不能大于密钥长度-11，我们通过提供的函数来直接计算出密钥的长度
    // 数据被加密后，数据的长度与密钥长度相同
    int keyLen = RSA_size(publicKey);
    char* buf = new char[keyLen];
    int secLen = RSA_public_encrypt(text.size(), (const unsigned char*)text.data(), (unsigned char*)buf, publicKey, RSA_PKCS1_PADDING);
    // 此时buf中的内容就是"让编程改变世界..."被加密后的内容
    cout << "加密后的密文：" << buf << endl;
    cout << "加密后的密文长度：" << secLen << endl;
    // 私钥解密
    // 从磁盘中读取私钥
    bio = BIO_new_file("./bio-private.pem", "r");
    RSA* privateKey = RSA_new();
    if (PEM_read_bio_RSAPrivateKey(bio, &privateKey, NULL, NULL) == NULL)
    {
        cout << "读取私钥失败"<< endl;
        return ;
    }
    BIO_free(bio);
    char* r_buf = new char[keyLen];
    int decLen = RSA_private_decrypt(secLen, (const unsigned char*)buf, (unsigned char*)r_buf, privateKey, RSA_PKCS1_PADDING);
    cout << "解密后的密文：" << string(r_buf, decLen) << endl;
    cout << "原始数据的长度：" << decLen << endl;
}


void rsaSignAndVerfiy()
{
    // 密钥、原始数据
    string text = "让编程改变世界...";

    RSA* pub_rsa = RSA_new();
    BIO* pub_bio = BIO_new_file("./bio-public.pem", "r");
    if (PEM_read_bio_RSAPublicKey(pub_bio, &pub_rsa, NULL, NULL) == NULL)
    {
        cout << "读取公钥失败" << endl;
        return;
    }
    BIO* pri_bio = BIO_new_file("./bio-private.pem", "r");
    RSA* pri_rsa = RSA_new();
    if (PEM_read_bio_RSAPrivateKey(pri_bio, &pri_rsa, NULL, NULL) == NULL)
    {
        cout << "读取私钥失败" << endl;
        return;
    }
    unsigned int rsa_len = RSA_size(pri_rsa);
    unsigned char* out_data = new unsigned char[rsa_len];
    unsigned int out_len = 0;
    RSA_sign(NID_sha1, (const unsigned char*)text.data(), text.size(), out_data, &out_len, pri_rsa);

    int ret = RSA_verify(NID_sha1, (const unsigned char*)text.data(), text.size(), out_data, out_len, pub_rsa);
    if (ret == 1)
    {
        cout << "签名验证通过" << endl;
    }

}


void testAes()
{
    // 1、准备需要加密的数据
    const unsigned char* data = reinterpret_cast<const unsigned char*>("AES是一套对称密钥的密码术，目前已广泛使用，用于替代已经不够安全的DES算法。所谓对称密钥，就是说加密和解密用的是同一个密钥，消息的发送方和接收方在消息传递前需要享有这个密钥。和非对称密钥体系不同，这里的密钥是双方保密的，不会让任何第三方知道。");
    // 2、准备密钥、初始化密钥
    const unsigned char* key = reinterpret_cast<const unsigned char*>("1234567887654321");
    AES_KEY encKey;
    AES_set_encrypt_key(key, 128, &encKey);
    // 3、加密
    int data_len = strlen((char*)data) + 1;
    int length = 0;
    if (data_len % 16 == 0)
    {
        length = data_len;
    }
    else
    {
        length = ((data_len / 16) + 1) * 16;
    }
    unsigned char* out_data = new unsigned char[length];
    unsigned char ivec[AES_BLOCK_SIZE];
    memset(ivec, 9, sizeof(ivec));
    AES_cbc_encrypt(data, out_data, length, &encKey, ivec, AES_ENCRYPT);
    // 4、解密
    unsigned char* dec_data = new unsigned char[length];
    AES_KEY decKey;
    AES_set_decrypt_key(key, 128, &decKey);
    memset(ivec, 9, sizeof(ivec));
    AES_cbc_encrypt(out_data, dec_data, length, &decKey, ivec, AES_DECRYPT);
    // 5、验证
    cout << "解密后:" << dec_data << endl;
}


void testBase64()
{
    MyRsa r;
    string s("123456789{}:><_)+");
    string b64Str = r.toBase64(s);
    cout << "原: "<< s << endl;
    cout << "base64编码: " << b64Str << endl;
    string ss = r.fromBase64(b64Str);
    cout << "base64解码: " << ss << endl;
}


int main()
{
    testBase64();
    return 0;
}