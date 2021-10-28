#include"com.h"

int main()
{
  if(mkfifo(FILE_NAME,0644)<0)
  {
    perror("mkfifo!\n");
    return 1;
  }
  int fd=open(FILE_NAME,O_RDONLY);
  int out=open("file-bat.txt",O_CREAT|O_WRONLY,0644);
  char buf[128];
  while(1)
  {
    ssize_t s=read(fd,buf,128);
    if(s>0)
    {
      buf[s]='\0';
      printf("server# %s\n",buf);
      write(out,buf,strlen(buf));
    }
    else if(s==0)
    {
      printf("server quit!\n");
      break;
    }
    else
    {
      printf("read error!\n");
      break;
    }
  }
}
