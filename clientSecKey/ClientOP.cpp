#include "ClientOP.h"
#include <fstream>

ClientOP::ClientOP(string file_name)
{
    // 读配置文件
    ifstream ifs(file_name);
    Reader r;
    Value root;
    r.parse(ifs, root);
    m_info.serverID = root["serverID"].asString();
    m_info.clientID = root["clientID"].asString();
    m_info.ip = root["ip"].asCString();
    m_info.port = root["port"].asUInt();
    cout << m_info.serverID << endl;
    cout << m_info.clientID << endl;
    cout << m_info.ip << endl;
    cout << m_info.port << endl;
}

ClientOP::~ClientOP()
{

}

bool ClientOP::seckeyConsult()
{
    // 一、准备数据
    // 二、序列化
    // 三、与服务器通信

    // 准备数据
    // 1、公钥
    // 2、客户端id
    // 3、服务的id
    // 4、cmdtype
    // 5、对公钥的签名
    RequestInfo r_info;
    r_info.serverId = m_info.serverID;
    r_info.clientId = m_info.clientID;
    r_info.cmdType = 1;

    MyRsa rsa;
    rsa.generateRsaKey(1024);
    ifstream ifs("public.pem");
    stringstream pubStr;
    pubStr << ifs.rdbuf();
    r_info.data = pubStr.str();
    // 对数据进行哈希
    Hash h(HashType::T_SHA1);
    h.addData(pubStr.str());
    r_info.sign = rsa.rsaSign(h.result());

    // 序列化
    // 创建工厂类、根据传入的对象来判断编码、解码
    CodecFactoryBase* f_request = new RequestCodecFactory(&r_info);
    // 多态
    Codec* req_c = f_request->createCodec();
    // 对数据进行序列化操作
    string encstr = req_c->encodeMsg();
    // 得到序列化的数据后要释放资源
    delete f_request;
    delete req_c;


    // 与服务器通信
    TcpSocket t;
    // 尝试连接服务器
    int ret = t.connectToHost(m_info.ip, m_info.port, 5);
    if (ret == 0)
    {
        cout << "成功连接服务器"<< endl;
    }
    else
    {
        cout << "连接服务器失败" << endl;
        return false;
    }
    // 给服务器发送数据
    t.sendMsg(encstr);
    // 等待接收服务器数据
    string msg = t.recvMsg();
    // 解析服务器数据
    CodecFactoryBase* response = new ResponseCodecFactory(msg);
    Codec* c = response->createCodec();
    ResponseMsg* serverMsg = (ResponseMsg*)c->decodeMsg();

    if (!serverMsg->rv())
    {
        cout << "密钥协商失败" << endl;
        return false;
    }
    
    // 获取对称加密的密钥
    cout << "base64的对称加密密钥: " << serverMsg->data() << endl;
    string aesKey = rsa.rsaPrivateKeyDec(serverMsg->data());
    // 解密
    cout << "对称加密密钥：" << aesKey << endl;
    delete response;
    delete c;
    t.disConnect();
    return true;
    
}