#include<stdio.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include"public.h"
int InitSocket()
{
  int lfd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in addr;
  addr.sin_family=AF_INET;
  addr.sin_port=htons(8888);
  addr.sin_addr.s_addr=0;
  bind(lfd,(const struct sockaddr*)&addr,sizeof(addr));
  listen(lfd,SOMAXCONN);
  return lfd;
}
int main()
{
  int lfd=InitSocket();
  int fd=accept(lfd,NULL,NULL);
  printf("some one link\n");
  msg_t msg;
  while(1)
  {
   memset(&msg,0x00,sizeof(msg));
   int r=(readn(fd,&msg.len,sizeof(int)));
   if(r==0)
   {
     printf("cli close \n");
     break;
   }
   int len=ntohl(msg.len);
   r=(readn(fd,msg.contex,len));
   if(r==0)
   {
     printf("cli close\n");
     break;
   }
   printf("len: %d\n",len);
   printf("mssage: %s\n",msg.contex);
   writen(fd,&msg.len,sizeof(int));
   writen(fd,msg.contex,len);
  }
}
