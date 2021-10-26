#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<sys/shm.h>
#include<sys/ipc.h>

#define PATH_NAME  "/home/sjw/Linux_Learning/test_10_26_进程间通信/3.共享内存"

#define PROJ_ID  0666

#define SIZE 4096
