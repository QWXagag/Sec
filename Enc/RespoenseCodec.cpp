#include "RespoenseCodec.h"


RespoenseCodec::RespoenseCodec()
{

}

RespoenseCodec::RespoenseCodec(std::string encStr)
{
	initMessage(encStr);
}

RespoenseCodec::RespoenseCodec(ResponseInfo* msg)
{
	initMessage(msg);
}

void RespoenseCodec::initMessage(std::string encStr)
{
	m_encStr = encStr;
}


void RespoenseCodec::initMessage(ResponseInfo* msg)
{
	m_msg.set_rv(msg->rv);
	m_msg.set_seckeyid(msg->seckeyId);
	m_msg.set_clientid(msg->clientId);
	m_msg.set_serverid(msg->serverId);
	m_msg.set_data(msg->data);
}


std::string RespoenseCodec::encodeMsg()
{
	std::string output;
	m_msg.SerializeToString(&output);
	return output;
}

void* RespoenseCodec::decodeMsg()
{
	m_msg.ParseFromString(m_encStr);
	return &m_msg;
}

RespoenseCodec::~RespoenseCodec()
{

}