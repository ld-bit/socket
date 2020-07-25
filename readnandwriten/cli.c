#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include"public.h"
pthread_mutex_t mutex;
void hander(int s)
{
  printf("rcv %d\n",s);
}
void f1(void* args)
{
   int fd=*((int*)args);
   msg_t msg;
  while(fgets(msg.contex,1024,stdin)!=NULL)
   {
     msg.len=htonl(strlen(msg.contex));
     writen(fd,&msg.len,sizeof(int));
     writen(fd,msg.contex,strlen(msg.contex));
     memset(&msg,0x00,sizeof(msg));
   }
}
void f2(void* args)
{
  int fd=*((int*)args);
  msg_t msg;
  while(1)
  {
   memset(&msg,0x00,sizeof(msg));
   int r=readn(fd,&msg.len,sizeof(int));
   if(r==0)
   {
     printf("svr close\n");
     break;
   }
   int len=ntohl(msg.len);
   r=readn(fd,msg.contex,len);
   printf("len: %d\n",len);
   printf("message: %s\n",msg.contex);
  }
  exit(0);
}
int main()
{
   struct sigaction act;
   act.sa_flags=0;
   act.sa_handler=hander;
   sigemptyset(&act.sa_mask);
   sigaction(SIGPIPE,&act,NULL);
   int fd=socket(AF_INET,SOCK_STREAM,0) ;
   struct sockaddr_in addr;
   addr.sin_family=AF_INET;
   addr.sin_port=htons(8888);
   inet_aton("172.17.0.16",&addr.sin_addr);
   int r=connect(fd,(const struct sockaddr*)&addr,sizeof(addr));
   if(r==-1)
   {
     perror("connect: "),exit(-1);
   }
   else 
   {
     printf("link successfull\n");
   }
   pthread_t tid1,tid2; 
   int *p=(int*)malloc(sizeof(int));
   *p=fd;
   pthread_create(&tid1,NULL,f1,p);
   pthread_create(&tid2,NULL,f2,p);
   pthread_join(tid1,NULL);
   pthread_join(tid2,NULL);
   free(p);
}
