#include<iostream>
#include<string>
#include<cstring>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;

class UdpClient{
  private:
    string ip;
    int port;
    int sockfd;
  public:
    UdpClient(string _ip,int _port):ip(_ip),port(_port),sockfd(-1){

    }

    ~UdpClient(){

    }

    void InitUdpClient(){
      sockfd=socket(AF_INET,SOCK_DGRAM,0);
      if(sockfd<0){
        cout<<"socket error!"<<endl;
        exit(1);
      }
    }

    void Start(){
      struct sockaddr_in peer;
      peer.sin_family=AF_INET;
      peer.sin_port=htons(port);
      peer.sin_addr.s_addr=inet_addr(ip.c_str());
      string msg;
      for(;;){
        cout<<"Please Enter#";
        cin>>msg;
        sendto(sockfd,msg.c_str(),msg.size(),0,(struct sockaddr*)&peer,sizeof(peer));
        
    

      } 
    }
};
