#include"com.h"

int main()
{
  int fd=open(FILE_NAME,O_WRONLY);
  if(fd<0)
  {
    perror("open!\n");
    return 0;
  }
  char buf[128];
  while(1)
  {
    ssize_t s=read(0,buf,128);
    if(s>0)
    {
      buf[s-1]='\0';
      write(fd,buf,strlen(buf));
    }
  }
  return 0;
}
