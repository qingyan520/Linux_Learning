#include<iostream>
#include<string>
#include<cstring>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using std::endl;
using std::cout;
using std::cerr;
using std::cin;
int main(int argc,char*argv[])
{
  if(argc!=3)
  {
    cerr<<"Usage!"<<endl;
    exit(0);
  }
  int fd=socket(AF_INET,SOCK_STREAM,0);
  if(fd<0)
  {
    cerr<<"socket error!"<<endl;
    exit(1);
  }

  std::string ip=argv[1];
  int port=atoi(argv[2]);
  struct sockaddr_in server;
  server.sin_family=AF_INET;
  server.sin_port=htons(port);
  server.sin_addr.s_addr=inet_addr(ip.c_str());
  
  if(connect(fd,(struct sockaddr*)&server,sizeof(server))<0)
  {
    cerr<<"Connect error!"<<endl;
  } 
  
  while(1)
  {
    std::string msg;
    cout<<"Please Enter#";
    cin>>msg;
    send(fd,msg.c_str(),msg.size(),0);

  }

}
