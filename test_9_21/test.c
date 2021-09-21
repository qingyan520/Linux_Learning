#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>


//定义一个全局变量g_val
int g_val=100;

int main()
{
    pid_t id=fork();
    if(id==0)
    {
        g_val=200;
        printf("child::pid:%d,ppid:%d,g_val:%d,&g_val:%p\n",
                getpid(),getppid(),g_val,&g_val);
    }
    else
    {
        sleep(2);
        printf("father::pid:%d,ppid:%d,g_val:%d,&g_val:%p\n",
                getpid(),getppid(),g_val,&g_val);
    }
    sleep(1);


    return 0;
}
