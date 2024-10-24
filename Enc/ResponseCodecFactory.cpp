#include "ResponseCodecFactory.h"


ResponseCodecFactory::ResponseCodecFactory(std::string encStr)
{
	m_flag = true;
	m_encStr = encStr;
}

ResponseCodecFactory::ResponseCodecFactory(ResponseInfo* msg)
{
	m_flag = false;
	m_msg = msg;
}

Codec* ResponseCodecFactory::createCodec()
{
	Codec* codec = NULL;
	if (m_flag)
	{
		codec = new RespoenseCodec(m_encStr);
	}
	else
	{
		codec = new RespoenseCodec(m_msg);
	}
	return codec;
}

ResponseCodecFactory::~ResponseCodecFactory()
{

}