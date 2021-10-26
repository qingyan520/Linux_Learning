#include"com.h"
#include<string.h>
int main()
{
  int fd=open(FILE_NAME,O_WRONLY);
  if(fd<0)
  {
    perror("fd");
    return 1;
  }
  char buf[1024];
  while(1)
  {
    //buf[0]='\0';
    printf("Please Enter# ");
    fflush(stdout);
    ssize_t s=read(0,buf,sizeof(buf));
    if(s>0)
    {
     buf[s-1]='\0';
      write(fd,buf,strlen(buf));
    }
  }
  close(fd);
  return 0;

}
