#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
  pid_t id=fork();
  if(id==0)
  {
    while(1)
    {
      printf("I am child, pid:%d, ppid:%d\n",getpid(),getppid());
    sleep(1);
    }
  }
  else if(id>0)
  {
    int count=5;
    while(count)
    {
      printf("I am father,pid:%d, ppid:%d,count:%d\n",getpid(),getppid(),count--);
      sleep(1);
    }
    printf("father exit()......\n");
    exit(-1);
  }
  else
  {
    printf("fork fail\n");
  }
  return 0;
}
