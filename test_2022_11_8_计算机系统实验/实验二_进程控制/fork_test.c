#include <unistd.h>
#include <stdio.h>
#include<sys/wait.h>
int main(void)
{
  
        pid_t fpid;
        int count = 0;
    // 调用fork函数，复制进程
        fpid = fork();
    // 若返回值小于0，则表示复制出错
        if(fpid < 0){
                printf("error in fork!");
        }
    // 若返回值等于0，则表示此时为复制出的子进程在运行
        else if(fpid == 0){
                printf("I am the child process, my PID is %d\n",getpid());
                count++;
        }
    // 若返回值大于0，则表示此时为源进程（父进程）在运行
        else{
                int ret=wait(NULL);     
                printf("I am the parent process, my PID is %d\n",getpid());
                count++;
              
        }
        printf("count: %d\n",count);
        return 0;
}

