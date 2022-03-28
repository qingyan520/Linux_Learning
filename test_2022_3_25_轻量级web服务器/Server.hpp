#include<iostream>
#include<string>
#include<cstring>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<dirent.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<ctime>
#include<cstdlib>
using std::cerr;
using std::cout;
using std::endl;
using std::cin;
using std::string;

#define  PATH "/home/sjw/Linux_Learning/test_2022_3_25_轻量级web服务器/Source"
#define MAXSIZE 3000
#define INFO 1
#define WARNING 2
#define ERROR 3
#define FALTAL 4
#define LOG(levle,message) Log(#levle,message,__FILE__,__LINE__)




void Log(string level,string message,string filename,int line)
{
  char now[64]={0};
  time_t tt;
  struct tm*ttime;
  tt=time(nullptr);
  ttime=localtime(&tt);
  strftime(now,64, "%Y-%m-%d %H:%M:%S", ttime);

  cout<<"["<<level<<"]"<<"["<<message<<"]"<<"["<<now<<"]"<<"["<<filename<<"]"<<"["<<line<<"]"<<endl;
}



int create_socket(int port);

//接受套接字，将套接字添加到epoll树上去
void do_accept(int fd,int epfd);

//读取数据 
void do_read(int fd,int epfd);

//断开连接
void disconnect(int fd,int epfd);

//处理http请求
void http_request(char*http,int fd);

//拼接响应报头，然后发送响应报头
void respond_header(int fd,int value,const char*desp,const char*type,long len);     
 //发送文件的函数
void send_file(int cfd,const char*filename);                                             

//判断文件类型，然后告诉响应报头                                                                                                                                                         
string determine_type(string &path);


//主函数
void epoll_run(int port)
{
  //创建套接字，返回lfd,将lfd挂载到epoll树上
  int lfd=create_socket(port);
  //将lfd设置为非阻塞状态
  
  
  int epfd=epoll_create(3000);
  struct epoll_event ev;
  ev.events=EPOLLIN;
  ev.data.fd=lfd;
  int _ret=epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&ev);
  if(_ret<0)
  {
    LOG(FALTAL,"epoll_ctl error");
    exit(0);
  }
  
  struct epoll_event all[MAXSIZE];


  while(1)
  {
    //委托内核进行轮询检测
    int ret=epoll_wait(epfd,all,MAXSIZE,-1);
    
    if(ret<0)
    {
      cerr<<"epoll_wait error!"<<endl;
      exit(1);
    }

    //循环进行检测
    for(int i=0;i<ret;i++)
    {
      int fd=all[i].data.fd;
      
      if(all[i].events!=EPOLLIN)
      {
        continue;
      }
      //用于新连接的fd
      if(fd==lfd)
      {
        //此时进行accpet,接受返回的套接字，添加到epoll树中去
        do_accept(fd,epfd);
        continue;    
      } 
      else
      { 
        //这时就是用于读取数据的fd,接受数据，对数据进行解析，分析http报头，发送对应的请求
        do_read(fd,epfd);        
      }

    }
  }
}

//读取数据
void do_read(int fd,int epfd)
{
  //读取http请求
  struct sockaddr_in peer;
  socklen_t len=sizeof(peer);
  char buf[102400]={0};
  
  int ret=recvfrom(fd,buf,102400,0,(struct sockaddr*)&peer,&len);
  if(ret<0)
  {
    cerr<<"recvfrom error!"<<endl;
    exit(0);
  }
  else if(ret==0)
  {
    //代表客户端断开了连接，之后将fd从epoll树中去除
    disconnect(fd,epfd);
  }
  else
  {
    //读取数据成功,打印一下发送方以及发送的http请求协议
    string ip=inet_ntoa(peer.sin_addr);
    int port=ntohs(peer.sin_port);
    cout<<"=========================Client "<<ip<<" : "<<port<<" Send a Http Request!================================"<<endl;

    cout<<buf<<endl;
    cout<<"=================================================End===================================================="<<endl;
    

    //成功读取了http响应之后，就需要解析http指令，找出第一行，然后找到要访问路径,然后发送消息
    http_request(buf,fd); 
  }
}



//处理http请求
void http_request(char*http,int fd)
{
  int n=strlen(http);
  int i=0;
  string methd;
  string path;

  //解析访问路径
  while(i<n&&http[i]!=' ')
  {
    methd.push_back(http[i]);
    i++;
  }
  ++i;
  if(http[i]=='/'&&http[i+1]!=' ')
  {
    ++i;
    while(i<n&&http[i]!=' ')
    {
      path.push_back(http[i]);
      i++;
    }
  }
  
  //如果没有指定资源路径，那么就默认显示当前资源路径

  //
  struct stat st;
  int ret=0;
  
  if(path.size()==0)
  {
    ret=stat("index.html",&st);
    cout<<"输入的路径为:"<<"index.html"<<endl;
  }
  else
  {
    
    ret=stat(path.c_str(),&st);
    cout<<"输入的路径为:"<<path<<endl;
  }
  

  //如果没有找到该文件就发送404Not Found
 // if(ret==-1)
 // {
 //   //没有找到该路径文件就发送http_respond_head,404,Not Found;
 //   respond_header(fd,404,"Not Found",".text",-1);    
 // }

  if(S_ISREG(st.st_mode))
  {
    //判断文件类型然后让浏览器进行解析
    
    //respond_header(fd,200,"OK",".html",st.st_size());
    if(path.size()!=0)
    {
      string type=determine_type(path);
      respond_header(fd,200,"OK",type.c_str(),st.st_size);
      send_file(fd,path.c_str());
    }
    else
    {
      respond_header(fd,200,"OK",".html",st.st_size);
      send_file(fd,"index.html");
    }
    
  }
}


//判断文件类型，然后告诉响应报头
string determine_type(string &path)
{
  string str;
  size_t i=0;
  while(i<path.size()&&path[i]!='.')
  {
    i++;
  }

  while(i<path.size())
  {
    str.push_back(path[i]);
    i++;
  }

  if(str==".img")
  {
    return ".img";
  }
  else if(str==".css")
  {
    return ".css";
  }
  else if(str==".html")
  {
    return ".html";
  }
  else if(str==".jpg")
  {
    return ".jpg";
  }
  else if(str==".js")
  {
    return ".js";
  }
  else if(str==".json")
  {
    return ".json";
  }
  else if(str==".ttf")
  {
    return ".ttf";
  }

}


//发送文件的函数
void send_file(int cfd,const char*filename)
{
  int fd=open(filename,O_RDONLY);
  if(fd==-1)
  {
    respond_header(cfd,404,"Not Found",".text",-1); 
    return;
  }

  char buf[40960]={0};
  int len=0;

  while((len=read(fd,buf,sizeof(buf)))>0)
  {
   // 发送读出的数据
    send(cfd,buf,len,0);
  }
    
 // len=read(fd,buf,sizeof(buf));
 // if(len==0)
 // {
 //   return;
 // }

 // if(len>0)
 // {
  //  send(cfd,buf,sizeof(buf),0);
  //var}

  if(len==-1)
  {
    respond_header(fd,404,"Not Found",".text",-1);    
    return;
  }
  close(fd);

}


//拼接响应报头，然后发送响应报头
void respond_header(int fd,int value,const char*desp,const char*type,long len)
{
  //状态行拼接
	string buf="http/1.1 ";
  buf+=std::to_string(value);
  buf+=" ";
  buf+=desp;
  buf+="\r\n";
  send(fd,buf.c_str(),buf.size(),0);
  
  //响应报头拼接
  string buf_="Content-Type:";
  buf_+=type;
  buf_+="\r\n";
  send(fd,buf_.c_str(),buf_.size(),0);

  string _buf;
  _buf="Content-Length:";
  _buf+=std::to_string(len);
  _buf+="\r\n";
  send(fd,_buf.c_str(),_buf.size(),0);

  //发送空行
  string hello="\r\n";
  send(fd,hello.c_str(),hello.size(),0);

  cout<<"发送的响应报头如下:"<<endl;
  cout<<buf<<buf_<<_buf<<hello<<endl;
}





//断开连接
void disconnect(int fd,int epfd)
{
  int ret=epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
  if(ret<0)
  {
    cerr<<"disconnect error!"<<endl;
    exit(0);
  }
}




//接受套接字，将套接字添加到epoll树上去
void do_accept(int fd,int epfd)
{
  struct sockaddr_in peer;
  socklen_t len=sizeof(peer);
  int cfd=accept(fd,(struct sockaddr*)&peer,&len);
  
  if(cfd<0)
  {
    cerr<<"accept error!"<<endl;
    exit(0);
  }
  
  int port=ntohs(peer.sin_port);
  string ip=inet_ntoa(peer.sin_addr);

  cout<<"Clietn:"<<ip<<"#"<<port<<" accept!"<<endl;

  //设置cfd为非阻塞状态
  //int flag=fcntl(cfd,F_GETFL);
  //flag|=O_NONBLOCK;
  //fcntl(cfd,F_SETFL,flag);

  //将cfd添加到树上
  struct epoll_event ev;
  ev.data.fd=cfd;
  ev.events=EPOLLIN;

  int ret=epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&ev);
  if(ret<0)
  {
    cerr<<"do_accept epoll_ctl error!"<<endl;
    exit(1);
  }
  cout<<"accept end!!!"<<endl;
}





//创建套接字
int create_socket(int port)
{
  int lfd=socket(AF_INET,SOCK_STREAM,0);
  if(lfd<0)
  {
    cerr<<"socket error!"<<endl;
    exit(0);
  }

  struct sockaddr_in ser;
  ser.sin_family=AF_INET;
  ser.sin_port=htons(port);
  ser.sin_addr.s_addr=INADDR_ANY;

  //设置端口复用
  int flags=1;
  setsockopt(lfd,SOL_SOCKET,SO_REUSEADDR,&flags,sizeof(flags));


  //绑定
  if(bind(lfd,(struct sockaddr*)&ser,sizeof(ser))<0)
  {
    cerr<<"bind error!"<<endl;
    exit(1);
  }

  //监听
  if(listen(lfd,32)<0)
  {
    cerr<<"listen error!"<<endl;
    exit(2);
  }

  LOG(INFO,"create_socket success");
  return lfd;
}
