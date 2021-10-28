#include"com.h"

int main()
{
  int fd=open(FILE_NAME,O_WRONLY);
  if(fd<0)
  {
    perror("open!");
    return 1;
  }
  char buf[64];
  while(1)
  {
      printf("client# ");
      fflush(stdout);
      ssize_t s=read(0,buf,64);
      if(s>0)
      {
        buf[s-1]='\0';
        write(fd,buf,strlen(buf));
      }
      else if(s==0)
      {
        printf("client quit!\n");
        break;
      }
      else
      {
        break;
      }
  }
  return 0;
}
