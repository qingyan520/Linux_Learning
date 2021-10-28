#include"com.h"
#include<string.h>
int main()
{
  if(mkfifo(FILE_NAME,0644)<0)
  {
    perror("mkfifo!\n");
    return 1;
  }
  int fd=open(FILE_NAME,O_RDONLY);
  if(fd<0)
  {
    perror("open fail!\n");
    return 2;
  }
  char buf[64];
  while(1)
  {
    ssize_t s=read(fd,buf,64);
    if(s>0)
    {
      buf[s]='\0';
      //创建flag标记+-*/%
      char lables[]="+-*/%";
      int flag=0;
      char*p=buf;
      while(*p)
      {
        switch(*p)
        {
          case '+':
            flag=0;
            break;
          case '-':
            flag=1;
            break;
          case '*':
            flag=2;
            break;
          case '/':
            flag=3;
            break;
          case '%':
            flag=4;
            break;
        }
        p++;
      }
      char*data1=strtok(buf,"+-*/%");
      char*data2=strtok(NULL,"+-*/%");
      int x=atoi(data1);
      int y=atoi(data2);
      int z=0;
      switch(flag)
      {
        case 0:
          z=x+y;
          break;
        case 1:
          z=x-y;
          break;
        case 2:
          z=x*y;
          break;
        case 3:
          z=x/y;
          break;
        case 4:
          z=x%y;
          break;
      }

      printf("%d %c %d = %d \n",x,lables[flag],y,z);


    }
  }
  return 0;
}
