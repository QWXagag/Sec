#include <iostream>
#include <occi.h>
using namespace std;
using namespace oracle::occi;



void queryData()
{
    // 初始化连接环境
    Environment* env = Environment::createEnvironment("utf8", "utf8");

    // 根据环境对象，创建一个连接数据库服务器的实例
    const string userName = "scott";
    const string passwd = "tiger";
    const string connstr = "192.168.1.4:1521/orcl";
    Connection* conn = env->createConnection(userName, passwd, connstr);

    // 创建一个操作sql的对象
    Statement* st = conn->createStatement();
    
    // 业务逻辑，数据库操作
    // sql语句
    string sql = "select * from dept";
    st->setSQL(sql);
    ResultSet* result = st->executeQuery();
    while (result->next())
    {
        cout << "no: " << result->getInt(1) << ", "
        << "name: " << result->getString(2) << ", "
        << "location: " << result->getString(3) << endl;
    }
    st->closeResultSet(result);

    conn->terminateStatement(st);
    // 销毁连接对象
    env->terminateConnection(conn);

    // 关闭释放资源
    Environment::terminateEnvironment(env);
}


void insertData()
{
        // 初始化连接环境
    Environment* env = Environment::createEnvironment("utf8", "utf8");

    // 根据环境对象，创建一个连接数据库服务器的实例
    const string userName = "scott";
    const string passwd = "tiger";
    const string connstr = "192.168.1.4:1521/orcl";
    Connection* conn = env->createConnection(userName, passwd, connstr);

    // 创建一个操作sql的对象
    Statement* st = conn->createStatement();
    
    // 业务逻辑，数据库操作
    // sql语句
    string sql = "insert into dept values(55, '小日本', '小日本')";
    st->setSQL(sql);
    bool bl = st->getAutoCommit();
    cout << "事务的默认状态：" << bl << endl;
    st->setAutoCommit(true); // 开启事务
    int ret = st->executeUpdate();
    cout << "insert return value: " << ret << endl;
    if (ret <= 0)
    {
        cout << "回滚" << endl;
        conn->rollback();
    }
    else
    {
        cout << "提交" << endl;
        conn->commit();   
    }

    conn->terminateStatement(st);
    // 销毁连接对象
    env->terminateConnection(conn);

    // 关闭释放资源
    Environment::terminateEnvironment(env);
}


int main()
{
    queryData();
    insertData();
    queryData();
    return 0;
}