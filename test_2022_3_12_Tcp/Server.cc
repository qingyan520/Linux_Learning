#include<iostream>
#include<string>
#include<cstring>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/wait.h>
#include<fstream>
using namespace std;
int main(int argc,char*argv[]){
  if(argc!=2){
    cout<<"Usage!"<<endl;
    exit(0);
  }
  int port=atoi(argv[1]);
  int listen_sock=socket(AF_INET,SOCK_STREAM,0);
  if(listen_sock<0){
    cerr<<"socket error!"<<endl;
    exit(1);
  }
  struct sockaddr_in local;
  local.sin_family=AF_INET;
  local.sin_port=htons(port);
  local.sin_addr.s_addr=INADDR_ANY;
  
  if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local))<0){
    cerr<<"bind error1"<<endl;
    exit(2);
  }

  if(listen(listen_sock,5)<0){
    cout<<"listen error!"<<endl;
    exit(3);
  }

  struct sockaddr_in peer;
  socklen_t len=sizeof(peer);
  for(;;){
    int sock=accept(listen_sock,(struct sockaddr*)&peer,&len);
    if(sock<0){
      cout<<"accept error!"<<endl;
      continue;
    }

    if(fork()==0){
      if(fork()>0){
        exit(0);
      }
      close(listen_sock);
      std::ifstream t;
      int length=1;
      t.open("index.html");
      t.seekg(0,std::ios::end);
      length=t.tellg();
      t.seekg(0,std::ios::beg);
      char*buffer=new char[length];
      t.read(buffer,length);
      string status_line="http/1.0 200 ok\n";
      string response_header="Content-Length"+std::to_string(len);
      response_header+="\n";
      string blank="\n";
      send(sock,status_line.c_str(),status_line.size(),0);
      send(sock,response_header.c_str(),response_header.size(),0);
      send(sock,blank.c_str(),blank.size(),0);
      send(sock,buffer,length,0);
      exit(0);
    }
    close(sock);
    waitpid(-1,nullptr,0);

  }
  return 0;
}
