#include"com.h"

int main()
{
  if(mkfifo(FILE_NAME,0644)<0)
  {
    perror("mkfifo");
    exit(1);
  }
  int fd=open(FILE_NAME,O_RDONLY);
  if(fd<0)
  {
    perror("open");
  }
  char buf[1024];
  while(1)
  {
    ssize_t s=read(fd,buf,sizeof(buf)-1);
    if(s>0)
    {
      buf[s]='\0';
      printf("client send to sever# %s\n",buf);
    }
    else if(s==0)
    {
      printf("client quit!\n");
      break;
    }
    else
    {
      printf("read fail!\n");
      break;
    }
  }
  return 0;
}
