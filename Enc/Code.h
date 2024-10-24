#pragma once
#include <string>


class Codec
{
public:
	Codec();
	virtual std::string encodeMsg();
	virtual void* decodeMsg();
	virtual ~Codec();
};

