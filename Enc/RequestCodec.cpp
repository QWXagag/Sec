#include "RequestCodec.h"
#include <iostream>
using namespace std;


RequestCodec::RequestCodec()
{

}
// 解码时使用
RequestCodec::RequestCodec(std::string encStr)
{
	initMessage(encStr);
}

// 编码时使用
RequestCodec::RequestCodec(RequestInfo* msg)
{
	initMessage(msg);
}

// 解码初始化
void RequestCodec::initMessage(std::string encStr)
{
	m_encStr = encStr;
}

// 编码初始化
void RequestCodec::initMessage(RequestInfo* msg)
{
	m_msg.set_cmdtype(msg->cmdType);
	m_msg.set_clientid(msg->clientId);
	m_msg.set_serverid(msg->serverId);
	m_msg.set_sign(msg->sign);
	m_msg.set_data(msg->data);
}

// 编码
std::string RequestCodec::encodeMsg()
{
	std::string output;
	m_msg.SerializeToString(&output);
	return output;
}

// 解码
void* RequestCodec::decodeMsg()
{
	m_msg.ParseFromString(m_encStr);
	return &m_msg;
}

RequestCodec::~RequestCodec()
{

}