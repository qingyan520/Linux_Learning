#include"com.h"

int main()
{
  int fd=open(FILE_NAME,O_WRONLY);
  if(fd<0)
  {
    perror("open fail!\n");
    return 1;
  }
  int in=open("file.txt",O_RDONLY);
  if(in<0)
  {
    perror("open!\n");
    return 2;
  }
  char buf[128];
  while(1)
  {
    dup2(in,0);
    ssize_t s=read(0,buf,128);
    if(s>0)
    {
      write(fd,buf,sizeof(buf));
    }
    else if(s==0)
    {
      printf("client quit!\n");
      break;
    }
    else
    {
      printf("read fail\n");
      break;
    }
  }
  return 0;

}
