#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
int main()
{
 // int fd=open("log.txt",O_RDONLY|O_CREAT,0666);
  //if(fd<0)
  //{
   // perror("open:");
  //}
  //int count=5;
 // char buf[1024];
 // while(count--)
  //{
   // ssize_t ret=read(fd,buf,1024);
    //printf("%s",buf);
  //}

  //输出重定向演示，让本来显示到屏幕上的东西显示到文件中
  //在这个例子中我们首先关闭1号文件描述符，本来一号文件描述符指向stdout,现在这个文件描述符指向了log.txt
  //那么本来应该写入到屏幕上的内容就显示到了文件中
  //close(1);
  //int fd=open("log.txt",O_WRONLY|O_CREAT,0666);
  //if(fd<0)
  //{
   // perror("open:");
  //}
  //printf("hello printf\n");
  //fprintf(stdout,"hello fprintf\n");
  //fputs("hello fputs\n",stdout);
  //fflush(stdout);



  //输出重定向演示：
  //stdin本来是从键盘中读取数据然后赋值，现在进行重定向让0号文件描述符指到log.txt中，那么原本通过键盘进行赋值的将通过读取文件log.txt进行赋值
 //umask(0);
 // close(0);
 //var int fd=open("log.txt",O_RDONLY|O_CREAT,0666);
 // if(fd<0)
  //{
  //  perror("open");
 //.autorelabel }
//  char buf[1024];
 // fgets(buf,1024,stdin);
 // printf("%s",buf);
 // fflush(stdout);
  //close(fd);
  //
  
  //追加重定向：向一个文件中追加东西
 // close(0);
 // int fd=open("log.txt",O_WRONLY|O_APPEND,0666);
 // if(fd<0)
 // {
 //   perror("open");
 // }

 // char buf[100];
 // fgets(buf,100,stdin);




  //分析下面代码
  //下面代码当我们正常输出时不会发生什么，但是当我们将他./myproc >log.txt后就会出现奇怪的现象
  //1.最先打印出
  //hello write!
  //hello printf!
  //hello fprintf
  //hello printf!
  //hello fprintf!
  //首先我们正常输出时是行缓冲，加了\n之后他会立即刷新缓冲区
  //而当我们重定向到log.txt之后，它的刷新方式就变成了全缓冲，即沾满缓冲区之后才会进行刷新
  //但是系统提供的write没有缓冲区，他会最先刷新出来，因此最先看到hello write!
  //之后printf ,fprintf打印得结果一直放在缓冲区里面，然后fork产生子进程发生写实拷贝，再次打印一遍printf,fprintf之后程序退出自动刷新缓冲区
  //因此发生上述现象
  //printf("hello printf!\n");
  //fprintf(stdout,"hello fprintf!\n");
  //char*str="hello write!\n";
  //write(1,str,strlen(str));
  //fork();
  


  //系统提供的重定向接口
  //int dup(int oldfd,int newfd)
  //dup(fd,1);
  //int fd=open("log.txt",O_WRONLY|O_CREAT,0666);
  //close(1);
  //dup2(fd,1);
  //printf("hello ,I am dup2\n");
  //fprintf(stdout,"hello I am fprintf\n");
  //close(fd);

  int fd=open("bite",O_RDONLY|O_CREAT,0666);
  if(fd<0)
  {
    perror("open!");
  }
  char buf[50];
  int ret=read(fd,buf,50);
  printf("%s",buf);
  


  return 0;
}
