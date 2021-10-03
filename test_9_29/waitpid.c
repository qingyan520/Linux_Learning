#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
    pid_t id=fork();
    if(id==0)
    {
        int count=0;
        while(count<20)
        {
            printf("I am child,pid:%d,ppid:%d\n",getpid(),getppid());
            sleep(1);
            count++;
        }
        exit(10);
    }
    else
    {
        int status=0;
      //  pid_t ret=waitpid(id,&status,0);
      //  if(ret>=0)
      //  {
      //      printf("child exit success,ret:%d\n",ret);
      //      printf("child exit code:%d\n",(status>>8)&0xFF);
      //      printf("child exit signal:%d\n",status&0x7F);
      //  }
      //
        pid_t ret=waitpid(id,&status,0);
        if(WIFEXITED(status))
        {
            printf("child exit success\n");
            printf("child exit code:%d\n",WEXITSTATUS(status));
        }
        else
        {
            printf("child exit signal:%d\n",status&0x7F);
        }
        printf("Father is running\n");
        sleep(5);
    }
    return 0;
}
