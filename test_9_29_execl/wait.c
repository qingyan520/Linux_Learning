#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
    pid_t id=fork();
    if(id==0)
    {
        printf("I am child porcess\n");
        sleep(5);
        exit(10);
    }
    else
    {
        int status=0;
        pid_t ret=wait(&status);
        if(ret>0)
        {
            printf("child wait success:\n");
            printf("child exit code:%d\n",(status>>8)&0XFF);
            printf("child exit signal:%d\n",status&0x7F);
            sleep(10);
        }
    }
    return 0;
}
