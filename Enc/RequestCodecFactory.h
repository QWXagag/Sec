#pragma once
#include "RequestCodec.h"
#include "CodecFactoryBase.h"


class RequestCodecFactory : public CodecFactoryBase
{
public:
	RequestCodecFactory(std::string encStr);
	RequestCodecFactory(RequestInfo* msg);
	Codec* createCodec();
	~RequestCodecFactory();
private:
	bool m_flag;
	std::string m_encStr;
	RequestInfo* m_msg;
};

