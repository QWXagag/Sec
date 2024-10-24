#include "ServerOP.h"


ServerOP::ServerOP()
{
    ifstream ifs("server.json");
    Value root;
    Reader r;
    r.parse(ifs, root);
    m_port = root["port"].asUInt();
    m_serverID = root["serverID"].asString();
}

ServerOP::~ServerOP()
{
    if (m_server != NULL)
    {
        delete m_server;
        m_server = NULL;
    }
}

string ServerOP::generateRandomStr(KeyLen num)
{
    srand(time(NULL));
    string randomStr;
    const char* buf = "~`@#$%^&*()_+{}:<>?,.[]|;";
    for (int i = 0; i < num; i++)
    {
        int flag = rand() % 4;
        switch (flag)
        {
        case 0:
            // 0~9
            randomStr.append(1, rand() % 10 + '0');
            break;
        case 1:
            // a-z
            randomStr.append(1, rand() % 26 + 'a');
            break;
        case 2:
            // A-Z
            randomStr.append(1, rand() % 26 + 'A');
            break;
        case 3:
            // 特殊字符
            randomStr.append(1, buf[rand() % strlen(buf)]);
            break;
        }
    }
    return randomStr;
}


string ServerOP::seckeyConsult(RequestMsg* msg)
{
    // 组织数据
    ResponseInfo resMsg;
    // 检验签名
    cout << "msg->data()base64: " << msg->data() << endl;
    Hash h(HashType::T_SHA1);
    h.addData(msg->data());
    ofstream ofs("public.pem");
    ofs << msg->data();
    ofs.flush();
    ofs.close();
    MyRsa rsa("public.pem", false);
    bool ret = rsa.rsaVerify(h.result(), msg->sign());
    string randomStr;
    if (!ret)
    {
        resMsg.rv = false;
        cout << "签名校验不通过" << endl;
    }
    else
    {
        // 生成随机字符串
        // 对随机字符串使用公钥加密
        randomStr = generateRandomStr(Len16);
        cout << "randomStr: " << randomStr << endl;
        resMsg.data = rsa.rsaPublicKeyEnc(randomStr);
        resMsg.rv = true;
        resMsg.clientId = msg->clientid();
        resMsg.serverId = m_serverID;
        resMsg.seckeyId = 1;
        cout << "签名校验通过" << endl;

    }
    CodecFactoryBase* f = new ResponseCodecFactory(&resMsg);
    Codec* c = f->createCodec();
    return c->encodeMsg();
}

void* ServerOP::workling(void* arg)
{
    ServerOP* op = (ServerOP*)arg;
    // 1、接收客户端请
    // sleep或者加锁
    TcpSocket* tcp = op->m_list[pthread_self()];
    pthread_t tid = pthread_self();

    string msg = tcp->recvMsg(5);
    // 反序列化
    CodecFactoryBase* f = new RequestCodecFactory(msg);
    Codec* c = f->createCodec();
    RequestMsg* requestM = (RequestMsg*)c->decodeMsg();
    // 2、查看cmdtype
    int cmdType = requestM->cmdtype();
    // 3、根据cmdtype执行不同的业务逻辑
    string data;
    switch (cmdType)
    {
    case 1:
        // 密钥协商
        data = op->seckeyConsult(requestM);
        break;
    case 2:
        // 密钥校验
        break;
    case 3:
        // 密钥注销
        break;
    }
    tcp->sendMsg(data);
    op->m_list.erase(pthread_self());
    delete f;
    delete c;
    return NULL;
}

void ServerOP::startServer()
{
    m_server = new TcpServer;
    m_server->setListen(m_port);
    while (1)
    {
        TcpSocket* tcp = m_server->acceptConn();
        if (tcp == NULL)
        {
            continue;
        }

        // 创建子线程
        pthread_t tid;
        // pthread_attr_t attr;
        // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        pthread_create(&tid, NULL, workling, this);
        m_list.insert(make_pair(tid, tcp));
    }
    
    
}