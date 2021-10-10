#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
int main()
{
  int fd=open("log.txt",O_WRONLY|O_CREAT,0666);
  if(ret<0)
  {
    perror("open fail\n");
  }
  int count=5;
  char*str="hello world\n";
  while(count--)
  {
    write(str,sizeof(str),fd);
  }
  

  return 0;
}
