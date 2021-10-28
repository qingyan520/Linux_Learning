#include"com.h"

int main()
{
  key_t key=ftok(PATH_NAME,PROJ_ID);
  int shm=shmget(key,SIZE,IPC_CREAT|IPC_EXCL);
  if(shm<0)
  {
    printf("shm fail!\n");
    return 1;
  }
  printf("%d\n",shm);
  sleep(10);
  return 0;
}
