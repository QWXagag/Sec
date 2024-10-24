ServerOP:
	g++ main.cpp ServerOP.cpp \
	./../Tcp/TcpSocket.cpp ./../Tcp/Tcpserver.cpp ./../Tcp/util.cpp \
	./../Enc/Code.cpp ./../Enc/CodecFactoryBase.cpp ./../Enc/RequestCodec.cpp ./../Enc/RequestCodecFactory.cpp ./../Enc/RequestMsg.pb.cc ./../Enc/RespoenseCodec.cpp ./../Enc/ResponseCodecFactory.cpp ./../Enc/ResponseMsg.pb.cc \
	./../openssl/MyRsa.cpp ./../openssl/Hash.cpp \
	-o main \
	-I ./../Tcp \
	-ljson \
	`pkg-config --libs --cflags protobuf` -I./../Enc \
	-I./../openssl -I /usr/include/openssl/ -l ssl -lcrypto