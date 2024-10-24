#pragma once
#include "Code.h"


class CodecFactoryBase
{
public:
	CodecFactoryBase();
	virtual ~CodecFactoryBase();
	virtual Codec* createCodec();
};

