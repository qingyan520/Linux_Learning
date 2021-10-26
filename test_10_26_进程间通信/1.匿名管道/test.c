#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<string.h>
int main()
{
  int fd[2];
  if(pipe(fd)<0)
  {
    perror("pipe fail!\n");
  }
  int id=fork();
  if(id==0)
  {
    //子进程负责写东西，父进程负责读东西
    close(fd[0]);
    //char buf[]="hello world\n";
    char buf[1024];    
    int count=5;
    while(count--)
    {
      //char buf[1024];
     ssize_t s=read(0,buf,1024);
      buf[s-1]='\n';
      write(fd[1],buf,strlen(buf));
      sleep(1);
    }
    exit(-1);
  }
  else
  {
    //父进程负责读东西
    close(fd[1]);
    char buf[1024];
    while(1)
    {
      //buf[0]=0;
      ssize_t s=read(fd[0],buf,1024);
      if(s>0)
      {
        buf[s]='\0';
        printf("child send to father# %s",buf);
        fflush(stdout);

      }
      else
      {
        break;
      }
    }
    waitpid(id,NULL,0);
    exit(-1);
  }
  return 0;

}
