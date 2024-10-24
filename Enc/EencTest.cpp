#include <iostream>
#include "RequestCodecFactory.h"
#include "ResponseCodecFactory.h"
using namespace std;



int main()
{
	// request编码
	RequestInfo req_info = {1001, "clientId: 1001", "serverId: 1001", "sign: 1001", "data: 1001"};
	CodecFactoryBase* req = new RequestCodecFactory(&req_info);
	Codec* req_codec = req->createCodec();
	std::string req_decode_str = req_codec->encodeMsg();
	delete req;
	delete req_codec;

	// request解码
	CodecFactoryBase* req_1 = new RequestCodecFactory(req_decode_str);
	Codec* req_codec_1 = req_1->createCodec();
	RequestMsg* req_msg = (RequestMsg*)req_codec_1->decodeMsg();
	
	cout << "cmdType: " << req_msg->cmdtype()
		<< req_msg->clientid()
		<< req_msg->serverid()
		<< req_msg->sign()
		<< req_msg->data()
		<< endl;

	// response编码
	ResponseInfo res_info = { true, 1002, "clientId: 1002", "serverId: 1002", "data: 1002" };
	CodecFactoryBase* res = new ResponseCodecFactory(&res_info);
	Codec* res_codec = res->createCodec();
	std::string res_decode_str = res_codec->encodeMsg();
	delete res;
	delete res_codec;

	// response解码
	CodecFactoryBase* res_1 = new ResponseCodecFactory(res_decode_str);
	Codec* res_codec_1 = res_1->createCodec();
	ResponseMsg* res_msg = (ResponseMsg*)res_codec_1->decodeMsg();
	cout << "返回值: " << res_msg->rv()
		<< "对称密钥: " << res_msg->seckeyid()
		<< res_msg->clientid()
		<< res_msg->serverid()
		<< res_msg->data()
		<< endl;


	return 0;
}