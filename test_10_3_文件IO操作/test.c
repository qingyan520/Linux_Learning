//1.cÓïÑÔ¶ÁÐ´ÎÄ¼þ
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
#define Len 1021
int main()
{
    char cmd[Len];
    while (1)
    {
        printf("sjw@iZ2zedu4njy79sqivntvprZ test_9_29_execl$");
        fgets(cmd, Len, stdin);
        cmd[strlen(cmd) - 1] = '\0';
        char* myargv[Len];
        //ÇÐ¸î×Ö·û´®:
        pid_t id = fork();
        int i = 1;
        myargv[0] = strtok(cmd, " ");
        while (myargv[i] = strtok(NULL, " "))
        {
            i++;
        }
        if (id == 0)
        {
            execv("/usr/bin/ls", myargv);
            exit(1);
        }
        else
        {
            int status = 0;
            pid_t ret = waitpid(id, &status, 0);
            if(WIFEXITED(status))
            {
                printf("exit code:%d\n", WEXITSTATUS(status));
            }
        }

    }
    return 0;
}