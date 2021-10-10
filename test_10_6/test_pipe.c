//进程间通信：管道
//管道实际上是一块内核缓冲区，他有读和写两个接口
//两个只能由父进程进行读，子进程负责写或者子进程负责负责读，父进程负责写，称作半双工通信
//int fd[2]:创建文件描述符，用来判断读写两端，其中fd[0]表示读端，fd[1]表示写端
//write(fd[1],"hello",5)
//read(fd[0],buf,sizeof(buf))
//
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
int main()
{
  int fd[2];
  pipe(fd);
  pid_t id=fork();
  if(id==0)
  {
    close(fd[0]);
    sleep(3);
    write(fd[1],"hello father,I am child",sizeof("hello father,I am child"));
    printf("son is email for father\n");
    printf("---------------------------------------------\n");
    sleep(2);
    
  }
  else if(id>0)
  {
      close(fd[1]);
      char buf[1024]="\0";
      ssize_t ret=read(fd[0],buf,sizeof(buf));
      printf("%s\n",buf);
      sleep(3);

  }

  return 0;
}
