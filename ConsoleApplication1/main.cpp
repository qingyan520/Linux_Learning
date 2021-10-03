#include <stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<stdlib.h>
int main()
{
    pid_t id = fork();
    if (id == 0)
    {
        int count = 0;
        while (count < 5)
        {
            printf("I am child,pid:%d,ppid:%d\n", getpid(), getppid());
            count++;
            sleep(1);
        }
        exit(10);
    }
    else
    {
        int status = 0;
        pid_t ret = waitpid(id, &status, 0);
        if (ret >= 0)
        {
            printf("child wait success,ret=%d\n", ret);

            if (WIFEXITED(status))
            {
                printf("child exit success,child exit code:%d\n", WEXITSTATUS(status));
            }
            else
            {
                printf("child exit not normal,child exit signal:%d\n", status & 0X7F);
            }

        }
        else
        {
            printf("child wait not success\n");
        }
        printf("Father is running\n");
        sleep(10);
    }
    return 0;
}