#pragma once
#include "CodecFactoryBase.h"
#include "RespoenseCodec.h"
#include <string>



class ResponseCodecFactory : public CodecFactoryBase
{
public:
	ResponseCodecFactory(std::string encStr);
	ResponseCodecFactory(ResponseInfo* msg);
	Codec* createCodec();
	~ResponseCodecFactory();

private:
	bool m_flag;
	std::string m_encStr;
	ResponseInfo* m_msg;
};

