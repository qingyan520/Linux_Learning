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
  int listen_sock=

}
