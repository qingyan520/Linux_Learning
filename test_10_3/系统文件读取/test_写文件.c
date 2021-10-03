#include<stdio.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
int main()
{
  umask(0);
  int fd=open("myfile",O_WRONLY|O_CREAT,0644);
  if(fd<0)
  {
    perror("open");
    return 1;
  }
  int count=5;
  const char*msg="hello bit!\n";
  int len=strlen(msg);
  while(count--)
  {
    write(fd,msg,len);
  }
  close(fd);
  return 0;
}
