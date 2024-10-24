#pragma once
#include "Code.h"
#include "RequestMsg.pb.h"


struct RequestInfo
{
	int cmdType;
	std::string clientId;
	std::string serverId;
	std::string sign;
	std::string data;
};



class RequestCodec : public Codec
{
private:
	std::string m_encStr;
	RequestMsg m_msg;

public:
	RequestCodec();
	// 解码时使用
	RequestCodec(std::string encStr);
	// 编码时使用
	RequestCodec(RequestInfo* msg);
	// 解码初始化
	void initMessage(std::string encStr);
	// 编码初始化
	void initMessage(RequestInfo* msg);
	// 编码
	std::string encodeMsg();
	// 解码
	void* decodeMsg();
	~RequestCodec();

};

