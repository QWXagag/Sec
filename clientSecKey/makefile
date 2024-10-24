ClientOP:
	g++ main.cpp ClientOP.cpp ./../Enc/Code.cpp ./../Enc/CodecFactoryBase.cpp ./../Enc/RequestCodec.cpp \
	./../Enc/RequestCodecFactory.cpp ./../Enc/RequestMsg.pb.cc ./../Enc/RespoenseCodec.cpp \
	./../Enc/ResponseCodecFactory.cpp ./../Enc/ResponseMsg.pb.cc \
	./../Tcp/TcpSocket.cpp ./../Tcp/Tcpserver.cpp ./../Tcp/util.cpp \
	./../openssl/MyRsa.cpp ./../openssl/Hash.cpp -o main \
	`pkg-config --libs --cflags protobuf` -I./../Enc \
	-I ./../Tcp \
	-I./../openssl \
	-ljson -I /usr/include/openssl/ -l ssl -lcrypto