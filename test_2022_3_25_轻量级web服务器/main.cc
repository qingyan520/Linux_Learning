# include"Server.hpp"

int main(int argc,char*argv[])
{
  char*path="/home/sjw/Linux_Learning/test_2022_3_25_轻量级web服务器/Source";

  if(argc==1)
  {
    cout<<"./a.out port path!"<<endl;
    exit(0);
  } 
  if(argc==3)
  {
    path=argv[2];
  }
  int port=atoi(argv[1]);
  chdir(path);
  epoll_run(port);
  return 0;
}
