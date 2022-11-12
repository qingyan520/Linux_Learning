#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* thread(void*)
{
    int i;
    for (i=0;i<20;i++)
    {
       printf("I am a pthread.\n");
       sleep(1);
       /*
       if(i>=4)
       {
        pthread_exit(NULL);
       }
 	   */	   	
    }
    printf("子线程结束\n");
    return NULL;
}


int main(void)
{
    pthread_t tid;
    int ret;
  
    ret=pthread_create(&tid,NULL,thread,NULL);
    if (ret!=0)
    {
        printf ("Create thread error!\n");
        exit (1);
    }

	// 注释掉死循环代码
    // while(1)
   //  {
        printf("I am the main process. My PID=%d\n",getpid());
        sleep(1);
   //  }
    
    // 等待创建的线程结束，tid为创建的线程编号
    pthread_join(tid,NULL);
    printf("主线程结束\n");

    return(0);
}

