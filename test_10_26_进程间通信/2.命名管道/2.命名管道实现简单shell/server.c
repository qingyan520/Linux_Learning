#include"com.h"

int main()
{
  if(mkfifo(FILE_NAME,0644)<0)
  {
    perror("mkfifo!\n");
    return 1;
  }
  int fd=open(FILE_NAME,O_RDONLY);
  if(fd<0)
  {
    perror("fd!\n");
    return 2;
  }
  char buf[64];
  while(1)
  {
      ssize_t s=read(fd,buf,64);
      if(s>0)
      {
          buf[s]='\0';
          printf("server# %s\n",buf);
          if(fork()==0)
          {
            //子进程进程进程替换
            execlp(buf,buf,NULL);
            sleep(1);
            exit(-1);
          }
          waitpid(fd,NULL,0);
      }
      else if(s==0)
      {
        printf("server quit!\n");
        break;
      }
      else 
      {
        break;
      }
  }
  return 0;
}
