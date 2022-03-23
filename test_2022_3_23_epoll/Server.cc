#include<iostream>
#include<string>
#include<cstring>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/epoll.h>
#include<unistd.h>
using std::cout;
using std::endl;
using std::cerr;

int main(int argc,char*argv[])
{
  if(argc!=2)
  {
    cerr<<"Usage!"<<endl;
    exit(0);
  }
  int listen_socket=socket(AF_INET,SOCK_STREAM,0);
  if(listen_socket<0)
  {
    cerr<<"listen error!"<<endl;
    exit(1);
  }

  int port=atoi(argv[1]);

  struct sockaddr_in local;
  local.sin_family=AF_INET;
  local.sin_port=htons(atoi(argv[1]));
  local.sin_addr.s_addr=INADDR_ANY;
  cout<<atoi(argv[1])<<endl;
  cout<<htons(atoi(argv[1]))<<endl;
  int flags=1;
  setsockopt(listen_socket,SOL_SOCKET,SO_REUSEADDR,&flags,sizeof(flags));

  if(bind(listen_socket,(struct sockaddr*)&local,sizeof(local))<0)
  {
    cerr<<"bind error!"<<endl;
    exit(3);
  }
  
  if(listen(listen_socket,6)<0)
  {
    cerr<<"listen error!"<<endl;
    exit(0);
  }

  struct sockaddr_in peer;
  socklen_t len=sizeof(peer);

  int epfd=epoll_create(2000);

  struct epoll_event all[3000];
  struct epoll_event ev;
  ev.events=EPOLLIN;
  ev.data.fd=listen_socket;

  epoll_ctl(epfd,EPOLL_CTL_ADD,listen_socket,&ev);
  while(1)
  {
    int ret=epoll_wait(epfd,all,3000,-1);
    
    for(int i=0;i<ret;i++)
    {
      int fd=all[i].data.fd;

      if(fd==listen_socket)
      {
        int cfd=accept(listen_socket,(struct sockaddr*)&peer,&len);

        ev.data.fd=cfd;
        epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);
      }
      else
      {
        char buf[1024]={0};
        int l=recv(fd,buf,1024,0);
        if(l==0)
        {
          epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
          close(fd);
        }
        else
        {
          cout<<buf<<endl;
        }
      }
    }


  }


}
