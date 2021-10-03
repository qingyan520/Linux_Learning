#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
int main()
{
    pid_t id=fork();
    char*argv[]={"ls","-a","-l",NULL};
    if(id==0)
    {
        execvp("ls",argv);
        sleep(5);
        exit(13);
    }
    else
    {
        int status=0;
        pid_t ret=waitpid(id,&status,0);
        if(WIFEXITED(status))
        {
            printf("child exit code:%d\n",WEXITSTATUS(status));
            printf("Father is running\n");
            sleep(5);
        }
    }
    return 0;
}
