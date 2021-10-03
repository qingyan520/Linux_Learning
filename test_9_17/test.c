#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(int argc, char*argv[],char*envp[])
{
    printf("%s",getenv("PATH"));
    return 0;
}
