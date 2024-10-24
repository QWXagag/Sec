#pragma once
#include "Code.h"
#include <string>
#include "ResponseMsg.pb.h"


struct ResponseInfo
{
    bool rv; // 返回值
    int seckeyId; 		// 对称密钥编号 keysn
    std::string clientId; 	// 客户端编号
    std::string serverId; 	// 服务器编号
    std::string data; 	// 服务器端随机数
};


class RespoenseCodec : public Codec
{
private:
	std::string m_encStr;
	ResponseMsg m_msg;
public:
    RespoenseCodec();
    RespoenseCodec(std::string encStr);
    RespoenseCodec(ResponseInfo* msg);
    void initMessage(std::string encStr);
    void initMessage(ResponseInfo* msg);
    std::string encodeMsg();
    void* decodeMsg();
    ~RespoenseCodec();
};

