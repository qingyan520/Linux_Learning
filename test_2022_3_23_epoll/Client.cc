#include<iostream>
#include<string>
#include<cstring>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>

using std::cerr;
using std::endl;
using std::cout;

int main(int argc,char*argv[])
{
  if(argc!=3)
  {
    cerr<<"Usage!"<<endl;
    exit(0);
  }
  int lfd=socket(AF_INET,SOCK_STREAM,0);

  if(lfd<0)
  {
    cerr<<"socket error!"<<endl;
    exit(1);
  }

  struct sockaddr_in server;
  server.sin_family=AF_INET;
  server.sin_port=htons(atoi(argv[2]));
  server.sin_addr.s_addr=inet_addr(argv[1]);
  
  if(connect(lfd,(struct sockaddr*)&server,sizeof(server))<0)
  {
    cerr<<"connect error!"<<endl;
    exit(2);
  }

  while(1)
  {
    std::string msg;
    cout<<"Please Enter#";
    std::cin>>msg;
    send(lfd,msg.c_str(),msg.size(),0);
  }
}
