#include<iostream>
#include<string>
#include<cstring>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
using std::endl;
using std::cout;
using std::cerr;
int main(int argc,char*argv[])
{
  if(argc!=2)
  {
    cerr<<"Usage!"<<endl;
    exit(0);
  }
  
  int lfd=socket(AF_INET,SOCK_STREAM,0);
  if(lfd<0)
  {
    cout<<"socket error!"<<endl;
    exit(0);
  }
  
  int port=atoi(argv[1]);
  struct sockaddr_in local;
  local.sin_family=AF_INET;
  local.sin_port=htons(port);
  local.sin_addr.s_addr=INADDR_ANY;

  if(bind(lfd,(struct sockaddr*)&local,sizeof(local))<0)
  {
    cout<<"bind error!"<<endl;
    exit(1);
  }

  if(listen(lfd,12)<0)
  {
    cout<<"listen error!"<<endl;
    exit(2);
  }

  //创建set
  fd_set read,temp;
  FD_ZERO(&read);
  FD_ZERO(&temp);
  FD_SET(lfd,&read);
  int maxfd=lfd;
  struct sockaddr_in peer;
  socklen_t len=sizeof(peer);
  while(1)
  {
    temp=read;
    int fd=select(maxfd+1,&temp,NULL,NULL,NULL);
    if(fd<0)
    {
      cout<<"Select error!"<<endl;
      exit(3);
    }
    if(FD_ISSET(lfd,&temp))
    {

      int size=accept(maxfd+1,(struct sockaddr*)&peer,&len);
      if(size<0)
      {
        cout<<"accept error!"<<endl;
        exit(4);
      }
      FD_SET(size,&read);
      maxfd=maxfd>size? maxfd:size;
    }
    for(int i=lfd+1;i<=maxfd;i++)
    {
      if(FD_ISSET(i,&temp))
      {
        char buf[1024]={0};
        int len=recv(i,buf,sizeof(buf),0);
        if(len>0)
        {
          cout<<"Client Send To: "<<buf<<endl;
        }
        else if(len==0)
        {
          cout<<"Client Quit!"<<endl;
          FD_CLR(i,&read);
          close(i);
          if(maxfd==i)
          {
            maxfd--;
          }
        }
        else {
          cout<<"recv error!"<<endl;
          exit(4);
        }
      }
    }
  }
  
  
}

