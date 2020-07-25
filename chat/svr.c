#include<stdio.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdlib.h>
typedef struct client
{
  int fd;
  char name[100];
}client_t;
typedef struct node
{
   struct node* next;
   struct node* prev;
   client_t cli;
}node_t;
node_t*  head=NULL;
void init_list()
{
  node_t* node=(node_t*)malloc(sizeof(node_t));
  node->next=node->prev=node;
  head=node;
}
void insert(client_t cli)
{
  node_t* p=(node_t*)malloc(sizeof(node_t));
  p->cli=cli;
  p->next=head->next;
  p->prev=head;
  head->next->prev=p;
  head->next=p;
}
void erase(int fd)
{
  node_t* p=head->next;
  while(p!=head)
  {
    if(p->cli.fd==fd)
    {
      p->prev->next=p->next;
      p->next->prev=p->prev;
      free(p);
      return;
    }
    p=p->next;
  }
}
int list_len()
{
  node_t* p=head->next;
  int i=0;
  while(p!=head)
  {
    ++i;
    p=p->next;
  }
  return i;
}
void send_all(char* arg)
{
  node_t* p=head->next;
  while(p!=head)
  {
    write(p->cli.fd,arg,strlen(arg));
    p=p->next;
  }
}
int tcp_init()
{
  int fd=socket(AF_INET,SOCK_STREAM,0);
  struct sockaddr_in addr;
  addr.sin_family=AF_INET;
  addr.sin_port=htons(8888);
  addr.sin_addr.s_addr=0;
  bind(fd,(const struct sockaddr*)&addr,sizeof(addr));
  listen(fd,SOMAXCONN);
  return fd;
}
void process(void* arg)
{
  int fd=*((int*)arg);
  free(arg);
  char buf[1024]={};

  sprintf(buf,"%s","欢迎来到聊天室\n");
  write(fd,buf,strlen(buf));

  memset(buf,0x00,1024);
  sprintf(buf,"%s","昵称: ");
  write(fd,buf,strlen(buf));


  char nincname[100]={};
  read(fd,nincname,100);
  nincname[strlen(nincname)-1]='\0';

  memset(buf,0x00,1024);
  sprintf(buf,"%s 上线了\n",nincname);
  send_all(buf);


  client_t cli;
  strcpy(cli.name,nincname);
  cli.fd=fd;
  insert(cli);
 
  while(1)
  {
    memset(buf,0x00,1024);
   char tmp[1024]={};
   sprintf(tmp,"%s ::",cli.name);
    int r=read(fd,buf,1024);
    if(r==0)
    {
      sprintf(buf,"%s 下线了\n",cli.name);
      send_all(buf);
      break;
    }
    else 
    {
      strcat(tmp,buf);
      send_all(tmp);
    }
  }
  erase(cli.fd);
  close(fd);
}
int main()
{
  init_list();
  int lfd=tcp_init();
  while(1)
  {
    int fd=accept(lfd,NULL,NULL);
    pthread_t tid;
    int* p=(int*)malloc(sizeof(int));
    *p=fd;
    pthread_create(&tid,NULL,process,p);
    pthread_detach(tid);
  }
  return 0;
}
