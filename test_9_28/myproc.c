#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
    pid_t id=fork();
    if(id==0)
    {
        int count=0;
        while(count<5)
        {
            printf("I am child ,pid:%d,ppid:%d\n",getpid(),getppid());
            sleep(1);
            count++;
        }
        exit(1);
    }
    else
    {
        int ret=wait(NULL);
        if(ret>=0)
        {
            printf("child exit success:pid:%d\n",ret);
        }
        printf("Father is running\n");
        sleep(5);

    }
    return 0;
}
