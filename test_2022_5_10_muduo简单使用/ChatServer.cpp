#include<iostream>
#include<string>
#include<muduo/net/TcpServer.h>
#include<muduo/net/EventLoop.h>

using namespace std;
using namespace muduo;
using namespace muduo::net;

class ChatServer
{
  public:
    ChatServer(EventLoop*loop,const InetAddress&addr,string name):
      _server(loop,addr,name),
      _loop(loop)
  {

  }
    ~ChatServer()
    {

    }
  
    void start()
    {
      //注册连接与断开连接时的回调
      
      _server.setConnectionCallback(std::bind(&ChatServer::onConnection,this,std::placeholders::_1));
      //注册事件处理的回调
     
      _server.setMessageCallback(std::bind(&ChatServer::onMessage,this,std::placeholders::_1,std::placeholders::_2,std::placeholders::_3));

      //指定线程数量
      _server.setThreadNum(5);
    }

  private:
    void onConnection(const TcpConnectionPtr&con)
    {
      if(con->connected())
      {
        cout<<"peer:"<<con->peerAddress().toIpPort()<<"->"<<con->localAddress().toIpPort()<<" "<<"stat:"<<"Online"<<endl;
      }
      else
      {
        
        cout<<"peer:"<<con->peerAddress().toIpPort()<<"->"<<con->localAddress().toIpPort()<<" "<<"stat:"<<"Offline"<<endl;
        con->shutdown();
      }
    }


    void onMessage(const TcpConnectionPtr&con,Buffer*buf,Timestamp time)
    {
      string Buf=buf->retrieveAllAsString();
      cout<<time.toString()<<":"<<Buf<<endl;
      con->send(Buf.c_str(),Buf.size());
    }

    TcpServer _server;
    muduo::net::EventLoop*_loop;
};

int main()
{
  EventLoop loop;
  muduo::net::InetAddress local(8080);
  cout<<1<<endl;
 ChatServer _s(&loop,local,"hello");
 cout<<2<<endl;
  _s.start();
  cout<<3<<endl;

  loop.loop();
  cout<<4<<endl;
  return 0;
}
