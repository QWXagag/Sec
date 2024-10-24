#include "Hash.h"


Hash::Hash(HashType type) : m_type(type)
{
    switch (m_type)
    {
    case T_MD5:
        MD5_Init(&m_md5);
        break;
    case T_SHA1:
        SHA1_Init(&m_sha1);
        break;
    case T_SHA224:
        SHA224_Init(&m_sha224);
        break;
    case T_SHA256:
        SHA256_Init(&m_sha256);
        break;
    case T_SHA384:
        SHA384_Init(&m_sha384);
        break;
    case T_SHA512:
        SHA512_Init(&m_sha512);
        break;
    }
}


void Hash::addData(std::string data)
{
    switch (m_type)
    {
    case T_MD5:
        MD5_Update(&m_md5, data.data(), data.size());
        break;
    case T_SHA1:
        SHA1_Update(&m_sha1, data.data(), data.size());
        break;
    case T_SHA224:
        SHA224_Update(&m_sha224, data.data(), data.size());
        break;
    case T_SHA256:
        SHA256_Update(&m_sha256, data.data(), data.size());
        break;
    case T_SHA384:
        SHA384_Update(&m_sha384, data.data(), data.size());
        break;
    case T_SHA512:
        SHA512_Update(&m_sha512, data.data(), data.size());
        break;
    }
}


std::string Hash::md5Result()
{
    unsigned char md[MD5_DIGEST_LENGTH];
    char res[MD5_DIGEST_LENGTH * 2 + 1];
    MD5_Final(md, &m_md5);
    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        sprintf(&res[i * 2], "%02x", md[i]);
    };
    return std::string(res, MD5_DIGEST_LENGTH * 2 + 1);
}

std::string Hash::SHA1Result()
{
    unsigned char md[SHA_DIGEST_LENGTH];
    char res[SHA_DIGEST_LENGTH * 2 + 1];
    SHA1_Final(md, &m_sha1);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        sprintf(&res[i * 2], "%02x", md[i]);
    };
    return std::string(res, SHA_DIGEST_LENGTH * 2 + 1);
}


std::string Hash::SHA224Result()
{
    unsigned char md[SHA224_DIGEST_LENGTH];
    char res[SHA224_DIGEST_LENGTH * 2 + 1];
    SHA224_Final(md, &m_sha224);
    for (int i = 0; i < SHA224_DIGEST_LENGTH; i++)
    {
        sprintf(&res[i * 2], "%02x", md[i]);
    };
    return std::string(res, SHA224_DIGEST_LENGTH * 2 + 1);
}

std::string Hash::SHA256Result()
{
    unsigned char md[SHA256_DIGEST_LENGTH];
    char res[SHA256_DIGEST_LENGTH * 2 + 1];
    SHA256_Final(md, &m_sha256);
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(&res[i * 2], "%02x", md[i]);
    };
    return std::string(res, SHA256_DIGEST_LENGTH * 2 + 1);
}

std::string Hash::SHA384Result()
{
    unsigned char md[SHA384_DIGEST_LENGTH];
    char res[SHA384_DIGEST_LENGTH * 2 + 1];
    SHA384_Final(md, &m_sha384);
    for (int i = 0; i < SHA384_DIGEST_LENGTH; i++)
    {
        sprintf(&res[i * 2], "%02x", md[i]);
    };
    return std::string(res, SHA384_DIGEST_LENGTH * 2 + 1);
}

std::string Hash::SHA512Result()
{
    unsigned char md[SHA512_DIGEST_LENGTH];
    char res[SHA512_DIGEST_LENGTH * 2 + 1];
    SHA512_Final(md, &m_sha512);
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
    {
        sprintf(&res[i * 2], "%02x", md[i]);
    };
    return std::string(res, SHA512_DIGEST_LENGTH * 2 + 1);
}

std::string Hash::result()
{
    std::string result;
    switch (m_type)
    {
    case T_MD5:
        result = md5Result();
        break;
    case T_SHA1:
        result = SHA1Result();
        break;
    case T_SHA224:
        result = SHA224Result();
        break;
    case T_SHA256:
        result = SHA256Result();
        break;
    case T_SHA384:
        result = SHA384Result();
        break;
    case T_SHA512:
        result = SHA512Result();
        break;
    }
    return result;
}