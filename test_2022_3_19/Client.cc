#include<iostream>
#include<cstring>
#include<string>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;


int main(int argc,char*argv[])
{
  if(argc!=3)
  {
    cerr<<"Usage!"<<endl;
    exit(0);
  }

  int sockfd=socket(AF_INET,SOCK_STREAM,0);
  if(sockfd<0)
  {
    cerr<<"socket error!"<<endl;
    exit(1);
  }
  
  int port=atoi(argv[2]);
  string ip=argv[1];
  struct sockaddr_in peer;
  peer.sin_family=AF_INET;
  peer.sin_port=htons(port);
  peer.sin_addr.s_addr=inet_addr(ip.c_str());
  if(connect(sockfd,(struct sockaddr*)&peer,sizeof(peer))<0)
  {
    cerr<<"Connect error!"<<endl;
    exit(2);
  }

  while(1)
  {
    string msg;
    cout<<"Please Enter#";
    cin>>msg;
    int ret=send(sockfd,msg.c_str(),msg.size(),0);
    if(ret<0)
    {
      cerr<<"send error!"<<endl;
    }
    char buf[1024]={0};
    ret=recv(sockfd,buf,sizeof(buf),0);
  
    if(ret>=0)
    {
      cout<<"Server send to :"<<buf<<endl;
    }

  }
}
