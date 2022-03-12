#include<iostream>
#include<string>
#include<cstring>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
using namespace std;

class UdpServer{
  private:
    int port;
    int sockfd;
  public:
    UdpServer(int _port=8081):port(_port),sockfd(-1){

    }
    ~UdpServer(){

    }

    void InitUdpServer(){
      sockfd=socket(AF_INET,SOCK_DGRAM,0);
      if(sockfd<0){
        cerr<<"socket error!"<<endl;
        exit(0);
      }
      struct sockaddr_in local;
      memset(&local,0,sizeof(local));
      local.sin_family=AF_INET;
      local.sin_port=htons(port);
      local.sin_addr.s_addr=INADDR_ANY;

      if(bind(sockfd,(struct sockaddr*)&local,sizeof(local))<0){
        cerr<<"bind error!"<<endl;
        exit(2);
      }
    
      cout<<"bind success!"<<endl;
    }

    void Start(){
      char buf[1024]={0};
      struct sockaddr_in peer;
      memset(&peer,0,sizeof(peer));
      socklen_t len=sizeof(peer);
      for(;;){
        cout<<"hehe"<<endl;
        ssize_t size=recvfrom(sockfd,buf,sizeof(buf)-1,0,(struct sockaddr*)&peer,&len);
        if(size>0)
        {
          buf[size]=0;
          string _ip=inet_ntoa(peer.sin_addr);
          int _port=ntohs(peer.sin_port);
          cout<<_ip<<":"<<_port<<"#"<<buf<<endl;
        }
        else{
          cout<<"recvfrom error"<<endl;
        }

      }

    }
};

