#include"UdpClient.hpp"

int main(int argc,char*argv[]){
  if(argc!=3){
    cout<<"Usage!"<<endl;
    exit(0);
  }
  string ip=argv[1];
  int port=atoi(argv[1]);
  UdpClient*cl=new UdpClient(ip,port);
  cl->InitUdpClient();
  cl->Start();
  return 0;
}
