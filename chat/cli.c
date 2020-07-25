#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
void f1(void* args)
{
  int fd=*((int*)args);
  char buf[1024];
  memset(buf,0x00,1024);
  while(fgets(buf,1024,stdin)!=NULL)
  {
    write(fd,buf,strlen(buf));
  }
}
void f2(void* args)
{
  int fd=*((int*)(args));
  char buf[1024]={};
  while(1)
  {
   memset(buf,0x00,1024);
   int r= read(fd,buf,1024);
   if(r==0) 
   {
     printf("sever close\n");
     break;
   }
   printf("%s",buf);
   fflush(stdout);
  }
  exit(0);
}
int main()
{
  int fd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in addr;
  addr.sin_family=AF_INET;
  addr.sin_port=htons(8888);
  inet_aton("106.54.70.78",&addr.sin_addr);
  connect(fd,(const struct sockaddr*)&addr,sizeof(addr));
  pthread_t pid1,pid2;
  int* p=(int*)malloc(sizeof(int));
  *p=fd;
  pthread_create(&pid1,NULL,f1,p);
  pthread_create(&pid2,NULL,f2,p);
  pthread_join(pid1,NULL);
  pthread_join(pid2,NULL);
  free(p);
}
