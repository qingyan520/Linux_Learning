#include"com.h"
#include<string.h>
int main()
{
  int fd=open(FILE_NAME,O_WRONLY);
  if(fd<0)
  {
    perror("open!\n");
    return 1;
  }
  char buf[64];
  while(1)
  {
    printf("Please Enter# ");
    fflush(stdout);
    ssize_t s=read(0,buf,64);
    if(s>0)
    {
      buf[s-1]='\0';
      write(fd,buf,strlen(buf));
    }
  }
  return 0;
}
