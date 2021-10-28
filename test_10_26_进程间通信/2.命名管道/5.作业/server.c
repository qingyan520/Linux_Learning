#include"com.h"

int main()
{
  if(mkfifo(FILE_NAME,0644)<0)
  {
    perror("mkfifo");
    return 1;
  }
  int fd=open(FILE_NAME,O_RDONLY);
  if(fd<0)
  {
    perror("open");
  }
  char buf[128];
  while(1)
  {
    ssize_t s=read(fd,buf,128);
    if(s>0)
    {
      buf[s]='\0';
      printf("server# %s\n",buf);

    }
    else if(s==0)
    {
      printf("server quit!\n");
      break;
    }
    else 
    {
      printf("read fail!\n");
    }
  }
  return 0;
}
