#include "RequestCodecFactory.h"


RequestCodecFactory::RequestCodecFactory(std::string encStr)
{
	m_flag = true;
	m_encStr = encStr;
}

RequestCodecFactory::RequestCodecFactory(RequestInfo* msg)
{
	m_flag = false;
	m_msg = msg;
}

Codec* RequestCodecFactory::createCodec()
{
	Codec* codec = NULL;
	if (m_flag)
	{
		codec = new RequestCodec(m_encStr);
	}
	else
	{
		codec = new RequestCodec(m_msg);
	}
	return codec;
}

RequestCodecFactory::~RequestCodecFactory()
{

}