//exec系列函数可是实现进程替换，如果我们实现多进程，那么可以让子进程利用exec系列函数执行另外一个函数
//使另外一个程序的数据和代码覆盖该子进程，实现进程的替换
#include<stdio.h>
#include<unistd.h>
int main()
{
    printf("hello -------------------------------------\n");
    execl("/usr/bin/ls","ls","-a","-l","-ll",NULL);
    printf("End----------------------------------------\n");
    return 0;
}
