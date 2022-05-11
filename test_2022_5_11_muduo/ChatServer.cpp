#include<iostream>
#include<string>
#include<muduo/net/EventLoop.h>
#include<muduo/net/TcpServer.h>
using namespace std;
using namespace muduo;
using namespace muduo::net;
class ChatServer
{
    public:
    ChatServer(EventLoop*loop,InetAddress&addr,string name):
    _server(loop,addr,name),
    _loop(loop)
    {
        //绑定两个回调函数
        _server.setConnectionCallback(std::bind(&ChatServer::onConnect,this,std::placeholders::_1));

        _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));

        _server.setThreadNum(4);
    }

    void start()
    {
        _server.start();
    }
    private:

    void onConnect(const TcpConnectionPtr&con)
    {
        if(con->connected())
        {
          cout<<"peer:"<<con->peerAddress().toIpPort()<<endl;
        }

        else
        {
          cout<<"peer:"<<con->peerAddress().toIpPort()<<"断开连接了"<<endl;

        }
    }

    void onMessage(const TcpConnectionPtr&con,Buffer*buf,Timestamp time)
    {
      string _buf=buf->retrieveAllAsString();
      cout<<"_buf:"<<_buf<<endl;
      cout<<"Time:"<<time.toString()<<endl;
    
      con->send(_buf.c_str(),_buf.size());
    }

    TcpServer _server;
    EventLoop*_loop;
};

int main()
{
  EventLoop loop;
  InetAddress addr("172.16.47.167",8080);

  ChatServer _s(&loop,addr,"hello");
  _s.start();
  loop.loop();
}
