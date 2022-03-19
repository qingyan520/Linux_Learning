#include<iostream>
#include<string.h>
#include<cstring>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;

int main(int argc,char*argv[])
{
  if(argc!=2)
  {
    cout<<"Usage!"<<endl;
    exit(0);
  }

  int port=atoi(argv[1]);
  int listen_sock=socket(AF_INET,SOCK_STREAM,0);
  if(listen_sock<0)
  {
    exit(0);
  }

  //int flag=0;
  //setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,&flag,flag);

  struct sockaddr_in local;
  local.sin_family=AF_INET;
  local.sin_port=htons(port);
  local.sin_addr.s_addr=INADDR_ANY;

  if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local))<0)
  {
    cerr<<"bind error!"<<endl;
    exit(1);
  }
  
  if(listen(listen_sock,10)<0)
  {
    cerr<<"listen error!"<<endl;
    exit(2);
  }


  struct sockaddr_in peer;
  socklen_t len=sizeof(peer);
  int fd=accept(listen_sock,(struct sockaddr*)&peer,&len);
  while(1)
  {
    //int fd=accept(listen_sock,(struct sockaddr*)&peer,&len);
    
    if(fd<0)
    {
      cerr<<"accept error!"<<endl;
      continue;
    }
    char buf[1024]={0};

    int size=recv(fd,buf,sizeof(buf),0);
    
    int port=ntohs(peer.sin_port);
    string ip=inet_ntoa(peer.sin_addr);

    if(size<0)
    {
      cout<<"recv error!"<<endl;
      exit(3);
    }
    else if(size>0)
    {
      buf[size]=0;
      cout<<ip<<":"<<port<<" send to:"<<buf<<endl;
      string msg;
      cout<<"Please Enter#";
      cin>>msg;
      int ret=send(fd,msg.c_str(),msg.size(),0);
      if(ret<0)
      {
        cerr<<"send error!"<<endl;
      }
    }
  
  }
}
