#include "MyRsa.h"


MyRsa::MyRsa()
{
    privateKey = RSA_new();
    publicKey = RSA_new();
}

MyRsa::MyRsa(std::string fileName, bool isPrivate)
{
    privateKey = RSA_new();
    publicKey = RSA_new();
    if (isPrivate)
    {
        initPrivateKey(fileName);
    }
    else
    {
        initPublicKey(fileName);
    }
}

MyRsa::~MyRsa()
{
    RSA_free(publicKey);
    RSA_free(privateKey);
}


bool MyRsa::initPublicKey(std::string pubfile)
{
    BIO* pubBio = BIO_new_file(pubfile.data(), "r");
    if (PEM_read_bio_RSAPublicKey(pubBio, &publicKey, NULL, NULL) == NULL)
    {
        ERR_print_errors_fp(stdout);
        return false;
    }
    BIO_free(pubBio);
    return true;

}

bool MyRsa::initPrivateKey(std::string prifile)
{
    BIO* priBio = BIO_new_file(prifile.data(), "r");
    if (PEM_read_bio_RSAPrivateKey(priBio, &privateKey, NULL, NULL) == NULL)
    {
        ERR_print_errors_fp(stdout);
        return false;
    }
    BIO_free(priBio);
    return true;
}



void MyRsa::generateRsaKey(int bits, std::string pub, std::string pri)
{
    // 创建密钥对对象
    RSA* rsa = RSA_new();
    // 创建BIGNUM对象
    BIGNUM* e = BN_new();
    // 初始化BIGNUM对象
    BN_set_word(e, 456787);
    // 生成密钥对
    RSA_generate_key_ex(rsa, bits, e, NULL);
    // 创建BIO公钥对象
    BIO* pubBio = BIO_new_file(pub.data(), "w");
    // 输出公钥到文件
    PEM_write_bio_RSAPublicKey(pubBio, rsa);
    BIO_flush(pubBio);
    BIO_free(pubBio);

    // 创建BIO私钥对象
    BIO* priBio = BIO_new_file(pri.data(), "w");
    // 输出私钥到文件
    PEM_write_bio_RSAPrivateKey(priBio, rsa, NULL, NULL, 0, NULL, NULL);
    BIO_flush(priBio);
    BIO_free(priBio);

    // 将公钥私钥保存到对象私有属性中
    publicKey = RSAPublicKey_dup(rsa);
    privateKey = RSAPrivateKey_dup(rsa);

    // 最后释放
    BN_free(e);
    RSA_free(rsa);
}


std::string MyRsa::rsaPublicKeyEnc(std::string data)
{
    // 获取公钥长度
    unsigned int out_len = RSA_size(publicKey);
    // 申请内存空间
    char* out = new char[out_len + 1];
    // 使用公钥加密
    int ret = RSA_public_encrypt(data.size(), (const unsigned char*)data.data(), (unsigned char*)out, publicKey, RSA_PKCS1_PADDING);
    std::string retString = std::string();
    if (ret >= 0)
    {
        std::cout << "使用rsa公钥加密成功" << std::endl;
        // base64编码
        retString = toBase64(out, ret);
    }
    delete[]out;
    return retString;
}

std::string MyRsa::rsaPrivateKeyDec(std::string data)
{
    char* text = fromBase64(data);
    // 获取私钥长度
    unsigned int keyLen = RSA_size(privateKey);
    // 申请内存空间
    char* out = new char[keyLen + 1];
    // base64解码

    // 使用私钥解密
    int ret = RSA_private_decrypt(keyLen, (const unsigned char*)text, (unsigned char*)out, privateKey, RSA_PKCS1_PADDING);
    std::string retString = std::string(); 
    if (ret >= 0)
    {
        retString = std::string(out, ret);
    }
    else
    {
        std::cout << "私钥解密失败" << std::endl;
    }
    delete[]out;
    delete[]text;
    return retString;
}

std::string MyRsa::rsaSign(std::string data, SignLevel level)
{
    // 获取私钥长度
    unsigned int rsa_len = RSA_size(privateKey);
    // 申请内存空间
    char* out = new char[rsa_len + 1];
    unsigned int out_len;
    RSA_sign(level, (const unsigned char*)data.data(), data.size(), (unsigned char*)out, &out_len, privateKey);
    std::string retString = toBase64(out, out_len);
    delete[]out;
    return retString;
}

bool MyRsa::rsaVerify(std::string data, std::string signData, SignLevel level)
{
    char* sign = fromBase64(signData);
    int keyLen = RSA_size(publicKey);
    int ret = RSA_verify(level, (const unsigned char*)data.data(), data.size(), (unsigned char*)sign, keyLen, publicKey);
    delete sign;
    if (ret != 1 || ret == -1)
    {
        return false;
    }
    return true;
}


std::string MyRsa::toBase64(const char* str, int len)
{
	BIO* mem = BIO_new(BIO_s_mem());
	BIO* bs64 = BIO_new(BIO_f_base64());
	// mem添加到bs64中
	bs64 = BIO_push(bs64, mem);
	// 写数据
	BIO_write(bs64, str, len);
	BIO_flush(bs64);
	// 得到内存对象指针
	BUF_MEM *memPtr;
	BIO_get_mem_ptr(bs64, &memPtr);
	std::string retStr = std::string(memPtr->data, memPtr->length - 1);
	BIO_free_all(bs64);
	return retStr;
}

char* MyRsa::fromBase64(std::string str)
{
	int length = str.size();
	BIO* bs64 = BIO_new(BIO_f_base64());
	BIO* mem = BIO_new_mem_buf(str.data(), length);
	BIO_push(bs64, mem);
	char* buffer = new char[length];
	memset(buffer, 0, length);
	BIO_read(bs64, buffer, length);
	BIO_free_all(bs64);

	return buffer;
}

