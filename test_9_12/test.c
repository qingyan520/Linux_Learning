//僵尸进程
//子进程先于父进程退出，其信息会被继续保存在task_struct中，被称为僵尸进程
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int main()
{
  //利用fork创建子进程，fork函数的返回值类型为pid_t，它会给父进程返回pid，给子进程返回0
  //父子进程哪个先执行不确定
  pid_t id=fork();
  if(id==0)
  {
    // int count=5;
    while(1)
    {
      printf("子进程，pid:%d,ppid:%d\n",getpid(),getppid());
      sleep(1);
    }
  }
  else if(id>0)
  {
    int count=5;
    while(count)
    {
      printf("父进程,pid:%d,ppid:%d,count:%d\n",getpid(),getppid(),count--);
      sleep(1);
    }
    printf("父进程退出\n");
    exit(-1);
  }
  else 
  {
    printf("fork fail\n");
  }
  return 0;
}
