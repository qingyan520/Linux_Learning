#include"UdpServer.hpp"
int main(int argc ,char*argv[]){
  UdpServer*usr=new UdpServer(atoi(argv[1]));
  usr->InitUdpServer();
  usr->Start();
  return 0;
}
