#include<stdio.h>
#include<string.h>
int main()
{
  FILE*fp=fopen("myfile","w");
  if(!fp)
  {
    printf("fopen error\n");
  }
  const char*msg="hello bit\n";
  int count=5;
  while(count--)
  {
    fwrite(msg,strlen(msg),1,fp);
  }
  fclose(fp);
  return 0;
}
