#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
int main()
{
    execlp("ls","ls","-al",NULL);
    return 0;
}
