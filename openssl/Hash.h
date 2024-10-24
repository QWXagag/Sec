#include <openssl/sha.h>
#include <openssl/md5.h>
#include <string>


enum HashType
{
    T_MD5,
    T_SHA1,
    T_SHA224,
    T_SHA256,
    T_SHA384,
    T_SHA512
};

class Hash
{
public:
    Hash(HashType type);
    void addData(std::string data);
    std::string result();

private:
    std::string md5Result();
    std::string SHA1Result();
    std::string SHA224Result();
    std::string SHA256Result();
    std::string SHA384Result();
    std::string SHA512Result();

private:
    HashType m_type;
    MD5_CTX m_md5;
    SHA_CTX m_sha1;
    SHA256_CTX m_sha224;
    SHA256_CTX m_sha256;
    SHA512_CTX m_sha384;
    SHA512_CTX m_sha512;
};